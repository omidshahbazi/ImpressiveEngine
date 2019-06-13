#include "PluginManager.h"
#include <Windows.h>

typedef IPlugin* (*LoadPluginFunction)(void);


IPlugin *PluginManager::LoadPlugin(const char *FileName)
{
	HINSTANCE hInst = LoadLibraryEx(FileName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	if (hInst)
	{
		LoadPluginFunction func = (LoadPluginFunction)GetProcAddress(hInst, "LoadPlugin");
	
		if (func)
			return func();
	}

	return NULL;
}