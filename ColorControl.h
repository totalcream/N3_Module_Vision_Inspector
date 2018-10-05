#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorControl.h : header file
//

#ifndef	_COLORCONTROL_H
#define _COLORCONTROL_H

class CColorControl : public CStatic
{
// Construction
public:
	CColorControl();
    enum {	
		ST_FLS_SLOW,
		ST_FLS_NORMAL,
		ST_FLS_FAST
	};

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorControl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void EnableNotify(CWnd* pParent = NULL, UINT nMsg = WM_USER);

	void StartBkBlink(BOOL bStart = TRUE, UINT nElapse = ST_FLS_NORMAL);
	void StartTextBlink(BOOL bStart = TRUE, UINT nElapse = ST_FLS_NORMAL);

	void SetBlinkTextColors(COLORREF crBlinkTextColor1, COLORREF crBlinkTextColor2);
	void SetBlinkBkColors(COLORREF crBlinkBkColor1, COLORREF crBlinkBkColor2);

	void SetBkColor(COLORREF crBkColor = 0xffffffff);
	COLORREF GetBkColor();

	void SetTextColor(COLORREF crTextColor = 0xffffffff);
	COLORREF GetTextColor();

	virtual ~CColorControl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorControl)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	UINT m_nTimerId;

	COLORREF m_crTextColor;
	COLORREF m_crBlinkTextColors[2];
	BOOL m_bTextBlink;
	int m_nTextBlinkStep;

	COLORREF m_crBkColor;
	COLORREF m_crBlinkBkColors[2];
	BOOL m_bBkBlink;
	int m_nBkBlinkStep;

	CBrush m_brBkgnd;
	CBrush m_brBlinkBkgnd[2];

	CWnd* m_pParent;
	UINT m_nMsg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORCONTROL_H__11B96604_EC68_49A3_8EDF_027C4F01F03F__INCLUDED_)
