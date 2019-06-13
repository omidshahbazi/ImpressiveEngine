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
#include "Ray.h"
#include "ScriptRegister.h"


BEGIN_NAMESPACE
Ray::Ray(const Ogre::Vector3 &Origin, const Ogre::Vector3 &Direction)
{
	m_Ray = Ogre::Ray(Origin, Direction);
}


Ray::Ray(Ogre::Ray Ray) :
	m_Ray(Ray)
{
}


//Ray::~Ray(void)
//{
//}


BEGIN_DEFINE_SCRIPT_REGISTRATION(Ray)
	BEGIN_DEFINE_TYPE(Ray, "Ray")
		.def(constructor<Ogre::Vector3, Ogre::Vector3>())

		DEFINE_FUNCTION("SetOrigin", &Ray::SetOrigin)
		DEFINE_FUNCTION("GetOrigin", &Ray::GetOrigin)
		
		DEFINE_FUNCTION("SetDirection", &Ray::SetDirection)
		DEFINE_FUNCTION("GetDirection", &Ray::GetDirection)
		
		DEFINE_FUNCTION("GetPoint", &Ray::GetPoint)
		
		DEFINE_FUNCTION("Intersects", &Ray::Intersects)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE