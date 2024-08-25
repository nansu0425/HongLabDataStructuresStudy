#pragma once

#include <cassert>
#include <algorithm>
#include <iostream>

template<typename T>
class MaxHeap
{
public:
	MaxHeap() = default;
	MaxHeap(T* arr, int arrSize, int heapSize = 0);

	void			insert(const T& key);
	T				extractMax();

	T				getMax() const { return m_arr[0]; }
	const T*		getArr() const { return m_arr; }
	int				getArrSize() const { return m_arrSize; }
	int				getHeapSize() const { return m_heapSize; }

	static void		sort(T* arr, int arrSize);

private:
	void	heapify(int rootIdx);
	void	heapifyRecursively(int rootIdx);
	void	heapifyIteratively(int rootIdx);
	void	buildHeap();
	void	increaseKey(int targetIdx);

	int		getParentIdx(int childIdx) const { return (childIdx - 1) / 2; }
	int		getLeftIdx(int parentIdx) const { return parentIdx * 2 + 1; }
	int		getRightIdx(int parentIdx) const { return parentIdx * 2 + 2; }

private:
	T*		m_arr = nullptr;
	int		m_arrSize = 0;
	int		m_heapSize = 0;
};

template<typename T>
inline MaxHeap<T>::MaxHeap(T* arr, int arrSize, int heapSize)
{
	assert((arr != nullptr) && (arrSize > 0) && (heapSize <= arrSize));

	m_arr = new T[arrSize];
	m_arrSize = arrSize;
	m_heapSize = heapSize;

	// 멤버 배열에 파라미터 배열을 복사하고 힙 구조로 만든다
	std::copy(arr, arr + m_arrSize, m_arr);
	buildHeap();
}

template<typename T>
inline void MaxHeap<T>::insert(const T& key)
{
	assert(m_heapSize < m_arrSize);

	// 마지막 노드 뒤에 새로운 노드 추가
	++m_heapSize;
	m_arr[m_heapSize - 1] = key;

	// 추가된 노드의 key를 heap property를 만족하는 위치로 올린다
	increaseKey(m_heapSize - 1);
}

template<typename T>
inline T MaxHeap<T>::extractMax()
{
	assert(m_heapSize > 0);

	// 마지막 노드 key와 루트 key를 스왑
	std::swap(m_arr[0], m_arr[m_heapSize - 1]);

	// 마지막 노드를 힙에서 제외
	--m_heapSize;

	// 힙에 노드가 2개 이상이면 heap property가 유지되도록
	// 마지막 노드의 key가 들어온 루트 노드를 heapify
	if (m_heapSize > 1)
	{
		heapify(0);
	}

	// 마지막 노드 뒤의 key는 스왑 전 힙의 루트 key이므로 최댓값
	return m_arr[m_heapSize];
}

template<typename T>
inline void MaxHeap<T>::sort(T* arr, int arrSize)
{
	MaxHeap<T> mh;
	mh.m_arr = arr;
	mh.m_arrSize = arrSize;
	mh.m_heapSize = arrSize;

	// 힙 구조로 만든다
	mh.buildHeap();

	// 힙에 노드가 하나 남을 때까지 반복
	while (mh.m_heapSize > 1)
	{
		// 마지막 노드 key와 루트 key를 스왑한 후
		// 힙 크기를 줄여 마지막 노드를 힙 범위에서 제외시킨다
		mh.extractMax();
	}
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const MaxHeap<T>& mh)
{
	if (mh.getHeapSize() > 0)
	{
		for (int arrIdx = 0; arrIdx < mh.getHeapSize(); ++arrIdx)
		{
			os << mh.getArr()[arrIdx] << " ";
		}
	}

	return os;
}

template<typename T>
inline void MaxHeap<T>::heapify(int rootIdx)
{
	// heapifyRecursively(rootIdx);
	heapifyIteratively(rootIdx);
}

template<typename T>
inline void MaxHeap<T>::heapifyRecursively(int rootIdx)
{
	int largestKeyIdx = rootIdx;
	int leftIdx = getLeftIdx(rootIdx);
	int rightIdx = getRightIdx(rootIdx);

	// 왼쪽 노드의 key가 루트 key보다 크면 
	// 가장 큰 key의 인덱스는 왼쪽 노드 인덱스로 설정
	if ((leftIdx < m_heapSize) &&
		(m_arr[leftIdx] > m_arr[rootIdx]))
	{
		largestKeyIdx = leftIdx;
	}

	// 오른쪽 노드의 key가 가장 큰 key보다 크면 
	// 가장 큰 key는 오른쪽 노드 인덱스가 된다
	if ((rightIdx < m_heapSize) &&
		(m_arr[rightIdx] > m_arr[largestKeyIdx]))
	{
		largestKeyIdx = rightIdx;
	}

	// 가장 큰 key의 인덱스가 루트 인덱스가 아니면
	// 두 key를 스왑하고 루트 key가 들어간 자식 노드를 heapify
	if (largestKeyIdx != rootIdx)
	{
		std::swap(m_arr[largestKeyIdx], m_arr[rootIdx]);
		heapify(largestKeyIdx);
	}
}

template<typename T>
inline void MaxHeap<T>::heapifyIteratively(int targetIdx)
{
	// target 노드가 리프 노드가 아니면 반복
	while (getLeftIdx(targetIdx) < m_heapSize)
	{
		int largestIdx = targetIdx;
		const int leftIdx = getLeftIdx(targetIdx);
		const int rightIdx = getRightIdx(targetIdx);

		// 왼쪽 자식 노드의 key가 더 크면 largestIdx를 왼쪽 노드 인덱스로 설정
		if (m_arr[targetIdx] < m_arr[leftIdx])
		{
			largestIdx = leftIdx;
		}

		// 오른쪽 노드가 largestIdx의 노드 key보다 크면 
		// largestIdx를 오른쪽 노드 인덱스로 설정
		if ((rightIdx < m_heapSize) && (m_arr[largestIdx] < m_arr[rightIdx]))
		{
			largestIdx = rightIdx;
		}

		// largestIdx가 자식 노드 인덱스가 아니면 heap property를 만족하므로 반복 종료
		if (largestIdx == targetIdx)
		{
			break;
		}

		// largestIdx와 targetIdx의 key를 스왑하고 targetIdx를 largestIdx로 설정
		std::swap(m_arr[largestIdx], m_arr[targetIdx]);
		targetIdx = largestIdx;
	}
}

template<typename T>
inline void MaxHeap<T>::buildHeap()
{
	const int numInternalNodes = m_heapSize / 2;

	// 모든 노드가 heap property를 만족하도록 만든다
	for (int rootIdx = numInternalNodes - 1; rootIdx >= 0; --rootIdx)
	{
		heapify(rootIdx);
	}
}

template<typename T>
inline void MaxHeap<T>::increaseKey(int targetIdx)
{
	// targetIdx 노드의 key가 부모 노드 key보다 클때 
	// key를 스왑하고 parentIdx를 targetIdx로 설정
	while ((targetIdx != 0) && (m_arr[getParentIdx(targetIdx)] < m_arr[targetIdx]))
	{
		std::swap(m_arr[getParentIdx(targetIdx)], m_arr[targetIdx]);
		targetIdx = getParentIdx(targetIdx);
	}
}
