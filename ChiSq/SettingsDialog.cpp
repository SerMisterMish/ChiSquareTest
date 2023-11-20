// SettingsDialog.cpp: файл реализации
//

#include "pch.h"
#include "ChiSq.h"
#include "afxdialogex.h"
#include "SettingsDialog.h"
#include "random"
#include "chrono"


// Диалоговое окно SettingsDialog

IMPLEMENT_DYNAMIC(SettingsDialog, CDialog)

SettingsDialog::SettingsDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SETTINGS, pParent)
	, m_h0_type(1)
	, m_h1_type(1)
	, m_h0_probabilities(_T(""))
	, m_h1_probabilities(_T(""))
	, m_states(2)
	, m_generation_method(0)
	, m_sample_size(0)
	, m_power_test_sl(0)
{

}

BOOL SettingsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (m_h0_type == 0)
	{
		m_h0_uni_edit.EnableWindow(0);
	}
	else
	{
		m_h0_arb_edit.EnableWindow(0);
	}

	if (m_h1_type == 1)
	{
		m_h1_edit.EnableWindow(0);
	}
	return true;
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DTYPE_A_H0, m_h0_type);
	DDX_Radio(pDX, IDC_DTYPE_A_H1, m_h1_type);
	DDX_Text(pDX, IDC_PROBABILITIES_H0, m_h0_probabilities);
	DDX_Text(pDX, IDC_PROBABILITIES_H1, m_h1_probabilities);
	DDX_Text(pDX, IDC_STATES_H0, m_states);
	DDX_Radio(pDX, IDC_METHOD_IF, m_generation_method);
	DDX_Control(pDX, IDC_PROBABILITIES_H0, m_h0_arb_edit);
	DDX_Control(pDX, IDC_STATES_H0, m_h0_uni_edit);
	DDX_Control(pDX, IDC_PROBABILITIES_H1, m_h1_edit);
	DDX_Text(pDX, IDC_SAMPLE_SIZE, m_sample_size);
	DDX_Text(pDX, IDC_PVSIZE, m_p_value_sample_size);
	DDX_Text(pDX, IDC_POWER_TEST_BOT, m_power_test_sample_size_min);
	DDX_Text(pDX, IDC_POWER_TEST_TOP, m_power_test_sample_size_max);
	DDX_Text(pDX, IDC_POWER_TEST_SL, m_power_test_sl);
}


BEGIN_MESSAGE_MAP(SettingsDialog, CDialog)
	ON_BN_CLICKED(IDC_DTYPE_A_H0, &SettingsDialog::OnBnClickedDtypeAH0)
	ON_BN_CLICKED(IDC_DTYPE_U_H0, &SettingsDialog::OnBnClickedDtypeUH0)
	ON_BN_CLICKED(IDC_DTYPE_A_H1, &SettingsDialog::OnBnClickedDtypeAH1)
	ON_BN_CLICKED(IDC_DTYPE_U_H2, &SettingsDialog::OnBnClickedDtypeUH2)
	ON_EN_CHANGE(IDC_PROBABILITIES_H0, &SettingsDialog::OnEnChangeProbabilitiesH0)
	ON_BN_CLICKED(IDOK, &SettingsDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RAND_H0, &SettingsDialog::OnBnClickedButtonRandH0)
	ON_BN_CLICKED(IDC_BUTTON_RAND_H1, &SettingsDialog::OnBnClickedButtonRandH1)
	ON_EN_KILLFOCUS(IDC_STATES_H0, &SettingsDialog::OnEnKillfocusStatesH0)
END_MESSAGE_MAP()


// Обработчики сообщений SettingsDialog


void SettingsDialog::OnBnClickedDtypeAH0()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_h0_uni_edit.EnableWindow(0);
	m_h0_arb_edit.EnableWindow(1);
	OnEnChangeProbabilitiesH0();
}


void SettingsDialog::OnBnClickedDtypeUH0()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_h0_uni_edit.EnableWindow(1);
	m_h0_arb_edit.EnableWindow(0);
}


void SettingsDialog::OnBnClickedDtypeAH1()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_h1_edit.EnableWindow(1);
}


void SettingsDialog::OnBnClickedDtypeUH2()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_h1_edit.EnableWindow(0);
}


void SettingsDialog::OnEnChangeProbabilitiesH0()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialog::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
	UpdateData(1);
	size_t len = m_h0_probabilities.GetLength();
	m_states = 0;
	for (size_t j{}; j < len; ++j)
	{
		if (m_h0_probabilities[j] != '.' && m_h0_probabilities[j] != ' ' && (m_h0_probabilities[j] < '0' || m_h0_probabilities[j] > '9'))
		{
			AfxMessageBox(L"Probabilities must be real non-negative numbers.");
			m_h0_probabilities = L"";
			UpdateData(0);
			return;
		}
		if (m_h0_probabilities[j] == ' ' && j != 0 && m_h0_probabilities[j - 1] != ' ')
			++m_states;
	}
	if (len > 0 && m_h0_probabilities[len - 1] != ' ')
		++m_states;
	UpdateData(0);
}


void SettingsDialog::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(1);

	if (m_states < 2)
	{
		AfxMessageBox(L"Number of states must be 2 or more.");
		return;
	}

	if (m_sample_size < 1)
	{
		AfxMessageBox(L"Size of the sample must be no less than 1.");
		return;
	}
	if (m_h1_type == 0)
	{
		size_t count{}, len = m_h1_probabilities.GetLength();
		for (size_t j{}; j < len; ++j)
		{
			if (m_h1_probabilities[j] != '.' && m_h1_probabilities[j] != ' ' && (m_h1_probabilities[j] < '0' || m_h1_probabilities[j] > '9'))
			{
				AfxMessageBox(L"Probabilities must be real non-negative numbers.");
				m_h1_probabilities = L"";
				UpdateData(0);
				return;
			}
			if (m_h1_probabilities[j] == ' ' && j != 0 && m_h1_probabilities[j - 1] != ' ')
				++count;
		}
		if (len > 0 && m_h1_probabilities[len - 1] != ' ')
			++count;
		if (m_states != count)
		{
			AfxMessageBox(L"Number of states for H0 and H1 must be the same.");
			return;
		}
	}
	
	CDialog::OnOK();
}

CString GetRandomDistribution(int& states)
{
	size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_real_distribution<double> uni_dist(0, 1);
	if (states < 2)
	{
		std::poisson_distribution<short> poiss_dist(5);
		states = poiss_dist(generator) + 2;
	}
	double* probabilities = new double[states], sum{};
	for (size_t j{}; j < states; ++j)
	{
		probabilities[j] = uni_dist(generator);
		sum += probabilities[j];
	}
	CString num_string{}, probabilities_str{};
	probabilities_str = L"";
	for (size_t j{}; j < states; ++j)
	{
		num_string.Format(L"%lf", probabilities[j] / sum);
		probabilities_str += num_string + ' ';
	}

	delete[] probabilities;
	return probabilities_str;
}

void SettingsDialog::OnBnClickedButtonRandH0()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_h0_probabilities = GetRandomDistribution(m_states);
	m_h0_type = 0;
	UpdateData(0);
	OnBnClickedDtypeAH0();
}


void SettingsDialog::OnBnClickedButtonRandH1()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_h1_probabilities = GetRandomDistribution(m_states);
	m_h1_type = 0;
	UpdateData(0);
	OnBnClickedDtypeAH1();
}

void SettingsDialog::OnEnKillfocusStatesH0()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(1);
}
