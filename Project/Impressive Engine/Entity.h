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

#pragma once

#include "Singleton.h"
#include "EntityControl.h"

BEGIN_NAMESPACE
class Entity : public Singleton<Entity>
{
public:
	//Entity(void);
	//~Entity(void);

	void Initialize(wxWindow *Parent);
	void Refresh(void);

	wxString GetName(void)
	{
		return "Entity";
	}
	
	wxString GetCaption(void)
	{
		return "Entity";
	}

	EntityControl *GetControl(void)
	{
		return m_Control;
	}

private:
	EntityControl *m_Control;
};
END_NAMESPACE

#endif