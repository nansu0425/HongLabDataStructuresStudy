#include <iostream>
#include "../shared/SinglyLinkedList.h"

using namespace std;

struct Term
{
	float coef;
	int exp;
};

class LinkedPolynomial : public SinglyLinkedList<Term>
{
public:
	using Node = SinglyLinkedList<Term>::Node;

	void NewTerm(float coef, int exp)
	{
		// 노드가 없는 경우
		if (first_ == nullptr)
		{
			first_ = new Node{ {coef, exp}, nullptr };
			return;
		}

		// 첫 번째 항 앞에 삽입하는 경우
		if (exp < first_->item.exp)
		{
			first_ = new Node{ {coef, exp}, first_ };
			return;
		}
		// 첫 번째 항과 차수가 같은 경우
		else if (exp == first_->item.exp)
		{
			first_->item.coef += coef;
			return;
		}

		Node* cur = first_;

		// cur으로 삽입할 위치를 순차적으로 탐색
		while (cur->next && (cur->next->item.exp < exp))
		{
			cur = cur->next;
		}

		// 추가하려는 항의 차수와 같은 차수의 항인 경우
		if (cur->next && (cur->next->item.exp == exp))
		{
			cur->next->item.coef += coef;
		}
		// cur에 해당하는 항 뒤에 새로운 항을 삽입해야 한다
		else
		{
			cur->next = new Node{ {coef, exp}, cur->next };
		}
	}

	float Eval(float x)
	{
		float temp = 0.0f;

		// TODO:
		Node* cur = first_;

		while (cur)
		{
			temp += cur->item.coef * std::powf(x, cur->item.exp);
			cur = cur->next;
		}

		return temp;
	}

	LinkedPolynomial Add(const LinkedPolynomial& poly)
	{
		// this와 poly의 terms_가 exp의 오름차순으로 정렬되어 있다고 가정
		// 하나의 다항식 안에 exp가 중복되는 term이 없다라고 가정 (한 exp는 하나의 term만 존재)

		LinkedPolynomial temp;

		Node* thisCur = this->first_;
		Node* paramCur = poly.first_;

		while (thisCur && paramCur)
		{
			if (thisCur->item.exp < paramCur->item.exp)
			{
				temp.PushBack(thisCur->item);
				thisCur = thisCur->next;
			}
			else if (thisCur->item.exp > paramCur->item.exp)
			{
				temp.PushBack(paramCur->item);
				paramCur = paramCur->next;
			}
			else
			{
				temp.PushBack({ thisCur->item.coef + paramCur->item.coef, thisCur->item.exp });
				thisCur = thisCur->next;
				paramCur = paramCur->next;
			}
		}

		assert((thisCur == nullptr) || (paramCur == nullptr));

		while (thisCur)
		{
			temp.PushBack(thisCur->item);
			thisCur = thisCur->next;
		}

		while (paramCur)
		{
			temp.PushBack(paramCur->item);
			paramCur = paramCur->next;
		}

		assert((thisCur == nullptr) && (paramCur == nullptr));

		return temp;
	}

	void Print()
	{
		bool is_first = true; // 더하기 출력시 확인용

		Node* cur = first_;

		while (cur)
		{
			if (is_first)
			{
				std::cout << cur->item.coef;

				if (cur->item.exp > 0)
				{
					std::cout << "x^" << cur->item.exp;
				}

				is_first = false;
			}
			else
			{
				std::cout << " + " << cur->item.coef << "x^" << cur->item.exp;
			}

			cur = cur->next;
		}

		cout << endl;
	}

private:
};

int main()
{
	using namespace std;

	LinkedPolynomial p1;

	p1.NewTerm(2.0f, 2);	// 2 * x^2
	p1.NewTerm(1.5f, 1);	// 1.5 * x^1
	p1.NewTerm(1.0f, 0);	// 1 * x^0 = 1
	
	p1.Print(); // 1 + 1.5*x^1 + 2*x^2

	cout << p1.Eval(0.0f) << endl; // 1 + 1.5*0 + 2*0^2 = 1
	cout << p1.Eval(1.0f) << endl; // 1 + 1.5*1 + 2*1^2 = 4.5
	cout << p1.Eval(2.0f) << endl; // 1 + 1.5*2 + 2*2^2 = 1 + 3 + 8 = 12

	cout << endl;

	// Add() Test1
	cout << "Add() Test" << endl;
	{
		LinkedPolynomial p1; // max_degree는 기본값으로 설정

		// exp가 작은 항부터 추가한다고 가정
		p1.NewTerm(1.0f, 0);
		p1.NewTerm(1.5f, 1);
		p1.NewTerm(2.0f, 2);

		p1.Print(); // 1 + 1.5*x^1 + 2*x^2

		LinkedPolynomial p2;

		// exp가 작은 항부터 추가한다고 가정
		p2.NewTerm(1.0f, 1);
		p2.NewTerm(3.0f, 2);
		p2.NewTerm(5.0f, 7);
		p2.NewTerm(2.0f, 11);

		p2.Print(); // 1*x^1 + 3*x^2 + 5*x^7 + 2*x^11

		cout << endl;

		LinkedPolynomial psum = p1.Add(p2);
		psum.Print(); // 1 + 2.5*x^1 + 5*x^2 + 5*x^7 + 2*x^11

		cout << endl;
	}

	// Add() Test2
	cout << "Add() Test2" << endl;
	{
		LinkedPolynomial p1; // max_degree는 기본값으로 설정

		p1.NewTerm(3.5f, 10);
		p1.NewTerm(5.5f, 20);
		p1.NewTerm(1.0f, 0);
		p1.NewTerm(5.0f, 1000);
		p1.NewTerm(2.0f, 2);
		p1.NewTerm(5.0f, 7);
		p1.NewTerm(1.5f, 1);

		p1.Print(); // 1 + 1.5*x^1 + 2*x^2 + 5*x^7 + 3.5*x^10 + 5.5*x^20 + 5*x^1000

		LinkedPolynomial p2;

		p2.NewTerm(2.0f, 11);
		p2.NewTerm(3.0f, 2);
		p2.NewTerm(3.2f, 0);
		p2.NewTerm(1.0f, 1);

		p2.Print(); // 3.2 + 1*x^1 + 3*x^2 + 2*x^11

		cout << endl;

		LinkedPolynomial psum = p1.Add(p2);
		psum.Print(); // 4.2 + 2.5*x^1 + 5*x^2 + 5*x^7 + 3.5*x^10 + 2*x^11 + 5.5*x^20 + 5*x^1000

		cout << endl;
	}

	return 0;
}
