// AutorunDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ADST3_VS80.h"
#include "AutorunDialog.h"
#include "afxdialogex.h"
#include "ADST3_VS80Dlg.h"
#include "include\AXL.h"
#include "include\AXD.h"
#include "include\AXHS.h"


// AutorunDialog ��ȭ �����Դϴ�.

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


// AutorunDialog �޽��� ó�����Դϴ�.


void AutorunDialog::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnDestroy();
}


BOOL AutorunDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
