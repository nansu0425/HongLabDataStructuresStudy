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

	// ��� �迭�� �Ķ���� �迭�� �����ϰ� �� ������ �����
	std::copy(arr, arr + m_arrSize, m_arr);
	buildHeap();
}

template<typename T>
inline void MaxHeap<T>::insert(const T& key)
{
	assert(m_heapSize < m_arrSize);

	// ������ ��� �ڿ� ���ο� ��� �߰�
	++m_heapSize;
	m_arr[m_heapSize - 1] = key;

	// �߰��� ����� key�� heap property�� �����ϴ� ��ġ�� �ø���
	increaseKey(m_heapSize - 1);
}

template<typename T>
inline T MaxHeap<T>::extractMax()
{
	assert(m_heapSize > 0);

	// ������ ��� key�� ��Ʈ key�� ����
	std::swap(m_arr[0], m_arr[m_heapSize - 1]);

	// ������ ��带 ������ ����
	--m_heapSize;

	// ���� ��尡 2�� �̻��̸� heap property�� �����ǵ���
	// ������ ����� key�� ���� ��Ʈ ��带 heapify
	if (m_heapSize > 1)
	{
		heapify(0);
	}

	// ������ ��� ���� key�� ���� �� ���� ��Ʈ key�̹Ƿ� �ִ�
	return m_arr[m_heapSize];
}

template<typename T>
inline void MaxHeap<T>::sort(T* arr, int arrSize)
{
	MaxHeap<T> mh;
	mh.m_arr = arr;
	mh.m_arrSize = arrSize;
	mh.m_heapSize = arrSize;

	// �� ������ �����
	mh.buildHeap();

	// ���� ��尡 �ϳ� ���� ������ �ݺ�
	while (mh.m_heapSize > 1)
	{
		// ������ ��� key�� ��Ʈ key�� ������ ��
		// �� ũ�⸦ �ٿ� ������ ��带 �� �������� ���ܽ�Ų��
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

	// ���� ����� key�� ��Ʈ key���� ũ�� 
	// ���� ū key�� �ε����� ���� ��� �ε����� ����
	if ((leftIdx < m_heapSize) &&
		(m_arr[leftIdx] > m_arr[rootIdx]))
	{
		largestKeyIdx = leftIdx;
	}

	// ������ ����� key�� ���� ū key���� ũ�� 
	// ���� ū key�� ������ ��� �ε����� �ȴ�
	if ((rightIdx < m_heapSize) &&
		(m_arr[rightIdx] > m_arr[largestKeyIdx]))
	{
		largestKeyIdx = rightIdx;
	}

	// ���� ū key�� �ε����� ��Ʈ �ε����� �ƴϸ�
	// �� key�� �����ϰ� ��Ʈ key�� �� �ڽ� ��带 heapify
	if (largestKeyIdx != rootIdx)
	{
		std::swap(m_arr[largestKeyIdx], m_arr[rootIdx]);
		heapify(largestKeyIdx);
	}
}

template<typename T>
inline void MaxHeap<T>::heapifyIteratively(int targetIdx)
{
	// target ��尡 ���� ��尡 �ƴϸ� �ݺ�
	while (getLeftIdx(targetIdx) < m_heapSize)
	{
		int largestIdx = targetIdx;
		const int leftIdx = getLeftIdx(targetIdx);
		const int rightIdx = getRightIdx(targetIdx);

		// ���� �ڽ� ����� key�� �� ũ�� largestIdx�� ���� ��� �ε����� ����
		if (m_arr[targetIdx] < m_arr[leftIdx])
		{
			largestIdx = leftIdx;
		}

		// ������ ��尡 largestIdx�� ��� key���� ũ�� 
		// largestIdx�� ������ ��� �ε����� ����
		if ((rightIdx < m_heapSize) && (m_arr[largestIdx] < m_arr[rightIdx]))
		{
			largestIdx = rightIdx;
		}

		// largestIdx�� �ڽ� ��� �ε����� �ƴϸ� heap property�� �����ϹǷ� �ݺ� ����
		if (largestIdx == targetIdx)
		{
			break;
		}

		// largestIdx�� targetIdx�� key�� �����ϰ� targetIdx�� largestIdx�� ����
		std::swap(m_arr[largestIdx], m_arr[targetIdx]);
		targetIdx = largestIdx;
	}
}

template<typename T>
inline void MaxHeap<T>::buildHeap()
{
	const int numInternalNodes = m_heapSize / 2;

	// ��� ��尡 heap property�� �����ϵ��� �����
	for (int rootIdx = numInternalNodes - 1; rootIdx >= 0; --rootIdx)
	{
		heapify(rootIdx);
	}
}

template<typename T>
inline void MaxHeap<T>::increaseKey(int targetIdx)
{
	// targetIdx ����� key�� �θ� ��� key���� Ŭ�� 
	// key�� �����ϰ� parentIdx�� targetIdx�� ����
	while ((targetIdx != 0) && (m_arr[getParentIdx(targetIdx)] < m_arr[targetIdx]))
	{
		std::swap(m_arr[getParentIdx(targetIdx)], m_arr[targetIdx]);
		targetIdx = getParentIdx(targetIdx);
	}
}
