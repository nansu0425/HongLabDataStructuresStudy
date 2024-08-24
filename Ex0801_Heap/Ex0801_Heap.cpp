#include <iostream>
#include <algorithm>

#include "../shared/MaxHeap.h"

using namespace std;

template<typename T>
void PrintArr(T* arr, int numElem)
{
	for (int idx = 1; idx <= numElem; ++idx)
	{
		std::cout << arr[idx] << " ";
	}

	std::cout << endl;
}

template<typename T>
void DoHeapSort(T* arr, int numElem)
{
	PrintArr(arr, numElem);

	int heapSize = 1;

	// 배열의 모든 원소를 힙에 추가
	while (heapSize < numElem)
	{
		++heapSize;

		int curIdx = heapSize;
		int newItem = arr[curIdx];

		// 현재 루트 노드가 아니고 새로운 아이템이 부모 노드 아이템보다 큰 경우 
		// 부모 노드 아이템을 현재 노드로 이동
		while (curIdx != 1 && arr[curIdx / 2] < newItem)
		{
			arr[curIdx] = arr[curIdx / 2];
			curIdx = curIdx / 2;
		}

		arr[curIdx] = newItem;
	}

	// 힙 크기가 하나가 될 때까지 반복
	while (heapSize > 1)
	{
		// 루트 노드의 아이템을 힙의 마지막 노드로 옮기고
		// 힙 사이즈를 줄인다
		T lastItem = arr[heapSize];
		arr[heapSize] = arr[1];
		--heapSize;

		PrintArr(arr, numElem);

		int curIdx = 1;
		int childIdx = 2;

		// 마지막 아이템을 삽입할 위치를 찾는다
		while (childIdx <= heapSize)
		{
			// 오른쪽 자식 노드의 아이템이 더 큰 경우
			if ((childIdx < heapSize) && (arr[childIdx] < arr[childIdx + 1]))
			{
				++childIdx;
			}

			// 자식 노드 아이템보다 lastItem이 크거나 같은 경우 현재 노드에 lastItem을 넣으면 된다
			if (arr[childIdx] <= lastItem)
			{
				break;
			}

			// 자식 노드 아이템을 현재 노드로 이동
			arr[curIdx] = arr[childIdx];

			// 현재 노드 인덱스와 자식 노드 인덱스 이동
			curIdx = childIdx;
			childIdx = curIdx * 2;
		}

		arr[curIdx] = lastItem;
	}
}

int main()
{
	// 개념 설명은 트리인데 구현은 배열 (연결 표현법으로도 구현 가능)
	// 완전 이진 트리 상태를 유지

	// 부모 노드의 값이 두 자식 노드의 값 이상이어야 한다. 
	// 자식들끼리는 값의 크기 순서 무관. <- 느슨한 정렬 상태
	// 중복된 값을 여러개 넣을 수 있음

	// 배열 사용: 0번은 비워둠
	// 부모 인덱스 * 2 = 왼쪽 자식 인덱스
	// 부모 인덱스 * 2 + 1 = 오른쪽 자식 인덱스
	// 자식인덱스 / 2 = 부모 인덱스

	// 삽입: 일단 맨 마지막에 삽입한 후에 부모 노드로 올린다.
	// 삭제: 가장 마지막 값을 루트로 옮긴 후에 내려 보낸다.

	MaxHeap<int> h;

	for (auto i : { 2, 8, 5, 3, 2, 1, 9, 3, 7 })
	{
		h.Push(i);
		h.Print();
	}

	// 힙 정렬 O(nlogn), Not stable
	while (!h.IsEmpty())
	{
		cout << h.Top() << " ";
		h.Pop();
		// h.Print();
	}
	cout << endl;

	int arr[] = { -1, 12, 11, 13, 5, 6, 7 };
	DoHeapSort(arr, 6);
	PrintArr(arr, 6);

	std::cout << std::endl;

	return 0;
}
