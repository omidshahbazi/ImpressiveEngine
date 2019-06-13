#pragma once

#include "IPlugin.h"

class PluginManager
{
public:
	IPlugin *LoadPlugin(const char *FileName);
};

