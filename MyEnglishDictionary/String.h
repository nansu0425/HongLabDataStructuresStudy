#pragma once

const int g_bufSize = 1024;

class String
{
public:
	String();
	String(const String& other);
	String(String&& other) noexcept;
	~String();

	String& operator=(const String& rhs);
	String& operator=(String&& rhs) noexcept;

	char*	getPtrBuf() const { return m_pBuf; }

private:
	void	swap(String& other);

private:
	char*		m_pBuf = nullptr;
};
