// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__6E171DE5_929C_4558_880B_14D7DDC8781D__INCLUDED_)
#define AFX_STDAFX_H__6E171DE5_929C_4558_880B_14D7DDC8781D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// 取得程序运行的目录
CString GetAppDirectory(BOOL bEndWithBackSlash = TRUE);

// 从文件全路径取得文件名
CString GetFilename(const char* sFilePath);
CString GetFilename(const CString& sFilePath);

// 取得文件扩展名
CString GetFileExtendName(const char* sFilename);
CString GetFileExtendName(const CString& sFilename);

// 取得文件大小
size_t GetFileSize(const char* sFilename);
size_t GetFileSize(const CString& sFilename);

// 写日志
void LOG(int iModuleID, int iLevel, const char* sFormat, ...);

// 计算文件的MD5摘要码
extern BOOL CalculateMD5(const char* sFilename, unsigned char acMD5Digest[]);
extern CString MD5toString(unsigned char acMD5Digest[]);

// 显示消息框函数，用于替代AfxMessageBox函数
int RMessageBox(CString& sText, UINT nType = MB_OK, UINT nIDHelp = 0);
int RMessageBox(LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0);

// 替换路径中的特殊字符串为实际路径
CString ResolvePath(const char* sPath);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6E171DE5_929C_4558_880B_14D7DDC8781D__INCLUDED_)
