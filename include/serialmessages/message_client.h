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
template<typename CommT>
class MessageClient : public MessageProtocol
{
public:

	template<typename... Args>
	MessageClient(Args... args) : 
        comm_(args...),
        sync_(false)
	{
	}

	~MessageClient()
	{
	}

    void initialize()
    {
        comm_.initialize();
    }
    
    virtual void spinOnce()
    {
    	int byte = comm_.read();

        if(byte >= 0)
        {
            if(!sync_)
            {
                signature.check((uint8_t)byte);

                if(signature.match())
                {
                    signature.reset();
                    sync_ = true;

                    comm_.write(acknowledge.data(), acknowledge.size());

                    // tell server the number of messages we want to right
                    uint8_t messages_to_write = (uint8_t)publisher_queue_.size();
                    comm_.write(&messages_to_write, 1);

                    while(messages_to_write--)
                    {
                        writeMessage();
                    }

                    // read number of messages server wants to send
                    // ...
                }
            }
        }
    }

private:
	CommT comm_;
    bool sync_;

private:
    void writeMessage()
    {
        PublisherBase* publisher = publisher_queue_.get();

        uint8_t header[64];
        SerialStream header_stream(header, 64);

        uint8_t data[128];
        SerialStream data_stream(data, 128);

        publisher->serializeMessage(data_stream);

        header_stream << publisher->topic << (uint32_t)data_stream.size();

        // write header
        comm_.write(header, header_stream.size());
        // write message data
        comm_.write(data, data_stream.size());
    }

    void writeInt32(uint32_t value)
    {
        uint8_t buff[4];
        SerialStream ss(buff, 4);
        ss << value;

        comm_.write(buff, 4);
    }

};
}

#endif // SERIALMESSAGES_MESSAGE_CLIENT_H

