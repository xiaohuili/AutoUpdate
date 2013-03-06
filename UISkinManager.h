//------------------------------------------------------------------------------
// 文件名称：UISkinManager.h
// 文件版本：v1.0
// 创建日期：2006-02-14 16:59
// 作    者：Richard Ree
// 文件描述：界面管理类，提供实现自绘界面的公用函数以及风格设置和切换
//------------------------------------------------------------------------------

#pragma once

#ifndef UISKINMANAGER_H
#define UISKINMANAGER_H

class CUISkinManager
{
public:
	CUISkinManager();
	~CUISkinManager();

	static BOOL Init();
	static void Uninit();
	static BOOL LoadImages();
	static HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	static COLORREF GetColor();
	static HBRUSH GetBrush();

	// 以渐变方式填充矩形
	static void GradientFillRect(CWnd* pWnd, CDC* pDC, CRect& rcFill, ULONG nMode, COLORREF crLeftTop, COLORREF crRightBottom);

	// 透明拉伸复制
	static void TransparentBlt(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest, HDC hdcSrc, int nXSrc, int nYSrc, int nWidthSrc,int nHeightSrc, COLORREF crTransparent);

	// 绘制对话框
	static void Paint(CWnd* pWnd, CDC* pDC = NULL, BOOL bFillClient = TRUE, COLORREF crFillColor = RGB(255, 255, 255));
	static void Paint_Normal(CWnd* pWnd, CDC* pDC = NULL, BOOL bFillClient = TRUE, COLORREF crFillColor = RGB(255, 255, 255));
	static void Paint_XP(CWnd* pWnd, CDC* pDC = NULL, BOOL bFillClient = TRUE, COLORREF crFillColor = RGB(255, 255, 255));
	static void PaintDialogBorder(CWnd* pWnd, CDC* pDC = NULL);

	// 设置对话框边框
	static void SetRgn(CWnd* pWnd);
	static void SetRgn_Normal(CWnd* pWnd);
	static void SetRgn_XP(CWnd* pWnd);

public:
	// 界面风格枚举
	enum SkinStyle
	{
		StyleNormal = 0,
		StyleSpring,
		StyleSummer,
		StyleAutumn,
		StyleWinter,
		StyleXP,
		StyleCustomize,
		SkinStyleBottom
	};
	// 切换界面风格
	static void ChangeStyle(enum SkinStyle iNewStyle);

public:
	// 透明色
	static const COLORREF TRANSPARENT_COLOR;

	// 当前界面风格
	static enum SkinStyle m_iCurrentStyle;

	// 窗口标题栏风格

	// 标题栏高度
	static UINT m_iTitleBarHeight;
	// 绘制标题栏所需的图像宽度（象素）
	static UINT m_iTitleBarImageWidth;
	// 标题栏图象左边部分的宽度
	static UINT m_iTitleBarImageLeftPartWidth;
	// 标题栏图象中间部分的宽度（此部分是可拉伸的）
	static UINT m_iTitleBarImageMiddlePartWidth;
	// 标题栏图象右边部分的宽度
	static UINT m_iTitleBarImageRightPartWidth;
	// 绘制标题栏所需的图像集（每个图像宽度为 m_iTitleBarImageWidth 象素，按正常、失去焦点的顺序排列，总宽度不小于 2 * m_iTitleBarImageWidth 象素）
	static CBitmap m_bmTitleBarImage;

	// 对话框窗口左右边距
	static UINT m_iLeftMargin;
	// 对话框窗口上下边距
	static UINT m_iTopMargin;

	// 按钮风格

	// 按钮高度（绘制按钮所需的图像高度）
	static UINT m_iButtonHeight;
	// 绘制按钮所需的图像宽度（象素）
	static UINT m_iButtonImageWidth;
	// 按钮图象左边部分的宽度
	static UINT m_iButtonImageLeftPartWidth;
	// 按钮图象中间部分的宽度（此部分是可拉伸的）
	static UINT m_iButtonImageMiddlePartWidth;
	// 按钮图象右边部分的宽度
	static UINT m_iButtonImageRightPartWidth;
	// 绘制按钮所需的图像集（每个图像宽度为 m_iButtonImageWidth 象素，按正常、缺省、鼠标悬停、鼠标按下的顺序排列，总宽度不小于 4 * m_iButtonImageWidth 象素）
	static CBitmap m_bmButtonImage;
	// 系统控制按钮的宽度和高度（象素）
	static UINT m_iSystemControlButtonWidth;
	static UINT m_iSystemControlButtonHeight;
	// 绘制系统控制按钮（最小化、最大化、关闭等按钮）所需的图像集（每个图像宽度为 m_iSystemControlButtonImageWidth 象素，按最小化、最大化、恢复、关闭、按钮按下状态的顺序排列，总宽度不小于 8 * m_iSystemControlButtonImageWidth 象素）
	static CBitmap m_bmSystemControlButtonImage;

	// 绘制控件的画笔
	static HBRUSH m_hbr;
};

#endif // #ifndef UISKINMANAGER_H
