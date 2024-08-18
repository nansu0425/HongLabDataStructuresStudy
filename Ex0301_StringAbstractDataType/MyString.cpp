#include "MyString.h"

using namespace std;

// 비어 있는 MyString() 생성
MyString::MyString()
{
	str_ = nullptr;
	size_ = 0;
}

// 맨 뒤에 널 캐릭터'\0'가 들어 있는 문자열로부터 초기화
MyString::MyString(const char* init)
{
	// 크기(size_) 결정
	size_ = 0;

	while (init[size_] != '\0') {
		++size_;
	}

	// 메모리 할당
	str_ = new char[size_];
	::memset(str_, 0, size_);

	// 데이터 복사
	::memcpy(str_, init, size_);
}

// MyString의 다른 instance로부터 초기화
MyString::MyString(const MyString& str)
{
	// 기본 복사 생성자는 포인터 주소만 복사하기 때문에 
	// 소멸시 오류 발생
	// 여기서는 새로 메모리를 할당 받아서 복사
	size_ = str.size_;
	str_ = new char[size_];
	::memset(str_, 0, size_);

	::memcpy(str_, str.str_, size_);
}

MyString::~MyString()
{
	// 메모리 해제
	if (str_ != nullptr) {
		delete[] str_;
		str_ = nullptr;
		size_ = 0;
	}
}

bool MyString::IsEmpty()
{
	return Length() == 0;
}

bool MyString::IsEqual(const MyString& str) // 편의상 참조& 사용
{
	// 힌트: str.str_, str.size_ 가능

	// size가 다른 경우
	if (size_ != str.size_) {
		return false;
	}
	else {
		int idx = 0;

		while (idx < size_) {
			if (str_[idx] != str.str_[idx]) {
				return false;
			}

			++idx;
		}
	}

	return true;
}

int MyString::Length()
{
	return size_;
}

void MyString::Resize(int new_size)
{
	// 메모리 재할당과 원래 갖고 있던 내용 복사
	if (size_ != new_size) {
		char* newStr = new char[new_size];
		::memset(newStr, 0, new_size);
		::memcpy(newStr, str_, std::min(new_size, size_));

		delete[] str_;
		str_ = newStr;
		size_ = new_size;
	}
}

// 인덱스 start위치의 글자부터 num개의 글자로 새로운 문자열 만들기
MyString MyString::Substr(int start, int num)
{
	// 복사할 인덱스: start, start + 1, ... , start + num - 1
	assert(start + num - 1 < this->size_); // 문제를 단순하게 만들기 위해 가정

	MyString temp;

	// TODO:
	temp.str_ = new char[num];
	temp.size_ = num;

	::memcpy(temp.str_,  str_ + start, num);

	return temp;
}

MyString MyString::Concat(MyString app_str)
{
	MyString temp;

	// TODO: 
	temp.size_ = size_ + app_str.size_;
	temp.str_ = new char[temp.size_];

	::memcpy(temp.str_, str_, size_);
	::memcpy(temp.str_ + size_, app_str.str_, app_str.size_);

	return temp;
}

MyString MyString::Insert(MyString t, int start)
{
	assert(start >= 0);
	assert(start <= this->size_);

	MyString temp;

	// TODO:
	temp.size_ = size_ + t.size_;
	temp.str_ = new char[temp.size_];

	::memcpy(temp.str_, str_, start);
	::memcpy(temp.str_ + start, t.str_, t.size_);
	::memcpy(temp.str_ + start + t.size_, str_ + start, size_ - start);

	return temp;
}

int MyString::Find(MyString pat)
{
	//TODO:
	for (int startIdx = 0; startIdx <= size_ - pat.size_; ++startIdx) {
		int idx = 0;

		while (idx < pat.size_) {
			if (str_[startIdx + idx] != pat.str_[idx]) {
				break;
			}

			++idx;
		}

		if (idx == pat.size_) {
			return startIdx;
		}
	}

	return -1;
}

void MyString::Print()
{
	for (int i = 0; i < size_; i++)
		cout << str_[i];
	cout << endl;
}

