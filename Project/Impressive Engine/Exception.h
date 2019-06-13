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
#include <OgreString.h>
#include <OgreException.h>

BEGIN_NAMESPACE
class Exception : public Singleton<Exception>
{
public:
	enum ExceptionReason
	{
		ER_INVALID_PARAMETER = 0,
		ER_FILE_NOT_FOUND,
		ER_UNKNOWN
	};

public:
	//Exception(void);
	//~Exception(void);
	
	void Throw(Ogre::Exception &Exception, bool Resume = true);
	void Throw(const ExceptionReason &Reason, const Ogre::String &Title, const Ogre::String &Text, bool Resume = true);
};


END_NAMESPACE