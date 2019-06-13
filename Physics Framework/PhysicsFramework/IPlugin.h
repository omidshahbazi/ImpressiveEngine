#pragma once

class IPlugin
{
public:
	virtual void Install(void) = 0;

	virtual const char *GetName(void) const = 0;
	
	virtual const char *GetVersion(void) const = 0;
};