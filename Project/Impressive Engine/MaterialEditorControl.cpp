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

#include "MaterialEditorControl.h"
#include "Engine.h"
#include "RenderControl.h"
#include "EditorCamera.h"
#include "MainWindow.h"
#include "Properties.h"
#include "Log.h"
#include "Scene.h"
#include "ManualFunctionality.h"
#include "MaterialPageControl.h"
#include "Utility.h"
#include <OgreGpuProgramUsage.h>


BEGIN_NAMESPACE
const char *MaterialEditorControl::PASS_BUMP_NAME("NormalBumpPass");

const char *MaterialEditorControl::PROGRAM_BUMPVP_NAME("NormalBumpVP");
const char *MaterialEditorControl::PROGRAM_BUMPFP_NAME("NormalBumpFP");

const char *MaterialEditorControl::TEXALIAS_DIFFUSE_NAME("Diffuse_Map");
const char *MaterialEditorControl::TEXALIAS_NORMAL_NAME("Normal_Map");
const char *MaterialEditorControl::TEXALIAS_SPECULAR_NAME("Specular_Map");
const char *MaterialEditorControl::TEXALIAS_REFLECTION_NAME("Reflection_Map");

const char *MATERIAL_PARENT_TEMPLATE_IMPORT("%PARENTIMPORT%");
const char *MATERIAL_PARENT_TEMPLATE_TEXALIAS("%TEXALIAS%");


MaterialEditorControl *MaterialEditorControl::m_ThisPointer = 0;


BEGIN_EVENT_TABLE(MaterialEditorControl, wxWindow)
	EVT_SIZE(MaterialEditorControl::OnSize)
END_EVENT_TABLE()


TechniquePropertyListener::TechniquePropertyListener(Ogre::Technique *Selected) :
		m_Selected(Selected)
{
}


void TechniquePropertyListener::OnPropertyChanged(/*wxPropertyGridManager *Manager, */const wxPropertyGridEvent &event)
{
	wxString &name = event.GetPropertyName();

	Ogre::Pass *pass;
	Ogre::TextureUnitState *tex;

	if (name == "Type")
	{
		MaterialEditorControl::MaterialType mt = (MaterialEditorControl::MaterialType)event.GetPropertyValue().GetInteger();
		pass = m_Selected->getPass(0);

		if (mt == MaterialEditorControl::MT_Diffuse)
		{
			ManualFunctionality::MakePassAsDiffuse(pass);
		}
		else if (mt == MaterialEditorControl::MT_BumpDiffuse)
		{
			ManualFunctionality::MakePassAsBumpDiffuse(pass);
		}
		else if (mt == MaterialEditorControl::MT_BumpSpecularDiffuse)
		{
			ManualFunctionality::MakePassAsBumpSpecularDiffuse(pass);
		}

		MaterialEditorControl::GetPointer()->RefreshMaterialProperties();
	}
	else if (Ogre::StringUtil::startsWith(STRING_WX_TO_OGRE(name), MaterialEditorControl::TEXALIAS_DIFFUSE_NAME, false))
	{
		Ogre::vector<Ogre::String>::type temp = Ogre::StringUtil::split(STRING_WX_TO_OGRE(name), "-");
		pass = m_Selected->getPass(Ogre::StringConverter::parseUnsignedInt(temp[1]));
		tex = pass->getTextureUnitState(0);

		tex->setTextureName(STRING_WX_TO_OGRE(event.GetPropertyValue().GetString()), tex->getTextureType());
	}
	else if (Ogre::StringUtil::startsWith(STRING_WX_TO_OGRE(name), MaterialEditorControl::TEXALIAS_NORMAL_NAME, false))
	{
		Ogre::vector<Ogre::String>::type temp = Ogre::StringUtil::split(STRING_WX_TO_OGRE(name), "-");
		pass = m_Selected->getPass(Ogre::StringConverter::parseUnsignedInt(temp[1]));
		tex = pass->getTextureUnitState(1);

		tex->setTextureName(STRING_WX_TO_OGRE(event.GetPropertyValue().GetString()), tex->getTextureType());
	}
	else if (Ogre::StringUtil::startsWith(STRING_WX_TO_OGRE(name), MaterialEditorControl::TEXALIAS_SPECULAR_NAME, false))
	{
		Ogre::vector<Ogre::String>::type temp = Ogre::StringUtil::split(STRING_WX_TO_OGRE(name), "-");
		pass = m_Selected->getPass(Ogre::StringConverter::parseUnsignedInt(temp[1]));
		tex = pass->getTextureUnitState(2);

		tex->setTextureName(STRING_WX_TO_OGRE(event.GetPropertyValue().GetString()), tex->getTextureType());
	}
	
	tex = NULL;
	pass = NULL;

	ManualFunctionality::ClearPassMaps();
	//MaterialEditorControl::GetPointer()->_GetRenderControl()->SetFocus();
	MaterialEditorControl::GetPointer()->RefreshScript();
}


PassPropertyListener::PassPropertyListener(Ogre::Pass *Selected) :
		m_Selected(Selected)
{
}


void PassPropertyListener::OnPropertyChanged(/*wxPropertyGridManager *Manager, */const wxPropertyGridEvent &event)
{
	wxString &name = event.GetPropertyName();

	if (name == "Ambient")
	{
		m_Selected->setAmbient(Properties::GetColorFromValue(event.GetPropertyValue()));
	}
	else if (name == "Diffuse")
	{
		m_Selected->setDiffuse(Properties::GetColorFromValue(event.GetPropertyValue()));
	}
	else if (name == "Specular")
	{
		m_Selected->setSpecular(Properties::GetColorFromValue(event.GetPropertyValue()));
	}
	else if (name == "Self Illumination (Emissive)")
	{
		m_Selected->setSelfIllumination(Properties::GetColorFromValue(event.GetPropertyValue()));
	}
	else if (name == "Shininess")
	{
		double value = event.GetPropertyValue().GetDouble();
		if (value < 0)
			value = 0;
		else if (value > 128)
			value = 128;

		m_Selected->setShininess(value);
	}
	else if (name == "Vertex Color Tracking")
	{
		m_Selected->setVertexColourTracking((Ogre::TrackVertexColourType)event.GetPropertyValue().GetInteger());
	}
	else if (name == "Source Blend Factor")
	{
		m_Selected->setSeparateSceneBlending(
			(Ogre::SceneBlendFactor)event.GetPropertyValue().GetInteger(),
			m_Selected->getDestBlendFactor(),
			m_Selected->getSourceBlendFactorAlpha(),
			m_Selected->getDestBlendFactorAlpha());
	}
	else if (name == "Destination Blend Factor")
	{
		m_Selected->setSeparateSceneBlending(
			m_Selected->getSourceBlendFactor(),
			(Ogre::SceneBlendFactor)event.GetPropertyValue().GetInteger(),
			m_Selected->getSourceBlendFactorAlpha(),
			m_Selected->getDestBlendFactorAlpha());
	}
	else if (name == "Source Blend Factor Alpha")
	{
		m_Selected->setSeparateSceneBlending(
			m_Selected->getSourceBlendFactor(),
			m_Selected->getDestBlendFactor(),
			(Ogre::SceneBlendFactor)event.GetPropertyValue().GetInteger(),
			m_Selected->getDestBlendFactorAlpha());
	}
	else if (name == "Destination Blend Factor Alpha")
	{
		m_Selected->setSeparateSceneBlending(
			m_Selected->getSourceBlendFactor(),
			m_Selected->getDestBlendFactor(),
			m_Selected->getSourceBlendFactorAlpha(),
			(Ogre::SceneBlendFactor)event.GetPropertyValue().GetInteger());
	}
	else if (name == "Scene Blending Operation")
	{
		m_Selected->setSeparateSceneBlendingOperation(
			(Ogre::SceneBlendOperation)event.GetPropertyValue().GetInteger(), 
			m_Selected->getSceneBlendingOperationAlpha());
	}
	else if (name == "Scene Blending Operation Alpha")
	{
		m_Selected->setSeparateSceneBlendingOperation(
			m_Selected->getSceneBlendingOperation(),
			(Ogre::SceneBlendOperation)event.GetPropertyValue().GetInteger());
	}
	else if (name == "Depth Check Enabled")
	{
		m_Selected->setDepthCheckEnabled(event.GetPropertyValue().GetBool());
	}
	else if (name == "Depth Write Enabled")
	{
		m_Selected->setDepthWriteEnabled(event.GetPropertyValue().GetBool());
	}
	else if (name == "Depth Function")
	{
		m_Selected->setDepthFunction((Ogre::CompareFunction)event.GetPropertyValue().GetInteger());
	}
	else if (name == "Depth Bias Constant")
	{
		m_Selected->setDepthBias(event.GetPropertyValue().GetDouble(), m_Selected->getDepthBiasSlopeScale());
	}
	else if (name == "Depth Bias Slope Scale")
	{
		m_Selected->setDepthBias(m_Selected->getDepthBiasConstant(), event.GetPropertyValue().GetDouble());
	}
	else if (name == "Iteration Depth Bias")
	{
		m_Selected->setIterationDepthBias(event.GetPropertyValue().GetDouble());
	}
	else if (name == "Color Write Enabled")
	{
		m_Selected->setColourWriteEnabled(event.GetPropertyValue().GetBool());
	}
	else if (name == "Alpha Reject Function")
	{
		m_Selected->setAlphaRejectFunction((Ogre::CompareFunction)event.GetPropertyValue().GetInteger());
	}
	else if (name == "Alpha Reject Value")
	{
		int value = event.GetPropertyValue().GetInteger();
		if (value < 0)
			value = 0;
		else if (value > 255)
			value = 255;

		m_Selected->setAlphaRejectValue(value);
	}
	else if (name == "Alpha To Coverage Enabled")
	{
		m_Selected->setAlphaToCoverageEnabled(event.GetPropertyValue().GetBool());
	}
	else if (name == "Transparent Sorting Enabled")
	{
		m_Selected->setTransparentSortingEnabled(event.GetPropertyValue().GetBool());
	}
	else if (name == "Transparent Sorting Forced")
	{
		m_Selected->setTransparentSortingForced(event.GetPropertyValue().GetBool());
	}
	else if (name == "Lighting Enabled")
	{
		m_Selected->setLightingEnabled(event.GetPropertyValue().GetBool());
	}
	else if (name == "Maximum Simultaneous Lights")
	{
		m_Selected->setMaxSimultaneousLights(event.GetPropertyValue().GetInteger());
	}
	else if (name == "Iterate Per Light")
	{
		m_Selected->setIteratePerLight(
			event.GetPropertyValue().GetBool(),
			m_Selected->getRunOnlyForOneLightType(),
			m_Selected->getOnlyLightType());
	}
	else if (name == "Light Count Per Iteration")
	{
		m_Selected->setLightCountPerIteration(event.GetPropertyValue().GetInteger());
	}
	else if (name == "Run Only For One Light Type")
	{
		m_Selected->setIteratePerLight(
			m_Selected->getIteratePerLight(),
			event.GetPropertyValue().GetBool(),
			m_Selected->getOnlyLightType());
	}
	else if (name == "Only Light Type")
	{
		m_Selected->setIteratePerLight(
			m_Selected->getIteratePerLight(),
			m_Selected->getRunOnlyForOneLightType(),
			(Ogre::Light::LightTypes)event.GetPropertyValue().GetInteger());
	}
	else if (name == "Shading Mode")
	{
		m_Selected->setShadingMode((Ogre::ShadeOptions)event.GetPropertyValue().GetInteger());
	}
	else if (name == "Polygon Mode")
	{
		m_Selected->setPolygonMode((Ogre::PolygonMode)event.GetPropertyValue().GetInteger());
	}
	else if (name == "Normalise Normals")
	{
		m_Selected->setNormaliseNormals(event.GetPropertyValue().GetBool());
	}
	else if (name == "Polygon Mode Overrideable")
	{
		m_Selected->setPolygonModeOverrideable(event.GetPropertyValue().GetBool());
	}
	else if (name == "Culling Mode")
	{
		m_Selected->setCullingMode((Ogre::CullingMode)event.GetPropertyValue().GetInteger());
	}
	else if (name == "Manual Culling Mode")
	{
		m_Selected->setManualCullingMode((Ogre::ManualCullingMode)event.GetPropertyValue().GetInteger());
	}
	else if (name == "Fog Override")
	{
		m_Selected->setFog(
			event.GetPropertyValue().GetBool(), 
			m_Selected->getFogMode(),
			m_Selected->getFogColour(),
			m_Selected->getFogDensity(),
			m_Selected->getFogStart(),
			m_Selected->getFogEnd());
	}
	else if (name == "Fog Mode")
	{
		m_Selected->setFog(
			m_Selected->getFogOverride(), 
			(Ogre::FogMode)event.GetPropertyValue().GetInteger(),
			m_Selected->getFogColour(),
			m_Selected->getFogDensity(),
			m_Selected->getFogStart(),
			m_Selected->getFogEnd());
	}
	else if (name == "Fog Color")
	{
		m_Selected->setFog(
			m_Selected->getFogOverride(),
			m_Selected->getFogMode(),
			Properties::GetColorFromValue(event.GetPropertyValue()),
			m_Selected->getFogDensity(),
			m_Selected->getFogStart(),
			m_Selected->getFogEnd());
	}
	else if (name == "Fog Start")
	{
		m_Selected->setFog(
			m_Selected->getFogOverride(),
			m_Selected->getFogMode(),
			m_Selected->getFogColour(),
			m_Selected->getFogDensity(),
			event.GetPropertyValue().GetDouble(),
			m_Selected->getFogEnd());
	}
	else if (name == "Fog End")
	{
		m_Selected->setFog(
			m_Selected->getFogOverride(),
			m_Selected->getFogMode(),
			m_Selected->getFogColour(),
			m_Selected->getFogDensity(),
			m_Selected->getFogStart(),
			event.GetPropertyValue().GetDouble());
	}
	else if (name == "Fog Density")
	{
		m_Selected->setFog(
			m_Selected->getFogOverride(),
			m_Selected->getFogMode(),
			m_Selected->getFogColour(),
			event.GetPropertyValue().GetDouble(),
			m_Selected->getFogStart(),
			m_Selected->getFogEnd());
	}
	else if (name.StartsWith("Tex-"))
	{
		Ogre::vector<Ogre::String>::type temp = Ogre::StringUtil::split(STRING_WX_TO_OGRE(name), "-");
		name = STRING_OGRE_TO_WX(temp[1]);
		Ogre::TextureUnitState *tex = m_Selected->getTextureUnitState(Ogre::StringConverter::parseUnsignedInt(temp[2]) - 1);

		if (name == "File Name")
		{
			tex->setTextureName(STRING_WX_TO_OGRE(event.GetPropertyValue().GetString()), tex->getTextureType());
		}
		else if (name == "Type")
		{
			tex->setTextureName(tex->getTextureName(), (Ogre::TextureType)event.GetPropertyValue().GetInteger());
		}
		else if (name == "Desired Format")
		{
			tex->setDesiredFormat((Ogre::PixelFormat)event.GetPropertyValue().GetInteger());
		}
		else if (name == "Number Of Mipmaps")
		{
			tex->setNumMipmaps(event.GetPropertyValue().GetInteger());
		}
		else if (name == "Texture Coord Set")
		{
			tex->setTextureCoordSet(event.GetPropertyValue().GetInteger());
		}
		else if (name == "U Texture Addressing Mode")
		{
			tex->setTextureAddressingMode((Ogre::TextureUnitState::TextureAddressingMode)event.GetPropertyValue().GetInteger(),
				tex->getTextureAddressingMode().v,
				tex->getTextureAddressingMode().w);
		}
		else if (name == "V Texture Addressing Mode")
		{
			tex->setTextureAddressingMode(tex->getTextureAddressingMode().u,
				(Ogre::TextureUnitState::TextureAddressingMode)event.GetPropertyValue().GetInteger(),
				tex->getTextureAddressingMode().w);
		}
		else if (name == "W Texture Addressing Mode")
		{
			tex->setTextureAddressingMode(tex->getTextureAddressingMode().u,
				tex->getTextureAddressingMode().v,
				(Ogre::TextureUnitState::TextureAddressingMode)event.GetPropertyValue().GetInteger());
		}
		else if (name == "Border Color")
		{
			tex->setTextureBorderColour(Properties::GetColorFromValue(event.GetPropertyValue()));
		}
		else if (name == "Source Blend Fallback")
		{
			tex->setColourOpMultipassFallback((Ogre::SceneBlendFactor)event.GetPropertyValue().GetInteger(), tex->getColourBlendFallbackDest());
		}
		else if (name == "Destination Blend Fallback")
		{
			tex->setColourOpMultipassFallback(tex->getColourBlendFallbackSrc(), (Ogre::SceneBlendFactor)event.GetPropertyValue().GetInteger());
		}
		else if (name == "Is Alpha")
		{
			tex->setIsAlpha(event.GetPropertyValue().GetBool());
		}
		else if (name == "Hardware Gamma Enabled")
		{
			tex->setHardwareGammaEnabled(event.GetPropertyValue().GetBool());
		}
		else if (name == "U Texture Scroll")
		{
			tex->setTextureUScroll(event.GetPropertyValue().GetDouble());
		}
		else if (name == "V Texture Scroll")
		{
			tex->setTextureVScroll(event.GetPropertyValue().GetDouble());
		}
		else if (name == "U Texture Scale")
		{
			tex->setTextureUScale(event.GetPropertyValue().GetDouble());
		}
		else if (name == "V Texture Scale")
		{
			tex->setTextureVScale(event.GetPropertyValue().GetDouble());
		}
		else if (name == "Texture Rotation")
		{
			tex->setTextureRotate(Ogre::Radian(Ogre::Degree(event.GetPropertyValue().GetDouble())));
		}
		else if (name == "Minification Filter Option")
		{
			tex->setTextureFiltering((Ogre::FilterOptions)event.GetPropertyValue().GetInteger(),
				tex->getTextureFiltering(Ogre::FT_MAG),
				tex->getTextureFiltering(Ogre::FT_MIP));
		}
		else if (name == "Magnification Filter Option")
		{
			tex->setTextureFiltering(tex->getTextureFiltering(Ogre::FT_MIN),
				(Ogre::FilterOptions)event.GetPropertyValue().GetInteger(),
				tex->getTextureFiltering(Ogre::FT_MIP));
		}
		else if (name == "MipMapping Filter Option")
		{
			tex->setTextureFiltering(tex->getTextureFiltering(Ogre::FT_MIN),
				tex->getTextureFiltering(Ogre::FT_MAG),
				(Ogre::FilterOptions)event.GetPropertyValue().GetInteger());
		}
		else if (name == "Anisotropy")
		{
			tex->setTextureAnisotropy(event.GetPropertyValue().GetInteger());
		}
		else if (name == "Binding Type")
		{
			tex->setBindingType((Ogre::TextureUnitState::BindingType)event.GetPropertyValue().GetInteger());
		}
	}

	MaterialEditorControl::GetPointer()->_GetRenderControl()->SetFocus();
	MaterialEditorControl::GetPointer()->RefreshScript();
}


MaterialEditorControl::MaterialEditorControl(wxWindow *Parent) :
	wxWindow(Parent, -1),
	m_ToolBar(NULL),
	m_MaterialsList(NULL),
	//m_TechniquesList(NULL),
	m_PassesList(NULL),
	m_SceneManager(NULL),
	m_SceneNodeCube(NULL),
	m_SceneNodeSphere(NULL),
	m_SceneNodeCurrent(NULL),
	m_TechniquePropertyListener(NULL),
	m_PassPropertyListener(NULL),
	m_RenderControl(NULL)
{
}


//MaterialEditorControl::~MaterialEditorControl(void)
//{
//}


void MaterialEditorControl::Initialize(void)
{
	wxBoxSizer *bs = new wxBoxSizer(wxVERTICAL);
	
	m_ToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_FLAT);
	m_ToolBar->AddTool(IDB_SAVE, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "SaveSmall.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Save");
	m_ToolBar->AddSeparator();
	m_ToolBar->AddTool(IDB_APPLY, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Apply.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Apply to all objects");	
	m_ToolBar->AddTool(IDB_OBJECT_ITERATION, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Objects.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Object Iteration");
	m_ToolBar->Realize();
	Connect(IDB_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MaterialEditorControl::OnSave));
	Connect(IDB_APPLY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MaterialEditorControl::OnApply));
	Connect(IDB_OBJECT_ITERATION, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MaterialEditorControl::OnObjectIteration));
	bs->Add(m_ToolBar, 0, wxEXPAND, 5);

	m_MaterialsList = new wxChoice(this, wxID_ANY);
	m_MaterialsList->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(MaterialEditorControl::OnMaterialSelected), NULL, this);
	bs->Add(m_MaterialsList, 0, wxEXPAND, 5);

	//m_TechniquesList = new wxChoice(this, wxID_ANY);
	//m_TechniquesList->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(MaterialEditorControl::OnTechniqueSelected), NULL, this);
	//bs->Add(m_TechniquesList, 0, wxEXPAND, 5);

	m_PassesList = new wxChoice(this, wxID_ANY);
	m_PassesList->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(MaterialEditorControl::OnPassSelected), NULL, this);
	bs->Add(m_PassesList, 0, wxEXPAND, 5);

	m_SceneManager = Engine::GetInstance().GetGraphic()->createSceneManager(Ogre::ST_GENERIC, "MaterialEditorSceneManager");
	m_SceneManager->setAmbientLight(Ogre::ColourValue::Black);

	m_SceneNodeCube = m_SceneManager->getRootSceneNode()->createChildSceneNode("CubeSceneNode");
	m_SceneNodeCube->attachObject(m_SceneManager->createEntity("Cube", Ogre::SceneManager::PT_CUBE));
	m_SceneNodeCube->setVisible(false);

	m_SceneNodeSphere = m_SceneManager->getRootSceneNode()->createChildSceneNode("SphereSceneNode");
	//m_SceneNodeSphere->attachObject(m_SceneManager->createEntity("Sphere", Ogre::SceneManager::PT_SPHERE));
	Ogre::ManualObject *mo = m_SceneManager->createManualObject("Sphere");
	ManualFunctionality::AddSphere(mo, 100, 30, 30, "BaseWhiteNoLighting");
	m_SceneNodeSphere->attachObject(m_SceneManager->createEntity("Sphere"));
	m_SceneNodeSphere->setVisible(false);
	m_SceneNodeCurrent = m_SceneNodeSphere;


	Ogre::Light *light = NULL;

	//light = m_SceneManager->createLight("DirectionalX1");
	//light->setType(Ogre::Light::LT_DIRECTIONAL);
	//light->setDirection(Ogre::Vector3::UNIT_X);

	light = m_SceneManager->createLight("DirectionalX2");
	light->setType(Ogre::Light::LT_POINT);
	light->setDirection(Ogre::Vector3::NEGATIVE_UNIT_X);
	light->setPosition(100, 100, 100);

	//light = m_SceneManager->createLight("DirectionalY1");
	//light->setType(Ogre::Light::LT_DIRECTIONAL);
	//light->setDirection(Ogre::Vector3::UNIT_Y);

	//light = m_SceneManager->createLight("DirectionalY2");
	//light->setType(Ogre::Light::LT_DIRECTIONAL);
	//light->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

	//light = m_SceneManager->createLight("DirectionalZ1");
	//light->setType(Ogre::Light::LT_DIRECTIONAL);
	//light->setDirection(Ogre::Vector3::UNIT_Z);

	//light = m_SceneManager->createLight("DirectionalZ2");
	//light->setType(Ogre::Light::LT_DIRECTIONAL);
	//light->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);
	
	
	m_Notebook = new wxNotebook(this, wxID_ANY);
	bs->Add(m_Notebook, 1, wxEXPAND | wxALL, 5);

	m_RenderControl = new RenderControl(m_Notebook, "MaterialEditorViewPort", false, new EditorCamera(EditorCamera::ECT_FLY, m_SceneManager->createCamera("MaterialEditorCamera")));
	m_Notebook->AddPage(m_RenderControl, "Render View", true);

	m_ScriptPage = new MaterialPageControl(m_Notebook);
	m_Notebook->AddPage(m_ScriptPage, "Material Script", false);
	
	SetSizer(bs);
	Layout();

	Refresh();
}


void MaterialEditorControl::Update(void)
{
	if (m_RenderControl)
		m_RenderControl->_Update();
}


void MaterialEditorControl::SelectMaterial(const wxString &MaterialName)
{
	int result = m_MaterialsList->FindString(MaterialName, true);
	
	if (result)
	{
		m_MaterialsList->Select(result);
		OnMaterialSelected(wxCommandEvent());

		((MainWindow*)GetParent())->ShowWindow(GetName());
	}
}


void MaterialEditorControl::Refresh(void)
{
	m_PassesList->Clear();
	//m_TechniquesList->Clear();
	m_MaterialsList->Clear();

	Ogre::ResourceManager::ResourceMapIterator it = Ogre::MaterialManager::getSingleton().getResourceIterator();
	Ogre::MaterialPtr material;
	while (it.hasMoreElements())
	{
		material = it.getNext();
		
		if (material->getGroup() == Engine::RESOURCES_MATERIAL)
			m_MaterialsList->Append(material->getName());
	}

	m_MaterialsList->Select(0);
	OnMaterialSelected(wxCommandEvent());
}


void MaterialEditorControl::Save(void)
{
	SaveCore();

	const wxString &selection = m_MaterialsList->GetString(m_MaterialsList->GetSelection());

	Refresh();

	m_MaterialsList->Select(m_MaterialsList->FindString(selection));
	OnMaterialSelected(wxCommandEvent());
}


void MaterialEditorControl::SaveCore(void)
{
	wxString path = STRING_OGRE_TO_WX(Engine::PATH_MATERIAL + "TempMaterial\\");
	CreateDirectory(path.c_str(), 0);

	std::vector<Ogre::String> &list = GetFiles(Engine::PATH_MATERIAL);

	for (unsigned int i = 0; i < list.size(); i++)
	{
		DeleteFile((path + wxFileNameFromPath(list[i])).c_str());

		MoveFile(list[i].c_str(), (path + wxFileNameFromPath(list[i])).c_str());
	}

	//path = Engine::PATH_MATERIAL + "Material.material";

	//Ogre::MaterialSerializer ms;
	//Ogre::MaterialPtr material;

	//for (unsigned int i = 0; i < m_MaterialsList->GetCount(); i++)
	//{
	//	if (m_MaterialsList->GetSelection() != i)
	//	{
	//		material = Ogre::MaterialManager::getSingleton().getByName(m_MaterialsList->GetString(i));

	//		//if (!material.isNull())
	//			ms.queueForExport(material);
	//	}
	//}

	//ms.exportQueued(path);

	bool flag = true;

	Ogre::MaterialPtr material;
	Ogre::MaterialSerializer ms;
	ms.addListener(this);
	//File *file;
	for (unsigned int i = 0; i < m_MaterialsList->GetCount(); i++)
	{
		//path = Engine::PATH_MATERIAL + "Material_" + Ogre::StringConverter::toString(i + 1) + ".material";
		material = Ogre::MaterialManager::getSingleton().getByName(STRING_WX_TO_OGRE(m_MaterialsList->GetString(i)));
		path = Engine::PATH_MATERIAL + "Material_" + Ogre::StringUtil::replaceAll(Ogre::StringUtil::replaceAll(material->getName(), "/", "_"), "\\", "_") + ".material";

		try
		{
			if (material->getName() == "_45_3stop1")
			{
				
				ms.clearQueue();
			}
			//if (m_MaterialsList->GetSelection() != i)
			//{
			//	file = new File(path, true);
			//	file->WriteText(m_ScriptPage->GetText());
			//	file->Close();
			//	delete file;
			//	file = NULL;
			//}
			//else
			//{
				ms.clearQueue();
				ms.exportMaterial(material, STRING_WX_TO_OGRE(path));
			//}

			ManualFunctionality::ReloadMaterial(material->getName(), Engine::RESOURCES_MATERIAL, STRING_WX_TO_OGRE(wxFileNameFromPath(path)), true);
		}
		catch(std::exception &e)
		{
			Log::GetInstance().AddError(STRING_WX_TO_OGRE("Saving material [" + m_MaterialsList->GetString(i) + "] has an error [" + e.what() + "]"));
			flag = false;
		}
	}

	if (flag)
		Log::GetInstance().AddInfo("Materials successfully saved");
}


void MaterialEditorControl::RefreshMaterialProperties(void)
{
	Ogre::String temp = "";
	const Ogre::MaterialPtr &mat = GetCurrentMaterial();

	if (!mat->getNumTechniques())
	{
		Log::GetInstance().AddError("Material [" + mat->getName() + "] has no technique");
		goto Cleanup;
	}
	else if (!mat->getTechnique(0)->getNumPasses())
	{
		Log::GetInstance().AddError("Material [" + mat->getName() + "] has no pass");
		goto Cleanup;
	}

	m_PassesList->Clear();

	Ogre::Technique *technique = mat->getTechnique(0);

	if (m_TechniquePropertyListener)
		delete m_TechniquePropertyListener;
	m_TechniquePropertyListener = new TechniquePropertyListener(technique);

	Properties &prop = Properties::GetInstance();
	prop.BeginNewProperties(GetCaption() + " - Material", m_TechniquePropertyListener);
	prop.AddCategory("Texture Mapping");
	
	MaterialEditorControl::MaterialType mt = GetTypeOfMaterial(mat);

	prop.AddPropertyOfEnumerator_MaterialType("Type", mt);

	Ogre::Pass *pass = NULL;
	Ogre::TextureUnitState *tex = NULL;
	for (unsigned int i = 0; i < technique->getNumPasses(); i++)
	{
		pass = technique->getPass(i);
		temp = pass->getName();
		
		m_PassesList->Append("Pass " + (temp.size() > 2 ? temp : Ogre::StringConverter::toString(i + 1)), pass);

		for (unsigned int texIndex = 0; texIndex < pass->getNumTextureUnitStates(); texIndex++)
		{
			tex = pass->getTextureUnitState(texIndex);
			temp = tex->getTextureNameAlias();

			if (!temp.size() && mt == MaterialEditorControl::MT_BumpDiffuse)
			{
				pass->removeTextureUnitState(texIndex);
				continue;
			}

			if (!temp.size() ||
				(temp == TEXALIAS_NORMAL_NAME && (mt != MaterialEditorControl::MT_BumpDiffuse && mt != MaterialEditorControl::MT_BumpSpecularDiffuse)) ||
				(temp == TEXALIAS_SPECULAR_NAME && mt != MaterialEditorControl::MT_BumpSpecularDiffuse) ||
				(temp == TEXALIAS_REFLECTION_NAME))
				continue;

			prop.AddPropertyOfString(
				Ogre::StringUtil::replaceAll(temp, "_", " "), 
				tex->getTextureName(), "", false, 
				temp + "-" + Ogre::StringConverter::toString(i) + "-" + Ogre::StringConverter::toString(texIndex));
		}
	}

	m_PassesList->Select(0);

	prop.EndNewProperties();

	//OnPassSelected(wxCommandEvent());

	ManualFunctionality::ClearPassMaps();

Cleanup:
	pass = NULL;
	technique = NULL;
}


void MaterialEditorControl::RefreshScript(void)
{
	Ogre::MaterialSerializer ms;
	ms.addListener(this);
	ms.queueForExport(GetCurrentMaterial());
	m_ScriptPage->SetText(STRING_OGRE_TO_WX(ms.getQueuedAsString()));
}


void MaterialEditorControl::SaveScript(void)
{
	Ogre::MaterialPtr &mat = GetCurrentMaterial();

	File file(Engine::PATH_MATERIAL + "Material_" + Ogre::StringUtil::replaceAll(Ogre::StringUtil::replaceAll(mat->getName(), "/", "_"), "\\", "_") + ".material", false);
	file.WriteText(STRING_WX_TO_OGRE(m_ScriptPage->GetText()));
	file.Close();
	
	Log::GetInstance().AddInfo(STRING_WX_TO_OGRE("Material [" + mat->getName() + "] saved successfully in [" + file.GetFileName() + "]"));
		
	ManualFunctionality::ReloadMaterial(mat->getName(), Engine::RESOURCES_MATERIAL, STRING_WX_TO_OGRE(wxFileNameFromPath(file.GetFileName())), true);
}


Ogre::MaterialPtr MaterialEditorControl::GetCurrentMaterial(void)
{
	return (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(STRING_WX_TO_OGRE(m_MaterialsList->GetString(m_MaterialsList->GetSelection())));
}


MaterialEditorControl::MaterialType MaterialEditorControl::GetTypeOfMaterial(const Ogre::MaterialPtr &Material)
{
	Ogre::Pass *pass = Material->getTechnique(0)->getPass(0);
	
	bool bump = (pass->getName() == PASS_BUMP_NAME), 
		specular = false;

	if (pass->hasFragmentProgram())
	{
		if (pass->getFragmentProgramParameters()->hasNamedParameters())
		{
			float f = -1;
			pass->getFragmentProgramParameters()->_readRawConstants(pass->getFragmentProgramParameters()->getConstantDefinition("useSpecular").physicalIndex, 1, &f);
			specular = (f == 1);
		}
	}
	
	if (bump && specular)
		return MT_BumpSpecularDiffuse;
	else if (specular)
		return MT_SpecularDiffuse;
	else if (bump)
		return MT_BumpDiffuse;
	else
		return MT_Diffuse;
}

	
void MaterialEditorControl::OnSize(wxSizeEvent &event)
{
	if (!m_MaterialsList)
		return;

	m_MaterialsList->SetSize(GetSize().x, m_MaterialsList->GetSize().y);
	int y = m_MaterialsList->GetSize().y;

	//m_TechniquesList->SetSize(GetSize().x, m_TechniquesList->GetSize().y);
	//y += m_TechniquesList->GetSize().y;

	m_PassesList->SetSize(GetSize().x, m_PassesList->GetSize().y);
	y += m_PassesList->GetSize().y;

	m_ToolBar->SetSize(GetSize().x, m_ToolBar->GetSize().y);
	y += m_ToolBar->GetSize().y;

	m_Notebook->SetSize(GetSize().x, GetSize().y - y);
}


void MaterialEditorControl::OnMaterialSelected(wxCommandEvent &event)
{
	//m_TechniquesList->Clear();

	const Ogre::MaterialPtr &mat = GetCurrentMaterial();

	if (mat.isNull())
		return;
	
	((Ogre::Entity*)m_SceneNodeCube->getAttachedObject(0))->setMaterial(mat);
	((Ogre::Entity*)m_SceneNodeSphere->getAttachedObject(0))->setMaterial(mat);
	m_SceneNodeCurrent->setVisible(true);

	m_RenderControl->GetCamera()->SetPosition(m_SceneNodeCurrent->getAttachedObject(0)->getBoundingBox().getAllCorners()[4] + Ogre::Vector3(100, 100, 100));
	m_RenderControl->GetCamera()->LookAt(0, 0, 0);

	//Ogre::Technique *technique = NULL;
	//Ogre::String cat =  "";
	//for (unsigned int i = 0; i < mat->getNumTechniques(); i++)
	//{
	//	technique = mat->getTechnique(i);

	//	if (technique->getName().size())
	//		cat = technique->getName();
	//	else
	//		cat = "Technique " + Ogre::StringConverter::toString(i + 1);

	//	m_TechniquesList->Append(cat, technique);
	//}

	//m_TechniquesList->Select(0);
	//OnTechniqueSelected(wxCommandEvent());

	RefreshScript();

	RefreshMaterialProperties();
}


//void MaterialEditorControl::OnTechniqueSelected(wxCommandEvent &event)
//{
//
//	
//
//	
//}


void MaterialEditorControl::OnPassSelected(wxCommandEvent &event)
{
	//Ogre::NameValuePairList param;
	//param["language"] = "hlsl";
	//Ogre::HighLevelGpuProgramManager::getSingleton().load("normalmap_vs.hlsl", Engine::RESOURCES_MATERIAL, false, 0, &param);
	Ogre::Pass *pass = (Ogre::Pass*)m_PassesList->GetClientData(m_PassesList->GetSelection());

	if (m_PassPropertyListener)
		delete m_PassPropertyListener;
	m_PassPropertyListener = new PassPropertyListener(pass);

	Properties &prop = Properties::GetInstance();

	prop.BeginNewProperties(GetCaption() + " - Pass", m_PassPropertyListener);

	prop.AddCategory("General");
	//prop.AddPropertyOfString("Name", pass->getParent()->getParent()->getName(), "", true);
	prop.AddPropertyOfBool("Lighting Enabled", pass->getLightingEnabled(), "Whether or not dynamic lighting is enabled");
	prop.AddPropertyOfInt("Maximum Simultaneous Lights", pass->getMaxSimultaneousLights(), "The maximum number of lights to be used by this pass");
	//pass->getStartLight();
	prop.AddPropertyOfBool("Iterate Per Light", pass->getIteratePerLight(), "Does this pass run once for every light in range");
	prop.AddPropertyOfInt("Light Count Per Iteration", pass->getLightCountPerIteration(), "If light iteration is enabled, determine the number of lights per iteration");
	prop.AddPropertyOfBool("Run Only For One Light Type", pass->getRunOnlyForOneLightType(), "Does this pass run only for a single light type (if getIteratePerLight is true)");
	prop.AddPropertyOfEnumerator_LightTypes("Only Light Type", pass->getOnlyLightType(), "The single light type this pass runs for if  getIteratePerLight and getRunOnlyForOneLightType are both true");
	prop.AddPropertyOfEnumerator_ShadeOptions("Shading Mode", pass->getShadingMode(), "The type of light shading to be used");
	prop.AddPropertyOfEnumerator_PolygonMode("Polygon Mode", pass->getPolygonMode(), "The type of polygon rendering to be used");
	prop.AddPropertyOfBool("Normalise Normals", pass->getNormaliseNormals(), "True if this pass has auto-normalisation of normals set");
	prop.AddPropertyOfBool("Polygon Mode Overrideable", pass->getPolygonModeOverrideable(), "Whether this renderable's chosen detail level can be overridden (downgraded) by the camera setting");

	prop.AddCategory("Color");
	prop.AddPropertyOfColor("Ambient", pass->getAmbient(), "The ambient color reflectance of the pass");
	prop.AddPropertyOfColor("Diffuse", pass->getDiffuse(), "The diffuse color reflectance of the pass");
	prop.AddPropertyOfColor("Specular", pass->getSpecular(), "The specular color reflectance of the pass");
	prop.AddPropertyOfColor("Self Illumination (Emissive)", pass->getSelfIllumination(), "The self illumination color of the pass");
	prop.AddPropertyOfFloat("Shininess", pass->getShininess(), "'Shininess' property of the pass (affects specular highlights)\nMust be a value between 0 and 128");
	prop.AddPropertyOfEnumerator_TrackVertexColourType("Vertex Color Tracking", pass->getVertexColourTracking(), "Which material properties follow the vertex color");

	prop.AddCategory("Blending");
	prop.AddPropertyOfEnumerator_SceneBlendFactor("Source Blend Factor", pass->getSourceBlendFactor());
	prop.AddPropertyOfEnumerator_SceneBlendFactor("Destination Blend Factor", pass->getDestBlendFactor());
	prop.AddPropertyOfEnumerator_SceneBlendFactor("Source Blend Factor Alpha", pass->getSourceBlendFactorAlpha());
	prop.AddPropertyOfEnumerator_SceneBlendFactor("Destination Blend Factor Alpha", pass->getDestBlendFactorAlpha());
	prop.AddPropertyOfEnumerator_SceneBlendOperation("Scene Blending Operation", pass->getSceneBlendingOperation());
	prop.AddPropertyOfEnumerator_SceneBlendOperation("Scene Blending Operation Alpha", pass->getSceneBlendingOperationAlpha());

	prop.AddCategory("Depth");
	prop.AddPropertyOfBool("Depth Check Enabled", pass->getDepthCheckEnabled(), "If depth-buffer checking is on, whenever a pixel is about to be written to the frame buffer the depth buffer is checked to see if the pixel is in front of all other pixels written at that point. If not, the pixel is not written");
	prop.AddPropertyOfBool("Depth Write Enabled", pass->getDepthWriteEnabled(), "If depth-buffer writing is on, whenever a pixel is written to the frame buffer the depth buffer is updated with the depth value of that new pixel, thus affecting future rendering operations if future pixels are behind this one");
	prop.AddPropertyOfEnumerator_CompareFunction("Depth Function", pass->getDepthFunction(), "If depth checking is enabled (see setDepthCheckEnabled) a comparison occurs between the depth value of the pixel to be written and the current contents of the buffer. This comparison is normally CMPF_LESS_EQUAL, i.e. the pixel is written if it is closer (or at the same distance) than the current contents. If you wish you can change this comparison using this method");
	prop.AddPropertyOfFloat("Depth Bias Constant", pass->getDepthBiasConstant(), "The const depth bias value as set by setDepthBias");
	prop.AddPropertyOfFloat("Depth Bias Slope Scale", pass->getDepthBiasSlopeScale(), "The slope-scale depth bias value as set by setDepthBias");
	prop.AddPropertyOfFloat("Iteration Depth Bias", pass->getIterationDepthBias(), "A factor which derives an additional depth bias from the number of times a pass is iterated");
	prop.AddPropertyOfBool("Color Write Enabled", pass->getColourWriteEnabled(), "If color buffer writing is enabled for this pass");
	prop.AddPropertyOfEnumerator_CompareFunction("Alpha Reject Function", pass->getAlphaRejectFunction(), "The alpha reject function. See setAlphaRejectSettings for more information");
	prop.AddPropertyOfInt("Alpha Reject Value", pass->getAlphaRejectValue(), "The alpha reject value. See setAlphaRejectSettings for more information\nMust be a value between 0 and 255");
	prop.AddPropertyOfBool("Alpha To Coverage Enabled", pass->isAlphaToCoverageEnabled(), "Whether to use alpha to coverage (A2C) when blending alpha rejected values");
	prop.AddPropertyOfBool("Transparent Sorting Enabled", pass->getTransparentSortingEnabled(), "Whether or not transparent sorting is enabled");
	prop.AddPropertyOfBool("Transparent Sorting Forced", pass->getTransparentSortingForced(), "Whether or not transparent sorting is forced");
	
	prop.AddCategory("Culling");
	prop.AddPropertyOfEnumerator_CullingMode("Culling Mode", pass->getCullingMode(), "A typical way for the rendering engine to cull triangles is based on the 'vertex winding' of triangles. Vertex winding refers to the direction in which the vertices are passed or indexed to in the rendering operation as viewed from the camera, and will wither be clockwise or Anti-Clockwise (that's 'counterclockwise' for you Americans out there ;) The default is Clockwise i.e. that only triangles whose vertices are passed/indexed in Anti-Clockwise order are rendered - this is a common approach and is used in 3D studio models for example. You can alter this culling mode if you wish but it is not advised unless you know what you are doing.");
	prop.AddPropertyOfEnumerator_ManualCullingMode("Manual Culling Mode", pass->getManualCullingMode(), "In some situations you want to use manual culling of triangles rather than sending the triangles to the hardware and letting it cull them. This setting only takes effect on SceneManager's that use it (since it is best used on large groups of planar world geometry rather than on movable geometry since this would be expensive), but if used can cull geometry before it is sent to the hardware.");

	prop.AddCategory("Fog");
	prop.AddPropertyOfBool("Fog Override", pass->getFogOverride(), "If true, you authorise this pass to override the scene's fog params with it's own settings.\nIf you specify false, so other parameters are necessary, and this is the default behaviour for passes");
	prop.AddPropertyOfEnumerator_FogMode("Fog Mode", pass->getFogMode(), "The fog mode for this pass\nExponentially from the camera (fog = 1/e^(distance * density))\nSquare of ponentially, i.e. even quicker (fog = 1/e^(distance * density)^2)\nIncreases linearly between the start and end distances");
	prop.AddPropertyOfColor("Fog Color", pass->getFogColour(), "The color of the fog. Either set this to the same as your viewport background colour, or to blend in with a skydome or skybox");
	prop.AddPropertyOfFloat("Fog Start", pass->getFogStart(), "Distance in world units at which linear fog starts to encroach. Only applicable if mode is Linear");
	prop.AddPropertyOfFloat("Fog End", pass->getFogEnd(), "Distance in world units at which linear fog becomes completely opaque. Only applicable if mode is Linear");
	prop.AddPropertyOfFloat("Fog Density", pass->getFogDensity(), "The density of the fog in 'Exponentially' or 'Square of Exponentially' mode, as a value between 0 and 1. The default is 0.001");
	
	if (pass->getNumTextureUnitStates())
	{
		wxPGProperty *texProp = prop.AddCategory("Texture");
		Ogre::String number = "";
		Ogre::String prefix = "Tex-";
		Ogre::TextureUnitState *texture = NULL;
		for (unsigned int i = 0; i < pass->getNumTextureUnitStates(); i++)
		{
			number = Ogre::StringConverter::toString(i + 1);

			prop.AddCategory("Layer " + number, "Layer" + number, texProp);

			number = "-" + number;

			texture = pass->getTextureUnitState(i);
			
			prop.AddPropertyOfString("File Name", texture->getTextureName(), "", false, prefix + "File Name" + number);

			//mCurrentFrame
			//mAnimDuration
			prop.AddPropertyOfEnumerator_TextureType("Type", texture->getTextureType(), "", prefix + "Type" + number);
			prop.AddPropertyOfEnumerator_PixelFormat("Desired Format", texture->getDesiredFormat(), "", prefix + "Desired Format" + number);
			prop.AddPropertyOfInt("Number Of Mipmaps", texture->getNumMipmaps(), "", prefix + "Number Of Mipmaps" + number);
			prop.AddPropertyOfInt("Texture Coord Set", texture->getTextureCoordSet(), "", prefix + "Texture Coord Set" + number);
		
			prop.AddPropertyOfEnumerator_TextureAddressingMode("U Texture Addressing Mode", texture->getTextureAddressingMode().u, "", prefix + "U Texture Addressing Mode" + number);
			prop.AddPropertyOfEnumerator_TextureAddressingMode("V Texture Addressing Mode", texture->getTextureAddressingMode().v, "", prefix + "V Texture Addressing Mode" + number);
			prop.AddPropertyOfEnumerator_TextureAddressingMode("W Texture Addressing Mode", texture->getTextureAddressingMode().w, "", prefix + "W Texture Addressing Mode" + number);
			prop.AddPropertyOfColor("Border Color", texture->getTextureBorderColour(), "", prefix + "Border Color" + number);

			//mColourBlendMode
			prop.AddPropertyOfEnumerator_SceneBlendFactor("Source Blend Fallback", texture->getColourBlendFallbackSrc(), "Multipass fallback for colour blending operation source factor", prefix + "Source Blend Fallback" + number);
			prop.AddPropertyOfEnumerator_SceneBlendFactor("Destination Blend Fallback", texture->getColourBlendFallbackDest(), "Multipass fallback for colour blending operation destination factor", prefix + "Destination Blend Fallback" + number);
			//mAlphaBlendMode
			prop.AddPropertyOfBool("Is Alpha", texture->getIsAlpha(), "Whether this texture is requested to be loaded as alpha if single channel", prefix + "Is Alpha" + number);
			prop.AddPropertyOfBool("Hardware Gamma Enabled", texture->isHardwareGammaEnabled(), "Whether this texture will be set up so that on sampling it, hardware gamma correction is applied", prefix + "Hardware Gamma Enabled" + number);

			prop.AddPropertyOfFloat("U Texture Scroll", texture->getTextureUScroll(), "", prefix + "U Texture Scroll" + number);
			prop.AddPropertyOfFloat("V Texture Scroll", texture->getTextureVScroll(), "", prefix + "V Texture Scroll" + number);
			prop.AddPropertyOfFloat("U Texture Scale", texture->getTextureUScale(), "", prefix + "U Texture Scale" + number);
			prop.AddPropertyOfFloat("V Texture Scale", texture->getTextureVScale(), "", prefix + "V Texture Scale" + number);
			prop.AddPropertyOfFloat("Texture Rotation", Ogre::Degree(texture->getTextureRotate()).valueDegrees(), "", prefix + "Texture Rotation" + number);

			prop.AddPropertyOfEnumerator_FilterOptions("Minification Filter Option", texture->getTextureFiltering(Ogre::FT_MIN), "", prefix + "Minification Filter Option" + number);
			prop.AddPropertyOfEnumerator_FilterOptions("Magnification Filter Option", texture->getTextureFiltering(Ogre::FT_MAG), "", prefix + "Magnification Filter Option" + number);
			prop.AddPropertyOfEnumerator_FilterOptions("MipMapping Filter Option", texture->getTextureFiltering(Ogre::FT_MIP), "", prefix + "MipMapping Filter Option" + number);
			prop.AddPropertyOfInt("Anisotropy", texture->getTextureAnisotropy(), "", prefix + "Anisotropy" + number);
			prop.AddPropertyOfEnumerator_BindingType("Binding Type", texture->getBindingType(), "", prefix + "Binding Type" + number);
			//mContentType
			//mCompositorRefMrtIndex
			//...
			//...
			//...

			const Ogre::TextureUnitState::EffectMap &effects = texture->getEffects();

			

			// ET_ENVIRONMENT_MAP,
   //         /// Generate texture coords based on a frustum
   //         ET_PROJECTIVE_TEXTURE,
   //         /// Constant u/v scrolling effect
   //         ET_UVSCROLL,
			///// Constant u scrolling effect
   //         ET_USCROLL,
			///// Constant u/v scrolling effect
   //         ET_VSCROLL,
   //         /// Constant rotation
   //         ET_ROTATE,
   //         /// More complex transform
   //         ET_TRANSFORM
		}
		texture = NULL;
	}

	prop.EndNewProperties();
	
	m_RenderControl->SetFocus();
}


void MaterialEditorControl::OnSave(wxCommandEvent &event)
{
	if (m_Notebook->GetPage(m_Notebook->GetSelection()) == m_ScriptPage)
		SaveScript();
	else
		Save();

	ManualFunctionality::UpdateForMaterialChanges(m_SceneManager);
}


void MaterialEditorControl::OnApply(wxCommandEvent &event)
{
	if (m_Notebook->GetPage(m_Notebook->GetSelection()) == m_ScriptPage)
		SaveScript();
	else
		Save();

	ManualFunctionality::UpdateForMaterialChanges();
}


void MaterialEditorControl::OnObjectIteration(wxCommandEvent &event)
{
	m_SceneNodeCurrent->setVisible(false);

	if (m_SceneNodeCurrent == m_SceneNodeCube)
	{
		m_SceneNodeCurrent = m_SceneNodeSphere;
	}
	else if (m_SceneNodeCurrent == m_SceneNodeSphere)
	{
		m_SceneNodeCurrent = m_SceneNodeCube;
	}

	m_SceneNodeCurrent->setVisible(true);

	Ogre::MovableObject *object = m_SceneNodeCurrent->getAttachedObject(0);
	m_RenderControl->GetCamera()->SetPosition(object->getBoundingBox().getAllCorners()[4] + Ogre::Vector3(100, 100, 100));
	m_RenderControl->GetCamera()->LookAt(object->getBoundingBox().getCenter());
	object = NULL;
}


void MaterialEditorControl::materialEventRaised(Ogre::MaterialSerializer *Serializer, Ogre::MaterialSerializer::SerializeEvent Event, bool &Skip, const Ogre::Material *Material)
{
	if (Event == Ogre::MaterialSerializer::MSE_PRE_WRITE)
	{
		m_ImportText = m_InheritanceText = "";
		Serializer->writeAttribute(0, MATERIAL_PARENT_TEMPLATE_IMPORT);
	}
	else if (Event == Ogre::MaterialSerializer::MSE_WRITE_END)
	{
		Ogre::String temp = Ogre::String("material ") + Material->getName();

		Serializer->mBuffer = Ogre::StringUtil::replaceAll(Serializer->mBuffer, MATERIAL_PARENT_TEMPLATE_IMPORT, m_ImportText);
		Serializer->mBuffer = Ogre::StringUtil::replaceAll(Serializer->mBuffer, temp, temp + " " + m_InheritanceText);
		
		//For be sure there's no any extera template in script!
		Serializer->mBuffer = Ogre::StringUtil::replaceAll(Serializer->mBuffer, MATERIAL_PARENT_TEMPLATE_TEXALIAS, "");
	}
}


void MaterialEditorControl::techniqueEventRaised(Ogre::MaterialSerializer *Serializer, Ogre::MaterialSerializer::SerializeEvent Event, bool &Skip, const Ogre::Technique *Technique)
{
	if (Event == Ogre::MaterialSerializer::MSE_PRE_WRITE)
		Serializer->writeAttribute(1, MATERIAL_PARENT_TEMPLATE_TEXALIAS);
}


void MaterialEditorControl::passEventRaised(Ogre::MaterialSerializer *Serializer, Ogre::MaterialSerializer::SerializeEvent Event, bool &Skip, const Ogre::Pass *Pass)
{
	Ogre::String texAlias = "";

	if (Pass->getName() == PASS_BUMP_NAME)
	{
		if (Event == Ogre::MaterialSerializer::MSE_PRE_WRITE)
		{
			m_ImportText = "import Bump from \"NormalBump.material\"\n";
			m_InheritanceText = ": Bump";

			m_IsThisBumpPass = true;

			texAlias = Ogre::String("set_texture_alias ") + TEXALIAS_DIFFUSE_NAME + " " + Pass->getTextureUnitState(0)->getTextureName() + "\n";

			if (Pass->getNumTextureUnitStates() > 1)
			{
				texAlias += "\t";
				texAlias += Ogre::String("set_texture_alias ") + TEXALIAS_NORMAL_NAME + " " + Pass->getTextureUnitState(1)->getTextureName() + "\n";
			}

			//if (Pass->getNumTextureUnitStates() > 2)
			//	texAlias += Ogre::String("\tset_texture_alias ") + TEXALIAS_SPECULAR_NAME + " " + Pass->getTextureUnitState(2)->getTextureName() + "\n";

			if (!Pass->getTextureUnitState(0)->getTextureName().size() || !Pass->getTextureUnitState(1)->getTextureName().size())
				texAlias = "";
		}
	}
	else
		m_IsThisBumpPass = false;

	Serializer->mBuffer = Ogre::StringUtil::replaceAll(Serializer->mBuffer, MATERIAL_PARENT_TEMPLATE_TEXALIAS, texAlias);
}


void MaterialEditorControl::gpuProgramRefEventRaised(Ogre::MaterialSerializer *Serializer, Ogre::MaterialSerializer::SerializeEvent Event, bool &Skip, const Ogre::String &Attribute, const Ogre::GpuProgramPtr &Program, const Ogre::GpuProgramParametersSharedPtr &Params, Ogre::GpuProgramParameters *DefaultParams)
{
	Skip = m_IsThisBumpPass;
}


void MaterialEditorControl::textureUnitStateEventRaised(Ogre::MaterialSerializer *Serializer, Ogre::MaterialSerializer::SerializeEvent Event, bool &Skip, const Ogre::TextureUnitState *TextureUnit)
{
	Skip = m_IsThisBumpPass;
}
END_NAMESPACE

#endif