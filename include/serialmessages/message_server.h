
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
                uint8_t intent = readByte();

                // if intent to send message
                if(intent == MessageProtocol::Intent::SEND_MESSAGE)
                {
                    // read 4 bytes for message length
                    // deserialize
                    // dispatch
                }
                else if(intent == MessageProtocol::Intent::READ_MESSAGE)
                {
                    // send the client a message
                }
                else
                {
                    LOG_ERROR("Received invalid client intent");
                    LOG_WARN("Client desync");
                }

                //
                sync_ = false;
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

    void gets(const char * str)
    {
        char c = (uint8_t)readByte();
        while(c != 0)
        {
            *str = c;
            str++;
        }
    }
};
}

#endif // SERIALMESSAGE_MESSAGE_SERVER_H
