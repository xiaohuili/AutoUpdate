//------------------------------------------------------------------------------
// 文件名称：UISkinManager.cpp
// 文件版本：v1.0
// 创建日期：2006-02-14 16:59
// 作    者：Richard Ree
// 文件描述：界面管理类的实现文件
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AutoUpdate.h"
#include "UISkinManager.h"

#pragma comment(lib, "msimg32.lib")
#pragma message("Automatically linking with msimg32.lib")

// 透明色
const COLORREF CUISkinManager::TRANSPARENT_COLOR = RGB(255, 0, 255);

// 当前界面风格
enum CUISkinManager::SkinStyle CUISkinManager::m_iCurrentStyle;

// 窗口标题栏风格

// 标题栏高度
UINT CUISkinManager::m_iTitleBarHeight;
// 绘制标题栏所需的图像宽度（象素）
UINT CUISkinManager::m_iTitleBarImageWidth;
// 标题栏图象左边部分的宽度
UINT CUISkinManager::m_iTitleBarImageLeftPartWidth;
// 标题栏图象中间部分的宽度（此部分是可拉伸的）
UINT CUISkinManager::m_iTitleBarImageMiddlePartWidth;
// 标题栏图象右边部分的宽度
UINT CUISkinManager::m_iTitleBarImageRightPartWidth;
// 绘制标题栏所需的图像集（每个图像宽度为 m_iTitleBarImageWidth 象素，按正常、失去焦点的顺序排列，总宽度不小于 2 * m_iTitleBarImageWidth 象素）
CBitmap CUISkinManager::m_bmTitleBarImage;

// 对话框窗口左右边距
UINT CUISkinManager::m_iLeftMargin;
// 对话框窗口上下边距
UINT CUISkinManager::m_iTopMargin;

// 按钮风格

// 按钮高度（绘制按钮所需的图像高度）
UINT CUISkinManager::m_iButtonHeight;
// 绘制按钮所需的图像宽度（象素）
UINT CUISkinManager::m_iButtonImageWidth;
// 按钮图象左边部分的宽度
UINT CUISkinManager::m_iButtonImageLeftPartWidth;
// 按钮图象中间部分的宽度（此部分是可拉伸的）
UINT CUISkinManager::m_iButtonImageMiddlePartWidth;
// 按钮图象右边部分的宽度
UINT CUISkinManager::m_iButtonImageRightPartWidth;
// 绘制按钮所需的图像集（每个图像宽度为 m_iButtonImageWidth 象素，按正常、缺省、鼠标悬停、鼠标按下的顺序排列，总宽度不小于 4 * m_iButtonImageWidth 象素）
CBitmap CUISkinManager::m_bmButtonImage;
// 系统控制按钮的宽度和高度（象素）
UINT CUISkinManager::m_iSystemControlButtonWidth;
UINT CUISkinManager::m_iSystemControlButtonHeight;
// 绘制系统控制按钮（最小化、最大化、关闭等按钮）所需的图像集（每个图像宽度为 m_iSystemControlButtonImageWidth 象素，按最小化、最大化、恢复、关闭、按钮按下状态的顺序排列，总宽度不小于 8 * m_iSystemControlButtonImageWidth 象素）
CBitmap CUISkinManager::m_bmSystemControlButtonImage;

// 绘制控件的画笔
HBRUSH CUISkinManager::m_hbr;

CUISkinManager::CUISkinManager()
{
}

CUISkinManager::~CUISkinManager()
{
}

BOOL CUISkinManager::Init()
{
	m_hbr = NULL;

	if (!LoadImages())
	{
		return FALSE;
	}

	ChangeStyle(StyleSummer);

	return TRUE;
}

void CUISkinManager::Uninit()
{
	if (m_hbr != NULL)
	{
		DeleteObject(m_hbr);
	}

	m_bmTitleBarImage.DeleteObject();
	m_bmButtonImage.DeleteObject();
	m_bmSystemControlButtonImage.DeleteObject();
}

BOOL CUISkinManager::LoadImages()
{
	m_bmTitleBarImage.DeleteObject();
	m_bmButtonImage.DeleteObject();
	m_bmSystemControlButtonImage.DeleteObject();

	switch (m_iCurrentStyle)
	{
	case StyleSummer:
	default:
		m_bmTitleBarImage.LoadBitmap(IDB_TITLEBAR);
		m_bmButtonImage.LoadBitmap(IDB_BUTTON);
		m_bmSystemControlButtonImage.LoadBitmap(IDB_SYSTEM_CONTROL_BUTTON);
		break;
	}

	return TRUE;
}

HBRUSH CUISkinManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pDC == NULL || pWnd == NULL || pWnd->m_hWnd == NULL)
	{
		return NULL;
	}

	if (nCtlColor == CTLCOLOR_EDIT
		|| nCtlColor == CTLCOLOR_MSGBOX
		|| nCtlColor == CTLCOLOR_LISTBOX
		|| nCtlColor == CTLCOLOR_SCROLLBAR)
	{
		return (HBRUSH)GetStockObject(WHITE_BRUSH);
	}

	pDC->SetBkMode(TRANSPARENT);

	return GetBrush();
}

// 返回窗口背景色
COLORREF CUISkinManager::GetColor()
{
	switch (m_iCurrentStyle)
	{
	case StyleSpring:
		return RGB(240, 245, 255);
	case StyleSummer:
		return RGB(238, 247, 255);
	case StyleAutumn:
		return RGB(122, 33, 123);
	case StyleWinter:
//		return RGB(225, 233, 240); // 蓝灰色
		return RGB(245, 245, 245); // 灰色
	case StyleXP:
		return RGB(240, 245, 255);
	case StyleNormal:
	case StyleCustomize:
	default:
		return ::GetSysColor(COLOR_WINDOW);
		break;
	}
}

HBRUSH CUISkinManager::GetBrush()
{
	return m_hbr;
}

// 切换界面风格
void CUISkinManager::ChangeStyle(enum CUISkinManager::SkinStyle iNewStyle)
{
	m_iCurrentStyle = iNewStyle;

	switch (m_iCurrentStyle)
	{
	case StyleSummer:
	default:
		m_iTitleBarHeight = 24;
		m_iTitleBarImageWidth = 64;
		m_iTitleBarImageLeftPartWidth = 15;
		m_iTitleBarImageMiddlePartWidth = 2;
		m_iTitleBarImageRightPartWidth = 15;
		m_iLeftMargin = 8;
		m_iTopMargin = 8;
		m_iButtonHeight = 20;
		m_iButtonImageWidth = 128;
		m_iButtonImageLeftPartWidth = 15;
		m_iButtonImageMiddlePartWidth = 2;
		m_iButtonImageRightPartWidth = 15;
		m_iSystemControlButtonWidth = 16;
		m_iSystemControlButtonHeight = 14;
		break;
	}

	if (m_hbr != NULL)
	{
		DeleteObject(m_hbr);
	}

	m_hbr = ::CreateSolidBrush(GetColor());
}

// 以渐变方式填充矩形
void CUISkinManager::GradientFillRect(CWnd* pWnd, CDC* pDC, CRect& rcFill, ULONG nMode, COLORREF crLeftTop, COLORREF crRightBottom)
{
	if (pWnd == NULL || pWnd->m_hWnd == NULL)
	{
		return;
	}

	COLOR16 r = (COLOR16)((crLeftTop & 0x000000FF) << 8);
	COLOR16 g = (COLOR16)(crLeftTop & 0x0000FF00);
	COLOR16 b = (COLOR16)((crLeftTop & 0x00FF0000) >> 8);

	TRIVERTEX vert[2] ;
	GRADIENT_RECT gRect;
	vert[0].x     = rcFill.left;
	vert[0].y     = rcFill.top;
	vert[0].Red   = r;
	vert[0].Green = g;
	vert[0].Blue  = b;
	vert[0].Alpha = 0x0000;

	r = (COLOR16) ((crRightBottom & 0x000000FF)<<8);
	g = (COLOR16) (crRightBottom & 0x0000FF00);
	b = (COLOR16) ((crRightBottom & 0x00FF0000)>>8);

	vert[1].x     = rcFill.right;
	vert[1].y     = rcFill.bottom;
	vert[1].Red   = r;
	vert[1].Green = g;
	vert[1].Blue  = b;
	vert[1].Alpha = 0x0000;

	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;

	GradientFill(pDC->m_hDC, vert, 2, &gRect, 1, nMode);
}

// 透明拉伸复制
void CUISkinManager::TransparentBlt(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest, HDC hdcSrc, int nXSrc, int nYSrc, int nWidthSrc,int nHeightSrc, COLORREF crTransparent)
{
	HBITMAP hbm = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);
	HBITMAP hbmMemory = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);	
	HDC hdc = CreateCompatibleDC(hdcDest);
	HDC hdcMemory = CreateCompatibleDC(hdcDest);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdc, hbm);
	HBITMAP hbmOldMemory = (HBITMAP)SelectObject(hdcMemory, hbmMemory);

	// step 1: hdcSrc--(copy)-->hdc
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
	{
		// 原样复制
		BitBlt(hdc, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXSrc, nYSrc, SRCCOPY);
	}
	else
	{
		// 拉伸复制
		StretchBlt(hdc, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXSrc, nYSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	}

	// step 2: hdc--(copy)-->hdcMemory
	SetBkColor(hdc, crTransparent);
	BitBlt(hdcMemory, 0, 0, nWidthDest, nHeightDest, hdc, 0, 0, SRCCOPY);

	// step 3: hdcMemory--(and)-->hdc
	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255, 255, 255));
	BitBlt(hdc, 0, 0, nWidthDest, nHeightDest, hdcMemory, 0, 0, SRCAND);

	// step 4: hdcMemory--(and)-->hdcDest
	SetBkColor(hdcDest, RGB(255, 255, 255));
	SetTextColor(hdcDest, RGB(0, 0, 0));
	BitBlt(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest, hdcMemory, 0, 0, SRCAND);

	// step 5: hdc--(paint)-->hdcDest
	BitBlt(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest, hdc, 0, 0, SRCPAINT);

	// 释放资源
	SelectObject(hdc, hbmOld);
	DeleteDC(hdc);
	SelectObject(hdcMemory, hbmOldMemory);
	DeleteDC(hdcMemory);
	DeleteObject(hbm);
	DeleteObject(hbmMemory);
}

// 绘制窗口
void CUISkinManager::Paint(CWnd* pWnd, CDC* pDC, BOOL bFillClient, COLORREF crFillColor)
{
	if (pWnd == NULL || pWnd->m_hWnd == NULL)
	{
		return;
	}

	BOOL bGetDC = FALSE;

	if (!pDC)
	{
		bGetDC = TRUE;
		pDC = pWnd->GetDC();
	}

	switch (m_iCurrentStyle)
	{
	case StyleXP:
		Paint_XP(pWnd, pDC, bFillClient, crFillColor);
		break;
	default:
		Paint_Normal(pWnd, pDC, bFillClient, crFillColor);
		break;
	}

	if (bGetDC)
	{
		pWnd->ReleaseDC(pDC);
	}
}

void CUISkinManager::Paint_Normal(CWnd* pWnd, CDC* pDC, BOOL bFillClient, COLORREF crFillColor)
{
	if (pWnd == NULL || pWnd->m_hWnd == NULL)
	{
		return;
	}

	CRect rcWnd;
	pWnd->GetClientRect(&rcWnd);

	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);
	CBitmap bmpMemory;
	bmpMemory.CreateCompatibleBitmap(pDC, rcWnd.Width(), rcWnd.Height());
	CBitmap* pOldbmpMemory = (CBitmap*)dcMemory.SelectObject(&bmpMemory);

	// 填充窗口背景色
	dcMemory.FillSolidRect(&rcWnd, GetColor());

	int cx = ::GetSystemMetrics(SM_CXDLGFRAME);
	int cy = ::GetSystemMetrics(SM_CYDLGFRAME);

	CRect rc;
	CRgn rgn;
	pWnd->GetWindowRect(&rc);
	rc.right -= 2 * cx;
	rc.bottom -= 2 * cy;

	// 以背景图填充标题栏
	CRect rcTitleBar;
	rcTitleBar.SetRect(rc.left, rc.top, rc.right, rc.top + m_iTitleBarHeight);
	HDC hdcSrc = CreateCompatibleDC(dcMemory.GetSafeHdc());
	HGDIOBJ hOldGdiObj = SelectObject(hdcSrc, m_bmTitleBarImage);
	// 左部
	TransparentBlt(dcMemory.GetSafeHdc(), 0, 0, m_iTitleBarImageLeftPartWidth, m_iTitleBarHeight
		, hdcSrc, 0, 0, m_iTitleBarImageLeftPartWidth, m_iTitleBarHeight, TRANSPARENT_COLOR);
	// 中部
	TransparentBlt(dcMemory.GetSafeHdc(), m_iTitleBarImageLeftPartWidth, 0
		, rcTitleBar.Width() - m_iTitleBarImageLeftPartWidth - m_iTitleBarImageRightPartWidth, m_iTitleBarHeight
		, hdcSrc, m_iTitleBarImageLeftPartWidth, 0
		, m_iTitleBarImageMiddlePartWidth, m_iTitleBarHeight, TRANSPARENT_COLOR);
	// 右部
	TransparentBlt(dcMemory.GetSafeHdc(), rcTitleBar.Width() - m_iTitleBarImageRightPartWidth, 0
		, m_iTitleBarImageRightPartWidth, m_iTitleBarHeight
		, hdcSrc, m_iTitleBarImageLeftPartWidth + m_iTitleBarImageMiddlePartWidth, 0
		, m_iTitleBarImageRightPartWidth, m_iTitleBarHeight, TRANSPARENT_COLOR);
	SelectObject(hdcSrc, hOldGdiObj);

	// 填充客户区背景色
	pWnd->GetClientRect(&rc);
	rc.OffsetRect(-rc.left, -rc.top);
	rc.DeflateRect(m_iLeftMargin, m_iTitleBarHeight + 8, m_iLeftMargin, m_iTopMargin);
	dcMemory.FillSolidRect(rc.left, rc.top, rc.Width(), rc.Height(), RGB(238, 247, 255));

	// 画稍暗的左、右、底边线
	pWnd->GetClientRect(&rc);
	rgn.CreateRoundRectRgn(0, 0, rc.Width(), rc.Height(), 5, 5);
	CBrush br3(RGB(141, 193, 250));
	dcMemory.FrameRgn(&rgn, &br3, 1, 1); 
	br3.DeleteObject();
	rgn.DeleteObject();

	// 输出标题栏文字

	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 18; // 字号
	strncpy(lf.lfFaceName, "Microsoft Sans Serif", 32); // 字体
	lf.lfWeight = 600; // 粗细
	VERIFY(font.CreateFontIndirect(&lf));

	CString strCaption;
	pWnd->GetWindowText(strCaption);
	CFont* pOldFont = dcMemory.SelectObject(&font);
	dcMemory.SetBkMode(TRANSPARENT);

	// 以灰、黑色输出文字形成阴影效果
	dcMemory.SetTextColor(RGB(160, 160, 160));
	dcMemory.TextOut(12, 4, strCaption);
	dcMemory.SetTextColor(RGB(10, 30, 100));
	dcMemory.TextOut(11, 3, strCaption);

	dcMemory.SelectObject(pOldFont);
	font.DeleteObject();

	// 从内存设备环境复制位图到窗口
	pDC->BitBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &dcMemory, 0, 0, SRCCOPY);

	// 释放资源
	dcMemory.SelectObject(::GetStockObject(WHITE_PEN));
	dcMemory.SelectObject(pOldbmpMemory);
	bmpMemory.DeleteObject();
	dcMemory.DeleteDC();
}

void CUISkinManager::Paint_XP(CWnd* pWnd, CDC* pDC, BOOL bFillClient, COLORREF crFillColor)
{
}

// 绘制对话框边框，对话框具有通用样式：蓝色框线、下方有按钮工具条
void CUISkinManager::PaintDialogBorder(CWnd* pWnd, CDC* pDC)
{
	if (pWnd == NULL || pWnd->m_hWnd == NULL)
	{
		return;
	}

	BOOL bGetDC = FALSE;

	if (!pDC)
	{
		bGetDC = TRUE;
		pDC = pWnd->GetDC();
	}

	const int BOTTOM_COMMAND_BUTTON_BAR_HEIGHT = 45; // 对话框底部按钮工具条高度（象素）
	CRect rcWnd;
	CRect rc;

	pWnd->GetClientRect(&rcWnd);
	rcWnd.DeflateRect(m_iLeftMargin, m_iTitleBarHeight + m_iTopMargin, m_iLeftMargin + 1, m_iTopMargin + 1);

	// 画水平分隔线
	rc.SetRect(rcWnd.left, rcWnd.bottom - BOTTOM_COMMAND_BUTTON_BAR_HEIGHT, rcWnd.right, rcWnd.bottom - BOTTOM_COMMAND_BUTTON_BAR_HEIGHT + 2);
	pDC->Draw3dRect(rc, RGB(162, 185, 201), RGB(255, 255, 255));

	// 画边框
	CBrush br(RGB(100, 155, 194));
	rc = rcWnd;
	pDC->FrameRect(&rc, &br);
	rc.DeflateRect(1, 1, 1, 1);
	pDC->FrameRect(&rc, &br);

	// 画下方的按钮栏渐变背景
	rc.DeflateRect(1, 1, 1, 1);
	rc.top = rc.bottom - BOTTOM_COMMAND_BUTTON_BAR_HEIGHT + 4;
	GradientFillRect(pWnd, pDC, rc, GRADIENT_FILL_RECT_V, RGB(170, 210, 245), RGB(255, 255, 255));

	if (bGetDC)
	{
		pWnd->ReleaseDC(pDC);
	}
}

// 设置窗口可见区域
void CUISkinManager::SetRgn(CWnd* pWnd)
{
	switch (m_iCurrentStyle)
	{
	case StyleXP:
		SetRgn_XP(pWnd);
		break;
	default:
		SetRgn_Normal(pWnd);
		break;
	}
}

void CUISkinManager::SetRgn_Normal(CWnd* pWnd)
{
	if (NULL == pWnd || NULL == pWnd->m_hWnd)
	{
		return;
	}

	// 取得窗口边框线宽度，边框线属于不可重绘范围，需隐藏掉
	int cx = ::GetSystemMetrics(SM_CXDLGFRAME);
	int cy = ::GetSystemMetrics(SM_CYDLGFRAME);

	CRect rc;
	CRgn rgn;
	pWnd->GetWindowRect(&rc);

	// 构造圆角矩形作为窗口的可见区域，排除边框线
	rgn.CreateRoundRectRgn(cx, cy, rc.Width() - cx, rc.Height() - cy, 5, 5);

	// 设置窗口可见区域
	pWnd->SetWindowRgn(rgn, TRUE);

	rgn.DeleteObject();
}

void CUISkinManager::SetRgn_XP(CWnd* pWnd)
{
	int cx = ::GetSystemMetrics(SM_CXDLGFRAME);
	int cy = ::GetSystemMetrics(SM_CYDLGFRAME);

	CRect rc;
	CRgn rgn;
	pWnd->GetWindowRect(&rc);

	// 构造圆角矩形作为窗口的可见区域
	rgn.CreateRoundRectRgn(cx, cy, rc.Width() - cx, rc.Height() - cy, 33, 33);

	// 设置窗口可见区域
	pWnd->SetWindowRgn(rgn, TRUE);

	rgn.DeleteObject();
}
