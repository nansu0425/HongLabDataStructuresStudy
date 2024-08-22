#pragma once

#include <cassert>
#include <stdint.h>
#include <algorithm>

template<typename T>
class DoublyLinkedList
{
public:
	struct Node
	{
		T item = T();

		Node* left = nullptr;
		Node* right = nullptr;

		// 참고: prev/next가 아니라 left/right
	};

	DoublyLinkedList()
	{
	}

	DoublyLinkedList(const DoublyLinkedList& list)
	{
		if (list.first_ == nullptr)
		{
			return;
		}

		Node* cur = new Node(*list.first_);
		first_ = cur;

		while (cur->right)
		{
			cur->right = new Node{ *(cur->right) };
			cur->right->left = cur;
			cur = cur->right;
		}
	}

	~DoublyLinkedList()
	{
		Clear();
	}

	void Clear() // 모두 지워야(delete) 합니다.
	{
		if (first_ == nullptr)
		{
			return;
		}

		Node* del = first_;

		while (del->right)
		{
			del = del->right;
			delete del->left;
		}

		delete del;
	}

	bool IsEmpty()
	{
		return (first_ == nullptr); // TODO:
	}

	int Size()
	{
		int size = 0;

		// TODO:
		Node* cur = first_;

		while (cur)
		{
			++size;
			cur = cur->right;
		}

		return size;
	}

	void Print()
	{
		using namespace std;

		Node* cur = first_;

		if (IsEmpty())
			cout << "Empty" << endl;
		else
		{
			cout << "Size = " << Size() << endl;

			cout << " Forward: ";
			
			while (cur)
			{
				std::cout << cur->item << " ";

				if (cur->right == nullptr)
				{
					break;
				}

				cur = cur->right;
			}

			cout << endl;

			cout << "Backward: ";
			
			while (cur)
			{
				std::cout << cur->item << " ";

				if (cur->left == nullptr)
				{
					break;
				}

				cur = cur->left;
			}

			cout << endl;
		}
	}

	Node* Find(T item)
	{
		Node* cur = first_;

		while (cur)
		{
			if (cur->item == item)
			{
				return cur;
			}

			cur = cur->right;
		}

		return nullptr;
	}

	void InsertBack(Node* node, T item)
	{
		if (IsEmpty())
		{
			PushBack(item);
		}
		else
		{
			node->right = new Node{ item, node, node->right };
			if (node->right->right)
			{
				node->right->right->left = node->right;
			}
		}
	}

	void PushFront(T item)
	{
		if (IsEmpty())
		{
			PushBack(item);
		}
		else
		{
			first_ = new Node{ item, nullptr, first_ };
			first_->right->left = first_;
		}
	}

	void PushBack(T item)
	{
		if (first_ == nullptr)
		{
			first_ = new Node;
			first_->item = item;
			return;
		}

		Node* cur = first_;

		while (cur->right)
		{
			cur = cur->right;
		}

		cur->right = new Node{ item, cur, nullptr };
	}

	void PopFront()
	{
		if (IsEmpty())
		{
			using namespace std;
			cout << "Nothing to Pop in PopFront()" << endl;
			return;
		}

		assert(first_);

		Node* oldFirst = first_;
		first_ = first_->right;
		
		if (first_)
		{
			first_->left = nullptr;
		}

		delete oldFirst;
	}

	void PopBack()
	{
		if (IsEmpty())
		{
			using namespace std;
			cout << "Nothing to Pop in PopBack()" << endl;
			return;
		}

		assert(first_);

		
		if (first_->right)
		{
			Node* cur = first_;

			while (cur->right->right)
			{
				cur = cur->right;
			}

			delete cur->right;
			cur->right = nullptr;
		}
		else
		{
			delete first_;
			first_ = nullptr;
		}
	}

	void Reverse()
	{
		Node* cur = first_;

		while (cur)
		{
			std::swap(cur->left, cur->right);

			if (cur->left == nullptr)
			{
				first_ = cur;
				break;
			}

			cur = cur->left;
		}
	}

	T Front()
	{
		assert(first_);

		return first_->item; // TODO:
	}

	T Back()
	{
		assert(first_);

		Node* cur = first_;

		while (cur->right)
		{
			cur = cur->right;
		}

		return cur->item; // TODO:
	}

protected:
	Node* first_ = nullptr;
};
