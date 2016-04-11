
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
            comm_.write(protocol_.signature.data(), protocol_.signature.size());
        }

        // attempt to read a byte from the client
        int byte = comm_.read();

        // read byte from client
        if(byte >= 0)
        {
            // check byte sequence to match client ack
            protocol_.acknowledge.check((uint8_t)byte);
            // if matched
            if(protocol_.acknowledge.match())
            {
                LOG_INFO("Client sync");

                protocol_.acknowledge.reset();
                sync_ = true;

                // read next byte for client intention

                // if intent to send message

                    // read 4 bytes for message length
                    // deserialize
                    // dispatch

                // 
            }
        }
    }

private:
	CommT comm_;
    MessageProtocol protocol_;
    bool sync_;

private:
    uint8_t readByte()
    {
        int byte = -1;

        while(byte == -1)
        {
            byte = comm_.read();
        }

        return (int)byte;
    }
};
}

#endif // SERIALMESSAGE_MESSAGE_SERVER_H
