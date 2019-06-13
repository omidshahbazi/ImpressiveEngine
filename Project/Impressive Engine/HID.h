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
#pragma once

#include "Singleton.h"
#include "StdAfxOGRE.h"
#include "ViewPortsManager.h"
#include "ScriptRegister.h"

#ifdef EDIT_MODE
#include "StdAfxwxWidgets.h"

class RenderControlBase;
#else
#include "StdAfxOIS.h"
#endif

BEGIN_NAMESPACE
class HID : public Singleton<HID>
{
public:
	enum Keys
	{
		K_ESCAPE =
#ifdef EDIT_MODE
		WXK_SPACE
#else
		OIS::KC_ESCAPE
#endif
		,K_1 =
#ifdef EDIT_MODE
		'1'
#else
		OIS::KC_1
#endif
		,K_2 =
#ifdef EDIT_MODE
		'2'
#else
		OIS::KC_2
#endif
		,K_3 =
#ifdef EDIT_MODE
		'3'
#else
		OIS::KC_3
#endif
		,K_4 =
#ifdef EDIT_MODE
		'4'
#else
		OIS::KC_4
#endif
		,K_5 =
#ifdef EDIT_MODE
		'5'
#else
		OIS::KC_5
#endif
		,K_6 =
#ifdef EDIT_MODE
		'6'
#else
		OIS::KC_6
#endif
		,K_7 =
#ifdef EDIT_MODE
		'7'
#else
		OIS::KC_7
#endif
		,K_8 =
#ifdef EDIT_MODE
		'8'
#else
		OIS::KC_8
#endif
		,K_9 =
#ifdef EDIT_MODE
		'9'
#else
		OIS::KC_9
#endif
		,K_0 =
#ifdef EDIT_MODE
		'0'
#else
		OIS::KC_0
#endif
		,K_MINUS =
#ifdef EDIT_MODE
		WXK_SUBTRACT
#else
		OIS::KC_MINUS
#endif
		,K_EQUALS =
#ifdef EDIT_MODE
		WXK_ADD
#else
		OIS::KC_EQUALS
#endif
		,K_BACK =
#ifdef EDIT_MODE
		WXK_BACK
#else
		OIS::KC_BACK
#endif
		,K_TAB =
#ifdef EDIT_MODE
		WXK_TAB
#else
		OIS::KC_TAB
#endif
		,K_Q =
#ifdef EDIT_MODE
		'Q'
#else
		OIS::KC_Q
#endif
		,K_W =
#ifdef EDIT_MODE
		'W'
#else
		OIS::KC_W
#endif
		,K_E =
#ifdef EDIT_MODE
		'E'
#else
		OIS::KC_E
#endif
		,K_R =
#ifdef EDIT_MODE
		'R'
#else
		OIS::KC_R
#endif
		,K_T =
#ifdef EDIT_MODE
		'T'
#else
		OIS::KC_T
#endif
		,K_Y =
#ifdef EDIT_MODE
		'y'
#else
		OIS::KC_Y
#endif
		,K_U =
#ifdef EDIT_MODE
		'U'
#else
		OIS::KC_U
#endif
		,K_I =
#ifdef EDIT_MODE
		'I'
#else
		OIS::KC_I
#endif
		,K_O =
#ifdef EDIT_MODE
		'O'
#else
		OIS::KC_O
#endif
		,K_P =
#ifdef EDIT_MODE
		'P'
#else
		OIS::KC_P
#endif
		,K_LBRACKET =
#ifdef EDIT_MODE
		'['
#else
		OIS::KC_LBRACKET
#endif
		,K_RBRACKET =
#ifdef EDIT_MODE
		']'
#else
		OIS::KC_RBRACKET
#endif
		,K_RETURN =
#ifdef EDIT_MODE
		WXK_RETURN
#else
		OIS::KC_RETURN
#endif
		,K_LCONTROL =
#ifdef EDIT_MODE
		WXK_CONTROL
#else
		OIS::KC_LCONTROL
#endif
		,K_A =
#ifdef EDIT_MODE
		'A'
#else
		OIS::KC_A
#endif
		,K_S =
#ifdef EDIT_MODE
		'S'
#else
		OIS::KC_S
#endif
		,K_D =
#ifdef EDIT_MODE
		'D'
#else
		OIS::KC_D
#endif
		,K_F =
#ifdef EDIT_MODE
		'F'
#else
		OIS::KC_F
#endif
		,K_G =
#ifdef EDIT_MODE
		'G'
#else
		OIS::KC_G
#endif
		,K_H =
#ifdef EDIT_MODE
		'H'
#else
		OIS::KC_H
#endif
		,K_J =
#ifdef EDIT_MODE
		'J'
#else
		OIS::KC_J
#endif
		,K_K =
#ifdef EDIT_MODE
		'K'
#else
		OIS::KC_K
#endif
		,K_L =
#ifdef EDIT_MODE
		'L'
#else
		OIS::KC_L
#endif
		,K_SEMICOLON =
#ifdef EDIT_MODE
		';'
#else
		OIS::KC_SEMICOLON
#endif
		,K_APOSTROPHE =
#ifdef EDIT_MODE
		'\''
#else
		OIS::KC_APOSTROPHE
#endif
		,K_GRAVE =
#ifdef EDIT_MODE
		'`'
#else
		OIS::KC_GRAVE
#endif
		,K_LSHIFT =
#ifdef EDIT_MODE
		WXK_SHIFT
#else
		OIS::KC_LSHIFT
#endif
		,K_BACKSLASH =
#ifdef EDIT_MODE
		'\\'
#else
		OIS::KC_BACKSLASH
#endif
		,K_Z =
#ifdef EDIT_MODE
		'Z'
#else
		OIS::KC_Z
#endif
		,K_X =
#ifdef EDIT_MODE
		'X'
#else
		OIS::KC_X
#endif
		,K_C =
#ifdef EDIT_MODE
		'C'
#else
		OIS::KC_C
#endif
		,K_V =
#ifdef EDIT_MODE
		'V'
#else
		OIS::KC_V
#endif
		,K_B =
#ifdef EDIT_MODE
		'B'
#else
		OIS::KC_B
#endif
		,K_N =
#ifdef EDIT_MODE
		'N'
#else
		OIS::KC_N
#endif
		,K_M =
#ifdef EDIT_MODE
		'M'
#else
		OIS::KC_M
#endif
		,K_COMMA =
#ifdef EDIT_MODE
		','
#else
		OIS::KC_COMMA
#endif
		,K_PERIOD =
#ifdef EDIT_MODE
		'.'
#else
		OIS::KC_PERIOD
#endif
		,K_SLASH =
#ifdef EDIT_MODE
		'/'
#else
		OIS::KC_SLASH
#endif
		,K_RSHIFT =
#ifdef EDIT_MODE
		WXK_SHIFT
#else
		OIS::KC_RSHIFT
#endif
		,K_MULTIPLY =
#ifdef EDIT_MODE
		'*'
#else
		OIS::KC_MULTIPLY
#endif
		,K_LMENU =
#ifdef EDIT_MODE
		WXK_MENU
#else
		OIS::KC_LMENU
#endif
		,K_SPACE =
#ifdef EDIT_MODE
		WXK_SPACE
#else
		OIS::KC_SPACE
#endif
		,K_CAPITAL =
#ifdef EDIT_MODE
		WXK_CAPITAL
#else
		OIS::KC_CAPITAL
#endif
		,K_F1 =
#ifdef EDIT_MODE
		WXK_F1
#else
		OIS::KC_F1
#endif
		,K_F2 =
#ifdef EDIT_MODE
		WXK_F2
#else
		OIS::KC_F2
#endif
		,K_F3 =
#ifdef EDIT_MODE
		WXK_F3
#else
		OIS::KC_F3
#endif
		,K_F4 =
#ifdef EDIT_MODE
		WXK_F4
#else
		OIS::KC_F4
#endif
		,K_F5 =
#ifdef EDIT_MODE
		WXK_F5
#else
		OIS::KC_F5
#endif
		,K_F6 =
#ifdef EDIT_MODE
		WXK_F6
#else
		OIS::KC_F6
#endif
		,K_F7 =
#ifdef EDIT_MODE
		WXK_F7
#else
		OIS::KC_F7
#endif
		,K_F8 =
#ifdef EDIT_MODE
		WXK_F8
#else
		OIS::KC_F8
#endif
		,K_F9 =
#ifdef EDIT_MODE
		WXK_F9
#else
		OIS::KC_F9
#endif
		,K_F10 =
#ifdef EDIT_MODE
		WXK_F10
#else
		OIS::KC_F10
#endif
		,K_NUMLOCK =
#ifdef EDIT_MODE
		WXK_NUMLOCK
#else
		OIS::KC_NUMLOCK
#endif
		,K_SCROLL =
#ifdef EDIT_MODE
		WXK_SCROLL
#else
		OIS::KC_SCROLL
#endif
		,K_NUMPAD7 =
#ifdef EDIT_MODE
		WXK_NUMPAD7
#else
		OIS::KC_NUMPAD7
#endif
		,K_NUMPAD8 =
#ifdef EDIT_MODE
		WXK_NUMPAD8
#else
		OIS::KC_NUMPAD8
#endif
		,K_NUMPAD9 =
#ifdef EDIT_MODE
		WXK_NUMPAD9
#else
		OIS::KC_NUMPAD9
#endif
		,K_SUBTRACT =
#ifdef EDIT_MODE
		WXK_SUBTRACT
#else
		OIS::KC_SUBTRACT
#endif
		,K_NUMPAD4 =
#ifdef EDIT_MODE
		WXK_NUMPAD4
#else
		OIS::KC_NUMPAD4
#endif
		,K_NUMPAD5 =
#ifdef EDIT_MODE
		WXK_NUMPAD5
#else
		OIS::KC_NUMPAD5
#endif
		,K_NUMPAD6 =
#ifdef EDIT_MODE
		WXK_NUMPAD6
#else
		OIS::KC_NUMPAD6
#endif
		,K_ADD =
#ifdef EDIT_MODE
		WXK_ADD
#else
		OIS::KC_ADD
#endif
		,K_NUMPAD1 =
#ifdef EDIT_MODE
		WXK_NUMPAD1
#else
		OIS::KC_NUMPAD1
#endif
		,K_NUMPAD2 =
#ifdef EDIT_MODE
		WXK_NUMPAD2
#else
		OIS::KC_NUMPAD2
#endif
		,K_NUMPAD3 =
#ifdef EDIT_MODE
		WXK_NUMPAD3
#else
		OIS::KC_NUMPAD3
#endif
		,K_NUMPAD0 =
#ifdef EDIT_MODE
		WXK_NUMPAD0
#else
		OIS::KC_NUMPAD0
#endif
		,K_DECIMAL =
#ifdef EDIT_MODE
		WXK_DECIMAL
#else
		OIS::KC_DECIMAL
#endif
		,K_OEM_102 =
#ifdef EDIT_MODE
		'?'// I don't know what's OEM 102 ?! THERE'S ANOTHER LIKE THIS!
#else
		OIS::KC_OEM_102
#endif
		,K_F11 =
#ifdef EDIT_MODE
		WXK_F11
#else
		OIS::KC_F11
#endif
		,K_F12 =
#ifdef EDIT_MODE
		WXK_F12
#else
		OIS::KC_F12
#endif
		,K_F13 =
#ifdef EDIT_MODE
		WXK_F13
#else
		OIS::KC_F13
#endif
		,K_F14 =
#ifdef EDIT_MODE
		WXK_F14
#else
		OIS::KC_F14
#endif
		,K_F15 =
#ifdef EDIT_MODE
		WXK_F15
#else
		OIS::KC_F15
#endif
		,K_KANA =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_KANA
#endif
		,K_ABNT_C1 =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_ABNT_C1
#endif
		,K_CONVERT =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_CONVERT
#endif
		,K_NOCONVERT =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_NOCONVERT
#endif
		,K_YEN =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_YEN
#endif
		,K_ABNT_C2 =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_ABNT_C2
#endif
		,K_NUMPADEQUALS =
#ifdef EDIT_MODE
		WXK_NUMPAD_EQUAL
#else
		OIS::KC_NUMPADEQUALS
#endif
		,K_PREVTRACK =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_PREVTRACK
#endif
		,K_AT =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_AT
#endif
		,K_COLON =
#ifdef EDIT_MODE
		':'
#else
		OIS::KC_COLON
#endif
		,K_UNDERLINE =
#ifdef EDIT_MODE
		'_'
#else
		OIS::KC_UNDERLINE
#endif
		,K_KANJI =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_KANJI
#endif
		,K_STOP =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_STOP
#endif
		,K_AX =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_AX
#endif
		,K_UNLABELED =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_UNLABELED
#endif
		,K_NEXTTRACK =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_NEXTTRACK
#endif
		,K_NUMPADENTER =
#ifdef EDIT_MODE
		WXK_NUMPAD_ENTER
#else
		OIS::KC_NUMPADENTER
#endif
		,K_RCONTROL =
#ifdef EDIT_MODE
		WXK_CONTROL
#else
		OIS::KC_RCONTROL
#endif
		,K_MUTE =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_MUTE
#endif
		,K_CALCULATOR =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_CALCULATOR
#endif
		,K_PLAYPAUSE =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_PLAYPAUSE
#endif
		,K_MEDIASTOP =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_MEDIASTOP
#endif
		,K_VOLUMEDOWN =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_VOLUMEDOWN
#endif
		,K_VOLUMEUP =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_VOLUMEUP
#endif
		,K_WEBHOME =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_WEBHOME
#endif
		,K_NUMPADCOMMA =
#ifdef EDIT_MODE
		WXK_NUMPAD_SEPARATOR
#else
		OIS::KC_NUMPADCOMMA
#endif
		,K_DIVIDE =
#ifdef EDIT_MODE
		WXK_NUMPAD_DIVIDE
#else
		OIS::KC_DIVIDE
#endif
		,K_SYSRQ =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_SYSRQ
#endif
		,K_RMENU =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_RMENU
#endif
		,K_PAUSE =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_PAUSE
#endif
		,K_HOME =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_HOME
#endif
		,K_UP =
#ifdef EDIT_MODE
		WXK_UP
#else
		OIS::KC_UP
#endif
		,K_PGUP =
#ifdef EDIT_MODE
		WXK_PAGEUP
#else
		OIS::KC_PGUP
#endif
		,K_LEFT =
#ifdef EDIT_MODE
		WXK_LEFT
#else
		OIS::KC_LEFT
#endif
		,K_RIGHT =
#ifdef EDIT_MODE
		WXK_RIGHT
#else
		OIS::KC_RIGHT
#endif
		,K_END =
#ifdef EDIT_MODE
		WXK_END
#else
		OIS::KC_END
#endif
		,K_DOWN =
#ifdef EDIT_MODE
		WXK_DOWN
#else
		OIS::KC_DOWN
#endif
		,K_PGDOWN =
#ifdef EDIT_MODE
		WXK_PAGEDOWN
#else
		OIS::KC_PGDOWN
#endif
		,K_INSERT =
#ifdef EDIT_MODE
		WXK_INSERT
#else
		OIS::KC_INSERT
#endif
		,K_DELETE =
#ifdef EDIT_MODE
		WXK_DELETE
#else
		OIS::KC_DELETE
#endif
		,K_LWIN =
#ifdef EDIT_MODE
		WXK_WINDOWS_LEFT
#else
		OIS::KC_LWIN
#endif
		,K_RWIN =
#ifdef EDIT_MODE
		WXK_WINDOWS_RIGHT
#else
		OIS::KC_RWIN
#endif
		,K_APPS =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_APPS
#endif
		,K_POWER =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_APPS
#endif
		,K_SLEEP =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_SLEEP
#endif
		,K_WAKE =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_WAKE
#endif
		,K_WEBSEARCH =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_WEBSEARCH
#endif
		,K_WEBFAVORITES =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_WEBFAVORITES
#endif
		,K_WEBREFRESH =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_WEBREFRESH
#endif
		,K_WEBSTOP =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_WEBSTOP
#endif
		,K_WEBFORWARD =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_WEBFORWARD
#endif
		,K_WEBBACK =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_WEBBACK
#endif
		,K_MYCOMPUTER =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_MYCOMPUTER
#endif
		,K_MAIL =
#ifdef EDIT_MODE
		'?'
#else
		OIS::KC_MAIL
#endif
	};

	//M_RIGHT is defined as #define M_RIGHT
	enum MouseButtons
	{
		M_LEFT =
#ifdef EDIT_MODE
		wxMOUSE_BTN_LEFT
#else
		OIS::MB_Left
#endif
		,M_MIDDLE =
#ifdef EDIT_MODE
		wxMOUSE_BTN_MIDDLE
#else
		OIS::MB_Middle
#endif
		,M_RIGHT =
#ifdef EDIT_MODE
		wxMOUSE_BTN_RIGHT
#else
		OIS::MB_Right
#endif
		,M_Button3 =
#ifdef EDIT_MODE
		4
#else
		OIS::MB_Button3
#endif
		,M_Button4 =
#ifdef EDIT_MODE
		5
#else
		OIS::MB_Button4
#endif
		,M_Button5 =
#ifdef EDIT_MODE
		6
#else
		OIS::MB_Button5
#endif
		,M_Button6 =
#ifdef EDIT_MODE
		7
#else
		OIS::MB_Button6
#endif
		,M_Button7 =
#ifdef EDIT_MODE
		8
#else
		OIS::MB_Button7
#endif
	};		

public:
	HID(void);
	//~HID(void);

	void Initialize(void);

#ifdef EDIT_MODE
	void UpdateKeyboard(const wxKeyEvent &event);
	//void UpdateKeyboard(unsigned int KeyCode, bool IsDown, bool IsControlDown, bool IsShiftDown, bool IsAltDown);
	void UpdateMouse(const wxMouseEvent &event);
#endif

	void Update(void);

	void SetCursorVisible(bool Value);
	
	void SetNormalizedMousePosition(float X, float Y);
	void SetNormalizedMousePosition(Ogre::Vector2 &Position);
	Ogre::Vector2 GetNormalizedMousePosition(void) const;
	
	Ogre::Vector2 GetMousePositionRelative(void) const;
	
	void SetMousePosition(int X, int Y);
	void SetMousePosition(const Ogre::Vector2 &Position);
	Ogre::Vector2 GetMousePosition(void) const;

	int GetMouseWheel(void) const;

	bool IsMouseDown(/*int*/MouseButtons Button) const;
	bool WasMouseButtonPressed(MouseButtons Button) const;

	bool IsKeyDown(/*int*/Keys Key) const;
	bool WasKeyPressed(/*int Key*/Keys Key) const;
    bool IsControlDown(void) const;
    bool IsShiftDown(void) const;
    bool IsAltDown(void) const;

#ifdef EDIT_MODE
	void SetActiveRenderControl(RenderControlBase *Control)
	{
		m_LastActiveRenderControl = Control;
	}

	bool IsThisActive(RenderControlBase *Control) const
	{
		return (m_LastActiveRenderControl == Control);
	}
#endif

	DECLARE_SCRIPT_REGISTATION()

private:
	bool m_KeysWasDown[255];
	bool m_KeysWasPressed[255];
	
	bool m_MouseButtonWasDown[3];
	bool m_MouseButtonWasPressed[3];

#ifdef EDIT_MODE
	bool m_KeysIsDown[255];
	bool m_ControlState, m_ShiftState, m_AltState;
	wxMouseEvent m_Mouse;
	int m_MouseWheel;
	Ogre::Vector2 m_LastMousePosition;

	RenderControlBase *m_LastActiveRenderControl;
#else
	OIS::InputManager* m_InputManager;
	OIS::Keyboard *m_Keyboard;
	OIS::Mouse *m_Mouse;
#endif
};
END_NAMESPACE