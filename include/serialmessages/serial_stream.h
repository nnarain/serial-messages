
#ifndef SERIALMESSAGES_SERIAL_STREAM_H
#define SERIALMESSAGES_SERIAL_STREAM_H

#include <stdint.h>

/**
	\class SerialStream
*/
class SerialStream
{
public:

	SerialStream(uint8_t * buffer, size_t len) : 
        buffer_(buffer), 
        length_(len),
        input_ptr_(buffer),
        output_ptr_(buffer)
	{
	}

	~SerialStream()
	{
	}
    
    void reset()
    {
        input_ptr_  = buffer_;
        output_ptr_ = buffer_;
    }
    
    /* Overloaded operators to serialize/deserialize types */
    
    // ...

private:
	uint8_t* buffer_; ///< Pointer to a buffer of data
    size_t length_;   ///< Length of the buffer
    
    uint8_t* input_ptr_; ///< Pointer to the next byte to be serialized
    uint8_t* output_ptr_; ///< Pointer to the next byte to be deserialized
};

#endif // SERIALMESSAGES_SERIAL_STREAM_H
