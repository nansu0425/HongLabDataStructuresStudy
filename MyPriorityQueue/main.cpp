#include <iostream>

#include "PriorityQueue.h"

int main()
{
	const int arrSize = 7;
	int arr[arrSize] = { 3, 2, 1, 6, 5, 7, 4 };
	auto fPriorityComparator = [](const int& lower, const int& higher)->bool { return lower < higher; };

	PriorityQueue<int> pq(arr, arrSize, 10, fPriorityComparator);
	
	std::cout << pq << std::endl;
	// Max heap: 7 6 4 2 5 1 3
	// Min heap: 1 2 3 6 5 7 4

	pq.insert(8);
	pq.insert(0);

	std::cout << pq << std::endl;
	// Max heap: 8 7 4 6 5 1 3 2 0
	// Min heap: 0 1 3 2 5 7 4 8 6

	pq.insert(9);
	pq.insert(10);
	pq.insert(11);
	pq.insert(-1);

	std::cout << pq << std::endl;
	// Max heap: 11 9 10 8 7 4 3 2 6 5
	// Min heap: 2 5 3 6 9 7 4 8 10 11

	for (int count = 0; count < 3; ++count)
	{
		std::cout << pq.extract(HeapType::MIN) << " ";
	}
	std::cout << std::endl;
	// 2 3 4

	for (int count = 0; count < 3; ++count)
	{
		std::cout << pq.extract(HeapType::MAX) << " ";
	}
	std::cout << std::endl << std::endl;
	// 11 10 9

	std::cout << pq << std::endl;
	// Max heap: 8 7 6 5
	// Min heap: 5 6 7 8

	return 0;
}
