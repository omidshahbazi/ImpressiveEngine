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
#include "Message.h"
#include "Mathematics.h"


BEGIN_NAMESPACE
const unsigned int m_SmallestDelay = 250;


Message::Message(void) : m_Sender(-1),
						 m_Receiver(-1),
						 m_Message(-1),
						 m_DispatchTime(-1),
						 m_ExtraInfo(0)
{
}


Message::Message(int Sender,
				 int Receiver,
				 unsigned int Message,
				 unsigned int DistatchTime,
				 void* ExtraInfo = NULL) : m_Sender(Sender),
									m_Receiver(Receiver),
									m_Message(Message),
									m_DispatchTime(DistatchTime),
									m_ExtraInfo(ExtraInfo)
{
}


Message::~Message(void)
{
}


inline bool Message::operator== (const Message& message)
{
	return ((Mathematics::Abs32(this->m_DispatchTime - message.m_DispatchTime) < m_SmallestDelay) &&
			this->m_Sender == message.m_Sender &&
			this->m_Receiver == message.m_Receiver &&
			this->m_Message == message.m_Message &&
			this->m_ExtraInfo == message.m_ExtraInfo);
}


inline bool Message::operator< (const Message& message)
{
	if(*this == message)
	{
		return false;
	}
	else
	{
		return this->m_DispatchTime < message.m_DispatchTime;
	}
}

END_NAMESPACE