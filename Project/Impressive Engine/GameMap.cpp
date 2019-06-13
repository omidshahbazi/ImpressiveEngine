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
#include "GameMap.h"
#include "NavigationGraph.h"
#include "SparseGraph.h"
#include "TerrainGameObject.h"
#include "Time.h"


BEGIN_NAMESPACE
GameMap *Singleton<GameMap>::m_Instance = 0;


GameMap::GameMap(void) : m_NavGraph(0), m_Terrain(0)
{
	std::srand(Time::GetInstance().GetRealTime());
	m_NavGraph = new NavigationGraph(this);
}


GameMap::~GameMap(void)
{
	Drop();
}


void GameMap::Drop(void)
{
	m_NavGraph->Drop();

	m_Terrain = NULL;
}

NavigationGraph* GameMap::GetNavGraph(void)
{
	return m_NavGraph;
}


void GameMap::SetTerrain(TerrainGameObject* Terrain)
{
	m_Terrain = Terrain;
}


TerrainGameObject* GameMap::GetTerrain(void)
{
	return m_Terrain;
}


void GameMap::SaveMap(void)
{
	m_NavGraph->SaveGraph();
}


void GameMap::LoadMap(const Ogre::String& FileName)
{
	m_NavGraph->LoadGraph(FileName);
}


const bool GameMap::TerrainAvailable(void) const
{
	return m_Terrain != NULL;
}


const bool GameMap::IsObstractedBetween(Ogre::Vector3& A, Ogre::Vector3& B, float& BoundingRadius)
{
	Ogre::Vector3 ToB = (B - A).normalisedCopy();

	Ogre::Vector3 curPos = A;

	while(curPos.squaredDistance(B) > BoundingRadius * BoundingRadius)
	{
		float n = curPos.squaredDistance(ToB);

		curPos += ToB * 0.5 * BoundingRadius;

		if (m_NavGraph->GetSparseGraph()->GetNode(curPos).Blocked()) return true;
	}

	return false;
}



void GameMap::AddObstacle(GameObject* Obstacle)
{
	if(!Obstacle) return;

	m_Obstacles.push_back(Obstacle);
}


void GameMap::RemoveObstacles(void)
{
	m_Obstacles.clear();
}


const GameMap::ObstacleList& GameMap::GetObstacles(void) const
{
	return m_Obstacles;
}


void GameMap::SetObstacles(std::list<GameObject*> &List)
{
	m_Obstacles.clear();

	m_Obstacles = List;
}


void GameMap::AddHideSpot(GameObject* HideSpot)
{
	if(!HideSpot) return;

	m_HideSpots.push_back(HideSpot);
}


void GameMap::RemoveHotSpots(void)
{
	m_HideSpots.clear();
}


const GameMap::HideSpotList& GameMap::GetHideSpots(void) const
{
	return m_HideSpots;
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(GameMap)
	BEGIN_DEFINE_TYPE(GameMap, "GameMap")
		DEFINE_FUNCTION("AddObstacle", (void(GameMap::*)(GameObject*))&GameMap::AddObstacle)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()


END_NAMESPACE