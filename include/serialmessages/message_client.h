#ifndef SERIALMESSAGES_MESSAGE_CLIENT_H
#define SERIALMESSAGES_MESSAGE_CLIENT_H

namespace serialmessages
{
/**
	\class MessageClient
*/
template<typename CommT>
class MessageClient
{
public:

	template<typename... Args>
	MessageClient(Args... args) : comm_(args...)
	{
	}

	~MessageClient()
	{
	}

    void initialize()
    {
        comm_.initialize();
    }
    
    void spinOnce()
    {
    	int byte = comm_.read();

    	if(byte > 0)
    	{
    		char c = (byte & 0x00FF);
			comm_.write((uint8_t*)&c, 1);
    	}
    }

private:
	CommT comm_;
};
}

#endif // SERIALMESSAGES_MESSAGE_CLIENT_H

