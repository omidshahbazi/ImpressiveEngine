#include "IeDebugDrawer.h"
#include "IeConvert.h"
#include "foundation\PxMath.h"


IeDebugDrawer::IeDebugDrawer(Ogre::DebugDrawer *DebugDrawer) : m_DebugDrawer(DebugDrawer)
{
}


IeDebugDrawer::~IeDebugDrawer(void)
{
}



void IeDebugDrawer::DrawLine(const Vector3 &From, const Vector3 &To, const ColourValue &Color)
{
	m_DebugDrawer->drawLine(From, To, Color);
}



void IeDebugDrawer::DrawWireShape(PxShape *shape, PseudoGameObject *gameObject, ColourValue color)
{
	if (shape->getGeometryType() == PxGeometryType::eBOX)
	{
		DrawWireBox(shape, gameObject, color);
	}
}


void ComputeBoxPoints(PxBounds3 &bounds, Vector3 *Vertices, unsigned int *Indices)
{

	Vector3 min = IeConvert::ToVector3(bounds.minimum);
	Vector3 max = IeConvert::ToVector3(bounds.maximum);

	Vertices[0] = min;
	Vertices[1] = Vector3(min.x, min.y, max.z);
	Vertices[2] = Vector3(max.x, min.y, max.z);
	Vertices[3] = Vector3(max.x, min.y, min.z);
	Vertices[4] = max;
	Vertices[5] = Vector3(max.x, max.y, min.z);
	Vertices[6] = Vector3(min.x, max.y, min.z);
	Vertices[7] = Vector3(min.x, max.y, max.z);

	Indices[0] = Indices[7] = Indices[17] = 0;
	Indices[1] = Indices[2] = Indices[19] = 1;
	Indices[3] = Indices[4] = Indices[21] = 2;
	Indices[5] = Indices[6] = Indices[23] = 3;
	Indices[8] = Indices[15] = Indices[20] = 4;
	Indices[9] = Indices[10] = Indices[22] = 5;
	Indices[11] = Indices[12] = Indices[16] = 6;
	Indices[13] = Indices[14] = Indices[18] = 7;
}


void IeDebugDrawer::DrawWireBox(PxShape *shape, PseudoGameObject *gameObject, ColourValue &color)
{
	//PxBounds3 bounds = PxBounds3(IeConvert::ToPxVec3(gameObject->OBB.minimum), IeConvert::ToPxVec3(gameObject->OBB.maximum));

	//DrawWireBox(bounds, color);
}


void IeDebugDrawer::DrawWireBox(PxBounds3 &bounds, ColourValue &color)
{
	Vector3 Vertices[8];
	unsigned int *Indices = new unsigned int[24];

	ComputeBoxPoints(bounds, Vertices, Indices);

	for (int i = 0; i < 12; i++)
	{
		unsigned int p1 = *Indices++;
		unsigned int p2 = *Indices++;

		DrawLine(Vertices[p1], Vertices[p2], color);
	}
}

