// ChiSqView.cpp : implementation of the CChiSqView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ChiSq.h"
#endif

#include "ChiSqDoc.h"
#include "ChiSqView.h"
#include "DiscreteDistribution.h"
#include "InverseFunctionDG.h"
#include "DichotomyDG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChiSqView

IMPLEMENT_DYNCREATE(CChiSqView, CView)

BEGIN_MESSAGE_MAP(CChiSqView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CChiSqView construction/destruction

CChiSqView::CChiSqView() noexcept
{
	// TODO: add construction code here
}

CChiSqView::~CChiSqView()
{
}

BOOL CChiSqView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CChiSqView drawing

void CChiSqView::OnDraw(CDC* pDC)
{
	CChiSqDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->SetTitle(L"Chi-Square test");

	// TODO: add draw code for native data here
	switch (pDoc->current_image)
	{
	case CChiSqDoc::image::HIST:
		DrawHist();
		break;
	case CChiSqDoc::image::PVDIST:
		DrawPVDF();
		break;
	case CChiSqDoc::image::POWER_LEVEL_ON_SAMPLE_SIZE:
		DrawPowerLevelOnSampleSize();
		break;
	case CChiSqDoc::image::NONE:
		break;
	}
}

void CChiSqView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CChiSqView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CChiSqView diagnostics

#ifdef _DEBUG
void CChiSqView::AssertValid() const
{
	CView::AssertValid();
}

void CChiSqView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CChiSqDoc* CChiSqView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChiSqDoc)));
	return (CChiSqDoc*)m_pDocument;
}
#endif //_DEBUG


// CChiSqView message handlers

// Draws a histogram of generated sample against expected frequencies.
void CChiSqView::DrawHist()
{
	// TODO: Добавьте сюда код реализации.
	CClientDC* pDC = new CClientDC(this);
	CChiSqDoc* pDoc = GetDocument();
	CRect rc;
	GetClientRect(&rc);
	int height = rc.Height(), width = rc.Width();

	// Axes drawing
	pDC->MoveTo(width / 18, height * 11 / 12);
	pDC->LineTo(width / 18, height / 12);
	pDC->LineTo(width / 18 - 6, height / 12 + 6);
	pDC->MoveTo(width / 18, height / 12);
	pDC->LineTo(width / 18 + 6, height / 12 + 6);
	pDC->MoveTo(width / 18, height * 11 / 12);
	pDC->LineTo(width * 17 / 18, height * 11 / 12);
	pDC->LineTo(width * 17 / 18 - 6, height * 11 / 12 - 6);
	pDC->MoveTo(width * 17 / 18, height * 11 / 12);
	pDC->LineTo(width * 17 / 18 - 6, height * 11 / 12 + 6);

	// Text drawing
	size_t states = pDoc->h0_distribution.GetStatesNumber();
	const double* probabilities = pDoc->h0_distribution.GetProbabilities();
	size_t sample_size = pDoc->generator->GetSampleSize();
	const long long* sample = pDoc->generator->GetSample();
	long long max_value_real{}, max_value_theoretical{};
	for (size_t j{}; j < states; ++j)
	{
		if (sample[j] > max_value_real)
			max_value_real = sample[j];
		if (probabilities[j] * sample_size > max_value_theoretical)
			max_value_theoretical = probabilities[j] * sample_size;
	}
	long long max_value = max(max_value_real, max_value_theoretical);
	pDC->MoveTo(width / 18, height * 11 / 12);
	double vertical_step = 19.0 * height / 24 / 16, horizontal_step = 8.0 * width / 9 / (states + 1);
	CString numbers;
	// Vertical axis
	for (size_t j{1}; j <= 16; ++j)
	{
		pDC->MoveTo(width / 18 - 4, height * 11 / 12 - vertical_step * j);
		pDC->LineTo(width / 18 + 4, height * 11 / 12 - vertical_step * j);
		numbers.Format(L"%llu", static_cast<size_t>(round(24.0 / 19 / height * max_value * j * vertical_step)));
		pDC->TextOutW(width / 18 - 30, height * 11 / 12 - vertical_step * j - 8, numbers);
	}
	// Horizontal axis
	for (size_t j{1}; j <= states; ++j)
	{
		pDC->MoveTo(horizontal_step * j + width / 18, height * 11 / 12 + 4);
		pDC->LineTo(horizontal_step * j + width / 18, height * 11 / 12 - 4);
		numbers.Format(L"%llu", j - 1);
		pDC->TextOutW(horizontal_step * j + width / 18 - 8, height * 11 / 12 + 30, numbers);
	}

	// Columns drawing
	for (size_t j{1}; j <= states; ++j)
	{
		LOGBRUSH lb;
		lb.lbColor = RGB(100, 100, 100);
		lb.lbStyle = BS_SOLID;

		CBrush brush, *oldBrush;
		brush.CreateBrushIndirect(&lb);
		oldBrush = pDC->SelectObject(&brush);
		// Theoretical
		pDC->Rectangle(horizontal_step * j - horizontal_step / 3 + width / 18, height * 11 / 12,
		               horizontal_step * j + horizontal_step / 3 + width / 18,
		               height * 11 / 12 - (probabilities[j - 1] * sample_size)
		               * 19 * height / 24 / max_value);

		lb.lbColor = RGB(200, 50, 50);
		brush.DeleteObject();
		brush.CreateBrushIndirect(&lb);
		pDC->SelectObject(&brush);
		// Real
		pDC->Rectangle(horizontal_step * j - horizontal_step / 6 + width / 18, height * 11 / 12,
		               horizontal_step * j + horizontal_step / 6 + width / 18,
		               height * 11 / 12 - sample[j - 1] * 19 * height / 24 / max_value);
		pDC->SelectObject(oldBrush);
		brush.DeleteObject();
	}

	delete pDC;
}

// Draws p-value distribution function
void CChiSqView::DrawPVDF()
{
	CClientDC* pDC = new CClientDC(this);
	CChiSqDoc* pDoc = GetDocument();
	CRect rc;
	GetClientRect(&rc);
	int height = rc.Height(), width = rc.Width();

	// Axes drawing
	pDC->MoveTo(width / 18, height * 11 / 12);
	pDC->LineTo(width / 18, height / 12);
	pDC->MoveTo(width / 18, height * 11 / 12);
	pDC->LineTo(width * 17 / 18, height * 11 / 12);
	pDC->TextOutW(width * 17 / 18 + 15, height * 11 / 12, L"x");
	pDC->TextOutW(width / 18 - 12, height / 12 - 24, L"p");

	// Diagonal line
	pDC->MoveTo(width / 18, height * 11 / 12);
	pDC->LineTo(width * 17 / 18, height / 12);

	// Text drawing
	size_t split_size = pDoc->p_value_split_size;
	double vertical_step{5.0 * height / 6 / split_size}, horizontal_step{8.0 * width / 9 / split_size};
	CString numbers;
	// Vertical axis
	for (size_t j{1}; j <= split_size; ++j)
	{
		pDC->MoveTo(width / 18 - 4, height * 11 / 12 - vertical_step * j);
		pDC->LineTo(width / 18 + 4, height * 11 / 12 - vertical_step * j);
		numbers.Format(L"%.2lf", static_cast<double>(j) / split_size);
		pDC->TextOutW(width / 18 - 32, height * 11 / 12 - vertical_step * j, numbers);
	}
	// Horizontal axis
	for (size_t j{1}; j <= split_size; ++j)
	{
		pDC->MoveTo(horizontal_step * j + width / 18, height * 11 / 12 + 4);
		pDC->LineTo(horizontal_step * j + width / 18, height * 11 / 12 - 4);
		numbers.Format(L"%.2lf", static_cast<double>(j) / split_size);
		pDC->TextOutW(horizontal_step * j + width / 18 - 12, height * 11 / 12 + 25, numbers);
	}

	// Drawing distribution function
	const long long* p_sample = pDoc->p_value_sample;
	size_t p_size = pDoc->p_value_sample_size;
	CPen redPen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* oldPen = pDC->SelectObject(&redPen);
	double prev_height{};
	for (size_t i{}; i < split_size; ++i)
	{
		pDC->MoveTo(width / 18 + i * horizontal_step, height * 11.0 / 12 - prev_height);
		prev_height = height * 5.0 * (static_cast<double>(p_sample[i]) / p_size) / 6;
		pDC->LineTo(width / 18 + (i + 1) * horizontal_step, height * 11.0 / 12 - prev_height);
	}

	pDC->SelectObject(oldPen);
	delete pDC;
}

void CChiSqView::DrawPowerLevelOnSampleSize()
{
	CClientDC* pDC = new CClientDC(this);
	CChiSqDoc* pDoc = GetDocument();
	CRect rc;
	GetClientRect(&rc);
	int height = rc.Height(), width = rc.Width();

	// Axes drawing
	pDC->MoveTo(width / 18, height * 11 / 12);
	pDC->LineTo(width / 18, height / 12);
	pDC->LineTo(width / 18 - 6, height / 12 + 6);
	pDC->MoveTo(width / 18, height / 12);
	pDC->LineTo(width / 18 + 6, height / 12 + 6);
	pDC->MoveTo(width / 18, height * 11 / 12);
	pDC->LineTo(width * 17 / 18, height * 11 / 12);
	pDC->LineTo(width * 17 / 18 - 6, height * 11 / 12 - 6);
	pDC->MoveTo(width * 17 / 18, height * 11 / 12);
	pDC->LineTo(width * 17 / 18 - 6, height * 11 / 12 + 6);

	// Text drawing
	size_t min_size{pDoc->power_test_min}, max_size{pDoc->power_test_max};
	const double* power_levels = pDoc->power_levels;
	double max_value{};
	for (size_t j{0}; j <= max_size - min_size; ++j)
	{
		if (power_levels[j] > max_value)
			max_value = power_levels[j];
	}

	double vertical_step = 19.0 * height / 24 / 16, horizontal_step = 8.0 * width / 9 / 25;
	CString numbers;
	// Vertical axis
	for (size_t j{1}; j <= 16; ++j)
	{
		pDC->MoveTo(width / 18 - 4, height * 11 / 12 - vertical_step * j);
		pDC->LineTo(width / 18 + 4, height * 11 / 12 - vertical_step * j);
		numbers.Format(L"%0.2lf", 24.0 / 19 / height * max_value * j * vertical_step);
		pDC->TextOutW(width / 18 - 40, height * 11 / 12 - vertical_step * j - 8, numbers);
	}
	// Horizontal axis
	for (size_t j{0}; j <= 24; ++j)
	{
		pDC->MoveTo(horizontal_step * j + width / 18, height * 11 / 12 + 4);
		pDC->LineTo(horizontal_step * j + width / 18, height * 11 / 12 - 4);
		numbers.Format(L"%llu", min_size + static_cast<size_t>(round(
			               (max_size - min_size) * j * horizontal_step * 25.0 / 24.0 * 9.0 / 8.0 / width)));
		pDC->TextOutW(horizontal_step * j + width / 18 - 14, height * 11 / 12 + 30, numbers);
	}

	// Drawing power-level on sample size dependance graph
	horizontal_step = 8.0 * 24 / 25 * width / 9 / (max_size - min_size + 1);
	double prev_height{ height * 19.0 * power_levels[0] / 24 / max_value };
	for (size_t i{ 0 }; i <= max_size - min_size; ++i)
	{
		pDC->MoveTo(width / 18 + i * horizontal_step, height * 11.0 / 12 - prev_height);
		prev_height = height * 19.0 * power_levels[i] / 24 / max_value;
		pDC->LineTo(width / 18 + (i + 1) * horizontal_step, height * 11.0 / 12 - prev_height);
	}

	delete pDC;
}
