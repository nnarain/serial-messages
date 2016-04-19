
#ifndef SERIALMESSAGES_RING_BUFFER_H
#define SERIALMESSAGES_RING_BUFFER_H

#include <string.h>
#include <stddef.h>

namespace serialmessages
{
template<typename T, size_t BUFFER_SIZE>
class RingBuffer
{
public:

	RingBuffer() :
		read_idx_(0),
		write_idx_(0)
	{
		// zero memory
		memset(buffer_, 0, sizeof(T) * BUFFER_SIZE);
	}

	~RingBuffer()
	{
	}

	void put(const T& t)
	{
		buffer_[write_idx_] = t;
		write_idx_ = next(write_idx_);
		size_++;
	}

	T get()
	{
		T t = buffer_[read_idx_];
		read_idx_ = next(read_idx_);
		size_--;

		return t;
	}

	size_t size()
	{
		return size_;
	}

private:
	T buffer_[BUFFER_SIZE];

	size_t read_idx_;
	size_t write_idx_;

	size_t size_;

private:

	inline size_t next(size_t idx)
	{
		return (idx + 1) % BUFFER_SIZE;
	}
};
}

#endif // SERIALMESSAGES_RING_BUFFER_H

