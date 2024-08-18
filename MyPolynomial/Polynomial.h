#pragma once

struct Term
{
	int coef;
	int exp;
};

class Polynomial
{
public:
	Polynomial();
	~Polynomial();

	void AddTerm(int coefficient, int exponent);
	Polynomial AddPolynomial(const Polynomial& param) const;
	
	void DoubleCapacity();

	friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);

private:
	void SortLastTerm();

private:
	Term* m_terms = nullptr;
	int m_initCapacity = 4;
	int m_capacity = m_initCapacity;
	int m_numTerms = 0;
};
