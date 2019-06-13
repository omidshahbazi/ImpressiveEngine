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
#include "PathEdge.h"


BEGIN_NAMESPACE
PathEdge::PathEdge(const Ogre::Vector3 Source, const Ogre::Vector3 Destination, const unsigned int &Behavior) : m_Source(Source),
																												m_Destination(Destination),
																												m_Behavior(Behavior)
{
}


const Ogre::Vector3 &PathEdge::GetSource(void) const
{
	return m_Source;
}


void PathEdge::SetSource(const Ogre::Vector3 Source)
{
	m_Source = Source;
}


const Ogre::Vector3 &PathEdge::GetDestination(void) const
{
	return m_Destination;
}


void PathEdge::SetDestination(const Ogre::Vector3 Destination)
{
	m_Destination = Destination;
}


const unsigned int &PathEdge::GetBehavior(void) const
{
	return m_Behavior;
}


void PathEdge::SetBehavior(const unsigned int Behavior)
{
	m_Behavior = Behavior;
}


END_NAMESPACE