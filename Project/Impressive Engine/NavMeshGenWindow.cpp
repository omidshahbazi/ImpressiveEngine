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

#include "NavMeshGenWindow.h"
#include "ScenesHolder.h"
#include "Scene.h"
#include "TerrainGameObject.h"
#include "GameMap.h"
#include "NavigationGraph.h"
#include "AIEngine.h"
#include "AISellerObject.h"
#include "AIObjectType.h"
#include "Utility.h"



BEGIN_NAMESPACE
NavMeshGenWindow::NavMeshGenWindow(void) :
	WindowBase("Navigation Mesh Generator", wxSize(450, 280))
{
    /*this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer *bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );
    
	m_cTerrains = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_cTerrains->SetSelection( 0 );
	bSizer->Add( m_cTerrains, 0, wxALL, 5 );
	
	wxString m_radioBoxChoices[] = { wxT("Get from terrain size"), wxT("Tile Width") };
	int m_radioBoxNChoices = sizeof( m_radioBoxChoices ) / sizeof( wxString );
	m_rbSizeType = new wxRadioBox( this, wxID_ANY, wxT("Size Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxNChoices, m_radioBoxChoices, 4, wxRA_SPECIFY_COLS|wxRA_SPECIFY_ROWS|wxRA_USE_CHECKBOX );
	m_rbSizeType->SetSelection( 0 );
    m_rbSizeType->Connect(wxEVT_COMMAND_RADIOBOX_SELECTED,wxCommandEventHandler(NavMeshGenWindow::OnRadioBox));
	bSizer->Add( m_rbSizeType, 0, wxALL, 10 );
	
	m_scTileWidth = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 50, 5 );
    //m_scTileWidth->Disable();
	bSizer->Add( m_scTileWidth, 0, wxALL, 5 );
	
    wxGridSizer* gSizer;
	gSizer = new wxGridSizer( 0, 2, 0, 0 );
	
	m_bOk = new wxButton( this, wxID_ANY, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bOk->SetDefault(); 
	gSizer->Add( m_bOk, 0, wxALL, 5 );
	
	m_bCancel = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer->Add( m_bCancel, 0, wxALL, 5 );

    bSizer->Add( gSizer, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer );
	this->Layout();
	
	this->Centre( wxBOTH );

	std::list<TerrainGameObject*> terrains = ScenesHolder::GetInstance().GetCurrentScene()->GetTerrains();

	BEGIN_LIST_ITERATOR(terrains, TerrainGameObject*)
		m_cTerrains->Append(current->GetName().c_str(), current);
	END_LIST_ITERATOR(NULL)


    m_bCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NavMeshGenWindow::bCancelOnButtonClick ), NULL, this );
    m_bOk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NavMeshGenWindow::bOkOnButtonClick ), NULL, this );*/

    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer_main;
	bSizer_main = new wxBoxSizer( wxVERTICAL );
	
	bSizer_main->SetMinSize( wxSize( 450,280 ) ); 
	wxBoxSizer* bSizer_TopSection;
	bSizer_TopSection = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer_TopSection->SetMinSize( wxSize( 380,230 ) ); 
	wxBoxSizer* bSizer_LeftSection;
	bSizer_LeftSection = new wxBoxSizer( wxVERTICAL );
	
	bSizer_LeftSection->SetMinSize( wxSize( 200,220 ) ); 
	wxBoxSizer* bSizer_ComboSection;
	bSizer_ComboSection = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer_ComboSection->SetMinSize( wxSize( 200,30 ) ); 
	m_stTerrainLabel = new wxStaticText( this, wxID_ANY, wxT("Select a terrain:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTerrainLabel->Wrap( -1 );
	bSizer_ComboSection->Add( m_stTerrainLabel, 0, wxALL, 5 );
	
	wxArrayString m_cTerrainsChoices;
	m_cTerrains = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cTerrainsChoices, 0 );
	m_cTerrains->SetSelection( 0 );
	bSizer_ComboSection->Add( m_cTerrains, 0, wxALL, 5 );
	
	bSizer_LeftSection->Add( bSizer_ComboSection, 0, wxALIGN_CENTER_HORIZONTAL, 0 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Size Type") ), wxVERTICAL );
	
	sbSizer3->SetMinSize( wxSize( 200,130 ) ); 
	m_rbTerrain = new wxRadioButton( this, wxID_ANY, wxT("Get from terrain size"), wxDefaultPosition, wxSize( 115,15 ), 0 );
    m_rbTerrain->SetValue(true);
	
	sbSizer3->Add( m_rbTerrain, 0, wxALL, 10 );
	
	m_rbManual = new wxRadioButton( this, wxID_ANY, wxT("Manual"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer3->Add( m_rbManual, 0, wxALL, 10 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer17->SetMinSize( wxSize( 200,30 ) ); 
	m_stTileWidthLabel = new wxStaticText( this, wxID_ANY, wxT("Tile Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTileWidthLabel->Wrap( -1 );
	bSizer17->Add( m_stTileWidthLabel, 0, wxALL, 5 );
	
	m_scTileWidth = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000000, 0 );
    m_scTileWidth->Disable();
	bSizer17->Add( m_scTileWidth, 0, wxALL, 5 );
	
	sbSizer3->Add( bSizer17, 0, 0, 0 );
	
	bSizer_LeftSection->Add( sbSizer3, 0, wxALIGN_CENTER_HORIZONTAL, 0 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	bSizer18->SetMinSize( wxSize( 200,60 ) ); 
	m_stSavePathLabel = new wxStaticText( this, wxID_ANY, wxT("Save the navigation mesh to the directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stSavePathLabel->Wrap( -1 );
	bSizer18->Add( m_stSavePathLabel, 0, wxALL, 5 );
	
	m_dpFilePath = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	bSizer18->Add( m_dpFilePath, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	bSizer_LeftSection->Add( bSizer18, 0, wxALIGN_CENTER_HORIZONTAL, 0 );
	
	bSizer_TopSection->Add( bSizer_LeftSection, 0, 0, 0 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Obstacle detection") ), wxVERTICAL );
	
	sbSizer4->SetMinSize( wxSize( 170,220 ) ); 
	m_bOD_DesignMode = new wxButton( this, wxID_ANY, wxT("Specify in design mode ..."), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( m_bOD_DesignMode, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_bOD_GetSceneObject = new wxButton( this, wxID_ANY, wxT("Get from scene objects ..."), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( m_bOD_GetSceneObject, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_stObstacleLabel = new wxStaticText( this, wxID_ANY, wxT("Obstacles:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stObstacleLabel->Wrap( -1 );
	sbSizer4->Add( m_stObstacleLabel, 0, wxALL, 7 );
	
	m_lcObstacles = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON );
	sbSizer4->Add( m_lcObstacles, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	bSizer_TopSection->Add( sbSizer4, 0, 0, 0 );
	
	bSizer_main->Add( bSizer_TopSection, 0, wxALIGN_CENTER_HORIZONTAL, 0 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer20->SetMinSize( wxSize( 200,50 ) ); 
	m_bCreate = new wxButton( this, wxID_ANY, wxT("Create"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bCreate->SetDefault(); 
	bSizer20->Add( m_bCreate, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	m_bCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_bCancel, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 0 );
	
	bSizer_main->Add( bSizer20, 0, wxRIGHT|wxALIGN_CENTER_HORIZONTAL, 3 );
	
	this->SetSizer( bSizer_main );
	this->Layout();
	bSizer_main->Fit( this );
	
	this->Centre( wxBOTH );

    std::list<TerrainGameObject*> terrains = ScenesHolder::GetInstance().GetCurrentScene()->GetTerrains();

	BEGIN_LIST_ITERATOR(terrains, TerrainGameObject*)
		m_cTerrains->Append(current->GetName().c_str(), current);
	END_LIST_ITERATOR(NULL)

    m_bCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NavMeshGenWindow::bCancelOnButtonClick ), NULL, this );
    m_bCreate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NavMeshGenWindow::bCreateOnButtonClick ), NULL, this );
    m_rbManual->Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( NavMeshGenWindow::rbManualonRadioButton ), NULL, this);
    m_rbTerrain->Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( NavMeshGenWindow::rbTerrainonRadioButton ), NULL, this);
}


NavMeshGenWindow::~NavMeshGenWindow(void)
{

}


void NavMeshGenWindow::bCancelOnButtonClick( wxCommandEvent& event )
{
	Close(true);
}


void NavMeshGenWindow::bCreateOnButtonClick( wxCommandEvent& event )
{
	if (m_cTerrains->GetSelection() < 0) return;

	TerrainGameObject* terrain = (TerrainGameObject*)m_cTerrains->GetClientData(m_cTerrains->GetSelection());

	unsigned int worldSize = terrain->_GetTerrain()->getWorldSize();

	if(m_rbManual->GetValue())
	{
		unsigned int tileSize =  m_scTileWidth->GetValue();

		//GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->SetSize((worldSize / tileSize) + 1);
	}
	else
	{
		//GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->SetSize(terrain->_GetTerrain()->getSize());
	}

	//GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->SetWorldSize(terrain->_GetTerrain()->getWorldSize());

	GameMap::GetInstance().RemoveObstacles();
	std::list<GameObject*> Obstacles;
	ScenesHolder::GetInstance().GetCurrentScene()->GetGameObjectsByName("Obstacle", Obstacles);
	GameMap::GetInstance().SetObstacles(Obstacles);

	std::vector<AISellerObject*> m_Sellers;
	for(unsigned int i = 0; i < m_Sellers.size(); i++)
	{
		GameMap::GetInstance().AddObstacle(m_Sellers[i]->GetGameObject());
	}

	GameMap::GetInstance().SetTerrain(terrain);
	GameMap::GetInstance().GetNavGraph()->CreateGraph();
	//GameMap::GetInstance().SaveMap();

	//GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->ShowGraph();

	Close(true);
}


void NavMeshGenWindow::rbManualonRadioButton( wxCommandEvent& event )
{
   m_scTileWidth->Enable();
}

void NavMeshGenWindow::rbTerrainonRadioButton( wxCommandEvent& event )
{
   m_scTileWidth->Disable();   
}
END_NAMESPACE
#endif