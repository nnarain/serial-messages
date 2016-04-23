/**
    \file message_protocol.h
    \author Natesh Narain <nnaraindev@gmail.com>
*/

#ifndef SERIALMESSAGES_MESSAGE_PROTOCOL_H
#define SERIALMESSAGES_MESSAGE_PROTOCOL_H

#include "byte_sequence.h"
#include "publisher_base.h"
#include "post_publisher.h"
#include "ring_buffer.h"

#define SIGNATURE "SYNCSIGNATURE"
#define ACK "ACK"

namespace serialmessages
{
	// const char * SIGNATURE = "SYNCSIGNATURE";
	// const char * ACK = "ACK";

/**
	\class MessageProtocol
	\brief encapsulate protocol specific information
*/
class MessageProtocol : public PostPublisher
{
protected:
	ByteSequence signature;
	ByteSequence acknowledge;

public:

	MessageProtocol() :
		signature((uint8_t*)SIGNATURE, sizeof(SIGNATURE)),
		acknowledge((uint8_t*)ACK, sizeof(ACK))
	{
	}

	~MessageProtocol()
	{
	}

	virtual void spinOnce() = 0;

	virtual void postPublisher(PublisherBase* pub)
	{
		publisher_queue_.put(pub);
	}

protected:
	RingBuffer<PublisherBase*, 10> publisher_queue_;
};
}

#endif // SERIALMESSAGES_MESSAGE_PROTOCOL_H
