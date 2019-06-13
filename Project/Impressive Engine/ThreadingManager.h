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
#include "ThreadHandler.h"


BEGIN_NAMESPACE
class ThreadingManager : public Singleton<ThreadingManager>
{
public:
	//ThreadingManager(void);
	//~ThreadingManager(void);

	ThreadHandler *CreateThread(IThreadWorker *Worker);

	void DestroyThread(ThreadHandler *Thread);

private:
	std::list<ThreadHandler*> m_Threads;
};


END_NAMESPACE