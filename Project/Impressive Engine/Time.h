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
#include <Windows.h>
#include "ScriptRegister.h"


BEGIN_NAMESPACE
class Time : public Singleton<Time>
{
public:
	Time(void);
	//~Time(void);

	void Reset(void);
	void Tick(void);

	const unsigned int GetRealTime(void);
	
	const float GetTime(void);
	
	const unsigned int GetDeltaTime(void);
	
	const unsigned int &GetStartTime(void)
	{
		return m_StartTime;
	}
	
	const float &GetTimeScale(void)
	{
		return m_TimeScale;
	}

	void SetTimeScale(const float &Value)
	{
		m_TimeScale = Value;
	}

	const unsigned int GetTimeSinceLastUpdate(void)
	{
		if (m_LastTime > m_CurrentTime)
			return 0;

		return m_CurrentTime - m_LastTime;
	}

	DECLARE_SCRIPT_REGISTATION()

private:
	unsigned int m_StartTime;
	unsigned int m_CurrentTime;
	float m_TimeScale;
	unsigned int m_LastTime;
	unsigned int m_DeltaTime;
};


END_NAMESPACE