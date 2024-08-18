#include "Matrix.h"

#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

Matrix::Matrix(int num_rows, int num_cols)
{
	// TODO:
	values_ = new float[num_rows * num_cols];
	::memset(values_, 0, sizeof(*values_) * num_rows * num_cols);

	num_rows_ = num_rows;
	num_cols_ = num_cols;
}

// 복사 생성자 (b를 복사)
Matrix::Matrix(const Matrix& b)
{
	// TODO:
	values_ = new float[b.num_rows_ * b.num_cols_];
	::memcpy(values_, b.values_, sizeof(*values_) * b.num_rows_ * b.num_cols_);

	num_rows_ = b.num_rows_;
	num_cols_ = b.num_cols_;
}

Matrix::~Matrix()
{
	// TODO:
	if (values_)
	{
		delete[] values_;
	}
}

void Matrix::SetValue(int row, int col, float value)
{
	// values_[TODO] = value;
	values_[row * num_cols_ + col] = value;

	return;
}

float Matrix::GetValue(int row, int col) const // 맨 뒤의 const는 함수 안에서 멤버 변수의 값을 바꾸지 않겠다는 의미
{
	return values_[row * num_cols_ + col];
}

Matrix Matrix::Transpose() const
{
	Matrix temp(num_cols_, num_rows_); // num_cols_, num_rows_ 순서 주의

	// TODO:
	for (int row = 0; row < num_rows_; ++row)
	{
		for (int col = 0; col < num_cols_; ++col)
		{
			temp.SetValue(col, row, GetValue(row, col));
		}
	}

	return temp;
}

Matrix Matrix::Add(const Matrix& b)
{
	assert(b.num_cols_ == num_cols_);
	assert(b.num_rows_ == num_rows_);

	Matrix temp(num_rows_, num_cols_);

	// TODO:
	for (int row = 0; row < num_rows_; ++row)
	{
		for (int col = 0; col < num_cols_; ++col)
		{
			temp.SetValue(row, col, GetValue(row, col) + b.GetValue(row, col));
		}
	}

	return temp;
}

Matrix Matrix::Multiply(const Matrix& mul)
{
	assert(num_cols_ == mul.num_rows_);

	Matrix result(num_rows_, mul.num_cols_);
	Matrix mulTranspose = mul.Transpose();

	for (int row = 0; row < num_rows_; ++row)
	{
		Vector a = { &values_[row * num_cols_], num_cols_ };

		for (int col = 0; col < mul.num_cols_; ++col)
		{
			Vector b = { &mulTranspose.values_[col * mulTranspose.num_cols_], mulTranspose.num_cols_ };
			result.SetValue(row, col, getDotProduct(a, b));
		}
	}

	return result;
}

void Matrix::Print()
{
	for (int r = 0; r < num_rows_; r++)
	{
		for (int c = 0; c < num_cols_; c++)
			cout << GetValue(r, c) << " ";

		cout << endl;
	}
}

float getDotProduct(const Vector& a, const Vector& b)
{
	assert(a.dimension == b.dimension);

	float sum = 0;

	for (int i = 0; i < a.dimension; ++i)
	{
		sum += a.components[i] * b.components[i];
	}

	return sum;
}
