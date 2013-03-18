// AutoUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include <shlobj.h>
#include "AutoUpdate.h"
#include "InternetGetFile.h"
#include "UpdateThread.h"
#include "UISkinManager.h"
#include "AutoUpdateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoUpdateDlg dialog

CAutoUpdateDlg::CAutoUpdateDlg(CWnd* pParent /*=NULL*/)
	: CRDialog(CAutoUpdateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoUpdateDlg)
	m_sPrompt = _T("");
	m_sPromptTotalProgress = _T("");
	m_sProgressBar = _T("");
	m_sTotalProgressBar = _T("");
	m_TotalFileInfo.iFileCount = 0;
	m_TotalFileInfo.iFileSize = 0;
	m_FinishedFileInfo.iFileCount = 0;
	m_FinishedFileInfo.iFileSize = 0;
	m_fDownloadPercent = 0.0;
	m_bSilenceMode = FALSE;
	m_bUserBreak = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pUpdateThread = NULL;
}

void CAutoUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoUpdateDlg)
	DDX_Control(pDX, IDOK, m_btnUpgrade);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_STATIC_PROMPT, m_sPrompt);
	DDX_Text(pDX, IDC_STATIC_PROMPT_TOTAL_PROGRESS, m_sPromptTotalProgress);
	DDX_Text(pDX, IDC_STATIC_PROGRESS, m_sProgressBar);
	DDX_Text(pDX, IDC_STATIC_TOTAL_PROGRESS, m_sTotalProgressBar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAutoUpdateDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoUpdateDlg)
	RDIALOG_UI_MESSAGE_MAP
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER, OnUserMessage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoUpdateDlg message handlers

BOOL CAutoUpdateDlg::OnInitDialog()
{
	CRDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetWindowText(m_sAppName + " " + STRING(STR_AUTO_UPDATE, "Auto Update"));
	m_btnUpgrade.SetWindowText(STRING(STR_BUTTON_START_UPGRADE, "开始升级"));
	m_btnCancel.SetWindowText(STRING(STR_BUTTON_CANCEL_UPGRADE, "取消升级"));
	m_sPrompt = STRING(STR_PROMPT_UPGRADE_READY, "Upgrade is ready. Please press \"Start upgrade\" button to upgrade.");
	UpdateData(FALSE);
	if (m_bSilenceMode)
	{
		ShowWindow(SW_HIDE);
		OnOK();
	}

	if (CUISkinManager::m_iCurrentStyle == CUISkinManager::StyleNormal)
	{
		// 以字符方式显示进度
		GetDlgItem(IDC_STATIC_PROGRESS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOTAL_PROGRESS)->ShowWindow(SW_SHOW);
	}
	else
	{
		// 以图形图显示进度
		GetDlgItem(IDC_STATIC_PROGRESS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TOTAL_PROGRESS)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAutoUpdateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutoUpdateDlg::OnPaint() 
{
	CRDialog::OnPaint();

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	// 重绘进度条
	if (m_fTotalDownloadPercent != 0.0)
	{
		UpdateProgress(m_TotalFileInfo.iFileCount, m_FinishedFileInfo.iFileCount
			, m_fTotalDownloadPercent, m_fDownloadPercent);
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAutoUpdateDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAutoUpdateDlg::OnCancel() 
{
	m_bUserBreak = TRUE;
	if (m_pUpdateThread != NULL)
	{
		LOG(0, 0, "User cancel upgrade.");
		HANDLE hThread = m_pUpdateThread->m_hThread;
		m_pUpdateThread->m_bUserBreak = TRUE;
		Internet::bInternetGetURLUserBreak = TRUE;
		WaitForSingleObject(hThread, 1000);
	}

	CDialog::OnCancel();
}

void CAutoUpdateDlg::OnOK() 
{
	// 点击开始升级按钮，执行升级
	if (!SetConfigFile(GetAppDirectory() + UPDATE_CONFIG_FILENAME))
	{
		RMessageBox(STRING(STR_PROMPT_FAIL_TO_OPEN_UPDATE_CONFIG_FILE, "Fail to open update config file."));
	}
	else
	{
		// 开始升级，禁用按钮，避免重复点击
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		DoUpdate();
	}

//	CDialog::OnOK();
}

// 设置升级配置文件名
BOOL CAutoUpdateDlg::SetConfigFile(CString &sFilename)
{
	CFileFind FileFinder;

	if (FileFinder.FindFile(sFilename))
	{
		FileFinder.FindNextFile();
		if (!FileFinder.IsDirectory())
		{
			m_sConfigFilename = sFilename;
			FileFinder.Close();
			return TRUE;
		}
		FileFinder.Close();
	}

	return FALSE;
}

BOOL CAutoUpdateDlg::DoUpdate(void)
{
	LOG(0, 0, "Start upgrade thread.");

	// 启动升级线程
	m_pUpdateThread = new CUpdateThread;
	m_pUpdateThread->m_bAutoDelete = TRUE;
	m_pUpdateThread->m_sConfigFilename = m_sConfigFilename;
	m_pUpdateThread->m_hProgressWindow = m_hWnd;
	m_pUpdateThread->CreateThread();

	return TRUE;
}

// 处理用户消息
LRESULT CAutoUpdateDlg::OnUserMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case NOTIFY_DOWNLOAD_INFO:			// 通知要下载的文件状况
		m_TotalFileInfo = *(DOWNLOAD_INFO_STRU*)lParam;
		m_fDownloadPercent = 0;
		m_fTotalDownloadPercent = 0;
		UpdateProgress(m_TotalFileInfo.iFileCount, m_FinishedFileInfo.iFileCount
			, m_fTotalDownloadPercent, m_fDownloadPercent);
		break;
	case NOTIFY_DOWNLOADED_INFO:		// 通知已下载的文件状况
		m_FinishedFileInfo = *(DOWNLOAD_INFO_STRU*)lParam;
		m_fDownloadPercent = 0;
		m_fTotalDownloadPercent = float(m_FinishedFileInfo.iFileSize) / m_TotalFileInfo.iFileSize;
		UpdateProgress(m_TotalFileInfo.iFileCount, m_FinishedFileInfo.iFileCount
			, m_fTotalDownloadPercent, m_fDownloadPercent);
		break;
	case NOTIFY_DOWNLOAD_PROGRESS:		// 通知下载单个文件进度
		{
			DOWNLOAD_PROGRESS_STRU* pDownloadProgress = (DOWNLOAD_PROGRESS_STRU*)lParam;
			m_fDownloadPercent = float(pDownloadProgress->iCurrentFileFinishedSize) / pDownloadProgress->iCurrentFileSize;
			m_fTotalDownloadPercent = float(m_FinishedFileInfo.iFileSize + pDownloadProgress->iCurrentFileFinishedSize) / m_TotalFileInfo.iFileSize;
			UpdateProgress(m_TotalFileInfo.iFileCount, m_FinishedFileInfo.iFileCount
				, m_fTotalDownloadPercent, m_fDownloadPercent);
		}
		break;
	case NOTIFY_DOWNLOADING_FILENAME:	// 通知正在下载的文件名
		if (lParam != 0)
		{
			m_sPrompt.Format(STRING(STR_PROMPT_DOWNLOADING_FILE, "Downloading file %s."), (char*)lParam);
		}
		else
		{
			m_sPrompt = "Downloading file :";
		}
		m_sProgressBar = "";
		UpdateData(FALSE);
		break;
	case NOTIFY_DOWNLOAD_FILE_FAIL:		// 通知下载文件失败
		m_sPrompt.Format(STRING(STR_PROMPT_FAIL_IN_DOWNLOADING_FILES, "下载文件 %s 失败！"), (char*)lParam);
		m_sProgressBar = "";
		UpdateData(FALSE);
		break;
	case NOTIFY_VERIFY_FILE_FAIL:		// 通知校验文件失败
		m_sPrompt.Format(STRING(STR_PROMPT_FAIL_IN_VERIFYING_FILES, "校验文件 %s 失败！"), (char*)lParam);
		m_sProgressBar = "";
		UpdateData(FALSE);
		break;
	case NOTIFY_UPDATE_FILE_FAIL:		// 通知更新文件失败
		m_sPrompt.Format(STRING(STR_PROMPT_FAIL_IN_UPDATING_FILES, "更新文件 %s 失败！"), (char*)lParam);
		m_sProgressBar = "";
		UpdateData(FALSE);
		break;
	case NOTIFY_FINISH_UPDATE:			// 通知升级完毕消息
		OnNotifyUpdateFinish((BOOL)lParam);
		break;
	default:
		break;
	}

	return TRUE;
}

// 更新显示下载文件进度
void CAutoUpdateDlg::UpdateProgress(UINT iTotalFileCount, UINT iFinishedFileCount, float fTotalPercent, float fPercent)
{
	if (m_hWnd == NULL || m_bSilenceMode || iTotalFileCount == 0)
	{
		return;
	}

	// 绘制进度条

	if (CUISkinManager::m_iCurrentStyle == CUISkinManager::StyleNormal)
	{
		const int BUFFER_SIZE = 500;
		char acBuffer[BUFFER_SIZE] = {0};

		fPercent *= 100;
		if (fPercent < 0)
		{
			fPercent = 0;
		}
		if (fPercent > 100)
		{
			fPercent = 100;
		}
		if (fPercent < 44)
		{
			strncpy(acBuffer, ">>>>>>>>>>>>>>>>>>>>>> 100% >>>>>>>>>>>>>>>>>>>>", int(fPercent / 2));
			strncat(acBuffer, "                                                  ", 22 - int(fPercent / 2));
			sprintf(acBuffer, "%s %2.1f%% ", acBuffer, fPercent);
		}
		else
		{
			strncpy(acBuffer, ">>>>>>>>>>>>>>>>>>>>>> 100% >>>>>>>>>>>>>>>>>>>>", 22);
			sprintf(acBuffer, "%s %2.1f%% ", acBuffer, fPercent);
			if (fPercent > 56)
			{
				strncat(acBuffer, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", int(fPercent / 2) - 28);
			}
		}
		m_sProgressBar = acBuffer;

		memset(acBuffer, 0, BUFFER_SIZE);
		fTotalPercent *= 100;
		if (fTotalPercent < 0)
		{
			fTotalPercent = 0;
		}
		if (fTotalPercent > 100)
		{
			fTotalPercent = 100;
		}
		if (fTotalPercent < 44)
		{
			strncpy(acBuffer, ">>>>>>>>>>>>>>>>>>>>>> 100% >>>>>>>>>>>>>>>>>>>>", int(fTotalPercent / 2));
			strncat(acBuffer, "                                                  ", 22 - int(fTotalPercent / 2));
			sprintf(acBuffer, "%s %2.1f%% ", acBuffer, fTotalPercent);
		}
		else
		{
			strncpy(acBuffer, ">>>>>>>>>>>>>>>>>>>>>> 100% >>>>>>>>>>>>>>>>>>>>", 22);
			sprintf(acBuffer, "%s %2.1f%% ", acBuffer, fTotalPercent);
			if (fTotalPercent > 56)
			{
				strncat(acBuffer, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", int(fTotalPercent / 2) - 28);
			}
		}
 		m_sTotalProgressBar = acBuffer;

		m_sPromptTotalProgress.Format(STRING(STR_TOTAL_UPGRADE_PROGRESS, "升级总进度 %d / %d")
			, iFinishedFileCount >= iTotalFileCount ? iTotalFileCount : iFinishedFileCount + 1
			, iTotalFileCount);

		UpdateData(FALSE);

		return;
	}
	else
	{
		CDC *pDC = GetDC();
		CRect rcWnd;
		CRect rc, rc2;

		GetClientRect(&rcWnd);

		// 画进度条外框
		rc = rcWnd;
		rc.DeflateRect(30, 75, 30, 75);
		rc.bottom = rc.top + 18;
		pDC->Draw3dRect(&rc, RGB(200, 210, 220), RGB(30, 70, 100));
		rc.DeflateRect(1, 1, 1, 1);
		pDC->Draw3dRect(&rc, RGB(140, 160, 180), RGB(90, 120, 180));

		// 填充进度条背景色
		rc.DeflateRect(2, 2, 2, 2);
		pDC->FillSolidRect(&rc, RGB(239, 247, 255));

		// 画进度条
		int iTotalWidth = rc.Width();
		int iFinishedWidth = int(iTotalWidth * fPercent);
		rc.right = rc.left + iFinishedWidth;
		rc2 = rc;
		rc.DeflateRect(0, 0, 0, rc.Height() / 2 - 1);
		rc2.DeflateRect(0, rc2.Height() / 2 - 1, 0, 0);
		CUISkinManager::GradientFillRect(this, pDC, rc, GRADIENT_FILL_RECT_V, RGB(193, 231, 255), RGB(140, 186, 247));
		CUISkinManager::GradientFillRect(this, pDC, rc2, GRADIENT_FILL_RECT_V, RGB(140, 186, 247), RGB(173, 211, 247));

		m_sPromptTotalProgress.Format(STRING(STR_TOTAL_UPGRADE_PROGRESS, "升级总进度 %d / %d")
			, iFinishedFileCount >= iTotalFileCount ? iTotalFileCount : iFinishedFileCount + 1
			, iTotalFileCount);
		UpdateData(FALSE);

		// 画总进度条外框
		rc = rcWnd;
		rc.DeflateRect(30, 130, 30, 0);
		rc.bottom = rc.top + 18;
		pDC->Draw3dRect(&rc, RGB(200, 210, 220), RGB(30, 70, 100));
		rc.DeflateRect(1, 1, 1, 1);
		pDC->Draw3dRect(&rc, RGB(140, 160, 180), RGB(90, 120, 180));

		// 填充总进度条背景色
		rc.DeflateRect(2, 2, 2, 2);
		pDC->FillSolidRect(&rc, RGB(239, 247, 255));

		// 画总进度条
		iFinishedWidth = int(iTotalWidth * fTotalPercent);
		rc.right = rc.left + iFinishedWidth;
		rc2 = rc;
		rc.DeflateRect(0, 0, 0, rc.Height() / 2 - 1);
		rc2.DeflateRect(0, rc2.Height() / 2 - 1, 0, 0);
		CUISkinManager::GradientFillRect(this, pDC, rc, GRADIENT_FILL_RECT_V, RGB(193, 231, 255), RGB(140, 186, 247));
		CUISkinManager::GradientFillRect(this, pDC, rc2, GRADIENT_FILL_RECT_V, RGB(140, 186, 247), RGB(173, 211, 247));

		ReleaseDC(pDC);
	}
}

// 升级程序执行完毕
void CAutoUpdateDlg::OnNotifyUpdateFinish(BOOL bSuccess)
{
	m_pUpdateThread = NULL;

	if (m_bSilenceMode)
	{
		CDialog::OnOK();
	}
	else if (bSuccess)
	{
		m_sPrompt = STRING(STR_PROMPT_UPGRADE_FINISHED, "升级完毕！");
		m_btnCancel.SetWindowText(STRING(STR_BUTTON_SUCCESS_UPGRADE, "完成升级"));
	}
	else
	{
		m_sPromptTotalProgress = "";
		m_sTotalProgressBar = "";
	}

	m_sProgressBar = "";
	UpdateData(FALSE);
	GetDlgItem(IDOK)->EnableWindow();
}
