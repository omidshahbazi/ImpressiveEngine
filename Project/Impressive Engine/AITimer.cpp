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
#include "AITimer.h"
#include "Time.h"


BEGIN_NAMESPACE
AITimer::AITimer(unsigned int Interval) : m_Interval(Interval),
										  m_CurrentTime(Time::GetInstance().GetRealTime())
{
}


const bool AITimer::IsReady(void)
{
	if (Time::GetInstance().GetRealTime() - m_CurrentTime > m_Interval)
	{
		m_CurrentTime = Time::GetInstance().GetRealTime();
		return true;
	}

	return false;
}

END_NAMESPACE