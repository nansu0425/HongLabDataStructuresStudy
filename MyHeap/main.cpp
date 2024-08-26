#include "MaxHeap.h"

int main()
{
	const int arrSize = 6;
	int arr[arrSize] = { 1, 2, 3, 4, 5, 6 };

	MaxHeap<int> mh(arr, arrSize, 1);

	std::cout << "Insertion test" << std::endl;
	
	for (int arrIdx = 1; arrIdx < arrSize; ++arrIdx)
	{
		std::cout << mh << std::endl;
		mh.insert(arr[arrIdx]);
	}

	for (int key = 7; key < 10; ++key)
	{
		std::cout << mh << std::endl;
		mh.insert(key);
	}
	std::cout << mh << std::endl;

	std::cout << "--------------------------------" << std::endl;

	std::cout << "Extraction test" << std::endl;
	
	for (int count = 0; count < arrSize; ++count)
	{
		std::cout << mh.extractMax() << " ";
	}
	std::cout << std::endl;

	std::cout << "--------------------------------" << std::endl;

	{
		std::cout << "Heapsort test" << std::endl;

		const int arrSize = 10;
		int arr[arrSize] = { 6, -1, 5, 10, 4, 2, 3, 1, 9, 8 };
		MaxHeap<int>::sort(arr, arrSize);

		for (int arrIdx = 0; arrIdx < arrSize; ++arrIdx)
		{
			std::cout << arr[arrIdx] << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}