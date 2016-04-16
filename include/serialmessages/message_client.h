#ifndef SERIALMESSAGES_MESSAGE_CLIENT_H
#define SERIALMESSAGES_MESSAGE_CLIENT_H

#include "message_protocol.h"
#include "serial_stream.h"

namespace serialmessages
{
/**
	\class MessageClient
*/
template<typename CommT>
class MessageClient
{
public:

	template<typename... Args>
	MessageClient(Args... args) : 
        comm_(args...),
        signature_(SIGNATURE),
        sync_(false),
        signature_counter_(0)
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
                protocol_.signature.check((uint8_t)byte);

                if(protocol_.signature.match())
                {
                    protocol_.signature.reset();
                    sync_ = true;

                    comm_.write(protocol_.acknowledge.data(), protocol_.acknowledge.size());

                    // for now just send a send intent, test topic string and message length
                    uint8_t out_buffer[512];
                    SerialStream ss(out_buffer, 512);

                    ss << (uint8_t)MessageProtocol::Intent::SEND_MESSAGE << "test_topic" << (uint32_t)100;

                    comm_.write(out_buffer, ss.size());
                }
            }
        }
    }

private:
	CommT comm_;
    MessageProtocol protocol_;
    const char * signature_;
    int signature_counter_;
    bool sync_;
};
}

#endif // SERIALMESSAGES_MESSAGE_CLIENT_H

