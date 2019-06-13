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
#include "Common.h"

BEGIN_NAMESPACE
class PathEdge
{
public:
	PathEdge(const Ogre::Vector3 Source, const Ogre::Vector3 Destination, const unsigned int &Behavior);

	const Ogre::Vector3 &GetSource(void) const;

	void SetSource(const Ogre::Vector3 Source);

	const Ogre::Vector3 &GetDestination(void) const;

	void SetDestination(const Ogre::Vector3 Destination);

	const unsigned int &GetBehavior(void) const;

	void SetBehavior(const unsigned int Behavior);

private:
	Ogre::Vector3 m_Source;

	Ogre::Vector3 m_Destination;

	unsigned int m_Behavior;

};


END_NAMESPACE