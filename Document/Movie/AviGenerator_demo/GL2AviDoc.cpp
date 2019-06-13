// GL2AviDoc.cpp : implementation of the CGL2AviDoc class
//

#include "stdafx.h"
#include "GL2Avi.h"

#include "GL2AviDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGL2AviDoc

IMPLEMENT_DYNCREATE(CGL2AviDoc, CDocument)

BEGIN_MESSAGE_MAP(CGL2AviDoc, CDocument)
	//{{AFX_MSG_MAP(CGL2AviDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGL2AviDoc construction/destruction

CGL2AviDoc::CGL2AviDoc()
{
	// TODO: add one-time construction code here

}

CGL2AviDoc::~CGL2AviDoc()
{
}

BOOL CGL2AviDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGL2AviDoc serialization

void CGL2AviDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CGL2AviDoc diagnostics

#ifdef _DEBUG
void CGL2AviDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGL2AviDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGL2AviDoc commands
