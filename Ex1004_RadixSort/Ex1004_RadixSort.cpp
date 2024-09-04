#include <iostream>
#include "../shared/Queue.h"

using namespace std;

bool CheckSorted(int* arr, int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		if (arr[i] > arr[i + 1])
			return false;
	}

	return true;
}

void Print(int* arr, int size)
{
	for (int i = 0; i < size; i++)
		cout << arr[i] << " ";
	cout << endl;
}

int GetMax(int arr[], int n)
{
	int mx = arr[0];
	for (int i = 1; i < n; i++)
		if (arr[i] > mx)
			mx = arr[i];
	return mx;
}

int main()
{
	int arr[] = { 170, 45, 75, 90, 802, 24, 2, 66 };
	int n = sizeof(arr) / sizeof(arr[0]);

	Print(arr, n);

	Queue<int> queues[10];

	int m = GetMax(arr, n); // 가장 큰 자리수를 찾기 위해서

	for (int exp = 1; m / exp > 0; exp *= 10)
	{
		for (int arrIdx = 0; arrIdx < n; ++arrIdx)
		{
			const int queueIdx = (arr[arrIdx] / exp) % 10;

			queues[queueIdx].Enqueue(arr[arrIdx]);
		}

		int queueIdx = 0;

		for (int arrIdx = 0; arrIdx < n; ++arrIdx)
		{
			while (queues[queueIdx].IsEmpty())
			{
				++queueIdx;
			}


			arr[arrIdx] = queues[queueIdx].Front();
			queues[queueIdx].Dequeue();
		}

		Print(arr, n);
	}

	return 0;
}
