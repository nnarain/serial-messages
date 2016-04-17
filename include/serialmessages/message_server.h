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

                // read next byte for number of messages the client wants to send
                uint8_t num_bytes = readByte();

                // if intent to send message
                while(num_bytes--)
                {
                    readMessage();
                }

                // send byte indicating number of messages we want to send to client

                //
                sync_ = false;
            }
        }
    }

    void subscribe(SubscriberBase* subscriber)
    {
        subscribers_.put(subscriber);
    }

private:
    CommT comm_;
    MessageProtocol protocol_;
    bool sync_;

    HashTable<SubscriberBase, 10> subscribers_;

private:
    void readMessage()
    {
        uint8_t in_buffer[IN_BUFFER_SIZE];

        // read topic string
        gets((char *)in_buffer);

        // read 4 bytes for message length
        size_t topic_length = strlen((const char *)in_buffer) + 1;
        readBytes(in_buffer + topic_length, 4);

        // deserialize topic string and message length
        stdmsgs::String topic;
        uint32_t message_length;

        SerialStream ss(in_buffer, IN_BUFFER_SIZE);
        topic.deserialize(ss);
        ss >> message_length;

        // read message
        readBytes(in_buffer + topic_length + 4, message_length);

        // get subscriber using topic string as key
        SubscriberBase* subscriber = subscribers_.get(topic.data);

        if(subscriber)
        {
            // call subscriber callback
            subscriber->callback(ss);
        }
        else
        {
            LOG_WARN("No subscriber for /%s", topic.data);
        }
    }

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
            *str++ = c;
        }
        *str = 0;
    }
};
}

#endif // SERIALMESSAGE_MESSAGE_SERVER_H
