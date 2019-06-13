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
#include "ThreadingManager.h"


BEGIN_NAMESPACE
ThreadingManager *Singleton<ThreadingManager>::m_Instance = NULL;


//ThreadingManager::ThreadingManager(void)
//{
//}


//ThreadingManager::~ThreadingManager(void)
//{
//}


ThreadHandler *ThreadingManager::CreateThread(IThreadWorker *Worker)
{
	ThreadHandler *th = new ThreadHandler(Worker);

	m_Threads.push_back(th);

	return th;
}


void ThreadingManager::DestroyThread(ThreadHandler *Thread)
{
	m_Threads.remove(Thread);
}
END_NAMESPACE