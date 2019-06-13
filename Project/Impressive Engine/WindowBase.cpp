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

#include "WindowBase.h"
#include "MainWindow.h"


BEGIN_NAMESPACE
BEGIN_EVENT_TABLE(WindowBase, wxDialog)
	//EVT_CHAR_HOOK(WindowBase::OnKeyUp)
END_EVENT_TABLE()


WindowBase::WindowBase(const wxString &Text, const wxSize &Size) :
	wxDialog(MainWindow::GetPointer(), wxID_ANY, Text, wxDefaultPosition, Size)
{
}


void WindowBase::OnKeyUp(wxKeyEvent &event)
{
	if (event.GetKeyCode() == WXK_ESCAPE)
	{
		Close(true);

		//delete this;
	}
}

END_NAMESPACE
#endif