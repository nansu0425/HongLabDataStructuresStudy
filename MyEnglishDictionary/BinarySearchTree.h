#pragma once

#include <utility>
#include <algorithm>
#include <iostream>

template<typename K, typename V>
struct Pair
{
	K	key;
	V	value;
};

template<typename K, typename V>
struct Node
{
	Pair<K, V>	pair;
	int			height = -1;
	Node*		pLeft = nullptr;
	Node*		pRight = nullptr;

	void		setHeight();
};

template<typename K, typename V> 
class BinarySearchTree
{
	using Node = Node<K, V>;
	using Pair = Pair<K, V>;

public:
	~BinarySearchTree();

	void		insert(const Pair& pair);
	void		remove(const K& key);
	
	const Node*		getPtrNode(const K& key) const;
	const Node*		getPtrRoot() const { return m_pRoot; }
	std::ostream&	print(std::ostream& os, const Node* pRoot) const;

private:
	void		clear(Node* pRoot);
	Node*		insert(Node* pTarget, const Pair& pair);

private:
	Node*		m_pRoot = nullptr;
};

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const BinarySearchTree<K, V>& bst)
{ 
	return bst.print(os, bst.getPtrRoot());
}

template<typename K, typename V>
inline BinarySearchTree<K, V>::~BinarySearchTree()
{
	if (m_pRoot != nullptr)
	{
		clear(m_pRoot);
	}
}

template<typename K, typename V>
inline void BinarySearchTree<K, V>::insert(const Pair& pair)
{
	if (m_pRoot == nullptr)
	{
		m_pRoot = new Node{ std::move(pair), 0, nullptr, nullptr };
		return;
	}

	insert(m_pRoot, pair);
}

template<typename K, typename V>
inline std::ostream& BinarySearchTree<K, V>::print(std::ostream& os, const Node* pRoot) const
{
	if (pRoot == nullptr)
	{
		return os;
	}

	print(os, pRoot->pLeft);
	os << pRoot->pair.key << std::endl;
	os << pRoot->pair.value << std::endl;
	print(os, pRoot->pRight);

	return os;
}

template<typename K, typename V>
inline void BinarySearchTree<K, V>::clear(Node* pRoot)
{
	if (pRoot == nullptr)
	{
		return;
	}

	clear(pRoot->pLeft);
	clear(pRoot->pRight);
	delete pRoot;
}

template<typename K, typename V>
inline Node<K, V>* BinarySearchTree<K, V>::insert(Node* pTarget, const Pair& pair)
{
	if (pTarget == nullptr)
	{
		return new Node{ std::move(pair), 0, nullptr, nullptr };
	}

	// 삽입하려는 key가 클 때
	if (pTarget->pair.key < pair.key)
	{
		pTarget->pRight = insert(pTarget->pRight, pair);
	}
	// 삽입하려는 key가 작을 때
	else if (pair.key < pTarget->pair.key)
	{
		pTarget->pLeft = insert(pTarget->pLeft, pair);
	}
	// 삽입하려는 key가 같을 때
	else
	{
		pTarget->pair.value = std::move(pair.value);
	}

	// 높이 설정
	pTarget->setHeight();

	return pTarget;
}

template<typename K, typename V>
inline void Node<K, V>::setHeight()
{
	// 자식 노드가 없을 때
	if ((pLeft == nullptr) && (pRight == nullptr))
	{
		height = 0;
		return;
	}

	// 오른쪽 자식 노드만 있는 경우
	if (pLeft == nullptr)
	{
		height = pRight->height + 1;
	}
	// 왼쪽 자식 노드만 있는 경우
	else if (pRight == nullptr)
	{
		height = pLeft->height + 1;
	}
	// 양쪽 자식 노드 모두 있는 경우
	else
	{
		height = std::max(pRight->height, pLeft->height) + 1;
	}
}
