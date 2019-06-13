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
#include "GraphNode.h"


BEGIN_NAMESPACE
GraphNode::GraphNode() : Node(-1),
						 m_Position(Ogre::Vector3::ZERO), m_X(0), m_Y(0)
{
}


GraphNode::GraphNode(const int Index,
					 const Ogre::Vector3 Position,
					 const unsigned X,
					 const unsigned Y) : Node(Index),
										 m_Position(Position), m_X(X), m_Y(Y)
{
}


GraphNode::~GraphNode(void)
{
}


const unsigned int GraphNode::GetX() const
{
	return m_X;
}


const unsigned int GraphNode::GetY() const
{
	return m_Y;
}


const Ogre::Vector3 &GraphNode::GetPosition() const
{
	return m_Position;
}


void GraphNode::SetPosition(const Ogre::Vector3 &Position)
{
	m_Position = Position;
}
END_NAMESPACE