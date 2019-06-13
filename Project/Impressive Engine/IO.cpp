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
#include "IO.h"
#include "RegistryKey.h"
#include "File.h"
#include "FileSecurity.h"
#include <fstream>
#include "ScriptRegister.h"
#include "Utility.h"

using namespace std;

#ifdef EDIT_MODE

wxString RegistryRootPath = "Software\\Impressive\\Engine\\";
#endif

#ifdef DEBUG
#include "Log.h"
#endif


BEGIN_NAMESPACE
IO *Singleton<IO>::m_Instance = NULL;


IO::IO(void)
{
#ifdef EDIT_MODE
	m_EnvironmentRegistryKey = new RegistryKey(RegistryRootPath + "Environment\\", true);
#endif
}


//IO::~IO(void)
//{
//}


bool IO::FileExists(const Ogre::String &FilePath)
{
	File file(FilePath);

	return file.IsOpen();
}


#ifdef EDIT_MODE
void IO::WriteScene(const Ogre::String &FilePath, const Ogre::String &Content)
{
	WriteToFile(FilePath, Content);
	//WriteToFile(FilePath, FileSecurity::EncryptContent(Content, ".s"));
}


Ogre::String IO::ReadScene(const Ogre::String &FilePath)
{
	return ReadFromFile(FilePath);
	//return FileSecurity::DecryptContent(ReadFromFile(FilePath), ".s");
}


void IO::WriteScript(const Ogre::String &FilePath, const Ogre::String &Content)
{
	WriteToFile(FilePath, Content);
}
#endif

Ogre::String IO::ReadScript(const Ogre::String &FilePath)
{
//#ifdef EDIT_MODE
	return ReadFromFile(FilePath);
//#else
//	return ReadFromEncryptedFile(FilePath, ".script");
//#endif
}

#ifdef EDIT_MODE
void IO::WriteLevel(const Ogre::String &FilePath, const Ogre::String &Content)
{
	//WriteToFile(FilePath, Content);
	WriteToEncryptedFile(FilePath, Content, "l");
}
#endif

Ogre::String IO::ReadLevel(const Ogre::String &FilePath)
{
	return ReadFromEncryptedFile(FilePath, "l");
}


Ogre::String IO::ReadFromFile(const Ogre::String &FilePath)
{
	File file(FilePath);

	if (!file.IsOpen())
	{
		Log::GetInstance().AddError("File [" + FilePath + "] doesn't exists");

		return "";
	}

	char *str = new char[sizeof(char) * file.GetSize()];

	file.Read(str, file.GetSize());
	
	//Log::GetInstance().AddInfo("File successfully opened [" + FilePath + "]");
	
	return string(str, str + file.GetSize());

//	string content, line;
//
//	ifstream file(FilePath);
//
//	if (file.is_open())
//	{
//		while (!file.eof())
//		{
//			getline(file, line);
//			content += line + "\n";
//		}
//
//		content = content.substr(0, content.size() - 1);
//
//		file.close();
//
//#ifdef DEBUG
//		Log::GetInstance().AddInfo("File opened, successfully [" + FilePath + "]");
//#endif
//	}
//#ifdef DEBUG
//	else
//	{
//		Log::GetInstance().AddError("Cannot open file to reading content [" + FilePath + "]");
//	}
//#endif
//
//	return content;
}


void IO::WriteToFile(const Ogre::String &FilePath, const Ogre::String &Content)
{
	File file(FilePath, false);

	file.Write(Content.c_str(), Content.size());

	Log::GetInstance().AddInfo("File successfully saved [" + FilePath + "]");
	
	//ofstream file(FilePath, ios_base::out);

	//if (file.is_open())
	//{
	//	file << Content;
	//	
	//	file.flush();
	//	file.close();

	//	#ifdef DEBUG
	//	Log::GetInstance().AddInfo("File saved, successfully [" + FilePath + "]");
	//	#endif
	//}
	//else
	//{
	//	Log::GetInstance().AddError("Cannot open file to writing content [" + FilePath + "]");
	//}
}


Ogre::String IO::ReadFromEncryptedFile(const Ogre::String &FilePath, const Ogre::String &Salt)
{
	return STRING_WX_TO_OGRE(FileSecurity::DecryptContent(STRING_OGRE_TO_WX(ReadFromFile(FilePath)), STRING_OGRE_TO_WX(Salt)));
}


void IO::WriteToEncryptedFile(const Ogre::String &FilePath, const Ogre::String &Content, const Ogre::String &Salt)
{
	WriteToFile(FilePath, STRING_WX_TO_OGRE(FileSecurity::EncryptContent(STRING_OGRE_TO_WX(Content), STRING_OGRE_TO_WX(Salt))));
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(IO)
	BEGIN_DEFINE_TYPE(IO, "IO")
		DEFINE_FUNCTION("FileExists", &IO::FileExists)

		DEFINE_FUNCTION("ReadFromFile", &IO::ReadFromFile)
		DEFINE_FUNCTION("WriteToFile", &IO::WriteToFile)
		
		DEFINE_FUNCTION("ReadFromEncryptedFile", &IO::ReadFromEncryptedFile)
		DEFINE_FUNCTION("WriteToEncryptedFile", &IO::WriteToEncryptedFile)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE