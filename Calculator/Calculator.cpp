#include <iostream>
#include <cstring>
#include <cstdlib>

#include "Calculator.h"

void Calculator::SetExpression(const char* expr)
{
	int exprIdx = 0;

	while (expr[exprIdx] != '\0')
	{
		// exprIdx의 문자가 숫자인 경우
		if (IsNumber(expr[exprIdx]))
		{
			int numStartIdx = exprIdx;
			++exprIdx;

			while (IsNumber(expr[exprIdx]) || (expr[exprIdx] == '.'))
			{
				++exprIdx;
			}
			
			int numLen = exprIdx - numStartIdx;
			char* numStr = new char[numLen + 1]; // 널 문자를 위해 1을 더한다
			
			::memcpy(numStr, &expr[numStartIdx], numLen);
			numStr[numLen] = '\0';

			m_infixExpr.Enqueue(numStr);
			continue;
		}
		// exprIdx의 문자가 연산자거나 괄호인 경우
		else if (IsOperator(expr[exprIdx]) || IsParenthesis(expr[exprIdx]))
		{
			char* opStr = new char[2];
			opStr[0] = expr[exprIdx];
			opStr[1] = '\0';

			m_infixExpr.Enqueue(opStr);
		}

		++exprIdx;
	}

	m_isSetExpr = true;
}

double Calculator::EvaluateExpression()
{
	assert(m_isSetExpr);
	SetPostfixExpression();

	while (!m_postfixExpr.IsEmpty())
	{
		const char* token = m_postfixExpr.Dequeue();

		// 연산자인 경우 피연산자 pop한 후 계산하여 스택에 push
		if (IsOperator(*token))
		{
			double op2 = m_operands.Pop();
			double op1 = m_operands.Pop();

			if (*token == '+')
			{
				m_operands.Push(op1 + op2);
			}
			else if (*token == '-')
			{
				m_operands.Push(op1 - op2);
			}
			else if (*token == '*')
			{
				m_operands.Push(op1 * op2);
			}
			else if (*token == '/')
			{
				m_operands.Push(op1 / op2);
			}
		}
		// 피연산자면 실수로 변환하여 push
		else
		{
			m_operands.Push(::atof(token));
		}
	}

	return m_operands.Pop();
}

void Calculator::PrintInfixExpression()
{
	std::cout << m_infixExpr;
}

void Calculator::PrintPostfixExpression()
{
	std::cout << m_postfixExpr;
}

int Calculator::GetOperatorPriority(char op)
{
	if (op == '*' || op == '/')
	{
		return 1;
	}
	else if (op == '+' || op == '-')
	{
		return 0;
	}
}

void Calculator::SetPostfixExpression()
{
	assert(m_isSetExpr);

	while (!m_infixExpr.IsEmpty())
	{
		const char* token = m_infixExpr.Dequeue();

		// 연산자인 경우
		if (IsOperator(*token))
		{
			// 연산자 스택의 top에 우선순위가 같거나 더 높은 연산자가 있는 경우 pop
			while (!m_operators.IsEmpty() && !IsParenthesis(*m_operators.Peek()) &&
				   (GetOperatorPriority(*token) <= GetOperatorPriority(*m_operators.Peek())))
			{
				m_postfixExpr.Enqueue(m_operators.Pop());
			}

			m_operators.Push(token);
		}
		// 괄호인 경우
		else if (IsParenthesis(*token))
		{
			if (*token == '(')
			{
				m_operators.Push(token);
			}
			else
			{
				// 여는 괄호까지 pop한 후 후위 표기식에 연산자들을 enqueue
				while (true)
				{
					const char* op = m_operators.Pop();

					if (*op == '(')
					{
						break;
					}

					m_postfixExpr.Enqueue(op);
				}
			}
		}
		// 숫자인 경우
		else
		{
			m_postfixExpr.Enqueue(token);
		}
	}

	while (!m_operators.IsEmpty())
	{
		m_postfixExpr.Enqueue(m_operators.Pop());
	}

	m_isSetExpr = false;
}

bool Calculator::IsOperator(char c) const
{
	return (c == '+') || (c == '-') || (c == '*') || (c == '/');
}

bool Calculator::IsParenthesis(char c) const
{
	return (c == '(') || (c == ')');
}

bool Calculator::IsNumber(char c) const
{
	return ('0' <= c) && (c <= '9');
}
