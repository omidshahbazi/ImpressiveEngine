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
#include "StdAfxBoost.h"


BEGIN_NAMESPACE
class ThreadHandler
{
public:
	ThreadHandler(IThreadWorker *Worker);
	//~ThreadHandler(void);

private:
	IThreadWorker *m_Worker;
	boost::thread m_Thread;
};


END_NAMESPACE