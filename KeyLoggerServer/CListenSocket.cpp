#include "pch.h"
#include "CListenSocket.h"
#include "KeyLoggerServerDlg.h"

CListenSocket::CListenSocket(CKeyLoggerServerDlg* dlg)
{
	m_mainDlg = dlg;
}
void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_mainDlg->ClientAccept();
	CSocket::OnAccept(nErrorCode);
}
