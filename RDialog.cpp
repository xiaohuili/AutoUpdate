//------------------------------------------------------------------------------
// 文件名称：RDialog.cpp
// 文件版本：v1.0
// 创建日期：2006-05-05 16:36
// 作    者：Richard Ree
// 文件描述：自绘对话框类实现文件
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AutoUpdate.h"
#include "UISkinManager.h"
#include "RDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CRDialog 对话框

IMPLEMENT_DYNAMIC(CRDialog, CDialog)

CRDialog::CRDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd /*=NULL*/)
	: CDialog(lpszTemplateName, pParentWnd)
{
}

CRDialog::CRDialog(UINT nIDTemplate, CWnd* pParentWnd /*=NULL*/)
	: CDialog(nIDTemplate, pParentWnd)
	, m_btnMinimize(CRButton::ButtonTypeMinimize)
	, m_btnMaximize(CRButton::ButtonTypeMaximize)
	, m_btnClose(CRButton::ButtonTypeClose)
{
}

CRDialog::~CRDialog()
{
}

BOOL CRDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 创建最小化、自动化、关闭按钮
	CRect rcWnd;
	CRect rcButton;

	GetClientRect(&rcWnd);
	rcButton.SetRect(0, 0, CUISkinManager::m_iSystemControlButtonWidth, CUISkinManager::m_iSystemControlButtonHeight);
	rcButton.OffsetRect(rcWnd.Width() - CUISkinManager::m_iLeftMargin - rcButton.Width(), 6);

	m_btnMinimize.Create("", WS_CHILD, rcButton, this, IDC_BUTTON_MINIMIZE);
	m_btnMaximize.Create("", WS_CHILD, rcButton, this, IDC_BUTTON_MAXIMIZE);
	m_btnClose.Create("", WS_CHILD, rcButton, this, IDC_BUTTON_CLOSE);

	m_btnClose.MoveWindow(rcButton);
	rcButton.OffsetRect(-rcButton.Width() - 2, 0);
	m_btnMaximize.MoveWindow(rcButton);
	rcButton.OffsetRect(-rcButton.Width() - 2, 0);
	m_btnMinimize.MoveWindow(rcButton);

	// 显示/隐藏按钮
	DWORD iWindowStyle;

	iWindowStyle = GetStyle();
	if (iWindowStyle & WS_MINIMIZEBOX)
	{
		m_btnMinimize.ShowWindow(SW_SHOW);
	}
	if (iWindowStyle & WS_MAXIMIZEBOX)
	{
		m_btnMaximize.ShowWindow(SW_SHOW);
	}
//	if (iWindowStyle & WS_CLOSE)
//	{
		m_btnClose.ShowWindow(SW_SHOW);
//	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// 处理命令消息
BOOL CRDialog::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nID == IDC_BUTTON_MINIMIZE)
	{
		OnBnClickedMinimize();
	}
	else if (nID == IDC_BUTTON_MAXIMIZE)
	{
		OnBnClickedMaximize();
	}
	else if (nID == IDC_BUTTON_CLOSE)
	{
		OnBnClickedClose();
	}

	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BEGIN_MESSAGE_MAP(CRDialog, CDialog)
//	RDIALOG_UI_MESSAGE_MAP
	ON_BN_CLICKED(IDC_BUTTON_MINIMIZE, OnBnClickedMinimize)
	ON_BN_CLICKED(IDC_BUTTON_MAXIMIZE, OnBnClickedMaximize)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnBnClickedClose)
END_MESSAGE_MAP()


// CRDialog 消息处理程序

void CRDialog::OnPaint()
{
	if (!IsWindowVisible() || IsIconic())
	{
		CDialog::OnPaint();
		return;
	}

	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	CUISkinManager::Paint(this, &dc);

	// 自绘窗口
	CUISkinManager::PaintDialogBorder(this, &dc);

	// 重绘按钮
//	m_btnMinimize.Invalidate();
//	m_btnMaximize.Invalidate();
//	m_btnClose.Invalidate();

	return;
}

void CRDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// 移动按钮
	if (m_btnClose.m_hWnd != NULL)
	{
		CRect rcButton;
		m_btnClose.GetWindowRect(&rcButton);
		rcButton.OffsetRect(cx - CUISkinManager::m_iLeftMargin - rcButton.Width() - rcButton.left, 6 - rcButton.top);
		m_btnClose.MoveWindow(rcButton);
		rcButton.OffsetRect(-rcButton.Width() - 2, 0);
		m_btnMaximize.MoveWindow(rcButton);
		rcButton.OffsetRect(-rcButton.Width() - 2, 0);
		m_btnMinimize.MoveWindow(rcButton);
	}

	CUISkinManager::SetRgn(this);
}

HBRUSH CRDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
//	return hbr;
	return CUISkinManager::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CRDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 鼠标点击窗口标题栏，发送消息模拟实现拖动窗口的功能
	if (point.y <= (LONG)CUISkinManager::m_iTitleBarHeight)
	{
		SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

// 点击最小化按钮
void CRDialog::OnBnClickedMinimize()
{
	// 发送最小化窗口消息
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

// 点击最大化按钮
void CRDialog::OnBnClickedMaximize()
{
	if (GetWindowLong(m_hWnd, GWL_STYLE) & WS_MAXIMIZE)
	{
		// 如果当前窗口处于最大化状态，发送恢复窗口大小消息
		SendMessage(WM_SYSCOMMAND, SC_RESTORE);
	}
	else
	{
		// 发送最大化窗口消息
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	}
}

// 点击关闭按钮
void CRDialog::OnBnClickedClose()
{
	// 发送关闭窗口消息
	SendMessage(WM_SYSCOMMAND, SC_CLOSE);
}
