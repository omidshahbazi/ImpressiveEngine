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

#include "PropertiesControl.h"

BEGIN_NAMESPACE
BEGIN_EVENT_TABLE(PropertiesControl, wxWindow)
	EVT_SIZE(PropertiesControl::OnSize)
	EVT_PG_CHANGED(IDC_PROPERTYGRID, PropertiesControl::OnPropertyChanged)
END_EVENT_TABLE()


PropertiesControl::PropertiesControl(wxWindow *Parent) :
	wxWindow(Parent, -1),
	m_Listener(0)
{
	wxFlexGridSizer *fgs = new wxFlexGridSizer(2, 1, 0, 0);
	fgs->SetFlexibleDirection(wxBOTH);
	fgs->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	InitializePropertyGrid();
	fgs->Add(m_PropertyGridManager, 0, wxALL);

	SetSizer(fgs);
}


//PropertiesControl::~PropertiesControl(void)
//{
//}

	
void PropertiesControl::InitializePropertyGrid(void)
{
	//int style = wxPG_BOLD_MODIFIED |
	//	wxPG_SPLITTER_AUTO_CENTER |
	//	wxPG_AUTO_SORT |
	//	//wxPG_HIDE_MARGIN|wxPG_STATIC_SPLITTER |
	//	//wxPG_TOOLTIPS |
	//	//wxPG_HIDE_CATEGORIES |
	//	//wxPG_LIMITED_EDITING |
	//	wxPG_TOOLBAR |
	//	wxPG_DESCRIPTION;

	//int extraStyle = wxPG_EX_MODE_BUTTONS |
	//	wxPG_EX_MULTIPLE_SELECTION |
	//	wxPG_EX_ENABLE_TLP_TRACKING |
	//	wxPG_EX_UNFOCUS_ON_ENTER;
	//	//| wxPG_EX_AUTO_UNSPECIFIED_VALUES
	//	//| wxPG_EX_GREY_LABEL_WHEN_DISABLED
	//	//| wxPG_EX_NATIVE_DOUBLE_BUFFERING
	//	//| wxPG_EX_HELP_AS_TOOLTIPS

	int style = //wxPG_BOLD_MODIFIED |
		wxPG_SPLITTER_AUTO_CENTER |
		//wxPG_AUTO_SORT |
		//wxPG_HIDE_MARGIN|wxPG_STATIC_SPLITTER |
		//wxPG_TOOLTIPS |
		//wxPG_HIDE_CATEGORIES |
		//wxPG_LIMITED_EDITING |
		wxPG_TOOLBAR |
		wxPG_DESCRIPTION;

	int extraStyle = wxPG_EX_MODE_BUTTONS
		| wxPG_EX_ENABLE_TLP_TRACKING
		//| wxPG_EX_UNFOCUS_ON_ENTER
		//| wxPG_EX_MULTIPLE_SELECTION;
		| wxPG_EX_AUTO_UNSPECIFIED_VALUES;
		//| wxPG_EX_GREY_LABEL_WHEN_DISABLED
		//| wxPG_EX_NATIVE_DOUBLE_BUFFERING
		//| wxPG_EX_HELP_AS_TOOLTIPS

	m_PropertyGridManager = new wxPropertyGridManager(this, IDC_PROPERTYGRID, wxDefaultPosition, wxDefaultSize, style);
	m_PropertyGridManager->SetExtraStyle(extraStyle);

	//m_PropertyGridManager->SetValidationFailureBehavior(wxPG_VFB_BEEP | wxPG_VFB_MARK_CELL);

	//m_PropertyGridManager->RegisterAdditionalEditors();

	m_PropertyGridPage = m_PropertyGridManager->GetPage(m_PropertyGridManager->AddPage("Properties"));
}
	

void PropertiesControl::OnSize(wxSizeEvent &event)
{
	m_PropertyGridManager->SetSize(event.GetSize());
}
	

void PropertiesControl::OnPropertyChanged(wxPropertyGridEvent &event)
{
	if (m_Listener)
	{
		//if (event.GetPropertyValue().IsNull())
		//	return;

		m_Listener->OnPropertyChanged(/*m_PropertyGridManager, */event);
	}
}
END_NAMESPACE

#endif