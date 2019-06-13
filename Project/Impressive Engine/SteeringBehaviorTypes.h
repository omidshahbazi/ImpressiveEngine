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


BEGIN_NAMESPACE
enum SteeringBehaviorType
{
    SBT_NONE					= 0x00000,
    SBT_SEEK					= 0x00002,
    SBT_FLEE					= 0x00004,
    SBT_ARRIVE					= 0x00008,
    SBT_WANDER					= 0x00010,
    SBT_SEPARATION				= 0x00020,
    SBT_ALIGNMENT				= 0x00040,
    SBT_COHESION				= 0x00080,
    SBT_WALL_AVOIDANCE			= 0x00100,
    SBT_OFFSET_PURSUIT			= 0x00200,
    SBT_OBSTACLE_AVOIDANCE		= 0x00400,
};

END_NAMESPACE