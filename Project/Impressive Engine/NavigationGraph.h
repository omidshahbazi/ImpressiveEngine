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

#include <list>

#include "Singleton.h"
#include "StdAfxOGRE.h"

BEGIN_NAMESPACE
class GameMap;
class SparseGraph;

class NavigationGraph
{
public:
	NavigationGraph(GameMap* GameMap);

	~NavigationGraph(void);

	SparseGraph* GetSparseGraph(void);

	void CreateGraph(void);

	void SaveGraph(void);

	void LoadGraph(const Ogre::String& FileName);

	const Ogre::Vector3 &GetRandomLocation(void);

	const bool& IsLoaded(void) const;

	void Drop(void);

	const float& GetWorldSize(void) const;

	const unsigned int GetNodeIndexInTerrainSpace(const Ogre::Vector3& Position);

private:
	void GenerateNodes(void);

	void GenerateNodeConnections(void);

	void RemoveObstacleNodes(void);

private:
	static const char *TAG_NAVMESH;
	static const char *TAG_SIZE;
	static const char *TAG_WORLDSIZE;
	static const char *TAG_TERRAIN;
	static const char *TAG_X;
	static const char *TAG_Y;
	static const char *TAG_POINT;
	static const char *TAG_BLOCKED;
	static const char *TAG_POSITION;

	GameMap* m_GameMap;

	SparseGraph* m_SparseGraph;

	short m_Size;
	float m_WorldSize;
	float m_HalfWorldSize;
	float m_TileSize;
	float m_HalfTileSize;
	float m_ObstacleOffset;

	bool m_bIsLoaded;

};


END_NAMESPACE