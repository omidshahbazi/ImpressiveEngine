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
#include "FileSecurity.h"
#include "StdAfxOGRE.h"


BEGIN_NAMESPACE
const unsigned int BLOCK = 10;


wxString FileSecurity::EncryptContent(const wxString &Content, const wxString &Salt)
{
	int salt = GetSalt(Salt);

	wxString content = "";

	for (unsigned int i = 0; i < Content.size(); i++)
		content += (char)((int)Content.GetChar(i) + salt);

	return Method(content, true);
}
	

wxString FileSecurity::DecryptContent(const wxString &Content, const wxString &Salt)
{
	int salt = GetSalt(Salt);

	wxString content = "";

	for (unsigned int i = 0; i < Content.size(); i++)
		content += (char)((int)Content.GetChar(i) - salt);

	return Method(content, false);
}


int FileSecurity::GetSalt(const wxString &Salt)
{
	int salt = 0;

	for (unsigned int i = 0; i < Salt.size(); i++)
		salt += (char)((int)Salt.GetChar(i) - 50);

	return Ogre::Math::Clamp(salt, 11, 136);
}


wxString FileSecurity::Method(const wxString &Content, bool Encrypting)
{
	wxString temp = "";

	int size = Content.size();
	
	unsigned int blocksCount = Ogre::Math::Ceil(Content.size() / (float)BLOCK);
	unsigned int blockIndex = 0;
	unsigned int charIndex = 0;
	unsigned int index = 0;
	unsigned int i = 0;

	for (charIndex = 0; charIndex < blocksCount * BLOCK; charIndex++)
	{
		temp += '\u25cf';
	}

	for (charIndex = 0; charIndex < Content.size(); charIndex++)
	{
		i = (index * BLOCK) + blockIndex;
		
		if (Encrypting)
		{
			temp.SetChar(i, Content.GetChar(charIndex));
		}
		else
		{
			char c = Content.GetChar(i);

			if (c != '\u25cf')
			{
				temp.SetChar(charIndex, c);
			}
		}
		
		index++;

		if (index >= blocksCount)
		{
			index = 0;

			blockIndex++;

			if (blockIndex >= BLOCK)
				blockIndex = 0;
		}
	}

	size = temp.size();
	
	return temp;
}
END_NAMESPACE