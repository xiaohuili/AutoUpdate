//------------------------------------------------------------------------------
// 文件名称：RButton.h
// 文件版本：v1.0
// 创建日期：2006-05-06 10:39
// 作    者：Richard Ree
// 文件描述：自绘按钮类
//------------------------------------------------------------------------------

#pragma once

#ifndef RBUTTON_H
#define RBUTTON_H

// CRButton

class CRButton : public CButton
{
public:
	enum enButtonType
	{
		ButtonTypeNormal = 0,
		ButtonTypeClose,
		ButtonTypeMaximize,
		ButtonTypeMinimize,
		ButtonTypeBottom
	};

	DECLARE_DYNAMIC(CRButton)

public:
	CRButton(enButtonType m_iButtonType = ButtonTypeNormal);
	virtual ~CRButton();

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

protected:
	void DrawRoundBorder(CDC *pDC, CRect rcBorder);
	// 以位图方式描绘按钮控件，要求位图宽度为32象素（左15 + 中2 + 右15，中间随着控件宽度拉伸）
	void DrawItemWithBitmap(HDC hdcDest, CRect& rc, HBITMAP hBitMap, UINT iBitMapHeight, UINT cx);
	// 以位图方式描绘系统控制按钮控件，要求位图宽度为128象素以上
	void DrawSystemControlButtonWithBitmap(HDC hdcDest, CRect& rc, HBITMAP hBitMap, UINT cx);

protected:
	enum enButtonType m_iButtonType;
	BOOL m_bTracking;
	BOOL m_bDefault;
	BOOL m_bFocus;
	BOOL m_bOver;
	BOOL m_bDown;
};

#endif // #ifndef RBUTTON_H
