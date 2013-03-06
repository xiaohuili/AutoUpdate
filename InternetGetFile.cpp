// InternetGetFile.cpp: implementation of the InternetGetFile namespace.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoUpdate.h"
#include <wininet.h>
#include <stdio.h>
#include <mmsystem.h>
#include "InternetGetFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define INTERNET_CONNECTION_OFFLINE 0x20
//#define NOTIFY_DOWNLOAD_PROGRESS 1003

BOOL Internet::bInternetGetURLUserBreak = FALSE;

// 通过HTTP协议从制定网址下载文件并保存到磁盘
// sURL：网址
// sSaveFilename：要保存的本地文件全路径
// sHeader：HTTP请求头信息
// hProgressWindow：接收下载文件进度消息并显示的窗口句柄
int Internet::InternetGetURL(const char* sURL, const char* sSaveFilename, const char* sHeader, HWND hProgressWindow)
{
	const int BUFFER_SIZE = 32768;
	const int iInterval = 250;
	DWORD iFlags;
	char sAgent[64];
	HINTERNET hInternet;
	HINTERNET hConnect;
	char acBuffer[BUFFER_SIZE];
	DWORD iReadBytes;
	DWORD iTotalReadBytes = 0;
	FILE *pFile = NULL;
	DWORD iStartTime;
	DWORD iCurrentTime;
	DWORD iDuration = 10;
	DWORD iLastRefreshTime = 0;
	DOWNLOAD_PROGRESS_STRU DownloadProgress = {0};
	DWORD iBytesToRead = 0;
	DWORD iReadBytesOfRq = 4;
	DWORD iCPUTickStart;
	DWORD iCPUTickEnd;

	// Get connection state
	InternetGetConnectedState(&iFlags, 0);
	if (iFlags & INTERNET_CONNECTION_OFFLINE) // take appropriate steps
	{
		return INTERNET_ERROR_OPEN;
	}

	// Set agent string
	_snprintf(sAgent, 64, "Agent%ld", GetTickCount() / 1000);
	// Open internet session
	if (!(iFlags & INTERNET_CONNECTION_PROXY))
	{
		hInternet = InternetOpenA(sAgent, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	}
	else
	{
		hInternet = InternetOpenA(sAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	}
	if (hInternet)
	{
		if (sHeader == NULL)
		{
			sHeader = "Accept: */*\r\n\r\n";
		}

		// Get URL
		if (!(hConnect = InternetOpenUrlA (hInternet, sURL, sHeader, lstrlenA(sHeader)
			, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0)))
		{
			InternetCloseHandle(hInternet);
			return INTERNET_ERROR_OPENURL;
		}

		// Open file to write
		if (!(pFile = fopen(sSaveFilename, "wb")))
		{
			InternetCloseHandle(hInternet);
			return INTERNET_ERROR_FILEOPEN;
		}

		// Get content size
		if (!HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER
			, (LPVOID)&iBytesToRead, &iReadBytesOfRq, NULL))
		{
			iBytesToRead = 0;
		}

		DownloadProgress.iCurrentFileSize = iBytesToRead;

		iStartTime = GetTickCount();
		iCPUTickStart = GetTickCount();

		do
		{
			if (bInternetGetURLUserBreak)
			{
				break;
			}

			// Keep copying in 16 KB chunks, while file has any data left.
			// Note: bigger buffer will greatly improve performance.
			if (!InternetReadFile(hConnect, acBuffer, BUFFER_SIZE, &iReadBytes))
			{
				fclose(pFile);
				InternetCloseHandle(hInternet);
				return INTERNET_ERROR_READFILE;
			}
			if (iReadBytes > 0)
			{
				// Write to file
				fwrite(acBuffer, sizeof(char), iReadBytes, pFile);
			}

			iTotalReadBytes += iReadBytes;

			iCurrentTime = GetTickCount();
			iDuration = iCurrentTime - iStartTime;
			if (iDuration < 10)
			{
				iDuration = 10;
			}

			if (iBytesToRead > 0
				&& hProgressWindow != NULL
				&& iCurrentTime - iLastRefreshTime > iInterval
				|| iTotalReadBytes == iBytesToRead)
			{
				// Show progress
				iLastRefreshTime = iCurrentTime;
				memset(acBuffer, 0, BUFFER_SIZE);
				DownloadProgress.iCurrentFileFinishedSize = iTotalReadBytes;
				SendMessage(hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOAD_PROGRESS, (LPARAM)&DownloadProgress);
#ifdef _DEBUG
				Sleep(100); // Delay some time to see progress clearly
#endif
			}

			if (iReadBytes <= 0)
			{
				break; // Condition of iReadBytes=0 indicate EOF. Stop.
			}

			// 降低资源CPU占用率
			iCPUTickEnd = GetTickCount();
			if (iCPUTickEnd - iCPUTickStart > 50)
			{
				Sleep(iCPUTickEnd - iCPUTickStart);
				iCPUTickStart = GetTickCount();
			}
		} while (TRUE);

		fflush (pFile);
		fclose (pFile);

		InternetCloseHandle(hInternet);
	}
	else
	{
		return INTERNET_ERROR_OPEN;
	}

	return INTERNET_SUCCESS;
}
