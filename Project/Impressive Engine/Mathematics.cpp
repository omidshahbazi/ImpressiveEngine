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
#include "Mathematics.h"
#include <random>
#include "Time.h"


BEGIN_NAMESPACE
Ogre::Real Mathematics::Sin(const float fValue)
{
	if(fValue == 0.0f) return 0;
	if(fValue == 90.0f) return 1;
	if(fValue == 180.0f) return 0;
	if(fValue == 270.0f) return -1;
	if(fValue == 360.0f) return 0;
	if(fValue == -90.0f) return -1;
	if(fValue == -180.0f) return 0;

	return Ogre::Math::Sin(Ogre::Degree(fValue));
}


Ogre::Real Mathematics::Cos(const float fValue)
{
	if(fValue == 0.0f) return 1;
	if(fValue == 90.0f) return 0;
	if(fValue == 180.0f) return -1;
	if(fValue == 270.0f) return 0;
	if(fValue == 360.0f) return 1;
	if(fValue == -90.0f) return 0;
	if(fValue == -180.0f) return 1;

	return Ogre::Math::Cos(Ogre::Degree(fValue));
}


unsigned int Mathematics::Abs32(int Value)
{
	return Value >= 0 ? Value : -Value;
}


double Mathematics::Clamp(const float value, const float min, const float max)
{
	return value < min ? min : max;
}


void Mathematics::TruncateVector3(Ogre::Vector3& Vector, const float& max)
{
	if(Vector.length() > max)
	{
		Vector.normalise();

		Vector *= max;
	}
}


float Mathematics::Max(const float X, const float Y)
{
	return X > Y ? X : Y;
}


float Mathematics::Min(const float X, const float Y)
{
	return X < Y ? X : Y;
}


int Mathematics::Rand32(const int X, const int Y)
{
	return std::rand() % (Y - X + 1) + X;
}


const Ogre::Quaternion Mathematics::GetRotationFromHeading(Ogre::Vector3 Look)
{
	Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;

	Ogre::Vector3 newX = Ogre::Vector3::UNIT_X;
	Ogre::Vector3 newY = Ogre::Vector3::UNIT_Y;
	Ogre::Vector3 newZ = Look;

	newZ.normalise();

	if (Look.dotProduct(Ogre::Vector3::UNIT_Y) == 1.0)
		newY = Ogre::Vector3::NEGATIVE_UNIT_Z;

	newX = newY.crossProduct(newZ);
	newX.normalise();
	newY = newZ.crossProduct(newX);

	rotation.FromAxes(newX, newY, newZ);

	return rotation;
}
END_NAMESPACE