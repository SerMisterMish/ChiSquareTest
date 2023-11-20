// ChiSqDoc.cpp : implementation of the CChiSqDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ChiSq.h"
#include "ChiSquare.h"
#endif

#include "ChiSqDoc.h"
#include "chrono"
#include "SettingsDialog.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChiSqDoc

IMPLEMENT_DYNCREATE(CChiSqDoc, CDocument)

BEGIN_MESSAGE_MAP(CChiSqDoc, CDocument)
	ON_COMMAND(ID_HISTOGRAM, &CChiSqDoc::OnHistogram)
	ON_COMMAND(ID_SETTINGS, &CChiSqDoc::OnSettings)
	ON_COMMAND(ID_PVDF, &CChiSqDoc::OnPvdf)
	ON_COMMAND(ID_POWERLEVELONSAMPLESIZE, &CChiSqDoc::OnPowerlevelonsamplesize)
END_MESSAGE_MAP()


// CChiSqDoc construction/destruction

CChiSqDoc::CChiSqDoc() noexcept : h0_distribution({0.1, 0.2, 0.3, 0.4}), h1_distribution(4),
                                  generator(nullptr), settings_applied(false), current_image(image::NONE), h0_type(1),
                                  h1_type(1), states(2), generation_method(0), h0_arb(L""), h1_arb(L""),
                                  h0_probabilities(nullptr),
                                  h1_probabilities(nullptr), power_levels(nullptr), power_test_sl(0.1), sample_size(100),
                                  p_value_sample_size(10000), power_test_max(200),
                                  power_test_min(100), p_value_sample()
{
	// TODO: add one-time construction code here
}

CChiSqDoc::~CChiSqDoc()
{
	delete generator;
	delete[] h0_probabilities;
	delete[] h1_probabilities;
	delete[] power_levels;
}

BOOL CChiSqDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CChiSqDoc serialization

void CChiSqDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CChiSqDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CChiSqDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CChiSqDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CChiSqDoc diagnostics

#ifdef _DEBUG
void CChiSqDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CChiSqDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CChiSqDoc commands


void CChiSqDoc::OnHistogram()
{
	// TODO: добавьте свой код обработчика команд
	if (!settings_applied)
		OnSettings();
	if (settings_applied)
	{
		current_image = image::HIST;
		generator->Simulate();
		UpdateAllViews(0);
	}
}


void CChiSqDoc::OnSettings()
{
	// TODO: добавьте свой код обработчика команд
	SettingsDialog SD;
	SD.m_h1_type = h1_type;
	SD.m_h0_type = h0_type;
	SD.m_generation_method = generation_method;
	SD.m_h0_probabilities = h0_arb;
	SD.m_h1_probabilities = h1_arb;
	SD.m_states = states;
	SD.m_sample_size = sample_size;
	SD.m_p_value_sample_size = p_value_sample_size;
	SD.m_power_test_sample_size_max = power_test_max;
	SD.m_power_test_sample_size_min = power_test_min;
	SD.m_power_test_sl = power_test_sl;
	if (SD.DoModal() == IDOK)
	{
		h1_type = SD.m_h1_type;
		h0_type = SD.m_h0_type;
		generation_method = SD.m_generation_method;
		h0_arb = SD.m_h0_probabilities;
		h1_arb = SD.m_h1_probabilities;
		states = SD.m_states;
		sample_size = SD.m_sample_size;
		p_value_sample_size = SD.m_p_value_sample_size;
		bool power_test_boundaries_changed{ false };
		if (power_test_max != SD.m_power_test_sample_size_max || power_test_min != SD.m_power_test_sample_size_min)
			power_test_boundaries_changed = true;
		power_test_max = SD.m_power_test_sample_size_max;
		power_test_min = SD.m_power_test_sample_size_min;
		power_test_sl = SD.m_power_test_sl;

		delete[] h1_probabilities;
		delete[] h0_probabilities;
		h1_probabilities = new double[states];
		h0_probabilities = new double[states];

		// Getting H0 probabilities
		if (h0_type == 1)
		{
			for (size_t j{}; j < states; ++j)
			{
				h0_probabilities[j] = 1.0 / states;
			}
		}
		else
		{
			CString numbers{};
			size_t len = h0_arb.GetLength(), count{};
			double sum{};
			for (size_t j{}; j < len; ++j)
			{
				if (h0_arb[j] != ' ')
				{
					numbers += h0_arb[j];
				}
				else if (j != 0 && h0_arb[j - 1] != ' ')
				{
					h0_probabilities[count++] = _tstof(numbers);
					sum += h0_probabilities[count - 1];
					numbers = L"";
				}
			}
			if (!numbers.IsEmpty())
			{
				h0_probabilities[count++] = _tstof(numbers);
				sum += h0_probabilities[count - 1];
			}
			for (size_t j{}; j < states; ++j)
			{
				h0_probabilities[j] /= sum;
			}
		}

		// Getting H1 probabilities
		if (h1_type == 1)
		{
			for (size_t j{}; j < states; ++j)
			{
				h1_probabilities[j] = 1.0 / states;
			}
		}
		else
		{
			CString numbers{};
			size_t len = h1_arb.GetLength(), count{};
			double sum{};
			for (size_t j{}; j < len; ++j)
			{
				if (h1_arb[j] != ' ')
				{
					numbers += h1_arb[j];
				}
				else if (j != 0 && h1_arb[j - 1] != ' ')
				{
					h1_probabilities[count++] = _tstof(numbers);
					sum += h1_probabilities[count - 1];
					numbers = L"";
				}
			}
			if (!numbers.IsEmpty())
			{
				h1_probabilities[count++] = _tstof(numbers);
				sum += h1_probabilities[count - 1];
			}
			for (size_t j{}; j < states; ++j)
			{
				h1_probabilities[j] /= sum;
			}
		}

		settings_applied = true;

		h0_distribution.SetProbabilities(h0_probabilities, states);
		h1_distribution.SetProbabilities(h1_probabilities, states);
		delete generator;
		if (generation_method == 0)
			generator = new InverseFunctionDG(h1_distribution, sample_size);
		else
			generator = new DichotomyDG(h1_distribution, sample_size);

		generator->Simulate();
		// delete[] h1_probabilities;
		// delete[] h0_probabilities;
		// h1_probabilities = nullptr;
		// h0_probabilities = nullptr;

		switch (current_image)
		{
		case image::HIST:
			OnHistogram();
			break;
		case image::PVDIST:
			OnPvdf();
			break;
		default:
			break;
		}

		if (power_test_boundaries_changed)
			OnPowerlevelonsamplesize();
		UpdateAllViews(0);
	}
}

// Generates a sample of p-values by performing multiple chi-squared tests.
void CChiSqDoc::OnPvdf()
{
	// TODO: добавьте свой код обработчика команд
	if (!settings_applied || !p_value_sample_size)
	{
		OnSettings();
	}
	if (!settings_applied)
	{
		return;
	}
	if (!p_value_sample_size)
	{
		AfxMessageBox(L"Incorrect p-value sample size");
		return;
	}

	memset(p_value_sample, 0, p_value_split_size * sizeof(long long));
	ChiSquare chi(generator, h0_distribution);
	for (size_t i{}; i < p_value_sample_size; ++i)
	{
		size_t index = static_cast<size_t>(chi.GetPValue() * p_value_split_size);
		if (index == 20)
			index = 19;
		++p_value_sample[index];
		chi.CalculateStatistic();
	}

	for (size_t i{1}; i < p_value_split_size; ++i)
	{
		p_value_sample[i] += p_value_sample[i - 1];
	}

	current_image = image::PVDIST;
	UpdateAllViews(0);
}


void CChiSqDoc::OnPowerlevelonsamplesize()
{
	// TODO: добавьте свой код обработчика команд
	if (!settings_applied)
	{
		OnSettings();
	}
	if (!settings_applied)
	{
		return;
	}
	if (power_test_min < 2 || power_test_max <= power_test_min || power_test_sl <= 0 || power_test_sl >= 1)
	{
		AfxMessageBox(L"Incorrect settings for the power level on sample size graph");
	}

	DiscreteGenerator* multi_gen;
	switch (generator->GetGenType())
	{
	case DiscreteGenerator::generator_type::DICHOTOMY:
		multi_gen = new DichotomyDG(generator);
		break;
	case DiscreteGenerator::generator_type::INVERSE_FUNCTION:
	default:
		multi_gen = new InverseFunctionDG(generator);
	}

	// multi_gen->SetSampleSize(power_test_min);
	ChiSquare chi(multi_gen, h0_distribution);
	delete[] power_levels;
	power_levels = new double [power_test_max - power_test_min + 1];
	for (size_t i{power_test_min}; i <= power_test_max; ++i)
	{
		size_t p_less_than_sl{};
		chi.ChangeSampleSize(i);
		for (size_t j{}; j < p_value_sample_size; ++j)
		{
			chi.CalculateStatistic();
			if (chi.GetPValue() < power_test_sl)
				++p_less_than_sl;
		}
		power_levels[i - power_test_min] = static_cast<double>(p_less_than_sl) / p_value_sample_size;
	}
	delete multi_gen;

	current_image = image::POWER_LEVEL_ON_SAMPLE_SIZE;
	UpdateAllViews(0);
}
