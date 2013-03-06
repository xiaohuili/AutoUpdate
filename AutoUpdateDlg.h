// AutoUpdateDlg.h : header file
//

#if !defined(AFX_AUTOUPDATEDLG_H__4CA1BB40_75FF_4A00_AF96_F141507CE885__INCLUDED_)
#define AFX_AUTOUPDATEDLG_H__4CA1BB40_75FF_4A00_AF96_F141507CE885__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAutoUpdateDlg dialog

#include "RDialog.h"

class CUpdateThread;

class CAutoUpdateDlg : public CRDialog
{
// Construction
public:
	CAutoUpdateDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL SetConfigFile(CString& sFilename);	// 设置配置文件名
	BOOL DoUpdate(void);
	LRESULT OnUserMessage(WPARAM wParam, LPARAM lParam);
	void UpdateProgress(UINT iTotalFileCount, UINT iFinishedFileCount, float fTotalPercent, float fPercent);
	void OnNotifyUpdateFinish(BOOL bSuccess = TRUE);

// Dialog Data
	//{{AFX_DATA(CAutoUpdateDlg)
	enum { IDD = IDD_AUTOUPDATE };
	CRButton	m_btnUpgrade;
	CRButton	m_btnCancel;
	CString m_sAppName;					// 应用程序名
	CString	m_sPrompt;					// 下载文件进度提示信息
	CString	m_sPromptTotalProgress;		// 升级总进度提示信息
	CString m_sStatus;
	CString	m_sProgressBar;				// 下载文件进度条
	CString	m_sTotalProgressBar;		// 升级总进度条
	BOOL m_bSilenceMode;				// 静默方式执行升级，不显示升级程序界面，只在升级完毕后提醒用户
	BOOL m_bUserBreak;					// 用户终止升级
	CUpdateThread *m_pUpdateThread;		// 执行升级的线程
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoUpdateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_sConfigFilename;			// 升级配置文件名
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAutoUpdateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	DOWNLOAD_INFO_STRU m_TotalFileInfo;	// 要下载的文件总数和大小总和
	DOWNLOAD_INFO_STRU m_FinishedFileInfo;	// 已下载的文件总数和大小总和
	float m_fDownloadPercent;			// 当前正在下载的文件进度
	float m_fTotalDownloadPercent;		// 所有要下载的文件总进度
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOUPDATEDLG_H__4CA1BB40_75FF_4A00_AF96_F141507CE885__INCLUDED_)
