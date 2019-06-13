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

#include "StdAfxOGRE.h"
#include "Node.h"

BEGIN_NAMESPACE
class GraphNode : public Node
{
public:
	GraphNode();
	GraphNode(const int Index, const Ogre::Vector3 Position, const unsigned X = 0, const unsigned Y = 0);

	~GraphNode(void);

	const unsigned int GetX() const;

	const unsigned int GetY() const;

	const Ogre::Vector3 &GetPosition() const;

	void SetPosition(const Ogre::Vector3 &Position);

private:
	unsigned int m_X;
	unsigned int m_Y;

	Ogre::Vector3 m_Position;

};


END_NAMESPACE