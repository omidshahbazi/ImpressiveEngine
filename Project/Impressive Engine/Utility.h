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
#include <Windows.h>
#include "StdAfxOGRE.h"
#include <wx\string.h>


#define ENABLE_BIT(Mask, Flag) Mask |= Flag
#define DISABLE_BIT(Mask, Flag) Mask &= ~Flag
#define IS_BIT_ENABLED(Mask, Flag) Mask & Flag


#define BEGIN_LIST_ITERATOR(List, Type) \
{ \
	const std::list<Type> &_list = List; \
	/*if (list.size() > 0) \
	{*/ \
		Type current; \
		for (std::list<Type>::const_iterator it = _list.begin(); it != _list.end(); it++) \
		{ \
			current = (*it);
		
#define END_LIST_ITERATOR(EmptyValue) \
		} \
	/*}*/ \
	current = EmptyValue; \
}


#define BEGIN_LIST_CONST_ITERATOR(List, Type) \
/*const std::list<Type*> &list = List; \
if (list.size() > 0)*/ \
{ \
	Type *current; \
	for (std::list<Type*>::const_iterator it = List.begin(); it != List.end(); it++) \
	{ \
		current = (*it);
		
#define END_LIST_CONST_ITERATOR \
	} \
	current = 0; \
}


#define BEGIN_VECTOR_ITERATOR(Vector, Type) \
{ \
	/*const std::vector<Type> &_vector = Vector; \
	if (list.size() > 0) \
	{*/ \
		Type current; \
		for (unsigned int i = 0; i < Vector.size(); i++) \
		{ \
			current = Vector[i];
		
#define END_VECTOR_ITERATOR(EmptyValue) \
		} \
	/*}*/ \
	current = EmptyValue; \
}

#ifdef DEBUG
#define BREAK_IF(Condition) \
if (Condition)\
{\
	_asm int 3\
}
#endif


#define STRING_WX_TO_OGRE(String) \
	(String).c_str()

#define STRING_OGRE_TO_WX(String) \
	String


inline std::vector<Ogre::String> GetFiles(const Ogre::String &Directory)
{
	std::vector<Ogre::String> list;

	WIN32_FIND_DATA data;

	Ogre::String dir = Directory + "\\*.*";

    HANDLE res = FindFirstFile(dir.c_str(), &data);

    if (res == INVALID_HANDLE_VALUE)
        return list;
    else 
	{
        do 
		{
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
				list.push_back(Directory + (Ogre::StringUtil::endsWith(Directory, "\\") ? "" : "\\") + data.cFileName);
            }

		} while (FindNextFile(res, &data) != 0);

        FindClose(res);
	}

	return list;
}


inline std::vector<Ogre::String> GetDirectories(const Ogre::String &Directory)
{
	std::vector<Ogre::String> list;

	WIN32_FIND_DATA data;

	Ogre::String dir = Directory + "\\*.*";

    HANDLE res = FindFirstFile(dir.c_str(), &data);

    if (res == INVALID_HANDLE_VALUE)
        return list;
    else 
	{
        do 
		{
			if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
				if (Ogre::String(data.cFileName).find('.') >= 0 || Ogre::String(data.cFileName).find("..") >= 0)
					continue;

				list.push_back(Ogre::String(data.cFileName) + "\\");
            }

		} while (FindNextFile(res, &data) != 0);

        FindClose(res);
	}

	return list;
}