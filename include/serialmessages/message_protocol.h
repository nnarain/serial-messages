/**
    \file message_protocol.h
    \author Natesh Narain <nnaraindev@gmail.com>
*/

#ifndef SERIALMESSAGES_MESSAGE_PROTOCOL_H
#define SERIALMESSAGES_MESSAGE_PROTOCOL_H

#include "byte_sequence.h"
#include "publisher_base.h"
#include "subscriber_base.h"
#include "post_publisher.h"
#include "hash_table.h"
#include "ring_buffer.h"
#include "stdmsgs/string.h"

#define SIGNATURE "SYNCSIGNATURE"
#define ACK "ACK"

namespace serialmessages
{
	// const char * SIGNATURE = "SYNCSIGNATURE";
	// const char * ACK = "ACK";

/**
	\class MessageProtocol
	\brief encapsulate protocol specific information
*/
template<
	class CommT,
	unsigned int IN_BUFFER_SIZE=512,
	unsigned int OUT_BUFFER_SIZE=512
>
class MessageProtocol : public PostPublisher
{
public:

    typedef void(*LogFuncPtr)(uint8_t);

	template<typename... Args>
	MessageProtocol(Args&... args) :
		comm_(args...),
		sync_(false),
		signature_((uint8_t*)SIGNATURE, sizeof(SIGNATURE)),
		acknowledge_((uint8_t*)ACK, sizeof(ACK)),
        log_(0)
	{
	}

	~MessageProtocol()
	{
	}

	virtual void spinOnce() = 0;

    bool initialize()
    {
        return comm_.initialize();
    }

	virtual void postPublisher(PublisherBase* pub)
	{
		publisher_queue_.put(pub);
        pub->setPublishPending(true);
	}

	void subscribe(SubscriberBase* subscriber)
    {
        subscribers_.put(subscriber);
    }

    void setLog(LogFuncPtr log)
    {
        log_ = log;
    }

protected:
    CommT comm_;
    bool sync_;

    HashTable<SubscriberBase, 10> subscribers_;
	RingBuffer<PublisherBase*, 10> publisher_queue_;

	ByteSequence signature_;
	ByteSequence acknowledge_;

    LogFuncPtr log_;

    void readMessage()
    {
        uint8_t in_buffer[IN_BUFFER_SIZE] = {0};

        // read topic string
        gets((char *)in_buffer);

        // read 4 bytes for message length
        size_t topic_length = strlen((const char *)in_buffer) + 1;
        readBytes(in_buffer + topic_length, 4);

        // deserialize topic string and message length
        stdmsgs::String<> topic;
        uint32_t message_length;

        SerialStream ss(in_buffer, IN_BUFFER_SIZE);
        topic.deserialize(ss);
        ss >> message_length;

        // uint8_t available = comm_.available();
        // comm_.write(&available, 1);

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
        }
    }

    void writeMessage()
    {
        // get the next publisher from the queue
        PublisherBase* publisher = publisher_queue_.get();

        // buffer for header
        uint8_t header[64];
        SerialStream header_stream(header, 64);

        // buffer for data
        uint8_t data[OUT_BUFFER_SIZE];
        SerialStream data_stream(data, OUT_BUFFER_SIZE);

        // serialize the message in the stream
        publisher->serializeMessage(data_stream);

        // header content
        header_stream << publisher->topic << (uint32_t)data_stream.size();

        // write header
        comm_.write(header, header_stream.size());
        // write message data
        comm_.write(data, data_stream.size());

        // indicate that this publisher has sent its message
        publisher->setPublishPending(false);
    }

    void readBytes(uint8_t* data, size_t nbytes)
    {
        while(nbytes--)
            *data++ = readByte();
    }

    uint8_t readByte()
    {
        int byte = -1;
        while((byte = comm_.read()) == -1);

        return (uint8_t)byte;
    }

    void gets(char * str)
    {
        char c = -1;
        while(c != 0)
        {
            c = (char)readByte();

            if(c >= 0)
            {
                *str++ = c;
            }
        }
    }

    void log(uint8_t b)
    {
        if(log_) log_(b);
    }
};
}

#endif // SERIALMESSAGES_MESSAGE_PROTOCOL_H
