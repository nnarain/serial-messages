/**
    \file subscriber.h
    \author Natesh Narain <nnaraindev@gmail.com>
    \date 04-16-2016
*/

#ifndef SERIALMESSAGE_SUBSCRIBER_H
#define SERIALMESSAGE_SUBSCRIBER_H

#include "subscriber_base.h"

namespace serialmessages
{
/**

*/
template<class MessageT>
class Subscriber : public SubscriberBase
{
public:
	typedef void(*Callback)(const MessageT& msg);

	Subscriber(const char * topic, Callback callback) :
		SubscriberBase(topic),
		callback_(callback)
	{
	}

	virtual void callback(SerialStream& stream)
	{
		msg_.deserialize(stream);
		callback_(msg_);
	}

private:
	Callback callback_;
	MessageT msg_;
};
}

#endif // SERIALMESSAGE_SUBSCRIBER_BASE_H
