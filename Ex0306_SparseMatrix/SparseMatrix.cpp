#include "SparseMatrix.h"

#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

SparseMatrix::SparseMatrix(int num_rows, int num_cols, int capacity)
{
	// TODO:
	terms_ = new MatrixTerm[capacity];
	::memset(terms_, 0x00, sizeof(*terms_) * capacity);

	num_rows_ = num_rows;
	num_cols_ = num_cols;
	capacity_ = capacity;
	num_terms_ = 0;
}

// 복사 생성자 (b를 복사)
SparseMatrix::SparseMatrix(const SparseMatrix& b)
{
	// TODO:
	terms_ = new MatrixTerm[b.capacity_];
	::memcpy(terms_, b.terms_, sizeof(*b.terms_) * capacity_);

	num_rows_ = b.num_rows_;
	num_cols_ = b.num_cols_;
	capacity_ = b.capacity_;
	num_terms_ = b.num_terms_;
}

SparseMatrix::~SparseMatrix()
{
	// TODO:
	if (terms_)
	{
		delete[] terms_;
	}
}

void SparseMatrix::IncreaseCapacity()
{
	assert(capacity_ == num_terms_);

	MatrixTerm* newTerms = new MatrixTerm[capacity_ * 2];
	::memcpy(newTerms, terms_, sizeof(*terms_) * capacity_);
	delete[] terms_;

	terms_ = newTerms;
	capacity_ *= 2;

	return;
}

int SparseMatrix::GetTermOrderKey(const MatrixTerm& term) const
{
	return term.row * num_cols_ + term.col;
}

int SparseMatrix::GetTermOrderKey(int row, int col)
{
	return row * num_cols_ + col;
}

void SparseMatrix::SetValue(int row, int col, float value)
{
	if (value == 0.0f) return; // value가 0이 아닌 term만 저장

	int paramOrderKey = row * num_cols_ + col;
	int insertionIdx = 0;

	// TODO:
	for (; insertionIdx < num_terms_; ++insertionIdx)
	{
		int insertionOrderKey = GetTermOrderKey(terms_[insertionIdx]);

		// 파라미터의 행, 열과 같은 MatrixTerm이 있는 경우
		if (insertionOrderKey == paramOrderKey)
		{
			terms_[insertionIdx].value = value;
			return;
		}
		// 파라미터의 행, 열과 같은 MatrixTerm이 없는 경우
		else if (paramOrderKey < insertionOrderKey)
		{
			break;
		}
	}

	if (capacity_ == num_terms_)
	{
		IncreaseCapacity();
	}

	for (int shiftIdx = num_terms_; shiftIdx > insertionIdx; --shiftIdx)
	{
		terms_[shiftIdx] = terms_[shiftIdx - 1];
	}

	terms_[insertionIdx].row = row;
	terms_[insertionIdx].col = col;
	terms_[insertionIdx].value = value;
	++num_terms_;

	return;
}

float SparseMatrix::GetValue(int row, int col) const // 맨 뒤의 const는 함수 안에서 멤버 변수의 값을 바꾸지 않겠다는 의미
{
	// TODO: key = col + num_cols * row;

	int paramOrderKey = row * num_cols_ + col;

	for (int termIdx = 0; termIdx < num_terms_; ++termIdx)
	{
		int termOrderKey = GetTermOrderKey(terms_[termIdx]);

		if (paramOrderKey == termOrderKey)
		{
			return terms_[termIdx].value;
		}
		else if (paramOrderKey < termOrderKey)
		{
			return 0.0f;
		}
	}

	return 0.0f;
}

SparseMatrix SparseMatrix::Transpose()
{
	SparseMatrix temp(num_cols_, num_rows_, capacity_); // num_cols_, num_rows_ 순서 주의

	// TODO:
	// O(num_rows_ * num_cols_ * num_terms_)
	/*for (int row = 0; row < num_rows_; ++row)
	{
		for (int col = 0; col < num_cols_; ++col)
		{
			temp.SetValue(col, row, GetValue(row, col));
		}
	}*/

	// O(num_cols_ * num_terms)
	for (int rowIdx = 0; rowIdx < temp.num_rows_; ++rowIdx)
	{
		for (int termIdx = 0; termIdx < num_terms_; ++termIdx)
		{
			if (terms_[termIdx].col == rowIdx)
			{
				temp.terms_[temp.num_terms_].row = terms_[termIdx].col;
				temp.terms_[temp.num_terms_].col = terms_[termIdx].row;
				temp.terms_[temp.num_terms_].value = terms_[termIdx].value;
				++temp.num_terms_;
			}
		}
	}

	return temp;
}

SparseMatrix SparseMatrix::Add(const SparseMatrix& param) const
{
	assert((num_rows_ == param.num_rows_) && (num_cols_ == param.num_cols_));

	int thisTermIdx = 0;
	int paramTermIdx = 0;

	SparseMatrix result(num_rows_, num_cols_, capacity_);

	while (thisTermIdx < num_terms_ && paramTermIdx < param.num_terms_)
	{
		int thisTermOrderKey = GetTermOrderKey(terms_[thisTermIdx]);
		int paramTermOrderKey = GetTermOrderKey(param.terms_[paramTermIdx]);

		if (thisTermOrderKey < paramTermOrderKey)
		{
			result.SetValue(terms_[thisTermIdx].row, terms_[thisTermIdx].col, terms_[thisTermIdx].value);
			++thisTermIdx;
		}
		else if (thisTermOrderKey > paramTermOrderKey)
		{
			result.SetValue(param.terms_[paramTermIdx].row, param.terms_[paramTermIdx].col, param.terms_[paramTermIdx].value);
			++paramTermIdx;
		}
		else
		{
			result.SetValue(terms_[thisTermIdx].row, terms_[thisTermIdx].col, terms_[thisTermIdx].value + param.terms_[paramTermIdx].value);
			++thisTermIdx;
			++paramTermIdx;
		}
	}

	while (thisTermIdx < num_terms_)
	{
		result.SetValue(terms_[thisTermIdx].row, terms_[thisTermIdx].col, terms_[thisTermIdx].value);
		++thisTermIdx;
	}

	while (paramTermIdx < param.num_terms_)
	{
		result.SetValue(param.terms_[paramTermIdx].row, param.terms_[paramTermIdx].col, param.terms_[paramTermIdx].value);
		++paramTermIdx;
	}

	assert((thisTermIdx == num_terms_) && (paramTermIdx == param.num_terms_));

	return result;
}

void SparseMatrix::PrintTerms()
{
	for (int i = 0; i < num_terms_; i++)
		cout << "(" << terms_[i].row
		<< ", " << terms_[i].col
		<< ", " << terms_[i].value
		<< ")" << endl;
}

void SparseMatrix::Print()
{
	for (int r = 0; r < num_rows_; r++)
	{
		for (int c = 0; c < num_cols_; c++)
			cout << GetValue(r, c) << " ";
		cout << endl;
	}
}
