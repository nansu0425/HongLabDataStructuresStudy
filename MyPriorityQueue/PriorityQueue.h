#pragma once

#include "LinkedList.h"

#include <algorithm>
#include <iostream>
#include <utility>

enum class HeapType
{
	NONE,
	MAX,
	MIN,
};

template<typename T>
struct QueueData
{
	T*			pObj = nullptr;
	int			maxHeapIdx = -1;
	int			minHeapIdx = -1;

	QueueData(const T& obj)
		: pObj(new T(obj))
	{ }

	QueueData(T&& obj) noexcept
		: pObj(new T(std::move(obj)))
	{ }

	QueueData(const QueueData& other)
		: pObj(new T(*other.pObj))
		, maxHeapIdx(other.maxHeapIdx)
		, minHeapIdx(other.minHeapIdx)
	{ }

	QueueData(QueueData&& other) noexcept
		: pObj(std::exchange(other.pObj, nullptr))
		, maxHeapIdx(other.maxHeapIdx)
		, minHeapIdx(other.minHeapIdx)
	{ }

	~QueueData()
	{
		if (pObj != nullptr)
		{
			delete pObj;
		}
	}
};

template<typename T>
class PriorityQueue
{
	using Data = QueueData<T>;
	using PriorityComparator = bool (*)(const T& lower, const T& higher);
	using PtrNode = Node<Data>*;

public:
	PriorityQueue(const T* pArr, int numElem, int queueSize, PriorityComparator fPriorityComparator);
	PriorityQueue(int queueSize, PriorityComparator fPriorityComparator);
	~PriorityQueue();

	template<typename U>
	void		insert(U&& obj);
	T			extract(HeapType heapType);

	const T&	getHighestObj() const { return *m_pMaxHeapArr[0]->pData->pObj; }
	const T&	getLowestObj() const { return *m_pMinHeapArr[0]->pData->pObj; }
	PtrNode*	getPtrMaxHeapArr() const { return m_pMaxHeapArr; }
	PtrNode*	getPtrMinHeapArr() const { return m_pMinHeapArr; }
	int			getHeapSize() const { return m_heapSize; }
	bool		isEmpty() const { return m_pList->isEmpty(); }
	bool		isFull() const { return (m_arrSize == m_heapSize); }

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
	LinkedList<Data>*	m_pList = nullptr;
	PtrNode*			m_pMaxHeapArr = nullptr;
	PtrNode*			m_pMinHeapArr = nullptr;
	int					m_arrSize = 0;
	int					m_heapSize = 0;
	PriorityComparator	m_fPriorityComparator = nullptr;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const PriorityQueue<T>& pq)
{
	Node<QueueData<T>>** pMaxHeapArr = pq.getPtrMaxHeapArr();
	Node<QueueData<T>>** pMinHeapArr = pq.getPtrMinHeapArr();
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
			os << *pMaxHeapArr[arrIdx]->pData->pObj << " ";
		}
	}
	os << std::endl;

	// �ּ� �� ���
	if (pMinHeapArr != nullptr)
	{
		os << "Min heap: ";

		for (int arrIdx = 0; arrIdx < heapSize; ++arrIdx)
		{
			os << *pMinHeapArr[arrIdx]->pData->pObj << " ";
		}
	}
	os << std::endl;

	return os;
}

template<typename T>
inline PriorityQueue<T>::PriorityQueue(const T* pArr, int numElem, int queueSize, PriorityComparator fPriorityComparator)
{
	// �迭 ���� ������ ť�� ũ�⸦ ���� �� ����
	assert((0 < numElem) && (numElem <= queueSize));

	// ���� ����Ʈ �ʱ�ȭ
	m_pList = new LinkedList<Data>();

	for (int arrIdx = 0; arrIdx < numElem; ++arrIdx)
	{
		m_pList->append(pArr[arrIdx]);
		m_pList->getPtrLast()->pData->maxHeapIdx = arrIdx;
		m_pList->getPtrLast()->pData->minHeapIdx = arrIdx;
	}

	assert(!m_pList->isEmpty());

	// �� �迭 ����
	m_pMaxHeapArr = new PtrNode[queueSize];
	m_pMinHeapArr = new PtrNode[queueSize];

	// �� �迭 �ʱ�ȭ
	std::fill(m_pMaxHeapArr, m_pMaxHeapArr + queueSize, nullptr);
	std::fill(m_pMinHeapArr, m_pMinHeapArr + queueSize, nullptr);

	PtrNode pCur = m_pList->getPtrFirst();
	
	// �� �迭�� ���ҿ� ���� ����Ʈ ����� �ּҸ� �ִ´�
	for (int arrIdx = 0; arrIdx < numElem; ++arrIdx)
	{
		m_pMaxHeapArr[arrIdx] = pCur;
		m_pMinHeapArr[arrIdx] = pCur;

		pCur = pCur->pNext;
	}

	// �� �迭 ũ��� �迭 ���� �� ũ��, �켱���� �Ǵ� �Լ� ����
	m_arrSize = queueSize;
	m_heapSize = numElem;
	m_fPriorityComparator = fPriorityComparator;

	// �� ũ�⸸ŭ �迭���� ���ҵ��� �� ������ ��迭
	buildHeap();
}

template<typename T>
inline PriorityQueue<T>::PriorityQueue(int queueSize, PriorityComparator fPriorityComparator)
{
	// ���� ����Ʈ �ʱ�ȭ
	m_pList = new LinkedList<Data>();

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
inline T PriorityQueue<T>::extract(HeapType heapType)
{
	assert((m_heapSize > 0) && (heapType != HeapType::NONE));

	T obj = T();

	if (heapType == HeapType::MAX)
	{
		obj = getHighestObj();
		PtrNode pDel = m_pMaxHeapArr[0];

		// �ִ� ���� ������ ��� ���� ��Ʈ ���� �̵�
		m_pMaxHeapArr[0] = m_pMaxHeapArr[m_heapSize - 1];

		// �ּ� ���� ������ ��� ���� ���� ����Ʈ�� ������ ��带 ����Ű�� �ִ� ���� ���� �̵�
		const int delMinHeapIdx = pDel->pData->minHeapIdx;
		m_pMinHeapArr[delMinHeapIdx] = m_pMinHeapArr[m_heapSize - 1];

		// ������ ������ ��带 ���ܽ�Ų��
		--m_heapSize;
		m_pMaxHeapArr[m_heapSize] = nullptr;
		m_pMinHeapArr[m_heapSize] = nullptr;

		// ���� ����Ʈ���� ��带 �����Ѵ�
		m_pList->remove(pDel);
		
		// �ִ� �� �迭�� �� ������ ��迭
		heapifyMax(0);

		// �ּ� ������ ���ܵ� ������ ��尡 ���� ����Ʈ���� ������ ��带 ����Ű�� ���� �ʾҴٸ�
		// ������ ��带 ����Ű�� �ּ� �� ����� ����� �����͸� �� ������ �µ��� �ø���
		if (delMinHeapIdx < m_heapSize)
		{
			movePtrNodeUpwardMin(delMinHeapIdx);
		}
	}
	else if (heapType == HeapType::MIN)
	{
		obj = getLowestObj();
		PtrNode pDel = m_pMinHeapArr[0];

		// �ּ� ���� ������ ��� ���� ��Ʈ ���� �̵�
		m_pMinHeapArr[0] = m_pMinHeapArr[m_heapSize - 1];

		// �ִ� ���� ������ ��� ���� ���� ����Ʈ�� ������ ��带 ����Ű�� �ִ� ���� ���� �̵�
		const int delMaxHeapIdx = pDel->pData->maxHeapIdx;
		m_pMaxHeapArr[delMaxHeapIdx] = m_pMaxHeapArr[m_heapSize - 1];

		// ������ ������ ��带 ���ܽ�Ų��
		--m_heapSize;
		m_pMinHeapArr[m_heapSize] = nullptr;
		m_pMaxHeapArr[m_heapSize] = nullptr;

		// ���� ����Ʈ���� ��带 �����Ѵ�
		m_pList->remove(pDel);

		// �ּ� �� �迭�� �� ������ ��迭
		heapifyMin(0);

		// �ִ� ������ ���ܵ� ������ ��尡 ���� ����Ʈ���� ������ ��带 ����Ű�� ���� �ʾҴٸ�
		// ������ ��带 ����Ű�� �ִ� �� ����� ����� �����͸� �� ������ �µ��� �ø���
		if (delMaxHeapIdx < m_heapSize)
		{
			movePtrNodeUpwardMax(delMaxHeapIdx);
		}
	}

	return obj;
}

template<typename T>
template<typename U>
inline void PriorityQueue<T>::insert(U&& obj)
{
	int targetIdx = -1;

	// �� �迭�� ���� �� ����
	if (isFull())
	{
		// �켱������ ���� ���� ������Ʈ���� ���ο� ������Ʈ�� �켱������ �� ���� ��
		if (m_fPriorityComparator(getLowestObj(), obj))
		{
			// �켱������ ���� ���� ����� ������Ʈ�� ���ο� ������Ʈ�� ����
			*m_pMinHeapArr[0]->pData->pObj = obj;
			targetIdx = m_pMinHeapArr[0]->pData->maxHeapIdx;

			// �ּ� ���� �� ������ �°� ����
			heapifyMin(0);
		}
		else
		{
			return;
		}
	}
	// �� �迭�� ������ �ִ� ���
	else
	{
		// ���ο� ��� �߰�
		m_pList->append(std::forward<U>(obj));
		Node<Data>* pLast = m_pList->getPtrLast();

		// �� ũ�� ����
		++m_heapSize;

		// ���� ������ ���Ҵ� ���� �߰��� ��带 ����Ų��
		m_pMaxHeapArr[m_heapSize - 1] = pLast;
		m_pMinHeapArr[m_heapSize - 1] = pLast;

		// �ε��� ���� ����
		pLast->pData->maxHeapIdx = m_heapSize - 1;
		pLast->pData->minHeapIdx = m_heapSize - 1;

		targetIdx = m_heapSize - 1;

		// �ּ� ���� �߰��� ��带 �� ������ �´� ��ġ�� �ø���
		movePtrNodeUpwardMin(targetIdx);
	}

	assert((-1 < targetIdx) && (targetIdx < m_heapSize));

	// �ִ� ���� ���� ����ǰų� ���ο� ���� ��带 �� ������ �´� ��ġ�� �ø���
	movePtrNodeUpwardMax(targetIdx);
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
		if (m_fPriorityComparator(*m_pMaxHeapArr[highestIdx]->pData->pObj, *m_pMaxHeapArr[leftIdx]->pData->pObj))
		{
			highestIdx = leftIdx;
		}

		// ������ ��尡 highestIdx ��庸�� �켱������ ������ highestIdx�� rightIdx�� ����
		if ((rightIdx < m_heapSize) && 
			m_fPriorityComparator(*m_pMaxHeapArr[highestIdx]->pData->pObj, *m_pMaxHeapArr[rightIdx]->pData->pObj))
		{
			highestIdx = rightIdx;
		}

		// highestIdx�� rootIdx�� max heap property�� �����ϹǷ� �ݺ� ����
		if (highestIdx == rootIdx)
		{
			break;
		}

		// highestIdx�� rootIdx�� ��� ������, �ε��� ������ �����ϰ� rootIdx�� highestIdx�� ����
		std::swap(m_pMaxHeapArr[highestIdx], m_pMaxHeapArr[rootIdx]);
		std::swap(m_pMaxHeapArr[highestIdx]->pData->maxHeapIdx, m_pMaxHeapArr[rootIdx]->pData->maxHeapIdx);
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
		if (m_fPriorityComparator(*m_pMinHeapArr[leftIdx]->pData->pObj, *m_pMinHeapArr[lowestIdx]->pData->pObj))
		{
			lowestIdx = leftIdx;
		}

		// ������ �ڽ� ��尡 lowestIdx ��庸�� �켱������ ������ lowestIdx�� rightIdx�� ����
		if ((rightIdx < m_heapSize) &&
			m_fPriorityComparator(*m_pMinHeapArr[rightIdx]->pData->pObj, *m_pMinHeapArr[lowestIdx]->pData->pObj))
		{
			lowestIdx = rightIdx;
		}

		// lowestIdx�� rootIdx�� min heap property�� �����ϹǷ� �ݺ� ����
		if (lowestIdx == rootIdx)
		{
			break;
		}

		// lowestIdx�� rootIdx�� ��� ������, �ε��� ������ �����ϰ� rootIdx�� lowestIdx�� ����
		std::swap(m_pMinHeapArr[lowestIdx], m_pMinHeapArr[rootIdx]);
		std::swap(m_pMinHeapArr[lowestIdx]->pData->minHeapIdx, m_pMinHeapArr[rootIdx]->pData->minHeapIdx);
		rootIdx = lowestIdx;
	}
}

template<typename T>
inline void PriorityQueue<T>::movePtrNodeUpwardMax(int targetIdx)
{
	// target ��尡 �θ� ��庸�� �켱������ �� ���� �� �θ�� target ���� �� �θ� target���� ����
	while ((targetIdx != 0) && 
		m_fPriorityComparator(*m_pMaxHeapArr[getParentIdx(targetIdx)]->pData->pObj, *m_pMaxHeapArr[targetIdx]->pData->pObj))
	{
		std::swap(m_pMaxHeapArr[getParentIdx(targetIdx)], m_pMaxHeapArr[targetIdx]);
		std::swap(m_pMaxHeapArr[getParentIdx(targetIdx)]->pData->maxHeapIdx, m_pMaxHeapArr[targetIdx]->pData->maxHeapIdx);
		targetIdx = getParentIdx(targetIdx);
	}
}

template<typename T>
inline void PriorityQueue<T>::movePtrNodeUpwardMin(int targetIdx)
{
	// target ��尡 �θ� ��庸�� �켱������ �� ���� �� �θ�� target ���� �� �θ� target���� ����
	while ((targetIdx != 0) &&
		m_fPriorityComparator(*m_pMinHeapArr[targetIdx]->pData->pObj, *m_pMinHeapArr[getParentIdx(targetIdx)]->pData->pObj))
	{
		std::swap(m_pMinHeapArr[getParentIdx(targetIdx)], m_pMinHeapArr[targetIdx]);
		std::swap(m_pMinHeapArr[getParentIdx(targetIdx)]->pData->minHeapIdx, m_pMinHeapArr[targetIdx]->pData->minHeapIdx);
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
