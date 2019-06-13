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

#pragma once

#include "StdAfxwxWidgets.h"
#include "Common.h"


BEGIN_NAMESPACE
class WindowBase : public wxDialog
{
public:
	WindowBase(const wxString &Text, const wxSize &Size);
	virtual ~WindowBase(void) {}

protected:
	void OnKeyUp(wxKeyEvent &event);

private:
	DECLARE_EVENT_TABLE()
};
END_NAMESPACE

#endif
