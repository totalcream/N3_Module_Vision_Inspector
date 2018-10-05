// ListCtrlFF.cpp : implementation file
//

#include "stdafx.h"
#include "ADST3_VS80Dlg.h"
#include "ListCtrlFF.h"
#include "CMemDC.h"


// CListCtrlFF

IMPLEMENT_DYNAMIC(CListCtrlFF, CListCtrl)

CListCtrlFF::CListCtrlFF()
{

}

CListCtrlFF::~CListCtrlFF()
{
}


BEGIN_MESSAGE_MAP(CListCtrlFF, CListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CListCtrlFF message handlers



BOOL CListCtrlFF::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CListCtrl::OnEraseBkgnd(pDC);
	return FALSE;
}

void CListCtrlFF::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CPaintDC dctmp(this);

	CRect headerRect;
	GetDlgItem(0)->GetWindowRect(&headerRect);
	ScreenToClient(&headerRect);
	dc.ExcludeClipRect(&headerRect);

	CMyMemDC memDC(&dc);

	CRect clip;
	memDC.GetClipBox(&clip);
	memDC.FillSolidRect(clip, GetSysColor(COLOR_WINDOW));

	DefWindowProc(WM_PAINT, (WPARAM)memDC->m_hDC, (LPARAM)0);

	// Do not call CListCtrl::OnPaint() for painting messages
}
