#include "Definitions.h"
#include "PhysXSample.h"

extern "C"
	IE_DLL_EXPORT IPlugin *LoadPlugin(void)
	{
		return new PhysXSample();
	}