#pragma once

#include <vector>
#include "CProcessViewer.h"
#include "CClientSocket.h"

// CProcessesViewer 대화 상자

class CProcessesViewer : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessesViewer)

public:
	CProcessesViewer(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CProcessesViewer();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROCESSES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_processSection;
	std::vector<CProcessViewer*> m_processList;
	CString IP;
	CString internal_IP;

	afx_msg void OnSelchangeTabProcesses(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	BOOL UpdateMSG(DWORD pid);
	afx_msg void OnClose();
};
