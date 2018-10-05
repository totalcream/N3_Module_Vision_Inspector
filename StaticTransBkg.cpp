// StaticTransBkgrnd.cpp : implementation file
//

#include "stdafx.h"
//#include "ADMT1.h"
#include "StaticTransBkg.h"


// CStaticTransBkgrnd

IMPLEMENT_DYNAMIC(CStaticTransBkgrnd, CStatic)

CStaticTransBkgrnd::CStaticTransBkgrnd()
{

}

CStaticTransBkgrnd::~CStaticTransBkgrnd()
{
}


BEGIN_MESSAGE_MAP(CStaticTransBkgrnd, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CStaticTransBkgrnd message handlers

void CStaticTransBkgrnd::DrawItem(CDC *pDC, int nXSize, int nYSize, int nXIndex, int nYIndex, COLORREF crMask)
{
	CRect rect;
	GetClientRect(&rect);

	//
	// Center led within an oversized window
	//
	if(rect.Width() >= nXSize && rect.Height() >= nYSize)
	{
		int nWidth = rect.Width();
		int nHeight = rect.Height();
		rect.left += (nWidth - nXSize)/2;
		rect.right -= (nWidth - nXSize)/2;
		rect.top += (nHeight - nYSize)/2;
		rect.bottom -= (nHeight - nYSize)/2;
	}

	//
	// Prepare temporary DCs and bitmaps
	//
	CBitmap TransBitmap;
	TransBitmap.CreateBitmap(rect.Width(),rect.Height(),1,1,NULL);
	CBitmap bitmapTemp;
	CBitmap* pBitmap = &m_ItemBitmap;
	CDC srcDC;
	CDC dcMask;
	CDC TempDC;
	TempDC.CreateCompatibleDC(pDC);
	srcDC.CreateCompatibleDC(pDC);
	dcMask.CreateCompatibleDC(pDC);

	CBitmap* pOldBitmap = srcDC.SelectObject(pBitmap);
	CBitmap* pOldMaskbitmap = dcMask.SelectObject(&TransBitmap);
	bitmapTemp.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());

	//
	// Work with tempDC and bitmapTemp to reduce flickering
	//
	CBitmap *pOldBitmapTemp = TempDC.SelectObject(&bitmapTemp);
	TempDC.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, rect.left, rect.top, SRCCOPY); 

	//
	// Create mask
	//
	//COLORREF OldBkColor = srcDC.SetBkColor(RGB(255,0,255));
	COLORREF OldBkColor = srcDC.SetBkColor(crMask);
	dcMask.BitBlt(0, 0, rect.Width(), rect.Height(),&srcDC, nXIndex*nXSize, nYIndex*nYSize, SRCCOPY); 
	TempDC.SetBkColor(OldBkColor);

	//
	// Using the IDB_LEDS bitmap, index into the bitmap for the appropriate
	// LED. By using the mask color (RGB(255,0,255)) a mask has been created
	// so the bitmap will appear transparent.
	//
	TempDC.BitBlt(0, 0, rect.Width(), rect.Height(), &srcDC, nXIndex*nXSize, nYIndex*nYSize, SRCINVERT); 
	TempDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dcMask, 0, 0, SRCAND); 
	TempDC.BitBlt(0, 0, rect.Width(), rect.Height(), &srcDC, nXIndex*nXSize, nYIndex*nYSize, SRCINVERT); 

	//
	// Since the actual minipulation is done to tempDC so there is minimal
	// flicker, it is now time to draw the result to the screen.
	//
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &TempDC, 0, 0, SRCCOPY); 

	//
	// House cleaning
	//
	srcDC.SelectObject(pOldBitmap);
	dcMask.SelectObject(pOldMaskbitmap);
	TempDC.SelectObject(pOldBitmapTemp);
	VERIFY(srcDC.DeleteDC());
	VERIFY(dcMask.DeleteDC());
	VERIFY(TempDC.DeleteDC());
	VERIFY(TransBitmap.DeleteObject());
	VERIFY(bitmapTemp.DeleteObject());
}



void CStaticTransBkgrnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawItem(&dc,m_nXSize,m_nYSize,m_nXIndex, m_nYIndex, m_crMask);

	// Do not call CStatic::OnPaint() for painting messages
}

BOOL CStaticTransBkgrnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	//return CStatic::OnEraseBkgnd(pDC);
}

void CStaticTransBkgrnd::SetBmItem(int nXSize, int nYSize, int nXIndex, int nYIndex, COLORREF crMask)
{
	m_nXSize = nXSize;
	m_nYSize = nYSize;
	m_nXIndex = nXIndex;
	m_nYIndex = nYIndex;
	m_crMask  = crMask;

	CDC *pDC;
	pDC = GetDC();
	DrawItem(pDC, m_nXSize, m_nYSize, m_nXIndex, m_nYIndex, m_crMask);
	ReleaseDC(pDC);
}
