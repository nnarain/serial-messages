
/**
    \file subscriber_base.h
    \author Natesh Narain <nnaraindev@gmail.com>
    \date 04-16-2016
*/

#ifndef SERIALMESSAGE_SUBSCRIBER_BASE_H
#define SERIALMESSAGE_SUBSCRIBER_BASE_H

#include "serial_stream.h"

namespace serialmessages
{
class SubscriberBase
{
public:
	const char * topic;

	SubscriberBase(const char * topic) : topic(topic)
	{
	}

	virtual void callback(SerialStream& stream) = 0;
};
}

#endif // SERIALMESSAGE_SUBSCRIBER_BASE_H