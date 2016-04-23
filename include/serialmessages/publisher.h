/**
    \file publisher_base.h
    \author Natesh Narain <nnaraindev@gmail.com>
    \date 04-17-2016
*/

#ifndef SERIALMESSAGES_PUBLISHER_H
#define SERIALMESSAGES_PUBLISHER_H

#include "publisher_base.h"
#include "post_publisher.h"

namespace serialmessages
{
/**
	\class Publisher
*/
template<class MessageT>
class Publisher : public PublisherBase
{
public:
	Publisher(const char * topic, PostPublisher* post) : 
		PublisherBase(topic),
		post_(post)
	{
	}

	~Publisher()
	{
	}

	void publish(const MessageT& msg)
	{
		msg_ = &msg;
		post_->postPublisher(this);
	}

	virtual void serializeMessage(SerialStream& stream)
	{
		msg_->serialize(stream);
	}

private:
	PostPublisher* post_;
	MessageT* msg_;
};
}

#endif // SERIALMESSAGES_PUBLISHER_H

