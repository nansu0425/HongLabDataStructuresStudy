#pragma once

#include <cstring>
#include <cassert>

template<typename T>
class Stack
{
public:
	Stack(int size = 4) : m_size(size)
	{
		m_items = new T[size];
		::memset(m_items, 0x00, sizeof(T) * size);
	}

	~Stack()
	{
		if (m_items)
		{
			delete[] m_items;
		}
	}

	bool IsFull() const { return (m_top + 1 == m_size); }
	bool IsEmpty() const { return (m_top == -1); }

	void Push(const T& item)
	{
		if (IsFull())
		{
			DoubleSize();
		}

		assert(!IsFull());

		++m_top;
		m_items[m_top] = item;
	}

	T Pop()
	{
		assert(!IsEmpty());

		T oldTop = m_items[m_top];
		--m_top;

		return oldTop;
	}

	const T& Peek() const
	{
		assert(!IsEmpty());

		return m_items[m_top];
	}

private:
	void DoubleSize()
	{
		assert(IsFull());

		T* newItems = new T[m_size * 2];
		::memset(newItems, 0x00, sizeof(T) * m_size * 2);
		::memcpy(newItems, m_items, sizeof(T) * m_size);

		delete[] m_items;
		m_items = newItems;
		m_size *= 2;
	}

private:
	T*		m_items = nullptr;
	int		m_size = 0;
	int		m_top = -1;
};
