/**
	Test message
	
	\file test.h
	\author Natesh Narain <nnaraindev@gmail.com>
*/

#ifndef SERIALMESSAGES_STDMSGS_TEST_H
#define SERIALMESSAGES_STDMSGS_TEST_H

#include "serialmessages/message_base.h"

#include <stdint.h>

namespace serialmessages
{
	namespace stdmsgs
	{
		class Test : public MessageBase
		{
		public:
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;

			uint8_t x, y, z;

			Test() : r(0), g(0), b(0), a(0), x(0), y(0), z(0)
			{
			}

			~Test()
			{
			}

			virtual void serialize(SerialStream& ss)
			{
				ss << r << g << b;
			}

			virtual void deserialize(SerialStream& ss)
			{
				ss >> r >> g >> b;
			}
		};	
	}
}

#endif // SERIALMESSAGES_STDMSGS_TEST_H