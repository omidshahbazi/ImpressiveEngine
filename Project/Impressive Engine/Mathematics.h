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
#include "Common.h"

BEGIN_NAMESPACE
class Mathematics
{
public:
	//Mathematics(void);
	//~Mathematics(void);

	static float Sin(const Ogre::Real fValue);

	static float Cos(const Ogre::Real fValue);

	static unsigned int Abs32(int Value);

	static double Clamp(const float value, const float min, const float max);

	static void TruncateVector3(Ogre::Vector3& Vector, const float& max);

	static float Max(const float X, const float Y);

	static float Min(const float X, const float Y);

	static int Rand32(const int X, const int Y);
	static float Randf(const float X, const float Y);

	static const Ogre::Quaternion GetRotationFromHeading(Ogre::Vector3 Look);

};


END_NAMESPACE