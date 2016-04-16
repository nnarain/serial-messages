#ifndef SERIALMESSAGES_MESSAGE_BASE_H
#define SERIALMESSAGES_MESSAGE_BASE_H

#include "serial_stream.h"

#include <stdint.h>

namespace serialmessages
{
class MessageBase
{
public:

	virtual void serialize(SerialStream& stream) = 0;
	virtual void deserialize(SerialStream& stream) = 0;

protected:

private:
};
}

#endif // SERIALMESSAGES_MESSAGE_BASE_H
