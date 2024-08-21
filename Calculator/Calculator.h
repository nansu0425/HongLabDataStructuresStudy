#pragma once

#include "Stack.h"
#include "Queue.h"

class Calculator
{
public:
	void SetExpression(const char* expr);
	void SetPostfixExpression();
	double EvaluateExpression();

	void PrintInfixExpression();
	void PrintPostfixExpression();

private:
	int GetOperatorPriority(char op);
	bool IsOperator(char c) const;
	bool IsParenthesis(char c) const;
	bool IsNumber(char c) const;

private:
	Queue<const char*>		m_infixExpr;
	Queue<const char*>		m_postfixExpr;
	Stack<const char*>		m_operators;
	Stack<double>			m_operands;
	bool					m_isSetExpr = false;
};
