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
	using PtrNode = Node<T>*;

public:
	PriorityQueue(const T* pArrFirst, const T* pArrLast, int queueSize, PriorityComparator fPriorityComparator);
	PriorityQueue(int queueSize, PriorityComparator fPriorityComparator);
	~PriorityQueue();

	void		insert(const T& data);
	T			extract(HeapType heapType);

	const T&	getHighest() const { return m_pMaxHeapArr[0]; }
	const T&	getLowest() const { return m_pMinHeapArr[0]; }
	PtrNode*	getPtrMaxHeapArr() const { return m_pMaxHeapArr; }
	PtrNode*	getPtrMinHeapArr() const { return m_pMinHeapArr; }
	int			getHeapSize() const { return m_heapSize; }
	bool		isEmpty() const { return m_pList->isEmpty(); }
	bool		isFull() const { return (m_arrSize == m_heapSize); }

	static void sort(T* arr, int arrSize, PriorityComparator fPriorityComparator);

private:
	PriorityQueue() = default;

	void		heapifyMax(int rootIdx);
	void		heapifyMin(int rootIdx);
	void		movePtrNodeUpwardMax(int targetIdx);
	void		movePtrNodeUpwardMin(int targetIdx);
	void		buildHeap(HeapType heapType = HeapType::NONE);

	int			getParentIdx(int childIdx) const { return (childIdx - 1) / 2; }
	int			getLeftIdx(int parentIdx) const { return parentIdx * 2 + 1; }
	int			getRightIdx(int parentIdx) const { return parentIdx * 2 + 2; }

private:
	LinkedList<T>*		m_pList = nullptr;
	PtrNode*			m_pMaxHeapArr = nullptr;
	PtrNode*			m_pMinHeapArr = nullptr;
	int					m_arrSize = 0;
	int					m_heapSize = 0;
	PriorityComparator	m_fPriorityComparator = nullptr;
	int					m_maxHeapLowestIdx = -1;
	int					m_minHeapHighestIdx = -1;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const PriorityQueue<T>& pq)
{
	Node<T>** pMaxHeapArr = pq.getPtrMaxHeapArr();
	Node<T>** pMinHeapArr = pq.getPtrMinHeapArr();
	const int heapSize = pq.getHeapSize();

	// ��尡 ���� ���
	if (pq.isEmpty())
	{
		os << "PriorityQueue is empty..";
		return os;
	}

	// �ִ� �� ���
	if (pMaxHeapArr != nullptr)
	{
		os << "Max heap: ";

		for (int arrIdx = 0; arrIdx < heapSize; ++arrIdx)
		{
			os << pMaxHeapArr[arrIdx]->data << " ";
		}
	}
	os << std::endl;

	// �ּ� �� ���
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

	// �迭 ���� ������ ť�� ũ�⸦ ���� �� ����
	assert(numArrElem <= queueSize);

	// ���� ����Ʈ �ʱ�ȭ
	m_pList = new LinkedList<T>(pArrFirst, pArrLast);

	// �� �迭 ����
	m_pMaxHeapArr = new PtrNode[queueSize];
	m_pMinHeapArr = new PtrNode[queueSize];

	// �� �迭 �ʱ�ȭ
	std::fill(m_pMaxHeapArr, m_pMaxHeapArr + queueSize, nullptr);
	std::fill(m_pMinHeapArr, m_pMinHeapArr + queueSize, nullptr);

	PtrNode pCur = m_pList->getPtrFirst();
	
	// �� �迭�� ���ҿ� ���� ����Ʈ ����� �ּҸ� �ִ´�
	for (int arrIdx = 0; arrIdx < numArrElem; ++arrIdx)
	{
		m_pMaxHeapArr[arrIdx] = pCur;
		m_pMinHeapArr[arrIdx] = pCur;

		pCur = pCur->pNext;
	}

	// �� �迭 ũ��� �迭 ���� �� ũ��, �켱���� �Ǵ� �Լ� ����
	m_arrSize = queueSize;
	m_heapSize = numArrElem;
	m_fPriorityComparator = fPriorityComparator;

	// �� ũ�⸸ŭ �迭���� ���ҵ��� �� ������ ��迭
	buildHeap();
}

template<typename T>
inline PriorityQueue<T>::PriorityQueue(int queueSize, PriorityComparator fPriorityComparator)
{
	// ���� ����Ʈ �ʱ�ȭ
	m_pList = new LinkedList<T>();

	// �� �迭 ����
	m_pMaxHeapArr = new PtrNode[queueSize];
	m_pMinHeapArr = new PtrNode[queueSize];

	// �� �迭 �ʱ�ȭ
	std::fill(m_pMaxHeapArr, m_pMaxHeapArr + queueSize, nullptr);
	std::fill(m_pMinHeapArr, m_pMinHeapArr + queueSize, nullptr);

	// �� �迭 ũ��� �迭 ���� �� ũ��, �켱���� �Ǵ� �Լ� ����
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
inline void PriorityQueue<T>::insert(const T& data)
{
	int targetIdx = -1;

	// �� �迭�� ���� �� ����
	if (isFull())
	{
		// TODO: �켱������ ���� ���� ����� data�� ���ο� data�� ���� (���ο� data�� �켱������ �� ���� ��)
	}
	// �� �迭�� ������ �ִ� ���
	else
	{
		// ���ο� ��� �߰�
		m_pList->append(data);

		// �� ũ�� ����
		++m_heapSize;

		// ���� ������ ���Ҵ� ���� �߰��� ��带 ����Ų��
		m_pMaxHeapArr[m_heapSize - 1] = m_pList->getPtrLast();
		m_pMinHeapArr[m_heapSize - 1] = m_pList->getPtrLast();

		targetIdx = m_heapSize - 1;
	}

	assert((-1 < targetIdx) && (targetIdx < m_heapSize));

	movePtrNodeUpwardMax(targetIdx);
	movePtrNodeUpwardMin(targetIdx);
}

template<typename T>
inline void PriorityQueue<T>::heapifyMax(int rootIdx)
{
	// ��Ʈ ��尡 ���� ��尡 �ƴϸ� �ݺ�
	while (getLeftIdx(rootIdx) < m_heapSize)
	{
		int highestIdx = rootIdx;
		const int leftIdx = getLeftIdx(rootIdx);
		const int rightIdx = getRightIdx(rootIdx);

		// ���� �ڽ� ����� �켱������ ������ highestIdx�� leftIdx�� ����
		if (m_fPriorityComparator(m_pMaxHeapArr[highestIdx]->data, m_pMaxHeapArr[leftIdx]->data))
		{
			highestIdx = leftIdx;
		}

		// ������ ��尡 highestIdx ��庸�� �켱������ ������ highestIdx�� rightIdx�� ����
		if ((rightIdx < m_heapSize) && 
			m_fPriorityComparator(m_pMaxHeapArr[highestIdx]->data, m_pMaxHeapArr[rightIdx]->data))
		{
			highestIdx = rightIdx;
		}

		// highestIdx�� rootIdx�� max heap property�� �����ϹǷ� �ݺ� ����
		if (highestIdx == rootIdx)
		{
			break;
		}

		// highestIdx�� rootIdx�� ��� �����͸� �����ϰ� rootIdx�� highestIdx�� ����
		std::swap(m_pMaxHeapArr[highestIdx], m_pMaxHeapArr[rootIdx]);
		rootIdx = highestIdx;
	}
}

template<typename T>
inline void PriorityQueue<T>::heapifyMin(int rootIdx)
{
	// ��Ʈ ��尡 ���� ��尡 �ƴϸ� �ݺ�
	while (getLeftIdx(rootIdx) < m_heapSize)
	{
		int lowestIdx = rootIdx;
		const int leftIdx = getLeftIdx(rootIdx);
		const int rightIdx = getRightIdx(rootIdx);

		// ���� �ڽ� ����� �켱������ ������ lowestIdx�� leftIdx�� ����
		if (m_fPriorityComparator(m_pMinHeapArr[leftIdx]->data, m_pMinHeapArr[lowestIdx]->data))
		{
			lowestIdx = leftIdx;
		}

		// ������ �ڽ� ��尡 lowestIdx ��庸�� �켱������ ������ lowestIdx�� rightIdx�� ����
		if ((rightIdx < m_heapSize) &&
			m_fPriorityComparator(m_pMinHeapArr[rightIdx]->data, m_pMinHeapArr[lowestIdx]->data))
		{
			lowestIdx = rightIdx;
		}

		// lowestIdx�� rootIdx�� min heap property�� �����ϹǷ� �ݺ� ����
		if (lowestIdx == rootIdx)
		{
			break;
		}

		// lowestIdx�� rootIdx�� ��� �����͸� �����ϰ� rootIdx�� lowestIdx�� ����
		std::swap(m_pMinHeapArr[lowestIdx], m_pMinHeapArr[rootIdx]);
		rootIdx = lowestIdx;
	}
}

template<typename T>
inline void PriorityQueue<T>::movePtrNodeUpwardMax(int targetIdx)
{
	// target ��尡 �θ� ��庸�� �켱������ �� ���� �� �θ�� target ���� �� �θ� target���� ����
	while ((targetIdx != 0) && 
		m_fPriorityComparator(m_pMaxHeapArr[getParentIdx(targetIdx)]->data, m_pMaxHeapArr[targetIdx]->data))
	{
		std::swap(m_pMaxHeapArr[getParentIdx(targetIdx)], m_pMaxHeapArr[targetIdx]);
		targetIdx = getParentIdx(targetIdx);
	}
}

template<typename T>
inline void PriorityQueue<T>::movePtrNodeUpwardMin(int targetIdx)
{
	// target ��尡 �θ� ��庸�� �켱������ �� ���� �� �θ�� target ���� �� �θ� target���� ����
	while ((targetIdx != 0) &&
		m_fPriorityComparator(m_pMinHeapArr[targetIdx]->data, m_pMinHeapArr[getParentIdx(targetIdx)]->data))
	{
		std::swap(m_pMinHeapArr[getParentIdx(targetIdx)], m_pMinHeapArr[targetIdx]);
		targetIdx = getParentIdx(targetIdx);
	}
}

template<typename T>
inline void PriorityQueue<T>::buildHeap(HeapType heapType)
{
	const int numInternalNodes = m_heapSize / 2;

	// ��� ��尡 heap property�� �����ϵ��� �����
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
