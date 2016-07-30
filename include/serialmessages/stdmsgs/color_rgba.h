/**
	RGBA message
	
	\file color_rgba.h
	\author Natesh Narain <nnaraindev@gmail.com>
*/

#ifndef SERIALMESSAGES_STDMSGS_COLORRGBA_H
#define SERIALMESSAGES_STDMSGS_COLORRGBA_H

#include "serialmessages/message_base.h"

#include <stdint.h>

namespace serialmessages
{
	namespace stdmsgs
	{
		class ColorRGBA : public MessageBase
		{
		public:
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;

			ColorRGBA() : r(0), g(0), b(0), a(0)
			{
			}

			~ColorRGBA()
			{
			}

			virtual void serialize(SerialStream& ss)
			{
				ss << r << g << b << a;
			}

			virtual void deserialize(SerialStream& ss)
			{
				ss >> r >> g >> b >> a;
			}
		};	
	}
}

#endif // SERIALMESSAGES_STDMSGS_COLORRGBA_H