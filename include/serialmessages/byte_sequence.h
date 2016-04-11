
#ifndef SERIALMESSAGES_BYTE_SEQUENCe_H
#define SERIALMESSAGES_BYTE_SEQUENCe_H

#include <stddef.h>

namespace serialmessages
{
class ByteSequence
{
public:

	ByteSequence(uint8_t* seq, size_t s) : 
		sequence_(seq),
		length_(s),
		match_count_(0)
	{
	}

	void check(uint8_t byte)
	{
		// check if the incoming byte matches the next byte in the sequence
		if(sequence_[match_count_] == byte)
		{
			match_count_++;
		}
		else
		{
			match_count_ = 0;
		}
	}

	bool match()
	{
		return match_count_ == length_;
	}

	void reset()
	{
		match_count_ = 0;
	}

	uint8_t* data()
	{
		return sequence_;
	}	

	size_t size()
	{
		return length_;
	}

	~ByteSequence()
	{
	}

private:
	uint8_t* sequence_;
	size_t length_;

	size_t match_count_;
};
}

#endif
