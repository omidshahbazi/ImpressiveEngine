/*/////////////////////////////////////////////////////////////////////////////////
///
///  Impressive Engine
///
/// Copyright (c) 2012-2013 Impressive Reality team
///
/// The license is
///
/// Permission is denied, to any person or company
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// Project leader : O.Shahbazi <sh_omid_m@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////*/
#include "PluginManager.h"
#include "Exception.h"
#include <Windows.h>


BEGIN_NAMESPACE
PluginManager *Singleton<PluginManager>::m_Instance = NULL;

typedef IPlugin* (*LoadPluginFunction)(void);


//PluginManager::PluginManager(void)
//{
//}


//PluginManager::~PluginManager(void)
//{
//}


void PluginManager::LoadPlugin(const char *FileName)
{
	HINSTANCE hInst = LoadLibraryEx(FileName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	if (hInst)
	{
		LoadPluginFunction func = (LoadPluginFunction)GetProcAddress(hInst, "LoadPlugin");
	
		func();
	}
	else
		Exception::GetInstance().Throw(Exception::ER_FILE_NOT_FOUND, "Invalid Plugin File Name", Ogre::String("There's no plugin file with name [") + FileName + "]");
}
END_NAMESPACE