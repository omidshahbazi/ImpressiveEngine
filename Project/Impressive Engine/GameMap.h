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
#include "StdAfxOGRE.h"
#include "ScriptRegister.h"

BEGIN_NAMESPACE
class NavigationGraph;
class TerrainGameObject;
class GameObject;

class GameMap : public Singleton<GameMap>
{
public:
	typedef std::list<GameObject*> ObstacleList;
	typedef std::list<GameObject*> HideSpotList;

public:
	GameMap(void);

	~GameMap(void);

	void Drop(void);

	NavigationGraph* GetNavGraph(void);

	void SetTerrain(TerrainGameObject* Terrain);

	TerrainGameObject* GetTerrain(void);

	void SaveMap(void);

	void LoadMap(const Ogre::String& FileName);

	const bool TerrainAvailable(void) const;

	const bool IsObstractedBetween(Ogre::Vector3& A, Ogre::Vector3& B, float& BoundingRadius);

	void AddObstacle(GameObject* Obstacle);

	void RemoveObstacles(void);

	const ObstacleList& GetObstacles(void) const;

	void SetObstacles(std::list<GameObject*> &List);

	void AddHideSpot(GameObject* Obstacle);

	void RemoveHotSpots(void);

	const HideSpotList& GetHideSpots(void) const;

	DECLARE_SCRIPT_REGISTATION()

private:
	NavigationGraph* m_NavGraph;

	TerrainGameObject* m_Terrain;

	ObstacleList m_Obstacles;
	HideSpotList m_HideSpots;

};


END_NAMESPACE