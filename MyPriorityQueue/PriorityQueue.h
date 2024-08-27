#pragma once

#include "LinkedList.h"

enum class HeapType
{
	MIN,
	MAX,
};

template<typename T>
class PriorityQueue
{
	using PriorityComparator = bool (*)(const T& lower, const T& higher);
	using PtrNode = Node<T>*;

public:
	PriorityQueue(const T* arrFirst, const T* arrLast, int queueSize, PriorityComparator fPriorityComparator);
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
	void		buildHeap(HeapType heapType);
	void		movePtrNodeUpward(int targetIdx, HeapType heapType);

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
};
