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

#include "WindowBase.h"


BEGIN_NAMESPACE
class TerrainWindow : public WindowBase
{
public:
	TerrainWindow(void);
	~TerrainWindow(void);

private:
	void bAddOnButtonClick( wxCommandEvent& event );
	void bDeleteOnButtonClick( wxCommandEvent& event );
	void bCancelOnButtonClick( wxCommandEvent& event );
	void bCreateOnButtonClick( wxCommandEvent& event );

private:
	wxTextCtrl* m_tbName;
	wxTextCtrl* m_tbHeight;
	wxTextCtrl* m_tbHeightMap;
	wxChoice* m_cSize;
	wxTextCtrl* m_tbWorldSize;
	wxTextCtrl* m_tbTextureName;
	wxTextCtrl* m_tbTextureSize;
	wxButton* m_bAdd;
	wxButton* m_bDelete;
	wxListBox* m_lbTextures;
	wxButton* m_bCancel;
	wxButton* m_bCreate;
};
END_NAMESPACE

#endif