
#ifndef SERIALMESSAGE_MESSAGE_SERVER_H
#define SERIALMESSAGE_MESSAGE_SERVER_H

#include <simplelogger/simplelogger.h>

#include "message_protocol.h"
#include "serial_stream.h"
#include "hash_table.h"
#include "stdmsgs/string.h"

#include <cstring>

namespace serialmessages
{
/**
	\class MessageServer
*/
template<typename CommT, unsigned int IN_BUFFER_SIZE=512>
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

                LOG_INFO("client intent: %d", intent);

                // if intent to send message
                if(intent == MessageProtocol::Intent::SEND_MESSAGE)
                {
                    uint8_t in_buffer[IN_BUFFER_SIZE];

                    // read topic string
                    gets((char *)in_buffer);

                    // read 4 bytes for message length
                    readBytes(in_buffer + strlen((const char *)in_buffer) + 1, 4);

                    // deserialize topic string and message length
                    stdmsgs::String topic;
                    uint32_t message_length;

                    SerialStream ss(in_buffer, IN_BUFFER_SIZE);
                    topic.deserialize(ss);
                    ss >> message_length;

                    LOG_INFO("%s message length: %d byte(s)", topic.data, message_length);
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
    void readBytes(uint8_t* data, size_t nbytes)
    {
        while(nbytes--)
            *data++ = readByte();
    }

    uint8_t readByte()
    {
        int byte = -1;

        while(byte == -1)
        {
            byte = comm_.read();
        }

        return (int)byte;
    }

    void gets(char * str)
    {
        char c;
        while(c = (char)readByte())
        {
            LOG_INFO("%c", c);
            *str++ = c;
        }
        *str = 0;
    }
};
}

#endif // SERIALMESSAGE_MESSAGE_SERVER_H
