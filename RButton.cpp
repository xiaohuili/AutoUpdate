//------------------------------------------------------------------------------
// 文件名称：RButton.h
// 文件版本：v1.0
// 创建日期：2006-05-06 10:39
// 作    者：Richard Ree
// 文件描述：自绘按钮类实现文件
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AutoUpdate.h"
#include "UISkinManager.h"
#include "RButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CRButton

IMPLEMENT_DYNAMIC(CRButton, CButton)

CRButton::CRButton(enButtonType iButtonType)
{
	m_iButtonType = iButtonType;
	m_bTracking = FALSE;
	m_bOver = FALSE;
	m_bDown = FALSE;
	m_bFocus = FALSE;
	m_bDefault = FALSE;
}

CRButton::~CRButton()
{
}

void CRButton::PreSubclassWindow()
{
	CButton::PreSubclassWindow();

	UINT uStyle = GetButtonStyle();
	if (uStyle == BS_DEFPUSHBUTTON)
	{
		m_bDefault = TRUE;
	}

	ModifyStyle(0, BS_OWNERDRAW);
}

// 自绘控件
void CRButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CRect rcButton = lpDrawItemStruct->rcItem;
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC); 
	UINT iState = lpDrawItemStruct->itemState;

	// 获取按钮的状态
	m_bFocus = iState & ODS_FOCUS;
	m_bDown = iState & ODS_SELECTED;

	// 绘制按钮背景
	if (CUISkinManager::m_iCurrentStyle == CUISkinManager::StyleSummer)
	{
		if (m_iButtonType == ButtonTypeMinimize)
		{
			// 绘制最小化窗口按钮
			DrawSystemControlButtonWithBitmap(pDC->GetSafeHdc(), rcButton
				, (HBITMAP)CUISkinManager::m_bmSystemControlButtonImage, m_bDown ? 16 : 0);
			return;
		}
		else if (m_iButtonType == ButtonTypeMaximize)
		{
			if (GetWindowLong(GetParent()->m_hWnd, GWL_STYLE) & WS_MAXIMIZE)
			{
				// 绘制最大化窗口按钮
				DrawSystemControlButtonWithBitmap(pDC->GetSafeHdc(), rcButton
					, (HBITMAP)CUISkinManager::m_bmSystemControlButtonImage, m_bDown ? 48 : 32);
			}
			else
			{
				// 绘制还原窗口大小按钮
				DrawSystemControlButtonWithBitmap(pDC->GetSafeHdc(), rcButton
					, (HBITMAP)CUISkinManager::m_bmSystemControlButtonImage, m_bDown ? 80 : 64);
			}
			return;
		}
		else if (m_iButtonType == ButtonTypeClose)
		{
			// 绘制关闭窗口按钮
			DrawSystemControlButtonWithBitmap(pDC->GetSafeHdc(), rcButton
				, (HBITMAP)CUISkinManager::m_bmSystemControlButtonImage, m_bDown ? 112 : 96);
			return;
		}
		else if (m_bDefault)
		{
			DrawItemWithBitmap(pDC->GetSafeHdc(), rcButton, (HBITMAP)CUISkinManager::m_bmButtonImage, 20, 32);
		}
		else if (m_bOver)
		{
			DrawItemWithBitmap(pDC->GetSafeHdc(), rcButton, (HBITMAP)CUISkinManager::m_bmButtonImage, 20, 32 * 2);
		}
		else if (m_bDown)
		{
			DrawItemWithBitmap(pDC->GetSafeHdc(), rcButton, (HBITMAP)CUISkinManager::m_bmButtonImage, 20, 32 * 3);
		}
		else
		{
			DrawItemWithBitmap(pDC->GetSafeHdc(), rcButton, (HBITMAP)CUISkinManager::m_bmButtonImage, 20, 0);
		}
	}
	else // 缺省风格
	{
		// 内边框
		COLORREF crTop, crBottom;
		CRect rcOver = rcButton;
		rcOver.DeflateRect(1,1);

		if (m_bDefault)
		{
			crTop = RGB(203, 231, 255);
			crBottom = RGB(105, 130, 238);
		}
		else if (m_bOver)
		{
			crTop = RGB(255, 240, 207);
			crBottom = RGB(229, 151, 0);
		}
		else if (m_bDown)
		{
			crTop = RGB(209, 204, 193);
			crBottom = RGB(218, 216, 207);
		}
		else
		{
			crTop = RGB(254, 254, 254);
			crBottom = RGB(234, 245, 255);
		}
		CUISkinManager::GradientFillRect(this, pDC, rcOver, GRADIENT_FILL_RECT_V, crTop, crBottom);

		// 外边框
		CPen pen(PS_SOLID, 1, RGB(200, 210, 240)); // 边框颜色
		DrawRoundBorder(pDC, rcButton);

		// 内部填充
		if (!m_bDown)
		{
			CRect rcIn = rcButton;
			if (m_bOver || m_bFocus)
			{
				rcIn.DeflateRect(3,3);
			}
			else
			{
				rcIn.DeflateRect(2,2);
			}
			crTop = RGB(252, 252, 251);
			crBottom = RGB(236, 235, 230);
			CUISkinManager::GradientFillRect(this, pDC, rcIn, GRADIENT_FILL_RECT_V, crTop, crBottom);
		}
	}

	rcButton.DeflateRect(CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));

	// 绘制焦点框
	if (m_bFocus)
	{
		CRect rcFoucs = rcButton;
		pDC->DrawFocusRect(rcFoucs);
	}

	// 绘制按钮标题文本

	CString strTitle;

	GetWindowText(strTitle);

	if (strlen(strTitle) > 0)
	{
		CFont* hFont = GetFont();
		CFont* hOldFont = pDC->SelectObject(hFont);
		// 计算文本显示的宽度
		CSize szExtent = pDC->GetTextExtent(strTitle, lstrlen(strTitle));
		if (strstr(strTitle, "&") != 0)
		{
			// 减去因 '&' 字符被隐藏时的宽度
			szExtent.cx -= pDC->GetTextExtent("&").cx;
		}

		CPoint pt(rcButton.CenterPoint().x - szExtent.cx / 2, rcButton.CenterPoint().y - szExtent.cy / 2);
		if (iState & ODS_SELECTED)
		{
			pt.Offset(1, 1);
		}
		int nMode = pDC->SetBkMode(TRANSPARENT);
		if (iState & ODS_DISABLED)
		{
			pDC->DrawState(pt, szExtent, strTitle, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		}
		else
		{
			pDC->DrawState(pt, szExtent, strTitle, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
		}

		pDC->SelectObject(hOldFont);
		pDC->SetBkMode(nMode);
	}
}


BEGIN_MESSAGE_MAP(CRButton, CButton)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


// CRButton 消息处理程序


void CRButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tms;
		tms.cbSize = sizeof(TRACKMOUSEEVENT);
		tms.dwFlags = TME_LEAVE|TME_HOVER;
		tms.dwHoverTime = 1;
		tms.hwndTrack = m_hWnd;
		m_bTracking = ::_TrackMouseEvent(&tms);
	}
	CButton::OnMouseMove(nFlags, point);
}

LRESULT CRButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	m_bOver = TRUE;
	InvalidateRect(NULL);
	return m_bTracking;
}

LRESULT CRButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	m_bOver = FALSE;
	InvalidateRect(NULL);
	return m_bTracking;
}

// 以位图方式描绘按钮控件，要求位图宽度为32象素（左15 + 中2 + 右15，中间随着控件宽度拉伸）
// HDC hdcDest：目标设备环境句柄
// CRect& rc：按钮所占的区域（描绘区域）
// HBITMAP hBitMap：位图句柄
// UINT iBitMapHeight：位图高度
// UINT cx：从位图复制的起点位置，按钮处于不同状态（正常、有焦点、鼠标悬停、按下）时对应不同的起点位置
void CRButton::DrawItemWithBitmap(HDC hdcDest, CRect& rc, HBITMAP hBitMap, UINT iBitMapHeight, UINT cx)
{
	if (hdcDest == NULL || hBitMap == NULL)
	{
		return;
	}

	HDC hdcSrc = CreateCompatibleDC(hdcDest);
	HGDIOBJ hOldGdiObj = SelectObject(hdcSrc, hBitMap);

	TransparentBlt(hdcDest, 0, 0, CUISkinManager::m_iButtonImageLeftPartWidth, rc.Height()
		, hdcSrc, cx, 0, CUISkinManager::m_iButtonImageLeftPartWidth, iBitMapHeight
		, CUISkinManager::TRANSPARENT_COLOR);
	TransparentBlt(hdcDest, CUISkinManager::m_iButtonImageLeftPartWidth, 0
		, rc.Width() - CUISkinManager::m_iButtonImageLeftPartWidth + CUISkinManager::m_iButtonImageRightPartWidth, rc.Height()
		, hdcSrc, cx + CUISkinManager::m_iButtonImageLeftPartWidth, 0
		, CUISkinManager::m_iButtonImageMiddlePartWidth, iBitMapHeight
		, CUISkinManager::TRANSPARENT_COLOR);
	TransparentBlt(hdcDest
		, rc.Width() - CUISkinManager::m_iButtonImageRightPartWidth, 0
		, CUISkinManager::m_iButtonImageRightPartWidth, rc.Height()
		, hdcSrc
		, cx + CUISkinManager::m_iButtonImageLeftPartWidth + CUISkinManager::m_iButtonImageMiddlePartWidth, 0
		, CUISkinManager::m_iButtonImageRightPartWidth, iBitMapHeight
		, CUISkinManager::TRANSPARENT_COLOR);

	SelectObject(hdcSrc, hOldGdiObj);
	DeleteDC(hdcSrc);
}

// 以位图方式描绘系统控制按钮控件，要求位图宽度为128象素以上
// HDC hdcDest：目标设备环境句柄
// CRect& rc：按钮所占的区域（描绘区域）
// HBITMAP hBitMap：位图句柄
// UINT cx：从位图复制的起点位置
void CRButton::DrawSystemControlButtonWithBitmap(HDC hdcDest, CRect& rc, HBITMAP hBitMap, UINT cx)
{
	if (hdcDest == NULL || hBitMap == NULL)
	{
		return;
	}

	HDC hdcSrc = CreateCompatibleDC(hdcDest);
	HGDIOBJ hOldGdiObj = SelectObject(hdcSrc, hBitMap);

	TransparentBlt(hdcDest, 0, 0, rc.Width(), rc.Height(), hdcSrc, cx, 0
		, CUISkinManager::m_iSystemControlButtonWidth
		, CUISkinManager::m_iSystemControlButtonHeight
		, CUISkinManager::TRANSPARENT_COLOR);

	SelectObject(hdcSrc, hOldGdiObj);
	DeleteDC(hdcSrc);
}

// 描绘边框
void CRButton::DrawRoundBorder(CDC *pDC, CRect rcBorder)
{
	rcBorder.DeflateRect(0, 0, 1, 1);
	CPoint pt[] = {
		CPoint(rcBorder.left, rcBorder.top + 2),
		CPoint(rcBorder.left, rcBorder.bottom - 2),
		CPoint(rcBorder.left + 1, rcBorder.bottom - 1),
		CPoint(rcBorder.left + 2, rcBorder.bottom),
		CPoint(rcBorder.right - 2, rcBorder.bottom),
		CPoint(rcBorder.right - 1, rcBorder.bottom - 1),
		CPoint(rcBorder.right, rcBorder.bottom - 2),
		CPoint(rcBorder.right, rcBorder.top + 2),
		CPoint(rcBorder.right - 1, rcBorder.top + 1),
		CPoint(rcBorder.right - 2, rcBorder.top),
		CPoint(rcBorder.left + 2, rcBorder.top),
		CPoint(rcBorder.left + 1, rcBorder.top + 1),
		CPoint(rcBorder.left, rcBorder.top + 2)
	};

	BYTE bt[] = {
		PT_MOVETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO
	};

	pDC->PolyDraw(pt, bt, 13);
}
