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

#include "Edge.h"

BEGIN_NAMESPACE
class GraphEdge : public Edge
{
public:
	GraphEdge(const unsigned int StartPoint, const unsigned int EndPoint, const float Distance, unsigned int Flags);

	~GraphEdge(void);

	const unsigned int &GetFlags(void) const;

	void SetFlags(unsigned int Flag);

private:
	unsigned int m_Flags;

};


END_NAMESPACE