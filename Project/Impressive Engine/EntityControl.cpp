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


#include "EntityControl.h"
#include "TerrainWindow.h"
#include "Entity.h"
#include "Utility.h"
#include "Properties.h"
#include "Engine.h"
#include "IO.h"
#include "ScriptControl.h"
#include "ScenesHolder.h"
#include "TerrainGameObject.h"
#include "ParticleGameObject.h"
#include "AnimationControl.h"
#include "MaterialEditorControl.h"
#include "ModelEditorControl.h"
#include "Log.h"


BEGIN_NAMESPACE
BEGIN_EVENT_TABLE(EntityControl, wxWindow)
	EVT_SIZE(EntityControl::OnSize)
END_EVENT_TABLE()


EntityControl::EntityControl(wxWindow *Parent) :
	wxWindow(Parent, -1)
{
	wxBoxSizer *fgs = new wxBoxSizer(wxVERTICAL);
	//fgs->SetFlexibleDirection(wxBOTH);
	//fgs->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_ToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_FLAT);
	m_ToolBar->AddTool(IDB_REFRESH, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Refresh.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Refresh");
	m_ToolBar->AddSeparator();
	m_OpenExternalTool = m_ToolBar->AddTool(IDB_OPEN_EXTERNALLY, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "External.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Open Externally");
	m_OpenAnimationTool = m_ToolBar->AddTool(IDB_OPEN_ANIMATION, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Animation.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Open in Animation Editor");
	m_OpenMaterialTool = m_ToolBar->AddTool(IDB_OPEN_MATERIAL, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Material.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Open in Material Editor");
	m_OpenModelTool = m_ToolBar->AddTool(IDB_OPEN_MODEL, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Objects.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Open in Model Editor");
	m_OpenScriptTool = m_ToolBar->AddTool(IDB_OPEN_SCRIPT, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Internal.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Open in Script Editor");
	m_OpenExternalTool->Enable(false);
	m_OpenAnimationTool->Enable(false);
	m_OpenMaterialTool->Enable(false);
	m_OpenModelTool->Enable(false);
	m_OpenScriptTool->Enable(false);
	m_ToolBar->AddSeparator();
	m_UseTool = m_ToolBar->AddTool(IDB_USE, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "New.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Use On Selected Game Object");
	m_UseTool->Enable(false);
	m_ToolBar->Realize();
	fgs->Add(m_ToolBar, 0, wxALL);

	m_Tree = new wxTreeCtrl(this, wxID_ANY);
	fgs->Add(m_Tree, 0, wxALL);

	m_TextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(10, 150), wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY);
	m_TextBox->Hide();
	
	SetSizer(fgs);
	Layout();
	
	m_Tree->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler(EntityControl::OnSelectedItem), NULL, this);

	Connect(IDB_REFRESH, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EntityControl::OnRefreshButton));
	Connect(IDB_OPEN_EXTERNALLY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EntityControl::OnOpenExternallyButton));
	Connect(IDB_OPEN_ANIMATION, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EntityControl::OnOpenAnimationButton));
	Connect(IDB_OPEN_MATERIAL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EntityControl::OnOpenMaterialButton));
	Connect(IDB_OPEN_MODEL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EntityControl::OnOpenModelButton));
	Connect(IDB_OPEN_SCRIPT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EntityControl::OnOpenScriptButton));
	Connect(IDB_USE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EntityControl::OnUseButton));
}

//EntityControl::~EntityControl(void)
//{
//}


void EntityControl::ReloadContent(void)
{
	m_Tree->DeleteAllItems();

	m_RootID = m_Tree->AddRoot("Resources");
	m_Prefabs = m_Tree->AppendItem(m_RootID, "Prefab");
	m_MeshesID = m_Tree->AppendItem(m_RootID, "Meshes");
	//m_AnimationID = m_Tree->AppendItem(m_RootID, "Animation");
	m_TextureID = m_Tree->AppendItem(m_RootID, "Texture");
	m_MaterialID = m_Tree->AppendItem(m_RootID, "Material");
	m_ScriptID = m_Tree->AppendItem(m_RootID, "Script");

	m_Tree->AppendItem(m_Prefabs, "Light");
	m_Tree->AppendItem(m_Prefabs, "Camera");
	m_Tree->AppendItem(m_Prefabs, "Terrain");
	m_Tree->AppendItem(m_Prefabs, "Ocean");
	m_Tree->AppendItem(m_Prefabs, "Particle");

	//
	// Adding Materials
	//
	//AddFolderContent(Engine::PATH_MATERIAL.c_str(), m_MaterialID);
	Ogre::ResourceManager::ResourceMapIterator it = Ogre::MaterialManager::getSingleton().getResourceIterator();
	while (it.hasMoreElements())
	{
		Ogre::MaterialPtr material = it.getNext();
		
		if (material->getGroup() == Engine::RESOURCES_MATERIAL)
			m_Tree->AppendItem(m_MaterialID, material->getName());
	}

	//
	// Adding Textures
	//
	AddFolderContent(Engine::PATH_TEXTURE.c_str(), m_TextureID);

	//
	// Adding Meshes
	//
	AddFolderContent(Engine::PATH_MODEL.c_str(), m_MeshesID, ".mesh");

	//
	// Adding Animations
	//
	//AddFolderContent(Engine::PATH_MODEL.c_str(), m_AnimationID, ".skeleton");

	//
	// Adding Scripts
	//
	AddFolderContent(Engine::PATH_SCRIPT.c_str(), m_ScriptID);

	m_Tree->Expand(m_RootID);
}


void EntityControl::AddFolderContent(const char *Path, const wxTreeItemId &ID, const char *Filter)
{
	std::vector<Ogre::String> &files = GetFiles(Path);

	for (unsigned int i = 0; i < files.size(); i++)
	{
		if (Filter != NULL && !Ogre::StringUtil::endsWith(files[i], Filter))
			continue;

		m_Tree->AppendItem(ID, wxFileNameFromPath(STRING_OGRE_TO_WX(files[i])));
	}
}


bool EntityControl::IsAnEntity(const wxTreeItemId &ID)
{
	if (ID == m_RootID ||
		ID == m_MeshesID ||
		ID == m_TextureID ||
		ID == m_MaterialID ||
		ID == m_ScriptID)
		return false;

	return true;
}


wxString EntityControl::GetTypeAsString(const wxTreeItemId &ID)
{
	const wxTreeItemId &parent = m_Tree->GetItemParent(ID);

	if (parent == m_MeshesID)
		return "Mesh";
	else if (parent == m_TextureID)
		return "Texture";
	//else if (parent == m_MaterialID)
	//	return "Material";
	else if (parent == m_ScriptID)
		return "Script";

	return "Unknown";
}


void EntityControl::OnSize(wxSizeEvent &event)
{
	int y = 0;

	m_ToolBar->SetSize(event.GetSize().x, y);
	y += m_ToolBar->GetSize().y + 3;

	int h = event.GetSize().y - y;

	if (m_TextBox->IsShown())
	{
		h -= m_TextBox->GetSize().y;
	}

	m_Tree->SetSize(event.GetSize().x, h);
	y += h;

	if (m_TextBox->IsShown())
	{
		m_TextBox->SetSize(0, y, event.GetSize().x, m_TextBox->GetSize().y);
	}
}


void EntityControl::OnSelectedItem(wxTreeEvent &event)
{
	const wxTreeItemId &id = m_Tree->GetSelection();
		
	m_TextBox->Hide();
	m_OpenExternalTool->Enable(false);
	m_OpenAnimationTool->Enable(false);
	m_OpenMaterialTool->Enable(false);
	m_OpenModelTool->Enable(false);
	m_OpenScriptTool->Enable(false);
	m_UseTool->Enable(false);

	if (IsAnEntity(id))
	{
		Properties &prop = Properties::GetInstance();
		prop.BeginNewProperties(Entity::GetInstance().GetCaption());
	
		prop.AddCategory("Entity");

		prop.AddPropertyOfString("Name", m_Tree->GetItemText(id), "Name of entity", true);
		prop.AddPropertyOfString("Type", GetTypeAsString(id), "Type of entity", true);

		prop.EndNewProperties();

		const wxTreeItemId &parent = m_Tree->GetItemParent(id);

		if (parent == m_Prefabs)
		{
			m_UseTool->Enable(true);
		}
		else if (parent == m_MeshesID)
		{
			m_UseTool->Enable(true);

			m_OpenAnimationTool->Enable(true);
			m_OpenModelTool->Enable(true);
		}
		//else if (parent == m_AnimationID)
		//{
		//	m_UseTool->Enable(true);
		//	m_OpenTool->Enable(true);
		//}
		else if (parent == m_TextureID)
		{
			m_OpenExternalTool->Enable(true);
		}
		else if (parent == m_MaterialID)
		{
			m_OpenMaterialTool->Enable(true);
		}
		else if (parent == m_ScriptID)
		{
			//m_TextBox->Show();

			//m_TextBox->SetValue(IO::GetInstance().ReadScript((Engine::PATH_SCRIPT + m_Tree->GetItemText(id))));

			m_OpenScriptTool->Enable(true);
		}
	}
		
	m_ToolBar->Realize();
	OnSize(wxSizeEvent(GetSize(), GetId()));
}


void EntityControl::OnRefreshButton(wxCommandEvent &event)
{
	ReloadContent();
}


void EntityControl::OnOpenExternallyButton(wxCommandEvent &event)
{
	wxSystem("rundll32.exe shimgvw.dll,ImageView_Fullscreen " + Engine::PATH_TEXTURE + m_Tree->GetItemText(m_Tree->GetSelection()));
}


void EntityControl::OnOpenAnimationButton(wxCommandEvent &event)
{
	AnimationControl::GetPointer()->SelectMesh(m_Tree->GetItemText(m_Tree->GetSelection()));
}


void EntityControl::OnOpenMaterialButton(wxCommandEvent &event)
{
	MaterialEditorControl::GetPointer()->SelectMaterial(m_Tree->GetItemText(m_Tree->GetSelection()));
}


void EntityControl::OnOpenModelButton(wxCommandEvent &event)
{
	ModelEditorControl::GetPointer()->SelectMesh(m_Tree->GetItemText(m_Tree->GetSelection()));
}


void EntityControl::OnOpenScriptButton(wxCommandEvent &event)
{
	ScriptControl::GetPointer()->Open(Engine::PATH_SCRIPT + m_Tree->GetItemText(m_Tree->GetSelection()));
}


void EntityControl::OnUseButton(wxCommandEvent &event)
{
	const wxTreeItemId &id = m_Tree->GetSelection();
	const wxTreeItemId &parent = m_Tree->GetItemParent(id);
	
	GameObject *obj = ScenesHolder::GetInstance().GetCurrentScene()->GetSelectedObject();

	if (parent == m_Prefabs)
	{
		const wxString &text = m_Tree->GetItemText(id);

		if (text == "Camera")
			ScenesHolder::GetInstance().GetCurrentScene()->CreateCameraGameObject("CameraGameObject", obj);
		else if (text == "Light")
			ScenesHolder::GetInstance().GetCurrentScene()->CreateLightGameObject("LightGameObject", obj);
		else if (text == "Terrain")
		{
			TerrainWindow *tw = new TerrainWindow;
			tw->ShowModal();
		}
		else if (text == "Ocean")
			ScenesHolder::GetInstance().GetCurrentScene()->CreateOceanGameObject("OceanGameObject", obj);
		else if (text == "Particle")
			ScenesHolder::GetInstance().GetCurrentScene()->CreateParticleGameObject("ParticleGameObject", obj);
	}
	else if (parent == m_MeshesID)
	{
		ScenesHolder::GetInstance().GetCurrentScene()->CreateMeshGameObject("MeshGameObject", STRING_WX_TO_OGRE(m_Tree->GetItemText(id)), obj);
	}

	ScenesHolder::GetInstance().GetCurrentScene()->RefreshSceneControlContent();
}
END_NAMESPACE

#endif