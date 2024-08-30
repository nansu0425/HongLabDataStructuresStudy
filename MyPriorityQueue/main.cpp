#include <iostream>

#include "PriorityQueue.h"

int main()
{
	const int arrSize = 7;
	int arr[arrSize] = { 3, 2, 1, 6, 5, 7, 4 };
	auto fPriorityComparator = [](const int& lower, const int& higher)->bool { return lower < higher; };

	PriorityQueue<int> pq(arr, arrSize, 10, fPriorityComparator);
	std::cout << pq << std::endl;

	/*pq.insert(8);
	pq.insert(0);

	std::cout << pq << std::endl;*/

	return 0;
}
