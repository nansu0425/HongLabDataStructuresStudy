#pragma once

#include <cassert>
#include <stdint.h>

template<typename T>
class SinglyLinkedList
{
public:
	struct Node
	{
		T item = T();
		Node* next = nullptr;
	};

	SinglyLinkedList()
	{
	}

	SinglyLinkedList(const SinglyLinkedList& list)
	{
		// TODO: 연결 리스트 복사
		if (list.first_)
		{
			first_ = new Node(*list.first_);
			Node* cur = first_;

			while (cur->next)
			{
				cur->next = new Node(*(cur->next));
				cur = cur->next;
			}
		}
	}

	~SinglyLinkedList()
	{
		Clear();
	}

	void Clear() // 모두 지워야(delete) 합니다.
	{
		// TODO: 모두 삭제
		Node* del = first_;

		while (del)
		{
			Node* prevDel = del;
			del = del->next;
			delete prevDel;
		}
	}

	bool IsEmpty()
	{
		return first_ == nullptr;
	}

	int Size()
	{
		int size = 0;

		// TODO: size를 하나하나 세어서 반환
		Node* cur = first_;

		while (cur)
		{
			++size;
			cur = cur->next;
		}

		return size;
	}

	T Front()
	{
		assert(first_);

		return first_->item; // TODO: 수정
	}

	T Back()
	{
		assert(first_);

		Node* cur = first_;

		while (cur->next)
		{
			cur = cur->next;
		}

		return cur->item; // TODO: 수정
	}

	Node* Find(T item)
	{
		// TODO: item이 동일한 노드 포인터 반환

		Node* cur = first_;

		while (cur)
		{
			if (cur->item == item)
			{
				return cur;
			}

			cur = cur->next;
		}

		return nullptr;
	}

	void InsertBack(Node* node, T item)
	{
		// TODO:
		if (node)
		{
			node->next = new Node{ item, node->next };
		}
	}

	void Remove(Node* n)
	{
		assert(first_ && n);

		// 하나 앞의 노드를 찾아야 합니다.
		// TODO:
		if (n == first_)
		{
			first_ = first_->next;
		}
		else
		{
			Node* prevDel = first_;

			while (prevDel && prevDel->next != n)
			{
				prevDel = prevDel->next;
			}

			assert(prevDel);

			prevDel->next = n->next;
		}

		delete n;
	}

	void PushFront(T item)
	{
		// first_가 nullptr인 경우와 아닌 경우 나눠서 생각해보기 (결국은 두 경우를 하나로 합칠 수 있음)

		Node* newFirst = new Node{ item, first_ };
		first_ = newFirst;
	}

	void PushBack(T item)
	{
		if (first_)
		{
			// TODO:
			Node* last = first_;

			while (last->next)
			{
				last = last->next;
			}

			last->next = new Node{ item, nullptr };
		}
		else
		{
			// TODO:
			PushFront(item);
		}
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

		// TODO: 메모리 삭제
		Remove(first_);
	}

	void PopBack()
	{
		if (IsEmpty())
		{
			using namespace std;
			cout << "Nothing to Pop in PopBack()" << endl;
			return;
		}

		// 맨 뒤에서 하나 앞의 노드를 찾아야 합니다.

		assert(first_);

		// TODO: 메모리 삭제
		if (first_->next == nullptr)
		{
			Remove(first_);
		}
		else
		{
			Node* prevDel = first_;

			while (prevDel->next->next)
			{
				prevDel = prevDel->next;
			}

			delete prevDel->next;
			prevDel->next = nullptr;
		}
	}

	void Reverse()
	{
		// TODO: 
		if (first_)
		{
			Node* newNext = nullptr;

			while (first_)
			{
				Node* oldNext = first_->next;
				first_->next = newNext;
				newNext = first_;
				first_ = oldNext;
			}

			first_ = newNext;
		}
	}

	void SetPrintDebug(bool flag)
	{
		print_debug_ = flag;
	}

	void Print()
	{
		using namespace std;

		Node* current = first_;

		if (IsEmpty())
			cout << "Empty" << endl;
		else
		{
			cout << "Size = " << Size() << " ";

			while (current)
			{
				if (print_debug_)
				{
					//cout << "[" << current << ", " << current->item << ", " << current->next << "]";

					// 주소를 짧은 정수로 출력 (앞 부분은 대부분 동일하기때문에 뒷부분만 출력)
					cout << "[" << reinterpret_cast<uintptr_t>(current) % 100000 << ", "
						<< current->item << ", "
						<< reinterpret_cast<uintptr_t>(current->next) % 100000 << "]";
				}
				else
				{
					cout << current->item;
				}

				if (current->next)
					cout << " -> ";
				else
					cout << " -> NULL";

				current = current->next;
			}
			cout << endl;
		}
	}

protected:
	Node* first_ = nullptr;

	bool print_debug_ = false;
};