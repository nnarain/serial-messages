#ifndef SERIALMESSAGES_MESSAGE_CLIENT_H
#define SERIALMESSAGES_MESSAGE_CLIENT_H

#include "message_protocol.h"

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
/*            if(!sync_)
            {
                if((char)byte == *signature_)
                {
                    signature_counter_++;
                    if(signature_counter_ == sizeof(SIGNATURE))
                    {
                        sync_ = true;

                    //    char c = ACK;
                        comm_.write((uint8_t*)ACK, sizeof(ACK));
                    }
                }
            }*/

            if(!sync_)
            {
                protocol_.signature.check((uint8_t)byte);

                if(protocol_.signature.match())
                {
                    protocol_.signature.reset();
                    sync_ = true;

                    comm_.write(protocol_.acknowledge.data(), protocol_.acknowledge.size());
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

