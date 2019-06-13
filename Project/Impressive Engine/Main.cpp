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

#include "Application.h"

#ifdef EDIT_MODE

#ifdef DEBUG

IMPLEMENT_APP_CONSOLE(Application);

#else

IMPLEMENT_APP(Application);

#endif

#else

#include <Windows.h>

#ifdef DEBUG

int main(int argc, const char **argv)

#else

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)

#endif

{
	Application app;

#ifdef DEBUG
	return app.Go(GetModuleHandle(0));
#else
	return app.Go(instance);
#endif
}

#endif