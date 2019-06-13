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
#include "IPropertiesListener.h"


BEGIN_NAMESPACE
class PropertiesControl : public wxWindow
{
public:
	PropertiesControl(wxWindow *Parent);
	//~PropertiesControl(void);

	wxPropertyGridManager *GetPropertyGridManager(void)
	{
		return m_PropertyGridManager;
	}

	wxPropertyGridPage *GetPropertyGridPage(void)
	{
		return m_PropertyGridPage;
	}

	void SetListener(IPropertiesListener *Listener)
	{
		m_Listener = Listener;
	}

private:
	void InitializePropertyGrid(void);

private:
	DECLARE_EVENT_TABLE()

	enum
	{
		IDC_PROPERTYGRID = 1000
	};

	void OnSize(wxSizeEvent &event);
	void OnPropertyChanged(wxPropertyGridEvent &event);

private:
	wxPropertyGridManager* m_PropertyGridManager;
	wxPropertyGridPage *m_PropertyGridPage;

	IPropertiesListener *m_Listener;
};
END_NAMESPACE

#endif
