/**
	String message
	
	\file string.h
	\author Natesh Narain <nnaraindev@gmail.com>
*/

#ifndef SERIALMESSAGES_STDMSGS_COLORRGB_H
#define SERIALMESSAGES_STDMSGS_COLORRGB_H

#include "serialmessages/message_base.h"

#include <stdint.h>

namespace serialmessages
{
	namespace stdmsgs
	{
		class ColorRGB : public MessageBase
		{
		public:
			uint8_t r;
			uint8_t g;
			uint8_t b;

			ColorRGB() : r(0), g(0), b(0)
			{
			}

			~ColorRGB()
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

#endif // SERIALMESSAGES_STDMSGS_COLORRGB_H