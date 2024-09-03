#include "String.h"

#include <utility>
#include <algorithm>

String::String()
	: m_pBuf(new char[g_bufSize])
	, m_strLen(0)
{
	std::fill(m_pBuf, m_pBuf + g_bufSize, '\0');
}

String::String(const String& other)
	: m_pBuf(new char[g_bufSize])
	, m_strLen(other.m_strLen)
{
	std::copy(other.m_pBuf, other.m_pBuf + g_bufSize, m_pBuf);
}

String::String(String&& other) noexcept
{
	swap(other);
}

String::String(const char* pStr) : String()
{
	int strLen = getStrLen(pStr);

	std::copy(pStr, pStr + strLen, m_pBuf);
	m_strLen = strLen;
}

String::~String()
{
	if (m_pBuf != nullptr)
	{
		delete[] m_pBuf;
		m_pBuf = nullptr;
	}

	m_strLen = -1;
}

String& String::operator=(const String& rhs)
{
	String rhsCopy(rhs);
	swap(rhsCopy);

	return *this;
}

String& String::operator=(String&& rhs) noexcept
{
	swap(rhs);

	return *this;
}

bool String::operator<(const String& rhs)
{
	int strIdx = 0;

	while ((m_pBuf[strIdx] != '\0') && (m_pBuf[strIdx] == rhs.m_pBuf[strIdx]))
	{
		++strIdx;
	}

	if (m_pBuf[strIdx] < rhs.m_pBuf[strIdx])
	{
		return true;
	}
	else
	{
		return false;
	}
}

void String::swap(String& other) noexcept
{
	std::swap(m_pBuf, other.m_pBuf);
	std::swap(m_strLen, other.m_strLen);
}

int String::getStrLen(const char* pStr)
{
	int strLen = 0;

	while (pStr[strLen] != '\0')
	{
		++strLen;
	}

	return strLen;
}
