#pragma once

template<typename K, typename V>
struct Node
{
	K		key;
	V		value;
	int		height = 0;
	Node*	pLeft = nullptr;
	Node*	pRight = nullptr;
};

template<typename K, typename V> 
class BinarySearchTree
{

};
