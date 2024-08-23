#pragma once

#include <iostream>
#include <fstream>
#include <string> // BinaryTree 출력
#include <algorithm>

#include "Queue.h"
#include "Stack.h"

template<typename T>
class BinaryTree
{
public:
	struct Node
	{
		T item = T();
		Node* left = nullptr; // Left child
		Node* right = nullptr; // Right child

		~Node()
		{
			std::cout << "Deleted " << item << std::endl;
		}
	};

	// 디버깅 도구: 큐에서 주소 대신에 아이템 출력
	class MyQueue : public Queue<Node*>
	{
	public:
		void Print()
		{
			using namespace std;

			for (int i = (this->front_ + 1) % this->capacity_; i != (this->rear_ + 1) % this->capacity_; i = (i + 1) % this->capacity_)
				cout << this->queue_[i]->item << " ";
			cout << endl;
		}
	};

	// 디버깅 도구: 스택에서 주소 대신에 아이템 출력
	class MyStack : public Stack<Node*>
	{
	public:
		void Print()
		{
			using namespace std;

			for (int i = 0; i < this->Size(); i++)
				cout << this->stack_[i]->item << " ";
			cout << endl;
		}
	};

	BinaryTree() {}

	BinaryTree(Node* root)
	{
		root_ = root;
	}

	bool IsEmpty()
	{
		return root_ == nullptr;
	}

	void Visit(Node* node)
	{
		using namespace std;
		cout << node->item << " "; // 수행하고 싶은 작업 구현(여기서는 출력)
	}

	int Sum()
	{
		return Sum(root_);
	}

	int Sum(Node* node)
	{
		if (node == nullptr)
		{
			return 0;
		}

		return node->item + Sum(node->left) + Sum(node->right); 
	}

	int Height()
	{
		return Height(root_);
	}

	int Height(Node* node)
	{
		if (node == nullptr)
		{
			return 0;
		}

		return std::max(Height(node->left), Height(node->right)) + 1; 
	}

	~BinaryTree()
	{
		DeleteTree(root_);
	}

	void DeleteTree(Node* node)
	{
		if (node)
		{
			DeleteTree(node->left);
			DeleteTree(node->right);
			delete node;
		}
	}

	void Preorder() { Preorder(root_); }
	void Preorder(Node* node)
	{
		if (node == nullptr)
		{
			return;
		}

		Visit(node);
		Preorder(node->left);
		Preorder(node->right);
	};

	void Inorder() { Inorder(root_); }
	void Inorder(Node* node)
	{
		if (node == nullptr)
		{
			return;
		}

		Inorder(node->left);
		Visit(node);
		Inorder(node->right);
	}

	void Postorder() { Postorder(root_); }
	void Postorder(Node* node)
	{
		if (node == nullptr)
		{
			return;
		}

		Postorder(node->left);
		Postorder(node->right);
		Visit(node);
	}

	void LevelOrder()
	{
		Queue<Node*> q; // 힌트: MyQueue q;
		q.Enqueue(root_);

		while (!q.IsEmpty())
		{
			Node* cur = q.Front();
			q.Dequeue();

			Visit(cur);
			
			if (cur->left)
			{
				q.Enqueue(cur->left);
			}

			if (cur->right)
			{
				q.Enqueue(cur->right);
			}
		}
	}

	void IterPreorder()
	{
		if (!root_) return;

		Stack<Node*> s; // 힌트: MyStack q;
		s.Push(root_);

		while (!s.IsEmpty())
		{
			Node* cur = s.Top();
			s.Pop();

			Visit(cur);

			if (cur->right)
			{
				s.Push(cur->right);
			}

			if (cur->left)
			{
				s.Push(cur->left);
			}
		}
	}

	void IterInorder()
	{
		if (!root_) return;

		Stack<Node*> s;
		Node* cur = root_;

		// 순회 순서: left -> root -> right
		while (cur || !s.IsEmpty())
		{
			// cur이 nullptr가 아니면 왼쪽으로 내려간다
			while (cur)
			{
				s.Push(cur);
				cur = cur->left;
			}

			assert(cur == nullptr);
			
			cur = s.Top();
			s.Pop();

			Visit(cur);

			cur = cur->right;
		}
	}

	void IterPostorder()
	{
		if (!root_) return;

		Stack<Node*> s1, s2;
		s1.Push(root_);

		while (!s1.IsEmpty())
		{
			s2.Push(s1.Top());
			s1.Pop();

			Node* cur = s2.Top();

			if (cur->left)
			{
				s1.Push(cur->left);
			}
			
			if (cur->right)
			{
				s1.Push(cur->right);
			}
		}

		while (!s2.IsEmpty())
		{
			Visit(s2.Top());
			s2.Pop();
		}
	}

	void Print2D();
	void PrintLevel(int n);
	void DisplayLevel(Node* p, int lv, int d);

protected:
	Node* root_ = nullptr;
};

// 디버깅 편의 도구
// https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
template<typename T>
void BinaryTree<T>::Print2D()
{
	using namespace std;
	int i = 0;
	while (i < Height()) {
		PrintLevel(i);
		i++;
		cout << endl;
	}
}

template<typename T>
void BinaryTree<T>::PrintLevel(int n) {
	using namespace std;
	Node* temp = root_;
	int val = (int)pow(2.0, Height() - n + 1.0);
	cout << setw(val) << "";
	DisplayLevel(temp, n, val);
}

template<typename T>
void BinaryTree<T>::DisplayLevel(Node* p, int lv, int d) {
	using namespace std;
	int disp = 2 * d;
	if (lv == 0) {
		if (p == NULL) {
			cout << "   ";
			cout << setw(disp - 3) << "";
			return;
		}
		else {
			int result = ((p->item <= 1) ? 1 : (int)log10(p->item) + 1);
			cout << " " << p->item << " ";
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
