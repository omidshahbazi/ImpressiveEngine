
#include "PluginManager.h"
#include "PhysicsFramework.h"
#include <stdio.h>

void main(void)
{
	FILE *file = fopen("Plugin.cfg", "r");

	if (file)
	{
		char str[255];
		unsigned int size = fread_s(&str, 255, 1, 255, file);
		str[size] = '\0';

		PluginManager pm;
		IPlugin *plugin = pm.LoadPlugin(str);

		if (plugin)
		{
			std::cout << plugin->GetName() << " v" << plugin->GetVersion() << " is loaded\n";

			PhysicsFramework pf;
			pf.Go(dynamic_cast<IPhysicsSample*>(plugin));
		}
	}
}