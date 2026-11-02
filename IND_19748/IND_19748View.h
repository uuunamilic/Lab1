
// IND_19748View.h : interface of the CIND19748View class
//

#pragma once


class CIND19748View : public CView
{
protected: // create from serialization only
	CIND19748View() noexcept;
	DECLARE_DYNCREATE(CIND19748View)

// Attributes
public:
	CIND19748Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIND19748View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	bool grid;   // da li se mreza iscrtava 

	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawPolygon(CDC* pDC, CPoint* points, int n);
	void DrawGrid(CDC* pDC, int cell);
	void DrawTangram(CDC* pDC);
	CPoint P(double gx, double gy) const;   // jedinice mreze tj. pikseli

	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in IND_19748View.cpp
inline CIND19748Doc* CIND19748View::GetDocument() const
   { return reinterpret_cast<CIND19748Doc*>(m_pDocument); }
#endif

