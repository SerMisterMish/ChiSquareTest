#pragma once
#include "afxdialogex.h"


// Диалоговое окно SettingsDialog

class SettingsDialog : public CDialog
{
	DECLARE_DYNAMIC(SettingsDialog)

public:
	SettingsDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	BOOL OnInitDialog() override;
	virtual ~SettingsDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	int m_h0_type;
	int m_h1_type;
	CString m_h0_probabilities;
	CString m_h1_probabilities;
	int m_states;
	int m_generation_method;
	CEdit m_h0_arb_edit;
	CEdit m_h0_uni_edit;
	CEdit m_h1_edit;
	afx_msg void OnBnClickedDtypeAH0();
	afx_msg void OnBnClickedDtypeUH0();
	afx_msg void OnBnClickedDtypeAH1();
	afx_msg void OnBnClickedDtypeUH2();
	afx_msg void OnEnChangeProbabilitiesH0();
	afx_msg void OnBnClickedOk();
	size_t m_sample_size;
	afx_msg void OnBnClickedButtonRandH0();
	afx_msg void OnBnClickedButtonRandH1();
	afx_msg void OnEnKillfocusStatesH0();
	size_t m_p_value_sample_size;
	size_t m_power_test_sample_size_min;
	size_t m_power_test_sample_size_max;
	double m_power_test_sl;
};
