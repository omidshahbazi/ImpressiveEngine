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

#include "Singleton.h"
#include "StdAfxOGRE.h"
#include "ScriptRegister.h"

BEGIN_NAMESPACE
class RegistryKey;

class IO : public Singleton<IO>
{
public:
	IO(void);
	//~IO(void);

	bool FileExists(const Ogre::String &FilePath);

#ifdef EDIT_MODE
	void WriteScene(const Ogre::String &FilePath, const Ogre::String &Content);
	Ogre::String ReadScene(const Ogre::String &FilePath);
	
	void WriteScript(const Ogre::String &FilePath, const Ogre::String &Content);
#endif
	Ogre::String ReadScript(const Ogre::String &FilePath);

#ifdef EDIT_MODE
	void WriteLevel(const Ogre::String &FilePath, const Ogre::String &Content);
#endif
	Ogre::String ReadLevel(const Ogre::String &FilePath);

	Ogre::String ReadFromFile(const Ogre::String &FilePath);
	void WriteToFile(const Ogre::String &FilePath, const Ogre::String &Content);

	Ogre::String ReadFromEncryptedFile(const Ogre::String &FilePath, const Ogre::String &Salt);
	void WriteToEncryptedFile(const Ogre::String &FilePath, const Ogre::String &Content, const Ogre::String &Salt);
	
#ifdef EDIT_MODE
	RegistryKey *GetEnvironmentRegistryKey(void)
	{
		return m_EnvironmentRegistryKey;
	}
#endif

	DECLARE_SCRIPT_REGISTATION()

private:
#ifdef EDIT_MODE
	RegistryKey *m_EnvironmentRegistryKey;
#endif
};
END_NAMESPACE