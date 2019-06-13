#pragma once 

#include <Ogre.h>

using namespace Ogre;

struct IeTransform
{
	Vector3 p;
	Quaternion q;

	IeTransform(Vector3 P, Quaternion Q) : p(P), q(Q)
	{
	}

	IeTransform(Vector3 P) : p(P), q(Quaternion::IDENTITY)
	{
	}
};

class PseudoGameObject
{
public:
		Ogre::SceneNode *OriginNode;
		Entity *m_Entity;
		bool IsPrimitive;
		Ogre::String m_Tag;

public:
	PseudoGameObject(Ogre::SceneNode *_EntityHolder, Entity *_Entity = NULL) :
		OriginNode(_EntityHolder), m_Entity(_Entity), IsPrimitive(false)
	{
		OriginNode->_updateBounds();
	}

	inline MovableObject *GetAttachedObject(const String &name)
	{
		return OriginNode->getAttachedObject(name);
	}

	inline void SetScale(float x, float y, float z)
	{
		OriginNode->setScale(x, y, z	);
		OriginNode->_updateBounds();
	}

	inline void SetScale(const Vector3 &scale)
	{
		OriginNode->setScale(scale);
		OriginNode->_updateBounds();
	}

	const Vector3 &GetScale(void)
	{
		return OriginNode->getScale();
	}

	inline void SetOrientation(float w, float x, float y, float z)
	{
		OriginNode->setOrientation(w, x, y, z);
	}

	inline void SetOrientation(Quaternion q)
	{
		OriginNode->setOrientation(q);
	}

	inline void SetPosition(float x, float y, float z)
	{
		OriginNode->setPosition(x, y, z);
	}

	inline void SetPosition(Vector3 p)
	{
		OriginNode->setPosition(p);
	}

	const Vector3 &GetPosition(void)
	{
		return OriginNode->getPosition();
	}

	void SetTransform(IeTransform &transform)
	{
		SetOrientation(transform.q);
		SetPosition(transform.p);
	}

	void SetTransform(Vector3 &p, Quaternion &q)
	{
		SetOrientation(q);
		SetPosition(p);
	}

	IeTransform GetTransform(void)
	{
		return IeTransform(OriginNode->getPosition(), OriginNode->getOrientation());
	}

	const AxisAlignedBox &GetWorldAABB(void)
	{
		return OriginNode->_getWorldAABB();
	}

	Entity *GetEntity(void)
	{
		return m_Entity;
	}

	void OnCollisionEnter()
	{
	}

	void SetTag(Ogre::String Tag)
	{
		m_Tag = Tag;
	}

	const String &GetTag(void)
	{
		return m_Tag;
	}
};