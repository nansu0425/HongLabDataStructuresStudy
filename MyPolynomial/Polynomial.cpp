#include <iostream>
#include <cstring>
#include <cassert>
#include "Polynomial.h"

Polynomial::Polynomial()
{
	m_terms = new Term[m_capacity];
	std::memset(m_terms, 0x00, sizeof(Term) * m_capacity);
}

Polynomial::~Polynomial()
{
	if (m_terms)
	{
		delete[] m_terms;
	}

	m_terms = nullptr;
}

void Polynomial::AddTerm(int coefficient, int exponent)
{
	if (coefficient == 0)
	{
		return;
	}

	if (m_capacity == m_numTerms)
	{
		DoubleCapacity();
	}

	assert(m_numTerms < m_capacity);

	m_terms[m_numTerms].coef = coefficient;
	m_terms[m_numTerms].exp = exponent;
	++m_numTerms;

	if ((m_numTerms > 1) && (m_terms[m_numTerms - 2].exp < m_terms[m_numTerms - 1].exp))
	{
		SortLastTerm();
	}

	return;
}

Polynomial Polynomial::AddPolynomial(const Polynomial& param) const
{
	Polynomial result;

	int thisIdx = 0;
	int paramIdx = 0;

	while ((thisIdx < this->m_numTerms) && (paramIdx < param.m_numTerms))
	{
		if (this->m_terms[thisIdx].exp < param.m_terms[paramIdx].exp)
		{
			result.AddTerm(param.m_terms[paramIdx].coef, param.m_terms[paramIdx].exp);
			++paramIdx;
		}
		else if (this->m_terms[thisIdx].exp > param.m_terms[paramIdx].exp)
		{
			result.AddTerm(this->m_terms[thisIdx].coef, this->m_terms[thisIdx].exp);
			++thisIdx;
		}
		else
		{
			result.AddTerm(this->m_terms[thisIdx].coef + param.m_terms[paramIdx].coef, this->m_terms[thisIdx].exp);
			++paramIdx;
			++thisIdx;
		}
	}

	while (thisIdx < this->m_numTerms)
	{
		result.AddTerm(this->m_terms[thisIdx].coef, this->m_terms[thisIdx].exp);
		++thisIdx;
	}

	while (paramIdx < param.m_numTerms)
	{
		result.AddTerm(param.m_terms[paramIdx].coef, param.m_terms[paramIdx].exp);
		++paramIdx;
	}

	assert((thisIdx == this->m_numTerms) && (paramIdx == param.m_numTerms));

	return result;
}

void Polynomial::DoubleCapacity()
{
	Term* newTerms = new Term[m_capacity * 2];
	std::memset(newTerms, 0x00, sizeof(Term) * m_numTerms);
	std::memcpy(newTerms, m_terms, sizeof(Term) * m_numTerms);
	delete[] m_terms;

	m_terms = newTerms;
	m_capacity *= 2;

	return;
}

void Polynomial::SortLastTerm()
{
	int insertionIdx = m_numTerms - 1;
	Term insertionTerm = m_terms[insertionIdx];

	while ((insertionIdx > 0) && (m_terms[insertionIdx - 1].exp < insertionTerm.exp))
	{
		m_terms[insertionIdx] = m_terms[insertionIdx - 1];
		--insertionIdx;
	}

	m_terms[insertionIdx] = insertionTerm;

	return;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& poly)
{
	os << "Capacity: " << poly.m_capacity << std::endl;
	os << "numTerms: " << poly.m_numTerms << std::endl;

	if (poly.m_numTerms == 0)
	{
		return os;
	}

	for (int termIdx = 0; termIdx < poly.m_numTerms; ++termIdx)
	{
		if (poly.m_terms[termIdx].coef > 0)
		{
			if (termIdx > 0)
			{
				os << " + ";
			}

			if (poly.m_terms[termIdx].coef != 1)
			{
				os << poly.m_terms[termIdx].coef;
			}
		}
		else
		{
			os << " - ";

			if (poly.m_terms[termIdx].coef != -1)
			{
				os << -poly.m_terms[termIdx].coef;
			}
		}

		if (poly.m_terms[termIdx].exp != 0)
		{
			os << "x";

			if (poly.m_terms[termIdx].exp != 1)
			{
				os << "^" << poly.m_terms[termIdx].exp;
			}
		}
	}

	return os;
}
