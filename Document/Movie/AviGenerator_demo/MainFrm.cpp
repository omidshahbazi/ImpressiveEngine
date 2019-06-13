// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GL2Avi.h"

#include "MainFrm.h"
#include "GL2AviDoc.h"
#include "GL2AviView.h"
#include "AVIGenerator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_AVIGENERATION_GENERATE, OnAvigenerationGenerate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnAvigenerationGenerate() 
{
	UINT i,nFrames=200;

	CAVIGenerator AviGen;
	LPBITMAPINFOHEADER lpbih;
	BYTE* bmBits;	
	CGL2AviView* pView=(CGL2AviView*)GetActiveView();
	HRESULT hr;

	CProgressBar Bar(_T("Generating movie"), 40, nFrames, true);
	Bar.SetStep(1);

	BeginWaitCursor();

	// set 15fps
	AviGen.SetRate(15);
	
	// give info about bitmap
	AviGen.SetBitmapHeader(pView);		

	// set filename, extension ".avi" is appended if necessary
	AviGen.SetFileName(_T("test.avo"));

	// retreiving size of image
	lpbih=AviGen.GetBitmapHeader();

	// allocating memory
	bmBits=new BYTE[lpbih->biSizeImage];

	hr=AviGen.InitEngine();
	if (FAILED(hr))
	{
		AfxMessageBox( AviGen.GetLastErrorMessage());
		goto Cleaning;
	}

	// reading back buffer
	glReadBuffer(GL_BACK);
	for(i=0;i<nFrames;i++)
	{
		// render frame
		pView->DrawGL();
		// Copy from OpenGL to buffer
		glReadPixels(0,0,lpbih->biWidth,lpbih->biHeight,GL_BGR_EXT,GL_UNSIGNED_BYTE,bmBits); 
		// send to avi engine
		hr=AviGen.AddFrame(bmBits);
		if (FAILED(hr))
		{
			AfxMessageBox( AviGen.GetLastErrorMessage());
			goto Cleaning;
		}
		Bar.StepIt();
	}

Cleaning:
	// releasing engine and memory
	AviGen.ReleaseEngine();
	delete[] bmBits;

	glReadBuffer(GL_FRONT);

	EndWaitCursor();
}
