
/**
	String message
	
	\file string.h
	\author Natesh Narain <nnaraindev@gmail.com>
*/

#ifndef SERIALMESSAGES_STDMSGS_STRING_H
#define SERIALMESSAGES_STDMSGS_STRING_H

#include "serialmessages/message_base.h"

namespace serialmessages
{
	namespace stdmsgs
	{
	template<unsigned int BUFFER_SIZE=64>
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
			stream >> str;
			data = (const char *)str;
		}

	private:
		char str[BUFFER_SIZE];
	};
	}
}

#endif // SERIALMESSAGES_STDMSGS_STRING_H
