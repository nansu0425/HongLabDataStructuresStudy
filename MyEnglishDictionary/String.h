#pragma once

#include <fstream>
#include <algorithm>
#include <utility>

const int g_bufSize = 1024;

class String
{
public:
	String()
	{
		m_pBuf = new char[g_bufSize];
		std::fill(m_pBuf, m_pBuf + g_bufSize, 0);
	}

	String(String&& other) noexcept
		: m_pBuf(std::exchange(other.m_pBuf, nullptr))
	{ }

	~String()
	{
		if (m_pBuf != nullptr)
		{
			delete m_pBuf;
		}
	}

	char*	getPtrBuf() { return m_pBuf; }

private:
	char*		m_pBuf = nullptr;
};
