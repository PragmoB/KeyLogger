#pragma once

#include "CClientSocket.h"

// CProcessViewer 대화 상자

class CProcessViewer : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessViewer)

public:
	CProcessViewer(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CProcessViewer();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_msgWnd;
	CClientSocket* m_process;
	int m_msgFocus; // 메시지 실시간 업데이트를 위해 필요, 현재까지 출력한 메시지 인덱스 저장
	virtual BOOL OnInitDialog();
	int UpdateMSG();
};
