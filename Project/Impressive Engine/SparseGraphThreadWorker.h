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
#include "IThreadWorker.h"


BEGIN_NAMESPACE
enum FunctionType
{
	FT_CLEARGRAPH,
	FT_LOADGRAPH
};

class SparseGraphThreadWorker : public IThreadWorker
{
private:
	FunctionType m_FunctionType;

	Ogre::String m_NavGraphFileName;

public:
	SparseGraphThreadWorker(FunctionType FType);

	void SetNavGraphFileName(const Ogre::String& FileName);

	const Ogre::String& GetNavGraphFileName(void) const;

	void Do();
};
END_NAMESPACE