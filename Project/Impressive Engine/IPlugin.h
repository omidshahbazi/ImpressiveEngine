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

#include "Common.h"


BEGIN_NAMESPACE
class IPlugin
{
public:
	virtual void Install(void) = 0;

	virtual const char *GetName(void) const = 0;
	
	virtual const char *GetVersion(void) const = 0;
};
END_NAMESPACE

#define LOAD_PLUGIN_DEFINITION extern "C" __declspec(dllexport) IE::IPlugin *LoadPlugin(void)