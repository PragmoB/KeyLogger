// CProcessesViewer.cpp: 구현 파일
//

#include "pch.h"
#include "KeyLoggerServer.h"
#include "CProcessesViewer.h"
#include "afxdialogex.h"

// CProcessesViewer 대화 상자

IMPLEMENT_DYNAMIC(CProcessesViewer, CDialogEx)

CProcessesViewer::CProcessesViewer(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PROCESSES, pParent)
{

}

CProcessesViewer::~CProcessesViewer()
{
}

void CProcessesViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PROCESSES, m_processSection);
}


BEGIN_MESSAGE_MAP(CProcessesViewer, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PROCESSES, &CProcessesViewer::OnSelchangeTabProcesses)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CProcessesViewer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int ret;
	/* 탭 생성 */
	for (int i = 0; i < m_processList.size(); i++)
	{
		CString str = m_processList[i]->m_process->m_processName;

		if (!m_processList[i]->m_process->m_status) // 종료된 상태면
			str += "(exit)"; // 종료됐다고 표시

		m_processSection.InsertItem(i, str);
		ret = m_processList[i]->Create(IDD_DIALOG_PROCESS, &m_processSection);

		// 탭 위치, 크기, 보이기 등 설정
		CRect size;
		m_processSection.GetWindowRect(&size);
		m_processList[i]->MoveWindow(0, 20, size.Width(), size.Height());
		ret = m_processList[i]->ShowWindow(SW_HIDE);
	}
	ret = m_processList[0]->ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


// CProcessesViewer 메시지 처리기


void CProcessesViewer::OnSelchangeTabProcesses(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < m_processSection.GetItemCount(); i++)
		m_processList[i]->ShowWindow(SW_HIDE);

	m_processList[m_processSection.GetCurSel()]->ShowWindow(SW_SHOW);
	*pResult = 0;
}

BOOL CProcessesViewer::UpdateMSG(DWORD pid)
{
	// TODO: 여기에 구현 코드 추가.

	int i;
	for (i = 0; i < m_processList.size(); i++)
	{
		// 일치하는 프로세스를 검색하며
		if (m_processList[i]->m_process->m_pid == pid)
		{
			// 탭이 생성 안돼있으면
			if (!m_processList[i]->m_hWnd)
			{ 
				// 생성하고
				m_processSection.InsertItem(i, m_processList[i]->m_process->m_processName);
				m_processList[i]->Create(IDD_DIALOG_PROCESS, &m_processSection);

				// 탭 위치, 크기, 보이기 등 설정
				CRect size;
				m_processSection.GetWindowRect(&size);
				m_processList[i]->MoveWindow(0, 20, size.Width(), size.Height());
				m_processList[i]->ShowWindow(SW_HIDE);

				m_processList[m_processSection.GetCurSel()]->ShowWindow(SW_HIDE);
				m_processList[m_processSection.GetCurSel()]->ShowWindow(SW_SHOW);
			}
			m_processList[i]->UpdateMSG(); // 그 프로세스에 해당해서만 업뎃

			if (!m_processList[i]->m_process->m_status) // 프로세스가 종료된 상태면
			{
				TCITEM tcItem;

				tcItem.mask = TCIF_TEXT;

				CString str = m_processList[i]->m_process->m_processName;
				tcItem.pszText = str + "(exit)";  // 탭 컨트롤에 종료 상태 표시
				m_processSection.SetItem(i, &tcItem);

				m_processList[m_processSection.GetCurSel()]->ShowWindow(SW_HIDE);
				m_processList[m_processSection.GetCurSel()]->ShowWindow(SW_SHOW);
			}
			break;
		}
	}
	if (i == m_processList.size()) // 맞는 프로세스가 없을 경우
	{

	}
	return TRUE;
}

void CProcessesViewer::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	DestroyWindow();
	CDialogEx::OnClose();
}
