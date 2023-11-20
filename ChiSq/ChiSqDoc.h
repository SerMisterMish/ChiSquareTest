
// ChiSqDoc.h : interface of the CChiSqDoc class
//


#pragma once
#include "DiscreteDistribution.h"
#include "InverseFunctionDG.h"
#include "DichotomyDG.h"

class CChiSqDoc : public CDocument
{
protected: // create from serialization only
	CChiSqDoc() noexcept;
	DECLARE_DYNCREATE(CChiSqDoc)

// Attributes
public:
	DiscreteDistribution h0_distribution, h1_distribution;
	DiscreteGenerator* generator;
	bool settings_applied;
	enum class image
	{
		NONE,
		HIST,
		PVDIST,
		POWER_LEVEL_ON_SAMPLE_SIZE,
	} current_image;

	int h0_type, h1_type, states, generation_method;
	CString h0_arb, h1_arb;
	double* h0_probabilities, *h1_probabilities, *power_levels, power_test_sl;
	size_t sample_size, p_value_sample_size, power_test_max, power_test_min;
	static constexpr size_t p_value_split_size{ 20 };
	long long p_value_sample[p_value_split_size];
// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CChiSqDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnHistogram();
	afx_msg void OnSettings();
	afx_msg void OnPvdf();
	afx_msg void OnPowerlevelonsamplesize();
};
