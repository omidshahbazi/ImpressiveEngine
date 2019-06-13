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
#include "ThreadHandler.h"


BEGIN_NAMESPACE
ThreadHandler::ThreadHandler(IThreadWorker *Worker) :
	m_Worker(Worker)
{
	m_Thread = boost::thread(boost::ref(*m_Worker));
}


//ThreadHandler::~ThreadHandler(void)
//{
//}

END_NAMESPACE