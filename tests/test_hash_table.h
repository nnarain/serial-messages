
#include <cxxtest/TestSuite.h>
#include <stdint.h>
#include <string.h>

#include <iostream>

#include <serialmessages/hash_table.h>

namespace sm = serialmessages;

class HashTableTestSuite : public CxxTest::TestSuite
{
public:
	struct Item
	{
		const char * topic;
	};

	void testGetAndPut()
	{
		sm::HashTable<Item, 10> table;

		Item item1;
		item1.topic = "item_key";

		table.put(&item1);

		Item* item_out;
		item_out = table.get("item_key");

		TS_ASSERT(item_out == &item1);
	}

	void testGetAndPutMultiple()
	{
		sm::HashTable<Item, 10> table;

		Item item1;
		item1.topic = "apples";

		Item item2;
		item2.topic = "oranges";

		Item item3;
		item3.topic = "grapes";

		Item item4;
		item4.topic = "carrots";

		Item item5;
		item5.topic = "pears";

		table.put(&item1);
		table.put(&item2);
		table.put(&item3);
		table.put(&item4);
		table.put(&item5);

		Item* item_out1;
		Item* item_out2;
		Item* item_out3;
		Item* item_out4;
		Item* item_out5;

		item_out1 = table.get("apples");
		item_out2 = table.get("oranges");
		item_out3 = table.get("grapes");
		item_out4 = table.get("carrots");
		item_out5 = table.get("pears");

		TS_ASSERT(item_out1 == &item1);
		TS_ASSERT(item_out2 == &item2);
		TS_ASSERT(item_out3 == &item3);
		TS_ASSERT(item_out4 == &item4);
		TS_ASSERT(item_out5 == &item5);
	}
};
