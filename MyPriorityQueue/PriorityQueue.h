#pragma once

#include "LinkedList.h"

#include <algorithm>
#include <iostream>

enum class HeapType
{
	NONE,
	MIN,
	MAX,
};

template<typename T>
class PriorityQueue
{
	using PriorityComparator = bool (*)(const T& lower, const T& higher);
	using NodePtr = Node<T>*;

public:
	PriorityQueue(const T* pArrFirst, const T* pArrLast, int queueSize, PriorityComparator fPriorityComparator);
	PriorityQueue(int queueSize, PriorityComparator fPriorityComparator);
	~PriorityQueue();

	void		insert(const T& data);
	T			extract(HeapType heapType);

	const T&	getHighest() const { return m_pMaxHeapArr[0]; }
	const T&	getLowest() const { return m_pMinHeapArr[0]; }
	NodePtr*	getPtrMaxHeapArr() const { return m_pMaxHeapArr; }
	NodePtr*	getPtrMinHeapArr() const { return m_pMinHeapArr; }
	int			getHeapSize() const { return m_heapSize; }
	bool		isEmpty() const { return m_pList->isEmpty(); }

	static void sort(T* arr, int arrSize, PriorityComparator fPriorityComparator);

private:
	PriorityQueue() = default;

	void		heapifyMax(int rootIdx);
	void		heapifyMin(int rootIdx);
	void		buildHeap(HeapType heapType = HeapType::NONE);
	void		movePtrNodeUpward(int targetIdx, HeapType heapType);

	int			getParentIdx(int childIdx) const { return (childIdx - 1) / 2; }
	int			getLeftIdx(int parentIdx) const { return parentIdx * 2 + 1; }
	int			getRightIdx(int parentIdx) const { return parentIdx * 2 + 2; }

private:
	LinkedList<T>*		m_pList = nullptr;
	NodePtr*			m_pMaxHeapArr = nullptr;
	NodePtr*			m_pMinHeapArr = nullptr;
	int					m_arrSize = 0;
	int					m_heapSize = 0;
	PriorityComparator	m_fPriorityComparator = nullptr;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const PriorityQueue<T>& pq)
{
	Node<T>** pMaxHeapArr = pq.getPtrMaxHeapArr();
	Node<T>** pMinHeapArr = pq.getPtrMinHeapArr();
	const int heapSize = pq.getHeapSize();

	// 노드가 없는 경우
	if (pq.isEmpty())
	{
		os << "PriorityQueue is empty..";
		return os;
	}

	// 최대 힙 출력
	if (pMaxHeapArr != nullptr)
	{
		os << "Max heap: ";

		for (int arrIdx = 0; arrIdx < heapSize; ++arrIdx)
		{
			os << pMaxHeapArr[arrIdx]->data << " ";
		}
	}
	os << std::endl;

	// 최소 힙 출력
	if (pMinHeapArr != nullptr)
	{
		os << "Min heap: ";

		for (int arrIdx = 0; arrIdx < heapSize; ++arrIdx)
		{
			os << pMinHeapArr[arrIdx]->data << " ";
		}
	}
	os << std::endl;

	return os;
}

template<typename T>
inline PriorityQueue<T>::PriorityQueue(const T* pArrFirst, const T* pArrLast, int queueSize, PriorityComparator fPriorityComparator)
{
	const int numArrElem = static_cast<int>(pArrLast - pArrFirst);

	// 배열 원소 개수는 큐의 크기를 넘을 수 없다
	assert(numArrElem <= queueSize);

	// 연결 리스트 초기화
	m_pList = new LinkedList<T>(pArrFirst, pArrLast);

	// 힙 배열 생성
	m_pMaxHeapArr = new NodePtr[queueSize];
	m_pMinHeapArr = new NodePtr[queueSize];

	// 힙 배열 초기화
	std::fill(m_pMaxHeapArr, m_pMaxHeapArr + queueSize, nullptr);
	std::fill(m_pMinHeapArr, m_pMinHeapArr + queueSize, nullptr);

	NodePtr pCur = m_pList->getPtrFirst();
	
	// 힙 배열의 원소에 연결 리스트 노드의 주소를 넣는다
	for (int arrIdx = 0; arrIdx < numArrElem; ++arrIdx)
	{
		m_pMaxHeapArr[arrIdx] = pCur;
		m_pMinHeapArr[arrIdx] = pCur;

		pCur = pCur->pNext;
	}

	// 힙 배열 크기와 배열 내의 힙 크기, 우선순위 판단 함수 설정
	m_arrSize = queueSize;
	m_heapSize = numArrElem;
	m_fPriorityComparator = fPriorityComparator;

	// 힙 크기만큼 배열내의 원소들을 힙 구조로 재배열
	buildHeap();
}

template<typename T>
inline PriorityQueue<T>::PriorityQueue(int queueSize, PriorityComparator fPriorityComparator)
{
	// 연결 리스트 초기화
	m_pList = new LinkedList<T>();

	// 힙 배열 생성
	m_pMaxHeapArr = new NodePtr[queueSize];
	m_pMinHeapArr = new NodePtr[queueSize];

	// 힙 배열 초기화
	std::fill(m_pMaxHeapArr, m_pMaxHeapArr + queueSize, nullptr);
	std::fill(m_pMinHeapArr, m_pMinHeapArr + queueSize, nullptr);

	// 힙 배열 크기와 배열 내의 힙 크기, 우선순위 판단 함수 설정
	m_arrSize = queueSize;
	m_heapSize = 0;
	m_fPriorityComparator = fPriorityComparator;
}

template<typename T>
inline PriorityQueue<T>::~PriorityQueue()
{
	if (m_pMaxHeapArr != nullptr)
	{
		delete[] m_pMaxHeapArr;
	}

	if (m_pMinHeapArr != nullptr)
	{
		delete[] m_pMinHeapArr;
	}
	
	if (m_pList != nullptr)
	{
		delete m_pList;
	}
}

template<typename T>
inline void PriorityQueue<T>::heapifyMax(int rootIdx)
{
	// 루트 노드가 리프 노드가 아니면 반복
	while (getLeftIdx(rootIdx) < m_heapSize)
	{
		int highestIdx = rootIdx;
		const int leftIdx = getLeftIdx(rootIdx);
		const int rightIdx = getRightIdx(rootIdx);

		// 왼쪽 자식 노드의 우선순위가 높으면 highestIdx를 leftIdx로 설정
		if (m_fPriorityComparator(m_pMaxHeapArr[highestIdx]->data, m_pMaxHeapArr[leftIdx]->data))
		{
			highestIdx = leftIdx;
		}

		// 오른쪽 노드가 highestIdx 노드보다 우선순위가 높으면 highestIdx를 rightIdx로 설정
		if ((rightIdx < m_heapSize) && 
			m_fPriorityComparator(m_pMaxHeapArr[highestIdx]->data, m_pMaxHeapArr[rightIdx]->data))
		{
			highestIdx = rightIdx;
		}

		// highestIdx가 rootIdx면 max heap property를 만족하므로 반복 종료
		if (highestIdx == rootIdx)
		{
			break;
		}

		// highestIdx와 rootIdx의 노드 포인터를 스왑하고 rootIdx를 highestIdx로 설정
		std::swap(m_pMaxHeapArr[highestIdx], m_pMaxHeapArr[rootIdx]);
		rootIdx = highestIdx;
	}
}

template<typename T>
inline void PriorityQueue<T>::heapifyMin(int rootIdx)
{
	// 루트 노드가 리프 노드가 아니면 반복
	while (getLeftIdx(rootIdx) < m_heapSize)
	{
		int lowestIdx = rootIdx;
		const int leftIdx = getLeftIdx(rootIdx);
		const int rightIdx = getRightIdx(rootIdx);

		// 왼쪽 자식 노드의 우선순위가 낮으면 lowestIdx를 leftIdx로 설정
		if (m_fPriorityComparator(m_pMinHeapArr[leftIdx]->data, m_pMinHeapArr[lowestIdx]->data))
		{
			lowestIdx = leftIdx;
		}

		// 오른쪽 자식 노드가 lowestIdx 노드보다 우선순위가 낮으면 lowestIdx를 rightIdx로 설정
		if ((rightIdx < m_heapSize) &&
			m_fPriorityComparator(m_pMinHeapArr[rightIdx]->data, m_pMinHeapArr[lowestIdx]->data))
		{
			lowestIdx = rightIdx;
		}

		// lowestIdx가 rootIdx면 min heap property를 만족하므로 반복 종료
		if (lowestIdx == rootIdx)
		{
			break;
		}

		// lowestIdx와 rootIdx의 노드 포인터를 스왑하고 rootIdx를 lowestIdx로 설정
		std::swap(m_pMinHeapArr[lowestIdx], m_pMinHeapArr[rootIdx]);
		rootIdx = lowestIdx;
	}
}

template<typename T>
inline void PriorityQueue<T>::buildHeap(HeapType heapType)
{
	const int numInternalNodes = m_heapSize / 2;

	// 모든 노드가 heap property를 만족하도록 만든다
	for (int rootIdx = numInternalNodes - 1; rootIdx >= 0; --rootIdx)
	{
		if ((heapType == HeapType::NONE) || (heapType == HeapType::MAX))
		{
			heapifyMax(rootIdx);
		}

		if ((heapType == HeapType::NONE) || (heapType == HeapType::MIN))
		{
			heapifyMin(rootIdx);
		}
	}
}
