// UpdateThread.cpp : implementation file
//

#include "stdafx.h"
#include "AutoUpdate.h"
#include "InternetGetFile.h"
#include "UpdateThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpdateThread

IMPLEMENT_DYNCREATE(CUpdateThread, CWinThread)

CUpdateThread::CUpdateThread()
{
	m_bSilenceMode = FALSE;
	m_bUserBreak = FALSE;
	m_fPercent = 0;
	m_hProgressWindow = NULL;
}

CUpdateThread::~CUpdateThread()
{
}

BOOL CUpdateThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CUpdateThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	m_bUserBreak = TRUE;

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUpdateThread, CWinThread)
	//{{AFX_MSG_MAP(CUpdateThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpdateThread message handlers

int CUpdateThread::Run() 
{
	if (DoUpdate())
	{
		if (m_hProgressWindow)
		{
			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_FINISH_UPDATE, (LPARAM)1);
		}
		return 1;
	}
	else
	{
		if (m_hProgressWindow)
		{
			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_FINISH_UPDATE, (LPARAM)0);
		}
		return 0;
	}

//	return CWinThread::Run();
}

// 执行升级
BOOL CUpdateThread::DoUpdate()
{
	if (m_sConfigFilename.IsEmpty())
	{
		return FALSE;
	}

	UINT iCommonFileCount = 0;
	UINT iLanguageFileCount = 0;
	DOWNLOAD_INFO_STRU DownloadInfo = {0};
	UINT i;
	const int BUFFER_SIZE = 512;
	char acBuffer[BUFFER_SIZE];
	CString sSection;
	CString sLanguageSection;
	CString sKey;
	CString sFilename;
	CString sHash;
	CString sURL;
	CString sTemp;

	// 创建保存升级文件的目录
	CreateDirectory(GetAppDirectory() + "Update", NULL);

	// 取得公共文件数及文件大小总和
	iCommonFileCount = GetPrivateProfileInt(SECTION_COMMON, "FileCount", 0,
		m_sConfigFilename.GetBuffer(0));
	for (i = 1; i <= iCommonFileCount; i++)
	{
		sSection.Format("CommonFile%d", i);
		DownloadInfo.iFileSize += GetPrivateProfileInt(sSection.GetBuffer(0), "Size", 0
			, m_sConfigFilename.GetBuffer(0));
	}
	DownloadInfo.iFileCount += iCommonFileCount;

	// 取得与操作系统代码页相关的文件数及文件大小总和
	sKey.Format("%d", GetSystemDefaultLangID());
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(SECTION_LANGUAGE, sKey.GetBuffer(0)
		, "", (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
	sLanguageSection = (char*)acBuffer;

	if (sLanguageSection.IsEmpty())
	{
		// 没有与操作系统代码页相关的语言，读取缺省的语言
		sKey = "Default";
		memset(acBuffer, 0, BUFFER_SIZE);
		GetPrivateProfileString(SECTION_LANGUAGE, sKey.GetBuffer(0), ""
			, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
		sLanguageSection = (char*)acBuffer;
	}

	if (!sLanguageSection.IsEmpty())
	{
		iLanguageFileCount = GetPrivateProfileInt(sLanguageSection.GetBuffer(0), "FileCount", 0
			, m_sConfigFilename.GetBuffer(0));
		for (i = 1; i <= iLanguageFileCount; i++)
		{
			sSection.Format("%sFile%d", sLanguageSection.GetBuffer(0), i);
			DownloadInfo.iFileSize += GetPrivateProfileInt(sSection.GetBuffer(0), "Size", 0
				, m_sConfigFilename.GetBuffer(0));
		}
		DownloadInfo.iFileCount += iLanguageFileCount;
	}

	// 将文件总数和文件大小总和上报到界面
	if (!m_bSilenceMode && m_hProgressWindow != NULL)
	{
		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOAD_INFO, (LPARAM)&DownloadInfo);
	}

	LOG(0, 0, "Total files to download is %u. Total file size is %u.", DownloadInfo.iFileCount, DownloadInfo.iFileSize);

	// 下载所有文件

	memset(&DownloadInfo, 0, sizeof(DOWNLOAD_INFO_STRU));

	// 循环下载所有公共文件
	for (i = 1; i <= iCommonFileCount; i++)
	{
		if (m_bUserBreak)
		{
			return FALSE;
		}

		sSection.Format("CommonFile%d", i);
		if (!DownloadFile(sSection))
		{
			// 文件下载失败，升级失败
			if (!m_bSilenceMode && m_hProgressWindow != NULL)
			{
				sKey = "Name";
				memset(acBuffer, 0, BUFFER_SIZE);
				GetPrivateProfileString(sSection.GetBuffer(0), sKey.GetBuffer(0), ""
					, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
				SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOAD_FILE_FAIL, (LPARAM)acBuffer);
			}
			return FALSE;
		}

		// 将升级进度上报到界面
		DownloadInfo.iFileCount++;
		DownloadInfo.iFileSize += GetPrivateProfileInt(sSection.GetBuffer(0), "Size", 0
			, m_sConfigFilename.GetBuffer(0));
		if (!m_bSilenceMode && m_hProgressWindow != NULL)
		{
			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOADED_INFO, (LPARAM)&DownloadInfo);
		}
		LOG(0, 0, "CUpdateThread::DoUpdate : Finished files %u (%u bytes).", DownloadInfo.iFileCount, DownloadInfo.iFileSize);
	}
	// 根据操作系统代码页下载所有相应语言的文件
	if (!sLanguageSection.IsEmpty())
	{
		for (i = 1; i <= iLanguageFileCount; i++)
		{
			if (m_bUserBreak)
			{
				return FALSE;
			}

			sSection.Format("%sFile%d", sLanguageSection.GetBuffer(0), i);
			if (!DownloadFile(sSection))
			{
				// 文件下载失败，升级失败
				if (!m_bSilenceMode && m_hProgressWindow != NULL)
				{
					sKey = "Name";
					memset(acBuffer, 0, BUFFER_SIZE);
					GetPrivateProfileString(sSection.GetBuffer(0), sKey.GetBuffer(0), ""
						, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
					SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOAD_FILE_FAIL, (LPARAM)acBuffer);
				}
				return FALSE;
			}

			// 将升级进度上报到界面
			DownloadInfo.iFileCount++;
			DownloadInfo.iFileSize += GetPrivateProfileInt(sSection.GetBuffer(0), "Size", 0
				, m_sConfigFilename.GetBuffer(0));
			if (!m_bSilenceMode && m_hProgressWindow != NULL)
			{
				SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOADED_INFO, (LPARAM)&DownloadInfo);
			}
			LOG(0, 0, "CUpdateThread::DoUpdate : Finished files %u (%u bytes).", DownloadInfo.iFileCount, DownloadInfo.iFileSize);
		}
	}

	// 下载完毕后校验文件

	// 循环校验所有公共文件
	for (i = 1; i <= iCommonFileCount; i++)
	{
		if (m_bUserBreak)
		{
			return FALSE;
		}

		sSection.Format("CommonFile%d", i);
		if (!VerifyFile(sSection))
		{
			// 文件校验不通过，升级失败
			if (!m_bSilenceMode && m_hProgressWindow != NULL)
			{
				sKey = "Name";
				memset(acBuffer, 0, BUFFER_SIZE);
				GetPrivateProfileString(sSection.GetBuffer(0), sKey.GetBuffer(0), ""
					, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
				SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_VERIFY_FILE_FAIL, (LPARAM)acBuffer);
			}
			return FALSE;
		}
	}
	// 循环校验所有文件
	for (i = 1; i <= iLanguageFileCount; i++)
	{
		if (m_bUserBreak)
		{
			return FALSE;
		}

		sTemp.Format("%sFile%d", sLanguageSection.GetBuffer(0), i);
		if (!VerifyFile(sTemp))
		{
			// 文件校验不通过，升级失败
			if (!m_bSilenceMode && m_hProgressWindow != NULL)
			{
				sKey = "Name";
				memset(acBuffer, 0, BUFFER_SIZE);
				GetPrivateProfileString(sSection.GetBuffer(0), sKey.GetBuffer(0), ""
					, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
				SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_VERIFY_FILE_FAIL, (LPARAM)acBuffer);
			}
			return FALSE;
		}
	}

	// 复制、更新文件

	// 创建备份文件目录
	CreateDirectory((GetAppDirectory() + "Backup").GetBuffer(0), NULL);
	// 循环更新所有文件
	for (i = 1; i <= iCommonFileCount; i++)
	{
		if (m_bUserBreak)
		{
			return FALSE;
		}

		sSection.Format("CommonFile%d", i);
		if (!UpdateFile(sSection))
		{
			// 文件更新不成功，升级失败
			if (!m_bSilenceMode && m_hProgressWindow != NULL)
			{
				sKey = "Name";
				memset(acBuffer, 0, BUFFER_SIZE);
				GetPrivateProfileString(sSection.GetBuffer(0), sKey.GetBuffer(0), ""
					, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
				SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_UPDATE_FILE_FAIL, (LPARAM)acBuffer);
			}
			return FALSE;
		}
	}
	// 循环更新所有文件
	for (i = 1; i <= iLanguageFileCount; i++)
	{
		if (m_bUserBreak)
		{
			return FALSE;
		}

		sTemp.Format("%sFile%d", sLanguageSection.GetBuffer(0), i);
		if (!UpdateFile(sTemp))
		{
			// 文件校验不通过，升级失败
			if (!m_bSilenceMode && m_hProgressWindow != NULL)
			{
				sKey = "Name";
				memset(acBuffer, 0, BUFFER_SIZE);
				GetPrivateProfileString(sSection.GetBuffer(0), sKey.GetBuffer(0), ""
					, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
				SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_UPDATE_FILE_FAIL, (LPARAM)acBuffer);
			}
			return FALSE;
		}
	}

	// 执行升级程序
	sKey = "RunAfterDownload";
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(SECTION_UPDATE, sKey.GetBuffer(0), ""
		, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
	sFilename = (char*)acBuffer;

	if (!sFilename.IsEmpty())
	{
		LOG(0, 0, "Run \"%s\" after downloading and verifying.", sFilename.GetBuffer(0));
		ShellExecute(NULL, "open", (GetAppDirectory() + "Update\\" + sFilename).GetBuffer(0), "", "", SW_NORMAL);
	}

	// 通知界面升级完毕
	if (!m_bSilenceMode && m_hProgressWindow != NULL)
	{
		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_FINISH_UPDATE, 0);
	}

	return TRUE;
}

// 检查并下载一个文件
BOOL CUpdateThread::DownloadFile(CString &sFileSection)
{
	const int BUFFER_SIZE = 512;
	char acBuffer[BUFFER_SIZE];
	CString sFilename;
	CString sHash;
	CString sURL;
	CString sKey;

	LOG(0, 0, "CUpdateThread::DownloadFile : sFileSection = %s.", sFileSection.GetBuffer(0));

	// 比较文件是否已经下载了，如是则跳过
	if (VerifyFile(sFileSection))
	{
		return TRUE;
	}

	// 取得文件名
	sKey = "Name";
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(sFileSection.GetBuffer(0), sKey.GetBuffer(0), ""
		, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
	sFilename = (char*)acBuffer;

	LOG(0, 0, "Start downloading file %s.", sFilename.GetBuffer(0));

	// 取得文件URL
	sKey = "URL";
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(sFileSection.GetBuffer(0), sKey.GetBuffer(0), ""
		, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
	sURL = (char*)acBuffer;

	// 更新显示正在下载的文件
	// 下载文件
	if (!m_bSilenceMode && m_hProgressWindow != NULL)
	{
		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOADING_FILENAME
			, (LPARAM)sFilename.GetBuffer(0));
	}
	if (Internet::InternetGetURL(sURL.GetBuffer(0)
		, (GetAppDirectory() + "Update\\" + sFilename).GetBuffer(0)
		, NULL
		, m_hProgressWindow)
		!= Internet::INTERNET_SUCCESS)
	{
		// 记录下载文件失败日志
		LOG(0, 0, "Fail to download file \"%s\".", sFilename.GetBuffer(0));
		return FALSE;
	}

	return TRUE;
}

// 校验文件
BOOL CUpdateThread::VerifyFile(CString &sFileSection)
{
	const int BUFFER_SIZE = 512;
	char acBuffer[BUFFER_SIZE];
	CString sFilename;
	UINT iFileSize;
	CString sHash;
	CString sKey;

	LOG(0, 0, "CUpdateThread::VerifyFile : sFileSection = %s.", sFileSection.GetBuffer(0));

	// 取得文件名
	sKey = "Name";
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(sFileSection.GetBuffer(0), sKey.GetBuffer(0), ""
		, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
	sFilename = (char*)acBuffer;

	// 取得文件大小
	sKey = "Size";
	iFileSize = GetPrivateProfileInt(sFileSection.GetBuffer(0), sKey.GetBuffer(0)
		, 0, m_sConfigFilename.GetBuffer(0));

	if (GetFileSize(GetAppDirectory() + "Update\\" + sFilename) == iFileSize)
	{
		sKey = "Hash";
		memset(acBuffer, 0, BUFFER_SIZE);
		GetPrivateProfileString(sFileSection.GetBuffer(0), sKey.GetBuffer(0), ""
			, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
		sHash = (char*)acBuffer;

		// 计算文件的Hash码以进行比较
		unsigned char acMD5Digest[16];
		CalculateMD5((GetAppDirectory() + "Update\\" + sFilename).GetBuffer(0), acMD5Digest);

		if (sHash.CompareNoCase(MD5toString(acMD5Digest)) == 0)
		{
			return TRUE;
		}
		else
		{
			LOG(0, 0, "Fail in verifying file \"%s\".", sFilename.GetBuffer(0));
		}
	}

	return FALSE;
}

// 更新文件
BOOL CUpdateThread::UpdateFile(CString &sFileSection)
{
	const int BUFFER_SIZE = 512;
	char acBuffer[BUFFER_SIZE];
	CString sFilename;
	CString sDestFilename;
	CString sBackupFilename;
	CString sKey;

	// 取得文件名
	sKey = "Name";
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(sFileSection.GetBuffer(0), sKey.GetBuffer(0), ""
		, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
	sFilename = (char*)acBuffer;

	// 取得目标文件名
	sKey = "DestPath";
	memset(acBuffer, 0, BUFFER_SIZE);
	GetPrivateProfileString(sFileSection.GetBuffer(0), sKey.GetBuffer(0), ""
		, (char*)acBuffer, BUFFER_SIZE, m_sConfigFilename.GetBuffer(0));
	sDestFilename = (char*)acBuffer;
	sDestFilename.TrimLeft();
	sDestFilename.TrimRight();
	if (sDestFilename.IsEmpty())
	{
		// 无目标目录，文件无需复制
		return TRUE;
	}
	sDestFilename += sFilename;

	// 替换变量字符串为系统变量
	sDestFilename = ResolvePath(sDestFilename.GetBuffer(0));

	// 备份原文件
	sBackupFilename = GetAppDirectory() + "Backup\\" + sFilename;
	if (GetFileSize(sDestFilename) > 0)
	{
		char acBuffer[MAX_PATH] = {0};

		// 取得本自动升级程序的文件全路径
		GetModuleFileName(AfxGetInstanceHandle(), acBuffer, sizeof(acBuffer));

		if (sFilename.CompareNoCase(GetFilename(acBuffer)) == 0)
		{
			// 要更新的文件是本自动升级程序，须换名更新
			CopyFile(sDestFilename.GetBuffer(0), sBackupFilename.GetBuffer(0), FALSE);
			// 复制新文件，新文件名加上 .new 后缀，由主程序来将其更名
			sDestFilename += ".new";
			return CopyFile((GetAppDirectory() + "Update\\" + sFilename).GetBuffer(0), sDestFilename.GetBuffer(0), FALSE);
		}
		else
		{
			MoveFile(sDestFilename.GetBuffer(0), sBackupFilename.GetBuffer(0));
		}
	}

	// 复制新文件
	return CopyFile((GetAppDirectory() + "Update\\" + sFilename).GetBuffer(0), sDestFilename.GetBuffer(0), FALSE);
}
