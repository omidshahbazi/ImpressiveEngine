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
#include "Edge.h"


BEGIN_NAMESPACE
Edge::Edge(const unsigned int StartPoint, const unsigned int EndPoint, const float Distance) : m_StartPoint(StartPoint),
																							   m_EndPoint(EndPoint),
																							   m_Distance(Distance)
{
}


Edge::~Edge(void)
{
}

const unsigned int &Edge::GetStartPoint() const
{
	return m_StartPoint;
}

void Edge::SetStartPoint(const unsigned int PointIndex)
{
	m_StartPoint = PointIndex;
}

const unsigned int &Edge::GetEndPoint() const
{
	return m_EndPoint;
}

void Edge::SetEndPoint(const unsigned int PointIndex)
{
	m_EndPoint = PointIndex;
}

const float &Edge::GetDistance() const
{
	return m_Distance;
}

void Edge::SetDistance(const float Distance)
{
	m_Distance = Distance;
}
END_NAMESPACE