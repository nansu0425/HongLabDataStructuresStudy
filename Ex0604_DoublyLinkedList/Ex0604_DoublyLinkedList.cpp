#include <iostream>

#include "../shared/DoublyLinkedList.h"

using namespace std;

int main()
{
	DoublyLinkedList<int> list;

	list.Print();

	list.PushFront(3);
	list.Print();

	list.PushBack(4);
	list.Print();

	list.PushFront(2);
	list.Print();

	list.PushFront(1);
	list.Print();

	list.PushBack(5);
	list.Print();

	DoublyLinkedList<int> list2(list);
	list2.Print();

	list2.Reverse();
	list2.Print();

	DoublyLinkedList<int>::Node* temp = list2.Find(3);
	list2.InsertBack(temp, 1000);
	list2.Print();

	list2.Reverse();
	list2.Print();

	// PopFront()/Back() 테스트
	for (int i = 0; i < 5; i++)
	{
		list2.PopFront();
		list2.Print();

		/*list.PopBack();
		list.Print();*/

		if (!list2.IsEmpty())
			cout << list2.Front() << " " << list2.Back() << endl;
	}

	// Edge case 테스트 주의

	return 0;
}
