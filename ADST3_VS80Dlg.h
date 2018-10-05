// ADST3_VS80Dlg.h : header file
//

#pragma once
#include "Resource.h"
#include "Socket.h"
#include "ColorControl.h"
#include "StaticTransBkg.h"
#include "BtnST.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "PYH_Comm.h"		//시리얼통신 코드 2018-06-14
#include "AutorunDialog.h"  //Autorun 제품 선택 다이얼로그 2018-07-27
#include "XLEzAutomation.h"

//#include "afxbutton.h"

#define BK_COLOR		RGB(0, 0, 0)//(255, 128, 128)
#define MASK_COLOR		RGB(255, 0, 255)//(255, 128, 128)
#define EXIT_ENABLE		FALSE

#define MAX_AI			8
#define MAX_AO			8

#define		CR_WHITE		RGB(0xFF, 0xFF, 0xFF)
#define		CR_BLACK		RGB(0x00, 0x00, 0x00)
#define		CR_RED			RGB(0xFF, 0x00, 0x00)
#define		CR_LIME			RGB(0x00, 0xFF, 0x00)
#define		CR_GREEN		RGB(0x00, 0xA5, 0x00)
#define		CR_DGREEN		RGB(0x00, 0x80, 0x00)
#define		CR_DBLUE		RGB(0x00, 0x00, 0xA0)
#define		CR_BLUE			RGB(0x00, 0x00, 0xFF)
#define		CR_NAVY			RGB(0x00, 0x00, 0x80)
#define		CR_ORANGE		RGB(0xFF, 0x80, 0x00)
#define		CR_DORANGE		RGB(0xB2, 0x58, 0x00)
#define		CR_GRAY			RGB(0x80, 0x80, 0x80)
#define		CR_SILVER		RGB(0xC0, 0xC0, 0xC0)
#define		CR_YELLOW		RGB(0xFF, 0xFF, 0x00)
#define		CR_CYAN			RGB(0x00, 0xFF, 0xFF)

#define		CL_READY		CR_BLUE
#define		CL_RUNNING		CR_ORANGE
#define		CL_RUNNING2		CR_DORANGE
#define		CL_STOP			CR_BLUE
#define		CL_SUCCESS		CR_GREEN
#define		CL_ERROR		CR_RED
#define		CL_DISABLE		CR_GRAY

#define RETURN_SUCCESS 1
#define RETURN_FAIL 0

#define ANALOG_INPUT   'i'
#define ANALOG_OUTPUT  'o'
#define RELAY_ON       'n'
#define RELAY_OFF      'f'

enum FLUKE_IP_ADDR
{
	IP_ADDR0	= 192,
	IP_ADDR1	= 168,
	IP_ADDR2	= 10,
	IP_ADDR3	= 93,
	PORT_NO		= 3490
};

enum MSG_TYPE
{
	MT_RSLT  = 0,
	MT_ERR,
	MT_MSG,
	MT_WRN
};

enum SOCKET_MSG_TYPE
{
	SMT_PRODUCT_ID = 0,
	SMT_DATA,
	SMT_TOTAL
};

enum ID_TIMER
{
	ID_TMR_0 = 0,
	ID_TMR_PROG_AI,
	ID_TMR_PROG_AO,
	ID_TMR_SOCKET_CONNECTION,
	ID_TMR_UPDATE_CH_STATUS,
	ID_TMR_PROGRESS_AUTO,	//2018-07-20 현재 검사ch 추적 및 다음ch 자동 검사 시작 Timer
	ID_TMR_CHECK_END,		//검사 끝을 감시하는 타이머
	ID_TMR_TOTAL
};

enum AIOCH_STATUS
{
	CHS_DISABLE = 0,
	CHS_READY,
	CHS_RUNNING,
	CHS_FAIL,
	CHS_SUCCESS
};

enum PRODUCT_INFO
{
	PRO_N3AI16 = 0,
	PRO_N3AO8,
	PRO_SIOAI8AO4,
	PRO_SIOAI8,
	PRO_SIOAO4,
	PRO_SIOAI4RB,
	PRO_SIOAO4RB
};
enum SIO_AI8AO4F
{
	TEST_AI_VOL = 0,
	TEST_AI_CUR,
	TEST_AO
};


#define MAX_PATTERN		25
typedef struct AnalogInOutParam
{
	UINT8  nAIChNum;
	UINT8  nAOChNum;
	UINT   nAITestCnt;
	UINT   nAOTestCnt;

	double dVInRange_Low;
	double dVInRange_High;
	double dVInThreshold_Low;
	double dVInThreshold_High;
	double dVInErrorRange;
	double dVInOffset;

	double dCInRange_Low;
	double dCInRange_High;
	double dCInThreshold_Low;
	double dCInThreshold_High;
	double dCInErrorRange;
	double dCInOffset;

	double dVOutRange_Low;
	double dVOutRange_High;
	double dVOutThreshold_Low;
	double dVOutThreshold_High;
	double dVOutErrorRange;
	double dVOutOffset;

	double dAIVPattern[MAX_PATTERN];
	double dAICPattern[MAX_PATTERN];
	double dAOPattern[MAX_PATTERN];

	UINT8  nAIVPtrnNum;
	UINT8  nAICPtrnNum;
	UINT8  nAOPtrnNum;

	UINT8  nAIPtrnIndex;
	UINT8  nAOPtrnIndex;

	UINT  nAISampleCnt;
	UINT  nAOSampleCnt;
}ANALOG_IN_OUT_PARAM;

// PROCESS_EVENTS_IN_GUI_THREAD = TRUE  -> process the events in the GUI  thread
// PROCESS_EVENTS_IN_GUI_THREAD = FALSE -> process the events in an extra thread
#define PROCESS_EVENTS_IN_GUI_THREAD   FALSE

// SEND_LARGE_DATA = 0      -> Send the text string which the user has entered
// SEND_LARGE_DATA = 100000 -> Send a 100kB string "AAAAAAAA...", each time with another character
// Note: When sending large data and the Output Edit box becomes very full, the output becomes very slow.
// The application may even crash if you write 100 Megabyte into an Edit box.
// Then hit the "Clear" button to reset the Editbox!
#define SEND_LARGE_DATA   0

// The time in seconds after which a server disconnects his idle clients
// Set 0 to disable the timeout
#define MAX_SERVER_IDLE_TIME  5*60	// 5 minutes

// The time in seconds after which a client disconnects from his idle server
// Set 0 to disable the timeout
#define MAX_CLIENT_IDLE_TIME  0		// off

#define SOCKET_TIMEOUT		500		// msec

typedef struct SocketDev
{
	TCP::cSocket	cSocket;
	SOCKET			hSocket;
	int				nPort;
	DWORD			u32IP;
	CRITICAL_SECTION  Critical;
	HANDLE			hThread;
	CString			sData;
	char			cData[100];
}SOCKET_DEV;


// CADST3_VS80Dlg dialog
class CADST3_VS80Dlg : public CDialog
{
// Construction
public:
	CADST3_VS80Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ADST3_VS80_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	long		m_lModuleCnt;
	DWORD		m_dwModuleID[32];
	//CString		m_strModuleName[32];
	CString		m_csModuleName;
	INT			m_lModuleNo;
	ANALOG_IN_OUT_PARAM m_aioTarget;

	SOCKET_DEV	m_SocketDev;
	int			m_YKGWDevId;
	double		m_dNPLC;
	BOOL		m_bDlgClosed;
	BOOL		m_bEOD;			// End of Data

	BOOL		m_bFlukeInit, m_bYKGWInit;
	BOOL		m_bAIChStarted[MAX_AI], m_bAOChStarted[MAX_AO];

	LARGE_INTEGER	m_PerfFreq;
	LARGE_INTEGER	m_StartCnt[20];
	DWORD		m_nSocketTimeout;

	void	UpdateMsg(CString strMsg, int nType);
	DWORD	TargetInit(void);
	void	AIOParamLoad();
	void	CtrlsInit();
	void	CtrlsEnable(BOOL bEn = TRUE);
	int		DeviceInit();
	int		FlukeSetting();
	void	FlukeParamLoad();
	int		FlukeEthInit();
	int		FlukeCfg();
	void	YokogawaParamLoad();
	int		YokogawaInit();
	int		YokogawaCfg(int nDevId);
	void	ProcessDataAI(int nAIChNum);
	void	ProcessDataAO();
	void	SetAOState(UINT8 nIndex);
	void	SetAIState(UINT8 nIndex);
	void	OpenCfgFile();

	// Socket functions
	static ULONG WINAPI SocketProcessEventThread(void* p_Param);
	void	SocketProcessEvents();
	void	SocketProcessReceivedDataNormal(TCP::cSocket::cMemory* pi_RecvMem);
	BOOL	SendTo(SOCKET h_Socket, CString s_SendData);
	void	CloseSockets();
	CString CopyToString(char* s8_Buf, DWORD u32_Bytes);
	void	PumpMessages();
	CString GetSocketErrMsg(DWORD u32_Error);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNMCustomdrawListLog(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	CStatic		m_sttProductName;
	CEdit		m_edtProductSel;
	CButtonST	m_btnPrev;
	CButtonST	m_btnNext;
	CButtonST	m_btnYKGWSetting;
	CButtonST	m_btnFlukeSetting;
	CButton		m_chkInpMode;
	CButtonST	m_btnExit;
	CListCtrl	m_lcLog;
	CColorControl	m_sttYKGWSetting;
	CColorControl	m_sttFlukeSetting;
	CColorControl	m_sttAI[MAX_AI], m_sttAO[MAX_AO];
	CColorControl	m_AICh[MAX_AI], m_AOCh[MAX_AO];

	CButtonST	m_btnAI[MAX_AI];
	CButtonST	m_btnAO[MAX_AO];

	CButtonST	m_btnYKGWFileOpen;
	CButtonST	m_btnFlukeFileOpen;

	CButtonST	m_btnProdFileOpen;
	CButtonST	m_btnClrLog;
	CColorControl m_sttAIG;
	CColorControl m_sttAOG;

	//2018-07-20 자동검사 및 stop 버튼 추가
	CButtonST m_autorun;
	CButtonST m_autorunstop;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedChkInpMode();
	afx_msg void OnBnClickedBtnPrev();
	afx_msg void OnBnClickedBtnNext();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnClose();
	//afx_msg void OnBnClickedBtnOut0();
	//afx_msg void OnBnClickedBtnOut1();
	//afx_msg void OnBnClickedBtnOut2();
	//afx_msg void OnBnClickedBtnOut3();
	//afx_msg void OnBnClickedBtnOut4();
	//afx_msg void OnBnClickedBtnOut5();
	//afx_msg void OnBnClickedBtnOut6();
	//afx_msg void OnBnClickedBtnOut7();
	//afx_msg void OnBnClickedBtnIn0();
	//afx_msg void OnBnClickedBtnIn1();
	//afx_msg void OnBnClickedBtnIn2();
	//afx_msg void OnBnClickedBtnIn3();
	//afx_msg void OnBnClickedBtnIn4();
	//afx_msg void OnBnClickedBtnIn5();
	//afx_msg void OnBnClickedBtnIn6();
	//afx_msg void OnBnClickedBtnIn7();
	afx_msg void OnBnClickedBtnIn(UINT nID);
	afx_msg void OnBnClickedBtnOut(UINT nID);

	afx_msg void OnBnClickedBtnYkgwSetting();
	afx_msg void OnBnClickedBtnFlukeSetting();
	afx_msg void OnBnClickedBtnYkgwFileOpen();
	afx_msg void OnBnClickedBtnFlukeFileOpen();
	afx_msg void OnBnClickedBtnProdFileOpen();
	afx_msg void OnBnClickedBtnClrlog();

	//2018-07-20추가
	afx_msg void OnBnClickedBtnAutorun();
	afx_msg void OnBnClickedButton3();
public:
	CSpinButtonCtrl m_spnInpByte;
public:
	CSpinButtonCtrl m_spnOutpByte;
public:
	afx_msg void OnDeltaposSpinInpbyte(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnDeltaposSpinOutpbyte(NMHDR *pNMHDR, LRESULT *pResult);
public:
	//2018-07-20 아두이노 시리얼 통신에 필요한 함수 및 변수 모음
	//아두이노 초기화 관련 코드 - 2018-06-14
	CPYH_Comm serial;
	// 시리얼통신 연결함수 2018-06-14
	int connect(CString portNum);
	// 시리얼통신을 통해 문자열전송 2018-06-14
	int sendCommand(char pos);
	// 시리얼통신 연결해제 2018-06-14
	void disconnect();
	// COM연결이 이루어졌는지 체크 2018-06-14
	bool m_bConnect;
	char Serialdata[5];
	int bufferindex = 0;

public :
	//2018-07-20 자동테스트 및 디버깅에 필요한 변수 모음

	int m_TestCount = 0;
	int m_TotalTestCount = 0;
	int m_Autorun_product_num;	//보조 다이얼로그 창에서 제품 선택 후 해당제품 번호 저장
	int m_iAgingCount = 0;
	int m_ProductCnt = 0;
	int m_iTestStage;

	double m_dStoreAIResult[MAX_AI];
	double m_dStoreAOResult[MAX_AO];

	bool Auto_start_AI16[16];
	bool Auto_start_AI8[8];
	bool Auto_start_AO8[8];
	bool Auto_start_AO4[4];
	bool testcomplete = false;
	bool m_boverthe16 = false;
	bool m_boverthe8 = false;
	bool m_bis16 = false;
	bool m_bis8 = false;
	bool m_bfinished = false;
	bool m_bitoo = false;
	bool m_bsub1end = false;
	bool m_bproductCnt[7];
	bool m_bTotalTestFinished = false;

	CString m_csModuleInfo;
	long m_lBoardNo;
	long m_lModulePos;
	DWORD m_dwModuleIDs;

public:
	//2018-07-20 자동테스트 및 디버깅에 필요한 함수 모음

	// TODO : AI자동 검사 시 현재 작동 중인 위치 추적
	void CheckforAI();
	//Serial Data 전송코드 재활용 함수
	void independent(int chIndex, CString strTemp, bool Rstate, bool VolnCur = 1);
	// CString을 SerialData로 송신하기 위한 Cahr* Coverting Function
	void CstringToChar(CString strData);
	// AI자동검사 시 결과를 판단하는 함수
	void CheckAIAutorunResult(int nCount);
	// Voltage or Current Mode 변경 함수
	void BntChkInpMode();
	// 이전 버튼 눌렀을 때 함수 호출
	void BnClickedBtnPrev();
	// 다음 버튼 눌렀을 때 함수 호출
	void BnClickedBtnNext();
	// 검사 완료 후 다음검사 제품이 있는지 확인
	void ChkNextProduct();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonSend2();
	afx_msg void OnBnClickedButtonSend4();
	afx_msg void OnBnClickedButtonSend3();
	afx_msg void OnBnClickedButtonConnect();
};
