// GL2AviView.cpp : implementation of the CGL2AviView class
//

#include "stdafx.h"
#include "GL2Avi.h"

#include "GL2AviDoc.h"
#include "GL2AviView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGL2AviView

IMPLEMENT_DYNCREATE(CGL2AviView, CView)

BEGIN_MESSAGE_MAP(CGL2AviView, CView)
	//{{AFX_MSG_MAP(CGL2AviView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGL2AviView construction/destruction

CGL2AviView::CGL2AviView()
{
	// TODO: add construction code here

}

CGL2AviView::~CGL2AviView()
{
}

BOOL CGL2AviView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN ;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGL2AviView drawing

void CGL2AviView::OnDraw(CDC* pDC)
{
	CGL2AviDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// making gl context current
	BOOL bResult = wglMakeCurrent (pDC->m_hDC, m_hrc);
	if (!bResult)
	{
		TRACE("wglMakeCurrent Failed %x\r\n", GetLastError() ) ;
		return ;
	}


	DrawGL();

	// Swap buffers.
	SwapBuffers(pDC->m_hDC) ;
}

/////////////////////////////////////////////////////////////////////////////
// CGL2AviView diagnostics

#ifdef _DEBUG
void CGL2AviView::AssertValid() const
{
	CView::AssertValid();
}

void CGL2AviView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGL2AviDoc* CGL2AviView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGL2AviDoc)));
	return (CGL2AviDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGL2AviView message handlers

int CGL2AviView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CClientDC dc(this) ;

	//
	// Fill in the pixel format descriptor.
	//
	PIXELFORMATDESCRIPTOR pfd ;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)) ;
	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR); 
	pfd.nVersion   = 1 ; 
	pfd.dwFlags    = PFD_DOUBLEBUFFER |
	                PFD_SUPPORT_OPENGL |
	                PFD_DRAW_TO_WINDOW ;
	pfd.iPixelType = PFD_TYPE_RGBA ;
	pfd.cColorBits = 16 ;
	pfd.cDepthBits = 32 ;
	pfd.cStencilBits = 8 ;
	pfd.iLayerType = PFD_MAIN_PLANE ;

	// Choosing avaible pixel format
   int nPixelFormat = ChoosePixelFormat(dc.m_hDC, &pfd);
   if (nPixelFormat == 0)
   {
      TRACE("ChoosePixelFormat Failed %d\r\n",GetLastError()) ;
      return -1 ;
   }
   TRACE("Pixel Format %d\r\n",nPixelFormat) ;

	// setting the pixel format
   BOOL bResult = SetPixelFormat (dc.m_hDC, nPixelFormat, &pfd);
   if (!bResult)
   {
      TRACE("SetPixelFormat Failed %d\r\n",GetLastError()) ;
      return -1 ;
   }

   //
   // Create rendering
   m_hrc = wglCreateContext(dc.m_hDC);
   if (!m_hrc)
   {
      TRACE("wglCreateContext Failed %x\r\n", GetLastError()) ;
      return -1;
   }

   CGL2AviApp* pApp=(CGL2AviApp*)AfxGetApp();
   pApp->SetIdleView(this);

	return 0;
}

void CGL2AviView::OnDestroy() 
{
	CView::OnDestroy();
	
	if (m_hrc)
	{
	   wglDeleteContext(m_hrc) ;
	   m_hrc = NULL ;
	}	
}

BOOL CGL2AviView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CGL2AviView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	ReSizeGLScene(cx,cy);

	InvalidateRect(NULL,FALSE);
}

int CGL2AviView::SetDCBITMAPPixelFormat(HDC hdc)
{
    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof (PIXELFORMATDESCRIPTOR),             // Size of this structure
        1,                                          // Version number
        PFD_DRAW_TO_BITMAP |                        // Flags
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL
        ,
        PFD_TYPE_RGBA,                              // RGBA pixel values
        16,                                         // 24-bit color
        0, 0, 0, 0, 0, 0,                           // Don't care about these
        0, 0,                                       // No alpha buffer
        0, 0, 0, 0, 0,                              // No accumulation buffer
        32,                                         // 32-bit depth buffer
        0,                                          // No stencil buffer
        0,                                          // No auxiliary buffers
        PFD_MAIN_PLANE,                             // Layer type
        0,                                          // Reserved (must be 0)
        0, 0, 0                                     // No layer masks
    };

    int nPixelFormat;
    
    nPixelFormat = ChoosePixelFormat (hdc, &pfd);
    if (SetPixelFormat(hdc, nPixelFormat, &pfd) == FALSE) {
      // SetPixelFormat error
      return FALSE ;
    }

    if (DescribePixelFormat(hdc, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR),&pfd) == 0) {
      // DescribePixelFormat error
      return FALSE ;
    }

    if (pfd.dwFlags & PFD_NEED_PALETTE) {
        // Need palete !
    }
    return TRUE ;
}

void CGL2AviView::ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void CGL2AviView::InitGL()
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
}

BOOL CGL2AviView::DrawGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glTranslatef(-1.5f,0.0f,-6.0f);						// Move Left 1.5 Units And Into The Screen 6.0
	glRotatef(m_rTri,0.0f,1.0f,0.0f);						// Rotate The Triangle On The Y axis ( NEW )
	glBegin(GL_TRIANGLES);								// Start Drawing A Triangle
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Front)
		glColor3f(0.0f,1.0f,0.0f);						// Green
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Left Of Triangle (Front)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Right Of Triangle (Front)
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Right)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Left Of Triangle (Right)
		glColor3f(0.0f,1.0f,0.0f);						// Green
		glVertex3f( 1.0f,-1.0f, -1.0f);					// Right Of Triangle (Right)
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Back)
		glColor3f(0.0f,1.0f,0.0f);						// Green
		glVertex3f( 1.0f,-1.0f, -1.0f);					// Left Of Triangle (Back)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f(-1.0f,-1.0f, -1.0f);					// Right Of Triangle (Back)
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Left)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Left Of Triangle (Left)
		glColor3f(0.0f,1.0f,0.0f);						// Green
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Right Of Triangle (Left)
	glEnd();											// Done Drawing The Pyramid

	glLoadIdentity();									// Reset The Current Modelview Matrix
	glTranslatef(1.5f,0.0f,-7.0f);						// Move Right 1.5 Units And Into The Screen 7.0
	glRotatef(m_rQuad,1.0f,1.0f,1.0f);					// Rotate The Quad On The X axis ( NEW )
	glBegin(GL_QUADS);									// Draw A Quad
		glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Blue
		glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f, 1.0f);					// Bottom Left Of The Quad (Top)
		glVertex3f( 1.0f, 1.0f, 1.0f);					// Bottom Right Of The Quad (Top)
		glColor3f(1.0f,0.5f,0.0f);						// Set The Color To Orange
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Top Right Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Top Left Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Bottom)
		glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Bottom)
		glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
		glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Front)
		glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Front)
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Front)
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Front)
		glColor3f(1.0f,1.0f,0.0f);						// Set The Color To Yellow
		glVertex3f( 1.0f,-1.0f,-1.0f);					// Top Right Of The Quad (Back)
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Top Left Of The Quad (Back)
		glVertex3f(-1.0f, 1.0f,-1.0f);					// Bottom Left Of The Quad (Back)
		glVertex3f( 1.0f, 1.0f,-1.0f);					// Bottom Right Of The Quad (Back)
		glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
		glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Left)
		glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Left)
		glColor3f(1.0f,0.0f,1.0f);						// Set The Color To Violet
		glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Right)
		glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Right)
	glEnd();											// Done Drawing The Quad

	m_rTri+=0.8f;											// Increase The Rotation Variable For The Triangle ( NEW )
	m_rQuad-=0.6f;										// Decrease The Rotation Variable For The Quad ( NEW )
	return TRUE;		
}

void CGL2AviView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	CClientDC dc(this);
	// making gl context current
	BOOL bResult = wglMakeCurrent (dc.m_hDC, m_hrc);
	if (!bResult)
	{
		TRACE("wglMakeCurrent Failed %x\r\n", GetLastError() ) ;
		return ;
	}
	
	InitGL();	
	m_rTri=0;
	m_rQuad=0;

	InvalidateRect(NULL,FALSE);
}
