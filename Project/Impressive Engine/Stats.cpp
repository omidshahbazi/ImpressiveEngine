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


#include "Stats.h"
#include "ScenesHolder.h"
#include "Gorilla.h"
#include "ViewPortsManager.h"
#include "RenderControl.h"
#include "Engine.h"
#include "Time.h"


#ifdef EDIT_MODE
#include "MainWindow.h"
#else
#include "CameraGameObject.h"
#endif



BEGIN_NAMESPACE

//Stats *Singleton<Stats>::m_Instance = NULL;


Stats::Stats(Ogre::Viewport *Viewport, Ogre::SceneManager *SceneManager) :
	m_Silverback(NULL),
	m_Screen(NULL),
	m_Layer(NULL),
	m_Text(NULL),
	m_ViewPort(Viewport)
{
	m_Silverback = Gorilla::Silverback::getSingletonPtr();
	if (!m_Silverback)
	{
		m_Silverback = new Gorilla::Silverback();
	
		m_Silverback->loadAtlas("dejavu");
	}


	m_Screen = m_Silverback->createScreen(Viewport, SceneManager, "dejavu");

	m_Layer = m_Screen->createLayer(0);

	m_Text = m_Layer->createMarkupText(9, 0, 0, "");
	//m_Text->align(Gorilla::TextAlign_Right);
	
	m_Text->left(10);
	m_Text->top(10);
	m_Text->width(500);
}


Stats::~Stats(void)
{
	m_Layer->destroyMarkupText(m_Text);
	m_Screen->destroy(m_Layer);
	m_Silverback->destroyScreen(m_Screen);
}


//void Stats::Initialize(void)
//{
//#ifdef EDIT_MODE
//	m_Silverback = new Gorilla::Silverback();
//	m_Silverback->loadAtlas("dejavu");
//
//	m_Screen = m_Silverback->createScreen(ViewPortsManager::GetInstance().GetViewPort(), ScenesHolder::GetInstance().GetCurrentScene()->GetSceneManager(), "dejavu");
//
//	m_Layer = m_Screen->createLayer(0);
//
//	m_Text = m_Layer->createMarkupText(9, 0, 0, "");
//	//m_Text->align(Gorilla::TextAlign_Right);
//	
//	m_Text->left(10);
//	m_Text->top(10);
//	m_Text->width(500);
//	
//	//m_Screen = m_Silverback->createScreen(ViewPortsManager::GetInstance().GetGameRenderControl()->GetViewport(), "dejavu");
//
//	//m_Layer = m_Screen->createLayer(0);
//
//	//m_Text = m_Layer->createMarkupText(9, 0, 0, "");
//	////m_Text->align(Gorilla::TextAlign_Right);
//	//
//	//m_Text->left(10);
//	//m_Text->top(10);
//	//m_Text->width(500);
//#endif
//}


void Stats::Update(void)
{
	if (!m_Text)
		return;

	//if (!ViewPortsManager::GetInstance().GetRenderWindow()->isActive())
	//	return;
	
	Ogre::RenderTarget::FrameStats stat;
	
	//if (MainWindow::GetPointer()->IsInEditMode())
	//{
		//stat = ViewPortsManager::GetInstance().GetRenderWindow()->getStatistics();
		//Ogre::Vector3 camPos = ViewPortsManager::GetInstance().GetCurrentCamera()->GetPosition();

	stat = m_ViewPort->getTarget()->getStatistics();

	Ogre::Vector3 camPos = Ogre::Vector3::ZERO;
	if (m_ViewPort->getCamera())
		camPos = m_ViewPort->getCamera()->getPosition();

		
	//}
	//else
	//{
	//	stat = ViewPortsManager::GetInstance().GetGameRenderControl()->GetRenderWindow()->getStatistics();
	//	camPos = ViewPortsManager::GetInstance().GetGameRenderControl()->GetOgreCamera()->getPosition();
	//}
	
	Ogre::String text;
	
	text += "Cam Pos : " + Ogre::StringConverter::toString(camPos) + "\n";

	text += Engine::GetInstance().GetGraphic()->getRenderSystem()->getName() + "\n";

	
	text += "Polys : ";
	if (stat.triangleCount >= 1000000)
		text += Ogre::StringConverter::toString(stat.triangleCount / 1000000.f) + "M";
	else if (stat.triangleCount >= 1000)
		text += Ogre::StringConverter::toString(stat.triangleCount / 1000.f) + "K";
	else
		text += Ogre::StringConverter::toString(stat.triangleCount);
	
	text += " Batches : " + Ogre::StringConverter::toString(stat.batchCount) + "\n";

	text += "FPS : " + Ogre::StringConverter::toString((int)stat.lastFPS);
	text += " (" + Ogre::StringConverter::toString((int)stat.worstFPS);
	text += " ... " + Ogre::StringConverter::toString((int)stat.bestFPS) + ")" + "\n";
	
	text += "Time : " + Ogre::StringConverter::toString(Time::GetInstance().GetTime());

	if (Time::GetInstance().GetTimeScale() != 1.f)
		text += " RealTime : " + Ogre::StringConverter::toString(Time::GetInstance().GetRealTime());

	//if (MainWindow::GetPointer()->IsInEditMode())
		m_Text->text(text);
	//else
	//	m_TextPlay->text(text);
}
END_NAMESPACE


#endif