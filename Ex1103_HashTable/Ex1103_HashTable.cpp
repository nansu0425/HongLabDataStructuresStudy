#include <iostream>
#include <string> // std::string

using namespace std;

template<typename K, typename V>
class HashTable
{
public:
	struct Item
	{
		K key = K();
		V value = V();
	};

	HashTable(const int& cap = 8)
	{
		capacity_ = cap;
		table_ = new Item[capacity_];
	}

	~HashTable()
	{
		delete[] table_;
	}

	// Quadratic probing
	void Insert(const Item& item)
	{
		if (numKeys_ == capacity_)
		{
			std::cout << "Failed to insert: table is full" << std::endl;
			return;
		}

		size_t index = HashFunc(item.key) % capacity_;

		for (int count = 1; count <= capacity_; ++count)
		{
			// 슬롯에 key가 없는 경우
			if (table_[index].key == K())
			{
				table_[index] = item;
				++numKeys_;

				return;
			}
			// 같은 key가 있는 경우
			if (table_[index].key == item.key)
			{
				table_[index].value = item.value;

				return;
			}

			index = (index + count * count) % capacity_;
		}

		std::cout << "Failed to insert: available slot isn't found" << std::endl;
	}

	V Get(const K& key)
	{
		if (numKeys_ == 0)
		{
			std::cout << "Failed to get: table is empty" << std::endl;
			return V();
		}

		size_t index = HashFunc(key) % capacity_;
 
		for (int count = 1; count <= capacity_; ++count)
		{
			// key가 같은 경우
			if (table_[index].key == key)
			{
				return table_[index].value;
			}
			// key가 없는 경우
			if (table_[index].key == K())
			{
				break;
			}

			index = (index + count * count) % capacity_;
		}

		std::cout << "Failed to get: key isn't found" << std::endl;

		return V();
	}

	// 정수 -> 해시값
	// Folding method
	size_t HashFunc(int key)
	{
		size_t hashVal = 0;

		while (key > 0)
		{
			hashVal += key % 1000;
			key /= 1000;
		}

		return hashVal;
	}

	// 문자열을 정수 인덱스(해시값)로 변환
	// Horner's method
	size_t HashFunc(const string& s)
	{
		size_t hashVal = 0;
		int c = 31;

		for (int idx = 0; idx < s.size(); ++idx)
		{
			hashVal = c * hashVal + s.at(idx);
		}

		return hashVal;
	}

	void Print()
	{
		for (int i = 0; i < capacity_; i++)
			cout << i << " : " << table_[i].key << " " << table_[i].value << endl;
		cout << endl;
	}

private:
	Item* table_ = nullptr;
	int capacity_ = 0;
	int numKeys_ = 0;
};

int main()
{
	// 충돌 
	// - 개방 주소법: 선형 조사법
	// - 체이닝: 멤버 변수 Item* table_ 대신에 LinkedList<Item>* table_사용

	// 키: int, 값: int 인 경우
	// 키의 범위가 아주 크고 아이템의 개수는 적을 경우
	//{
	//	using Item = HashTable<int, int>::Item;

	//	HashTable<int, int> h(8);

	//	h.Insert(Item{ 123, 456 });

	//	h.Print();

	//	cout << "Get 123 " << h.Get(123) << endl;

	//	h.Insert(Item{ 1000021, 9898 });

	//	h.Print();

	//	cout << "Get 1000021 " << h.Get(1000021) << endl;

	//	h.Insert(Item{ 1211, 999 }); // 충돌!

	//	h.Print();

	//	cout << "Get 123 " << h.Get(123) << endl;
	//	cout << "Get 1211 " << h.Get(1211) << endl;
	//}

	// 키: std::string, 값: int
	{
		using Item = HashTable<string, int>::Item;

		HashTable<string, int> h(8);

		h.Insert(Item{ "apple", 1 });
		h.Insert(Item{ "orange", 2 });
		h.Insert(Item{ "mandarin", 4 });

		h.Print();

		cout << "apple " << h.Get("apple") << endl;
		cout << "orange " << h.Get("orange") << endl;
		cout << endl;

		h.Print();

		h.Insert(Item{ "tomato", 4 });

		h.Print(); 

		cout << "apple " << h.Get("apple") << endl;
		cout << "orange " << h.Get("orange") << endl;
		cout << "pineapple " << h.Get("pineapple") << endl;
		cout << endl;
	}

	return 0;
}