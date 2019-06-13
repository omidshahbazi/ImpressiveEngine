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

#include "MessageTypes.h"

BEGIN_NAMESPACE
struct Message
{
public:
	int m_Sender;

	int m_Receiver;

	unsigned int m_Message;

	unsigned int m_DispatchTime;

	void* m_ExtraInfo;

	Message(void);
	Message(int Sender, int Receiver, unsigned int Message, unsigned int DistatchTime, void* ExtraInfo);

	~Message(void);

	inline bool operator== (const Message& message1);

	inline bool operator< (const Message& message1);

};
END_NAMESPACE

