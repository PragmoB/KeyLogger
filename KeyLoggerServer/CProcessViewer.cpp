// CProcessViewer.cpp: 구현 파일
//

#include "pch.h"
#include "KeyLoggerServer.h"
#include "CProcessViewer.h"
#include "afxdialogex.h"


// CProcessViewer 대화 상자

IMPLEMENT_DYNAMIC(CProcessViewer, CDialogEx)

CProcessViewer::CProcessViewer(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PROCESS, pParent)
{

}

CProcessViewer::~CProcessViewer()
{
}

void CProcessViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MESSAGE, m_msgWnd);
}


BEGIN_MESSAGE_MAP(CProcessViewer, CDialogEx)
END_MESSAGE_MAP()


// CProcessViewer 메시지 처리기


BOOL CProcessViewer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_msgFocus = 0;
	this->UpdateMSG();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


int CProcessViewer::UpdateMSG()
{
	// TODO: 여기에 구현 코드 추가.
	/* 저장된 키보드 입력을 ListBox에 가공해서 출력 */

	WCHAR buff[51] = L"";
	int buffer_index = 0;

	for (int i = m_msgFocus + 1; i < m_process->m_MSGList.size(); i++)
	{
		switch (m_process->m_MSGList[i])
		{
		case VK_RETURN:
			buff[buffer_index] = 0;
			m_msgWnd.InsertString(-1, buff);
			buffer_index = 0;
			m_msgFocus = i;
			continue;

		default:
			if (buffer_index >= 49) // 엔터를 안누르고 50자를 꽉 채운경우
			{
				// 시각적 편의를 위해 여기서 끊음
				buff[buffer_index++] = m_process->m_MSGList[i];
				buff[buffer_index] = 0;
				m_msgWnd.InsertString(-1, buff);
				buffer_index = 0;
				m_msgFocus = i;
				continue;
			}
			buff[buffer_index] = m_process->m_MSGList[i];
			break;
		}
		buffer_index++;
	}
	return 0;
}
