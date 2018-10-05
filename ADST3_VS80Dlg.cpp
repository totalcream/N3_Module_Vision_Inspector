// ADST3_VS80Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ADST3_VS80.h"
#include "ADST3_VS80Dlg.h"

#include "include\AXL.h"
#include "include\AXA.h"
#include "include\AXHS.h"
#include "tmctl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "lib/AXL.lib")
#pragma comment(lib, "lib/tmctl.lib")

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CADST3_VS80Dlg dialog

double m_dFlukeNPLC[5]	= {0.02, 0.2, 1, 10, 100};


CADST3_VS80Dlg::CADST3_VS80Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CADST3_VS80Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_lModuleNo		= 0;
	m_bDlgClosed	= FALSE;
	m_bFlukeInit	= FALSE;
	m_bYKGWInit		= FALSE;
	m_lModuleCnt	= 0;
	m_SocketDev.hSocket = NULL;
	m_YKGWDevId		= NULL;
	m_bFlukeInit	= FALSE;
	m_bYKGWInit		= FALSE;
	m_dNPLC			= m_dFlukeNPLC[3];
	m_nSocketTimeout = 0;

	for (INT i=0; i<MAX_AI; i++)
		m_bAIChStarted[i]  = FALSE;

	for (INT i=0; i<MAX_AO; i++)
		m_bAOChStarted[i]  = FALSE;

	memset(m_StartCnt, 0, sizeof(m_StartCnt));
	memset(&m_aioTarget, 0, sizeof(m_aioTarget));
}

void CADST3_VS80Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_lcLog);
	DDX_Control(pDX, IDC_EDIT_PRODUCT_SEL, m_edtProductSel);
	DDX_Control(pDX, IDC_BTN_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BTN_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_CHK_INP_MODE, m_chkInpMode);
	DDX_Control(pDX, IDC_STT_PRODUCT_NAME, m_sttProductName);
	DDX_Control(pDX, IDC_BTN_YKGW_SETTING, m_btnYKGWSetting);
	DDX_Control(pDX, IDC_BTN_FLUKE_SETTING, m_btnFlukeSetting);
	DDX_Control(pDX, IDC_STT_YKGW, m_sttYKGWSetting);
	DDX_Control(pDX, IDC_STT_FLUKE, m_sttFlukeSetting);
	DDX_Control(pDX, IDC_BTN_YKGW_FILE_OPEN, m_btnYKGWFileOpen);
	DDX_Control(pDX, IDC_BTN_FLUKE_FILE_OPEN, m_btnFlukeFileOpen);
	DDX_Control(pDX, IDC_BTN_PROD_FILE_OPEN, m_btnProdFileOpen);
	DDX_Control(pDX, IDC_BTN_CLRLOG, m_btnClrLog);
	DDX_Control(pDX, IDC_STT_AI, m_sttAIG);
	DDX_Control(pDX, IDC_STT_AO, m_sttAOG);
	DDX_Control(pDX, IDC_SPIN_INPBYTE, m_spnInpByte);
	DDX_Control(pDX, IDC_SPIN_OUTPBYTE, m_spnOutpByte);
	DDX_Control(pDX, IDC_BTN_AUTORUN, m_autorun);
	DDX_Control(pDX, IDC_BUTTON_STOP_AUTOTEST, m_autorunstop);
}

BEGIN_MESSAGE_MAP(CADST3_VS80Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_LOG, &CADST3_VS80Dlg::OnNMCustomdrawListLog)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_EXIT, &CADST3_VS80Dlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_CHK_INP_MODE, &CADST3_VS80Dlg::OnBnClickedChkInpMode)
	ON_BN_CLICKED(IDC_BTN_PREV, &CADST3_VS80Dlg::OnBnClickedBtnPrev)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CADST3_VS80Dlg::OnBnClickedBtnNext)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_OUT0, IDC_BTN_OUT7, OnBnClickedBtnOut)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_IN0, IDC_BTN_IN7, OnBnClickedBtnIn)
	ON_BN_CLICKED(IDC_BTN_YKGW_SETTING, &CADST3_VS80Dlg::OnBnClickedBtnYkgwSetting)
	ON_BN_CLICKED(IDC_BTN_FLUKE_SETTING, &CADST3_VS80Dlg::OnBnClickedBtnFlukeSetting)
	ON_BN_CLICKED(IDC_BTN_YKGW_FILE_OPEN, &CADST3_VS80Dlg::OnBnClickedBtnYkgwFileOpen)
	ON_BN_CLICKED(IDC_BTN_FLUKE_FILE_OPEN, &CADST3_VS80Dlg::OnBnClickedBtnFlukeFileOpen)
	ON_BN_CLICKED(IDC_BTN_PROD_FILE_OPEN, &CADST3_VS80Dlg::OnBnClickedBtnProdFileOpen)
	ON_BN_CLICKED(IDC_BTN_CLRLOG, &CADST3_VS80Dlg::OnBnClickedBtnClrlog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INPBYTE, &CADST3_VS80Dlg::OnDeltaposSpinInpbyte)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_OUTPBYTE, &CADST3_VS80Dlg::OnDeltaposSpinOutpbyte)
	ON_BN_CLICKED(IDC_BTN_AUTORUN, &CADST3_VS80Dlg::OnBnClickedBtnAutorun)
	ON_BN_CLICKED(IDC_BUTTON_STOP_AUTOTEST, &CADST3_VS80Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CADST3_VS80Dlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_SEND2, &CADST3_VS80Dlg::OnBnClickedButtonSend2)
	ON_BN_CLICKED(IDC_BUTTON_SEND4, &CADST3_VS80Dlg::OnBnClickedButtonSend4)
	ON_BN_CLICKED(IDC_BUTTON_SEND3, &CADST3_VS80Dlg::OnBnClickedButtonSend3)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CADST3_VS80Dlg::OnBnClickedButtonConnect)
END_MESSAGE_MAP()


// CADST3_VS80Dlg message handlers

BOOL CADST3_VS80Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CtrlsInit();
	
	//int i = 1;
	//if (i % 2)
	//	AfxMessageBox(_T("참"));
	//else
	//	AfxMessageBox(_T("부"));

	UpdateMsg(_T("모든 테스트가 종료되었습니다."), MT_RSLT);
	UpdateMsg(_T("로그 및 성적서가 저장되었습니다."), MT_RSLT);
	if (TargetInit() != AXT_RT_SUCCESS)
	{
		UpdateMsg(_T("Target Init fail!"), MT_ERR);
		return FALSE;
	}

	else
		m_edtProductSel.EnableWindow(TRUE);

	QueryPerformanceFrequency(&m_PerfFreq);
	AIOParamLoad();
	DeviceInit();
	CtrlsEnable(TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CADST3_VS80Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CADST3_VS80Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CADST3_VS80Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CADST3_VS80Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_EDIT)
	{
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_EDIT_PRODUCT_SEL:
			pDC->SetTextColor(CR_BLUE);
			break;
		default:
			break;
		}
	}

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_STT_PRODUCT_NAME:
			pDC->SetTextColor(CR_LIME);
			pDC->SetBkColor(BK_COLOR);
			break;

		case IDC_STT_AI:
			//SetWindowTheme(GetDlgItem(IDC_STT_AI)->GetSafeHwnd(), L"", L"");
			pDC->SetTextColor(CR_WHITE);
			pDC->SetBkMode(TRANSPARENT);
			//hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
			//return CreateSolidBrush(BK_COLOR);
			break;
		case IDC_STT_AO:
			//SetWindowTheme(GetDlgItem(IDC_STT_AO)->GetSafeHwnd(), L"", L"");
			pDC->SetTextColor(CR_WHITE);
			pDC->SetBkMode(TRANSPARENT);
			//hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
			//return CreateSolidBrush(BK_COLOR);
			//HBRUSH hBGBrush = CreateSolidBrush(/*RGB(128, 184, 223)*/ BK_COLOR);
			//return hBGBrush;

			break;
		default:
			break;
		}
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CADST3_VS80Dlg::OnClose()
{
	for (int i=ID_TMR_0; i<ID_TMR_TOTAL; i++)
		KillTimer(i);
	Sleep(100);

	CloseSockets();
	//disconnect();	//2018-07-17 Arduino COM통신 Disconnect

	AxlClose();
	m_bDlgClosed = TRUE;
	CDialog::OnClose();
}

BOOL CADST3_VS80Dlg::OnEraseBkgnd(CDC* pDC)
{
	// Set brush to desired background color
	CBrush backBrush(BK_COLOR);

	// Save old brush
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);

	CRect rect;
	pDC->GetClipBox(&rect);     // Erase the area needed

	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),
		PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CADST3_VS80Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN))
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CADST3_VS80Dlg::OnNMCustomdrawListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we 
	// set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's pre-paint
	// stage, then tell Windows we want messages for every item.

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
		*pResult = CDRF_NOTIFYITEMDRAW;
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		// This is the pre-paint stage for an item. Here's where we set the
		// item's text color. Our return value will tell Windows to draw the
		// item itself, but it will use the new color we set here.
		// We'll cycle the colors through red, green, and light blue.

		COLORREF crText;

		CString str;
		str = m_lcLog.GetItemText(pLVCD->nmcd.dwItemSpec, 1);
		if (str == "Message")
			crText = RGB(0,0,255);
		else if (str == "Error")
			crText = RGB(255,0,0);
		else if (str == "Warning")
			crText = RGB(255,127,0);
		else
			crText = RGB(0,0,0);

		// Store the color back in the NMLVCUSTOMDRAW struct.
		pLVCD->clrText = crText;

		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;
	}

	//	*pResult = 0;
}

void CADST3_VS80Dlg::UpdateMsg(CString strMsg, int nType)
{
	CString strType;
	CString strTime;
	COleDateTime CurTime = COleDateTime::GetCurrentTime();
	strTime = CurTime.Format(_T("%y/%m/%d %H:%M:%S"));
	int	nIndex = m_lcLog.GetItemCount();

	switch (nType)
	{
	case MT_RSLT:	strType.Format(_T("Result"));	break;
	case MT_ERR:	strType.Format(_T("Error"));	break;
	case MT_MSG:	strType.Format(_T("Message"));	break;
	case MT_WRN:	strType.Format(_T("Warning"));	break;
	default:		strType.Format(_T("N/A"));		break;
	}

	m_lcLog.InsertItem(nIndex, strTime);
	m_lcLog.SetItemText(nIndex, 1, strType);
	m_lcLog.SetItemText(nIndex, 2, strMsg);

	int nTopIndex = m_lcLog.GetTopIndex();
	int nCntPerPage = m_lcLog.GetCountPerPage();
	if ((nIndex - m_lcLog.GetCountPerPage()) >= nTopIndex)
		m_lcLog.EnsureVisible(nIndex, TRUE);

}

void CADST3_VS80Dlg::CtrlsInit()
{
	int i;
	CRect rect;
	m_lcLog.GetWindowRect(rect);

	m_lcLog.SetExtendedStyle(m_lcLog.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_lcLog.InsertColumn(0, _T(""),	LVCFMT_CENTER, 0);
	m_lcLog.InsertColumn(1, _T("Log Time"),	LVCFMT_CENTER, 125);
	m_lcLog.InsertColumn(2, _T("Type"),	LVCFMT_CENTER, 65);
	m_lcLog.InsertColumn(3, _T("Message"),	LVCFMT_LEFT, rect.Width()-195);
	m_lcLog.DeleteColumn(0);

	CFont* pSttFontL = new CFont;
	pSttFontL->CreatePointFont(150, _T("Consolas Bold"), 0);

	CFont* pSttFontM = new CFont;
	pSttFontM->CreatePointFont(120, _T("Consolas Bold"), 0);

	CFont* pSttFontS = new CFont;
	pSttFontS->CreatePointFont(105, _T("Consolas Bold"), 0);

	CFont* pSttFontSS = new CFont;
	pSttFontSS->CreatePointFont(90, _T("Consolas Bold"), 0);

	m_edtProductSel.SetFont(pSttFontL);
	m_sttProductName.SetFont(pSttFontM);

	m_btnPrev.SetFont(pSttFontM);
// 	m_btnPrev.SetTextColor(CR_BLUE);
// 	m_btnPrev.SetTextHotColor(CR_GREEN);
// 	m_btnPrev.SetMouseCursorHand();
// 	m_btnPrev.SetTooltip(_T("Prev Product"));

	m_btnNext.SetFont(pSttFontM);
//	m_btnNext.SetTextColor(CR_BLUE);
// 	m_btnNext.SetTextHotColor(CR_GREEN);
// 	m_btnNext.SetMouseCursorHand();
// 	m_btnNext.SetTooltip(_T("Next Product"));

	m_chkInpMode.SetFont(pSttFontS);
	m_btnYKGWSetting.SetFont(pSttFontS);
	m_btnFlukeSetting.SetFont(pSttFontS);

// 	m_btnYKGWFileOpen.SetMouseCursorHand();
// 	m_btnYKGWFileOpen.SetTooltip(_T("Open Yokogawa Config File"));
// 	m_btnYKGWFileOpen.SetImage(IDB_BMP_FILE_OPEN);
// 
// 	m_btnFlukeFileOpen.SetMouseCursorHand();
// 	m_btnFlukeFileOpen.SetTooltip(_T("Open Fluke Config File"));
// 	m_btnFlukeFileOpen.SetImage(IDB_BMP_FILE_OPEN);
// 
// 	m_btnProdFileOpen.SetMouseCursorHand();
// 	m_btnProdFileOpen.SetTooltip(_T("Open Product Config File"));
// 	m_btnProdFileOpen.SetImage(IDB_BMP_FILE_OPEN);

	m_sttYKGWSetting.SetFont(pSttFontL);
	m_sttYKGWSetting.SetTextColor(CR_WHITE);
	m_sttYKGWSetting.SetBkColor(CR_BLUE);

	m_sttFlukeSetting.SetFont(pSttFontL);
	m_sttFlukeSetting.SetTextColor(CR_WHITE);
	m_sttFlukeSetting.SetBkColor(CR_BLUE);

	for (i=0; i<MAX_AI; i++)
	{
		m_sttAI[i].SubclassDlgItem(IDC_STT_IN0 + i, this);
		m_sttAI[i].SetFont(pSttFontL);
		m_sttAI[i].SetTextColor(RGB(250, 250, 250));
		m_sttAI[i].SetBkColor(CL_READY);
		m_sttAI[i].SetBlinkBkColors(CR_ORANGE, CR_DORANGE);
		m_sttAI[i].ShowWindow(SW_HIDE);

		m_AICh[i].SubclassDlgItem(IDC_STT_INCH0 + i, this);
		m_AICh[i].SetFont(pSttFontM);
		m_AICh[i].EnableWindow(TRUE);
		m_AICh[i].SetTextColor(CR_GREEN);
		m_AICh[i].SetBkColor(/*RGB(250, 0, 0)*/BK_COLOR);
		m_AICh[i].ShowWindow(SW_HIDE);

		m_btnAI[i].SubclassDlgItem(IDC_BTN_IN0 + i, this);
		m_btnAI[i].ShowWindow(SW_HIDE);

		m_btnAI[i].SetBitmaps(IDB_BMP_BTN_IDLE, MASK_COLOR, (int)BTNST_AUTO_DARKER);
		m_btnAI[i].SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
		m_btnAI[i].SetPressedStyle(CButtonST::BTNST_PRESSED_LEFTRIGHT, FALSE);
		m_btnAI[i].SizeToContent();
		m_btnAI[i].DrawBorder(FALSE, FALSE);
		m_btnAI[i].DrawTransparent();
	}

	for (i=0; i<MAX_AO; i++)
	{
		m_sttAO[i].SubclassDlgItem(IDC_STT_OUT0 + i, this);
		m_sttAO[i].SetFont(pSttFontL);
		m_sttAO[i].SetTextColor(CR_WHITE);
		m_sttAO[i].SetBkColor(CL_READY);
		m_sttAO[i].SetBlinkBkColors(CR_ORANGE, CR_DORANGE);
		m_sttAO[i].ShowWindow(SW_HIDE);

		m_AOCh[i].SubclassDlgItem(IDC_STT_OUTCH0 + i, this);
		m_AOCh[i].SetFont(pSttFontM);
		m_AOCh[i].EnableWindow(TRUE);
		m_AOCh[i].SetTextColor(CR_GREEN);
		m_AOCh[i].SetBkColor(BK_COLOR);
		m_AOCh[i].ShowWindow(SW_HIDE);

		m_btnAO[i].SubclassDlgItem(IDC_BTN_OUT0 + i, this);
		m_btnAO[i].ShowWindow(SW_HIDE);

		m_btnAO[i].SetBitmaps(IDB_BMP_BTN_IDLE, MASK_COLOR, (int)BTNST_AUTO_DARKER);
		m_btnAO[i].SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
		m_btnAO[i].SetPressedStyle(CButtonST::BTNST_PRESSED_LEFTRIGHT, FALSE);
		m_btnAO[i].SizeToContent();
		m_btnAO[i].DrawBorder(FALSE, FALSE);
		m_btnAO[i].DrawTransparent();
	}
	//m_lcLog.SetFont(pSttFontSS);

	m_btnClrLog.SetFont(pSttFontM);
//	m_btnClrLog.SetTextColor(CR_BLUE);
// 	m_btnClrLog.SetTextHotColor(CR_RED);
// 	m_btnClrLog.SetMouseCursorHand();
// 	m_btnClrLog.SetTooltip(_T("Clear Log data"));

	m_btnExit.SetFont(pSttFontM);
//	m_btnExit.SetTextColor(CR_BLUE);
// 	m_btnExit.SetTextHotColor(CR_RED);
// 	m_btnExit.SetMouseCursorHand();
// 	m_btnExit.SetTooltip(_T("Exit!"));
// 	m_btnExit.SetImage(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_NO3)));

	m_autorun.SetFont(pSttFontM);
	m_autorunstop.SetFont(pSttFontM);

}

DWORD CADST3_VS80Dlg::TargetInit()
{
	DWORD ret = AXT_RT_SUCCESS;
	DWORD dwStatus;
	CString	str;
	int i;

	if (!AxlIsOpened())
	{
		ret = AxlOpenNoReset(7);
		if (ret != AXT_RT_SUCCESS)
		{
			UpdateMsg(_T("Axl Library open fail!..."), MT_ERR);
			return ret;
		}
	}

	if (AxaInfoIsAIOModule(&dwStatus) != AXT_RT_SUCCESS)
		return -1;

	if (dwStatus  == STATUS_EXIST)
		UpdateMsg(_T("AIO modules exist!"), MT_MSG);

	else
	{
		UpdateMsg(_T("No AIO modules detected!"), MT_ERR);
		return -1;
	}

	if (AxaInfoGetModuleCount(&m_lModuleCnt) != AXT_RT_SUCCESS)
		return -1;

	long	lBoardNo, lModulePos;
	//DWORD	dwModuleID;

	for (i=0; i<m_lModuleCnt; i++)
	{
		ret = AxaInfoGetModule(i, &lBoardNo, &lModulePos, &m_dwModuleID[i]);
		if (ret == AXT_RT_SUCCESS)
		{
			AxaInfoGetModuleStatus(i);
			//switch (m_dwModuleID[i])
			//{
			//case AXT_SIO_AI8AO4F:
			//	m_strModuleName[i].Format(_T("AXT_SIO_AI8AO4F"));
			//	break;
			//
			//case AXT_SIO_AI8F:
			//	m_strModuleName[i].Format(_T("AXT_SIO_AI8F"));
			//	break;
			//
			//case AXT_SIO_AO4F:
			//	m_strModuleName[i].Format(_T("AXT_SIO_AO4F"));
			//	break;
			//
			//case AXT_SIO_RAI16MLIII:
			//	m_strModuleName[i].Format(_T("AXT_SIO_AI16MLIII"));
			//	break;
			//
			//case AXT_SIO_RAO8MLIII:
			//	m_strModuleName[i].Format(_T("AXT_SIO_AO8MLIII"));
			//	break;
			//
			//case AXT_SIO_RAI16RTEX:
			//	m_strModuleName[i].Format(_T("AXT_SIO_AI16RTEX"));
			//	break;
			//
			//case AXT_SIO_RAO08RTEX:
			//	m_strModuleName[i].Format(_T("AXT_SIO_AO08RTEX"));
			//	break;
			//
			//case AXT_SIO_RAI8RB:	// AI 8Ch, 16 bit, For RTEX only  
			//	m_strModuleName[i].Format(_T("AXT_SIO_RAI8RB"));
			//	break;
			//
			//case AXT_SIO_RAO4RB:	// AO 4Ch, 16 bit, For RTEX only
			//	m_strModuleName[i].Format(_T("AXT_SIO_RAO4RB"));
			//	break;
			//
			//default:
			//	break;
			//}
		}
		else
		{
			str.Format(_T("Get Module Info Error, Code: %d"), ret);
			UpdateMsg(str, MT_ERR);
		}
	}
	m_lModuleNo = 0;
	//AxaInfoGetModule(m_lModuleNo, &lBoardNo, &lModulePos, &dwModuleID);
	//str.Format(_T(" [B/No:%02d,M/No:%02d] %s"), lBoardNo, m_lModuleNo, m_strModuleName[m_lModuleNo]);
	//m_edtProductSel.SetWindowText(str);

	GetDlgItem(IDC_BTN_AUTORUN)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_STOP_AUTOTEST)->EnableWindow(true);

	return ret;
}

void CADST3_VS80Dlg::AIOParamLoad()
{
	INT		i;
	TCHAR	tszPath[255];
	TCHAR	tszFile[24];
	FILE	*fp;
	BOOL	bIsTarget = FALSE;

	memset(&m_aioTarget, 0, sizeof(m_aioTarget));

	GetModuleFileName(NULL, tszPath, 255);

	for (i = (INT)_tcslen(tszPath); i > 0; i--)
	{
		if (tszPath[i] == '\\')
		{
			i++;
			tszPath[i]	= '\0';
			break;
		}
	}

	CString strAIOParamDir;
	strAIOParamDir.LoadString(IDS_AIO_PARAM_DIR);
	_stprintf(tszFile, strAIOParamDir);
	_tcscat(tszPath, tszFile);

	if (fp = _tfopen(tszPath, _T("r")))
	{
		unsigned short 	nIndex;
		PTCHAR			tszToken;
		TCHAR			tszBuffer[255];

		while (!feof(fp))
		{
			memset(tszBuffer, NULL, 255);
			_fgetts(tszBuffer, 255, fp);

			if (tszBuffer[0] == '#')
				continue;

			if (tszBuffer[_tcslen(tszBuffer) - 1] == '\n')
				tszBuffer[_tcslen(tszBuffer) - 1] = NULL;

			for (nIndex = 0; nIndex < _tcslen(tszBuffer); nIndex++)
			{
				if (tszBuffer[nIndex] == '\t')
					tszBuffer[nIndex]	= ' ';
				if (tszBuffer[nIndex] == '#')
					tszBuffer[nIndex]	= NULL;
			}

			tszToken	= _tcstok(tszBuffer, _T(" "));

			if (!bIsTarget)
			{
				if (CString(tszToken) != _T("PN001"))
					continue;
				else 
				{
					tszToken = _tcstok(NULL, _T(" "));
					CString csModuleId = CString(tszToken);
					//if (CString(tszToken) != m_strModuleName[m_lModuleNo])
					//DWORD dwModuleId = strtoul((const char *)csModuleId, NULL, 16);
					DWORD dwModuleId;
					_stscanf(csModuleId, _T("%X"), &dwModuleId);
					if (dwModuleId != m_dwModuleID[m_lModuleNo])
						continue;
					else
						bIsTarget = TRUE;
				}
			}
			else
			{
				CString str = CString(tszToken);
				if (str.Find(_T("EOD")) != -1)
					break;
				if (str.Find(_T("PN")) == -1)
					continue;
				else
				{
					tszToken = _tcstok(NULL, _T(" "));
					UINT nParam = _wtoi(str.Right(str.GetLength()-2));
					switch (nParam)
					{
					case 2:
						for (int i=0; i<8; i++)
						{
							CString csChNo;
							csChNo.Format(_T("IN%d"), i);
							GetDlgItem(IDC_STT_INCH0+i)->SetWindowTextW(csChNo);
						}
						m_aioTarget.nAIChNum = _wtoi(tszToken);
						if (m_aioTarget.nAIChNum > 8)
						{
							m_spnInpByte.EnableWindow(TRUE);
							short Upper = (short)(m_aioTarget.nAIChNum-1)/8;
							m_spnInpByte.SetRange32(0, Upper);
							m_spnInpByte.SetPos(0);
						}
						else
							m_spnInpByte.EnableWindow(FALSE);
						break;
					case 3:	
						for (int i=0; i<8; i++)
						{
							CString csChNo;
							csChNo.Format(_T("OUT%d"), i);
							GetDlgItem(IDC_STT_OUTCH0+i)->SetWindowTextW(csChNo);
						}
						m_aioTarget.nAOChNum = _wtoi(tszToken);
						if (m_aioTarget.nAOChNum > 8)
						{
							m_spnOutpByte.EnableWindow(TRUE);
							short Upper = (short)(m_aioTarget.nAOChNum-1)/8;
							m_spnOutpByte.SetRange32(0, Upper);
							m_spnOutpByte.SetPos(0);
						}
						else
							m_spnOutpByte.EnableWindow(FALSE);
						break;
					
					//case 4:		m_aioTarget.nTestCnt = _wtoi(tszToken);			break;
					case 4:
						{
							long lBoardNo, lModulePos;
							DWORD dwModuleID;
							CString csModuleInfo;
							m_csModuleName = CString(tszToken);
							AxaInfoGetModule(m_lModuleNo, &lBoardNo, &lModulePos, &dwModuleID);
							csModuleInfo.Format(_T(" [B/No:%02d,M/No:%02d] %s"), lBoardNo, m_lModuleNo, m_csModuleName);
							m_edtProductSel.SetWindowText(csModuleInfo);
						}
						break;

					case 10:	m_aioTarget.dVInRange_Low = _wtof(tszToken);	break;
					case 11:	m_aioTarget.dVInRange_High = _wtof(tszToken);	break;
					case 13:	m_aioTarget.dVInThreshold_Low = _wtof(tszToken);	break;
					case 14:	m_aioTarget.dVInThreshold_High = _wtof(tszToken);	break;
					case 15:	m_aioTarget.dVInErrorRange = _wtof(tszToken);	break;
					case 16:	m_aioTarget.dVInOffset = _wtof(tszToken);	break;
					case 17:	m_aioTarget.nAITestCnt = _wtoi(tszToken);	break;
					case 18:
						{
							UINT8 nIndex = 0;
							while (tszToken != NULL)
							{
								m_aioTarget.dAIVPattern[nIndex++] = _wtof(tszToken);
								tszToken = _tcstok(NULL, _T(" "));
							}
							m_aioTarget.nAIVPtrnNum = nIndex;
							break;
						}

					case 30:	m_aioTarget.dCInRange_Low = _wtof(tszToken);	break;
					case 31:	m_aioTarget.dCInRange_High = _wtof(tszToken);	break;
					case 33:	m_aioTarget.dCInThreshold_Low = _wtof(tszToken);	break;
					case 34:	m_aioTarget.dCInThreshold_High = _wtof(tszToken);	break;
					case 35:	m_aioTarget.dCInErrorRange = _wtof(tszToken);	break;
					case 36:	m_aioTarget.dCInOffset = _wtof(tszToken);	break;
					case 38:
						{
							UINT8 nIndex = 0;
							while (tszToken != NULL)
							{
								m_aioTarget.dAICPattern[nIndex++] = _wtof(tszToken);
								tszToken = _tcstok(NULL, _T(" "));
							}
							m_aioTarget.nAICPtrnNum = nIndex;
							break;
						}

					case 110:	m_aioTarget.dVOutRange_Low = _wtof(tszToken);	break;
					case 111:	m_aioTarget.dVOutRange_High = _wtof(tszToken);	break;
					case 113:	m_aioTarget.dVOutThreshold_Low = _wtof(tszToken);	break;
					case 114:	m_aioTarget.dVOutThreshold_High = _wtof(tszToken);	break;
					case 115:	m_aioTarget.dVOutErrorRange = _wtof(tszToken);	break;
					case 116:	m_aioTarget.dVOutOffset = _wtof(tszToken);	break;
					case 117:	m_aioTarget.nAOTestCnt = _wtoi(tszToken);	break;
					case 118:
						{
							UINT8 nIndex = 0;
							while (tszToken != NULL)
							{
								m_aioTarget.dAOPattern[nIndex++] = _wtof(tszToken);
								tszToken = _tcstok(NULL, _T(" "));
							}
							m_aioTarget.nAOPtrnNum = nIndex;
							break;
						}
					default:
						break;
					}
				}
			}
		}
		fclose(fp);
	}
	else
		UpdateMsg(_T("File load fail! Default Fluke Settings applied!"), MT_ERR);
}

int CADST3_VS80Dlg::DeviceInit()
{
	if ((m_aioTarget.nAOChNum>0) && (!m_bFlukeInit))
	{
		FlukeParamLoad();
		if (FlukeEthInit() != 0)
		{
			UpdateMsg(_T("FLUKE Init fail!"), MT_ERR);
			m_bFlukeInit = FALSE;
		}
	}

	if ((m_aioTarget.nAIChNum>0) && (!m_bYKGWInit))
	{
		if (YokogawaInit() != 0)
		{
			UpdateMsg(_T("YOKOGAWA Init fail!"), MT_ERR);
			m_bYKGWInit = FALSE;
		}
		else
		{
			UpdateMsg(_T("YOKOGAWA Init done!"), MT_MSG);
			m_bYKGWInit = TRUE;
		}
	}

	if (!(m_bFlukeInit | m_bYKGWInit))
	{
		// 		MessageBox(_T("Device Init fail! Restart ADST3 SW and Device(s) Please!"), _T("Error-Restart!!!"), MB_OK | MB_ICONERROR);
		// 		EndDialog(IDOK);
		return FALSE;
	}

	return TRUE;
}

void CADST3_VS80Dlg::FlukeParamLoad()
{
	INT		i;
	TCHAR	tszPath[255];
	TCHAR	tszFile[24];
	FILE	*fp;

	GetModuleFileName(NULL, tszPath, 255);

	for (i = (INT)_tcslen(tszPath); i > 0; i--)
	{
		if (tszPath[i] == '\\')
		{
			i++;
			tszPath[i]	= '\0';
			break;
		}
	}

	CString strFlukeParamDir;
	strFlukeParamDir.LoadString(IDS_FLUKE_PARAM_DIR);
	_stprintf(tszFile, strFlukeParamDir);
	_tcscat(tszPath, tszFile);

	if (fp = _tfopen(tszPath, _T("r")))
	{
		unsigned short 	nIndex;
		PTCHAR			tszToken;
		TCHAR			tszBuffer[255];

		while (!feof(fp))
		{
			memset(tszBuffer, NULL, 255);
			_fgetts(tszBuffer, 255, fp);

			if (tszBuffer[0] == '#')
				continue;

			if (tszBuffer[_tcslen(tszBuffer) - 1] == '\n')
				tszBuffer[_tcslen(tszBuffer) - 1]	= NULL;

			for (nIndex = 0; nIndex < _tcslen(tszBuffer); nIndex++)
			{
				if (tszBuffer[nIndex] == '\t')
					tszBuffer[nIndex]	= ' ';
			}

			tszToken	= _tcstok(tszBuffer, _T(" "));

			nIndex		= 0;

			while (tszToken != NULL)
			{
				switch (nIndex)
				{
				case 0:				// Port
					m_SocketDev.nPort = _ttoi(tszToken);
					break;

				case 1:				// IP Address
					{
						int char_str_len = WideCharToMultiByte(CP_ACP, 0, tszToken, -1, NULL, 0, NULL, NULL);
						char* s8_Data = new char[char_str_len]; // get buffer AFTER Insert() !!!
						WideCharToMultiByte(CP_ACP, 0, tszToken, -1, s8_Data, char_str_len, 0,0);
						m_SocketDev.u32IP = inet_addr(s8_Data);
						if (m_SocketDev.u32IP == INADDR_NONE)
						{
							UpdateMsg(_T("Load IP Address fail, default IP 192.168.10.48 applied"), MT_WRN);
							m_SocketDev.u32IP = inet_addr("192.168.10.48");
						}

					}
					break;

				case 2:				// NPLC
					m_dNPLC = m_dFlukeNPLC[_ttoi(tszToken)-1];
					break;

				default:
					break;
				}
				tszToken = _tcstok(NULL, _T(" "));
				nIndex++;
			}
		}
		fclose(fp);
	}
	else
		UpdateMsg(_T("File load fail! Default Fluke Settings applied!"), MT_ERR);

}

int CADST3_VS80Dlg::FlukeEthInit()
{
	int ret = 0;
	CString str;

	// 	if (m_SocketDev.cSocket.GetSocketCount())
	// 	{
	// 		CloseSockets();
	// 		Sleep(100);
	// 	}

	KillTimer(ID_TMR_PROG_AO);

	DWORD u32_EventTimeout = (PROCESS_EVENTS_IN_GUI_THREAD) ? 50 : INFINITE;

	// 	m_SocketDev.u32IP = IP_ADDR0 | (IP_ADDR1<<8) | (IP_ADDR2<<16) | (IP_ADDR3<<24);
	// 	m_SocketDev.nPort = PORT_NO;

	DWORD u32_Err = m_SocketDev.cSocket.ConnectTo(m_SocketDev.u32IP, m_SocketDev.nPort, u32_EventTimeout, MAX_CLIENT_IDLE_TIME);

	if (u32_Err) 
	{
		str.Format(_T("Connect Error %s"), GetSocketErrMsg(u32_Err));
		UpdateMsg(str, MT_ERR);
	}
	else
	{ 
		BYTE* pu8_Addr = (BYTE*)&m_SocketDev.u32IP;
		CString s_IP;
		s_IP.Format(_T("%d.%d.%d.%d"), pu8_Addr[0],pu8_Addr[1],pu8_Addr[2],pu8_Addr[3]);
		str.Format(_T("Connecting to Server (%s) on Port %d.... (waiting for FD_CONNECT)"), s_IP, m_SocketDev.nPort);
		UpdateMsg(str, MT_MSG);
		if (MAX_CLIENT_IDLE_TIME > 0)
		{
			str.Format(_T("Maximum idle time: %d seconds"), MAX_CLIENT_IDLE_TIME);
			UpdateMsg(str, MT_MSG);
		}
	}

	if (u32_Err) 
	{
		CloseSockets();
		return u32_Err;
	}

	// runs until an error occurred or all sockets have closed
#if PROCESS_EVENTS_IN_GUI_THREAD
	ProcessEvents();
#else
	DWORD u32_ID;
	m_SocketDev.hThread = CreateThread(0, 0, SocketProcessEventThread, this, 0, &u32_ID);
#endif

	SetTimer(ID_TMR_SOCKET_CONNECTION, 50, 0);

	return ret;
}

int CADST3_VS80Dlg::FlukeCfg()
{
	int  ret;

	ret = SendTo(m_SocketDev.hSocket, _T("SYST:REM\r\n"));	// Remote Mode
	if( !ret )
		return	ret;

	ret = SendTo(m_SocketDev.hSocket, _T("CONF:VOLT:DC 10\r\n"));
	if( !ret )
		return	ret;

	ret = SendTo(m_SocketDev.hSocket, _T("VOLT:RANG:AUTO OFF\r\n"));
	if( !ret )
		return	ret;

	ret = SendTo(m_SocketDev.hSocket, _T("VOLT:DC:NPLC 1\r\n"));
	if( !ret )
		return	ret;

	ret = SendTo(m_SocketDev.hSocket, _T("ZERO:AUTO 0\r\n"));
	if( !ret )
		return	ret;

	ret = SendTo(m_SocketDev.hSocket, _T("VOLT:DC:RES MIN\r\n"));
	if( !ret )
		return	ret;

	ret = SendTo(m_SocketDev.hSocket, _T("TRIG:SOUR IMM\r\n"));
	if( !ret )
		return	ret;

	ret = SendTo(m_SocketDev.hSocket, _T("TRIG:DEL 0\r\n"));
	if( !ret )
		return	ret;

	ret = SendTo(m_SocketDev.hSocket, _T("TRIG:COUN 1\r\n"));
	if( !ret )
		return	ret;

	ret = SendTo(m_SocketDev.hSocket, _T("SAMP:COUN 1\r\n"));

	if (ret)
	{
		m_bFlukeInit = TRUE;
		SetTimer(ID_TMR_PROG_AO, 1, NULL);
	}

	return ret;
}

#define DEV_MAX			1	
#define DEV_NAME_BUF	100
int CADST3_VS80Dlg::YokogawaInit()
{
	int ret = 0;
	DEVICELIST DevList[DEV_MAX];
	int nDevNum;

	KillTimer(ID_TMR_PROG_AI);

	TmcFinish(m_YKGWDevId);

	ret = TmcSearchDevices(TM_CTL_USBTMC2, DevList, DEV_MAX, &nDevNum, NULL);
	if ( ret != 0 ) 
		return	ret;

	if (nDevNum == 0)
	{
		UpdateMsg(_T("No YOKOGAWA Device found!"), MT_ERR);
		return 1;
	}

	ret = TmcInitialize( TM_CTL_USBTMC2, DevList[0].adr, &m_YKGWDevId );

	if( ret != 0 ) 
		return	TmcGetLastError( m_YKGWDevId );

	ret = YokogawaCfg(m_YKGWDevId);

	if (!ret)
		m_bYKGWInit = TRUE;

	return ret;
}

int CADST3_VS80Dlg::YokogawaCfg(int nDevId)
{
	int ret = 0;
	char buf[DEV_NAME_BUF];
	int  length;

	ret = TmcSetTerm( nDevId, 2, 1 );
	if( ret != 0 ) 
		return	TmcGetLastError( nDevId );

	ret = TmcSetTimeout( nDevId, 300 );
	if( ret != 0 ) 
		return	TmcGetLastError( nDevId );

	ret = TmcSetRen( nDevId, 1 );
	if( ret != 0 )
		return	TmcGetLastError( nDevId );

	// sending *RST 
	ret = TmcSend( nDevId, "*RST" );
	if( ret != 0 )
		return	TmcGetLastError( nDevId );

	// *sending IDN? & receiving query
	ret = TmcSend( nDevId, "*IDN?" );
	if( ret != 0 )
		return	TmcGetLastError( nDevId );

	ret = TmcReceive( nDevId, buf, DEV_NAME_BUF, &length );
	if( ret != 0 )
		return	TmcGetLastError( nDevId );

	if (!m_chkInpMode.GetCheck())
	{
		ret = TmcSend(nDevId, ":SOUR:FUNC VOLT");
		if( ret != 0 )
			return	TmcGetLastError( nDevId );
		ret = TmcSend(nDevId, ":SOUR:RANG 10E+0");
		if( ret != 0 )
			return	TmcGetLastError( nDevId );
	}
	else
	{
		ret = TmcSend(nDevId, ":SOUR:FUNC CURR");
		if( ret != 0 )
			return	TmcGetLastError( nDevId );
		ret = TmcSend(nDevId, ":SOUR:RANG 100E-3");
		if( ret != 0 )
			return	TmcGetLastError( nDevId );
	}

	ret = TmcSend(nDevId, ":SOUR:LEV 0");
	if( ret != 0 )
		return	TmcGetLastError( nDevId );

	return ret;
}

void CADST3_VS80Dlg::OnTimer(UINT_PTR nIDEvent)
{
	int ret;
	CString	strText;
	LARGE_INTEGER	CurCnt;
	double	dET;
	CString str;
	CString strTemp;

	switch (nIDEvent)
	{
	case ID_TMR_PROG_AI:
		{
			CString strCmd;
			double	dInpRef = m_chkInpMode.GetCheck() ? m_aioTarget.dAICPattern[m_aioTarget.nAIPtrnIndex]/1000 : m_aioTarget.dAIVPattern[m_aioTarget.nAIPtrnIndex];
			UINT	nPtrnNum = m_chkInpMode.GetCheck() ? m_aioTarget.nAICPtrnNum : m_aioTarget.nAIVPtrnNum;

			if (m_aioTarget.nAISampleCnt == 0)
			{
				strCmd.Format(_T("SOUR:LEV %f"), dInpRef);
				ret = TmcSend(m_YKGWDevId, CT2CA(strCmd));
				if (ret != 0)
				{
					UpdateMsg(_T("Set Outp Level Error! Config Device again please!"), MT_ERR);
					KillTimer(ID_TMR_PROG_AI);
					m_bYKGWInit = FALSE;
					m_sttYKGWSetting.SetWindowTextW(_T("Error"));
					m_sttYKGWSetting.SetBkColor(CL_ERROR);
					// nhqt
					GetDlgItem(IDC_BTN_PREV)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);
					return;
				}
				Sleep(500);
			}
			ProcessDataAI(m_aioTarget.nAIChNum);
			if (++m_aioTarget.nAISampleCnt >= m_aioTarget.nAITestCnt)
			{
				m_aioTarget.nAISampleCnt = 0;
				if (++m_aioTarget.nAIPtrnIndex >= nPtrnNum)
				{
					// nhqt
					//KillTimer(ID_TMR_PROG_AI);
					for (int i=0; i</*m_aioTarget.nAIChNum*/ MAX_AI; i++)
					{
						if (m_bAIChStarted[i])	
						{
							m_sttAI[i].StartBkBlink(FALSE);
							m_sttAI[i].GetWindowTextW(strText);
							if (strText == _T("Running"))
							{
								m_sttAI[i].SetWindowTextW(_T("Success"));
								m_sttAI[i].SetBkColor(CL_SUCCESS);
							}
							m_bAIChStarted[i] = FALSE;
							m_btnAI[i].SetBitmaps(IDB_BMP_BTN_START, MASK_COLOR, (int)BTNST_AUTO_DARKER);

							QueryPerformanceCounter(&CurCnt);
							dET = (double)(CurCnt.QuadPart - m_StartCnt[i].QuadPart)/(double)m_PerfFreq.QuadPart;
							str.Format(_T("[AICH%d] Test finished. Test sample: %d. Elapsed time: %.3f [sec]"), i+8*m_spnInpByte.GetPos32(), m_aioTarget.nAITestCnt, dET);
							UpdateMsg(str, MT_MSG);

							CheckAIAutorunResult(i);
						}

					}
					ret  = TmcSend( m_YKGWDevId, "SOUR:LEV 0");
					ret |= TmcSend( m_YKGWDevId, ":OUTP OFF");

					if (ret != 0)
						UpdateMsg(_T("Yokogawa device Output off error!"), MT_ERR);

					return;
				}
			}

			// nhqt
			for (int j=0; j</*m_aioTarget.nAIChNum*/ MAX_AI; j++)
			{
				if (m_bAIChStarted[j])
				{
					GetDlgItem(IDC_BTN_PREV)->EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
					return;
				}
			}

			ret  = TmcSend( m_YKGWDevId, "SOUR:LEV 0");
			ret |= TmcSend( m_YKGWDevId, ":OUTP OFF");
			if (ret != 0)
				UpdateMsg(_T("Yokogawa Output OFF Error!"), MT_ERR);
			GetDlgItem(IDC_BTN_PREV)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);
			m_spnInpByte.EnableWindow(TRUE);
			KillTimer(ID_TMR_PROG_AI);
		}
		break;

	case ID_TMR_PROG_AO:
		{
			CString str;
			long	lChNo;
			DWORD	ret;
			m_nSocketTimeout++;
			if (m_bEOD)
			{
				// Socket timeout evaluating
				// 				str.Format(_T("[Fluke] Socket Timeout: %d"), m_nSocketTimeout);
				// 				UpdateMsg(str, MT_WRN);

				m_nSocketTimeout = 0;
				int nByteOffset = m_spnOutpByte.GetPos32();
				
				if (m_aioTarget.nAOSampleCnt == 0)
				{
					for (int i=0; i</*m_aioTarget.nAOChNum*/ MAX_AO; i++)
					{
						if (m_bAOChStarted[i])
						{
							
							ret = AxaInfoGetChannelNoDacOfModuleNo(m_lModuleNo, &lChNo);
							if (ret != AXT_RT_SUCCESS)
							{
								str.Format(_T("Get Number Channel DAC of module %d fail. Code:%d"), m_lModuleNo, ret);
								UpdateMsg(str, MT_ERR);
							}
							double	dOutpVolt;
							if (m_aioTarget.nAOPtrnIndex == 0)
							{
								if( !SendTo(m_SocketDev.hSocket, _T("SYST:LOC\r\n")) )
									UpdateMsg(_T("[Fluke] Set Local Mode Error"), MT_ERR);
								
								AxaoSetRange(i+8*m_spnOutpByte.GetPos32()+lChNo, m_aioTarget.dVOutRange_Low, m_aioTarget.dVOutRange_High);
								AxaoReadVoltage(i+8*m_spnOutpByte.GetPos32()+lChNo, &dOutpVolt);
								//if(AxaoSetRange(i + lChNo, m_aioTarget.dVOutRange_Low, m_aioTarget.dVOutRange_High)!= AXT_RT_SUCCESS) 
								//{
								//	str.Format(_T("[AOCH%d] Set Output Range Error"), i + lChNo);
								//	UpdateMsg(str, MT_ERR);
								//}
								//if (AxaoReadVoltage(i+lChNo, &dOutpVolt) != AXT_RT_SUCCESS)
								//{
								//	str.Format(_T("[AOCH%d] Read Voltage Error"), i+8*m_spnOutpByte.GetPos32()+lChNo);
								//	UpdateMsg(str, MT_ERR);
								//}

							}
							// Threshold validation
							double	dVOut = m_aioTarget.dAOPattern[m_aioTarget.nAOPtrnIndex];
							if (dVOut > m_aioTarget.dVOutThreshold_High)
								dVOut = m_aioTarget.dVOutThreshold_High;
							if (dVOut < m_aioTarget.dVOutThreshold_Low)
								dVOut = m_aioTarget.dVOutThreshold_Low;

							if (AxaoWriteVoltage(i+8*nByteOffset+lChNo, dVOut + m_aioTarget.dVOutOffset) != AXT_RT_SUCCESS) 
							{
								str.Format(_T("[AOCH%d]: Voltage Output error!"), i+8*m_spnOutpByte.GetPos32());
								UpdateMsg(str, MT_ERR);
								// nhqt
								//KillTimer(ID_TMR_PROG_AO);
								m_sttAO[i].SetWindowTextW(_T("Error"));
								m_sttAO[i].SetBkColor(CL_ERROR);
								return;
							}	
						}//end if
					}//end for
					if (m_aioTarget.nAOPtrnIndex == 0)
					{
						if( !SendTo(m_SocketDev.hSocket, _T("SYST:REM\r\n")) )
							UpdateMsg(_T("[Fluke] Set Remote Mode Error"), MT_ERR);
					}
					++m_aioTarget.nAOSampleCnt;
				}//end if
				else
				{
					ProcessDataAO();

					if (++m_aioTarget.nAOSampleCnt > m_aioTarget.nAOTestCnt)
					{
						m_aioTarget.nAOSampleCnt = 0;
						if (++m_aioTarget.nAOPtrnIndex >= m_aioTarget.nAOPtrnNum)
						{
							// nhqt
							//KillTimer(ID_TMR_PROG_AO);
							for (int i=0; i<m_aioTarget.nAOChNum; i++)
							{
								if (m_bAOChStarted[i])
								{
									m_sttAO[i].StartBkBlink(FALSE);
									m_sttAO[i].GetWindowTextW(strText);
									if (strText == _T("Running"))
									{
										m_sttAO[i].SetWindowTextW(_T("Success"));
										m_sttAO[i].SetBkColor(CL_SUCCESS);
									}
									QueryPerformanceCounter(&CurCnt);
									dET = (double)(CurCnt.QuadPart - m_StartCnt[i+10].QuadPart)/(double)m_PerfFreq.QuadPart;
									m_bAOChStarted[i] = FALSE;
									m_btnAO[i].SetBitmaps(IDB_BMP_BTN_START, MASK_COLOR, (int)BTNST_AUTO_DARKER);
									str.Format(_T("[AOCH%d] Test finished. Test sample: %d. Elapsed time: %.3f [sec]"), i+8*m_spnOutpByte.GetPos32(), m_aioTarget.nAOTestCnt, dET);
									UpdateMsg(str, MT_MSG);

									UpdateMsg(str, MT_MSG);
									CheckAIAutorunResult(i);
								}
							}//end for
							return;
						}//end if
					}//end if
				}//end if

				m_bEOD = FALSE;
				if( !SendTo(m_SocketDev.hSocket, _T("READ?\r\n")) )
					UpdateMsg(_T("Get Input Data Error!"), MT_ERR);

			}
			if (m_nSocketTimeout > SOCKET_TIMEOUT)
			{
				for (int j=0; j<m_aioTarget.nAOChNum; j++)
				{
					if (m_bAOChStarted[j])
					{
						m_sttAO[j].SetWindowTextW(_T("Error"));
						m_sttAO[j].StartBkBlink(FALSE);
						m_sttAO[j].SetBkColor(CL_ERROR);
						m_bAOChStarted[j] = FALSE;
						m_btnAO[j].SetBitmaps(IDB_BMP_BTN_START, MASK_COLOR, (int)BTNST_AUTO_DARKER);
						str.Format(_T("[AOCH%d]: Communication Timeout!"), j+8*m_spnOutpByte.GetPos32());
						UpdateMsg(str, MT_ERR);
					}
				}
				KillTimer(ID_TMR_PROG_AO);
				//nhqt
				GetDlgItem(IDC_BTN_PREV)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);
			}

			// nhqt
			for (int j=0; j<m_aioTarget.nAOChNum; j++)
			{
				if (m_bAOChStarted[j])
				{
					GetDlgItem(IDC_BTN_PREV)->EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
					return;
				}
			}
			GetDlgItem(IDC_BTN_PREV)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);
			m_spnOutpByte.EnableWindow(TRUE);
			KillTimer(ID_TMR_PROG_AO);
		}
		break;

	case ID_TMR_SOCKET_CONNECTION:
		{
			if (m_SocketDev.cSocket.GetState() & TCP::cSocket::E_Connected)
			{
				SOCKET h_Socket;
				CString str;
				TCP::cSocket::cHash<SOCKET,DWORD> SocketList;
				DWORD u32_Err = m_SocketDev.cSocket.GetAllConnectedSockets(&SocketList);

				if (u32_Err) 
				{
					str.Format(_T("Error getting connected Sockets: %s"), GetSocketErrMsg(u32_Err));
					UpdateMsg(str, MT_ERR);
					//KillTimer(ID_TMR_SOCKET_CONNECTION);
					break;
				}

				DWORD u32_Count = SocketList.GetCount();
				for (DWORD i=0; i<u32_Count; i++)
				{
					h_Socket = SocketList.GetKeyByIndex(i);
				}
				m_SocketDev.hSocket = SocketList.GetKeyByIndex(0);
				//KillTimer(ID_TMR_SOCKET_CONNECTION);
				if (!m_bFlukeInit)
				{
					if (FlukeCfg())
					{
						m_bFlukeInit = TRUE;
						UpdateMsg(_T("FLUKE Init done!"), MT_MSG);
						CtrlsEnable();
					}
					else
						UpdateMsg(_T("FLUKE Init fail!"), MT_ERR);
				}
			}
			else
				m_bFlukeInit = FALSE;
		}
		break;

	case ID_TMR_UPDATE_CH_STATUS:
		break;

	case ID_TMR_PROGRESS_AUTO:

		AxaInfoGetModuleCount(&m_lModuleCnt);
		if (!(m_ProductCnt <= m_lModuleCnt))
		{
			m_TestCount = 0;
			KillTimer(ID_TMR_PROG_AI);
			KillTimer(ID_TMR_PROG_AO);
			KillTimer(ID_TMR_PROGRESS_AUTO);
		}
	
		switch (m_Autorun_product_num)
		{
		case PRO_N3AI16:
			if (Auto_start_AI16[m_TestCount] == FALSE &&
				(!m_bAIChStarted[m_TestCount]))
			{
				SetAIState(m_TestCount);
			}
			break;

		case PRO_N3AO8:
			if (Auto_start_AO8[m_TestCount] == FALSE &&
				(!m_bAOChStarted[m_TestCount]))
			{
				SetAOState(m_TestCount);
			}
			break;

		case  PRO_SIOAI8AO4:

			if (!m_bitoo)
			{
				if (Auto_start_AI8[m_TestCount] == FALSE &&
					(!m_bAIChStarted[m_TestCount]))
				{
					SetAIState(m_TestCount);
				}
			}

			else
			{
				if (Auto_start_AO8[m_TestCount] == FALSE &&
					(!m_bAOChStarted[m_TestCount]))
				{
					SetAOState(m_TestCount);
				}
			}
			break;

		case PRO_SIOAI8:
			if (Auto_start_AI8[m_TestCount] == FALSE &&
				(!m_bAIChStarted[m_TestCount]))
			{
				SetAIState(m_TestCount);
			}
			break;

		case PRO_SIOAO4:

			break;

		case PRO_SIOAI4RB:

			break;

		case PRO_SIOAO4RB:

			break;

		default:
			break;

		}
		/*
		if (m_bis16)
		{
			if (Auto_start_AI16[m_TestCount] == FALSE &&
				(!m_bAIChStarted[m_TestCount]))
			{
				SetAIState(m_TestCount);
			}
		}
		 */

		if (m_bis8)
		{
			if (Auto_start_AI8[m_TestCount] == FALSE &&
				(!m_bAIChStarted[m_TestCount]))
			{
				SetAIState(m_TestCount);
			}
		}

	case ID_TMR_CHECK_END:
		if (m_ProductCnt >= m_lModuleCnt)
		{
			KillTimer(ID_TMR_PROGRESS_AUTO);
			if (m_bAIChStarted[m_TestCount])
			{
				SetAIState(m_TestCount);
			}

			else if (m_bAOChStarted[m_TestCount])
			{
				SetAOState(m_TestCount);
			}
			sendCommand('c');	//Relay 초기화
			m_TestCount = 0;	//TestCount 초기화
			m_ProductCnt = 0;
			GetDlgItem(IDC_BTN_AUTORUN)->EnableWindow(true);
			str.Format(_T("테스트가 모두 종료되었습니다."));
			UpdateMsg(str, MT_RSLT);
		}
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CADST3_VS80Dlg::ProcessDataAI(int nAIChNum)
{
	UINT8 j;
	double	dInpData[MAX_AI] = {0};
	CString	str;
	CString strTemp;
	//CString path;
	UINT8	nCHIndex = 0;
	double	dRef = m_chkInpMode.GetCheck() ? (m_aioTarget.dAICPattern[m_aioTarget.nAIPtrnIndex]/4) : m_aioTarget.dAIVPattern[m_aioTarget.nAIPtrnIndex];
	double	dER	= m_chkInpMode.GetCheck() ? m_aioTarget.dCInErrorRange : m_aioTarget.dVInErrorRange ;
	long	lChNo;
	TCHAR chThisPath[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, chThisPath);

	//UpdateData[TRUE];

	//CString strThisPath;
	//strThisPath.Format(_T("%s\\%s.xlsx"), chThisPath, _T("test"));
	//CXLEzAutomation XL(FALSE);

	//	LARGE_INTEGER	CurCnt;
	//	double	dET;

	//XL.ReleaseExcel();

	DWORD	dwRet;
	dwRet = AxaInfoGetChannelNoAdcOfModuleNo(m_lModuleNo, &lChNo);
	if (dwRet != AXT_RT_SUCCESS)
	{
		str.Format(_T("Get Number Channel DAC of module %d fail. Code:%d"), m_lModuleNo, dwRet);
		UpdateMsg(str, MT_ERR);
	}

	if(AXT_RT_SUCCESS == dwRet)
	{
		for (j=0; j</*m_aioTarget.nAIChNum*/ MAX_AI; j++)
		{
			if (m_bAIChStarted[j])	
			{
				AxaiSetRange(j+8*m_spnInpByte.GetPos32()+lChNo, m_aioTarget.dVInRange_Low, m_aioTarget.dVInRange_High);
				Sleep(100);
				if (AxaiSwReadVoltage(j+8*m_spnInpByte.GetPos32()+lChNo, &dInpData[j]) != AXT_RT_SUCCESS) 
				{
					str.Format(_T("[AICH%d]: Voltage read error!"), j+8*m_spnInpByte.GetPos32());
					UpdateMsg(str, MT_ERR);
				}
				m_dStoreAIResult[j] = dInpData[j];

				if ((dInpData[j] > dRef + dER) || (dInpData[j] < dRef - dER))
				{
					m_sttAI[j].SetWindowTextW(_T("Error"));
					m_sttAI[j].StartBkBlink(FALSE);
					m_sttAI[j].SetBkColor(CL_ERROR);
					//KillTimer(ID_TMR_PROG_AI);
					m_bAIChStarted[j] = FALSE;
					m_btnAI[j].SetBitmaps(IDB_BMP_BTN_START, MASK_COLOR, (int)BTNST_AUTO_DARKER);
					str.Format(_T("[AICH%d]: Wr = %.4f, Rd = %.4f, Fail!"), j+8*m_spnInpByte.GetPos32(), dRef, dInpData[j]);
					UpdateMsg(str, MT_ERR);

					independent(j + 8 * m_spnInpByte.GetPos32(), strTemp, 1);
					//CheckAIAutorunResult(j);	//2018-08-20 Feedback 받은 결과 실패 시 실패Ch에서 멈추면 좋겠다라고 받음
					m_TestCount = 0;
					m_bfinished = true;
					KillTimer(ID_TMR_PROGRESS_AUTO);
					KillTimer(ID_TMR_PROG_AI);
					GetDlgItem(IDC_BTN_AUTORUN)->EnableWindow(true);


				}
				//			QueryPerformanceCounter(&CurCnt);
				//			dET = (double)(m_StartCnt[j].QuadPart - CurCnt.QuadPart)/(double)m_PerfFreq.QuadPart;
	#ifdef DEBUG
				else
				{
					str.Format(_T("%d.[AICH%d]: Wr = %.4f, Rd = %.4f, Passed!"), m_aioTarget.nAISampleCnt+1, j+8*m_spnInpByte.GetPos32(), dRef, dInpData[j]);
					UpdateMsg(str, MT_RSLT);
				}
	#endif // DEBUG
			}
		}
	}
}

void CADST3_VS80Dlg::ProcessDataAO()
{
	UINT8 j;
	double	dRef = m_aioTarget.dAOPattern[m_aioTarget.nAOPtrnIndex];
	double	dER	= m_aioTarget.dVOutErrorRange ;
	CString str;
	double dReadData = atof(m_SocketDev.cData);

	if ((dReadData > dRef + dER) || (dReadData < dRef - dER))
	{
		for (j=0; j</*m_aioTarget.nAOChNum*/ MAX_AO; j++)
		{
			if (m_bAOChStarted[j])
			{
				m_sttAO[j].SetWindowTextW(_T("Error"));
				m_sttAO[j].StartBkBlink(FALSE);
				m_sttAO[j].SetBkColor(CL_ERROR);
				m_bAOChStarted[j] = FALSE;
				m_btnAO[j].SetBitmaps(IDB_BMP_BTN_START, MASK_COLOR, (int)BTNST_AUTO_DARKER);
				str.Format(_T("[AOCH%d]: Wr = %.4f, Rd = %.4f, Fail!"), j+8*m_spnOutpByte.GetPos32(), dRef, dReadData);
				UpdateMsg(str, MT_ERR);
				//CheckAIAutorunResult(j);
				independent(j + 8 * m_spnInpByte.GetPos32(), str, 1, 0);
				m_TestCount = 0;
				m_bfinished = true;
				KillTimer(ID_TMR_PROGRESS_AUTO);
				KillTimer(ID_TMR_PROG_AO);
				GetDlgItem(IDC_BTN_AUTORUN)->EnableWindow(true);
			}
		}
		// nhqt
		//KillTimer(ID_TMR_PROG_AO);
	}
#ifdef DEBUG
	else
	{
		for (j=0; j<m_aioTarget.nAOChNum; j++)
		{
			if (m_bAOChStarted[j])
			{
				str.Format(_T("%d.[AOCH%d]: Wr = %.4f, Rd = %.4f, Passed!"), m_aioTarget.nAOSampleCnt, j+8*m_spnOutpByte.GetPos32(), dRef, dReadData);
				UpdateMsg(str, MT_RSLT);
			}
		}
	}
#endif
}

void CADST3_VS80Dlg::OnBnClickedBtnExit()
{
//#ifndef DEBUG
	for (int i=0; i</*m_aioTarget.nAIChNum*/ MAX_AI;i++)
	{
		if (m_bAIChStarted[i])
		{
			MessageBox(_T("Stop all current tests before exit!"), _T("Test Busy!"), MB_ICONWARNING);
			return;
		}
	}
//#endif

	for (int i=0; i</*m_aioTarget.nAOChNum*/ MAX_AO;i++)
	{
		if (m_bAOChStarted[i])
		{
			MessageBox(_T("Stop all current tests before exit!"), _T("Test Busy!"), MB_ICONWARNING);
			return;
		}
	}

	if (MessageBox(_T("Do you really want to Exit ADST3?"), _T("Exit?"), MB_YESNO | MB_ICONQUESTION) != IDYES)
		return;

	for (int i=ID_TMR_0; i<ID_TMR_TOTAL; i++)
		KillTimer(i);

	if (AxlIsOpened())
		AxlClose();

	//	if (m_YKGWDevId != NULL)
	//	{
	TmcSend( m_YKGWDevId, ":OUTP OFF");
	TmcSetRen(m_YKGWDevId, 0);
	TmcFinish(m_YKGWDevId);
	//	}

	if (m_SocketDev.hSocket != NULL)
	{
		SendTo(m_SocketDev.hSocket, _T("SYST:LOCAL\r\n"));
		Sleep(100);
	}
	m_bDlgClosed = TRUE;
	m_SocketDev.cSocket.Close();
	EndDialog(NULL);
}


void CADST3_VS80Dlg::OnBnClickedChkInpMode()
{
	BntChkInpMode();		//입력 소스 변경 컨트롤 함수화로 인해 코드이동
}

void CADST3_VS80Dlg::OnBnClickedBtnPrev()
{
	BnClickedBtnPrev();
}


void CADST3_VS80Dlg::OnBnClickedBtnNext()
{
	BnClickedBtnNext();
}

void CADST3_VS80Dlg::CtrlsEnable(BOOL bEn)
{
	int i;
	CString strStatusAI = (bEn&&m_bYKGWInit) ? _T("Ready") : _T("Disabled");
	COLORREF cfStatusAI = (bEn&&m_bYKGWInit) ? CL_READY : CL_DISABLE;

	CString strStatusAO = (bEn&&m_bFlukeInit) ? _T("Ready") : _T("Disabled");
	COLORREF cfStatusAO = (bEn&&m_bFlukeInit) ? CL_READY : CL_DISABLE;

	for (i=0; i<MAX_AI; i++)
	{
		if (i+8*m_spnInpByte.GetPos32() < m_aioTarget.nAIChNum)
		{
			m_sttAI[i].EnableWindow(bEn);
			m_sttAI[i].ShowWindow(TRUE);
			m_sttAI[i].SetWindowTextW(strStatusAI);
			m_sttAI[i].SetBkColor(cfStatusAI);

			m_AICh[i].EnableWindow(bEn);
			m_AICh[i].ShowWindow(TRUE);
			m_btnAI[i].ShowWindow(TRUE);
			if (m_bYKGWInit)
				m_btnAI[i].SetBitmaps(IDB_BMP_BTN_START, MASK_COLOR, (int)BTNST_AUTO_DARKER);
		}
		else
		{
			m_sttAI[i].EnableWindow(FALSE);
			m_sttAI[i].ShowWindow(FALSE);
			m_AICh[i].EnableWindow(FALSE);
			m_AICh[i].ShowWindow(FALSE);
			m_btnAI[i].ShowWindow(FALSE);
		}
	}

	for (i=0; i < MAX_AO; i++)
	{
		if (i+8*m_spnOutpByte.GetPos32() < m_aioTarget.nAOChNum)
		{
			m_sttAO[i].EnableWindow(bEn);
			m_sttAO[i].ShowWindow(bEn);
			m_sttAO[i].SetWindowTextW(strStatusAO);
			m_sttAO[i].SetBkColor(cfStatusAO);

			m_AOCh[i].EnableWindow(bEn);
			m_AOCh[i].ShowWindow(TRUE);
			m_btnAO[i].ShowWindow(TRUE);
			if (m_bFlukeInit)
				m_btnAO[i].SetBitmaps(IDB_BMP_BTN_START, MASK_COLOR, (int)BTNST_AUTO_DARKER);
		}
		else
		{
			m_sttAO[i].EnableWindow(FALSE);
			m_sttAO[i].ShowWindow(FALSE);
			m_AOCh[i].EnableWindow(FALSE);
			m_AOCh[i].ShowWindow(FALSE);
			m_btnAO[i].ShowWindow(FALSE);
		}
	}

	if (m_aioTarget.nAIChNum>0)
	{
		m_chkInpMode.EnableWindow(bEn);
		m_btnYKGWSetting.EnableWindow(bEn);
		m_sttYKGWSetting.EnableWindow(bEn);

		m_chkInpMode.ShowWindow(TRUE);
		m_btnYKGWSetting.ShowWindow(TRUE);
		m_sttYKGWSetting.ShowWindow(TRUE);

		//m_btnYKGWFileOpen.ShowWindow(TRUE);
		if (m_aioTarget.nAIChNum>8)
			m_spnInpByte.ShowWindow(TRUE);
		else
			m_spnInpByte.ShowWindow(FALSE);

		m_sttAIG.ShowWindow(TRUE);

		if (m_bYKGWInit)
		{
			m_sttYKGWSetting.EnableWindow(bEn);
			m_sttYKGWSetting.SetWindowText(_T("Ready"));
			m_sttYKGWSetting.SetBkColor(CL_READY);
		}
		else
		{
			m_sttYKGWSetting.EnableWindow(FALSE);
			m_sttYKGWSetting.SetWindowText(_T("Disabled"));
			m_sttYKGWSetting.SetBkColor(CL_DISABLE);
		}
	}
	else
	{
		m_chkInpMode.ShowWindow(FALSE);
		m_btnYKGWSetting.ShowWindow(FALSE);
		m_sttYKGWSetting.ShowWindow(FALSE);

		//m_btnYKGWFileOpen.ShowWindow(FALSE);
		m_spnInpByte.ShowWindow(FALSE);

		m_sttAIG.ShowWindow(FALSE);
	}

	if (m_aioTarget.nAOChNum>0)
	{
		m_btnFlukeSetting.EnableWindow(bEn);
		m_sttFlukeSetting.EnableWindow(bEn);

		m_btnFlukeSetting.ShowWindow(TRUE);
		m_sttFlukeSetting.ShowWindow(TRUE);

		//m_btnFlukeFileOpen.ShowWindow(TRUE);
		if (m_aioTarget.nAOChNum>8)
			m_spnOutpByte.ShowWindow(TRUE);
		else
			m_spnOutpByte.ShowWindow(FALSE);

		m_sttAOG.ShowWindow(TRUE);

		if (m_bFlukeInit)
		{
			m_sttFlukeSetting.EnableWindow(bEn);
			m_sttFlukeSetting.SetWindowText(_T("Ready"));
			m_sttFlukeSetting.SetBkColor(CL_READY);
		}
		else
		{
			m_sttFlukeSetting.EnableWindow(FALSE);
			m_sttFlukeSetting.SetWindowText(_T("Disabled"));
			m_sttFlukeSetting.SetBkColor(CL_DISABLE);
		}
	}
	else
	{
		m_btnFlukeSetting.ShowWindow(FALSE);
		m_sttFlukeSetting.ShowWindow(FALSE);

		//m_btnFlukeFileOpen.ShowWindow(FALSE);
		m_spnOutpByte.ShowWindow(FALSE);

		m_sttAOG.ShowWindow(FALSE);
	}
}

// static
ULONG WINAPI CADST3_VS80Dlg::SocketProcessEventThread(void* p_Param)
{
	CADST3_VS80Dlg* p_This = (CADST3_VS80Dlg*)p_Param;
	p_This->SocketProcessEvents();
	if (p_This->m_SocketDev.hThread != NULL)
		CloseHandle(p_This->m_SocketDev.hThread);
	return 0;
}

// Process all events which occur on one of the open sockets
void CADST3_VS80Dlg::SocketProcessEvents()
{
	CString	str;

	while (TRUE) // Loop runs until the main window was closed or a severe error occurred
	{
#if PROCESS_EVENTS_IN_GUI_THREAD
		PumpMessages();
#endif

		TCP::cSocket::cMemory* pi_RecvMem;
		SOCKET  h_Socket;
		DWORD u32_Event, u32_IP, u32_Read, u32_Sent;
		DWORD u32_Err = m_SocketDev.cSocket.ProcessEvents(&u32_Event, &u32_IP, &h_Socket, &pi_RecvMem, &u32_Read,  &u32_Sent);

		// Main Dialog was closed -> !Immediately! stop all output and printing into GUI.
		// Otherwise the application will not shut down correctly and the EXE keeps running. (only visible in Task Manager)
		// There may appear a lot of other strange things when the Events thread still runs while the GUI thread already finished!
		if (m_bDlgClosed) 
			return;  // return NOT break!

		if (u32_Err == ERROR_TIMEOUT) // 50 ms interval has elapsed
			continue;

		CString s_Msg, s_Events;
		if (u32_Event) // ATTENTION: u32_Event may be == 0 -> do nothing.
		{
			//			s_Events.Format(_T("Server (%s) --> "), FormatIP(u32_IP));
			char s8_Events[200];
			m_SocketDev.cSocket.FormatEvents(u32_Event, s8_Events);
			//			s_Events += s8_Events;

			if (u32_Event & FD_READ)  s_Msg.Format(_T(" %d Bytes received."), u32_Read);
			if (u32_Event & FD_WRITE) s_Msg.Format(_T(" %d Bytes sent"),      u32_Sent);

			if (u32_Event & FD_READ && pi_RecvMem) // pi_RecvMem may be NULL if an error occurred!!
			{
				SocketProcessReceivedDataNormal(pi_RecvMem);
			}
		}

		if (u32_Err)
		{
			// mi_Socket.Close() has been called -> don't print this error message
			if (u32_Err == WSAENOTCONN)
				break;

			// Print all the other error messages
			str.Format(_T("ProcessEvent Error %s"), GetSocketErrMsg(u32_Err));

			// An error normally means that the socket has a problem -> abort the loop.
			// A few errors should not abort the processing:
			if (u32_Err != WSAECONNABORTED && // e.g. after the other side was killed in TaskManager 
				u32_Err != WSAECONNRESET   && // Connection reset by peer.
				u32_Err != WSAECONNREFUSED && // FD_ACCEPT with already 62 clients connected
				u32_Err != WSAESHUTDOWN)      // Sending data to a socket just in the short timespan 
				break;                        //   between shutdown() and closesocket()
		}
	}; // end loop

	CloseSockets();
	KillTimer(ID_TMR_SOCKET_CONNECTION);
	UpdateMsg(_T("Socket Connection abandoned. Connection with Fluke-8846A is closed!"), MT_ERR);
}

// ##################################################################################################
//                                  PROCESS RECEIVED DATA
// ##################################################################################################

// Mode NORMAL:
// This simple "data processor" prints the data blocks immediately and unchanged as they arrive from the network
void CADST3_VS80Dlg::SocketProcessReceivedDataNormal(TCP::cSocket::cMemory* pi_RecvMem)
{
	char*  s8_Buf = pi_RecvMem->GetBuffer();
	DWORD u32_Len = pi_RecvMem->GetLength();
	//CString str;

	//CString s_String = CopyToString(s8_Buf, u32_Len);
	// 	str.Format(_T("Received: '%s'"), s_String);
	// 	UpdateMsg(str, 2);

	//memset(&m_SocketDev.sData, NULL, sizeof(m_SocketDev.sData));
	//m_SocketDev.sData = CopyToString(s8_Buf, u32_Len);


	if ((u32_Len == 2) && (*s8_Buf == 0x0d) && (*(s8_Buf+1) == 0x0a))
		m_bEOD = TRUE;
	else
	{
		memset(m_SocketDev.cData, 0, sizeof(m_SocketDev.cData));
		memcpy(m_SocketDev.cData, s8_Buf, u32_Len);
	}

	// Delete all received data from the receive memory
	pi_RecvMem->DeleteLeft(u32_Len);
}

BOOL CADST3_VS80Dlg::SendTo(SOCKET h_Socket, CString s_SendData) 
{
	CString str;
	CString s_Text = s_SendData;
	if (s_Text.GetLength() > 50)
		s_Text = s_Text.Left(50) + _T("...<cut>");

	s_SendData.Replace(_T("\\n"), _T("\n"));
	s_SendData.Replace(_T("\\r"), _T("\r"));

	// If Unicode: 1 character = 2 Bytes!
	DWORD u32_Len = s_SendData.GetLength();// * sizeof(TCHAR);

	//str.Format(_T("Sending %d Bytes to %s: '%s'"), u32_Len, FormatDisplayName(h_Socket), s_Text);
	//UpdateMsg(str, 0);

	m_bEOD = FALSE;
	//char* s8_Data = (char*)(const TCHAR*)s_SendData; // get buffer AFTER Insert() !!!

	wchar_t* wc_Data = s_SendData.GetBuffer(s_SendData.GetLength());
	int char_str_len = WideCharToMultiByte(CP_ACP, 0, wc_Data, -1, NULL, 0, NULL, NULL);
	char* s8_Data = new char[char_str_len]; // get buffer AFTER Insert() !!!

	WideCharToMultiByte(CP_ACP, 0, wc_Data, -1, s8_Data, char_str_len, 0,0);

	DWORD u32_Err = m_SocketDev.cSocket.SendTo(h_Socket, s8_Data, u32_Len);

	switch (u32_Err)
	{
	case 0:
		return TRUE;

	case WSAEWOULDBLOCK:
		UpdateMsg(_T("WSAEWOULDBLOCK -> The data will be send after the next FD_WRITE event."), 1);
		return TRUE;

	case WSA_IO_PENDING:
		UpdateMsg(_T("WSA_IO_PENDING -> Error: A previous Send operation is still pending. This data will not be sent."), 1);
		return TRUE;

	default:
		str.Format(_T("%s"), _T(" -> Error ") + GetSocketErrMsg(u32_Err));
		UpdateMsg(str, 1);
		// Severe error -> abort event loop
		CloseSockets();
		return FALSE; 
	};
}

// Close all open sockets (if any)
void CADST3_VS80Dlg::CloseSockets() 
{
	if (m_SocketDev.cSocket.GetSocketCount())
	{
		m_SocketDev.cSocket.Close();
		UpdateMsg(_T("Socket(s) closed."), MT_MSG);
	}
}

// Allows to update the GUI from within an endless loop without needing an extra thread
void CADST3_VS80Dlg::PumpMessages()
{
	MSG k_Msg;
	while (PeekMessage(&k_Msg, NULL, NULL, NULL, PM_NOREMOVE))
	{
		AfxGetThread()->PumpMessage();
	}
}

CString CADST3_VS80Dlg::CopyToString(char* s8_Buf, DWORD u32_Bytes)
{
	DWORD u32_StrLen = u32_Bytes / sizeof(TCHAR);

	CString s_String;
	char*  s8_String = (char*)s_String.GetBuffer(u32_StrLen+1);
	memcpy(s8_String, s8_Buf, u32_Bytes);
	s_String.ReleaseBuffer(u32_StrLen);

	return s_String;
}


// Get a human readable error message for an API error code
CString CADST3_VS80Dlg::GetSocketErrMsg(DWORD u32_Error)
{
	// Some translations of error codes are really stupid --> show the original error code.
	CString s_Code;
	switch (u32_Error)
	{
	case WSAEINTR:                s_Code = _T("WSAEINTR"); break;
	case WSAEBADF:                s_Code = _T("WSAEBADF"); break;
	case WSAEACCES:               s_Code = _T("WSAEACCES"); break;
	case WSAEFAULT:               s_Code = _T("WSAEFAULT"); break;
	case WSAEINVAL:               s_Code = _T("WSAEINVAL"); break;
	case WSAEMFILE:               s_Code = _T("WSAEMFILE"); break;
	case WSAEWOULDBLOCK:          s_Code = _T("WSAEWOULDBLOCK"); break;
	case WSAEINPROGRESS:          s_Code = _T("WSAEINPROGRESS"); break;
	case WSAEALREADY:             s_Code = _T("WSAEALREADY"); break;
	case WSAENOTSOCK:             s_Code = _T("WSAENOTSOCK"); break;
	case WSAEDESTADDRREQ:         s_Code = _T("WSAEDESTADDRREQ"); break;
	case WSAEMSGSIZE:             s_Code = _T("WSAEMSGSIZE"); break;
	case WSAEPROTOTYPE:           s_Code = _T("WSAEPROTOTYPE"); break;
	case WSAENOPROTOOPT:          s_Code = _T("WSAENOPROTOOPT"); break;
	case WSAEPROTONOSUPPORT:      s_Code = _T("WSAEPROTONOSUPPORT"); break;
	case WSAESOCKTNOSUPPORT:      s_Code = _T("WSAESOCKTNOSUPPORT"); break;
	case WSAEOPNOTSUPP:           s_Code = _T("WSAEOPNOTSUPP"); break;
	case WSAEPFNOSUPPORT:         s_Code = _T("WSAEPFNOSUPPORT"); break;
	case WSAEAFNOSUPPORT:         s_Code = _T("WSAEAFNOSUPPORT"); break;
	case WSAEADDRINUSE:           s_Code = _T("WSAEADDRINUSE"); break;
	case WSAEADDRNOTAVAIL:        s_Code = _T("WSAEADDRNOTAVAIL"); break;
	case WSAENETDOWN:             s_Code = _T("WSAENETDOWN"); break;
	case WSAENETUNREACH:          s_Code = _T("WSAENETUNREACH"); break;
	case WSAENETRESET:            s_Code = _T("WSAENETRESET"); break;
	case WSAECONNABORTED:         s_Code = _T("WSAECONNABORTED"); break;
	case WSAECONNRESET:           s_Code = _T("WSAECONNRESET"); break;
	case WSAENOBUFS:              s_Code = _T("WSAENOBUFS"); break;
	case WSAEISCONN:              s_Code = _T("WSAEISCONN"); break;
	case WSAENOTCONN:             s_Code = _T("WSAENOTCONN"); break;
	case WSAESHUTDOWN:            s_Code = _T("WSAESHUTDOWN"); break;
	case WSAETOOMANYREFS:         s_Code = _T("WSAETOOMANYREFS"); break;
	case WSAETIMEDOUT:            s_Code = _T("WSAETIMEDOUT"); break;
	case WSAECONNREFUSED:         s_Code = _T("WSAECONNREFUSED"); break;
	case WSAELOOP:                s_Code = _T("WSAELOOP"); break;
	case WSAENAMETOOLONG:         s_Code = _T("WSAENAMETOOLONG"); break;
	case WSAEHOSTDOWN:            s_Code = _T("WSAEHOSTDOWN"); break;
	case WSAEHOSTUNREACH:         s_Code = _T("WSAEHOSTUNREACH"); break;
	case WSAENOTEMPTY:            s_Code = _T("WSAENOTEMPTY"); break;
	case WSAEPROCLIM:             s_Code = _T("WSAEPROCLIM"); break;
	case WSAEUSERS:               s_Code = _T("WSAEUSERS"); break;
	case WSAEDQUOT:               s_Code = _T("WSAEDQUOT"); break;
	case WSAESTALE:               s_Code = _T("WSAESTALE"); break;
	case WSAEREMOTE:              s_Code = _T("WSAEREMOTE"); break;
	case WSASYSNOTREADY:          s_Code = _T("WSASYSNOTREADY"); break;
	case WSAVERNOTSUPPORTED:      s_Code = _T("WSAVERNOTSUPPORTED"); break;
	case WSANOTINITIALISED:       s_Code = _T("WSANOTINITIALISED"); break;
	case WSAEDISCON:              s_Code = _T("WSAEDISCON"); break;
	case WSAENOMORE:              s_Code = _T("WSAENOMORE"); break;
	case WSAECANCELLED:           s_Code = _T("WSAECANCELLED"); break;
	case WSAEINVALIDPROCTABLE:    s_Code = _T("WSAEINVALIDPROCTABLE"); break;
	case WSAEINVALIDPROVIDER:     s_Code = _T("WSAEINVALIDPROVIDER"); break;
	case WSAEPROVIDERFAILEDINIT:  s_Code = _T("WSAEPROVIDERFAILEDINIT"); break;
	case WSASYSCALLFAILURE:       s_Code = _T("WSASYSCALLFAILURE"); break;
	case WSASERVICE_NOT_FOUND:    s_Code = _T("WSASERVICE_NOT_FOUND"); break;
	case WSATYPE_NOT_FOUND:       s_Code = _T("WSATYPE_NOT_FOUND"); break;
	case WSA_E_NO_MORE:           s_Code = _T("WSA_E_NO_MORE"); break;
	case WSA_E_CANCELLED:         s_Code = _T("WSA_E_CANCELLED"); break;
	case WSAEREFUSED:             s_Code = _T("WSAEREFUSED"); break;
	case WSAHOST_NOT_FOUND:       s_Code = _T("WSAHOST_NOT_FOUND"); break;
	case WSATRY_AGAIN:            s_Code = _T("WSATRY_AGAIN"); break;
	case WSANO_RECOVERY:          s_Code = _T("WSANO_RECOVERY"); break;
	case WSANO_DATA:              s_Code = _T("WSANO_DATA"); break;
	case WSA_IO_PENDING:          s_Code = _T("WSA_IO_PENDING"); break;
	case WSA_IO_INCOMPLETE:       s_Code = _T("WSA_IO_INCOMPLETE"); break;
	case WSA_INVALID_HANDLE:      s_Code = _T("WSA_INVALID_HANDLE"); break;
	case WSA_INVALID_PARAMETER:   s_Code = _T("WSA_INVALID_PARAMETER"); break;
	case WSA_NOT_ENOUGH_MEMORY:   s_Code = _T("WSA_NOT_ENOUGH_MEMORY"); break;
	case WSA_OPERATION_ABORTED:   s_Code = _T("WSA_OPERATION_ABORTED"); break;

	default:
		s_Code.Format(_T("Code %u"), u32_Error); 
		break;
	}

	CString s_Out;
	const DWORD BUFLEN = 1000;
	TCHAR t_Buf[BUFLEN];

	if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, u32_Error, 0, t_Buf, BUFLEN, 0))
		s_Out.Format(_T("%s: %s"), s_Code, t_Buf);
	else 
		s_Out.Format(_T("%s: Windows has no explanation for this error"), s_Code);

	s_Out.TrimRight(); // some messages end with useless Linefeeds
	return s_Out;
}


void CADST3_VS80Dlg::SetAOState(UINT8 nIndex)
{
	BOOL	bIsStart = FALSE;
	CString	str;

	for (int i=0; i<m_aioTarget.nAOChNum; i++)
	{
		if ((i!=nIndex) && (m_bAOChStarted[i]))
		{
			m_btnAO[i].SetBitmaps(IDB_BMP_BTN_START, MASK_COLOR, (int)BTNST_AUTO_DARKER);
			m_sttAO[i].StartBkBlink(FALSE);
			// nhqt
			//m_sttAO[i].SetWindowTextW(_T("Ready"));
			m_sttAO[i].SetWindowTextW(_T("Stopped"));
			m_sttAO[i].SetBkColor(CL_READY);
			m_bAOChStarted[i] = FALSE;
			independent(nIndex + 8 * m_spnInpByte.GetPos32(), str, 1, 0);
		}
	}

	if (m_bAOChStarted[nIndex])
	{
		independent(nIndex + 8 * m_spnInpByte.GetPos32(), str, 1, 0);
		m_btnAO[nIndex].SetBitmaps(IDB_BMP_BTN_START, MASK_COLOR, (int)BTNST_AUTO_DARKER);
		m_sttAO[nIndex].StartBkBlink(FALSE, 500);
		m_sttAO[nIndex].SetWindowTextW(_T("Stopped"));
		m_sttAO[nIndex].SetBkColor(CL_STOP);
		str.Format(_T("[AOCH%d] Test stopped!"), nIndex+8*m_spnOutpByte.GetPos32());
		UpdateMsg(str, MT_MSG);
	}
	else
	{
		m_aioTarget.nAOPtrnIndex = 0;
		m_aioTarget.nAOSampleCnt = 0;
		m_nSocketTimeout = 0;

		independent(nIndex + 8 * m_spnInpByte.GetPos32(), str, 0, 0);

		m_btnAO[nIndex].SetBitmaps(IDB_BMP_BTN_STOP, MASK_COLOR, (int)BTNST_AUTO_DARKER);
		m_sttAO[nIndex].StartBkBlink(TRUE, 500);
		m_sttAO[nIndex].SetWindowTextW(_T("Running"));
		m_sttAO[nIndex].SetBkColor(CL_RUNNING);
		str.Format(_T("[AOCH%d] Test started!"), nIndex+8*m_spnOutpByte.GetPos32());
		UpdateMsg(str, MT_MSG);

		for (int i=0; i</*m_aioTarget.nAOChNum*/ MAX_AO; i++)
			bIsStart |= m_bAOChStarted[i];

		// if first start => send first READ? cmd, start timer 
		if (!bIsStart)
		{
			SetTimer(ID_TMR_PROG_AO, 1, NULL);
			m_bEOD = FALSE;
			if( !SendTo(m_SocketDev.hSocket, _T("READ?\r\n")) )
				UpdateMsg(_T("Get Input Data Error!"), MT_ERR);
			m_spnInpByte.EnableWindow(FALSE);
		}
		QueryPerformanceCounter(&m_StartCnt[nIndex+10]);
	}
	m_bAOChStarted[nIndex] = !m_bAOChStarted[nIndex];

	// last stop, kill update TMR
	// nhqt
	//for (int i=0; i<m_aioTarget.nAOChNum; i++)
	//{
	//	if (m_bAOChStarted[i])
	//		return;
	//}
	//KillTimer(ID_TMR_PROG_AO);
}

void CADST3_VS80Dlg::SetAIState(UINT8 nIndex)
{
	int ret;
	CString str;
	CString strTemp;
	BOOL	bIsStart = FALSE;

	//nhqt
	for (int i=0; i<m_aioTarget.nAIChNum; i++)
	{
		if ((i!=nIndex) && (m_bAIChStarted[i]))
		{
			m_btnAI[i].SetBitmaps(IDB_BMP_BTN_START, MASK_COLOR, (int)BTNST_AUTO_DARKER);
			m_sttAI[i].StartBkBlink(FALSE);
			// nhqt
			//m_sttAO[i].SetWindowTextW(_T("Ready"));
			m_sttAI[i].SetWindowTextW(_T("Stopped"));
			m_sttAI[i].SetBkColor(CL_READY);
			m_bAIChStarted[i] = FALSE;		
			independent(nIndex + 8 * m_spnInpByte.GetPos32(), strTemp, 1);
		}
	}

	if (m_bAIChStarted[nIndex])
	{

		independent(nIndex + 8 * m_spnInpByte.GetPos32(), strTemp, 1);

		m_btnAI[nIndex].SetBitmaps(IDB_BMP_BTN_START, MASK_COLOR, (int)BTNST_AUTO_DARKER);
		m_sttAI[nIndex].StartBkBlink(FALSE, 500);
		m_sttAI[nIndex].SetWindowTextW(_T("Stopped"));
		m_sttAI[nIndex].SetBkColor(CL_STOP);
		str.Format(_T("[AICH%d] Test stopped!"), nIndex+8*m_spnInpByte.GetPos32());
		UpdateMsg(str, MT_MSG);
		
	}
	//실제 검사 코드
	else
	{
		m_aioTarget.nAIPtrnIndex = 0;
		m_aioTarget.nAISampleCnt = 0;

			independent(nIndex + 8 * m_spnInpByte.GetPos32(), strTemp, 0);

			m_btnAI[nIndex].SetBitmaps(IDB_BMP_BTN_STOP, MASK_COLOR, (int)BTNST_AUTO_DARKER);
			m_sttAI[nIndex].StartBkBlink(TRUE, 500);
			m_sttAI[nIndex].SetWindowTextW(_T("Running"));
			m_sttAI[nIndex].SetBkColor(CL_RUNNING);
			str.Format(_T("[AICH%d] Test started!"), nIndex + 8 * m_spnInpByte.GetPos32());
			UpdateMsg(str, MT_MSG);

			for (int i = 0; i </*m_aioTarget.nAIChNum*/ MAX_AI; i++)
				bIsStart |= m_bAIChStarted[i];

			// if first start => output ON, start timer 
			if (!bIsStart)
			{
				ret = TmcSend(m_YKGWDevId, ":OUTP ON");
				if (ret != 0)
					UpdateMsg(_T("Yokogawa Output On Error"), MT_ERR);
				SetTimer(ID_TMR_PROG_AI, 1, NULL);
				m_spnInpByte.EnableWindow(FALSE);
			}
			QueryPerformanceCounter(&m_StartCnt[nIndex]);
		}

		m_bAIChStarted[nIndex] = !m_bAIChStarted[nIndex];
	// last stop, output OFF
	//for (int i=0; i</*m_aioTarget.nAIChNum*/ MAX_AI; i++)
	//{
	//	if (m_bAIChStarted[i])
	//		return;
	//}
	//KillTimer(ID_TMR_PROG_AI);
	//ret = TmcSend( m_YKGWDevId, ":OUTP OFF");
	//if (ret != 0)
	//	UpdateMsg(_T("Yokogawa Output OFF Error!"), MT_ERR);
}

void CADST3_VS80Dlg::OnBnClickedBtnOut(UINT nID)
{

	if (!m_bFlukeInit)
		return;

	SetAOState(nID - IDC_BTN_OUT0);
}

void CADST3_VS80Dlg::OnBnClickedBtnIn(UINT nID)
{
	if (!m_bYKGWInit)
		return;

	SetAIState(nID - IDC_BTN_IN0);
}

void CADST3_VS80Dlg::OnBnClickedBtnYkgwSetting()
{
	if (YokogawaInit())
		UpdateMsg(_T("Yokogawa Init fail!"), MT_ERR);
}


void CADST3_VS80Dlg::OnBnClickedBtnFlukeSetting()
{
	if (m_SocketDev.cSocket.GetSocketCount())
	{
		SendTo(m_SocketDev.hSocket, _T("SYST:LOCAL\r\n"));
		Sleep(100);
		CloseSockets();
	}

	if (FlukeEthInit())
		UpdateMsg(_T("Fluke Init fail!"), MT_ERR);
}

void CADST3_VS80Dlg::OpenCfgFile()
{
	this->UpdateData();

	CFile f;

	LPCTSTR strFilter = _T("CFG Files (*.cfg)|*.cfg|All Files (*.*)|*.*||" );

	CFileDialog FileDlg(TRUE, _T(".cfg"), NULL, 0, strFilter);

	TCHAR Path[255];
	GetCurrentDirectory(255, Path);
	CString strPath = CString(Path);
	strPath += _T("\\config");
	FileDlg.m_ofn.lpstrInitialDir = strPath;

	CString	strFileName;

	if( FileDlg.DoModal() == IDOK )
	{
		// 		if( f.Open(FileDlg.GetFileName(), CFile::modeReadWrite) == FALSE )
		// 			return;
		strFileName = FileDlg.GetPathName();
	}
	else
		return;
	//	f.Close();
	ShellExecute( NULL, _T("open"), /*_T("c:\\windows\\notepad.exe")*/_T("NOTEPAD"), strFileName, _T(""), SW_SHOWNORMAL) ;
	this->UpdateData(FALSE);
}

void CADST3_VS80Dlg::OnBnClickedBtnYkgwFileOpen()
{
	OpenCfgFile();
}


void CADST3_VS80Dlg::OnBnClickedBtnFlukeFileOpen()
{
	OpenCfgFile();
}


void CADST3_VS80Dlg::OnBnClickedBtnProdFileOpen()
{
	OpenCfgFile();
}

void CADST3_VS80Dlg::OnBnClickedBtnClrlog()
{
	if (m_lcLog.GetItemCount() && (MessageBox(_T("Clear all log data?"), _T("Clear Log Confirm"), MB_YESNO | MB_ICONQUESTION) == IDYES))
		m_lcLog.DeleteAllItems();
}
void CADST3_VS80Dlg::OnDeltaposSpinInpbyte(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nLower, nUpper;
	CString csChNo;
	m_spnInpByte.GetRange32(nLower, nUpper);
	int nPos = pNMUpDown->iPos + pNMUpDown->iDelta;
	if (nPos>nUpper)	
		nPos = nLower;
	if (nPos<nLower)
		nPos = nUpper;
	for (int i=0; i<8; i++)
	{
		int nChNo = nPos*8 + i;
		csChNo.Format(_T("IN%d"), nChNo);
		GetDlgItem(IDC_STT_INCH0+i)->SetWindowTextW(csChNo);
	}

	*pResult = 0;
}

void CADST3_VS80Dlg::OnDeltaposSpinOutpbyte(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nLower, nUpper;
	CString csChNo;

	m_spnOutpByte.GetRange32(nLower, nUpper);
	int nPos = pNMUpDown->iPos + pNMUpDown->iDelta;
	if (nPos>nUpper)	
		nPos = nLower;
	if (nPos<nLower)
		nPos = nUpper;
	for (int i=0; i<8; i++)
	{
		int nChNo = nPos*8 + i;
		csChNo.Format(_T("OUT%d"), nChNo);
		GetDlgItem(IDC_STT_OUTCH0+i)->SetWindowTextW(csChNo);
	}

	*pResult = 0;
}


// 시리얼통신 연결함수 2018-06-14
int CADST3_VS80Dlg::connect(CString portNum)
{
	if (!serial.OpenPort(portNum)) //포트를 오픈하고 오픈에 실패하였으면 fail을 반환한다.
		return RETURN_FAIL;

	serial.ConfigurePort(CBR_9600, 8, FALSE, NOPARITY, ONESTOPBIT); //포트 기본값을 설정한다.
	serial.SetCommunicationTimeouts(0, 0, 0, 0, 0); //Timeout값 설정
	m_bConnect = TRUE;

	return RETURN_SUCCESS;
}


// 시리얼통신을 통해 문자열전송 2018-06-14
int CADST3_VS80Dlg::sendCommand(char pos)
{
	if (serial.WriteByte(pos)) //WriteByte()를 통해 데이터 전송에 성공하면 SUCCESS, 실패하면 FAIL을 반환한다.
		return RETURN_SUCCESS;
	else
		return RETURN_FAIL;
}


// 시리얼통신 연결해제 2018-06-14
void CADST3_VS80Dlg::disconnect()
{
	serial.ClosePort();
}


void CADST3_VS80Dlg::OnBnClickedBtnAutorun()
{
	
	AutorunDialog dlg;
	INT_PTR nResponse = dlg.DoModal();	//다이얼로그를 하나 띄위 자동검사를 시작할 제품을 선택한다.

	if (!(nResponse == IDOK))	//선택한 제품의 코드를 반환하여 해당 제품에 맞는 검사 시작
		return;
	//{

	//if ((YokogawaInit() != 0) | (FlukeEthInit() != 0))
	//{
	//	return;
	//}

	long lModuleCnt;
	AxaInfoGetModuleCount(&lModuleCnt);

	//for (size_t i = 0; i < lModuleCnt; i++)
	//{
	//	AxaInfoGetModule(i, &m_lBoardNo, &m_lModulePos[i], &m_dwModuleIDs[i]);
	//}
	////AxaInfoGetModule(m_lModuleNo, &m_lBoardNo, &m_lModulePos, &m_dwModuleIDs);
	//
	//if (m_dwModuleIDs[m_TotalTestCount] == AXT_SIO_RAI16RB)
	//	m_Autorun_product_num = PRO_N3AI16;
	//else if (m_dwModuleIDs[m_TotalTestCount] == AXT_SIO_RAI8RB)
	//	m_Autorun_product_num = PRO_N3AO8;
	//else if (m_dwModuleIDs[m_TotalTestCount] == AXT_SIO_AI8AO4F)
	//	m_Autorun_product_num = PRO_N3AO8;
	//else if (m_dwModuleIDs[m_TotalTestCount] == AXT_SIO_RAI8RB)
	//	m_Autorun_product_num = PRO_N3AO8;
	//else if (m_dwModuleIDs[m_TotalTestCount] == AXT_SIO_RAI8RB)
	//	m_Autorun_product_num = PRO_N3AO8;
	//else if (m_dwModuleIDs[m_TotalTestCount] == AXT_SIO_RAI8RB)
	//	m_Autorun_product_num = PRO_N3AO8;
	//else if (m_dwModuleIDs[m_TotalTestCount] == AXT_SIO_RAI8RB)
	//	m_Autorun_product_num = PRO_N3AO8;

		if (m_csModuleName.Find(_T("AI16")) != -1)
			m_Autorun_product_num = PRO_N3AI16;
		else if (m_csModuleName.Find(_T("AO08")) != -1)
			m_Autorun_product_num = PRO_N3AO8;
		else if (m_csModuleName.Find(_T("AI8AO4")) != -1)
			m_Autorun_product_num = PRO_SIOAI8AO4;
		else if (m_csModuleName.Find(_T("AI8F")) != -1)
			m_Autorun_product_num = PRO_SIOAI8;
		else if (m_csModuleName.Find(_T("AO4F")) != -1)
			m_Autorun_product_num = PRO_SIOAO4;
		else if (m_csModuleName.Find(_T("AI4RB")) != -1)
			m_Autorun_product_num = PRO_SIOAI4RB;
		else if (m_csModuleName.Find(_T("AO4RB")) != -1)
			m_Autorun_product_num = PRO_SIOAO4RB;

			switch (m_Autorun_product_num)
			{
			case PRO_N3AI16:
				for (size_t i = 0; i < MAX_AI * 2; i++)
				{
					Auto_start_AI16[i] = false;
				}
				break;

			case PRO_N3AO8:
				for (size_t j = 0; j < MAX_AO; j++)
				{
					Auto_start_AO8[j] = false;
				}
				break;

			case PRO_SIOAI8AO4:
				//if (m_lModuleCnt >= 1)
				//	m_bproductCnt[PRO_SIOAI8AO4] = true;

				for (size_t k = 0; k < MAX_AI; k++)
					Auto_start_AI8[k] = false;
				for (size_t i = 0; i < MAX_AO; i++)
					Auto_start_AO4[i] = false;
				m_iTestStage = 0;
				break;

			case PRO_SIOAI8:
				m_bis8 = true;
				for (size_t l = 0; l < MAX_AI; l++)
				{
					Auto_start_AI8[l] = false;
					break;
				}

			case PRO_SIOAO4:	//TODO : AO검사로 변경할 것
				m_bis8 = true;
				for (size_t m = 0; m < MAX_AI; m++)
				{
					Auto_start_AI8[m] = false;
				}
				break;

			case PRO_SIOAI4RB:
				m_bis8 = true;
				for (size_t n = 0; n < MAX_AI; n++)
				{
					Auto_start_AI8[n] = false;
				}
				break;

			case PRO_SIOAO4RB:
				m_bis8 = true;
				for (size_t o = 0; o < MAX_AI; o++)
				{
					Auto_start_AI8[o] = false;
				}
				break;

			default:
				break;
			}

		m_TestCount = 0;
		GetDlgItem(IDC_BTN_AUTORUN)->EnableWindow(false);
		SetTimer(ID_TMR_PROGRESS_AUTO, 100, NULL);
		SetTimer(ID_TMR_CHECK_END, 50, NULL);
	//}

	//else if (nResponse == IDCANCEL)
	//{
	//	return;
	//}

}


// CString을 SerialData로 송신하기 위한 Cahr* Coverting Function
void CADST3_VS80Dlg::CstringToChar(CString strData)
{
	char* st = LPSTR(LPCTSTR(strData));		// CString to char*
	unsigned int iLength = strlen(st);		// Length of char 

	for (size_t i = 0; i < iLength; i++)
	{
		sendCommand(st[i]);
		Sleep(10);
	}
}


// AI자동 검사 시 현재 작동 중인 위치 추적
void CADST3_VS80Dlg::CheckforAI()
{
	//TODO 쓸 일이 없을 듯
}


void CADST3_VS80Dlg::OnBnClickedButton3()
{
	// AutoRUN Stop버튼 관련 코드
	KillTimer(ID_TMR_PROGRESS_AUTO);
	if (m_bAIChStarted[m_TestCount])
	{
		SetAIState(m_TestCount);
	}

	else if (m_bAOChStarted[m_TestCount])
	{
		SetAOState(m_TestCount);
	}
	sendCommand('c');	//Relay 초기화
	m_TestCount = 0;	//TestCount 초기화
	m_ProductCnt = 0;
	GetDlgItem(IDC_BTN_AUTORUN)->EnableWindow(true);
}


void CADST3_VS80Dlg::independent(int chIndex, CString strTemp, bool Rstate, bool VolnCur)
{
	long lBoardNo;
	long lModulePos;
	DWORD dwModuleID;
	CString	strData;
	//sendCommand('c');
	Sleep(20);

	if (AxaInfoGetModule(m_lModuleNo, &lBoardNo, &lModulePos, &dwModuleID) == AXT_RT_SUCCESS)	//TODO: N3 Slave 검사 시 정상작동 하는지 검토 필요
	{
		//strData.Format(_T("No. %d"), m_lModuleNo);
		//UpdateMsg(strData, MT_MSG);
		//strData.Format(_T("Pos. %d"), lModulePos);
		//UpdateMsg(strData, MT_MSG);
		//strData.Format(_T("ID. %x"), dwModuleID);
		//UpdateMsg(strData, MT_MSG);
	}
	else
	{
		AfxMessageBox(_T("axa읽기 실패"));
		return;
	}

	//AfxMessageBox(strData);

	if (dwModuleID == AXT_SIO_RAI16RTEX  ||
		dwModuleID == AXT_SIO_RAI16MLIII ||
		dwModuleID == AXT_SIO_RAI16SIIIH)
	{
		//strData.Format(_T("In RAI16 R M S"));
		//UpdateMsg(strData, MT_MSG);
		if (!VolnCur)
			sendCommand('t');
		else
			sendCommand('T');

		if (chIndex > 9)
		{
			strTemp.Format(_T("%d"), chIndex);
			CstringToChar(strTemp.Left(1));
			CstringToChar(strTemp.Right(1));
		}

		else
		{
			strTemp.Format(_T("%d"), chIndex);
			CstringToChar(strTemp);
		}

		if (Rstate)
		{
			strTemp.Format(_T("I"));
			CstringToChar(strTemp);
		}

		else
		{
			strTemp.Format(_T("i"));
			CstringToChar(strTemp);
		}
	}

	else if (dwModuleID == AXT_SIO_RAO08RTEX ||
		     dwModuleID == AXT_SIO_RAO8MLIII ||
			 dwModuleID == AXT_SIO_RAO08SIIIH)
	{
		//strData.Format(_T("In RAO08 R M S"));
		//UpdateMsg(strData, MT_MSG);
		strTemp.Format(_T("%d"), chIndex);
		CstringToChar(strTemp);

		if (Rstate)
		{
			strTemp.Format(_T("I"));
			CstringToChar(strTemp);
		}

		else
		{
			strTemp.Format(_T("i"));
			CstringToChar(strTemp);
		}
	}

	else if (dwModuleID == AXT_SIO_AI8AO4F)
	{
		//strData.Format(_T("In AI8AO4F"));
		//UpdateMsg(strData, MT_MSG);
		if (lModulePos % 2)
			chIndex += 8;

		if ((lModulePos > 1) || !VolnCur)
			sendCommand('t');
		else
			sendCommand('T');

		if (chIndex > 9)
		{
			strTemp.Format(_T("%d"), chIndex);
			CstringToChar(strTemp.Left(1));
			CstringToChar(strTemp.Right(1));
		}

		else
		{
			strTemp.Format(_T("%d"), chIndex);
			CstringToChar(strTemp);
		}

		Sleep(50);
		if (VolnCur)
		{
			if (Rstate)
			{
				strTemp.Format(_T("I"));
				CstringToChar(strTemp);
			}

			else
			{
				strTemp.Format(_T("i"));
				CstringToChar(strTemp);
			}
		}

		else
		{
			if (Rstate)
			{
				strTemp.Format(_T("A"));
				CstringToChar(strTemp);
			}

			else
			{
				strTemp.Format(_T("a"));
				CstringToChar(strTemp);
			}
		}
	}

	else if (dwModuleID == AXT_SIO_AI8F)
	{
		//strData.Format(_T("In AI8F"));
		//UpdateMsg(strData, MT_MSG);
		//(int)lModulePos;
		//int temp = (int)lModulePos;
		if ((lModulePos % 2))
			chIndex += 8;

		//strData.Format(_T("TEMP. %d"), lModulePos);
		//UpdateMsg(strData, MT_MSG);

		if ((lModulePos > 1))
			sendCommand('t');
		else
			sendCommand('T');

		if (chIndex > 9)
		{
			strTemp.Format(_T("%d"), chIndex);
			CstringToChar(strTemp.Left(1));
			CstringToChar(strTemp.Right(1));
		}

		else
		{
			strTemp.Format(_T("%d"), chIndex);
			CstringToChar(strTemp);
		}

		Sleep(50);
		if (Rstate)
		{
			strTemp.Format(_T("I"));
			CstringToChar(strTemp);
		}

		else
		{
			strTemp.Format(_T("i"));
			CstringToChar(strTemp);
		}
	}

	else if (dwModuleID == AXT_SIO_AO4F)
	{
	//	strData.Format(_T("In RAO4F"));
		//UpdateMsg(strData, MT_MSG);
		if (lModulePos % 2)
			chIndex += 4;

		if ((lModulePos > 1) || !VolnCur)
			sendCommand('t');
		else
			sendCommand('T');

		if (chIndex > 9)
		{
			strTemp.Format(_T("%d"), chIndex);
			CstringToChar(strTemp.Left(1));
			CstringToChar(strTemp.Right(1));
		}

		else
		{
			strTemp.Format(_T("%d"), chIndex);
			CstringToChar(strTemp);
		}

		Sleep(50);

		if (Rstate)
		{
			strTemp.Format(_T("A"));
			CstringToChar(strTemp);
		}

		else
		{
			strTemp.Format(_T("a"));
			CstringToChar(strTemp);
		}
	}

	else if (dwModuleID == AXT_SIO_AI4RB)
	{
		//strData.Format(_T("In AI4RB"));
		//UpdateMsg(strData, MT_MSG);
		sendCommand('x');	//Trigger Signal On

		chIndex = (2 * chIndex + 2);	// An = 2a+2

		if (chIndex > 9)
		{
			strTemp.Format(_T("%d"), chIndex);
			CstringToChar(strTemp.Left(1));
			CstringToChar(strTemp.Right(1));
		}

		else
		{
			strTemp.Format(_T("%d"), chIndex);
			CstringToChar(strTemp);
		}

		if (Rstate)
		{
			strTemp.Format(_T("I"));
			CstringToChar(strTemp);
		}

		else
		{
			strTemp.Format(_T("i"));
			CstringToChar(strTemp);
		}
	}

	else if (dwModuleID == AXT_SIO_AO4RB)
	{
		//strData.Format(_T("In RO4RB"));
		//UpdateMsg(strData, MT_MSG);
		sendCommand('x');	//Trigger Signal On
		//if (lModulePos > 0)
		//{
			chIndex += 8;
		//}

		//chIndex = (2 * chIndex + 2);	// An = 2a+2

		if (chIndex > 9)
		{
			strTemp.Format(_T("%d"), chIndex);
			CstringToChar(strTemp.Left(1));
			CstringToChar(strTemp.Right(1));
		}

		else
		{
			strTemp.Format(_T("%d"), chIndex);
			CstringToChar(strTemp);
		}

		if (Rstate)
		{
			strTemp.Format(_T("I"));
			CstringToChar(strTemp);
		}

		else
		{
			strTemp.Format(_T("i"));
			CstringToChar(strTemp);
		}
	}

	else
	{
		AfxMessageBox(_T("제품을 찾을 수 없습니다."));
	}

	/*
	if (lModulePos % 2)
		chIndex += 8;

	if ((lModulePos > 1) || !VolnCur)
		sendCommand('t');
	else
		sendCommand('T');

	if (chIndex > 9)
	{
		strTemp.Format(_T("%d"), chIndex);
		CstringToChar(strTemp.Left(1));
		CstringToChar(strTemp.Right(1));
	}

	else
	{
		strTemp.Format(_T("%d"), chIndex);
		CstringToChar(strTemp);
	}

	Sleep(50);
	if (VolnCur)
	{
		if (Rstate)
		{
			strTemp.Format(_T("I"));
			CstringToChar(strTemp);
		}

		else
		{
			strTemp.Format(_T("i"));
			CstringToChar(strTemp);
		}
	}

	else
	{
		if (Rstate)
		{
			strTemp.Format(_T("A"));
			CstringToChar(strTemp);
		}

		else
		{
			strTemp.Format(_T("a"));
			CstringToChar(strTemp);
		}
	}
	*/

}


// AI자동검사 시 결과를 판단하는 함수
void CADST3_VS80Dlg::CheckAIAutorunResult(int nCount)
{
	CString strTemp;
	m_iAgingCount = GetDlgItemInt(IDC_EDIT_AGING);
	independent(nCount + 8 * m_spnInpByte.GetPos32(), strTemp, 1);	//검사완료 된 릴레이 off


	if (m_bTotalTestFinished)	//전체 테스트 종료
	{
		m_bTotalTestFinished = false;
		for (size_t i = 0; i < ID_TMR_TOTAL; i++)
		{
			KillTimer(i);
		}

		//m_TestCount = 0;
		return;
	}

	switch (m_Autorun_product_num)
	{
	case PRO_N3AI16:
		Auto_start_AI16[nCount + 8 * m_spnInpByte.GetPos32()] = true;
		m_TestCount++;	//현재 Ch 추적

		if (m_TestCount > 7 && (m_spnInpByte.GetPos32() == 0))
		{
			m_TestCount = 0;
			m_boverthe8 = true;
			
			for (int i = 0; i < 8; i++)
			{
				int nChNo = 8 + i;
				strTemp.Format(_T("IN%d"), nChNo);
				GetDlgItem(IDC_STT_INCH0 + i)->SetWindowTextW(strTemp);
			}

			m_spnInpByte.SetPos32(m_spnInpByte.GetPos32() + 1);

			if (m_boverthe8)
			{
				m_boverthe8 = false;
				m_boverthe16 = true;

				for (size_t i = 0; i < 16; i++)
					Auto_start_AI16[i] = false;
			}
		}

		else if (m_TestCount > 7 && (m_spnInpByte.GetPos32() == 1))
		{
			m_TestCount = 0;
			m_bfinished = true;
			KillTimer(ID_TMR_PROGRESS_AUTO);
			KillTimer(ID_TMR_PROG_AI);

			BnClickedBtnPrev();
			if (m_chkInpMode.GetCheck() == BST_CHECKED)
			{
				m_chkInpMode.SetCheck(0);
				BntChkInpMode();
			}

			else
			{
				m_chkInpMode.SetCheck(1);
				BntChkInpMode();
			}

			AfxMessageBox(_T("전류모드 스위치 변경 후 클릭"));

			for (size_t i = 0; i < 16; i++)
				Auto_start_AI16[i] = false;

			Sleep(100);
			SetTimer(ID_TMR_PROGRESS_AUTO, 100, NULL);
		}

		else if (m_TestCount > 3 && m_bfinished)
		{
			m_bfinished = false;
			m_TestCount = 0;
			KillTimer(ID_TMR_PROGRESS_AUTO);
			KillTimer(ID_TMR_PROG_AI);

			if (m_chkInpMode.GetCheck() == BST_CHECKED)
			{
				m_chkInpMode.SetCheck(0);
				BntChkInpMode();
			}

			else
			{
				m_chkInpMode.SetCheck(1);
				BntChkInpMode();
			}

			GetDlgItem(IDC_BTN_AUTORUN)->EnableWindow(true);
			//ChkNextProduct();
		}
		break;

	case PRO_N3AO8:
		Auto_start_AO8[nCount + 8 * m_spnInpByte.GetPos32()] = true;
		m_TestCount++;	//현재 Ch 추적

		if (m_TestCount > 7)
		{
			m_TestCount = 0;
			KillTimer(ID_TMR_PROGRESS_AUTO);
			KillTimer(ID_TMR_PROG_AO);
			ChkNextProduct();
		}
		break;

	case  PRO_SIOAI8AO4:
		
		if (!m_bitoo)
			Auto_start_AI8[nCount + 8 * m_spnInpByte.GetPos32()] = true;
		else
			Auto_start_AO8[nCount + 8 * m_spnInpByte.GetPos32()] = true;

		m_TestCount++;

		if (m_TestCount > 7 && !m_boverthe8)
		{
			m_TestCount = 0;
			m_boverthe8 = true;
			m_bfinished = true;
			KillTimer(ID_TMR_PROGRESS_AUTO);
			KillTimer(ID_TMR_PROG_AI);

			if (m_chkInpMode.GetCheck() == BST_CHECKED)
			{
				m_chkInpMode.SetCheck(0);
				BntChkInpMode();
			}
			 
			else
			{
				m_chkInpMode.SetCheck(1);
				BntChkInpMode();
			}

			AfxMessageBox(_T("전류모드 스위치 변경 후 클릭-sio"));

			for (size_t i = 0; i < MAX_AI; i++)
				Auto_start_AI8[i] = false;

			Sleep(100);
			SetTimer(ID_TMR_PROGRESS_AUTO, 100, NULL);
		}

		else if (m_TestCount > 7 && m_bfinished)
		{
			m_bfinished = false;
			m_TestCount = 0;
			KillTimer(ID_TMR_PROGRESS_AUTO);
			KillTimer(ID_TMR_PROG_AI);

			if (m_chkInpMode.GetCheck() == BST_CHECKED)
			{
				m_chkInpMode.SetCheck(0);
				BntChkInpMode();
			}

			else
			{
				m_chkInpMode.SetCheck(1);
				BntChkInpMode();
			}

			AfxMessageBox(_T("AO검사를 진행합니다. 준비가 완료되면 확인을 눌러주세요"));	//TODO: AO검사까지 한 번에 준비가 된다면 이 부분 주석처리하여 바로 진행가능하도록
			m_bitoo = true;

			SetTimer(ID_TMR_PROGRESS_AUTO, 100, NULL);
		}

		else if (m_TestCount > 3 && m_bitoo)
		{
			m_bitoo = false;
			m_TestCount = 0;
			KillTimer(ID_TMR_PROGRESS_AUTO);
			KillTimer(ID_TMR_PROG_AO);
			m_bsub1end = true;
			ChkNextProduct();
		}
		
		/*
		if (m_iTestStage == TEST_AI_VOL)
		{
			Auto_start_AI8[nCount + 8 * m_spnInpByte.GetPos32()] = true;
			m_TestCount++;	//현재 Ch 추적

			if (m_TestCount > 7)
			{
				m_TestCount = 0;
				KillTimer(ID_TMR_PROGRESS_AUTO);
				KillTimer(ID_TMR_PROG_AI);
				ChkNextProduct();
			}
		}

		else if (m_iTestStage == TEST_AI_CUR)
		{
			Auto_start_AI8[nCount + 8 * m_spnInpByte.GetPos32()] = true;
			m_TestCount++;	//현재 Ch 추적

			if (m_TestCount > 7)
			{
				m_TestCount = 0;
				KillTimer(ID_TMR_PROGRESS_AUTO);
				KillTimer(ID_TMR_PROG_AI);
				ChkNextProduct();
			}
		}

		else if (m_iTestStage == TEST_AO)
		{
			Auto_start_AO8[nCount + 8 * m_spnInpByte.GetPos32()] = true;
			m_TestCount++;	//현재 Ch 추적

			if (m_TestCount > 4)
			{
				m_TestCount = 0;
				KillTimer(ID_TMR_PROGRESS_AUTO);
				KillTimer(ID_TMR_PROG_AO);
				ChkNextProduct();
			}
		}
		*/

		break;

	case PRO_SIOAI8:
		Auto_start_AI8[nCount + 8 * m_spnInpByte.GetPos32()] = true;
		m_TestCount++;	//현재 Ch 추적

		if ((m_TestCount > 7) && m_bTotalTestFinished)
		{
			m_TestCount = 0;
			KillTimer(ID_TMR_PROGRESS_AUTO);
			KillTimer(ID_TMR_PROG_AI);
		}

		else if (m_TestCount > 7)
		{
			m_TestCount = 0;
			KillTimer(ID_TMR_PROGRESS_AUTO);
			KillTimer(ID_TMR_PROG_AI);
			ChkNextProduct();
		}
		break;

	case PRO_SIOAO4:
		Auto_start_AO8[nCount + 8 * m_spnInpByte.GetPos32()] = true;
		m_TestCount++;	//현재 Ch 추적

		if (m_TestCount > 4)
		{
			m_TestCount = 0;
			KillTimer(ID_TMR_PROGRESS_AUTO);
			KillTimer(ID_TMR_PROG_AO);
			ChkNextProduct();
		}
		break;

	case PRO_SIOAI4RB:
		Auto_start_AI8[nCount + 8 * m_spnInpByte.GetPos32()] = true;
		m_TestCount++;	//현재 Ch 추적

		if (m_TestCount > 3)
		{
			m_TestCount = 0;
			KillTimer(ID_TMR_PROGRESS_AUTO);
			KillTimer(ID_TMR_PROG_AI);
			ChkNextProduct();
		}
		break;

	case PRO_SIOAO4RB:
		Auto_start_AO8[nCount + 8 * m_spnInpByte.GetPos32()] = true;
		m_TestCount++;	//현재 Ch 추적

		if (m_TestCount > 4)
		{
			m_TestCount = 0;
			KillTimer(ID_TMR_PROGRESS_AUTO);
			KillTimer(ID_TMR_PROG_AO);
			ChkNextProduct();
		}
		break;

	default:
		break;

	}
	/*
	if (m_bis16)
	{
		Auto_start_AI16[nCount + 8 * m_spnInpByte.GetPos32()] = true;
	}

	if (m_bis8)
	{
		Auto_start_AI8[nCount + 8 * m_spnInpByte.GetPos32()] = true;
	}
	
	//if (!m_bfinished)
	m_TestCount++;	//현재 Ch 추적

	if (m_TestCount > 7 && m_bis8)
	{
		m_TestCount = 0;
		KillTimer(ID_TMR_PROGRESS_AUTO);
		GetDlgItem(IDC_BTN_AUTORUN)->EnableWindow(true);
	}

	if (m_TestCount > 7 && m_bis16 && (m_spnInpByte.GetPos32() == 0))
	{
		m_TestCount = 0;
		m_boverthe8 = true;
		//m_boverthe8 = true;
		//if (m_boverthe8 && (!m_boverthe16))
		//	m_TestCount = 0;
		//else
		//{
		//	KillTimer(ID_TMR_PROGRESS_AUTO);
		//	GetDlgItem(IDC_BTN_AUTORUN)->EnableWindow(true);
		//}

		for (int i = 0; i<8; i++)
		{
			int nChNo = 8 + i;
			strTemp.Format(_T("IN%d"), nChNo);
			GetDlgItem(IDC_STT_INCH0 + i)->SetWindowTextW(strTemp);
		}

		m_spnInpByte.SetPos32(m_spnInpByte.GetPos32() + 1);
		//testcomplete = true;

		if (m_boverthe8)
		{
			m_boverthe8 = false;
			m_boverthe16 = true;

			for (size_t i = 0; i < 16; i++)
			{
				Auto_start_AI16[i] = false;
				//m_bAIChStarted[i] = true;
			}
		}
		
	}

	if ((m_TestCount > 7) && m_boverthe16 && (m_spnInpByte.GetPos32() == 1))
	{
		m_TestCount = 0;
		m_bfinished = true;
		KillTimer(ID_TMR_PROGRESS_AUTO);
		KillTimer(ID_TMR_PROG_AI);
		GetDlgItem(IDC_BTN_AUTORUN)->EnableWindow(true);
	}*/
}


// Voltage or Current Mode 변경 함수
void CADST3_VS80Dlg::BntChkInpMode()
{
	int ret;
	CString str;

	if (m_chkInpMode.GetCheck())
	{
		m_chkInpMode.SetWindowTextW(_T("Current"));
		ret = TmcSend(m_YKGWDevId, ":SOUR:FUNC CURR");
		if (ret != 0)
		{
			str.Format(_T("Set Current Mode error, Code: %d"), TmcGetLastError(m_YKGWDevId));
			UpdateMsg(str, MT_ERR);
		}
		ret = TmcSend(m_YKGWDevId, ":SOUR:RANG 100E-3");
		if (ret != 0)
		{
			str.Format(_T("Set Current Range error, Code: %d"), TmcGetLastError(m_YKGWDevId));
			UpdateMsg(str, MT_ERR);
		}
	}
	else
	{
		m_chkInpMode.SetWindowTextW(_T("Voltage"));
		ret = TmcSend(m_YKGWDevId, ":SOUR:FUNC VOLT");
		if (ret != 0)
		{
			str.Format(_T("Set Voltage Mode error, Code: %d"), TmcGetLastError(m_YKGWDevId));
			UpdateMsg(str, MT_ERR);
		}
		ret = TmcSend(m_YKGWDevId, ":SOUR:RANG 10E+0");
		if (ret != 0)
		{
			str.Format(_T("Set Voltage Range error, Code: %d"), TmcGetLastError(m_YKGWDevId));
			UpdateMsg(str, MT_ERR);
		}
	}
}


// 이전 버튼 눌렀을 때 함수 호출
void CADST3_VS80Dlg::BnClickedBtnPrev()
{
	CString str;

	if (m_lModuleCnt == 0)
		return;
	if (--m_lModuleNo<0)
		m_lModuleNo = m_lModuleCnt - 1;

	//long lBoardNo, lModulePos;
	//DWORD dwModuleID;
	//DWORD ret = AxaInfoGetModule(m_lModuleNo, &lBoardNo, &lModulePos, &dwModuleID);

	//if (ret != AXT_RT_SUCCESS)
	//{
	//	str.Format(_T("Get Module Info Error, Code: %d"), ret);
	//	UpdateMsg(str, MT_ERR);
	//}
	//str.Format(_T(" [B/No:%02d,M/No:%02d] %s"), lBoardNo, m_lModuleNo, m_strModuleName[m_lModuleNo]);
	//m_edtProductSel.SetWindowText(str);

	AxaInfoGetModuleStatus(m_lModuleNo);
	AIOParamLoad();
	DeviceInit();
	CtrlsEnable(TRUE);
}


// 다음 버튼 눌렀을 때 함수 호출
void CADST3_VS80Dlg::BnClickedBtnNext()
{
	CString str;

	if (m_lModuleCnt == 0)
		return;
	if (++m_lModuleNo>m_lModuleCnt - 1)
		m_lModuleNo = 0;

	//long lBoardNo, lModulePos;
	//DWORD dwModuleID;
	//DWORD ret = AxaInfoGetModule(m_lModuleNo, &lBoardNo, &lModulePos, &dwModuleID);
	//if (ret != AXT_RT_SUCCESS)
	//{
	//	str.Format(_T("Get Module Info Error, Code: %d"), ret);
	//	UpdateMsg(str, MT_ERR);
	//}
	//str.Format(_T(" [B/No:%02d,M/No:%02d] %s"), lBoardNo, m_lModuleNo, m_strModuleName[m_lModuleNo]);
	//m_edtProductSel.SetWindowText(str);

	AIOParamLoad();
	AxaInfoGetModuleStatus(m_lModuleNo);
	DeviceInit();
	CtrlsEnable(TRUE);
}


// 검사 완료 후 다음검사 제품이 있는지 확인
// TODO: 정상작동하는지 Test 해볼 것
void CADST3_VS80Dlg::ChkNextProduct()
{
	if ((m_ProductCnt <= m_lModuleCnt))
	{
		BnClickedBtnNext();
		Sleep(300);
		AxaInfoGetModule(m_ProductCnt, &m_lBoardNo, &m_lModulePos, &m_dwModuleIDs);

		if (m_lModulePos > 1)
		{
			sendCommand('t');
		}

		else
		{
			sendCommand('T');
		}

		if (m_csModuleName.Find(_T("AI16")) != -1)
			m_Autorun_product_num = PRO_N3AI16;
		else if (m_csModuleName.Find(_T("AO08")) != -1)
			m_Autorun_product_num = PRO_N3AO8;
		else if (m_csModuleName.Find(_T("AI8AO4")) != -1)
			m_Autorun_product_num = PRO_SIOAI8AO4;
		else if (m_csModuleName.Find(_T("AI8F")) != -1)
			m_Autorun_product_num = PRO_SIOAI8;
		else if (m_csModuleName.Find(_T("AO4F")) != -1)
			m_Autorun_product_num = PRO_SIOAO4;
		else if (m_csModuleName.Find(_T("AI4RB")) != -1)
			m_Autorun_product_num = PRO_SIOAI4RB;
		else if (m_csModuleName.Find(_T("AO4RB")) != -1)
			m_Autorun_product_num = PRO_SIOAO4RB;

		switch (m_Autorun_product_num)
		{
		case PRO_N3AI16:
			for (size_t i = 0; i < MAX_AI * 2; i++)
			{
				Auto_start_AI16[i] = false;
			}
			break;

		case PRO_N3AO8:
			for (size_t j = 0; j < MAX_AO; j++)
			{
				Auto_start_AO8[j] = false;
			}
			break;

		case PRO_SIOAI8AO4:
			//if (m_lModuleCnt >= 1)
			//	m_bproductCnt[PRO_SIOAI8AO4] = true;

			for (size_t k = 0; k < MAX_AI; k++)
				Auto_start_AI8[k] = false;
			for (size_t i = 0; i < MAX_AO / 2; i++)
				Auto_start_AO4[i] = false;
			m_iTestStage++;
			break;

		case PRO_SIOAI8:
			m_bis8 = true;
			for (size_t l = 0; l < MAX_AI; l++)
			{
				Auto_start_AI8[l] = false;
				break;
			}

		case PRO_SIOAO4:	//TODO : AO검사로 변경할 것
			m_bis8 = true;
			for (size_t m = 0; m < MAX_AI; m++)
			{
				Auto_start_AI8[m] = false;
			}
			break;

		case PRO_SIOAI4RB:
			m_bis8 = true;
			for (size_t n = 0; n < MAX_AI; n++)
			{
				Auto_start_AI8[n] = false;
			}
			break;

		case PRO_SIOAO4RB:
			m_bis8 = true;
			for (size_t o = 0; o < MAX_AI; o++)
			{
				Auto_start_AI8[o] = false;
			}
			break;

		default:
			break;
		}

		m_TestCount = 0;
		m_ProductCnt++;
		GetDlgItem(IDC_BTN_AUTORUN)->EnableWindow(false);
		SetTimer(ID_TMR_PROGRESS_AUTO, 100, NULL);
	}

	else
	{
		m_bTotalTestFinished = true;
		//m_ProductCnt = 0;
		m_TestCount = 0;
		GetDlgItem(IDC_BTN_AUTORUN)->EnableWindow(true);
		KillTimer(ID_TMR_PROGRESS_AUTO);
		return;
	}

	//if (!(m_iAgingCount >= GetDlgItemInt(IDC_EDIT_AGING)))
	//{
	//	m_iAgingCount++;
	//	Sleep(100);
	//	SetTimer(ID_TMR_PROGRESS_AUTO, 100, NULL);
	//}

}


void CADST3_VS80Dlg::OnBnClickedButtonSend()
{
	//sendCommand('t');
	CString str;
	CString path;

	CFile file;
	CFileException ex;
	CStdioFile csvfile;

	int a, b, c;
	double d, e;
	CString strTemp  = _T("Test string");

	if (!csvfile.Open(_T("test.txt"), CFile::modeWrite | CFile::modeCreate, &ex))
	{
		ex.ReportError();
		return;
	}
	csvfile.WriteString(strTemp);

	a = 100;
	b = 200;
	c = 300;
	d = 400.400;
	e = 500.500;

	strTemp.Format(_T("%d %d %d %f %f"), a, b, c, d, e);
	csvfile.WriteString(strTemp);

}


void CADST3_VS80Dlg::OnBnClickedButtonSend2()
{
	sendCommand('T');
}


void CADST3_VS80Dlg::OnBnClickedButtonSend4()
{
	// Write 버튼
	//sendCommand('y');
	CString str;
	CString path;
	CString szFilter = _T("Logfile (*.scv) | *.scv;*.SCV | All Files(*.*)|*.*||");

	CFileDialog dlg(false, _T("scv"), NULL, OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();
	}

	else
	{
		return;
	}

	CFile file;
	CArchive ar(&file, CArchive::store);
	file.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, NULL);

	int a, b, c;
	double ddouble[8];

	for (size_t i = 0; i < 8; i++)
	{
		ddouble[i] += i;
	}

	str.Format(_T("MFC File Read Write Test!!"));

	a = 1;
	b = 2;
	c = 3;

	ar << a << b << str << c;
	for (size_t i = 0; i < 8; i++)
	{
		ar << ddouble[i];
	}
	ar.Flush();

	ar.Close();
	file.Close();
}


void CADST3_VS80Dlg::OnBnClickedButtonSend3()
{
	// Read 버튼
	//SendCommand('x');

	CString path;
	CString strData;
	CString szFilter = _T("Logfile (*.scv) | *.scv;*.SCV | All Files(*.*)|*.*||");

	CFileDialog dlg(true, _T("scv"), NULL, OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();
	}
	else
	{
		return;
	}

	CFile file;
	CArchive ar(&file, CArchive::load);
	file.Open(path, CFile::modeRead);

	int a, b, c;
	CString str;
	double ddouble[8];

	for (size_t i = 0; i < 8; i++)
	{
		ddouble[i] += i;
	}

	ar >> a >> b >> str >> c;

	for (size_t i = 0; i < 8; i++)
	{
	ar >> ddouble[i];
	}

	ar.Close();
	file.Close();

	strData.Format(_T("%d %d %s %d %f"), a, b, str, c, ddouble[4]);
	AfxMessageBox(strData);
}


void CADST3_VS80Dlg::OnBnClickedButtonConnect()
{
	CADST3_VS80Dlg* pDlg;//2018-06-14
	pDlg = (CADST3_VS80Dlg*)AfxGetMainWnd();

	CString strData;

	GetDlgItemText(IDC_EDIT_COM, strData);

	if (!pDlg->connect(strData))
		UpdateMsg(strData + _T("(Arduino) Connect Failed!"), MT_ERR);

	else
		UpdateMsg(strData + _T("COM3(Arduino) Connect Successed!"), MT_RSLT);
}
