#pragma once

#include <utility>

template<typename Key, typename Value>
class Node
{
	Key		key;
	Value	value;
	int		height = 0;
	Node*	pLeft = nullptr;
	Node*	pRight = nullptr;
};

template<typename Key, typename Value> 
class BinarySearchTree
{
	using BstNode = Node<Key, Value>;

public:
	~BinarySearchTree();

	void	insert(BstNode node);
	void	remove(const Key& key);
	
	const BstNode*	getPtrNode(const Key& key) const;

private:
	void	clear(BstNode* pRoot);

private:
	BstNode*	m_pRoot = nullptr;
};
