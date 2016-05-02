# Serial Messages

Rosserial inspired serial messaging library.

This library defines an interface for sending serialized binary messages between a server and a client.

This library is independent of data transport method, as the user of this library, you define a `Communcation` class for sending bytes to either end point. As along as you define a class that can `read` a byte and `write` bytes to your end point, this library will take care of the rest!


Example
-------

Create a server program. Where SerialComm is a user defined type for sending data over the serial port

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}

// server
// main.cpp

#include <serialmessages/message_server.h>
#include <serialmessages/publisher.h>
#include <serialmessages/stdmsgs/string.h>

#include "serial_comm.h"

using namespace serialmessages;

int main()
{
	MessageServer<SerialComm> server;

	Publisher<std::String<>> publisher("my_topic", &server);

	stdmsgs::String my_string;
	my_string.data = "Hello World";

	server.initialize();

	while(true)
	{
		publisher.publish(my_string);
		server.spinOnce();
	}


	return 0;
}

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Create a client.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}

// client
// main.cpp

#include <serialmessages/message_client.h>
#include <serialmessages/subscriber.h>

#include <iostream>

#include "serial_comm.h"

using namespace serialmessages;

void myCallback(stdmsgs::String<>& msg)
{
	std::cout << msg.data << std::endl;
}

int main()
{
	MessageClient<SerialComm> client;
	
	Subscriber<stdmsgs::String<>> subscriber("my_topic", myCallback);

	client.initialize();

	while(true)
	{
		client.spinOnce();
	}

	return 0;
}

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TODO
----

* More detailed example
* added checksum error detection
