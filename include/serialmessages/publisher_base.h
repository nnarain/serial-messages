/**
    \file publisher_base.h
    \author Natesh Narain <nnaraindev@gmail.com>
    \date 04-16-2016
*/

#ifndef SERIALMESSAGE_PUBLISHER_BASE_H
#define SERIALMESSAGE_PUBLISHER_BASE_H

#include "serial_stream.h"
//#include "message_protocol.h"

namespace serialmessages
{
class MessageProtocol;

class PublisherBase
{
	friend class MessageProtocol;

public:
	const char * topic;

	PublisherBase(const char * topic) : topic(topic)
	{
	}

	virtual void serializeMessage(SerialStream& stream) = 0;

protected:
	MessageProtocol* protocol_;

	void post()
	{
		protocol_->queuePublisher(this);
	}
};
}

#endif // SERIALMESSAGE_PUBLISHER_BASE_H
