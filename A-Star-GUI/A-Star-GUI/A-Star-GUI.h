
// A-Star-GUI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAStarGUIApp:
// See A-Star-GUI.cpp for the implementation of this class
//

class CAStarGUIApp : public CWinApp
{
public:
	CAStarGUIApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CAStarGUIApp theApp;
