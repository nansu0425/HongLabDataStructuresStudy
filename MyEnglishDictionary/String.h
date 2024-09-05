#pragma once

#include <iostream>

const int g_bufSize = 1024;

class String
{
public:
	String();
	String(const String& other);
	String(String&& other) noexcept;
	String(const char* pStr);
	~String();

	String&		operator=(const String& rhs);
	String&		operator=(String&& rhs) noexcept;
	bool		operator<(const String& rhs) const;

	const char*		getPtrBuf() const { return m_pBuf; }
	char*			getPtrBuf() { return m_pBuf; }
	std::istream&	getLine(std::istream& is);

private:
	void	swap(String& other) noexcept;
	int		getStrLen(const char* pStr);

private:
	char*	m_pBuf = nullptr;
	int		m_strLen = -1;
};

std::ostream& operator<<(std::ostream& os, const String& str);
