#pragma once

#include "LinkedList.h"

#include <algorithm>

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

	const T&	getHighest() const;
	const T&	getLowest() const;

	static void sort(T* arr, int arrSize, PriorityComparator fPriorityComparator);

private:
	PriorityQueue() = default;

	void		heapify(int rootIdx, HeapType heapType);
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

	// 힙 배열 크기와 배열 내의 힙 크기 설정
	m_arrSize = queueSize;
	m_heapSize = numArrElem;

	// 힙 크기만큼 배열내의 원소들을 힙 구조로 재배열
	buildHeap();

	// 우선순위를 판단하는 함수 포인터 설정
	m_fPriorityComparator = fPriorityComparator;
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

	// 힙 배열 크기와 배열 내의 힙 크기 설정
	m_arrSize = queueSize;
	m_heapSize = 0;

	// 우선순위를 판단하는 함수 포인터 설정
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
