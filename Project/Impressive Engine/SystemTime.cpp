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
#include "SystemTime.h"


BEGIN_NAMESPACE
//const int HOURS_IN_DAY = 24;
//const int MINUTES_IN_HOUR = 60;
//const int SECONDS_IN_MINUTE = 60;


tm SystemTime::m_TM;
time_t SystemTime::m_Time;


//SystemTime::SystemTime(void)
//{
//}
//
//
//SystemTime::~SystemTime(void)
//{
//}


int &SystemTime::GetHours(void)
{
	localtime_s(&m_TM, &m_Time);

	return m_TM.tm_hour;
}


int &SystemTime::GetMinutes(void)
{
	localtime_s(&m_TM, &m_Time);

	return m_TM.tm_min;
}


int &SystemTime::GetSeconds(void)
{
	localtime_s(&m_TM, &m_Time);

	return m_TM.tm_sec;
}
END_NAMESPACE