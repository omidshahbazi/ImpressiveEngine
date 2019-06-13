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

#include "StdAfxwxWidgets.h"
#include "Common.h"

BEGIN_NAMESPACE
class IPropertiesListener
{
public:
	//IPropertiesListener(void);
	//~IPropertiesListener(void);

	virtual void OnPropertyChanged(/*wxPropertyGridManager *Manager, */const wxPropertyGridEvent &event) = 0;
};
END_NAMESPACE


#endif
