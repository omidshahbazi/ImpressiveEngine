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

#include "AIDecisionMaker.h"

BEGIN_NAMESPACE
class AIObject;
class GameObject;

enum WallSideType;


class AIStateWallFollow : public AIDecisionMaker<AIObject>
{
public:
	AIStateWallFollow(AIObject* Owner, GameObject* Wall, WallSideType WSType);

	void Activate(void);

	int Process(void);

	void Terminate(void);

	bool HandleMessage(const Message& Msg);

private:
	GameObject* m_Wall;

	WallSideType m_WallSide;

	Ogre::Vector3 m_Destination;

	bool m_bRayCasted;

};



END_NAMESPACE