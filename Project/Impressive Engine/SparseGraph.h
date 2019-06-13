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

#include "GraphEdge.h"
#include "GraphNode.h"


BEGIN_NAMESPACE
class NavigationGraph;

class SparseGraph
{
public:
	typedef std::vector<GraphNode*> NodeVector;
	typedef std::list<GraphEdge*> EdgeList;
	typedef std::vector<EdgeList> EdgeListVector;


public:
	SparseGraph(NavigationGraph* NavigationGraph);

	~SparseGraph(void);

	void Clear(void);

	void AddNode(const unsigned int X, const unsigned int Y, const Ogre::Vector3 Position, const bool Blocked = false);

	void RemoveNode(const unsigned int Index);

	const GraphNode* GetNodePtr(const int& Index) const;
	GraphNode& GetNode(const int& Index) const;
	const GraphNode* GetNodePtr(const Ogre::Vector3& Position);
	const GraphNode& GetNode(const Ogre::Vector3& Position);

	const unsigned int GetNodeSize() const;

	const unsigned int GetActiveNodeSize(void);

	void AddEdge(const unsigned int NodeStart, const unsigned int NodeEnd, const float Cost);

	void RemoveInvalidEdges(void);

	const bool IsEmpty(void) const;

	class EdgeIterator
	{
	private:
		EdgeList::iterator m_CurrentEdge;

		const unsigned int m_NodeIndex;

		SparseGraph* m_Graph;

	public:
		EdgeIterator(SparseGraph* Graph, unsigned int NodeIdx) : m_Graph(Graph), m_NodeIndex(NodeIdx)
		{
			m_CurrentEdge = m_Graph->m_Edges[m_NodeIndex].begin();
		}


		GraphEdge* begin(void)
		{
			m_CurrentEdge = m_Graph->m_Edges[m_NodeIndex].begin();

			return *m_CurrentEdge;
		}


		GraphEdge* next(void)
		{
			++m_CurrentEdge;

			if(end())
			{
				return NULL;
			}
			else
			{
				return *m_CurrentEdge;
			}
		}


		bool end(void)
		{
			return m_CurrentEdge == m_Graph->m_Edges[m_NodeIndex].end();
		}
	};

	friend class NodeIterator;

	class NodeIterator
	{
	private:
		NodeVector::iterator m_CurrentNode;

		SparseGraph* m_Graph;

		const NodeVector::iterator GetNextValideNode(void)
		{
			if(m_CurrentNode == m_Graph->m_Nodes.end() ||
				!(*m_CurrentNode)->Blocked())
			{
				return m_CurrentNode;
			}

			while ((*m_CurrentNode)->Blocked())
			{
				++m_CurrentNode;

				if (m_CurrentNode == m_Graph->m_Nodes.end()) break;
			}

			return m_CurrentNode;
		}

	public:
		NodeIterator(SparseGraph* Graph) : m_Graph(Graph)
		{
			m_CurrentNode = m_Graph->m_Nodes.begin();
		}


		GraphNode* begin(void)
		{
			m_CurrentNode = m_Graph->m_Nodes.begin();

			m_CurrentNode = GetNextValideNode();

			return *m_CurrentNode;
		}


		GraphNode* next(void)
		{
			++m_CurrentNode;

			if(end())
			{
				return NULL;
			}
			else
			{
				m_CurrentNode = GetNextValideNode();

				return *m_CurrentNode;
			}
		}


		bool end(void)
		{
			return m_CurrentNode == m_Graph->m_Nodes.end();
		}
	};

	friend class EdgeIterator;

private:
	class NavigationGraph* m_NavigationGraph;

	NodeVector m_Nodes;
	EdgeListVector m_Edges;

	int m_NextValidNodeIndex;

};
END_NAMESPACE