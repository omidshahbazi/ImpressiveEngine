
#include <Ogre.h>
#include <OIS.h>
#include "IPhysicsSample.h"


class PhysicsFramework
{
public:
	void Go(IPhysicsSample *Sample);

protected:
	PseudoGameObject *AddGameObject(const Ogre::String &Name, const Ogre::String &MeshName, const Ogre::Vector3 &Position = Ogre::Vector3::ZERO, const Ogre::Vector3 &Rotation = Ogre::Vector3::ZERO, const Ogre::Vector3 &OriginOffset = Ogre::Vector3::ZERO);

	void DrawLine(const Ogre::Vector3 &From, const Ogre::Vector3 &To, const Ogre::ColourValue &Color);

	virtual void HandleInput(void);

private:
	void InitializeOgreRoot(void);

protected:
	Ogre::Root *m_OgreRoot;
	Ogre::RenderWindow *m_Window;
	Ogre::SceneManager *m_Scene;
	Ogre::Camera *m_Camera;
	Ogre::Viewport *m_Viewport;

	OIS::InputManager *m_InputManager;
	OIS::Keyboard *m_Keyboard;
	OIS::Mouse *m_Mouse;

private:
	HWND m_WindowHandle;

	unsigned long m_LastUpdateTime;

protected:
	bool m_IsEndReached;

};