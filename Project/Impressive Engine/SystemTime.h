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

#include "Common.h"
#include <time.h>


BEGIN_NAMESPACE
class SystemTime
{
public:
	//SystemTime(void);
	//~SystemTime(void);

	static int &GetHours(void);
	static int &GetMinutes(void);
	static int &GetSeconds(void);

private:
	static tm m_TM;
	static time_t m_Time;
};
END_NAMESPACE