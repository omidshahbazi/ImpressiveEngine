// GL2Avi.h : main header file for the GL2AVI application
//

#if !defined(AFX_GL2AVI_H__F1858DCD_1279_4C5B_9A04_CBE9BD578B08__INCLUDED_)
#define AFX_GL2AVI_H__F1858DCD_1279_4C5B_9A04_CBE9BD578B08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGL2AviApp:
// See GL2Avi.cpp for the implementation of this class
//

class CGL2AviApp : public CWinApp
{
public:
	void SetIdleView(CView* pView);
	CGL2AviApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGL2AviApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGL2AviApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CView* m_pIdleView;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GL2AVI_H__F1858DCD_1279_4C5B_9A04_CBE9BD578B08__INCLUDED_)
