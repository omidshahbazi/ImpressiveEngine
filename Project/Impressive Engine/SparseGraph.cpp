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
#include "SparseGraph.h"
#include "NavigationGraph.h"
//#include "TreeWriter.h"
#include "Log.h"

#include "MeshGameObject.h"


BEGIN_NAMESPACE
SparseGraph::SparseGraph(NavigationGraph* NavigationGraph) : m_NavigationGraph(NavigationGraph),
															 m_NextValidNodeIndex(0)

{

}


SparseGraph::~SparseGraph(void)
{

}


void SparseGraph::Clear()
{
	for(NodeVector::iterator node = m_Nodes.begin(); node != m_Nodes.end(); node++)
	{
		delete *node;
	}

	m_Nodes.clear();

	for(EdgeListVector::iterator edgeVector = m_Edges.begin(); edgeVector != m_Edges.end(); edgeVector++)
	{
		for(EdgeList::iterator edge = edgeVector->begin(); edge != edgeVector->end(); edge++)
		{
			delete *edge;
		}

		edgeVector->clear();
	}

	m_Edges.clear();
}


void SparseGraph::AddNode(const unsigned int X, const unsigned int Y, const Ogre::Vector3 Position, const bool Blocked)
{
	m_Nodes.push_back(new GraphNode(m_NextValidNodeIndex, Position, X, Y));

	m_Edges.push_back(EdgeList());

	if (Blocked) 
	{
		m_Nodes[m_NextValidNodeIndex]->Block();
	}

	m_NextValidNodeIndex++;
}


void SparseGraph::RemoveNode(const unsigned int Index)
{
	m_Nodes[Index]->Block();
	////RemoveInvalidEdges();
}


const GraphNode* SparseGraph::GetNodePtr(const int& Index) const
{
	if(m_Nodes.empty()) return NULL;

	return m_Nodes[Index];
}


GraphNode& SparseGraph::GetNode(const int& Index) const
{
	return *m_Nodes[Index];
}


const GraphNode* SparseGraph::GetNodePtr(const Ogre::Vector3& Position)
{
	return m_Nodes[m_NavigationGraph->GetNodeIndexInTerrainSpace(Position)];
}


const GraphNode& SparseGraph::GetNode(const Ogre::Vector3& Position)
{
	return *m_Nodes[m_NavigationGraph->GetNodeIndexInTerrainSpace(Position)];
}


const unsigned int SparseGraph::GetNodeSize(void) const
{
	return m_Nodes.size();
}


const unsigned int SparseGraph::GetActiveNodeSize(void)
{
	unsigned int Counter = 0;

	for(unsigned int i = 0; i < m_Nodes.size(); i++)
	{
		if (!m_Nodes[i]->Blocked())
		{
			Counter++;
		}
	}

	return Counter;
}


void SparseGraph::AddEdge(const unsigned int NodeStart, const unsigned int NodeEnd, const float Cost)
{
	if(!m_Nodes[NodeStart]->Blocked() && !m_Nodes[NodeEnd]->Blocked())
	{
		m_Edges[NodeStart].push_back(new GraphEdge(NodeStart, NodeEnd, Cost, 0));
	}
}


void SparseGraph::RemoveInvalidEdges(void)
{
	for(EdgeListVector::iterator edgeList = m_Edges.begin(); edgeList != m_Edges.end(); ++edgeList)
	{
		for(EdgeList::iterator edge = (*edgeList).begin(); edge != (*edgeList).end(); ++edge)
		{
			if(m_Nodes[(*edge)->GetStartPoint()]->Blocked() || m_Nodes[(*edge)->GetEndPoint()]->Blocked())
			{
				edge = (*edgeList).erase(edge);
				break;
			}
		}
	}
}


const bool SparseGraph::IsEmpty(void) const
{
	return m_Nodes.size() <= 0;
}
END_NAMESPACE