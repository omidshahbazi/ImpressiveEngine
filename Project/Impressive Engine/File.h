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

#include <iostream>
#include "StdAfxwxWidgets.h"
#include "StdAfxOGRE.h"
#include "ScriptRegister.h"

BEGIN_NAMESPACE
class File
{
public:
	// Use this for reading a file
	File(const wxString &FileName);
	// Use this for writing a file
	File(const wxString &FileName, bool Append);
	~File(void);

	Ogre::String ReadText(void);

	int Read(void *Buffer, unsigned int SizeToRead);
	
	void WriteText(const Ogre::String &Buffer);

	int Write(const void *Buffer, unsigned int SizeToWrite);

	bool Seek(long FinalPos, bool RelativeMovement = false);

	long GetPosition(void) const;

	bool IsOpen(void) const;

	void Close(void);

	const wxString &GetFileName(void) const
	{
		return m_FileName;
	}

	long GetSize(void) const
	{
		return m_FileSize;
	}

	bool IsWriteMode(void) const
	{
		return m_WriteMode;
	}

	DECLARE_SCRIPT_REGISTATION()

private:
	void OpenFile(bool Append = false);

private:
	wxString m_FileName;
	FILE *m_File;
	long m_FileSize;
	bool m_WriteMode;
};


END_NAMESPACE