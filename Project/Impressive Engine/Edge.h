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
#include "Common.h"

BEGIN_NAMESPACE
class Edge
{
public:
	Edge(const unsigned int StartPoint, const unsigned int EndPoint, const float Distance);

	~Edge(void);

	const unsigned int &GetStartPoint() const;

	void SetStartPoint(const unsigned int PointIndex);

	const unsigned int &GetEndPoint() const;

	void SetEndPoint(const unsigned int PointIndex);

	const float &GetDistance() const;

	void SetDistance(const float Distance);

private:
	unsigned int m_StartPoint;

	unsigned int m_EndPoint;

	float m_Distance;

};
END_NAMESPACE
