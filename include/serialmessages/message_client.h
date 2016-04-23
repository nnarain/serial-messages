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
                this->signature.check((uint8_t)byte);

                if(this->signature.match())
                {
                    this->signature.reset();
                    this->sync_ = true;

                    this->comm_.write(this->acknowledge.data(), this->acknowledge.size());

                    // tell server the number of messages we want to right
                    uint8_t messages_to_write = (uint8_t)this->publisher_queue_.size();
                    this->comm_.write(&messages_to_write, 1);

                    while(messages_to_write--)
                    {
                        this->writeMessage();
                    }

                    // read number of messages server wants to send
                    // ...
                }
            }
        }
    }

private:


    // void writeInt32(uint32_t value)
    // {
    //     uint8_t buff[4];
    //     SerialStream ss(buff, 4);
    //     ss << value;

    //     comm_.write(buff, 4);
    // }

};
}

#endif // SERIALMESSAGES_MESSAGE_CLIENT_H

