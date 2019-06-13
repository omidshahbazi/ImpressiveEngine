/*/////////////////////////////////////////////////////////////////////////////////
///
///  Impressive Engine
///
/// Copyright (c) 2012-2013 Impressive Reality team
//
/// The license is
///
/// Permission is denied, to any person or company
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// Project leader : O.Shahbazi <sh_omid_m@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////*/
#ifdef EDIT_MODE

#include "AboutWindow.h"
#include "Engine.h"


BEGIN_NAMESPACE
AboutWindow::AboutWindow(wxWindow *Parent) :
	wxDialog(Parent, wxID_ANY, "About Editor", wxDefaultPosition, wxSize(500, 316))
{
	SetBackgroundColour(wxColour(0, 0, 0));
	
	wxBoxSizer *bs = new wxBoxSizer(wxVERTICAL);

	bs->Add(new wxStaticBitmap(this, wxID_ANY, wxBitmap(Engine::PATH_EDITOR_IMAGE + "About.jpg", wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, 0), wxALL);
	
	SetSizer(bs);
	Layout();
	Centre(wxBOTH);
}


AboutWindow::~AboutWindow(void)
{
	Destroy();
}
END_NAMESPACE

#endif
