#pragma once

#include <fstream>

const int g_bufSize = 1024;

class String
{
public:
	String()
	{
		m_pBuf = new char[g_bufSize];
	}

	~String()
	{
		if (m_pBuf != nullptr)
		{
			delete m_pBuf;
		}
	}


private:
	char*		m_pBuf = nullptr;
};
