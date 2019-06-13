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

#include "GameObject.h"


BEGIN_NAMESPACE
class TerrainGameObject : public GameObject
{
public:
	TerrainGameObject(const Ogre::String &Name, Scene *Holder, GameObject *Parent = NULL);
	~TerrainGameObject(void);
	
	void Start(void);
	bool Update(void);
	
#ifdef EDIT_MODE
	bool Serialize(Attributes *Attributes);
#endif
	bool Deserialize(Attributes *Attributes);

	bool Prepare(const Ogre::String &HeightMapName, float TerrainHeight, unsigned int TerrainSize, float TerrainWorldSize, Ogre::Terrain::LayerInstanceList *LayerList = NULL);

	const Ogre::Terrain* _GetTerrain(void)
	{
		return m_Terrain;
	}

	const Ogre::Vector3 &GetPosition(void)
	{
		//if (GetVisible())
			return m_TerrainGroup->getOrigin();
		
		//return m_PositionBeforeInvisibility;
	}

	void SetPosition(const Ogre::Vector3 &Position)
	{
		m_TerrainGroup->setOrigin(Position);

		GameObject::SetPosition(Position);
	}
	
	void SetPosition(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		SetPosition(Ogre::Vector3(X, Y, Z));
	}

	void SetScale(const Ogre::Vector3 &Scale)
	{
		// Nothing
	}
	
	void SetScale(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		// Nothing
	}

	void Pitch(Ogre::Real Value)
	{
		// Nothing
	}

	void Yaw(Ogre::Real Value)
	{
		// Nothing
	}

	void Roll(Ogre::Real Value)
	{
		// Nothing
	}
	
	void SetRotation(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{	
		// Nothing
	}
	
	void SetRotation(const Ogre::Vector3 &Rotation)
	{
		// Nothing
	}
	
	void SetOrientation(const Ogre::Quaternion &Quaternion)
	{
		// Nothing
	}

	void SetOrientation(Ogre::Real W, Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		// Nothing
	}
	
	const Ogre::AxisAlignedBox &GetWorldAABB(void) const
	{
		return m_WorldAABB;
	}

	const Ogre::String &GetNavMeshFileName(void) const
	{
		return m_NavMeshFileName;
	}

	void SetNavMeshFileName(const Ogre::String &FileName)
	{
		m_NavMeshFileName = FileName;
	}

	//void SetVisible(bool Value);

	GameObjectType GetType(void) const
	{
		return GOT_TERRAIN;
	}

	DECLARE_SCRIPT_REGISTATION()

private:
	Ogre::TerrainGroup *m_TerrainGroup;
	Ogre::Terrain *m_Terrain;
	Ogre::String m_HeightMapName;
	
	Ogre::String m_NavMeshFileName;
	bool m_NavMeshNotLoaded;

	Ogre::AxisAlignedBox m_WorldAABB;

	//Ogre::Vector3 m_PositionBeforeInvisibility;
};
END_NAMESPACE

