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
#include "MessageDispatcher.h"
#include "Message.h"
#include "AIEngine.h"
#include "AIObject.h"
#include "AIObjectType.h"
#include "Time.h"
#include "Log.h"


BEGIN_NAMESPACE
MessageDispatcher *Singleton<MessageDispatcher>::m_Instance = NULL;


MessageDispatcher::MessageDispatcher(void)
{
}


MessageDispatcher::~MessageDispatcher(void)
{
}


void MessageDispatcher::DispatchTheMessage(float delayTime, int Sender, int Receiver, unsigned int Msg, void* ExtraInfo)
{
	AIObject* pReceiver = NULL;

	if(Receiver >= 0)
	{
		 pReceiver = AIEngine::GetInstance().GetAIObject(Receiver);

		if(!pReceiver)
		{
			Log::GetInstance().AddInfo("AI Object with ID " + Ogre::StringConverter::toString(Receiver) + " does not exist.");
			return;
		}
	}

	Message *msg = new Message(Sender, Receiver, Msg, delayTime, ExtraInfo);

	if(delayTime <= SEND_MESSAGE_IMMEDIATELY)
	{
		Discharge(pReceiver, *msg);
	}
	else
	{
		const unsigned int currentTime = Time::GetInstance().GetRealTime();

		msg->m_DispatchTime = currentTime + delayTime;

		m_MessageSet.insert(msg);
	}
}

void MessageDispatcher::DispatchDelayedMessages(void)
{
	const unsigned int currentTime = Time::GetInstance().GetRealTime();

	while(!m_MessageSet.empty() &&
		  (*m_MessageSet.begin())->m_DispatchTime < currentTime &&
		  (*m_MessageSet.begin())->m_DispatchTime > 0)
	{
		const Message &msg = *(*m_MessageSet.begin());

		AIObject* pReceiver = AIEngine::GetInstance().GetAIObject(msg.m_Receiver);

		Discharge(pReceiver, msg);

		m_MessageSet.erase(m_MessageSet.begin());
	}
}


void MessageDispatcher::Discharge(AIObject* pReceiver, const Message& msg)
{
	if (!pReceiver->HandleMessage(msg))
	{
		Log::GetInstance().AddInfo("The Message could not be handled.");
	}
}
END_NAMESPACE

