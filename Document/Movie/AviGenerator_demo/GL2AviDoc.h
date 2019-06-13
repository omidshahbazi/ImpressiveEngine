// GL2AviDoc.h : interface of the CGL2AviDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GL2AVIDOC_H__A9947E62_8B7D_4B48_82E2_6C111D730411__INCLUDED_)
#define AFX_GL2AVIDOC_H__A9947E62_8B7D_4B48_82E2_6C111D730411__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGL2AviDoc : public CDocument
{
protected: // create from serialization only
	CGL2AviDoc();
	DECLARE_DYNCREATE(CGL2AviDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGL2AviDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGL2AviDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGL2AviDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GL2AVIDOC_H__A9947E62_8B7D_4B48_82E2_6C111D730411__INCLUDED_)
