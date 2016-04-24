/**
    \file message_server.h
    \author Natesh Narain <nnaraindev@gmail.com>
*/
#ifndef SERIALMESSAGE_MESSAGE_SERVER_H
#define SERIALMESSAGE_MESSAGE_SERVER_H

#include <simplelogger/simplelogger.h>

#include "message_protocol.h"
#include "serial_stream.h"
#include "subscriber.h"
#include "hash_table.h"
#include "stdmsgs/string.h"

#include <cstring>

namespace serialmessages
{
/**
	\class MessageServer
*/
template<
    class CommT, 
    unsigned int IN_BUFFER_SIZE=512, 
    unsigned int OUT_BUFFER_SIZE=512
>
class MessageServer : public MessageProtocol<CommT, IN_BUFFER_SIZE, OUT_BUFFER_SIZE>
{
public:

	template<typename... Args>
	MessageServer(Args&... args) : MessageProtocol<CommT, IN_BUFFER_SIZE, OUT_BUFFER_SIZE>(args...)
	{
	}

	~MessageServer()
	{
	}
    
    virtual void spinOnce()
    {
        // not synced with the client
        if(!this->sync_)
        {
            // send sync sequence
            this->comm_.write(this->signature_.data(), this->signature_.size());
        }

        // attempt to read a byte from the client
        int byte = this->comm_.read();

        // read byte from client
        if(byte >= 0)
        {
            // check byte sequence to match client ack
            this->acknowledge_.check((uint8_t)byte);
            // if matched
            if(this->acknowledge_.match())
            {
                this->acknowledge_.reset();
                this->sync_ = true;

                // read next byte for number of messages the client wants to send
                uint8_t messages_to_read = this->readByte();

                // if intent to send message
                while(messages_to_read--)
                {
                    this->readMessage();
                }

                // send byte indicating number of messages we want to send to client
                uint8_t messages_to_write = this->publisher_queue_.size();

                this->comm_.write(&messages_to_write, 1);

                while(messages_to_write--)
                {
                    this->writeMessage();
                }

                // transaction complete, set sync false
                this->sync_ = false;
            }
        }
    }

private:
};
}

#endif // SERIALMESSAGE_MESSAGE_SERVER_H
