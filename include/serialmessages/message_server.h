
#ifndef SERIALMESSAGE_MESSAGE_SERVER_H
#define SERIALMESSAGE_MESSAGE_SERVER_H

#include <simplelogger/simplelogger.h>

namespace serialmessages
{
/**
	\class MessageServer
*/
template<typename CommT>
class MessageServer
{
public:

	template<typename... Args>
	MessageServer(Args&... args) : comm_(args...)
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
        static const char sequence[] = "Hello World";
        static const size_t len = sizeof(sequence);

        comm_.write((uint8_t*)sequence, len);

        int byte = comm_.read();

        if(byte >= 0)
        {
        	if(byte != 0)
        		LOG_DEBUG("- %c", (char)byte);
        }
        else
        {

        }
    }

private:
	CommT comm_;
};
}

#endif // SERIALMESSAGE_MESSAGE_SERVER_H
