
#include "Definitions.h"
#include "BulletSample.h"


extern "C" 
{
	IE_DLL_EXPORT IPlugin *LoadPlugin(void)
	{
		return new BulletSample();
	}
}