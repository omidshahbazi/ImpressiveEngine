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
#pragma once

#include "Singleton.h"
#include <vector>

BEGIN_NAMESPACE
class IPlugin;

class PluginManager : public Singleton<PluginManager>
{
public:
	//PluginManager(void);
	//~PluginManager(void);
	
	void LoadPlugin(const char *FileName);

private:
	std::vector<IPlugin*> m_Plugins;
};
END_NAMESPACE