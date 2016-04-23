/**
    \file publisher_base.h
    \author Natesh Narain <nnaraindev@gmail.com>
    \date 04-16-2016
*/

#ifndef SERIALMESSAGE_PUBLISHER_BASE_H
#define SERIALMESSAGE_PUBLISHER_BASE_H

#include "serial_stream.h"

namespace serialmessages
{
class PublisherBase
{
public:
	const char * topic;

	PublisherBase(const char * topic) : topic(topic), publish_pending_(false)
	{
	}

	virtual void serializeMessage(SerialStream& stream) = 0;

	bool isPublishPending() const
	{
		return publish_pending_;
	}

	void setPublishPending(bool pending)
	{
		publish_pending_ = pending;
	}

private:
	bool publish_pending_;
};
}

#endif // SERIALMESSAGE_PUBLISHER_BASE_H
