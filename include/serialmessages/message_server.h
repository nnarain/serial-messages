
#ifndef SERIALMESSAGE_MESSAGE_SERVER_H
#define SERIALMESSAGE_MESSAGE_SERVER_H

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

#endif // SERIALMESSAGE_MESSAGE_SERVER_H
