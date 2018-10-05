#pragma once


// CStaticTransBkgrnd

class CStaticTransBkgrnd : public CStatic
{
	DECLARE_DYNAMIC(CStaticTransBkgrnd)

protected:

public:
	CStaticTransBkgrnd();
	virtual ~CStaticTransBkgrnd();

	CBitmap m_ItemBitmap;
	int m_nXSize;
	int m_nYSize;
	int m_nXIndex;
	int m_nYIndex;
	COLORREF m_crMask;

	void DrawItem(CDC *pDC, int nXSize, int nYSize, int nXIndex, int nYIndex, COLORREF crMask);
	void SetBmItem(int nXSize, int nYSize, int nXIndex, int nYIndex, COLORREF crMask);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


