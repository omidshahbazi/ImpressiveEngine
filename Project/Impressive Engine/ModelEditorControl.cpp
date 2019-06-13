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


#include "ModelEditorControl.h"
#include "Engine.h"
#include "EditorCamera.h"
#include "RenderControl.h"
#include "MainWindow.h"
#include "Scene.h"
#include "Properties.h"
#include "Utility.h"


BEGIN_NAMESPACE
BEGIN_EVENT_TABLE(ModelEditorControl, wxWindow)
	EVT_SIZE(ModelEditorControl::OnSize)
END_EVENT_TABLE()


ModelEditorControl *ModelEditorControl::m_ThisPointer = 0;

Ogre::String RESOURCES_GROUP(Engine::RESOURCES_MATERIAL + Ogre::String("_Temp"));


ModelEditorControl::ModelEditorControl(wxWindow *Parent) :
	wxWindow(Parent, wxID_ANY),
	m_ToolBar(NULL),
	m_SubMeshList(NULL),
	m_SceneManager(NULL),
	m_SceneNode(NULL),
	m_RenderControl(NULL),
	m_LastSelectedSubMesh(NULL)
{
}


//ModelEditorControl::~ModelEditorControl(void)
//{
//}


void ModelEditorControl::Initialize(void)
{
	wxBoxSizer *bs = new wxBoxSizer(wxVERTICAL);
	
	m_ToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_FLAT);
	m_ToolBar->AddTool(IDB_SAVE, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "SaveSmall.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Save");
	m_ToolBar->Realize();
	Connect(IDB_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(ModelEditorControl::OnSave));
	bs->Add(m_ToolBar, 0, wxEXPAND, 5);

	m_SubMeshList = new wxChoice(this, wxID_ANY);
	m_SubMeshList->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(ModelEditorControl::OnSubMeshSelected), NULL, this);
	bs->Add(m_SubMeshList, 0, wxEXPAND, 5);

	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(RESOURCES_GROUP);

	m_SceneManager = Engine::GetInstance().GetGraphic()->createSceneManager(Ogre::ST_GENERIC, "ModelEditorSceneManager");
	m_SceneManager->setAmbientLight(Ogre::ColourValue::Black);

	Ogre::Light *light = NULL;

	//light = m_SceneManager->createLight("DirectionalX1");
	//light->setType(Ogre::Light::LT_DIRECTIONAL);
	//light->setDirection(Ogre::Vector3::UNIT_X);

	light = m_SceneManager->createLight("DirectionalX2");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3::NEGATIVE_UNIT_X);

	//light = m_SceneManager->createLight("DirectionalY1");
	//light->setType(Ogre::Light::LT_DIRECTIONAL);
	//light->setDirection(Ogre::Vector3::UNIT_Y);

	light = m_SceneManager->createLight("DirectionalY2");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

	//light = m_SceneManager->createLight("DirectionalZ1");
	//light->setType(Ogre::Light::LT_DIRECTIONAL);
	//light->setDirection(Ogre::Vector3::UNIT_Z);

	light = m_SceneManager->createLight("DirectionalZ2");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);

	m_RenderControl = new RenderControl(this, "ModelEditorViewPort", false, new EditorCamera(EditorCamera::ECT_FLY, m_SceneManager->createCamera("ModelEditorCamera")));
	bs->Add(m_RenderControl, 0, wxEXPAND, 5);
	
	SetSizer(bs);
	Layout();

	Refresh();
}


void ModelEditorControl::Update(void)
{
	if (m_RenderControl)
		m_RenderControl->_Update();
}


void ModelEditorControl::SelectMesh(const wxString &Name)
{
	if (m_SceneNode)
		m_SceneManager->destroySceneNode(m_SceneNode);

	m_SceneNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("SceneNode");

	Ogre::Entity * entity = NULL;
	if (m_SceneManager->hasEntity(STRING_WX_TO_OGRE(Name)))
		entity = m_SceneManager->getEntity(STRING_WX_TO_OGRE(Name));
	if (!entity)
		entity = m_SceneManager->createEntity(STRING_WX_TO_OGRE(Name), STRING_WX_TO_OGRE(Name));

	m_SceneNode->attachObject(entity);

	m_SubMeshList->Clear();

	for (unsigned int i = 0; i < entity->getNumSubEntities(); i++)
		m_SubMeshList->Append(wxString("Sub-Mesh ") + Ogre::StringConverter::toString(i + 1), entity->getSubEntity(i));

	m_RenderControl->GetCamera()->SetPosition(entity->getBoundingBox().getAllCorners()[4] + Ogre::Vector3(100, 100, 100));
	m_RenderControl->GetCamera()->LookAt(entity->getBoundingBox().getCenter());

	entity = NULL;

	((MainWindow*)GetParent())->ShowWindow(GetName());
}

	
void ModelEditorControl::RefreshProperties(void)
{
	Properties &prop = Properties::GetInstance();
	prop.BeginNewProperties("Model Editor", this);
	
	prop.AddCategory("General");
	prop.AddPropertyOfString("Material Name", m_LastSelectedMaterial->getName());

	prop.EndNewProperties();
}


void ModelEditorControl::OnSize(wxSizeEvent &event)
{
	if (!m_SubMeshList)
		return;

	m_SubMeshList->SetSize(GetSize().x, m_SubMeshList->GetSize().y);
	int y = m_SubMeshList->GetSize().y;

	m_ToolBar->SetSize(GetSize().x, m_ToolBar->GetSize().y);
	y += m_ToolBar->GetSize().y;

	m_RenderControl->SetSize(GetSize().x, GetSize().y - y);
}


void ModelEditorControl::OnSubMeshSelected(wxCommandEvent &event)
{
	if (m_LastSelectedSubMesh)
	{
		m_LastSelectedSubMesh->setMaterial(m_LastSelectedMaterial);

		m_LastSelectedSubMesh->getMaterial()->unload();
		Ogre::MaterialManager::getSingleton().remove(m_LastSelectedSubMesh->getMaterial()->getName());
	}

	m_LastSelectedSubMesh = (Ogre::SubEntity*)m_SubMeshList->GetClientData(m_SubMeshList->GetSelection());

	m_LastSelectedMaterial = m_LastSelectedSubMesh->getMaterial();
	
	m_LastSelectedSubMesh->setMaterial(m_LastSelectedMaterial->clone(m_LastSelectedSubMesh->getMaterialName() + "_Temp", true, RESOURCES_GROUP));

	m_LastSelectedSubMesh->getMaterial()->getBestTechnique(0)->getPass(m_LastSelectedSubMesh->getMaterial()->getBestTechnique(0)->getNumPasses() - 1)->setPolygonMode(Ogre::PM_WIREFRAME);

	RefreshProperties();
}


void ModelEditorControl::OnSave(wxCommandEvent &event)
{
	Ogre::Entity *entity = (Ogre::Entity*)m_SceneNode->getAttachedObject(0);

	Ogre::MeshSerializer ms;

	ms.exportMesh(entity->getMesh().getPointer(), Engine::PATH_MODEL + entity->getName() + "1.mesh");

	entity = NULL;
}


void ModelEditorControl::OnPropertyChanged(/*wxPropertyGridManager *Manager, */const wxPropertyGridEvent &event)
{
	const wxString &name = event.GetPropertyName();

	if (name == "Material Name")
	{
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(STRING_WX_TO_OGRE(event.GetPropertyValue().GetString()), Engine::RESOURCES_MATERIAL);

		if (material.isNull())
			RefreshProperties();
		else
		{
			Ogre::MaterialPtr temp = m_LastSelectedSubMesh->getMaterial();

			m_LastSelectedMaterial = material;
			m_LastSelectedSubMesh->setMaterial(m_LastSelectedMaterial->clone(m_LastSelectedMaterial->getName() + "_Temp", true, RESOURCES_GROUP));
			m_LastSelectedSubMesh->getMaterial()->getBestTechnique(0)->getPass(m_LastSelectedSubMesh->getMaterial()->getBestTechnique(0)->getNumPasses() - 1)->setPolygonMode(Ogre::PM_WIREFRAME);
			
			temp->unload();
			Ogre::MaterialManager::getSingleton().remove(temp->getName());
		}
	}
}

END_NAMESPACE

#endif