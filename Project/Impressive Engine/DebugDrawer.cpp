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
#ifdef EDIT_MODE

#include "DebugDrawer.h"
#include "OgreDebugDrawer.h"
#include "Scene.h"

BEGIN_NAMESPACE
DebugDrawer *Singleton<DebugDrawer>::m_Instance = NULL;

//DebugDrawer::DebugDrawer(void)
//{
//	Ogre::DebugDrawer::getSingleton().drawLine(Ogre::Vector3(0, 0, 0), Ogre::Vector3(100, 100, 100), Ogre::ColourValue::Red);
//
//	for (int i = 0; i < 5; ++i)
//	{
//		for (int j = 0; j < 5; j++)
//		{
//			for (int k = 0; k < 5; k++)
//			{
//				Ogre::AxisAlignedBox box(Ogre::Vector3(i * 10.0f + 2.0f, j * 10.0f + 2.0f, k * 10.0f + 2.0f),
//				Ogre::Vector3((i + 1) * 10.0f - 2.0f, (j + 1) * 10.0f - 2.0f, (k + 1) * 10.0f - 2.0f));
//				DebugDrawer::getSingleton().drawCuboid(box.getAllCorners(), Ogre::ColourValue(51.0f * i / 255.0f, 51.0f * j / 255.0f, 51.0f * k / 255.0f), true);
//			}
//		}
//	}
//}


DebugDrawer::~DebugDrawer(void)
{
	delete Ogre::DebugDrawer::getSingletonPtr();
}


void DebugDrawer::Initialize(Scene *Scene)
{
	new Ogre::DebugDrawer(Scene->GetSceneManager(), 0.3f);
}


void DebugDrawer::BeginUpdate(void)
{
	Ogre::DebugDrawer::getSingleton().clear();
}


void DebugDrawer::EndUpdate(void)
{
	Ogre::DebugDrawer::getSingleton().build();
}


void DebugDrawer::DrawLine(const DrawType &Type, const Ogre::Vector3 &Start, const Ogre::Vector3 &End, const Ogre::ColourValue &Color)
{
	Ogre::DebugDrawer::getSingleton().drawLine(Start, End, Color);
}


void DebugDrawer::DrawCircle(const DrawType &Type, const Ogre::Vector3 &Centre, const float &Radius, const int &SegmentsCount, const Ogre::ColourValue &Color, const bool &IsFilled)
{
	Ogre::DebugDrawer::getSingleton().drawCircle(Centre, Radius, SegmentsCount, Color, IsFilled);
}


void DebugDrawer::DrawCylinder(const DrawType &Type, const Ogre::Vector3 &Centre, const float &Radius, const int &SegmentsCount, const float &Height, const Ogre::ColourValue &Color, const bool &IsFilled)
{
	Ogre::DebugDrawer::getSingleton().drawCylinder(Centre, Radius, SegmentsCount, Height, Color, IsFilled);
}


void DebugDrawer::DrawQuad(const DrawType &Type, const Ogre::Vector3 &LeftUp, const Ogre::Vector3 &RightUp, const Ogre::Vector3 &RightDown, const Ogre::Vector3 &LeftDown, const Ogre::ColourValue &Color, const bool &IsFilled)
{
	const Ogre::Vector3 verts[] = {LeftUp, RightUp, RightDown, LeftDown};

	Ogre::DebugDrawer::getSingleton().drawQuad(&verts[0], Color, IsFilled);
}


void DebugDrawer::DrawCuboid(const DrawType &Type, const Ogre::AxisAlignedBox &AAB, const Ogre::ColourValue &Color, const bool &IsFilled)
{
	Ogre::DebugDrawer::getSingleton().drawCuboid(AAB.getAllCorners(), Color, IsFilled);
}


void DebugDrawer::DrawSphere(const DrawType &Type, const Ogre::Vector3 &Centre, const float &Radius, const Ogre::ColourValue &Color, const bool &IsFilled)
{
	Ogre::DebugDrawer::getSingleton().drawSphere(Centre, Radius, Color, IsFilled);
}


void DebugDrawer::DrawTetrahedron(const DrawType &Type, const Ogre::Vector3 &Centre, const float &Scale, const Ogre::ColourValue &Color, const bool &IsFilled)
{
	Ogre::DebugDrawer::getSingleton().drawSphere(Centre, Scale, Color, IsFilled);
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(DebugDrawer)		
	BEGIN_DEFINE_ENUMERATOR(DrawType, "DrawType")
		DEFINE_ENUMERATOR_VALUE("DT_AI", DebugDrawer::DT_AI),
		DEFINE_ENUMERATOR_VALUE("DT_SYSTEM", DebugDrawer::DT_SYSTEM)
	END_DEFINE_ENUMERATOR()

	BEGIN_DEFINE_TYPE(DebugDrawer, "DebugDrawer")
		DEFINE_FUNCTION("DrawLine", &DebugDrawer::DrawLine)
		DEFINE_FUNCTION("DrawCircle", &DebugDrawer::DrawCircle)
		DEFINE_FUNCTION("DrawCylinder", &DebugDrawer::DrawCylinder)
		DEFINE_FUNCTION("DrawQuad", &DebugDrawer::DrawQuad)
		DEFINE_FUNCTION("DrawCuboid", &DebugDrawer::DrawCuboid)
		DEFINE_FUNCTION("DrawSphere", &DebugDrawer::DrawSphere)
		DEFINE_FUNCTION("DrawTetrahedron", &DebugDrawer::DrawTetrahedron)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE
#endif