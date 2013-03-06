//------------------------------------------------------------------------------
// 文件名称：UpdateThread.h
// 文件版本：v1.0
// 创建日期：2006-05-04 14:25
// 作    者：Richard Ree
// 文件描述：自动升级线程类
//------------------------------------------------------------------------------

#if !defined(AFX_UPDATETHREAD_H__194A514F_A0D7_4ADD_8D2A_9E7081810D82__INCLUDED_)
#define AFX_UPDATETHREAD_H__194A514F_A0D7_4ADD_8D2A_9E7081810D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// UpdateThread.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpdateThread thread

class CUpdateThread : public CWinThread
{
	DECLARE_DYNCREATE(CUpdateThread)
protected:

// Attributes
public:
	CString m_sConfigFilename;			// 升级配置文件名
	BOOL m_bSilenceMode;				// 静默方式执行升级，不显示升级程序界面，只在升级完毕后提醒用户
	BOOL m_bUserBreak;					// 用户终止升级
	double m_fPercent;					// 下载文件进度百分比
	HWND m_hProgressWindow;				// 显示升级进度的窗口句柄

// Operations
public:
	CUpdateThread();					// protected constructor used by dynamic creation
	BOOL DoUpdate();					// 执行升级

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpdateThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CUpdateThread();

	// Generated message map functions
	//{{AFX_MSG(CUpdateThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	BOOL DownloadFile(CString& sFileSection);	// 下载文件
	BOOL VerifyFile(CString &sFileSection);		// 校验文件
	BOOL UpdateFile(CString &sFileSection);		// 更新文件
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDATETHREAD_H__194A514F_A0D7_4ADD_8D2A_9E7081810D82__INCLUDED_)
