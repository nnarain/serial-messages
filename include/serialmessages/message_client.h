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
        
    }

private:
	CommT comm_;
};
}

#endif // SERIALMESSAGES_MESSAGE_CLIENT_H

