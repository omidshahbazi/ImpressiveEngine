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
#include "File.h"
#include "ScriptRegister.h"

BEGIN_NAMESPACE
File::File(const wxString &FileName) :
	m_FileName(FileName),
	m_File(NULL),
	m_FileSize(0),
	m_WriteMode(false)
{
	OpenFile();
}


File::File(const wxString &FileName, bool Append) :
	m_FileName(FileName),
	m_File(NULL),
	m_FileSize(0),
	m_WriteMode(true)
{
	OpenFile(Append);
}


File::~File(void)
{
	Close();
}


Ogre::String File::ReadText(void)
{
	char *str = new char[m_FileSize];

	Read(str, m_FileSize);
	
	return Ogre::String(str);
}


int File::Read(void *Buffer, unsigned int SizeToRead)
{
	if (!IsOpen())
		return 0;

	return fread(Buffer, 1, SizeToRead, m_File);
}


void File::WriteText(const Ogre::String &Buffer)
{
	Write(Buffer.c_str(), Buffer.size());
}


int File::Write(const void *Buffer, unsigned int SizeToWrite)
{
	if (!IsOpen())
		return 0;

	return fwrite(Buffer, 1, SizeToWrite, m_File);
}


bool File::Seek(long FinalPos, bool RelativeMovement)
{
	if (!IsOpen())
		return false;

	return (fseek(m_File, FinalPos, (RelativeMovement ? SEEK_CUR : SEEK_SET)) == 0);
}


long File::GetPosition() const
{
	return ftell(m_File);
}


void File::OpenFile(bool Append)
{
	if (m_FileName.size() == 0)
	{
		m_File = NULL;
		return;
	}

	if (m_WriteMode)
	{
		//m_File = _wfopen(m_FileName.c_str(), Append ? L"ab" : L"wb");
		m_File = fopen(m_FileName.c_str(), Append ? "ab" : "wb");
	}
	else
	{
		//m_File = _wfopen(Filename.c_str(), L"rb");
		m_File = fopen(m_FileName.c_str(), "rb");
	}

	if (m_File)
	{
		fseek(m_File, 0, SEEK_END);
		m_FileSize = ftell(m_File);
		fseek(m_File, 0, SEEK_SET);
	}
}


bool File::IsOpen() const
{
	return (m_File != 0);
}


void File::Close(void)
{
	if (m_File)
	{
		fclose(m_File);
	}
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(File)
	BEGIN_DEFINE_TYPE(File, "File")
		.def(constructor<const char*>())
		.def(constructor<const char*, bool>())

		DEFINE_FUNCTION("Read", &File::ReadText)
		DEFINE_FUNCTION("Write", &File::WriteText)

		DEFINE_FUNCTION("Close", &File::Close)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE