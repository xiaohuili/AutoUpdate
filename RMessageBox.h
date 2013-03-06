//------------------------------------------------------------------------------
// 文件名称：RMessageBox.h
// 文件版本：v1.0
// 创建日期：2006-05-06 04:25
// 作    者：Richard Ree
// 文件描述：自定义消息框类
//------------------------------------------------------------------------------

#pragma once

#ifndef RMESSAGEBOX_H
#define RMESSAGEBOX_H

#include "RDialog.h"
#include "RButton.h"

// CRMessageBox 对话框

class CRMessageBox : public CRDialog
{
	DECLARE_DYNAMIC(CRMessageBox)

public:
	CRMessageBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRMessageBox();

// 对话框数据
	enum { IDD = IDD_MESSAGEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAbort();
	afx_msg void OnBnClickedIganore();
	afx_msg void OnBnClickedRetry();
	afx_msg void OnBnClickedContinue();
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedNo();
	void ResetMessageBox();

public:
	// 消息框标题
	CString m_sTitle;
	// 消息框样式
	int m_iType;
	// 用户点中的按钮
	int m_iID;
	// 提示信息
	CString m_sPromptMessage;
	// 复选框1提示信息，当此字符串不为空时，显示复选框，否则隐藏复选框
	CString m_sOptionPromptMessage1;
	// 复选框1选则状态的值
	BOOL m_bOption1;
	// 复选框2提示信息，当此字符串不为空时，显示复选框，否则隐藏复选框
	CString m_sOptionPromptMessage2;
	// 复选框2选则状态的值
	BOOL m_bOption2;

protected:
	CStatic m_stcIcon;
	CStatic m_stcPromptMessage;
	CButton m_chkOption1;
	CButton m_chkOption2;
	CRButton m_btnOK;
	CRButton m_btnCancel;
	CRButton m_btnAbort;
	CRButton m_btnIganore;
	CRButton m_btnRetry;
	CRButton m_btnContinue;
	CRButton m_btnYes;
	CRButton m_btnNo;
};

#endif // #ifndef RMESSAGEBOX_H
