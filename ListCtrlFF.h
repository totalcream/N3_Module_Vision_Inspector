#pragma once


// CListCtrlFF

class CListCtrlFF : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlFF)

public:
	CListCtrlFF();
	virtual ~CListCtrlFF();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


