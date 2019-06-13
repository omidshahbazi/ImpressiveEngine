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
class NavMeshGenWindow: public WindowBase
{

public:
    NavMeshGenWindow(void);
    ~NavMeshGenWindow(void);

    
private:
    void bCancelOnButtonClick( wxCommandEvent& event );
	void bCreateOnButtonClick( wxCommandEvent& event );
	void bLoadOnButtonClick( wxCommandEvent& event );
    void rbManualonRadioButton( wxCommandEvent& event );
    void rbTerrainonRadioButton( wxCommandEvent& event );

private:
    wxStaticText     *m_stTerrainLabel;
	wxChoice         *m_cTerrains;
	wxRadioButton    *m_rbTerrain;
	wxRadioButton    *m_rbManual;
	wxStaticText     *m_stTileWidthLabel;
	wxSpinCtrl       *m_scTileWidth;
	wxStaticText     *m_stSavePathLabel;
	wxDirPickerCtrl  *m_dpFilePath;
	wxButton         *m_bOD_DesignMode;
	wxButton         *m_bOD_GetSceneObject;
	wxStaticText     *m_stObstacleLabel;
	wxListCtrl       *m_lcObstacles;
	wxButton         *m_bCreate;
	wxButton         *m_bCancel;
	wxStaticText     *m_stObstacleTag;
	wxTextCtrl       *m_tcObstacleTag;

    /*wxRadioBox *m_rbSizeType;
	wxSpinCtrl *m_scTileWidth;
    wxChoice   *m_cTerrains;
    wxButton   *m_bOk;
	wxButton   *m_bCancel;*/
};
END_NAMESPACE
#endif