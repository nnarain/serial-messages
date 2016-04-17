/**
    \file message_protocol.h
    \author Natesh Narain <nnaraindev@gmail.com>
*/

#ifndef SERIALMESSAGES_MESSAGE_PROTOCOL_H
#define SERIALMESSAGES_MESSAGE_PROTOCOL_H

#include "byte_sequence.h"

#define SIGNATURE  "SYNCSIGNATURE"
#define ACK "ACK"

namespace serialmessages
{
/**
	\class MessageProtocol
	\brief encapsulate protocol specific information
*/
class MessageProtocol
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

private:
};
}

#endif // SERIALMESSAGES_MESSAGE_PROTOCOL_H
