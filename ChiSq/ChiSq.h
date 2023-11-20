
// ChiSq.h : main header file for the ChiSq application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
//#include <random>
//extern std::default_random_engine global_rne;
// CChiSqApp:
// See ChiSq.cpp for the implementation of this class
//


class CChiSqApp : public CWinAppEx
{
public:
	CChiSqApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CChiSqApp theApp;
