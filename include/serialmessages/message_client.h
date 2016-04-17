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
    
    void spinOnce()
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

                    // for now just send a send intent, test topic string and message length
                    uint8_t out_buffer[512];
                    SerialStream ss(out_buffer, 512);

                    stdmsgs::String msg;
                    msg.data = "Hello World!!!";

                    uint8_t num_messages = 4;
                    comm_.write(&num_messages, 1);

                    for(int i = 0; i < 4; i++)
                    {
                        comm_.write("test_topic", sizeof("test_topic"));

                        msg.serialize(ss);

                        writeInt32((uint32_t)ss.size());
                        comm_.write(out_buffer, ss.size());
                    }
                }
            }
        }
    }

private:
	CommT comm_;
    bool sync_;

private:
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

