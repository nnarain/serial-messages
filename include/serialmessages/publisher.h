/**
    \file publisher_base.h
    \author Natesh Narain <nnaraindev@gmail.com>
    \date 04-17-2016
*/

#ifndef SERIALMESSAGES_PUBLISHER_H
#define SERIALMESSAGES_PUBLISHER_H

#include "publisher_base.h"

template<class MessageT>
class Publisher : public PublisherBase
{
public:

	Publisher(const char * topic) : PublisherBase(topic)
	{
	}

	~Publisher()
	{
	}

	void publish(const MessageT& msg)
	{
		msg_ = msg;
	}

	virtual void serializeMessage(SerialStream& stream)
	{
		msg_.serialize(stream);
	}

private:
	MessageT msg_;
};

#endif // SERIALMESSAGES_PUBLISHER_H

