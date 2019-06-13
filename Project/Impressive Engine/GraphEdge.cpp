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
#include "GraphEdge.h"


BEGIN_NAMESPACE
GraphEdge::GraphEdge(const unsigned int StartPoint, const unsigned int EndPoint,
					 const float Distance, unsigned int Flags) : Edge(StartPoint, EndPoint, Distance),
																 m_Flags(Flags)
{
}


GraphEdge::~GraphEdge(void)
{
}


const unsigned int &GraphEdge::GetFlags(void) const
{
	return m_Flags;
}


void GraphEdge::SetFlags(unsigned int Flags)
{
	m_Flags = Flags;
}

END_NAMESPACE