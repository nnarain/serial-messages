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
	MessageServer(Args&... args) : 
        MessageProtocol<CommT, IN_BUFFER_SIZE, OUT_BUFFER_SIZE>(args...),
        last_send_(0)
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
            uint32_t now = this->comm_.time();

            // check if sync send period has elapsed
            if((now - last_send_) > 5)
            {
                // send sync sequence
                this->comm_.write(this->signature_.data(), this->signature_.size());

                last_send_ = now;
            }
        }

        // attempt to read a byte from the client
        int byte = this->comm_.read();

        // read byte from client
        if(byte >= 0)
        {
            // check byte sequence to match client acknowledge
            this->acknowledge_.check((uint8_t)byte);
            // if matched
            if(this->acknowledge_.match())
            {
                this->acknowledge_.reset();
                this->sync_ = true;

                // send single ACK character to client so it can sync with the byte stream
                uint8_t ack = 6;
                this->comm_.write(&ack, 1);

                uint8_t count = 0;

                // read next byte for number of messages the client wants to send
                uint8_t messages_to_read = this->readByte();

                // read messages from client
                count = messages_to_read;
                while(count--)
                {
                    this->readMessage();
                }

                // ACK to client, that server has read the messages
                this->comm_.write(&ack, 1);

                // send byte indicating number of messages we want to send to client
                messages_to_write_ = (uint8_t)this->publisher_queue_.size();
                this->comm_.write(&messages_to_write_, 1);
                
                count = messages_to_write_;
                while(count--)
                {
                    this->writeMessage();
                }

                // uint8_t last_byte = this->readByte();
                // LOG_INFO("avaible: %d", last_byte);

                LOG_INFO("wait for client ack");
                uint8_t messages_recieved_ack = this->readByte();

                if(messages_recieved_ack == ack) 
                    LOG_INFO("client has recieved our messages");
                else
                    LOG_INFO("client sent %d", messages_recieved_ack);

                // transaction complete, set sync false
                this->sync_ = false;
            }
        }
    }

private:
    uint32_t last_send_;
    uint8_t messages_to_write_;
};
}

#endif // SERIALMESSAGE_MESSAGE_SERVER_H
