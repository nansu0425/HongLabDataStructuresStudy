#include "SparsePolynomial.h"

#include <iostream>
#include <cassert>
#include <algorithm>

using namespace std;

SparsePolynomial::SparsePolynomial(const SparsePolynomial& poly)
{
	terms_ = new Term[poly.capacity_];
	::memcpy(terms_, poly.terms_, poly.capacity_);

	capacity_ = poly.capacity_;
	num_terms_ = poly.num_terms_;
}

// 새로운 항을 추가할 때 자기 위치를 찾아서 넣어줘야 함
// exp항이 이미 존재하면 거기에 coef를 더해준다.
// 존재하지 않는다면 exp 오름 차순으로 정렬된 상태로 새로 추가한다.
void SparsePolynomial::NewTerm(float coef, int exp)
{
	if (coef == 0.0f) return; // 0이면 추가하지 않음

	if (num_terms_ >= capacity_)
	{
		// capacity 증가 (num_terms는 일단 고정)
		capacity_ = capacity_ > 0 ? capacity_ * 2 : 1; // 2배씩 증가
		Term* new_term = new Term[capacity_];

		// 원래 가지고 있던 데이터 복사
		memcpy(new_term, terms_, sizeof(Term) * num_terms_);

		// 메모리 교체
		if (terms_) delete[] terms_;
		terms_ = new_term;
	}

	terms_[num_terms_].coef = coef;
	terms_[num_terms_].exp = exp;

	int insertionIdx = num_terms_;

	// 새로운 항을 삽입할 index를 탐색
	while ((insertionIdx > 0) && (terms_[insertionIdx - 1].exp > terms_[num_terms_].exp))
	{
		--insertionIdx;
	}

	// num_terms_가 삽입할 인덱스인 경우
	if (insertionIdx == num_terms_)
	{
		++num_terms_;

		return;
	}

	// 차수가 같은 항이 있는 경우
	if (terms_[insertionIdx].exp == terms_[num_terms_].exp)
	{
		terms_[insertionIdx].coef += terms_[num_terms_].coef;

		return;
	}
	// 차수가 같은 항이 없는 경우
	else
	{
		// 삽입할 항 임시 저장
		Term insertionTerm = terms_[num_terms_];
		
		// 삽입 index 뒤의 모든 항을 뒤로 이동
		for (int shiftIdx = num_terms_; insertionIdx < shiftIdx; --shiftIdx)
		{
			terms_[shiftIdx] = terms_[shiftIdx - 1];
		}

		// 새로운 항 삽입
		terms_[insertionIdx] = insertionTerm;
		++num_terms_;

		return;
	}
}

float SparsePolynomial::Eval(float x)
{
	float temp = 0.0f;

	// TODO:
	for (int termIdx = 0; termIdx < num_terms_; ++termIdx)
	{
		temp += terms_[termIdx].coef * std::powf(x, terms_[termIdx].exp);
	}

	return temp;
}

SparsePolynomial SparsePolynomial::Add(const SparsePolynomial& poly)
{
	// this와 poly의 terms_가 exp의 오름차순으로 정렬되어 있다고 가정
	// 하나의 다항식 안에 exp가 중복되는 term이 없다라고 가정 (한 exp는 하나의 term만 존재)

	// 간단한 방법 (메모리를 더 사용하는 방법)
	// - 1. 최대 exp를 찾는다.
	// - 2. 필요한 크기의 Polynomial을 만든다. (Sparse 아님)
	// - 3. 더하면서 Polynomial에 업데이트 한다. 구조가 고정되어 있어서 쉽다.
	// - 4. Polynomial을 SparsePolynomial로 변환한다.

	SparsePolynomial temp;

	// TODO:
	int thisTermIdx = 0;
	int paramTermIdx = 0;

	while (thisTermIdx < num_terms_ && paramTermIdx < poly.num_terms_)
	{
		if (terms_[thisTermIdx].exp < poly.terms_[paramTermIdx].exp)
		{
			temp.NewTerm(terms_[thisTermIdx].coef, terms_[thisTermIdx].exp);
			++thisTermIdx;
		}
		else if (terms_[thisTermIdx].exp > poly.terms_[paramTermIdx].exp)
		{
			temp.NewTerm(poly.terms_[paramTermIdx].coef, poly.terms_[paramTermIdx].exp);
			++paramTermIdx;
		}
		else
		{
			float sumCoef{ terms_[thisTermIdx].coef + poly.terms_[paramTermIdx].coef };
			temp.NewTerm(sumCoef, terms_[thisTermIdx].exp);
			++thisTermIdx;
			++paramTermIdx;
		}
	}

	for (int termIdx = thisTermIdx; termIdx < num_terms_; ++termIdx)
	{
		temp.NewTerm(terms_[termIdx].coef, terms_[termIdx].exp);
	}

	for (int termIdx = paramTermIdx; termIdx < poly.num_terms_; ++termIdx)
	{
		temp.NewTerm(poly.terms_[termIdx].coef, poly.terms_[termIdx].exp);
	}

	return temp;
}

void SparsePolynomial::Print()
{
	bool is_first = true; // 더하기 출력시 확인용

	for (int i = 0; i < num_terms_; i++)
	{
		// 0이 아닌 항들만 골라서 출력할 필요가 없음

		if (!is_first)
			cout << " + "; // 첫 항이 아니라면 사이사이에 더하기 출력

		cout << terms_[i].coef;

		if (terms_[i].exp != 0) cout << "*" << "x^" << terms_[i].exp;

		is_first = false;
	}

	cout << endl;
}
