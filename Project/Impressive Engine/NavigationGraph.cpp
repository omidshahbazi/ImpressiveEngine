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
#include "NavigationGraph.h"
#include "GameMap.h"
#include "SparseGraph.h"
#include "SparseGraphThreadWorker.h"
#include "ThreadingManager.h"
#include "GameObject.h"
#include "TerrainGameObject.h"
#include "Attributes.h"
#include "TreeParser.h"
#include "Engine.h"
#include "Mathematics.h"
#include "ScenesHolder.h"
#include "Scene.h"
#include "Time.h"
#include "Utility.h"
#include "Log.H"

#include <vector>

BEGIN_NAMESPACE
const char *NavigationGraph::TAG_NAVMESH("NAVMESH");
const char *NavigationGraph::TAG_SIZE("S");
const char *NavigationGraph::TAG_WORLDSIZE("W");
const char *NavigationGraph::TAG_TERRAIN("TERRAIN");
const char *NavigationGraph::TAG_X("X");
const char *NavigationGraph::TAG_Y("Y");
const char *NavigationGraph::TAG_POINT("N");
const char *NavigationGraph::TAG_BLOCKED("B");
const char *NavigationGraph::TAG_POSITION("P");


NavigationGraph *Singleton<NavigationGraph>::m_Instance = 0;


NavigationGraph::NavigationGraph(GameMap* GameMap) : m_GameMap(GameMap),
													 m_SparseGraph(0),
													 m_TileSize(1),
													 m_ObstacleOffset(0.0f),
													 m_Size(0),
													 m_WorldSize(0),
													 m_bIsLoaded(false)

{
	m_SparseGraph = new SparseGraph(this);
}


NavigationGraph::~NavigationGraph(void)
{
	delete m_SparseGraph;
	m_SparseGraph = NULL;
}


SparseGraph* NavigationGraph::GetSparseGraph(void)
{
	return m_SparseGraph;
}


void NavigationGraph::CreateGraph(void)
{
	if (!m_GameMap->GetTerrain())
	{
		Log::GetInstance().AddInfo("Terrain has not been set yet.");
	}

	const unsigned int startTime = Time::GetInstance().GetRealTime();

	m_Size = m_GameMap->GetTerrain()->_GetTerrain()->getSize();
	m_WorldSize = m_GameMap->GetTerrain()->_GetTerrain()->getWorldSize();
	m_HalfWorldSize = m_WorldSize / 2;
	m_TileSize = m_WorldSize / (m_Size - 1);
	m_HalfTileSize = m_TileSize / 2;
	m_ObstacleOffset = m_TileSize * (m_Size - 1) / 32;

	m_SparseGraph->Clear();
	GenerateNodes();
	RemoveObstacleNodes();
	GenerateNodeConnections();

	m_bIsLoaded = true;

	Log::GetInstance().AddInfo("Navigation Mesh for terrain [" + m_GameMap->GetTerrain()->GetName() + "] with " + Ogre::StringConverter::toString(m_SparseGraph->GetNodeSize())
	+ " nodes was generated in " + Ogre::StringConverter::toString(Time::GetInstance().GetRealTime() - startTime) + "ms");
}


void NavigationGraph::SaveGraph(void)
{
	if (m_SparseGraph->IsEmpty())
	{
		Log::GetInstance().AddInfo("Navigation Mesh has not been generated yet.");
	}

	unsigned int startTime = Time::GetInstance().GetRealTime();

	TreeParser tp = TreeParser();

	TreeElement *root = new TreeElement(tp.GetRoot());
	root->Name = TAG_NAVMESH;
	root->Attributes.push_back(TreeElementAttribute(TAG_TERRAIN, m_GameMap->GetTerrain()->GetName().c_str()));
	root->Attributes.push_back(TreeElementAttribute(TAG_SIZE, Ogre::StringConverter::toString(m_Size)));
	root->Attributes.push_back(TreeElementAttribute(TAG_WORLDSIZE, Ogre::StringConverter::toString(m_WorldSize)));

	TreeElement *elem = NULL;
	Attributes *att = NULL;

	for (unsigned int i = 0; i < m_SparseGraph->GetNodeSize(); i++)
	{
		elem = new TreeElement(root);
		elem->Name = TAG_POINT;
		elem->Attributes.push_back(TreeElementAttribute(TAG_BLOCKED, m_SparseGraph->GetNode(i).Blocked() ? Ogre::StringConverter::toString(1).c_str() : Ogre::StringConverter::toString(0).c_str()));
		elem->Attributes.push_back(TreeElementAttribute(TAG_POSITION, Ogre::StringConverter::toString(m_SparseGraph->GetNode(i).GetPosition())));
	}

	tp.WriteToFile(Engine::PATH_AI + "navmesh.xml");

	ScenesHolder::GetInstance().GetCurrentScene()->SetNavGraphFileName("navmesh.xml");

	Log::GetInstance().AddInfo("Navigation Mesh for terrain [" + m_GameMap->GetTerrain()->GetName() + "] saved. " + Ogre::StringConverter::toString(Time::GetInstance().GetRealTime() - startTime) + "ms");
}

void NavigationGraph::LoadGraph(const Ogre::String& FileName)
{
#ifdef EDIT_MODE
	SparseGraphThreadWorker* worker = new SparseGraphThreadWorker(FT_CLEARGRAPH);
	ThreadingManager::GetInstance().CreateThread(worker);

	unsigned int startTime = Time::GetInstance().GetRealTime();
#endif

	if(FileName == "") return;

	const Ogre::String path = Engine::PATH_AI + FileName;
	TreeParser *tr = new TreeParser(path);

	std::vector<bool> Blocks;
	std::vector<Ogre::Vector3> Positions;

	BOOST_FOREACH(const TreeElement *root, tr->GetRoot()->Children)
	{
		const unsigned short arraySize = m_Size;

		if(root->Name == TAG_NAVMESH)
		{
			m_Size = Ogre::StringConverter::parseInt(root->GetAttributeValue(TAG_SIZE));
			m_WorldSize = Ogre::StringConverter::parseInt(root->GetAttributeValue(TAG_WORLDSIZE));
			m_HalfWorldSize = m_WorldSize / 2;
			m_TileSize = m_WorldSize / (m_Size - 1);
			m_HalfTileSize = m_TileSize / 2;
			m_ObstacleOffset = m_TileSize * (m_Size - 1) / 32;
		}

		BOOST_FOREACH(const TreeElement *child, root->Children)
		{
			if(child->Name == TAG_POINT)
			{
				Blocks.push_back(Ogre::StringConverter::parseBool(child->GetAttributeValue(TAG_BLOCKED)));
				Positions.push_back(Ogre::StringConverter::parseVector3(child->GetAttributeValue(TAG_POSITION)));
			}
		}
	}

	delete tr;
#ifdef EDIT_MODE
	delete worker;
#endif

	for (unsigned short y = 0; y < m_Size; y++)
	{
		for (unsigned short x = 0; x < m_Size; x++)
		{
			const unsigned int node = y * m_Size + x;
			//m_SparseGraph->AddNode(x, y, m_GameMap->GetTerrain()->_GetTerrain()->convertPosition(Ogre::Terrain::POINT_SPACE, Ogre::Vector3(x, y, m_GameMap->GetTerrain()->_GetTerrain()->getHeightAtPoint(x, y)), Ogre::Terrain::WORLD_SPACE), Blocks[y * m_Size + x]);
			m_SparseGraph->AddNode(x, y, Positions[node], Blocks[node]);
		}
	}

	GenerateNodeConnections();

	m_bIsLoaded = true;

#ifdef EDIT_MODE
	Log::GetInstance().AddInfo("Navigation Mesh for terrain [ ] with " + Ogre::StringConverter::toString(m_SparseGraph->GetNodeSize())
	+ " nodes was loaded in " + Ogre::StringConverter::toString(Time::GetInstance().GetRealTime() - startTime) + "ms");
#endif
}


const Ogre::Vector3 &NavigationGraph::GetRandomLocation(void)
{
	int RandIndex = Mathematics::Rand32(0, m_SparseGraph->GetActiveNodeSize());

	SparseGraph::NodeIterator NodeItr(m_SparseGraph);
	
	GraphNode* pNode = NodeItr.begin();

	while (--RandIndex > 0)
	{
		pNode = NodeItr.next();
	}

	return pNode->GetPosition();
}


const bool& NavigationGraph::IsLoaded(void) const
{
	return m_bIsLoaded;
}


void NavigationGraph::Drop(void)
{
	m_SparseGraph->Clear();

	m_bIsLoaded = false;
}


const float& NavigationGraph::GetWorldSize(void) const
{
	return m_WorldSize;
}

const unsigned int NavigationGraph::GetNodeIndexInTerrainSpace(const Ogre::Vector3& Position)
{
	if(!m_GameMap->GetTerrain()) return 0;

	const Ogre::Vector3 Point = 
		m_GameMap->GetTerrain()->_GetTerrain()->convertPosition(Ogre::Terrain::WORLD_SPACE, Position, Ogre::Terrain::POINT_SPACE);

	return Point.y * m_Size + Point.x;
}


void NavigationGraph::GenerateNodes(void)
{
	for (unsigned short y = 0; y < m_Size; y++)
	{
		for (unsigned short x = 0; x < m_Size; x++)
		{
			m_SparseGraph->AddNode(x, y, m_GameMap->GetTerrain()->_GetTerrain()->convertPosition(Ogre::Terrain::POINT_SPACE, Ogre::Vector3(x, y, m_GameMap->GetTerrain()->_GetTerrain()->getHeightAtPoint(x, y)), Ogre::Terrain::WORLD_SPACE));
		}
	}
}


void NavigationGraph::GenerateNodeConnections(void)
{
	if (!m_GameMap->GetTerrain()) return;

	for (unsigned short y = 0; y < m_Size ; y++)
	{
		for (unsigned short x = 0; x < m_Size; x++)
		{
			for(short i=-1; i<2; ++i)
			{
				for(short j=-1; j<2; ++j)
				{
					if (i==0 && j==0) continue;

					short neighbourX = x + j;
					short neighbourY = y + i;

					if(neighbourX > 0 && neighbourX < m_Size && neighbourY > 0 && neighbourY < m_Size)
					{
						Ogre::Vector3 nodePosition = m_SparseGraph->GetNode(y * m_Size + x).GetPosition();
						Ogre::Vector3 neighborPosition = m_SparseGraph->GetNode(neighbourY * m_Size + neighbourX).GetPosition();

						float dist = nodePosition.squaredDistance(neighborPosition);

						m_SparseGraph->AddEdge(GetNodeIndexInTerrainSpace(nodePosition), GetNodeIndexInTerrainSpace(neighborPosition), dist);
					}
				}
			}
		}
	}
}



void NavigationGraph::RemoveObstacleNodes(void)
{
	BOOST_FOREACH(const GameObject* current, m_GameMap->GetObstacles())
	{
		Ogre::Vector3 Pos1 = current->GetWorldAABB().getMinimum();
		Ogre::Vector3 Pos4 = current->GetWorldAABB().getMaximum();

		for(float z = Pos1.z - m_ObstacleOffset; z <= Pos4.z + m_ObstacleOffset; z += m_TileSize)
		{
			for(float x = Pos1.x - m_ObstacleOffset; x <= Pos4.x + m_ObstacleOffset; x += m_TileSize)
			{
				Ogre::Vector3 localPos = m_GameMap->GetTerrain()->_GetTerrain()->convertPosition(Ogre::Terrain::WORLD_SPACE, 
																									Ogre::Vector3(x, Pos1.y, z), 
																									Ogre::Terrain::POINT_SPACE);
				if(localPos.x > 0 && localPos.x < m_Size && localPos.y > 0 && localPos.y < m_Size)
				{
					const unsigned int nd = localPos.y * m_Size + localPos.x;

					m_SparseGraph->RemoveNode(nd);
				}
			}
		}
	}
}



END_NAMESPACE