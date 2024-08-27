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

	// �迭 ���� ������ ť�� ũ�⸦ ���� �� ����
	assert(numArrElem <= queueSize);

	// ���� ����Ʈ �ʱ�ȭ
	m_pList = new LinkedList<T>(pArrFirst, pArrLast);

	// �� �迭 ����
	m_pMaxHeapArr = new NodePtr[queueSize];
	m_pMinHeapArr = new NodePtr[queueSize];

	// �� �迭 �ʱ�ȭ
	std::fill(m_pMaxHeapArr, m_pMaxHeapArr + queueSize, nullptr);
	std::fill(m_pMinHeapArr, m_pMinHeapArr + queueSize, nullptr);

	NodePtr pCur = m_pList->getPtrFirst();
	
	// �� �迭�� ���ҿ� ���� ����Ʈ ����� �ּҸ� �ִ´�
	for (int arrIdx = 0; arrIdx < numArrElem; ++arrIdx)
	{
		m_pMaxHeapArr[arrIdx] = pCur;
		m_pMinHeapArr[arrIdx] = pCur;

		pCur = pCur->pNext;
	}

	// �� �迭 ũ��� �迭 ���� �� ũ�� ����
	m_arrSize = queueSize;
	m_heapSize = numArrElem;

	// �� ũ�⸸ŭ �迭���� ���ҵ��� �� ������ ��迭
	buildHeap();

	// �켱������ �Ǵ��ϴ� �Լ� ������ ����
	m_fPriorityComparator = fPriorityComparator;
}

template<typename T>
inline PriorityQueue<T>::PriorityQueue(int queueSize, PriorityComparator fPriorityComparator)
{
	// ���� ����Ʈ �ʱ�ȭ
	m_pList = new LinkedList<T>();

	// �� �迭 ����
	m_pMaxHeapArr = new NodePtr[queueSize];
	m_pMinHeapArr = new NodePtr[queueSize];

	// �� �迭 �ʱ�ȭ
	std::fill(m_pMaxHeapArr, m_pMaxHeapArr + queueSize, nullptr);
	std::fill(m_pMinHeapArr, m_pMinHeapArr + queueSize, nullptr);

	// �� �迭 ũ��� �迭 ���� �� ũ�� ����
	m_arrSize = queueSize;
	m_heapSize = 0;

	// �켱������ �Ǵ��ϴ� �Լ� ������ ����
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
