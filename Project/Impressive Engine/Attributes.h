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
#include <vector>
using namespace std;

BEGIN_NAMESPACE
struct TreeElement;
class Attribute;

class Attributes
{
public:
	//Attributes(void);
	~Attributes(void);

	void Read(const TreeElement *Element);

	void Write(TreeElement *Element);


	void AddInteger(const std::string &Name, int Value);
	void AddFloat(const std::string &Name, float Value);
	void AddString(const std::string &Name, const std::string &Value);
	void AddBoolean(const std::string &Name, bool Value);
	void AddColor(const std::string &Name, const Ogre::ColourValue &Value);
	void AddVector2(const std::string &Name, const Ogre::Vector2 &Value);
	void AddVector3(const std::string &Name, const Ogre::Vector3 &Value);
	void AddVector4(const std::string &Name, const Ogre::Vector4 &Value);
	void AddQuaternion(const std::string &Name, const Ogre::Quaternion &Value);



	void SetInteger(const char *Name, int Value);
	void SetFloat(const char *Name, float Value);
	void SetString(const char *Name, const char *Value);
	void SetBoolean(const char *Name, bool Value);
	void SetColor(const char *Name, const Ogre::ColourValue &Value);
	void SetVector2(const char *Name, const Ogre::Vector2 &Value);
	void SetVector3(const char *Name, const Ogre::Vector3 &Value);
	void SetVector4(const char *Name, const Ogre::Vector4 &Value);
	void SetQuaternion(const char *Name, const Ogre::Quaternion &Value);

	

	int GetInteger(const char *Name);
	float GetFloat(const char *Name);
	std::string GetString(const char *Name);
	bool GetBoolean(const char *Name);
	Ogre::ColourValue GetColor(const char *Name);
	Ogre::Vector2 GetVector2(const char *Name);
	Ogre::Vector3 GetVector3(const char *Name);
	Ogre::Vector4 GetVector4(const char *Name);
	Ogre::Quaternion GetQuaternion(const char *Name);

	void Remove(const char *Name);

private:
	Attribute *GetAttribute(const char *Name);

private:
	vector<Attribute*> m_Attributes;
};


END_NAMESPACE