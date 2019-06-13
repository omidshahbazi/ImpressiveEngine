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
#pragma once

#include "StdAfxOGRE.h"
#include "Singleton.h"
#include "Application.h"


BEGIN_NAMESPACE
class Engine : public Singleton<Engine>
{
public:
	//Engine(void);
	//~Engine(void);
	
	void InitializePaths(void);

	void InitializeResources(void);

	bool Initialize(void);
	
#ifndef EDIT_MODE
	void StartRendering(void);
#endif

	bool Tick(void);
	
	void Shutdown(void);

	Ogre::Root *GetGraphic(void)
	{
		return m_Graphic;
	}

private:
	bool InitializeGraphic(void);
	
#ifdef EDIT_MODE
	void PauseRendering(void);
	void ResumeRendering(void);
#endif

	void Update(void);
	void Render(void);
	
	bool LoadPlugIns(void);
	void SetRenderSystem(void);
	
	void AddResourceLocation(const Ogre::String &Path, const Ogre::String &GroupName = "Other", bool Recursive = false);

public:
	const float GetFPS(void) const;

private:
	Ogre::Root *m_Graphic;
	
	bool m_WasRenderingPaused;

public:
	static Ogre::String ENGINE_NAME;

	static Ogre::String PATH_DATA;

#ifdef DEBUG
	static Ogre::String PATH_EDITOR;
	static Ogre::String PATH_EDITOR_2DSPACE;
	static Ogre::String PATH_EDITOR_IMAGE;
	static Ogre::String PATH_EDITOR_TEXTURE;
#endif
	
	static Ogre::String PATH_AI;
	//static Ogre::String PATH_DEFERREDSHADING;
	static Ogre::String PATH_LEVEL;
	static Ogre::String PATH_MATERIAL;
	static Ogre::String PATH_MODEL;
	static Ogre::String PATH_OCEAN;
	static Ogre::String PATH_POSTPROCESS;
	static Ogre::String PATH_PARTICLE;
	static Ogre::String PATH_PLUGINS;
	static Ogre::String PATH_SCENE;
	static Ogre::String PATH_SCRIPT;
	static Ogre::String PATH_SKY;
	static Ogre::String PATH_TERRAIN;
	static Ogre::String PATH_TEXTURE;
	
#ifdef DEBUG
	static const char *RESOURCES_EDITOR;
#endif
	static const char *RESOURCES_DEFERREDSHADING;
	static const char *RESOURCES_CORE_TEXTURE;
	static const char *RESOURCES_CORE_MATERIAL;
	static const char *RESOURCES_TEXTURE;
	static const char *RESOURCES_MATERIAL;
	static const char *RESOURCES_MODEL;
	static const char *RESOURCES_OCEAN;
	static const char *RESOURCES_POSTPROCESS;
	static const char *RESOURCES_PARTICLE;
	static const char *RESOURCES_SKY;
	static const char *RESOURCES_TERRAIN;
};
END_NAMESPACE