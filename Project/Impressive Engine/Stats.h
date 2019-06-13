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
#ifdef DEBUG


#pragma once

//#include "Singleton.h"
#include "Common.h"
#include "StdAfxOGRE.h"


namespace Gorilla
{
	class Silverback;
	class Screen;
	class Layer;
	class MarkupText;
}


BEGIN_NAMESPACE
class Stats/* : public Singleton<Stats>*/
{
public:
	Stats(Ogre::Viewport *Viewport, Ogre::SceneManager *SceneManager);
	~Stats(void);

	//void Initialize(void);

	void Update(void);

private:
	Gorilla::Silverback *m_Silverback;
	Gorilla::Screen *m_Screen;
	Gorilla::Layer *m_Layer;
	Gorilla::MarkupText *m_Text;

	Ogre::Viewport *m_ViewPort;
};
END_NAMESPACE


#endif