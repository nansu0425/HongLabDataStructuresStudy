#pragma once

#include <cassert>
#include <iostream>

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
	LinkedList(const T* pArr, int numElem);
	~LinkedList();

	void			append(const T& data);
	void			remove(const Node<T>* pTarget);

	Node<T>*		getPtrFirst() const { return m_pFirst; }
	Node<T>*		getPtrLast() const { return m_pLast; }
	bool			isEmpty() const;

private:
	Node<T>*		m_pFirst = nullptr;
	Node<T>*		m_pLast = nullptr;
};

template<typename T>
inline LinkedList<T>::LinkedList(const T* pArr, int numElem)
{
	assert((pArr != nullptr) && (numElem > 0));

	for (int arrIdx = 0; arrIdx < numElem; ++arrIdx)
	{
		append(pArr[arrIdx]);
	}

	assert(!isEmpty());
}

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
inline void LinkedList<T>::append(const T& data)
{
	Node<T>* pNode = new Node<T>(data);

	// 노드가 없는 경우
	if (isEmpty())
	{
		m_pFirst = pNode;
		m_pLast = pNode;

		assert(!isEmpty() && (m_pFirst->pPrev == nullptr));
	}
	// 마지막 노드 뒤에 새로운 노드 연결
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

	// 대상 노드가 첫 번째 노드인 경우
	if (pTargetPrev == nullptr)
	{
		m_pFirst = pTargetNext;
	}
	// 첫 번째 노드가 아니면 이전 노드의 다음 노드를 대상 노드의 다음 노드로 설정
	else
	{
		pTargetPrev->pNext = pTargetNext;
	}

	// 대상 노드가 마지막 노드인 경우
	if (pTargetNext == nullptr)
	{
		m_pLast = pTargetPrev;
	}
	// 마지막 노드가 아니면 다음 노드의 이전 노드를 대상 노드의 이전 노드로 설정
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
