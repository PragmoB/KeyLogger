#include "pch.h"

#include <windows.h>
#include <TlHelp32.h>

#include "mydef.h"
#include "UsableAPI.h"

WCHAR* ConvertCtoWC(const char* str)
{
	wchar_t* p;
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);

	p = new WCHAR[strSize];

	MultiByteToWideChar(CP_ACP, 0, str, strlen(str) + 1, p, strSize);
	return p;
}

ARCH_UINT FindProcessName(DWORD pid, WCHAR* out)
{
	DWORD dwPID = -1;
	HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe;

	pe.dwSize = sizeof(PROCESSENTRY32);
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	Process32First(hSnapShot, &pe);
	do
	{
		if (pe.th32ProcessID == pid)
		{
			wcscpy(out, pe.szExeFile);
			break;
		}
	} while (Process32Next(hSnapShot, &pe));

	CloseHandle(hSnapShot);
	return wcslen(out);
}