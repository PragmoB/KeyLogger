
// KeyLoggerServerDlg.h: 헤더 파일
//

#pragma once

#include <vector>

#include "CClientSocket.h"
#include "CListenSocket.h"
#include "CProcessesViewer.h"
#include "mydef.h"

// CKeyLoggerServerDlg 대화 상자
class CKeyLoggerServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CKeyLoggerServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KEYLOGGERSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	CListCtrl m_clientListControl;
	CListenSocket* m_pListenSocket;
	std::vector<CProcessesViewer*> m_userList;
	void ClientAccept();
	int ReceiveMSG(LPMSG32 msg, CClientSocket* process, DWORD pid);
	int ReceiveMSG(LPMSG64 msg, CClientSocket* process, DWORD pid);
	afx_msg void OnDblclkListClient(NMHDR *pNMHDR, LRESULT *pResult);
	void UpdateListCtrl(int index, CClientSocket* process);
};
