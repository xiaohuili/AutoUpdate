//------------------------------------------------------------------------------
// 文件名称：RDialog.h
// 文件版本：v1.0
// 创建日期：2006-05-05 16:36
// 作    者：Richard Ree
// 文件描述：自绘对话框类
// 使用自绘对话框类派生自定义对话框的步骤是：
// 1、将自定义对话框类的基类由CDialog改为CRDialog
// 2、在派生类的构造函数、对话框初始化函数OnInitDialog等各处，将CDialog改为CRDialog
// 3、在派生类的消息定义BEGIN_MESSAGE_MAP后添加宏RDIALOG_UI_MESSAGE_MAP
//------------------------------------------------------------------------------

#pragma once

#ifndef RDIALOG_H
#define RDIALOG_H

#include "RButton.h"

// 实现自绘界面的消息映射宏，将此宏插入到派生类的BEGIN_MESSAGE_MAP后
#define RDIALOG_UI_MESSAGE_MAP\
	ON_WM_PAINT()\
	ON_WM_SIZE()\
	ON_WM_CTLCOLOR()\
	ON_WM_LBUTTONDOWN()

// CRDialog 对话框

class CRDialog : public CDialog
{
	DECLARE_DYNAMIC(CRDialog)

public:
	// 构造/析构函数
	CRDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	CRDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CRDialog();

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedMinimize();
	afx_msg void OnBnClickedMaximize();
	afx_msg void OnBnClickedClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

protected:
	// 最小化按钮
	CRButton m_btnMinimize;
	// 最大化按钮
	CRButton m_btnMaximize;
	// 关闭按钮
	CRButton m_btnClose;
};

#endif // #ifndef RDIALOG_H
