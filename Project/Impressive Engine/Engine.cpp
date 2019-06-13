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
#include "Engine.h"

#include "StdAfxOGRE.h"
#include "VirtualMachine.h"
#include "HID.h"
#include "NodeBase.h"
#include "ViewPortsManager.h"
#include "RenderControl.h"
#include "GameObject.h"
#include "ScenesHolder.h"
#include "Entity.h"
#include "Utility.h"
#include "Time.h"
#include "Exception.h"
#include "Log.h"
#include "ManualFunctionality.h"
#include "FrameListener.h"
#include "DSCommon.h"

#ifdef EDIT_MODE
#include "StdAfxwxWidgets.h"
#include "AnimationControl.h"
#include "Editor.h"
#include "MaterialEditorControl.h"
#include "ModelEditorControl.h"
#include "DebugDrawer.h"
#else
#include "CameraGameObject.h"
#endif

#ifdef DEBUG
#include "Stats.h"
#endif

#include "AIEngine.h"

BEGIN_NAMESPACE
Ogre::String Engine::ENGINE_NAME = "";

Ogre::String Engine::PATH_DATA = "";

#ifdef DEBUG
Ogre::String Engine::PATH_EDITOR = "";
Ogre::String Engine::PATH_EDITOR_2DSPACE = "";
Ogre::String Engine::PATH_EDITOR_IMAGE = "";
Ogre::String Engine::PATH_EDITOR_TEXTURE = "";
#endif

Ogre::String Engine::PATH_AI = "";
//Ogre::String Engine::PATH_DEFERREDSHADING = "";
Ogre::String Engine::PATH_LEVEL = "";
Ogre::String Engine::PATH_MATERIAL = "";
Ogre::String Engine::PATH_MODEL = "";
Ogre::String Engine::PATH_OCEAN = "";
Ogre::String Engine::PATH_POSTPROCESS = "";
Ogre::String Engine::PATH_PARTICLE = "";
Ogre::String Engine::PATH_PLUGINS = "";
Ogre::String Engine::PATH_SCENE = "";
Ogre::String Engine::PATH_SCRIPT = "";
Ogre::String Engine::PATH_SKY = "";
Ogre::String Engine::PATH_TERRAIN = "";
Ogre::String Engine::PATH_TEXTURE = "";

#ifdef DEBUG
const char *Engine::RESOURCES_EDITOR("EditorResources");
#endif
const char *Engine::RESOURCES_DEFERREDSHADING("DeferredShadingResources");
const char *Engine::RESOURCES_CORE_TEXTURE("CoreTextureResources");
const char *Engine::RESOURCES_CORE_MATERIAL("CoreMaterialResources");
const char *Engine::RESOURCES_TEXTURE("TextureResources");
const char *Engine::RESOURCES_MATERIAL("MaterialResources");
const char *Engine::RESOURCES_MODEL("ModelResources");
const char *Engine::RESOURCES_OCEAN("Hydrax");
const char *Engine::RESOURCES_POSTPROCESS("PostProcessResources");
const char *Engine::RESOURCES_PARTICLE("ParticleResources");
const char *Engine::RESOURCES_SKY("SkyX");
const char *Engine::RESOURCES_TERRAIN("TerrainResources");


//const Ogre::String RENDER_SYSTEM("Direct3D9 Rendering Subsystem");


Engine *Singleton<Engine>::m_Instance = NULL;


//Engine::Engine(void)
//{
//}
//
//
//Engine::~Engine(void)
//{
//}

	
void Engine::InitializePaths(void)
{
	ENGINE_NAME = "Impressive Engine";

	PATH_DATA = STRING_WX_TO_OGRE(Application::EXECUTABLE_PATH + "Data\\");

#ifdef DEBUG
	PATH_EDITOR = PATH_DATA + "Editor\\";
	PATH_EDITOR_2DSPACE = PATH_EDITOR + "2DSpace\\";
	PATH_EDITOR_IMAGE = PATH_EDITOR + "Image\\";
	PATH_EDITOR_TEXTURE = PATH_EDITOR + "Texture\\";
#endif

	PATH_AI = PATH_DATA + "AI\\";
	//PATH_DEFERREDSHADING = PATH_DATA + "DeferredShading\\";
	PATH_LEVEL = PATH_DATA + "Level\\";
	PATH_MATERIAL = PATH_DATA + "Material\\";
	PATH_MODEL = PATH_DATA + "Model\\";
	PATH_OCEAN = PATH_DATA + "Ocean\\";
	PATH_POSTPROCESS = PATH_DATA + "PostProcess\\";
	PATH_PARTICLE = PATH_DATA + "Particle\\";
	PATH_PLUGINS = PATH_DATA + "PlugIns\\";
	PATH_SCENE = PATH_DATA + "Scene\\";
	PATH_SCRIPT = PATH_DATA + "Script\\";
	PATH_SKY = PATH_DATA + "Sky\\";
	PATH_TERRAIN = PATH_DATA + "Terrain\\";
	PATH_TEXTURE = PATH_DATA + "Texture\\";
}


void Engine::InitializeResources(void)
{
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	//std::vector<Ogre::String> files = GetFiles(PATH_MATERIAL + "Core\\");

	//File *file = NULL;
	//for (unsigned int i = 0; i < files.size(); i++)
	//{
	//	file = new File(files[i]);

	//	Ogre::DataStreamPtr mem = Ogre::MemoryDataStreamPtr(OGRE_NEW Ogre::MemoryDataStream(file->ReadText().c_str(), file->GetSize()));

	//	Ogre::ScriptCompilerManager::getSingleton().parseScript(mem, RESOURCES_CORE_MATERIAL);
	//}

#ifdef DEBUG
	AddResourceLocation(PATH_EDITOR_2DSPACE, RESOURCES_EDITOR);
	AddResourceLocation(PATH_EDITOR_TEXTURE, RESOURCES_EDITOR);

	AddResourceLocation(PATH_DATA + "OgreCore\\", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
#endif
	
	AddResourceLocation(PATH_TEXTURE + "Core\\", RESOURCES_CORE_TEXTURE, true);
	AddResourceLocation(PATH_MATERIAL + "Core\\", RESOURCES_CORE_MATERIAL);
	
	//AddResourceLocation(PATH_DEFERREDSHADING + "GBuffer\\", RESOURCES_DEFERREDSHADING);
	//AddResourceLocation(PATH_DEFERREDSHADING + "Lighting\\", RESOURCES_DEFERREDSHADING);
	
	AddResourceLocation(PATH_MATERIAL + "Core\\", RESOURCES_CORE_MATERIAL);
	AddResourceLocation(PATH_POSTPROCESS + "Core\\Texture\\", RESOURCES_POSTPROCESS);
	AddResourceLocation(PATH_POSTPROCESS + "Core\\Shader\\", RESOURCES_POSTPROCESS);
	AddResourceLocation(PATH_POSTPROCESS + "Core\\Material\\", RESOURCES_POSTPROCESS);
	AddResourceLocation(PATH_POSTPROCESS, RESOURCES_POSTPROCESS);
	
	AddResourceLocation(PATH_PARTICLE, RESOURCES_PARTICLE);

	AddResourceLocation(PATH_TEXTURE, RESOURCES_TEXTURE, true);
	AddResourceLocation(PATH_MATERIAL, RESOURCES_MATERIAL);
	AddResourceLocation(PATH_MODEL, RESOURCES_MODEL);
	
	AddResourceLocation(PATH_SKY, RESOURCES_SKY);
	
	AddResourceLocation(PATH_TERRAIN, RESOURCES_TERRAIN);
	
	AddResourceLocation(PATH_OCEAN, RESOURCES_OCEAN);
	
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	DSCommon::CreateResources();
}


bool Engine::Initialize(void)
{
#ifndef EDIT_MODE
	Log::GetInstance().Initialize();
	//Scene::GetInstance().Initialize();
#endif

	if (InitializeGraphic())
	{
#ifdef EDIT_MODE
		Editor::GetInstance().Initialize();
#endif
		return true;
	}
	else
		return false;
}


#ifndef EDIT_MODE
void Engine::StartRendering(void)
{
	do
	{
		Ogre::WindowEventUtilities::messagePump();

	} while (Tick());
}
#endif


bool Engine::Tick(void)
{
	if (m_WasRenderingPaused)
#ifdef EDIT_MODE
		return true;
#else
		return false;
#endif

	if (m_Graphic->isInitialised())
	{
		if (ViewPortsManager::GetInstance().GetRenderWindow()->isClosed())
			return false;

#ifndef EDIT_MODE
		if (!ViewPortsManager::GetInstance().GetRenderWindow()->isActive())
			return true;
#endif

		HID::GetInstance().Update();
		
#ifdef DEBUG
		if (HID::GetInstance().IsControlDown() && HID::GetInstance().WasKeyPressed(HID::K_P))
		{
			Ogre::Profiler &profiler = Ogre::Profiler::getSingleton();
			profiler.setEnabled(!profiler.getEnabled());
		}
#endif

#ifdef EDIT_MODE
		ViewPortsManager::GetInstance().Update();
#endif

		Time::GetInstance().Tick();

		Update();
		Render();

		return true;
	}

	return false;
}


void Engine::Shutdown(void)
{
	m_WasRenderingPaused = true;
}

//#include "DeferredShadingSystem.h"

bool Engine::InitializeGraphic(void)
{
	bool result = true;

	m_Graphic = new Ogre::Root("", ""
#ifndef DEBUG
		,""
#endif
		);

#ifdef DEBUG
	Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_BOREME);
#else
	Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_LOW);
#endif

	if (LoadPlugIns())
	{
		SetRenderSystem();

		try
		{
			m_Graphic->initialise(false);
		}
		catch (Ogre::Exception &e)
		{
			Exception::GetInstance().Throw(e, false);

			return false;
		}

		VirtualMachine::GetInstance().Initialize();
		
		ViewPortsManager::GetInstance().Initialize();

		ManualFunctionality::InitializeManualMeshes();
		
		InitializeResources();
		
#ifdef EDIT_MODE
		ScenesHolder::GetInstance().InitializeScene();
#endif

		FrameListener::GetInstance().Initialize();

		HID::GetInstance().Initialize();

#ifdef EDIT_MODE
		ViewPortsManager::GetInstance().InitializeCameras();

		//ScenesHolder::GetInstance().GetCurrentScene()->InitializeDSS();

#ifdef DEBUG
		ScenesHolder::GetInstance().GetCurrentScene()->InitializeStats();
#endif

		MaterialEditorControl::GetPointer()->Initialize();

		ModelEditorControl::GetPointer()->Initialize();
		
#ifdef EDIT_MODE
		AnimationControl::GetPointer()->Initialize();
#endif

		//ScenesHolder::GetInstance().GetCurrentScene()->UpdateDSS();
#endif

		result = m_Graphic->renderOneFrame();

		if (result)
			Log::GetInstance().AddInfo("Engine graphic successfully initialized");
		
#ifdef DEBUG
		//Stats::GetInstance().Initialize();
#endif

//		GameObject *obj = 0;
//		for (unsigned int x = 0; x < 10; x++)
//		{
//			for (unsigned int y = 0; y < 10; y++)
//			{
//				obj = Scene::GetInstance().CreateGameObject(Ogre::String("Name") + Ogre::StringConverter::toString(x * y));
//
//				obj->AddEntity("batman.mesh");
//
//				obj->SetPosition(x * 150, y * 200, 0);
//
//#ifdef EDIT_MODE
//				ViewPortsManager::GetInstance().GetCurrentCamera()->SetPosition(708, 850, 2610);
//				ViewPortsManager::GetInstance().GetCurrentCamera()->LookAt(708, 850, 0);
//#else
//				ViewPortsManager::GetInstance().SetPosition(708, 850, 2610);
//				ViewPortsManager::GetInstance().GetCamera()->LookAt(708, 850, 2610);
//#endif
//
//				obj->GetParent()->RemoveChild(obj);
//				obj->SetParent(Scene::GetInstance().GetRootGameObject());
//				Scene::GetInstance().GetRootGameObject()->AddChild(obj);
//			}
//		}

		return true;
	}

	return false;
}


#ifdef EDIT_MODE
void Engine::PauseRendering(void)
{
	m_WasRenderingPaused = true;
}


void Engine::ResumeRendering(void)
{
	m_WasRenderingPaused = false;
}
#endif


void Engine::Update(void)
{
#ifdef EDIT_MODE
	DebugDrawer::GetInstance().BeginUpdate();
#endif
	
	ScenesHolder::GetInstance().Update();
	AIEngine::GetInstance().Update();

//#ifdef EDIT_MODE
//	BEGIN_LIST_CONST_ITERATOR(ViewPortsManager::GetInstance().GetCameras(), EditorCamera)
//		current->Update();
//	END_LIST_CONST_ITERATOR
//#else
//#ifndef EDIT_MODE
//	ViewPortsManager::GetInstance().GetCamera()->Update();
//#endif


#ifdef EDIT_MODE
	AnimationControl::GetPointer()->Update();

	MaterialEditorControl::GetPointer()->Update();

	ModelEditorControl::GetPointer()->Update();

	DebugDrawer::GetInstance().EndUpdate();
#endif
}


void Engine::Render(void)
{
#ifdef DEBUG
	try 
	{
#endif
		bool success = m_Graphic->renderOneFrame();
		
#ifdef DEBUG
		if (!success)
		{
			Log::GetInstance().AddError("Rendering frame failed.");
		}
	}
	catch(Ogre::Exception &e)
	{
		Exception::GetInstance().Throw(e);
	}
#endif
		
//#ifdef EDIT_MODE
//	BEGIN_LIST_CONST_ITERATOR(ViewPortsManager::GetInstance().GetCameras(), EditorCamera)
//		current->Render();
//	END_LIST_CONST_ITERATOR
//#else
//#ifndef EDIT_MODE
//	ViewPortsManager::GetInstance().GetCamera()->Render();
//#endif
}


bool Engine::LoadPlugIns(void)
{
	try
	{
		m_Graphic->loadPlugin(PATH_PLUGINS + "RenderSystem_Direct3D9_d");
		//m_Graphic->loadPlugin(PATH_PLUGINS + "RenderSystem_Direct3D10_d");
		//m_Graphic->loadPlugin(PATH_PLUGINS + "RenderSystem_GL_d");
	}
	catch (Ogre::Exception &e)
	{
		Exception::GetInstance().Throw(e, false);

		return false;
	}
	
	m_Graphic->loadPlugin(PATH_PLUGINS + "Plugin_ParticleFX_d");
	m_Graphic->loadPlugin(PATH_PLUGINS + "Plugin_CgProgramManager_d");

	return true;
}


void Engine::SetRenderSystem(void)
{
	m_Graphic->setRenderSystem(*m_Graphic->getAvailableRenderers().begin());

	//const Ogre::RenderSystemList &list = m_Graphic->getAvailableRenderers();
	//Ogre::RenderSystemList::const_iterator it = list.begin();
	//Ogre::RenderSystem *system = 0;

	//while (it != list.end())
	//{
	//	system = *(it++);

	//	if (system->getName() == RENDER_SYSTEM) 
	//	{
	//		m_Graphic->setRenderSystem(system);
	//		break;
	//	}
	//}
}


void Engine::AddResourceLocation(const Ogre::String &Path, const Ogre::String &GroupName, bool Recursive)
{
	Ogre::String path = Path;

	if (!Ogre::StringUtil::endsWith(Path, "/") && !Ogre::StringUtil::endsWith(Path, "\\"))
		path = Path + "/";

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", GroupName, Recursive);
	
	//Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(GroupName);

#ifdef EDIT_MODE
	Entity::GetInstance().Refresh();
#endif
}


const float Engine::GetFPS(void) const
{
#ifdef EDIT_MODE
	return ViewPortsManager::GetInstance().GetGameRenderControl()->GetViewport()->getTarget()->getLastFPS();
#else
	return ViewPortsManager::GetInstance().GetViewPort()->getTarget()->getLastFPS();
#endif
}
END_NAMESPACE