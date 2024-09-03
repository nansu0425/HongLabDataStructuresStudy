#pragma once

#include <utility>

template<typename Key, typename Value>
struct Node
{
	Key		key;
	Value	value;
	int		height = 0;
	Node*	pLeft = nullptr;
	Node*	pRight = nullptr;

	Node() = default;
	Node(Node&& other) = default;

	Node(const Key& key, const Value& value)
		: key(key)
		, value(value)
	{ }

	Node(Key&& key, Value&& value) noexcept
		: key(std::move(key))
		, value(std::move(value))
	{ }
};

template<typename Key, typename Value> 
class BinarySearchTree
{
	using BstNode = Node<Key, Value>;

public:
	~BinarySearchTree();

	template<typename Node>
	void	insert(Node&& node);
	void	remove(const Key& key);
	
	const BstNode*	getPtrNode const (const Key& key);

private:
	void	deleteAllNodes(BstNode* pRoot);

private:
	BstNode*	m_pRoot = nullptr;
};
