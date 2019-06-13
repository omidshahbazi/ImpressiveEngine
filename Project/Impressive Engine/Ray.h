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

#include "StdAfxOGRE.h"
#include "ScriptRegister.h"

BEGIN_NAMESPACE
class Ray
{
public:
	Ray(const Ogre::Vector3 &Origin, const Ogre::Vector3 &Direction);
	Ray(Ogre::Ray Ray);
	//~Ray(void);

	void SetOrigin(const Ogre::Vector3 &Origin)
	{
		m_Ray.setOrigin(Origin);
	}

	const Ogre::Vector3 &GetOrigin() const
	{
		return m_Ray.getOrigin();
	}
		
	void SetDirection(const Ogre::Vector3 &Direction)
	{
		m_Ray.setDirection(Direction);
	}

	const Ogre::Vector3 &GetDirection() const
	{
		return m_Ray.getDirection();
	}
	
	Ogre::Vector3 GetPoint(float T) const
	{
		return m_Ray.getPoint(T);
	}

	float Intersects(const Ogre::Plane &Plane)
	{
		std::pair<bool, float> &result = m_Ray.intersects(Plane);

		if (result.first)
			return result.second;

		return -1;
	}

	DECLARE_SCRIPT_REGISTATION()

private:
	Ogre::Ray m_Ray;
};


END_NAMESPACE