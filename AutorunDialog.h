#pragma once


// AutorunDialog ��ȭ �����Դϴ�.

class AutorunDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AutorunDialog)

public:
	AutorunDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~AutorunDialog(); 

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_AUTORUN_CHOICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	// ������ư ���� ���� 2018-07-27
	int m_N3AI16;
	afx_msg void OnBnClickedRadio(UINT msg);
	afx_msg void OnBnClickedClose();
	virtual BOOL OnInitDialog();
};
