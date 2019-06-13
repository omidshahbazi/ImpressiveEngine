
#include "PhysicsFramework.h"


void PhysicsFramework::Go(IPhysicsSample *Sample)
{
	InitializeOgreRoot();

	if (!m_OgreRoot->showConfigDialog())
		return;

	m_Window = m_OgreRoot->initialise(true, "Physics Framework");
	m_Window->getCustomAttribute("WINDOW", &m_WindowHandle);

	m_InputManager = OIS::InputManager::createInputSystem((size_t)m_WindowHandle);
	m_Keyboard = static_cast<OIS::Keyboard*>(m_InputManager->createInputObject(OIS::OISKeyboard, true));
	m_Mouse = static_cast<OIS::Mouse*>(m_InputManager->createInputObject(OIS::OISMouse, true));
	
	m_Camera = m_Scene->createCamera("Camera1");
	m_Camera->setNearClipDistance(0.01f);
	m_Camera->setPosition(-100, 50, 500);
	m_Camera->lookAt(0, 0, 0);
	m_Camera->setAutoAspectRatio(true);


	Ogre::Light *light = m_Scene->createLight();
	light->setDiffuseColour(0.1f, 0.1f, 0.1f);
	light->setSpecularColour(0.25, 1.0, 1.0);
	light->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3::UNIT_X);

	light = m_Scene->createLight();
	light->setDiffuseColour(0.1f, 0.1f, 0.1f);
	light->setSpecularColour(0.25, 1.0, 1.0);
	light->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3::UNIT_Y);

	light = m_Scene->createLight();
	light->setDiffuseColour(0.1f, 0.1f, 0.1f);
	light->setSpecularColour(0.25, 1.0, 1.0);
	light->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3::UNIT_Z);


	m_Viewport = m_Window->addViewport(m_Camera);

	new Ogre::DebugDrawer(m_Scene, 0.5f);

	Sample->InitializeRendering(m_Scene, m_Camera, Ogre::DebugDrawer::getSingletonPtr());

	Sample->InitializePhysics();

	m_IsEndReached = false;

	m_LastUpdateTime = 0;

	unsigned long currTime;

	while (!m_Window->isClosed() && !m_IsEndReached)
	{
		Ogre::WindowEventUtilities::messagePump();

		m_Keyboard->capture();
		m_Mouse->capture();

		HandleInput();
		Sample->HandleInput(m_Keyboard, m_Mouse);

		Ogre::DebugDrawer::getSingleton().clear();

		currTime = m_OgreRoot->getTimer()->getMilliseconds();

		Sample->Update(currTime - m_LastUpdateTime);

		m_LastUpdateTime = currTime;
		
		Ogre::DebugDrawer::getSingleton().build();

		m_OgreRoot->renderOneFrame();

		SetWindowText(m_WindowHandle, (Ogre::String("FPS : ") + Ogre::StringConverter::toString(m_Window->getLastFPS())).c_str());
	}
}


void PhysicsFramework::DrawLine(const Ogre::Vector3 &From, const Ogre::Vector3 &To, const Ogre::ColourValue &Color)
{
	Ogre::DebugDrawer::getSingleton().drawLine(From, To, Color);
}


void PhysicsFramework::HandleInput(void)
{
	if (m_Keyboard->isKeyDown(OIS::KC_ESCAPE))
		m_IsEndReached = true;

	static const float TranslateStep = 0.1f;
	static const float RotateStep = 0.09f;
	Ogre::Vector3 translate = Ogre::Vector3::ZERO;

	if (m_Keyboard->isKeyDown(OIS::KC_W))
		translate.z -= TranslateStep;
	if (m_Keyboard->isKeyDown(OIS::KC_S))
		translate.z += TranslateStep;
	if (m_Keyboard->isKeyDown(OIS::KC_A))
		translate.x -= TranslateStep;
	if (m_Keyboard->isKeyDown(OIS::KC_D))
		translate.x += TranslateStep;

	m_Camera->moveRelative(translate);

	m_Camera->pitch(Ogre::Degree(-m_Mouse->getMouseState().Y.rel * RotateStep));
	m_Camera->yaw(Ogre::Degree(-m_Mouse->getMouseState().X.rel * RotateStep));
}


void PhysicsFramework::InitializeOgreRoot(void)
{
	m_OgreRoot = new Ogre::Root("");

	m_OgreRoot->loadPlugin("RenderSystem_Direct3D9_d.dll");

	m_OgreRoot->setRenderSystem(*m_OgreRoot->getAvailableRenderers().begin());

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/", "FileSystem");
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		
	m_Scene = m_OgreRoot->createSceneManager(Ogre::ST_GENERIC);
}