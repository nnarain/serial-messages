
#ifndef SERIALMESSAGES_POST_PUBLISER_H
#define SERIALMESSAGES_POST_PUBLISER_H

#include "publisher_base.h"

namespace serialmessages
{
class PostPublisher
{
public:
	virtual void postPublisher(PublisherBase* pub) = 0;
};
}

#endif // SERIALMESSAGES_POST_PUBLISER_H
