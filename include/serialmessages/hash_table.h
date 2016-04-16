
#ifndef SERIALMESSAGES_HASH_TABLE_H
#define SERIALMESSAGES_HASH_TABLE_H

#include <cstring>
#include <cstdint>

namespace serialmessages
{

/**
	\class HashTable
	\brief Class wrapping key,value paired data. Not for public use!
*/
template<typename ItemT, unsigned int BUFFER_SIZE>
class HashTable
{
public:
	HashTable()
	{
		// zero memory
		memset(items_, 0, BUFFER_SIZE * sizeof(ItemT));
		memset(collision_flags_, 0, BUFFER_SIZE * sizeof(uint16_t));
	}

	~HashTable()
	{
	}

	bool putItem(ItemT* item)
	{
		uint16_t hashKey = hash(item->topic);
		uint16_t idx = hashKey % BUFFER_SIZE;

		// check if the slot is not empty
		if(items_[idx] != 0 && items_[idx] != item)
		{
			// mark this index as a collision
			collision_flags_[idx] = true;

			// search for empty slot
			for(uint16_t i = idx + 1; (i+1) < idx; i = (i + 1) % BUFFER_SIZE)
			{
				if(items_[i] == 0)
				{
					items_[i] = item;
					return true;
				}
			}

			// searched entire buffer, failed to find empty slot
			return false;
		}
		else
		{
			items_[idx] = item;
			collision_flags_[idx] = false;
			return true;
		}

		// nope
		return false;
	}

	ItemT* getItem(const char * key)
	{
		uint16_t hashKey = hash(key);
		uint16_t idx = hashKey % BUFFER_SIZE;

		// check if this index had a collision
		if(collision_flags_[idx])
		{
			// search for empty slot
			for(uint16_t i = idx + 1; (i+1) < idx; i = (i + 1) % BUFFER_SIZE)
			{
				if(strcmp(key, items_[i]->topic) == 0)
				{
					return items_[i];
				}
			}

			// search failed
			return 0;
		}
		else
		{
			return items_[idx];
		}

		// nope
		return 0;
	}

private:
	uint16_t hash(const char * str)
	{
		// http://stackoverflow.com/questions/7666509/hash-function-for-string

		uint16_t hash = 5381;
		int c;

		while(c = *str++)
			hash = ((hash << 5) + hash) + c;

		return hash;
	}


private:
	ItemT *items_[BUFFER_SIZE];
	bool collision_flags_[BUFFER_SIZE];
};

}

#endif // SERIALMESSAGES_HASH_TABLE_H
