//------------------------------------------------------------------------------
// 文件名称：RMessageBox.cpp
// 文件版本：v1.0
// 创建日期：2006-05-06 04:25
// 作    者：Richard Ree
// 文件描述：自定义消息框类实现文件
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AutoUpdate.h"
#include "RMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CRMessageBox 对话框

IMPLEMENT_DYNAMIC(CRMessageBox, CDialog)

CRMessageBox::CRMessageBox(CWnd* pParent /*=NULL*/)
	: CRDialog(CRMessageBox::IDD, pParent)
{
	m_iID = MB_OK;
	m_bOption1 = FALSE;
	m_bOption2 = FALSE;
}

CRMessageBox::~CRMessageBox()
{
}

void CRMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ICON, m_stcIcon);
	DDX_Control(pDX, IDC_STATIC_PROMPT, m_stcPromptMessage);
	DDX_Control(pDX, IDC_CHECK_OPTION1, m_chkOption1);
	DDX_Check(pDX, IDC_CHECK_OPTION1, m_bOption1);
	DDX_Control(pDX, IDC_CHECK_OPTION2, m_chkOption2);
	DDX_Check(pDX, IDC_CHECK_OPTION2, m_bOption2);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOK);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_ABORT, m_btnAbort);
	DDX_Control(pDX, IDC_BUTTON_IGANORE, m_btnIganore);
	DDX_Control(pDX, IDC_BUTTON_RETRY, m_btnRetry);
	DDX_Control(pDX, IDC_BUTTON_CONTINUE, m_btnContinue);
	DDX_Control(pDX, IDC_BUTTON_YES, m_btnYes);
	DDX_Control(pDX, IDC_BUTTON_NO, m_btnNo);
}

BOOL CRMessageBox::OnInitDialog()
{
	CRDialog::OnInitDialog();

	// 设置对话框标题
	SetWindowText(m_sTitle);

	m_btnOK.SetWindowText(STRING(STR_BUTTON_OK, "确定(&O)"));
	m_btnCancel.SetWindowText(STRING(STR_BUTTON_CANCEL, "取消(&C)"));
	m_btnAbort.SetWindowText(STRING(STR_BUTTON_ABORT, "跳出(&A)"));
	m_btnIganore.SetWindowText(STRING(STR_BUTTON_IGANORE, "忽略(&I)"));
	m_btnRetry.SetWindowText(STRING(STR_BUTTON_RETRY, "重试(&R)"));
	m_btnContinue.SetWindowText(STRING(STR_BUTTON_CONTINUE, "继续(&C)"));
	m_btnYes.SetWindowText(STRING(STR_BUTTON_YES, "是(&Y)"));
	m_btnNo.SetWindowText(STRING(STR_BUTTON_NO, "否(&N)"));

	// 判断是否需要显示复选框
	if (!m_sOptionPromptMessage1.IsEmpty())
	{
		m_chkOption1.SetWindowText(m_sOptionPromptMessage1);
		m_chkOption1.ShowWindow(SW_SHOW);
	}
	else
	{
		m_chkOption1.ShowWindow(SW_HIDE);
	}
	if (!m_sOptionPromptMessage2.IsEmpty())
	{
		m_chkOption2.SetWindowText(m_sOptionPromptMessage2);
		m_chkOption2.ShowWindow(SW_SHOW);
	}
	else
	{
		m_chkOption2.ShowWindow(SW_HIDE);
	}

	// 图标控件仅作为占位符来用，不显示
	m_stcIcon.ShowWindow(SW_HIDE);

	UpdateData(FALSE);

	// 重置对话框大小、按钮位置
	ResetMessageBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRMessageBox::OnOK()
{
	CWnd *pActiveControl = GetFocus();
	if (*pActiveControl == m_btnOK)
	{
		OnBnClickedOk();
	}
	else if (*pActiveControl == m_btnCancel)
	{
		OnBnClickedCancel();
	}
	else if (*pActiveControl == m_btnAbort)
	{
		OnBnClickedAbort();
	}
	else if (*pActiveControl == m_btnIganore)
	{
		OnBnClickedIganore();
	}
	else if (*pActiveControl == m_btnRetry)
	{
		OnBnClickedRetry();
	}
	else if (*pActiveControl == m_btnContinue)
	{
		OnBnClickedContinue();
	}
	else if (*pActiveControl == m_btnYes)
	{
		OnBnClickedYes();
	}
	else if (*pActiveControl == m_btnNo)
	{
		OnBnClickedNo();
	}

//	CDialog::OnOK();
}

void CRMessageBox::OnCancel()
{
	// 根据消息框样式对按下ESC键及关闭对话框按钮操作的结果进行修正
	switch (m_iType & 0xF)
	{
	case MB_OK:
		m_iID = IDOK;
		break;
	case MB_OKCANCEL:
		m_iID = IDCANCEL;
		break;
	case MB_YESNO:
		m_iID = IDNO;
		break;
	case MB_YESNOCANCEL:
		m_iID = IDCANCEL;
		break;
	case MB_RETRYCANCEL:
		m_iID = IDCANCEL;
		break;
//	case MB_CANCELTRYCONTINUE:
//		m_iID = IDCANCEL;
//		break;
	case MB_ABORTRETRYIGNORE:
		m_iID = IDABORT;
		break;
	default:
		m_iID = IDCANCEL;
		break;
	}
	CDialog::OnCancel();
}


BEGIN_MESSAGE_MAP(CRMessageBox, CDialog)
	RDIALOG_UI_MESSAGE_MAP
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ABORT, OnBnClickedAbort)
	ON_BN_CLICKED(IDC_BUTTON_IGANORE, OnBnClickedIganore)
	ON_BN_CLICKED(IDC_BUTTON_RETRY, OnBnClickedRetry)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, OnBnClickedContinue)
	ON_BN_CLICKED(IDC_BUTTON_YES, OnBnClickedYes)
	ON_BN_CLICKED(IDC_BUTTON_NO, OnBnClickedNo)
END_MESSAGE_MAP()

// CRMessageBox 消息处理程序

void CRMessageBox::OnPaint()
{
	if (!IsWindowVisible() || IsIconic())
	{
		CDialog::OnPaint();
		return;
	}

	// 绘制窗口
	CRDialog::OnPaint();

	// 显示图标
	HICON hIcon;
	CRect rcIcon;
	POINT point;

	m_stcIcon.GetWindowRect(&rcIcon);
	ScreenToClient(&rcIcon);
	point.x = rcIcon.left;
	point.y = rcIcon.top;

	// 根据消息框类型加载不同的图标
	switch (m_iType & 0xF0)
	{
	case MB_ICONHAND: // MB_ICONERROR/MB_ICONSTOP
		hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_HAND));
		break;
	case MB_ICONQUESTION:
		hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_QUESTION));
		break;
	case MB_ICONEXCLAMATION: // MB_ICONWARNING
		hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_WARNING));
		break;
	case MB_ICONINFORMATION: // MB_ICONASTERISK
		hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_INFORMATION));
		break;
	case MB_USERICON:
	default:
		hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_INFORMATION));
		break;
	}

	// 画图标
	CDC *pDC = GetDC();
	pDC->DrawIcon(point, hIcon);
	ReleaseDC(pDC);
}

void CRMessageBox::OnBnClickedOk()
{
	m_iID = IDOK;

	// 隐藏窗口
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedCancel()
{
	m_iID = IDCANCEL;

	// 隐藏窗口
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedAbort()
{
	m_iID = IDABORT;

	// 隐藏窗口
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedIganore()
{
	m_iID = IDIGNORE;

	// 隐藏窗口
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedRetry()
{
	m_iID = IDRETRY;

	// 隐藏窗口
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedContinue()
{
//	m_iID = IDCONTINUE;

	// 隐藏窗口
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedYes()
{
	m_iID = IDYES;

	// 隐藏窗口
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedNo()
{
	m_iID = IDNO;

	// 隐藏窗口
	CDialog::OnOK();
}

// 重置消息框样式和外观
void CRMessageBox::ResetMessageBox()
{
	// 根据信息的多少重置对话框窗口大小
	CRect rcWnd;
	CRect rcPromptMessage;
	CRect rcButton;
	CSize size;
	CDC* pDC;
	int iOffsetY; // 按钮控件需要在垂直方向上的移位距离

	GetWindowRect(&rcWnd);

	// 根据提示文字的多少计算窗口大小
	pDC = m_stcPromptMessage.GetDC();
	size = pDC->GetTextExtent(m_sPromptMessage);
	m_stcPromptMessage.ReleaseDC(pDC);
	if (size.cx < 200)
	{
		size.cx = 200;
	}
	else if (size.cx > 500)
	{
		size.cy = size.cx / 500 * size.cy;
		size.cx = 500;
	}
	if (size.cy < 32)
	{
		size.cy = 32;
	}
	rcWnd.right = rcWnd.left + size.cx + 100;
	rcWnd.bottom = rcWnd.top + size.cy + 155;
	// 如果要显示复选框，要加上复选框的高度
	if (!m_sOptionPromptMessage1.IsEmpty())
	{
		rcWnd.bottom += 18;
	}
	if (!m_sOptionPromptMessage2.IsEmpty())
	{
		rcWnd.bottom += 18;
	}

	// 将窗口移动到屏幕中间
	CRect rcScreen;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcScreen, 0); // 取得屏幕大小
	rcWnd.OffsetRect(rcScreen.CenterPoint() - rcWnd.CenterPoint());

	MoveWindow(&rcWnd);

	// 重置提示信息标签的大小
	m_stcPromptMessage.GetWindowRect(&rcPromptMessage);
	ScreenToClient(&rcPromptMessage);
	rcPromptMessage.right = rcPromptMessage.left + size.cx;
	rcPromptMessage.bottom = rcPromptMessage.top + size.cy + 5;
	m_stcPromptMessage.MoveWindow(&rcPromptMessage);

	// 重置复选框控件的大小
	m_chkOption1.GetWindowRect(&rcPromptMessage);
	ScreenToClient(&rcPromptMessage);
	rcPromptMessage.right = rcPromptMessage.left + size.cx;
	m_chkOption1.MoveWindow(&rcPromptMessage);
	m_chkOption2.GetWindowRect(&rcPromptMessage);
	ScreenToClient(&rcPromptMessage);
	rcPromptMessage.right = rcPromptMessage.left + size.cx;
	m_chkOption2.MoveWindow(&rcPromptMessage);

	// 显示提示信息
	m_stcPromptMessage.SetWindowText(m_sPromptMessage);

	// 根据类型显示不同的按钮，并将它们移动到对话框中间
	m_btnOK.GetWindowRect(&rcButton);
	ScreenToClient(&rcButton);
	iOffsetY = rcWnd.Height() - rcButton.top - 48;
	switch (m_iType & 0xF)
	{
	case MB_OK:
		m_btnOK.ShowWindow(SW_SHOW);
		rcButton.OffsetRect((rcWnd.Width() - rcButton.Width()) / 2 - rcButton.left, iOffsetY);
		m_btnOK.MoveWindow(&rcButton);
		break;
	case MB_OKCANCEL:
		m_btnOK.ShowWindow(SW_SHOW);
		m_btnCancel.ShowWindow(SW_SHOW);
		rcButton.OffsetRect(rcWnd.Width() / 2 - rcButton.Width() - 2 - rcButton.left, iOffsetY);
		m_btnOK.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnCancel.MoveWindow(&rcButton);
		break;
	case MB_YESNO:
		m_btnYes.ShowWindow(SW_SHOW);
		m_btnNo.ShowWindow(SW_SHOW);
		rcButton.OffsetRect(rcWnd.Width() / 2 - rcButton.Width() - 2 - rcButton.left, iOffsetY);
		m_btnYes.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnNo.MoveWindow(&rcButton);
		break;
	case MB_YESNOCANCEL:
		m_btnYes.ShowWindow(SW_SHOW);
		m_btnNo.ShowWindow(SW_SHOW);
		m_btnCancel.ShowWindow(SW_SHOW);
		rcButton.OffsetRect((rcWnd.Width() - rcButton.Width()) / 2 - 2 - rcButton.Width() - rcButton.left, iOffsetY);
		m_btnYes.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnNo.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnCancel.MoveWindow(&rcButton);
		break;
	case MB_RETRYCANCEL:
		m_btnRetry.ShowWindow(SW_SHOW);
		m_btnCancel.ShowWindow(SW_SHOW);
		rcButton.OffsetRect(rcWnd.Width() / 2 - rcButton.Width() - 2 - rcButton.left, iOffsetY);
		m_btnRetry.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnCancel.MoveWindow(&rcButton);
		break;
//	case MB_CANCELTRYCONTINUE:
//		m_btnCancel.ShowWindow(SW_SHOW);
//		m_btnContinue.ShowWindow(SW_SHOW);
//		rcButton.OffsetRect(rcWnd.Width() / 2 - rcButton.Width() - 2 - rcButton.left, iOffsetY);
//		m_btnCancel.MoveWindow(&rcButton);
//		rcButton.OffsetRect(rcButton.Width() + 4, 0);
//		m_btnContinue.MoveWindow(&rcButton);
//		break;
	case MB_ABORTRETRYIGNORE:
		m_btnAbort.ShowWindow(SW_SHOW);
		m_btnRetry.ShowWindow(SW_SHOW);
		m_btnIganore.ShowWindow(SW_SHOW);
		rcButton.OffsetRect((rcWnd.Width() - rcButton.Width()) / 2 - 2 - rcButton.Width() - rcButton.left, iOffsetY);
		m_btnAbort.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnRetry.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnIganore.MoveWindow(&rcButton);
		break;
	default:
		break;
	}

	// 图标的显示在OnPaint函数中实现
}
