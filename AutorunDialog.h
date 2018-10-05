#pragma once


// AutorunDialog 대화 상자입니다.

class AutorunDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AutorunDialog)

public:
	AutorunDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~AutorunDialog(); 

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_AUTORUN_CHOICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	// 라디오버튼 시작 변수 2018-07-27
	int m_N3AI16;
	afx_msg void OnBnClickedRadio(UINT msg);
	afx_msg void OnBnClickedClose();
	virtual BOOL OnInitDialog();
};
