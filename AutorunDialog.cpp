// AutorunDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ADST3_VS80.h"
#include "AutorunDialog.h"
#include "afxdialogex.h"
#include "ADST3_VS80Dlg.h"
#include "include\AXL.h"
#include "include\AXD.h"
#include "include\AXHS.h"


// AutorunDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(AutorunDialog, CDialogEx)

AutorunDialog::AutorunDialog(CWnd* pParent /*=NULL*/)
: CDialogEx(AutorunDialog::IDD, pParent)
, m_N3AI16(0)
{

}

AutorunDialog::~AutorunDialog()
{
}

void AutorunDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Radio(pDX, IDC_RADIO_N3_AI16, (int&)m_radio);
	DDX_Radio(pDX, IDC_RADIO_N3_AI16, m_N3AI16);
}


BEGIN_MESSAGE_MAP(AutorunDialog, CDialogEx)
	//ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_N3_AI16, IDC_RADIO_N3_AI22, RadioCtrl)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_N3_AI16, IDC_RADIO_SIO_AO4RB, OnBnClickedRadio)
	ON_BN_CLICKED(IDCLOSE, &AutorunDialog::OnBnClickedClose)
END_MESSAGE_MAP()


// AutorunDialog 메시지 처리기입니다.


void AutorunDialog::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//((CADST3_VS80Dlg *)GetParent())->m_Autorun_product_num = m_radio;
	//UpdateData();


	//switch (((CADST3_VS80Dlg *)GetParent())->m_Autorun_product_num)
	//{
	//case 0:
	//	AfxMessageBox(_T("AI16"));
	//	break;
	//case 1:
	//	AfxMessageBox(_T("AO8"));
	//	break;
	//case 2:
	//	AfxMessageBox(_T("AI8AO4"));
	//	break;
	//case 3:
	//	AfxMessageBox(_T("AI8"));
	//	break;
	//case 4:
	//	AfxMessageBox(_T("AO4"));
	//	break;
	//case 5:
	//	AfxMessageBox(_T("AI4RB"));
	//	break;
	//case 6:
	//	AfxMessageBox(_T("AO4RB"));
	//	break;
	//default:
	//	break;
	//}
	//CADST3_VS80Dlg::



	CDialogEx::OnOK();
}

void AutorunDialog::OnBnClickedRadio(UINT msg)
{
	//UpdateData(TRUE);
	//((CADST3_VS80Dlg *)GetParent())->m_Autorun_product_num = m_N3AI16;
	//OnOK();
}

void AutorunDialog::OnBnClickedClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnDestroy();
}


BOOL AutorunDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	AxlOpenNoReset(7);

	DWORD dwModuleID;
	CString strData;

	long lModuleCnt, lBoardNo, ModulePos;

	AxdInfoGetModuleCount(&lModuleCnt);

	for (size_t i = 0; i < 4; i++)
	{
		if (AxdInfoGetModule(i, &lBoardNo, &ModulePos, &dwModuleID) == AXT_RT_SUCCESS)
		{
			strData.Format(_T("No.%d : %s"), i, dwModuleID);
			GetDlgItem(IDC_STATIC_0 + i)->SetWindowTextW(strData);
			GetDlgItem(IDC_EDIT_SERIAL_0 + i)->ShowWindow(true);
		}

		else
		{
			strData.Format(_T("No.%d : X"), i);
			GetDlgItem(IDC_STATIC_0 + i)->SetWindowTextW(strData);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
