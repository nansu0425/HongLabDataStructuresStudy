#include <iostream>
#include "Matrix.h"

int main()
{
	// 4x5 행렬 생성
	Matrix mat1(4, 5);
	mat1.SetEntry({ 2, 3, 5 });
	mat1.SetEntry({ 1, 1, 1 });
	mat1.SetEntry({ 1, 1, 4 });
	mat1.SetEntry({ 3, 2, 8 });
	mat1.SetEntry({ 0, 0, 7 });
	mat1.SetEntry({ 2, 2, 3 });

	std::cout << "mat1\n" << mat1 << std::endl;
	std::cout << "transpose of mat1\n" << mat1.GetTranspose() << std::endl;

	std::cout << std::endl;

	// 4x5 행렬 생성
	Matrix mat2(4, 5);
	mat2.SetEntry({ 1, 2, 5 });
	mat2.SetEntry({ 3, 2, 1 });
	mat2.SetEntry({ 2, 0, 4 });
	mat2.SetEntry({ 1, 4, 8 });
	mat2.SetEntry({ 3, 1, 7 });
	mat2.SetEntry({ 0, 2, 3 });

	std::cout << "mat2\n" << mat2 << std::endl;
	std::cout << "mat1 + mat2\n" << mat1 + mat2 << std::endl;

	std::cout << std::endl;

	// Vector 내적
	Vector vec1 = mat1.GetRowVector(2);
	Vector vec2 = mat2.GetRowVector(3);

	std::cout << "mat1 row2: " << vec1 << std::endl;
	std::cout << "mat2 row3: " << vec2 << std::endl;
	std::cout << "dot product: " << Vector::GetDotProduct(vec1, vec2) << std::endl;

	std::cout << std::endl;

	// 행렬 곱
	Matrix mat3(5, 2);
	mat3.SetEntry({ 1, 0, 5 });
	mat3.SetEntry({ 0, 1, 3 });
	mat3.SetEntry({ 2, 1, 1 });
	mat3.SetEntry({ 4, 0, 2 });
	mat3.SetEntry({ 3, 0, 9 });
	mat3.SetEntry({ 3, 1, 7 });

	std::cout << "mat3\n" << mat3 << std::endl;
	std::cout << "mat1 * mat3\n" << mat1 * mat3 << std::endl;
	std::cout << "mat2 * mat3\n" << mat2 * mat3 << std::endl;

	std::cout << std::endl;

	return 0;
}
