#include <iostream>

using namespace std;

int rec_count = 0;

int InterpolationSearch(int arr[], int lowIdx, int highIdx, int x)
{
	rec_count++;

	if (lowIdx <= highIdx && x >= arr[lowIdx] && x <= arr[highIdx])
	{
		// int targetIdx = (lowIdx + highIdx) / 2; // 이진 탐색 (중간)

		// 보간 탐색
		int targetIdx = (highIdx - lowIdx) * (x - arr[lowIdx]) / (arr[highIdx] - arr[lowIdx]) + lowIdx;

		if (arr[targetIdx] == x)
			return targetIdx;

		if (arr[targetIdx] < x)
			return InterpolationSearch(arr, targetIdx + 1, highIdx, x);

		if (arr[targetIdx] > x)
			return InterpolationSearch(arr, lowIdx, targetIdx - 1, x);
	}

	return -1;
}

// https://www.geeksforgeeks.org/interpolation-search/
int main()
{
	int arr[] = { 10, 12, 13, 16, 18, 19, 20, 21, 22, 23, 24, 33, 35, 42, 47 };
	int n = sizeof(arr) / sizeof(arr[0]);

	for (auto x : { 10, 18, 33, 47, -1, 99 })
	{
		rec_count = 0;

		int index = InterpolationSearch(arr, 0, n - 1, x);

		if (index != -1)
			cout << x << " was found at index " << index << endl;
		else
			cout << x << " was not found." << endl;

		cout << "rec_count = " << rec_count << endl;
		cout << endl;
	}

	return 0;
}