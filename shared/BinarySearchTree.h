#pragma once

#include <iomanip>
#include <cassert>
#include <iostream>

template<typename K, typename V>
class BinarySearchTree
{
public:
	struct Item // key와 value의 쌍(pair)
	{
		K key = K();	// first
		V value = V();	// second
	};

	struct Node
	{
		Item item;

		Node* left = nullptr;
		Node* right = nullptr;
	};

	void Preorder()
	{
		using namespace std;
		Preorder(root_);
		cout << endl;
	}

	void Preorder(Node* node)
	{
		using namespace std;
		if (node)
		{
			cout << node->item.key << " ";
			Preorder(node->left);
			Preorder(node->right);
		}
	}

	void Inorder()
	{
		using namespace std;
		Inorder(root_);
		cout << endl;
	}

	void Inorder(Node* node)
	{
		using namespace std;
		if (node)
		{
			Inorder(node->left);
			cout << node->item.key << " ";
			Inorder(node->right);
		}
	}

	Item* RecurGet(const K& key)
	{
		return RecurGet(root_, key);
	}

	Item* RecurGet(Node* node, const K& key)
	{
		if (!node) return nullptr; // <- 동일한 키를 가진 노드를 찾지 못하면 nullptr을 반환하는 기능

		if (key < node->item.key)
			return RecurGet(node->left, key);
		if (key > node->item.key)
			return RecurGet(node->right, key);

		return &node->item; // if (key == node->item.key)
	}

	Item* IterGet(const K& key)
	{
		Node* pCur = root_;

		while (root_ != nullptr)
		{
			if (key < pCur->item.key)
			{
				pCur = pCur->left;
			}
			else if (key > pCur->item.key)
			{
				pCur = pCur->right;
			}
			else
			{
				return &pCur->item;
			}
		}

		return nullptr; // No matching
	}

	void Insert(const Item& item)
	{
		using namespace std;
		cout << "Insert " << item.key << item.value << endl;
		root_ = Insert(root_, item);
	}

	Node* Insert(Node* node, const Item& item)
	{
		// 힌트: RecurGet()

		if (node == nullptr)
		{
			node = new Node{ item, nullptr, nullptr };
		}
		else
		{
			if (item.key < node->item.key)
			{
				node->left = Insert(node->left, item);
			}
			else if (item.key > node->item.key)
			{
				node->right = Insert(node->right, item);
			}
			else
			{
				node->item.value = item.value;
			}
		}

		return node;
	}

	void IterInsert(const Item& item)
	{
		if (root_ == nullptr)
		{
			root_ = new Node{ item, nullptr, nullptr };
			return;
		}

		Node* pCur = root_;
		Node** pTarget = nullptr;

		while (pCur != nullptr)
		{
			if (item.key < pCur->item.key)
			{
				pTarget = &pCur->left;
				pCur = pCur->left; 
			}
			else if (item.key > pCur->item.key)
			{
				pTarget = &pCur->right;
				pCur = pCur->right;
			}
			else
			{
				pCur->item.value = item.value;
				return;
			}
		}

		*pTarget = new Node{ item, nullptr, nullptr };
	}

	Node* MinKeyLeft(Node* node)
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
		if (!node) return node;

		if (key < node->item.key)
			node->left = Remove(node->left, key);
		else if (key > node->item.key)
			node->right = Remove(node->right, key);
		else
		{
			Node* pDel = node;

			// 양쪽 자식 노드 모두 있는 경우
			if ((node->left != nullptr) && (node->right != nullptr))
			{
				pDel = MinKeyLeft(node->right);
				node->item = pDel->item;

				node->right = Remove(node->right, pDel->item.key);
			}
			else
			{
				// 왼쪽 자식 노드가 없을 때
				if (node->left == nullptr)
				{
					node = node->right;
				}
				// 오른쪽 자식 노드가 없을 때
				else if (node->right == nullptr)
				{
					node = node->left;
				}

				delete pDel;
			}
		}

		return node;
	}

	int Height()
	{
		return Height(root_);
	}

	int Height(Node* node)
	{
		if (!node) return 0;
		return 1 + std::max(Height(node->left), Height(node->right));
	}

	void Print2D();
	void PrintLevel(int n);
	void DisplayLevel(Node* p, int lv, int d);

protected:
	Node* root_ = nullptr;
};

// 디버깅 편의 도구
// https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
template<typename K, typename V>
void BinarySearchTree<K, V>::Print2D()
{
	using namespace std;
	cout << "Height = " << Height() << endl;
	int i = 0;
	while (i < Height()) {
		PrintLevel(i);
		i++;
		cout << endl;
	}
	cout << endl;
}

template<typename K, typename V>
void BinarySearchTree<K, V>::PrintLevel(int n) {
	using namespace std;
	Node* temp = root_;
	int val = (int)pow(2.0, Height() - n + 1.0);
	cout << setw(val) << "";
	DisplayLevel(temp, n, val);
}

template<typename K, typename V>
void BinarySearchTree<K, V>::DisplayLevel(Node* p, int lv, int d) {
	using namespace std;
	int disp = 2 * d;
	if (lv == 0) {
		if (p == NULL) {
			cout << "   ";
			cout << setw(disp - 3) << "";
			return;
		}
		else {
			int result = ((p->item.key <= 1) ? 1 : (int)log10(p->item.key) + 1);
			cout << " " << p->item.key << p->item.value << " ";
			cout << setw(static_cast<streamsize>(disp) - result - 2) << "";
		}
	}
	else
	{
		if (p == NULL && lv >= 1) {
			DisplayLevel(NULL, lv - 1, d);
			DisplayLevel(NULL, lv - 1, d);
		}
		else {
			DisplayLevel(p->left, lv - 1, d);
			DisplayLevel(p->right, lv - 1, d);
		}
	}
}
