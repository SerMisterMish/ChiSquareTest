
// ChiSqView.h : interface of the CChiSqView class
//

#pragma once


class CChiSqView : public CView
{
protected: // create from serialization only
	CChiSqView() noexcept;
	DECLARE_DYNCREATE(CChiSqView)

// Attributes
public:
	CChiSqDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CChiSqView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	void DrawHist();
	void DrawPVDF();
	void DrawPowerLevelOnSampleSize();
};

#ifndef _DEBUG  // debug version in ChiSqView.cpp
inline CChiSqDoc* CChiSqView::GetDocument() const
   { return reinterpret_cast<CChiSqDoc*>(m_pDocument); }
#endif

