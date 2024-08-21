#pragma once

#include <cassert>
#include <cstring>
#include <iostream>

template <typename T>
class Queue
{
public:
	Queue(int size = 4) : m_size(size)
	{
		assert(m_size > 0);

		m_items = new T[m_size];
		::memset(m_items, 0x00, sizeof(T) * m_size);
	}

	~Queue()
	{
		if (m_items)
		{
			delete[] m_items;
		}
	}

	bool IsEmpty() { return (m_front == m_rear); }
	bool IsFull() { return (GetNextIdx(m_rear) == m_front); }
	
	const T& Peek() const { return m_items[GetNextIdx(m_front)]; }

	void Enqueue(const T& item)
	{
		if (IsFull())
		{
			DoubleSize();
		}

		assert(!IsFull());

		m_rear = GetNextIdx(m_rear);
		m_items[m_rear] = item;
	}

	T Dequeue()
	{
		assert(!IsEmpty());

		T item = Peek();
		m_front = GetNextIdx(m_front);

		return item;
	}

	friend std::ostream& operator<<(std::ostream& os, const Queue<T>& q)
	{
		int itemIdx = q.GetNextIdx(q.m_front);

		while (itemIdx != q.m_rear + 1)
		{
			std::cout << q.m_items[itemIdx] << " ";
			itemIdx = q.GetNextIdx(itemIdx);
		}

		return os;
	}

private:
	int GetNextIdx(int idx) const { return (idx + 1) % m_size; }
	int GetPrevIdx(int idx) const { return (idx - 1 + m_size) % m_size; }

	void DoubleSize()
	{
		assert(IsFull());

		int newSize = m_size * 2;
		T* newItems = new T[newSize];
		::memset(newItems, 0x00, sizeof(T) * newSize);

		// m_front < m_rear인 경우는 m_front가 0인 경우가 유일
		if (m_front == 0)
		{
			::memcpy(&newItems[1], &m_items[1], sizeof(T) * (m_size - 1));
		}
		// m_rear < m_front인 경우 => m_rear + 1 == m_front
		else
		{
			::memcpy(&newItems[GetNextIdx(m_front)], &m_items[GetNextIdx(m_front)], sizeof(T) * (m_size - 1 - m_front));
			::memcpy(&newItems[m_size], &m_items[0], sizeof(T) * (m_rear + 1));
			m_rear += m_size;
		}

		delete[] m_items;
		m_items = newItems;
		m_size = newSize;
	}

private:
	T*		m_items = nullptr;
	int		m_size = 0;
	int		m_front = 0; // 첫 번째 item 앞의 index
	int		m_rear = 0; // 마지막 item의 index
};
