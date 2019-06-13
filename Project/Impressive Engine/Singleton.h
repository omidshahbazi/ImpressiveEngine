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

BEGIN_NAMESPACE
template<typename T> class Singleton 
{
public:
	//Singleton(void)
	//{
	//	if (!m_Instance)
	//		m_Instance = static_cast<T*>(this);
	//}

	static T &GetInstance() 
	{
		if (!m_Instance)
			m_Instance = new T();

		return *m_Instance;
	}
	
	static T *GetInstancePointer() 
	{
		if (!m_Instance)
			m_Instance = new T();

		return m_Instance;
	}

protected:
	static T *m_Instance;
};
END_NAMESPACE