
#ifndef SERIALMESSAGE_MESSAGE_SERVER_H
#define SERIALMESSAGE_MESSAGE_SERVER_H

#include <simplelogger/simplelogger.h>

#include "message_protocol.h"

namespace serialmessages
{
/**
	\class MessageServer
*/
template<typename CommT>
class MessageServer
{
public:

	template<typename... Args>
	MessageServer(Args&... args) : 
        comm_(args...),
        sync_(false)
	{
	}

	~MessageServer()
	{
	}
    
    bool initialize()
    {
        return comm_.initialize();
    }
    
    void spinOnce()
    {
        // not synced with the client
        if(!sync_)
        {
            // send sync sequence
            comm_.write((uint8_t*)SIGNATURE, sizeof(SIGNATURE));
        }

        // attempt to read a byte from the client
        int byte = comm_.read();

        if(byte > 0)
        {
            // recieved client acknowlege
            if(byte == CLIENT_ACK)
            {
                sync_ = true;
                LOG_INFO("Client synchronised");
            }
        }
    }

private:
	CommT comm_;
    bool sync_;
};
}

#endif // SERIALMESSAGE_MESSAGE_SERVER_H
