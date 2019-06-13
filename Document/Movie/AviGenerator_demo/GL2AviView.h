// GL2AviView.h : interface of the CGL2AviView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GL2AVIVIEW_H__D7A22D25_D7B5_48AD_9F1A_92BE5451CF12__INCLUDED_)
#define AFX_GL2AVIVIEW_H__D7A22D25_D7B5_48AD_9F1A_92BE5451CF12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGL2AviView : public CView
{
protected: // create from serialization only
	CGL2AviView();
	DECLARE_DYNCREATE(CGL2AviView)
	BOOL DrawGL();
// Attributes
public:
	CGL2AviDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGL2AviView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGL2AviView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	GLfloat m_rQuad;
	GLfloat m_rTri;

	void InitGL();
	void ReSizeGLScene(GLsizei width, GLsizei height);
	HGLRC m_hrc;
	//{{AFX_MSG(CGL2AviView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int SetDCBITMAPPixelFormat(HDC hdc);
};

#ifndef _DEBUG  // debug version in GL2AviView.cpp
inline CGL2AviDoc* CGL2AviView::GetDocument()
   { return (CGL2AviDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GL2AVIVIEW_H__D7A22D25_D7B5_48AD_9F1A_92BE5451CF12__INCLUDED_)
