#include <iostream>
#include <cassert>
#include "Matrix.h"

Matrix::Matrix(int numRows, int numCols) : m_numRows(numRows), m_numCols(numCols)
{
	// Entry 배열 할당 및 초기화
	m_entries = new Entry[m_capacity];
	::memset(m_entries, 0x00, sizeof(Entry) * m_capacity);
}

Matrix::~Matrix()
{
	assert(m_entries != nullptr);

	delete[] m_entries;
}

void Matrix::SetEntry(Entry entry)
{
	// row와 col은 행렬의 행의 개수, 열의 개수보다 작아야 한다
	assert((entry.row < m_numRows) && (entry.col < m_numCols));

	// Entry 배열에 val이 0인 원소는 추가하지 않는다 
	if (entry.val == 0)
	{
		return;
	}

	// Entry 배열 용량이 가득차면 용량을 2배로 증가
	if (m_capacity == m_numEntries)
	{
		DoubleCapacity();
	}

	int orderKey = GetOrderKey(entry);
	int entryIdx = GetEntryIndex(orderKey);

	// row, col에 해당하는 Entry가 없는 경우
	if (entryIdx == -1)
	{
		// Entry 배열의 마지막 원소 뒤에 추가
		m_entries[m_numEntries] = entry;
		++m_numEntries;

		// Entry 배열 원소가 1개보다 많고, 
		// 추가한 Entry가 orderKey 기준으로 오름차순으로 정렬돼 있지 않은 경우
		if ((m_numEntries > 1) && (GetOrderKey(m_entries[m_numEntries - 2]) > orderKey))
		{
			SortLastEntry();
		}
	}
	// row, col에 해당하는 Entry가 있는 경우
	else
	{
		m_entries[entryIdx].val = entry.val;
	}
}

void Matrix::AppendEntry(Entry entry)
{
	// row와 col은 행렬의 행의 개수, 열의 개수보다 작아야 한다
	assert((entry.row < m_numRows) && (entry.col < m_numCols));

	// Entry 배열에 val이 0인 원소는 추가하지 않는다 
	if (entry.val == 0)
	{
		return;
	}

	// Entry 배열 용량이 가득차면 용량을 2배로 증가
	if (m_capacity == m_numEntries)
	{
		DoubleCapacity();
	}

	// Entry 배열의 마지막 원소 뒤에 새로운 Entry 추가
	m_entries[m_numEntries] = entry;
	++m_numEntries;
}

Matrix Matrix::GetTranspose() const
{
	Matrix result(m_numCols, m_numRows);

	// Entry 배열 각 원소의 행, 열을 바꿔서 result의 Entry 배열에 추가
	for (int entryIdx = 0; entryIdx < m_numEntries; ++entryIdx)
	{
		// std::swap(m_entries[entryIdx].row, m_entries[entryIdx].col);
		// 왜 swap 가능한거야
		result.SetEntry({ 
			m_entries[entryIdx].col, m_entries[entryIdx].row, m_entries[entryIdx].val 
			});
	}

	return result;
}

Vector Matrix::GetRowVector(int row) const
{
	assert((-1 < row) && (row < m_numRows));

	if (m_numEntries == 0)
	{
		return { nullptr, 0, m_numCols };
	}

	Vector result;
	result.numCols = m_numCols;

	int entryIdx = GetRowEntryIdx(row);

	if (entryIdx != -1)
	{
		result.entries = &m_entries[entryIdx];

		// row에 해당하는 Entry의 개수를 센다
		while ((entryIdx < m_numEntries) && (m_entries[entryIdx].row == row))
		{
			++result.numEntries;
			++entryIdx;
		}
	}

	return result;
}

Vector Matrix::GetNextRowVector()
{
	assert((-1 < m_nextRow) && (m_nextRow < m_numRows));

	if (m_numEntries == 0)
	{
		return { nullptr, 0, m_numCols };
	}

	Vector result = { &m_entries[m_rowFirstEntryIdx], 0, m_numCols };

	// m_nextRow에 해당하는 Entry의 개수를 센다
	while ((m_rowFirstEntryIdx < m_numEntries) && (m_entries[m_rowFirstEntryIdx].row == m_nextRow))
	{
		++result.numEntries;
		++m_rowFirstEntryIdx;
	}

	// 현재 탐색한 벡터가 마지막 행 벡터면 다음 행 벡터는 첫 번째 행 벡터
	if (m_nextRow == m_numRows - 1)
	{
		m_nextRow = 0;
		m_rowFirstEntryIdx = 0;
	}
	else
	{
		++m_nextRow;
	}

	return result;
}

Matrix Matrix::operator+(const Matrix& param) const
{
	Matrix result(m_numRows, m_numCols);

	// this 행렬과 param 행렬의 행, 열 크기가 같아야 한다
	assert((m_numRows == param.m_numRows) && (m_numCols == param.m_numCols));

	int thisEntryIdx = 0;
	int paramEntryIdx = 0;

	// this 행렬과 param 행렬 둘 중 하나라도 탐색 완료되면 반복 종료
	while ((thisEntryIdx < m_numEntries) && (paramEntryIdx < param.m_numEntries))
	{
		// thisEntryIdx의 Entry orderKey가 paramEntryIdx의 Entry보다 작은 경우
		// result에 thisEntryIdx의 Entry 추가
		if (GetOrderKey(m_entries[thisEntryIdx]) < GetOrderKey(param.m_entries[paramEntryIdx]))
		{
			result.AppendEntry(m_entries[thisEntryIdx]);
			++thisEntryIdx;
		}
		// paramEntryIdx의 Entry orderKey가 thisEntryIdx의 Entry보다 작은 경우
		// result에 paramEntryIdx의 Entry 추가
		else if (GetOrderKey(m_entries[thisEntryIdx]) > GetOrderKey(param.m_entries[paramEntryIdx]))
		{
			result.AppendEntry(param.m_entries[paramEntryIdx]);
			++paramEntryIdx;
		}
		// paramEntryIdx의 Entry orderKey가 thisEntryIdx의 Entry와 같은 경우
		// result에 paramEntryIdx와 thisEntryIdx의 Entry 합을 추가
		else
		{
			Entry entry = m_entries[thisEntryIdx];
			entry.val += param.m_entries[paramEntryIdx].val;

			result.AppendEntry(entry);
			++thisEntryIdx;
			++paramEntryIdx;
		}
	}

	// 탐색하지 못 한 this 행렬의 나머지 Entry를 result에 추가
	while (thisEntryIdx < m_numEntries)
	{
		result.AppendEntry(m_entries[thisEntryIdx]);
		++thisEntryIdx;
	}

	// 탐색하지 못 한 param 행렬의 나머지 Entry를 result에 추가
	while (paramEntryIdx < param.m_numEntries)
	{
		result.AppendEntry(param.m_entries[paramEntryIdx]);
		++paramEntryIdx;
	}

	// 모든 행렬의 탐색을 완료해야 한다
	assert((thisEntryIdx == m_numEntries) && (paramEntryIdx == param.m_numEntries));

	return result;
}

Matrix Matrix::operator*(const Matrix& param)
{
	// this 행렬의 열 개수와 곱하려는 param 행렬의 행 개수가 같아야 한다
	assert(m_numCols == param.m_numRows);

	// 곱한 결과는 행의 개수가 this 행렬과 같고 열의 개수는 param 행렬과 같다 
	Matrix result(m_numRows, param.m_numCols);

	// param의 열 벡터를 쉽게 구하기 위해 전치행렬을 구한다
	Matrix transposeParam = param.GetTranspose();

	for (int row = 0; row < result.m_numRows; ++row)
	{
		// 반복마다 this 행렬의 행 벡터를 순서대로 구한다
		Vector thisRowVec = GetNextRowVector();

		// 행 벡터의 모든 성분이 0일 경우 내적은 0이므로 
		// 해당 행 성분은 계산하지 않는다
		if (thisRowVec.numEntries > 0)
		{
			for (int col = 0; col < result.m_numCols; ++col)
			{
				// col에 해당하는 param의 열 벡터를 구하기 위해
				// param 전치행렬의 행 벡터를 구한다
				Vector paramColVec = transposeParam.GetNextRowVector();

				// 열 벡터의 모든 성분이 0이면 내적은 0이므로 
				// 해당 열의 성분은 계산하지 않는다
				if (paramColVec.numEntries > 0)
				{
					// 행 벡터와 열 벡터의 내적을 구한 후 result의 Entry 배열에 추가
					Entry entry = { row, col, Vector::GetDotProduct(thisRowVec, paramColVec) };
					result.AppendEntry(entry);
				}
			}
		}
	}

	return result;
}

void Matrix::DoubleCapacity()
{
	assert(m_entries != nullptr);

	// 현재 Entry 배열 용량보다 2배 크기의 동적 배열 할당 및 초기화
	Entry* newEntries = new Entry[m_capacity * 2];
	::memset(newEntries, 0x00, sizeof(Entry) * m_capacity * 2);
	
	// 새로 할당한 Entry 배열에 기존 원소 복사 후 이전 배열 할당 해제
	::memcpy(newEntries, m_entries, sizeof(Entry) * m_numEntries);
	delete[] m_entries;

	// 새로운 배열과 용량으로 값 교체
	m_entries = newEntries;
	m_capacity *= 2;

	// Entry 배열 용량에 여유가 생겨야 한다
	assert(m_numEntries < m_capacity);

	return;
}

int Matrix::GetOrderKey(int row, int col) const
{
	return row * m_numCols + col;
}

int Matrix::GetOrderKey(Entry entry) const
{
	return entry.row * m_numCols + entry.col;
}

int Matrix::GetEntryIndex(int orderKey) const
{
	// Entry 배열 원소가 없는 경우
	if (m_numEntries == 0)
	{
		return -1;
	}

	// Entry 배열 원소가 한 개만 있는 경우
	if (m_numEntries == 1)
	{
		return (GetOrderKey(m_entries[0]) == orderKey) ? 0 : -1;
	}

	assert(m_numEntries > 1);

	int leftIdx = 0;
	int rightIdx = m_numEntries - 1;

	// leftIdx가 rightIdx를 넘어가면 찾으려는 Entry가 없는 것이다
	while (leftIdx <= rightIdx)
	{
		int midIdx = (leftIdx + rightIdx) / 2;

		// orderKey가 가운데 원소의 orderKey보다 작은 경우
		if (orderKey < GetOrderKey(m_entries[midIdx]))
		{
			rightIdx = midIdx - 1;
		}
		// orderKey가 가운데 원소의 orderKey보다 큰 경우
		else if (GetOrderKey(m_entries[midIdx]) < orderKey)
		{
			leftIdx = midIdx + 1;
		}
		// orderKey가 가운데 원소의 orderKey와 같은 경우
		else
		{
			return midIdx;
		}
	}

	assert(leftIdx > rightIdx);

	return -1;
}

int Matrix::GetRowEntryIdx(int row) const
{
	// Entry 배열 원소가 없는 경우
	if (m_numEntries == 0)
	{
		return -1;
	}

	// Entry 배열 원소가 한 개만 있는 경우
	if (m_numEntries == 1)
	{
		return (m_entries[0].row == row) ? 0 : -1;
	}

	assert(m_numEntries > 1);

	int leftIdx = 0;
	int rightIdx = m_numEntries - 1;

	// leftIdx가 rightIdx를 넘어가면 row에 해당하는 Entry가 없는 것이다
	while (leftIdx <= rightIdx)
	{
		int midIdx = (leftIdx + rightIdx) / 2;

		// midIdx에 해당하는 Entry의 row보다 찾으려는 row가 큰 경우
		if (m_entries[midIdx].row < row)
		{
			leftIdx = midIdx + 1;
		}
		// midIdx에 해당하는 Entry의 row보다 찾으려는 row가 작은 경우
		else if (row < m_entries[midIdx].row)
		{
			rightIdx = midIdx - 1;
		}
		// midIdx에 해당하는 Entry의 row와 찾으려는 row가 같은 경우
		else
		{
			int firstIdx = midIdx;

			// firstIdx는 row에 해당하는 Entry 중 첫 번째 Entry의 인덱스가 저장된다
			while ((firstIdx > 0) && (m_entries[firstIdx - 1].row == row))
			{
				--firstIdx;
			}

			assert((-1 < firstIdx) && (firstIdx < m_numEntries) && 
				   (m_entries[firstIdx].row == row));

			return firstIdx;
		}
	}

	return -1;
}

void Matrix::SortLastEntry()
{
	// Entry 배열의 원소가 2개 이상일 때만 호출해야 한다
	assert(m_numEntries > 1);

	int insertionIdx = m_numEntries - 1;
	Entry lastEntry = m_entries[insertionIdx];

	// 마지막 Entry의 삽입할 위치를 탐색하면서 삽입 위치 뒤에 있는 원소들을 뒤로 이동
	// 삽입 후 orderKey 기준 오름차순으로 정렬돼야 한다
	while ((insertionIdx > 0) && 
		   (GetOrderKey(m_entries[insertionIdx - 1]) > GetOrderKey(lastEntry)))
	{
		m_entries[insertionIdx] = m_entries[insertionIdx - 1];
		--insertionIdx;
	}

	m_entries[insertionIdx] = lastEntry;

	// 삽입 위치가 마지막 인덱스인 경우
	if (insertionIdx == m_numEntries - 1)
	{
		assert(GetOrderKey(m_entries[m_numEntries - 2]) < GetOrderKey(m_entries[insertionIdx]));
	}
	// 0번 인덱스에 삽입한 경우
	else if (insertionIdx == 0)
	{
		assert(GetOrderKey(m_entries[insertionIdx]) < GetOrderKey(m_entries[1]));
	}
	else
	{
		assert((GetOrderKey(m_entries[insertionIdx - 1]) < GetOrderKey(lastEntry)) &&
			(GetOrderKey(lastEntry) < GetOrderKey(m_entries[insertionIdx + 1])));
	}

	return;
}

std::ostream& operator<<(std::ostream& os, const Vector& vec)
{
	int entryIdx = 0;

	// vec의 모든 열 탐색
	for (int col = 0; col < vec.numCols; ++col)
	{
		// 열에 대응되는 Entry 배열 원소가 존재하면 해당 성분 값을 출력 
		if ((entryIdx < vec.numEntries) && (vec.entries[entryIdx].col == col))
		{
			std::cout << vec.entries[entryIdx].val << " ";
			++entryIdx;
		}
		else
		{
			std::cout << "0 ";
		}
	}

	return os;
}

std::ostream& operator<<(std::ostream& os, const Matrix& mat)
{
	int entryIdx = 0;

	// mat의 모든 행과 열 탐색
	for (int row = 0; row < mat.m_numRows; ++row)
	{
		for (int col = 0; col < mat.m_numCols; ++col)
		{
			// 행과 열에 대응되는 Entry 배열 원소가 존재하면 해당 성분 값을 출력 
			if ((entryIdx < mat.m_numEntries) && 
				(mat.GetOrderKey(row, col) == mat.GetOrderKey(mat.m_entries[entryIdx])))
			{
				std::cout << mat.m_entries[entryIdx].val << " ";
				++entryIdx;
			}
			else
			{
				std::cout << "0 ";
			}
		}

		std::cout << std::endl;
	}

	return os;
}

int Vector::GetDotProduct(Vector vec1, Vector vec2)
{
	// 두 벡터의 열 개수가 같아야 한다
	assert(vec1.numCols == vec2.numCols);

	// 두 벡터 중 하나라도 모든 성분이 0이면 내적은 0
	if ((vec1.numEntries == 0) || (vec2.numEntries == 0))
	{
		return 0;
	}

	int result = 0;
	int vec1EntryIdx = 0;
	int vec2EntryIdx = 0;

	// 두 벡터 중 하나라도 Entry 탐색이 끝나면 반복 종료
	while ((vec1EntryIdx < vec1.numEntries) && (vec2EntryIdx < vec2.numEntries))
	{
		// vec1EntryIdx에 해당하는 Entry의 열이 더 작으면 다음 Entry로 탐색 위치 이동
		if (vec1.entries[vec1EntryIdx].col < vec2.entries[vec2EntryIdx].col)
		{
			++vec1EntryIdx;
		}
		// vec2EntryIdx에 해당하는 Entry의 열이 더 작으면 다음 Entry로 탐색 위치 이동
		else if (vec1.entries[vec1EntryIdx].col > vec2.entries[vec2EntryIdx].col)
		{
			++vec2EntryIdx;
		}
		// 같은 열의 성분 곱을 result에 더한다
		else
		{
			result += vec1.entries[vec1EntryIdx].val * vec2.entries[vec2EntryIdx].val;
			++vec1EntryIdx;
			++vec2EntryIdx;
		}
	}

	return result;
}
