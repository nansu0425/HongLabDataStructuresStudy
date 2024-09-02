#pragma once

#include "BinarySearchTree.h"

template<typename K, typename V>
class AVL : public BinarySearchTree<K, V>
{
public:
	using Base = BinarySearchTree<K, V>;
	using Item = BinarySearchTree<K, V>::Item;
	using Node = BinarySearchTree<K, V>::Node;

	//struct Item {
	//	K key = K();	// first
	//	V value = V();	// second
	//};

	//struct Node {
	//	Item item;
	//	Node* left = nullptr;
	//	Node* right = nullptr;
	//};

	int Height(Node* node)
	{
		return Base::Height(node); // 헷갈림 방지용
	}

	int getHeightDiff(Node* node)
	{
		if (node)
			return Base::Height(node->left) - Base::Height(node->right);
		else
			return 0;
	}

	void balance(Node*& node)
	{
		// heightDiff가 0, 1, -1 이면 조절할 필요가 없다고 판단
		const int heightDiff = getHeightDiff(node);

		// LL -> Right Rotation
		if (heightDiff > 1 && getHeightDiff(node->left) >= 0)
		{
			node = RotateRight(node);
		}

		// RR -> Left Rotation
		if (heightDiff < -1 && getHeightDiff(node->right) <= 0)
		{
			node = RotateLeft(node);
		}

		// LR -> Left-Right Rotation
		if (heightDiff > 1 && getHeightDiff(node->left) < 0)
		{
			node->left = RotateLeft(node->left);
			node = RotateRight(node);
		}

		// RL -> Right-Left Rotation
		if (heightDiff < -1 && getHeightDiff(node->right) > 0)
		{
			node->right = RotateRight(node->right);
			node = RotateLeft(node);
		}
	}

	Node* RotateLeft(Node* parent)
	{
		Node* rightChild = parent->right;

		assert(rightChild != nullptr);

		parent->right = rightChild->left;
		rightChild->left = parent;

		return rightChild;
	}

	Node* RotateRight(Node* parent)
	{
		Node* leftChild = parent->left;

		assert(leftChild != nullptr);

		parent->left = leftChild->right;
		leftChild->right = parent;

		return leftChild;
	}

	void Insert(const Item& item)
	{
		root_ = Insert(root_, item);
	}

	Node* Insert(Node* node, const Item& item)
	{
		if (!node)
			return new Node{ item, nullptr, nullptr };

		const auto& key = item.key;

		if (key < node->item.key)
			node->left = Insert(node->left, item);
		else if (key > node->item.key)
			node->right = Insert(node->right, item);
		else
		{
			node->item = item;
			return node;
		}

		// 균형 잡기
		balance(node);

		return node;
	}

	Node* MinKeyNode(Node* node)
	{
		assert(node);
		while (node->left)
			node = node->left;
		return node;
	}

	void Remove(const K& key)
	{
		using namespace std;
		cout << "Remove " << key << endl;
		root_ = Remove(root_, key);
	}

	Node* Remove(Node* node, const K& key)
	{
		// BST와 동일
		if (!node) return node;

		if (key < node->item.key)
			node->left = Remove(node->left, key);
		else if (key > node->item.key)
			node->right = Remove(node->right, key);
		else
		{
			if (!node->left)
			{
				Node* temp = node->right;
				delete node;
				return temp;
			}
			else if (!node->right)
			{
				Node* temp = node->left;
				delete node;
				return temp;
			}

			Node* temp = MinKeyNode(node->right);
			node->item = temp->item;
			node->right = Remove(node->right, temp->item.key);
		}

		//if (node == NULL)	return node; // 불필요 (입력 node의 자식이 하나이거나 없는 경우에는 위에서 return 하기 때문)

		// 균형 잡기
		balance(node);

		return node;
	}

private:
	Node*& root_ = BinarySearchTree<K, V>::root_;
	// this->root_로도 사용 가능
};