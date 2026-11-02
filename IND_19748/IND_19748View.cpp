
// IND_19748View.cpp : implementation of the CIND19748View class
//

#include "pch.h"
#include "framework.h"
#include<cmath>
static const int CELL = 25;                 // velicina polja => 500 x 500
static const double PI_D = 3.14159265358979323846;

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_19748.h"
#endif

#include "IND_19748Doc.h"
#include "IND_19748View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIND19748View

IMPLEMENT_DYNCREATE(CIND19748View, CView)

BEGIN_MESSAGE_MAP(CIND19748View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND19748View construction/destruction

CIND19748View::CIND19748View() noexcept
{
	// TODO: add construction code here
	grid = true;

}

CIND19748View::~CIND19748View()
{
}


BOOL CIND19748View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


// CIND19748View drawing

void CIND19748View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == 'G'){
        grid = !grid;
        Invalidate();
    }
   
}

//za pretvaranje koordinata u piksele
CPoint CIND19748View::P(double gx, double gy) const
{
    return CPoint((int)(gx * CELL + 0.5), (int)(gy * CELL + 0.5));
}

void CIND19748View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
    if (n < 3)
        return;
    CPoint* pts = new CPoint[n];
    for (int i = 0; i < n; i++)
    {
        double alfa = rotAngle + 2.0 * PI_D * i / n;
        pts[i].x = cx + (int)floor(r * cos(alfa) + 0.5);
        pts[i].y = cy + (int)floor(r * sin(alfa) + 0.5);
    }
    pDC->Polygon(pts, n);
    delete[] pts;
}

void CIND19748View::DrawPolygon(CDC* pDC, CPoint* points, int n)
{
    if (n < 3)
        return;
    int oldMode = pDC->SetPolyFillMode(ALTERNATE);
    pDC->Polygon(points, n);
    pDC->SetPolyFillMode(oldMode);
}

void CIND19748View::DrawGrid(CDC* pDC, int cell)
{
    CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
    CPen* oldPen = pDC->SelectObject(&pen);
    for (int i = 0; i <= 20; i++)
    {
        pDC->MoveTo(i * cell, 0);
        pDC->LineTo(i * cell, 20 * cell);
        pDC->MoveTo(0, i * cell);
        pDC->LineTo(20 * cell, i * cell);
    }
    pDC->SelectObject(oldPen);
}

void CIND19748View::DrawTangram(CDC* pDC)
{
    struct Part { CPoint pts[4]; int n; COLORREF clr; bool hatch; };//pomocna struktura za iscrtavanje

    Part parts[] = {
        { { P(1, 14.5), P(7, 8.5), P(7, 14.5) },                   3, RGB(255, 255, 0),   false },//zuti
        { { P(13, 8.5), P(19, 14.5), P(13, 14.5) },                3, RGB(50, 205, 50),   false },//zeleni
        { { P(10, 5.5), P(7, 8.5), P(13, 8.5) },                   3, RGB(255, 153, 51),  false },//narandzasti
        { { P(13, 8.5), P(13, 11.5), P(10, 11.5) },                3, RGB(255, 153, 204), false },//roze
        { { P(7, 8.5), P(10, 11.5), P(10, 14.5), P(7, 11.5) },     4, RGB(153, 0, 204),   false },//ljubicasti paralelogram
        { { P(7, 11.5), P(10, 14.5), P(7, 14.5) },                 3, RGB(0, 0, 255),     true  },//trougao sa sablonom
    };
    const int cnt = sizeof(parts) / sizeof(parts[0]);

    const CPoint redC = P(11.5, 13);            // centar kvadrata
    const int    redR = 53;                     // poluprecnik opisane kruznice
    const float  redRot = (float)(PI_D / 4);    // stranice
    
    //spoljasni mnogouglovi
    CPen pen(PS_SOLID, 4, RGB(0, 255, 255));
    CPen* oldPen = pDC->SelectObject(&pen);
    for (int i = 0; i < cnt; i++)
    {
        CBrush brush;
        COLORREF oldBk = pDC->GetBkColor();
        int oldMode = pDC->GetBkMode();
        if (parts[i].hatch)
        {
            brush.CreateHatchBrush(HS_CROSS, parts[i].clr);
            pDC->SetBkColor(RGB(255, 255, 255));
            pDC->SetBkMode(OPAQUE);
        }
        else
            brush.CreateSolidBrush(parts[i].clr);

        CBrush* ob = pDC->SelectObject(&brush);
        DrawPolygon(pDC, parts[i].pts, parts[i].n);
        pDC->SelectObject(ob);
        pDC->SetBkMode(oldMode);
        pDC->SetBkColor(oldBk);
    }
    
    //crveni kvadrat (pravilni mnogougao)
    CBrush red(RGB(255, 0, 0));
    CBrush* ob1 = pDC->SelectObject(&red);
    DrawRegularPolygon(pDC, redC.x, redC.y, redR, 4, redRot);
    pDC->SelectObject(ob1);
    
    
    //unutrasnji mnogouglovi
    CPen newPen(PS_SOLID, 2, RGB(0, 255, 255));
    pDC->SelectObject(&newPen);
    CBrush* ob2 = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

    CPoint c;
    c = P(5, 12.5);   DrawRegularPolygon(pDC, c.x, c.y, 22, 8, 0.0f);
    c = P(15, 12.5);  DrawRegularPolygon(pDC, c.x, c.y, 22, 5, 0.0f);
    c = P(10, 7.5);   DrawRegularPolygon(pDC, c.x, c.y, 16, 4, 0.0f);
    c = P(12, 10.5);  DrawRegularPolygon(pDC, c.x, c.y, 12, 6, 0.0f);
    c = P(8, 13.5);   DrawRegularPolygon(pDC, c.x, c.y, 12, 7, 0.0f);

    pDC->SelectObject(ob2);
    pDC->SelectObject(oldPen);

    if (grid)
        DrawGrid(pDC, CELL);
}


void CIND19748View::OnDraw(CDC* pDC)
{
	CIND19748Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
    
    CBrush bg(RGB(221, 221, 221));
    CRect rc(0, 0, 20 * CELL, 20 * CELL);
    pDC->FillRect(&rc, &bg);

    DrawTangram(pDC);
}


// CIND19748View printing

BOOL CIND19748View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND19748View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND19748View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND19748View diagnostics

#ifdef _DEBUG
void CIND19748View::AssertValid() const
{
	CView::AssertValid();
}

void CIND19748View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND19748Doc* CIND19748View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND19748Doc)));
	return (CIND19748Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND19748View message handlers
