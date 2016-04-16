
#ifndef SERIALMESSAGES_STDMSGS_STRING_H
#define SERIALMESSAGES_STDMSGS_STRING_H

#include "serialmessages/message_base.h"

namespace serialmessages
{
	namespace stdmsgs
	{
	class String : public MessageBase
	{
	public:
		const char * data;

	public:
		String() : data(0)
		{
		}

		virtual void serialize(SerialStream& stream)
		{
			stream << data;
		}

		virtual void deserialize(SerialStream& stream)
		{
			stream >> in_buffer;
			data = (const char *)in_buffer;
		}

	private:
		char in_buffer[128];
	};
	}
}

#endif // SERIALMESSAGES_STDMSGS_STRING_H
