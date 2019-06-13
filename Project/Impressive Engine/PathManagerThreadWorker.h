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

#include "IThreadWorker.h"
#include "StdAfxOGRE.h"

BEGIN_NAMESPACE
class PathManager;

class PathManagerThreadWorker : public IThreadWorker
{
public:
	PathManagerThreadWorker(void);
	
	void Do(void);
};
END_NAMESPACE