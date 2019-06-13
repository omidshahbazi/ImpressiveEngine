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
enum AIStateType
{
  AIST_BRAIN,
  AIST_IDLE,
  AIST_EXPLORE,
  AIST_SHOPPING,
  AIST_GUARD,
  AIST_SELLER,
  AIST_COMMANDER,
  goal_arrive_at_position,
  AIST_SEETTOPOSITION,
  AIST_PATHFOLLOW,
  AIST_WALLFOLLOW,
  AIST_TRAVERSEEDGE,
  AIST_MOVETOPOSITION,
  AIST_SMOOTHROTATION,
  goal_get_health,
  goal_get_shotgun,
  goal_get_rocket_launcher,
  goal_get_railgun,
  goal_wander,
  goal_negotiate_door,
  goal_attack_target,
  goal_hunt_target,
  goal_strafe,
  goal_adjust_range,
  goal_say_phrase
  
};

END_NAMESPACE