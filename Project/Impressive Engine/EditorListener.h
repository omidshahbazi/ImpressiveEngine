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

#include "StdAfxOGRE.h"
#include "Common.h"

BEGIN_NAMESPACE
class EditorListener
{
public:
	//EditorListener(void);
	//~EditorListener(void);

	virtual void OnTranslate(const Ogre::Vector3 &Value);
	virtual void OnScale(const Ogre::Vector3 &Value);
	virtual void OnRotate(const Ogre::Vector3 &Value);
};
END_NAMESPACE

#endif