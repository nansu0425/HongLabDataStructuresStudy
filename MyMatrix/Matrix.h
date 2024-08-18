#pragma once

struct Entry
{
	int row;
	int col;
	int val;
};

struct Vector
{
	Entry* entries = nullptr;
	int numEntries = 0;
	int numCols = 0;

	static int GetDotProduct(Vector vec1, Vector vec2);
};

std::ostream& operator<<(std::ostream& os, const Vector& vec);

class Matrix
{
public:
	Matrix(int numRows, int numCols);
	~Matrix();

	void	SetEntry(Entry entry);
	void	AppendEntry(Entry entry);
	Matrix	GetTranspose() const;
	Vector	GetRowVector(int row) const;

	Matrix					operator+(const Matrix& param) const;
	Matrix					operator*(const Matrix& param);
	friend std::ostream&	operator<<(std::ostream& os, const Matrix& mat);

private:
	void	DoubleCapacity();
	int		GetOrderKey(int row, int col) const;
	int		GetOrderKey(Entry entry) const;
	int		GetEntryIndex(int orderKey) const;
	int		GetRowEntryIdx(int row) const;
	void	SortLastEntry();
	Vector	GetNextRowVector();

private:
	Entry*	m_entries = nullptr;
	int		m_initCapacity = 4;
	int		m_capacity = m_initCapacity;
	int		m_numRows = 0;
	int		m_numCols = 0;
	int		m_numEntries = 0;
	int		m_rowFirstEntryIdx = 0;
	int		m_nextRow = 0;
};
