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

#include "TerrainWindow.h"
#include "MainWindow.h"
#include "ScenesHolder.h"
#include "TerrainGameObject.h"
#include "Utility.h"


BEGIN_NAMESPACE
TerrainWindow::TerrainWindow(void) :
	WindowBase("Terrain", wxSize(463, 343))
{
	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 0, 2, 0, 0 );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 0, 2, 0, 0 );
	
	wxStaticText* text = new wxStaticText( this, wxID_ANY, wxT("Terrain Name :"), wxDefaultPosition, wxDefaultSize, 0 );
	text->Wrap( -1 );
	gSizer1->Add( text, 0, wxALL, 5 );
	
	m_tbName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_tbName, 0, wxALL, 5 );
	
	text = new wxStaticText( this, wxID_ANY, wxT("Height Map :"), wxDefaultPosition, wxDefaultSize, 0 );
	text->Wrap( -1 );
	gSizer1->Add( text, 0, wxALL, 5 );
	
	m_tbHeightMap = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_tbHeightMap, 0, wxALL, 5 );
	
	text = new wxStaticText( this, wxID_ANY, wxT("Height (float) :"), wxDefaultPosition, wxDefaultSize, 0 );
	text->Wrap( -1 );
	gSizer1->Add( text, 0, wxALL, 5 );
	
	m_tbHeight = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_tbHeight, 0, wxALL, 5 );
	
	text = new wxStaticText( this, wxID_ANY, wxT("Size :"), wxDefaultPosition, wxDefaultSize, 0 );
	text->Wrap( -1 );
	gSizer1->Add( text, 0, wxALL, 5 );
	
	wxString m_cSizeChoices[] = { wxT("128 * 128"), wxT("256 * 256"), wxT("512 * 512"), wxT("1024 * 1024"), wxT("2048 * 2048"), wxT("4096 * 4096") };
	int m_cSizeNChoices = sizeof( m_cSizeChoices ) / sizeof( wxString );
	m_cSize = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 100,-1 ), m_cSizeNChoices, m_cSizeChoices, 0 );
	m_cSize->SetSelection(0);
	gSizer1->Add( m_cSize, 0, wxALL, 5 );
	
	text = new wxStaticText( this, wxID_ANY, wxT("World Size (float) :"), wxDefaultPosition, wxDefaultSize, 0 );
	text->Wrap( -1 );
	gSizer1->Add( text, 0, wxALL, 5 );
	
	m_tbWorldSize = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_tbWorldSize, 0, wxALL, 5 );
	
	gSizer3->Add( gSizer1, 1, 0, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer4;
	gSizer4 = new wxGridSizer( 0, 2, 0, 0 );
	
	text= new wxStaticText( this, wxID_ANY, wxT("Texture Name :"), wxDefaultPosition, wxDefaultSize, 0 );
	text->Wrap( -1 );
	gSizer4->Add( text, 0, wxALL, 5 );
	
	m_tbTextureName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer4->Add( m_tbTextureName, 0, wxALL, 5 );
	
	text = new wxStaticText( this, wxID_ANY, wxT("Texture Size (float) :"), wxDefaultPosition, wxDefaultSize, 0 );
	text->Wrap( -1 );
	gSizer4->Add( text, 0, wxALL, 5 );
	
	m_tbTextureSize = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer4->Add( m_tbTextureSize, 0, wxALL, 5 );
	
	m_bAdd = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer4->Add( m_bAdd, 0, wxALL, 5 );
	
	m_bDelete = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer4->Add( m_bDelete, 0, wxALL, 5 );
	
	bSizer4->Add( gSizer4, 1, wxEXPAND, 5 );
	
	m_lbTextures = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( 200,200 ), 0, NULL, 0 ); 
	bSizer4->Add( m_lbTextures, 0, wxALL, 5 );
	
	gSizer3->Add( bSizer4, 1, 0, 5 );
	
	wxGridSizer* gSizer5;
	gSizer5 = new wxGridSizer( 0, 2, 0, 0 );
	
	m_bCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer5->Add( m_bCancel, 0, wxALL, 5 );
	
	m_bCreate = new wxButton( this, wxID_ANY, wxT("Create"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer5->Add( m_bCreate, 0, wxALL, 5 );
	
	gSizer3->Add( gSizer5, 1, 0, 5 );
	
	this->SetSizer( gSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );

	// Connect Events
	m_bAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TerrainWindow::bAddOnButtonClick ), NULL, this );
	m_bDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TerrainWindow::bDeleteOnButtonClick ), NULL, this );
	m_bCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TerrainWindow::bCancelOnButtonClick ), NULL, this );
	m_bCreate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TerrainWindow::bCreateOnButtonClick ), NULL, this );

	
	m_tbName->SetValue("TerrainName");
	m_tbHeightMap->SetValue("terrain21.png");
	m_tbHeight->SetValue("20");
	m_tbWorldSize->SetValue("1000");
	m_tbTextureName->SetValue("terrain_texture.jpg");
}


TerrainWindow::~TerrainWindow(void)
{
}


void TerrainWindow::bAddOnButtonClick( wxCommandEvent& event )
{
	Ogre::Terrain::LayerInstance *layer = new Ogre::Terrain::LayerInstance();
	layer->textureNames.push_back(STRING_WX_TO_OGRE(m_tbTextureName->GetValue()));

	layer->worldSize = Ogre::StringConverter::parseReal(STRING_WX_TO_OGRE(m_tbTextureSize->GetValue()), 0);

	if (layer->worldSize < 1)
		layer->worldSize = Ogre::StringConverter::parseReal(STRING_WX_TO_OGRE(m_tbWorldSize->GetValue()), 0);

	if (layer->worldSize > 0)
	{
		m_lbTextures->Append(m_tbTextureName->GetValue(), layer);
		m_tbTextureName->SetValue("");
		m_tbTextureSize->SetValue("0");
	}
}


void TerrainWindow::bDeleteOnButtonClick( wxCommandEvent& event )
{
	if (m_lbTextures->GetSelection() > -1)
		m_lbTextures->Delete(m_lbTextures->GetSelection());
}


void TerrainWindow::bCancelOnButtonClick( wxCommandEvent& event )
{
	Close(true);
}


void TerrainWindow::bCreateOnButtonClick( wxCommandEvent& event )
{
	float height = Ogre::StringConverter::parseReal(STRING_WX_TO_OGRE(m_tbHeight->GetValue()), 0);

	unsigned int size = 128;
	for (int i = 0; i < m_cSize->GetSelection(); i++)
		size *= 2;
	size++;

	float worldSize = Ogre::StringConverter::parseReal(STRING_WX_TO_OGRE(m_tbWorldSize->GetValue()), 0);

	Ogre::Terrain::LayerInstanceList layers;
	for (unsigned int i = 0; i < m_lbTextures->GetCount(); i++)
		layers.push_back(*(Ogre::Terrain::LayerInstance*)m_lbTextures->GetClientData(i));

	GameObject *parent = ScenesHolder::GetInstance().GetCurrentScene()->GetSelectedObject();

	TerrainGameObject *tgo = ScenesHolder::GetInstance().GetCurrentScene()->CreateTerrainGameObject(STRING_WX_TO_OGRE(m_tbName->GetValue()), parent);
	tgo->Prepare(STRING_WX_TO_OGRE(m_tbHeightMap->GetValue()), height, size, worldSize, &layers);
}
END_NAMESPACE

#endif