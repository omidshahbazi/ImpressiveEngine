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
#include "Time.h"
#include "ScriptRegister.h"


BEGIN_NAMESPACE
Time *Singleton<Time>::m_Instance = NULL;

Time::Time(void) :
	m_StartTime(0),
	m_CurrentTime(0),
	m_TimeScale(1.f),
	m_LastTime(0),
	m_DeltaTime(0)
{
	Reset();
}


//Time::~Time(void)
//{
//}


void Time::Reset(void)
{
	m_StartTime/* = m_CurrentTime*/ = GetRealTime();
}


void Time::Tick(void)
{
	m_LastTime = m_CurrentTime;

	m_CurrentTime = GetRealTime();

	m_DeltaTime = m_CurrentTime - m_LastTime;
}


const unsigned int Time::GetRealTime(void)
{
	return timeGetTime() - GetStartTime();
}

	
const float Time::GetTime(void)
{
	return (m_CurrentTime * m_TimeScale) / 1000.f;
}

	
const unsigned int Time::GetDeltaTime(void)
{
	//deltaTime should be calculated once in each update otherwise it tends to zero and the object dose not translate
	//return GetRealTime() - m_CurrentTime;

	return m_DeltaTime;
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(Time)
	BEGIN_DEFINE_TYPE(Time, "Time")
		DEFINE_FUNCTION("GetRealTime", &Time::GetRealTime)
		DEFINE_FUNCTION("GetTime", &Time::GetTime)
		DEFINE_FUNCTION("GetDeltaTime", &Time::GetDeltaTime)
		DEFINE_FUNCTION("GetStartTime", &Time::GetStartTime)
		DEFINE_FUNCTION("GetTimeScale", &Time::GetTimeScale)
		DEFINE_FUNCTION("SetTimeScale", &Time::SetTimeScale)
		DEFINE_FUNCTION("GetTimeSinceLastUpdate", &Time::GetTimeSinceLastUpdate)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE