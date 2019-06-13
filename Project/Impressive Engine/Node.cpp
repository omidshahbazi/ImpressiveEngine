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
#include "Node.h"


BEGIN_NAMESPACE
Node::Node(const int Index) : m_Index(Index)
{
}


Node::~Node(void)
{
}


const int Node::GetIndex() const
{
	return m_Index;
}


void Node::Block()
{
	m_Index = -1;
}


const bool Node::Blocked() const
{
	return (m_Index == -1);
}

END_NAMESPACE