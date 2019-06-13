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
#include "HID.h"
#include "ViewPortsManager.h"
#include "ScriptRegister.h"
#include <Windows.h>

#ifdef EDIT_MODE
#include "RenderControl.h"
#include "MainWindow.h"
#endif


BEGIN_NAMESPACE
HID *Singleton<HID>::m_Instance = NULL;


HID::HID(void)
#ifdef EDIT_MODE 
	: m_MouseWheel(0),
	m_LastActiveRenderControl(NULL)
#endif
{
}

//HID::~HID(void)
//{
//}


void HID::Initialize(void)
{
	for (unsigned int i = 0; i < 255; i++)
	{
#ifdef EDIT_MODE
		m_KeysIsDown[i] = false;
#endif
		m_KeysWasDown[i] = false;
		m_KeysWasPressed[i] = false;
	}
	
#ifdef EDIT_MODE
	m_ControlState = false;
	m_ShiftState = false;
	m_AltState = false;
#else
	//Engine::GetInstance().GetGraphic()->addFrameListener(this);

	m_InputManager = OIS::InputManager::createInputSystem((size_t)ViewPortsManager::GetInstance().GetHWND());

	//std::ostringstream hWndString;
	//hWndString << hWnd;

	//OIS::ParamList param;
	//param.insert(std::make_pair("WINDOW", hWndString.str()));
	//param.insert(std::make_pair("w32_mouse", "DISCL_FOREGROUND"));
	//param.insert(std::make_pair("w32_mouse", "DISCL_NONEXCLUSIVE"));
	//param.insert(std::make_pair("w32_keyboard", "DISCL_FOREGROUND"));
	//param.insert(std::make_pair("w32_keyboard", "DISCL_NONEXCLUSIVE"));

	//hidManager = OIS::InputManager::createInputSystem(param);

	m_Keyboard = static_cast<OIS::Keyboard*>(m_InputManager->createInputObject(OIS::OISKeyboard, false));
	//m_Keyboard->setEventCallback(this);


	m_Mouse = static_cast<OIS::Mouse*>(m_InputManager->createInputObject(OIS::OISMouse, false));
	//m_Mouse->setEventCallback(this);
#endif
}


#ifdef EDIT_MODE
void HID::UpdateKeyboard(const wxKeyEvent &event)
//void HID::UpdateKeyboard(unsigned int KeyCode, bool IsDown, bool IsControlDown, bool IsShiftDown, bool IsAltDown)
{
	m_KeysIsDown[event.GetKeyCode()] = (event.GetEventType() == wxEVT_KEY_DOWN);

	m_ControlState = event.ControlDown();
	m_ShiftState = event.ShiftDown();
	m_AltState = event.AltDown();	
}


void HID::UpdateMouse(const wxMouseEvent &event)
{
	m_LastMousePosition = Ogre::Vector2(event.GetPosition().x - m_Mouse.GetPosition().x, event.GetPosition().y - m_Mouse.GetPosition().y);

	m_Mouse = event;

	m_MouseWheel = 0;
}
#endif


void HID::Update(void)
{
	bool isDown = true;
	unsigned int code = 0;
	for (code = 0; code < 255; code++)
	{
		isDown = IsKeyDown((Keys)code);
	
		if (m_KeysWasDown[code])
		{
			if (!isDown)
			{
				m_KeysWasDown[code] = false;
				m_KeysWasPressed[code] = true;
			}
		}
		else
		{
			m_KeysWasPressed[code] = false;
		}
		
		m_KeysWasDown[code] = isDown;
	}
	
	for (code = 0; code < 3; code++)
	{
		isDown = IsMouseDown((MouseButtons)
#ifdef EDIT_MODE
			code
#else
			(code + 1)
#endif
			);
	
		if (m_MouseButtonWasDown[code])
		{
			if (!isDown)
			{
				m_MouseButtonWasDown[code] = false;
				m_MouseButtonWasPressed[code] = true;
			}
		}
		else
		{
			m_MouseButtonWasPressed[code] = false;
		}
		
		m_MouseButtonWasDown[code] = isDown;
	}

#ifndef EDIT_MODE
	m_Keyboard->capture();
	m_Mouse->capture();

	m_Mouse->getMouseState().width = ViewPortsManager::GetInstance().GetRenderWindow()->getWidth();
	m_Mouse->getMouseState().height = ViewPortsManager::GetInstance().GetRenderWindow()->getHeight();
#endif
}


void HID::SetCursorVisible(bool Value)
{
	ShowCursor(Value);
}


void HID::SetNormalizedMousePosition(float X, float Y)
{
#ifdef EDIT_MODE
	if (m_LastActiveRenderControl)
	{
		X *=  m_LastActiveRenderControl->GetSize().x;
		Y *=  m_LastActiveRenderControl->GetSize().y;
		//return Ogre::Vector2((float)m_Mouse.GetX() / ViewPortsManager::GetInstance().GetRenderControl()->GetSize().x, (float)m_Mouse.GetY() / ViewPortsManager::GetInstance().GetRenderControl()->GetSize().y);
	}

	X += 1;
	Y += 31.499983787878787878f;

	if (!MainWindow::GetPointer()->IsMaximized())
		X += MainWindow::GetPointer()->GetPosition().x;
		Y += MainWindow::GetPointer()->GetPosition().y;

#else
	X *= ViewPortsManager::GetInstance().GetRenderWindow()->getWidth();
	Y *= ViewPortsManager::GetInstance().GetRenderWindow()->getHeight();
#endif

	SetMousePosition(X, Y);
}


void HID::SetNormalizedMousePosition(Ogre::Vector2 &Position)
{
	SetNormalizedMousePosition(Position.x, Position.y);
}


Ogre::Vector2 HID::GetNormalizedMousePosition(void) const
{
#ifdef EDIT_MODE
	if (m_LastActiveRenderControl)
		return Ogre::Vector2((float)m_Mouse.GetX() / m_LastActiveRenderControl->GetSize().x, (float)m_Mouse.GetY() / m_LastActiveRenderControl->GetSize().y);
		//return Ogre::Vector2((float)m_Mouse.GetX() / ViewPortsManager::GetInstance().GetRenderControl()->GetSize().x, (float)m_Mouse.GetY() / ViewPortsManager::GetInstance().GetRenderControl()->GetSize().y);
	return Ogre::Vector2(-1);
#else
	//return Ogre::Vector2((float)m_Mouse->getMouseState().X.abs / ViewPortsManager::GetInstance().GetRenderWindow()->getWidth(), 
	//	(float)m_Mouse->getMouseState().Y.abs / ViewPortsManager::GetInstance().GetRenderWindow()->getHeight());
	POINT pos;
	GetCursorPos(&pos);

	return Ogre::Vector2((float)pos.x / ViewPortsManager::GetInstance().GetRenderWindow()->getWidth(), 
		(float)pos.y / ViewPortsManager::GetInstance().GetRenderWindow()->getHeight());
#endif
}


Ogre::Vector2 HID::GetMousePositionRelative(void) const
{
#ifdef EDIT_MODE
	return m_LastMousePosition;
#else
	return Ogre::Vector2(m_Mouse->getMouseState().X.rel, m_Mouse->getMouseState().Y.rel);
#endif
}


void HID::SetMousePosition(int X, int Y)
{
#ifdef EDIT_MODE
	m_LastMousePosition.x = X;
	m_LastMousePosition.y = Y;
#endif

	SetCursorPos(X, Y);
}


void HID::SetMousePosition(const Ogre::Vector2 &Position)
{
	SetMousePosition(Position.x, Position.y);
}


Ogre::Vector2 HID::GetMousePosition(void) const
{
#ifdef EDIT_MODE
	return Ogre::Vector2(m_Mouse.GetX(), m_Mouse.GetY());
#else
	//return Ogre::Vector2(m_Mouse->getMouseState().X.abs, m_Mouse->getMouseState().Y.abs);
	POINT pos;
	GetCursorPos(&pos);
	return Ogre::Vector2(pos.x, pos.y);
#endif
}


int HID::GetMouseWheel(void) const
{
#ifdef EDIT_MODE
	return m_MouseWheel;
#else
	return m_Mouse->getMouseState().Z.rel;
#endif
}


bool HID::IsMouseDown(/*int*/MouseButtons Button) const
{
#ifdef EDIT_MODE
	switch (Button)
	{
	case M_LEFT:
	case M_MIDDLE:
	case M_RIGHT:
	case M_Button3:
	case M_Button4:
	case M_Button5:
	case M_Button6:
	case M_Button7:
		return m_Mouse.ButtonIsDown(/*(wxMouseButton)*/Button);
		break;

	default:
		return false;
	}
#else
	return (m_Mouse->getMouseState().buttonDown((OIS::MouseButtonID)Button));
#endif
}


bool HID::WasMouseButtonPressed(MouseButtons Button) const
{
	return m_MouseButtonWasPressed[Button];
}


bool HID::IsKeyDown(/*int*/Keys Key) const
{
#ifdef EDIT_MODE
	return m_KeysIsDown[Key];
#else
	return m_Keyboard->isKeyDown((OIS::KeyCode)Key);
#endif
}

	
bool HID::WasKeyPressed(Keys Key) const
{
	return m_KeysWasPressed[Key];
}


bool HID::IsControlDown(void) const
{
#ifdef EDIT_MODE
	return m_ControlState;
#else
	return m_Keyboard->isModifierDown(OIS::Keyboard::Ctrl);
#endif
}


bool HID::IsShiftDown(void) const
{
#ifdef EDIT_MODE
	return m_ShiftState;
#else
	return m_Keyboard->isModifierDown(OIS::Keyboard::Shift);
#endif
}


bool HID::IsAltDown(void) const
{
#ifdef EDIT_MODE
	return m_AltState;
#else
	return m_Keyboard->isModifierDown(OIS::Keyboard::Alt);
#endif
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(HID)
	BEGIN_DEFINE_ENUMERATOR(HID::Keys, "Keys")
		DEFINE_ENUMERATOR_VALUE("K_ESCAPE", HID::K_ESCAPE),
		DEFINE_ENUMERATOR_VALUE("K_1", HID::K_1),
		DEFINE_ENUMERATOR_VALUE("K_2", HID::K_2),
		DEFINE_ENUMERATOR_VALUE("K_3", HID::K_3),
		DEFINE_ENUMERATOR_VALUE("K_4", HID::K_4),
		DEFINE_ENUMERATOR_VALUE("K_5", HID::K_5),
		DEFINE_ENUMERATOR_VALUE("K_6", HID::K_6),
		DEFINE_ENUMERATOR_VALUE("K_7", HID::K_7),
		DEFINE_ENUMERATOR_VALUE("K_8", HID::K_8),
		DEFINE_ENUMERATOR_VALUE("K_9", HID::K_9),
		DEFINE_ENUMERATOR_VALUE("K_0", HID::K_0),
		DEFINE_ENUMERATOR_VALUE("K_MINUS", HID::K_MINUS),
		DEFINE_ENUMERATOR_VALUE("K_EQUALS", HID::K_EQUALS),
		DEFINE_ENUMERATOR_VALUE("K_BACK", HID::K_BACK),
		DEFINE_ENUMERATOR_VALUE("K_BACK", HID::K_BACK),
		DEFINE_ENUMERATOR_VALUE("K_Q", HID::K_Q),
		DEFINE_ENUMERATOR_VALUE("K_W", HID::K_W),
		DEFINE_ENUMERATOR_VALUE("K_E", HID::K_E),
		DEFINE_ENUMERATOR_VALUE("K_R", HID::K_R),
		DEFINE_ENUMERATOR_VALUE("K_T", HID::K_T),
		DEFINE_ENUMERATOR_VALUE("K_Y", HID::K_Y),
		DEFINE_ENUMERATOR_VALUE("K_U", HID::K_U),
		DEFINE_ENUMERATOR_VALUE("K_I", HID::K_I),
		DEFINE_ENUMERATOR_VALUE("K_O", HID::K_O),
		DEFINE_ENUMERATOR_VALUE("K_P", HID::K_P),
		DEFINE_ENUMERATOR_VALUE("K_LBRACKET", HID::K_LBRACKET),
		DEFINE_ENUMERATOR_VALUE("K_RBRACKET", HID::K_RBRACKET),
		DEFINE_ENUMERATOR_VALUE("K_RETURN", HID::K_RETURN),
		DEFINE_ENUMERATOR_VALUE("K_LCONTROL", HID::K_LCONTROL),
		DEFINE_ENUMERATOR_VALUE("K_A", HID::K_A),
		DEFINE_ENUMERATOR_VALUE("K_S", HID::K_S),
		DEFINE_ENUMERATOR_VALUE("K_D", HID::K_D),
		DEFINE_ENUMERATOR_VALUE("K_F", HID::K_F),
		DEFINE_ENUMERATOR_VALUE("K_G", HID::K_G),
		DEFINE_ENUMERATOR_VALUE("K_H", HID::K_H),
		DEFINE_ENUMERATOR_VALUE("K_J", HID::K_J),
		DEFINE_ENUMERATOR_VALUE("K_K", HID::K_K),
		DEFINE_ENUMERATOR_VALUE("K_L", HID::K_L),
		DEFINE_ENUMERATOR_VALUE("K_SEMICOLON", HID::K_SEMICOLON),
		DEFINE_ENUMERATOR_VALUE("K_APOSTROPHE", HID::K_APOSTROPHE),
		DEFINE_ENUMERATOR_VALUE("K_GRAVE", HID::K_GRAVE),
		DEFINE_ENUMERATOR_VALUE("K_LSHIFT", HID::K_LSHIFT),
		DEFINE_ENUMERATOR_VALUE("K_BACKSLASH", HID::K_BACKSLASH),
		DEFINE_ENUMERATOR_VALUE("K_X", HID::K_X),
		DEFINE_ENUMERATOR_VALUE("K_C", HID::K_C),
		DEFINE_ENUMERATOR_VALUE("K_V", HID::K_V),
		DEFINE_ENUMERATOR_VALUE("K_B", HID::K_B),
		DEFINE_ENUMERATOR_VALUE("K_N", HID::K_N),
		DEFINE_ENUMERATOR_VALUE("K_M", HID::K_M),
		DEFINE_ENUMERATOR_VALUE("K_COMMA", HID::K_COMMA),
		DEFINE_ENUMERATOR_VALUE("K_PERIOD", HID::K_PERIOD),
		DEFINE_ENUMERATOR_VALUE("K_SLASH", HID::K_SLASH),
		DEFINE_ENUMERATOR_VALUE("K_RSHIFT", HID::K_RSHIFT),
		DEFINE_ENUMERATOR_VALUE("K_MULTIPLY", HID::K_MULTIPLY),
		DEFINE_ENUMERATOR_VALUE("K_LMENU", HID::K_LMENU),
		DEFINE_ENUMERATOR_VALUE("K_SPACE", HID::K_SPACE),
		DEFINE_ENUMERATOR_VALUE("K_CAPITAL", HID::K_CAPITAL),
		DEFINE_ENUMERATOR_VALUE("K_F1", HID::K_F1),
		DEFINE_ENUMERATOR_VALUE("K_F2", HID::K_F2),
		DEFINE_ENUMERATOR_VALUE("K_F3", HID::K_F3),
		DEFINE_ENUMERATOR_VALUE("K_F4", HID::K_F4),
		DEFINE_ENUMERATOR_VALUE("K_F5", HID::K_F5),
		DEFINE_ENUMERATOR_VALUE("K_F6", HID::K_F6),
		DEFINE_ENUMERATOR_VALUE("K_F7", HID::K_F7),
		DEFINE_ENUMERATOR_VALUE("K_F8", HID::K_F8),
		DEFINE_ENUMERATOR_VALUE("K_F9", HID::K_F9),
		DEFINE_ENUMERATOR_VALUE("K_F10", HID::K_F10),
		DEFINE_ENUMERATOR_VALUE("K_NUMLOCK", HID::K_NUMLOCK),
		DEFINE_ENUMERATOR_VALUE("K_SCROLL", HID::K_SCROLL),
		DEFINE_ENUMERATOR_VALUE("K_NUMPAD7", HID::K_NUMPAD7),
		DEFINE_ENUMERATOR_VALUE("K_NUMPAD8", HID::K_NUMPAD8),
		DEFINE_ENUMERATOR_VALUE("K_NUMPAD9", HID::K_NUMPAD9),
		DEFINE_ENUMERATOR_VALUE("K_SUBTRACT", HID::K_SUBTRACT),
		DEFINE_ENUMERATOR_VALUE("K_NUMPAD4", HID::K_NUMPAD4),
		DEFINE_ENUMERATOR_VALUE("K_NUMPAD5", HID::K_NUMPAD5),
		DEFINE_ENUMERATOR_VALUE("K_NUMPAD6", HID::K_NUMPAD6),
		DEFINE_ENUMERATOR_VALUE("K_ADD", HID::K_ADD),
		DEFINE_ENUMERATOR_VALUE("K_NUMPAD1", HID::K_NUMPAD1),
		DEFINE_ENUMERATOR_VALUE("K_NUMPAD2", HID::K_NUMPAD2),
		DEFINE_ENUMERATOR_VALUE("K_NUMPAD3", HID::K_NUMPAD3),
		DEFINE_ENUMERATOR_VALUE("K_NUMPAD0", HID::K_NUMPAD0),
		DEFINE_ENUMERATOR_VALUE("K_DECIMAL", HID::K_DECIMAL),
		DEFINE_ENUMERATOR_VALUE("K_OEM_102", HID::K_OEM_102),
		DEFINE_ENUMERATOR_VALUE("K_F11", HID::K_F11),
		DEFINE_ENUMERATOR_VALUE("K_F12", HID::K_F12),
		DEFINE_ENUMERATOR_VALUE("K_F13", HID::K_F13),
		DEFINE_ENUMERATOR_VALUE("K_F14", HID::K_F14),
		DEFINE_ENUMERATOR_VALUE("K_F15", HID::K_F15),
		DEFINE_ENUMERATOR_VALUE("K_KANA", HID::K_KANA),
		DEFINE_ENUMERATOR_VALUE("K_ABNT_C1", HID::K_ABNT_C1),
		DEFINE_ENUMERATOR_VALUE("K_CONVERT", HID::K_CONVERT),
		DEFINE_ENUMERATOR_VALUE("K_NOCONVERT", HID::K_NOCONVERT),
		DEFINE_ENUMERATOR_VALUE("K_YEN", HID::K_YEN),
		DEFINE_ENUMERATOR_VALUE("K_ABNT_C2", HID::K_ABNT_C2),
		DEFINE_ENUMERATOR_VALUE("K_NUMPADEQUALS", HID::K_NUMPADEQUALS),
		DEFINE_ENUMERATOR_VALUE("K_PREVTRACK", HID::K_PREVTRACK),
		DEFINE_ENUMERATOR_VALUE("K_AT", HID::K_AT),
		DEFINE_ENUMERATOR_VALUE("K_COLON", HID::K_COLON),
		DEFINE_ENUMERATOR_VALUE("K_UNDERLINE", HID::K_UNDERLINE),
		DEFINE_ENUMERATOR_VALUE("K_KANJI", HID::K_KANJI),
		DEFINE_ENUMERATOR_VALUE("K_STOP", HID::K_STOP),
		DEFINE_ENUMERATOR_VALUE("K_AX", HID::K_AX),
		DEFINE_ENUMERATOR_VALUE("K_UNLABELED", HID::K_UNLABELED),
		DEFINE_ENUMERATOR_VALUE("K_NEXTTRACK", HID::K_NEXTTRACK),
		DEFINE_ENUMERATOR_VALUE("K_NUMPADENTER", HID::K_NUMPADENTER),
		DEFINE_ENUMERATOR_VALUE("K_RCONTROL", HID::K_RCONTROL),
		DEFINE_ENUMERATOR_VALUE("K_MUTE", HID::K_MUTE),
		DEFINE_ENUMERATOR_VALUE("K_CALCULATOR", HID::K_CALCULATOR),
		DEFINE_ENUMERATOR_VALUE("K_PLAYPAUSE", HID::K_PLAYPAUSE),
		DEFINE_ENUMERATOR_VALUE("K_MEDIASTOP", HID::K_MEDIASTOP),
		DEFINE_ENUMERATOR_VALUE("K_VOLUMEDOWN", HID::K_VOLUMEDOWN),
		DEFINE_ENUMERATOR_VALUE("K_VOLUMEUP", HID::K_VOLUMEUP),
		DEFINE_ENUMERATOR_VALUE("K_WEBHOME", HID::K_WEBHOME),
		DEFINE_ENUMERATOR_VALUE("K_NUMPADCOMMA", HID::K_NUMPADCOMMA),
		DEFINE_ENUMERATOR_VALUE("K_DIVIDE", HID::K_DIVIDE),
		DEFINE_ENUMERATOR_VALUE("K_SYSRQ", HID::K_SYSRQ),
		DEFINE_ENUMERATOR_VALUE("K_RMENU", HID::K_RMENU),
		DEFINE_ENUMERATOR_VALUE("K_PAUSE", HID::K_PAUSE),
		DEFINE_ENUMERATOR_VALUE("K_HOME", HID::K_HOME),
		DEFINE_ENUMERATOR_VALUE("K_UP", HID::K_UP),
		DEFINE_ENUMERATOR_VALUE("K_PGUP", HID::K_PGUP),
		DEFINE_ENUMERATOR_VALUE("K_LEFT", HID::K_LEFT),
		DEFINE_ENUMERATOR_VALUE("K_RIGHT", HID::K_RIGHT),
		DEFINE_ENUMERATOR_VALUE("K_END", HID::K_END),
		DEFINE_ENUMERATOR_VALUE("K_DOWN", HID::K_DOWN),
		DEFINE_ENUMERATOR_VALUE("K_PGDOWN", HID::K_PGDOWN),
		DEFINE_ENUMERATOR_VALUE("K_INSERT", HID::K_INSERT),
		DEFINE_ENUMERATOR_VALUE("K_DELETE", HID::K_DELETE),
		DEFINE_ENUMERATOR_VALUE("K_LWIN", HID::K_LWIN),
		DEFINE_ENUMERATOR_VALUE("K_RWIN", HID::K_RWIN),
		DEFINE_ENUMERATOR_VALUE("K_APPS", HID::K_APPS),
		DEFINE_ENUMERATOR_VALUE("K_POWER", HID::K_POWER),
		DEFINE_ENUMERATOR_VALUE("K_SLEEP", HID::K_SLEEP),
		DEFINE_ENUMERATOR_VALUE("K_WAKE", HID::K_WAKE),
		DEFINE_ENUMERATOR_VALUE("K_WEBSEARCH", HID::K_WEBSEARCH),
		DEFINE_ENUMERATOR_VALUE("K_WEBFAVORITES", HID::K_WEBFAVORITES),
		DEFINE_ENUMERATOR_VALUE("K_WEBREFRESH", HID::K_WEBREFRESH),
		DEFINE_ENUMERATOR_VALUE("K_WEBSTOP", HID::K_WEBSTOP),
		DEFINE_ENUMERATOR_VALUE("K_WEBFORWARD", HID::K_WEBFORWARD),
		DEFINE_ENUMERATOR_VALUE("K_WEBBACK", HID::K_WEBBACK),
		DEFINE_ENUMERATOR_VALUE("K_MYCOMPUTER", HID::K_MYCOMPUTER),
		DEFINE_ENUMERATOR_VALUE("K_MAIL", HID::K_MAIL)
	END_DEFINE_ENUMERATOR()
		
	BEGIN_DEFINE_ENUMERATOR(HID::MouseButtons, "MouseButtons")
		DEFINE_ENUMERATOR_VALUE("MB_LEFT", HID::M_LEFT),
		DEFINE_ENUMERATOR_VALUE("MB_MIDDLE", HID::M_MIDDLE),
		DEFINE_ENUMERATOR_VALUE("MB_RIGHT", HID::M_RIGHT),
		DEFINE_ENUMERATOR_VALUE("MB_Button3", HID::M_Button3),
		DEFINE_ENUMERATOR_VALUE("MB_Button4", HID::M_Button4),
		DEFINE_ENUMERATOR_VALUE("MB_Button5", HID::M_Button5),
		DEFINE_ENUMERATOR_VALUE("MB_Button6", HID::M_Button6),
		DEFINE_ENUMERATOR_VALUE("MB_Button6", HID::M_Button6),
		DEFINE_ENUMERATOR_VALUE("MB_Button7", HID::M_Button7)
	END_DEFINE_ENUMERATOR()
		
	BEGIN_DEFINE_TYPE(HID, "HID")
		DEFINE_FUNCTION("SetCursorVisible", &HID::SetCursorVisible)

		DEFINE_FUNCTION("SetNormalizedMousePosition", (void(HID::*)(float, float))&HID::SetNormalizedMousePosition)
		DEFINE_FUNCTION("SetNormalizedMousePosition", (void(HID::*)(Ogre::Vector2&))&HID::SetNormalizedMousePosition)
		DEFINE_FUNCTION("GetNormalizedMousePosition", &HID::GetNormalizedMousePosition)

		DEFINE_FUNCTION("GetMousePositionRelative", &HID::GetMousePositionRelative)
		
		DEFINE_FUNCTION("SetMousePosition", (void(HID::*)(int, int))&HID::SetMousePosition)
		DEFINE_FUNCTION("SetMousePosition", (void(HID::*)(const Ogre::Vector2&))&HID::SetMousePosition)
		DEFINE_FUNCTION("GetMousePosition", &HID::GetMousePosition)

		DEFINE_FUNCTION("GetMouseWheel", &HID::GetMouseWheel)
		DEFINE_FUNCTION("IsMouseDown", &HID::IsMouseDown)
		DEFINE_FUNCTION("IsKeyDown", &HID::IsKeyDown)
		
		DEFINE_FUNCTION("WasMouseButtonPressed", &HID::WasMouseButtonPressed)
		DEFINE_FUNCTION("WasKeyPressed", &HID::WasKeyPressed)

		DEFINE_FUNCTION("IsControlDown", &HID::IsControlDown)
		DEFINE_FUNCTION("IsShiftDown", &HID::IsShiftDown)
		DEFINE_FUNCTION("IsAltDown", &HID::IsAltDown)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE