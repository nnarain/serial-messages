/**
    \file message_client.h
    \author Natesh Narain <nnaraindev@gmail.com>
*/

#ifndef SERIALMESSAGES_MESSAGE_CLIENT_H
#define SERIALMESSAGES_MESSAGE_CLIENT_H

#include "message_protocol.h"
#include "serial_stream.h"
#include "stdmsgs/string.h"

namespace serialmessages
{
/**
	\class MessageClient
*/
template<
    class CommT,
    unsigned int IN_BUFFER_SIZE=512,
    unsigned int OUT_BUFFER_SIZE=512
>
class MessageClient : public MessageProtocol<CommT, IN_BUFFER_SIZE, OUT_BUFFER_SIZE>
{
public:

	template<typename... Args>
	MessageClient(Args&... args) : MessageProtocol<CommT, IN_BUFFER_SIZE, OUT_BUFFER_SIZE>(args...)
	{
	}

	~MessageClient()
	{
	}

    void initialize()
    {
        this->comm_.initialize();
    }
    
    virtual void spinOnce()
    {
    	int byte = this->comm_.read();

        if(byte >= 0)
        {
            if(!this->sync_)
            {
                // check byte in signature state machine
                this->signature_.check((uint8_t)byte);

                // if sequence matches
                if(this->signature_.match())
                {
                    // reset state machine
                    this->signature_.reset();
                    this->sync_ = true;

                    // send server acknowledge
                    this->comm_.write(this->acknowledge_.data(), this->acknowledge_.size());

                    // discard any bytes until a server ACK, this is to sync the byte stream with the server
                    uint8_t byte = 0;
                    while((byte = this->readByte()) != 6);

                    uint8_t count;

                    // tell server the number of messages we want to right
                    uint8_t messages_to_write = (uint8_t)this->publisher_queue_.size();
                    this->comm_.write(&messages_to_write, 1);

                    // right messages
                    count = messages_to_write;
                    while(count--)
                    {
                        this->writeMessage();
                    }

                    // read that the server has recieved our messages
                    uint8_t messages_recieved_ack = this->readByte();

                    // read number of messages server wants to send
                    uint8_t messages_to_read = this->readByte();

                    count = messages_to_read;
                    while(count--)
                    {
                        this->readMessage();
                    }

                    this->comm_.write(&messages_recieved_ack, 1);

                    // transaction has completed

                    this->sync_ = false;
                }
            }
        }
    }

};
}

#endif // SERIALMESSAGES_MESSAGE_CLIENT_H

