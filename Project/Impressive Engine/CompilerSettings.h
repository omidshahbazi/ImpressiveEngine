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
#ifdef EDIT_MODE

//
// wxWidgets
//
#pragma comment(lib, "wxmsw28d_core.lib")
#pragma comment(lib, "wxmsw28d_adv.lib")
#pragma comment(lib, "wxmsw28d_aui.lib")
#pragma comment(lib, "wxmsw28d_propgrid.lib")
#pragma comment(lib, "wxzlibd.lib")
#pragma comment(lib, "wxregexd.lib")
#pragma comment(lib, "wxpngd.lib")
#pragma comment(lib, "wxtiffd.lib")
#pragma comment(lib, "wxjpegd.lib")

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "comctl32.lib")


//
// Scintilla
//
#pragma comment(lib, "Scintilla.lib")

#pragma comment(lib, "imm32.lib")


//
// Theora
//
#pragma comment(lib, "libtheora_static.lib")

#pragma comment(lib, "libogg_static.lib")

#else

//
// OIS
//
#pragma comment(lib, "OIS_d.lib")
#endif


//
// OGRE
//
#pragma comment(lib, "OgreMain_d.lib")
#pragma comment(lib, "OgreTerrain_d.lib")
#pragma comment(lib, "Hydrax_d.lib")
#pragma comment(lib, "SkyX_d.lib")


//
// Lua
//
#pragma comment(lib, "Lua.lib")
#pragma comment(lib, "LuaBind.lib")


//
// wxWidgets
//
#pragma comment(lib, "wxbase28d.lib")

#pragma comment(lib, "winmm.lib")



// warning C4244: 'argument' : conversion from 'NUMBER' to 'NUMBER', possible loss of data
#pragma warning(disable : 4244)

// Doesn't affected
// warning C4172: returning address of local variable or temporary
// http://msdn.microsoft.com/en-us/library/d0ws2xs1%28v=vs.71%29.aspx
// Compile with: /W1 /LD
//float f = 10;
//const double& bar() {
// Try the following line instead
// const float& bar() {
//   return f;
//}
//#pragma warning(disable : 4172)