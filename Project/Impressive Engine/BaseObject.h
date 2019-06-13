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
#include "ScriptRegister.h"

BEGIN_NAMESPACE
enum AIObjectType;

class GameObject;

class BaseObject
{
public:
	BaseObject(const unsigned int ID, GameObject* gameObject);

	const unsigned int &GetID(void) const;

	GameObject* GetGameObject(void);

	void SetType(const AIObjectType Type);

	const AIObjectType& GetType(void) const;

	const bool &IsPossessed(void) const;

	void Tag(void);

	const bool &IsTagged(void) const;

	void UnTag(void);

	const float &GetBoundingRadius(void) const;

	DECLARE_SCRIPT_REGISTATION()

protected:
	GameObject* m_GameObject;

	float m_BoundingRadius;

	AIObjectType m_Type;

private:
	const unsigned int m_ID;

	bool m_bPossessed;

	bool m_bTag;

};


END_NAMESPACE