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
	void		insert(U&& data);
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
			os << *pMaxHeapArr[arrIdx]->pData->pObj << " ";
		}
	}
	os << std::endl;

	// 최소 힙 출력
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
	// 배열 원소 개수는 큐의 크기를 넘을 수 없다
	assert((0 < numElem) && (numElem <= queueSize));

	// 연결 리스트 초기화
	m_pList = new LinkedList<Data>();

	for (int arrIdx = 0; arrIdx < numElem; ++arrIdx)
	{
		m_pList->append(pArr[arrIdx]);
		m_pList->getPtrLast()->pData->maxHeapIdx = arrIdx;
		m_pList->getPtrLast()->pData->minHeapIdx = arrIdx;
	}

	assert(!m_pList->isEmpty());

	// 힙 배열 생성
	m_pMaxHeapArr = new PtrNode[queueSize];
	m_pMinHeapArr = new PtrNode[queueSize];

	// 힙 배열 초기화
	std::fill(m_pMaxHeapArr, m_pMaxHeapArr + queueSize, nullptr);
	std::fill(m_pMinHeapArr, m_pMinHeapArr + queueSize, nullptr);

	PtrNode pCur = m_pList->getPtrFirst();
	
	// 힙 배열의 원소에 연결 리스트 노드의 주소를 넣는다
	for (int arrIdx = 0; arrIdx < numElem; ++arrIdx)
	{
		m_pMaxHeapArr[arrIdx] = pCur;
		m_pMinHeapArr[arrIdx] = pCur;

		pCur = pCur->pNext;
	}

	// 힙 배열 크기와 배열 내의 힙 크기, 우선순위 판단 함수 설정
	m_arrSize = queueSize;
	m_heapSize = numElem;
	m_fPriorityComparator = fPriorityComparator;

	// 힙 크기만큼 배열내의 원소들을 힙 구조로 재배열
	buildHeap();
}

template<typename T>
inline PriorityQueue<T>::PriorityQueue(int queueSize, PriorityComparator fPriorityComparator)
{
	// 연결 리스트 초기화
	m_pList = new LinkedList<Data>();

	// 힙 배열 생성
	m_pMaxHeapArr = new PtrNode[queueSize];
	m_pMinHeapArr = new PtrNode[queueSize];

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
template<typename U>
inline void PriorityQueue<T>::insert(U&& data)
{
	int targetIdx = -1;

	// 힙 배열이 가득 찬 상태
	if (isFull())
	{
		// TODO: 우선순위가 가장 낮은 노드의 data를 새로운 data로 변경 (새로운 data의 우선순위가 더 높을 때)
	}
	// 힙 배열에 여유가 있는 경우
	else
	{
		// 새로운 노드 추가
		m_pList->append(std::forward<U>(data));
		Node<Data>* pLast = m_pList->getPtrLast();

		// 힙 크기 증가
		++m_heapSize;

		// 힙의 마지막 원소는 새로 추가된 노드를 가리킨다
		m_pMaxHeapArr[m_heapSize - 1] = pLast;
		m_pMinHeapArr[m_heapSize - 1] = pLast;

		// 인덱스 정보 저장
		pLast->pData->maxHeapIdx = m_heapSize - 1;
		pLast->pData->minHeapIdx = m_heapSize - 1;

		targetIdx = m_heapSize - 1;
	}

	assert((-1 < targetIdx) && (targetIdx < m_heapSize));

	movePtrNodeUpwardMax(targetIdx);
	movePtrNodeUpwardMin(targetIdx);
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
		if (m_fPriorityComparator(*m_pMaxHeapArr[highestIdx]->pData->pObj, *m_pMaxHeapArr[leftIdx]->pData->pObj))
		{
			highestIdx = leftIdx;
		}

		// 오른쪽 노드가 highestIdx 노드보다 우선순위가 높으면 highestIdx를 rightIdx로 설정
		if ((rightIdx < m_heapSize) && 
			m_fPriorityComparator(*m_pMaxHeapArr[highestIdx]->pData->pObj, *m_pMaxHeapArr[rightIdx]->pData->pObj))
		{
			highestIdx = rightIdx;
		}

		// highestIdx가 rootIdx면 max heap property를 만족하므로 반복 종료
		if (highestIdx == rootIdx)
		{
			break;
		}

		// highestIdx와 rootIdx의 노드 포인터, 인덱스 정보를 스왑하고 rootIdx를 highestIdx로 설정
		std::swap(m_pMaxHeapArr[highestIdx], m_pMaxHeapArr[rootIdx]);
		std::swap(m_pMaxHeapArr[highestIdx]->pData->maxHeapIdx, m_pMaxHeapArr[rootIdx]->pData->maxHeapIdx);
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
		if (m_fPriorityComparator(*m_pMinHeapArr[leftIdx]->pData->pObj, *m_pMinHeapArr[lowestIdx]->pData->pObj))
		{
			lowestIdx = leftIdx;
		}

		// 오른쪽 자식 노드가 lowestIdx 노드보다 우선순위가 낮으면 lowestIdx를 rightIdx로 설정
		if ((rightIdx < m_heapSize) &&
			m_fPriorityComparator(*m_pMinHeapArr[rightIdx]->pData->pObj, *m_pMinHeapArr[lowestIdx]->pData->pObj))
		{
			lowestIdx = rightIdx;
		}

		// lowestIdx가 rootIdx면 min heap property를 만족하므로 반복 종료
		if (lowestIdx == rootIdx)
		{
			break;
		}

		// lowestIdx와 rootIdx의 노드 포인터, 인덱스 정보를 스왑하고 rootIdx를 lowestIdx로 설정
		std::swap(m_pMinHeapArr[lowestIdx], m_pMinHeapArr[rootIdx]);
		std::swap(m_pMinHeapArr[lowestIdx]->pData->minHeapIdx, m_pMinHeapArr[rootIdx]->pData->minHeapIdx);
		rootIdx = lowestIdx;
	}
}

template<typename T>
inline void PriorityQueue<T>::movePtrNodeUpwardMax(int targetIdx)
{
	// target 노드가 부모 노드보다 우선순위가 더 높을 때 부모와 target 스왑 후 부모를 target으로 설정
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
	// target 노드가 부모 노드보다 우선순위가 더 낮을 때 부모와 target 스왑 후 부모를 target으로 설정
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
