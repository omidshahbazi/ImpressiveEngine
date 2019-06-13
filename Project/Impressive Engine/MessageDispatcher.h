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

#include <set>
#include "Singleton.h"

BEGIN_NAMESPACE
struct Message;

class AIEngine;
class AIObject;

const float SEND_MESSAGE_IMMEDIATELY = 0.0;
const unsigned int NO_ADDITIONAL_INFO = 0;

const int SENDER_ID_IRRELEVANT = -1;
const int SENDER_ID_SALESMAN = -2;

const int RECEIVER_ID_NPC = -2;

class MessageDispatcher : public Singleton<MessageDispatcher>
{
public:
	MessageDispatcher(void);

	~MessageDispatcher(void);

	void DispatchDelayedMessages(void);

	void DispatchTheMessage(float delayTime, int Sender, int Receiver, unsigned int Message, void* ExtraInfo = NULL);

private:
	void Discharge(AIObject* pReceiver, const Message& msg);

private:
	std::set<Message*>			m_MessageSet;

};
END_NAMESPACE

