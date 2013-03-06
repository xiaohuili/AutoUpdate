// InternetGetFile.h: interface for the InternetGetFile namespace.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if !defined(INTERNETGETFILE_H)
#define INTERNETGETFILE_H

namespace Internet
{
	enum
	{
		INTERNET_SUCCESS = 0,
		INTERNET_ERROR_OPENURL,
		INTERNET_ERROR_FILEOPEN,
		INTERNET_ERROR_READFILE,
		INTERNET_ERROR_OPEN
	};

	extern BOOL bInternetGetURLUserBreak;	// 中断下载标记，置为真则中断下载文件

	// Get URL form internet with inet API
	int InternetGetURL(const char* sURL, const char* sSaveFilename, const char* sHeader = NULL, HWND hProgressWindow = NULL);
};

#endif // !defined(INTERNETGETFILE_H)
