#pragma once

#include <cassert>
#include <iostream>
#include <utility>

template<typename T>
struct Node
{
	T*			pData = nullptr;
	Node<T>*	pPrev = nullptr;
	Node<T>*	pNext = nullptr;;

	Node(const T& data)
	{
		pData = new T(data);
	}

	Node(T&& data) 
	{ 
		pData = new T(std::move(data));
	}

	~Node() 
	{ 
		if (pData != nullptr)
		{
			delete pData;
		}
	}
};

template<typename T>
class LinkedList
{
public:
	LinkedList() = default;
	~LinkedList();

	template<typename U>
	void			append(U&& data);
	void			remove(const Node<T>* pTarget);

	Node<T>*		getPtrFirst() const { return m_pFirst; }
	Node<T>*		getPtrLast() const { return m_pLast; }
	bool			isEmpty() const;

private:
	Node<T>*		m_pFirst = nullptr;
	Node<T>*		m_pLast = nullptr;
};

template<typename T>
inline LinkedList<T>::~LinkedList()
{
	while (m_pFirst != nullptr)
	{
		remove(m_pFirst);
	}

	assert(isEmpty());
} 

template<typename T>
template<typename U>
inline void LinkedList<T>::append(U&& data)
{
	Node<T>* pNode = new Node<T>(std::forward<U>(data));

	// ��尡 ���� ���
	if (isEmpty())
	{
		m_pFirst = pNode;
		m_pLast = pNode;

		assert(!isEmpty() && (m_pFirst->pPrev == nullptr));
	}
	// ������ ��� �ڿ� ���ο� ��� ����
	else
	{
		m_pLast->pNext = pNode;
		pNode->pPrev = m_pLast;
		m_pLast = pNode;

		assert((m_pLast != nullptr) && (m_pLast->pPrev != nullptr));
	}

	assert(m_pLast->pNext == nullptr);
}

template<typename T>
inline void LinkedList<T>::remove(const Node<T>* pTarget)
{
	if (isEmpty())
	{
		return;
	}

	Node<T>* pTargetNext = pTarget->pNext;
	Node<T>* pTargetPrev = pTarget->pPrev;

	// ��� ��尡 ù ��° ����� ���
	if (pTargetPrev == nullptr)
	{
		m_pFirst = pTargetNext;
	}
	// ù ��° ��尡 �ƴϸ� ���� ����� ���� ��带 ��� ����� ���� ���� ����
	else
	{
		pTargetPrev->pNext = pTargetNext;
	}

	// ��� ��尡 ������ ����� ���
	if (pTargetNext == nullptr)
	{
		m_pLast = pTargetPrev;
	}
	// ������ ��尡 �ƴϸ� ���� ����� ���� ��带 ��� ����� ���� ���� ����
	else
	{
		pTargetNext->pPrev = pTargetPrev;
	}

	delete pTarget;
}

template<typename T>
inline bool LinkedList<T>::isEmpty() const
{
	return ((m_pFirst == nullptr) && (m_pLast == nullptr));
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& li)
{
	const Node<T>* pCur = li.getPtrFirst();

	while (pCur != nullptr)
	{
		os << *pCur->pData << " ";
		pCur = pCur->pNext;
	}

	return os;
}
