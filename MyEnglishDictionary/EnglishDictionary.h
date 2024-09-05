#pragma once

#include "String.h"
#include "BinarySearchTree.h"

class EnglishDictionary
{
public:
	void	setWordSearchTree(const char* pFileName);
	String	search(const String& word);

private:
	BinarySearchTree<String, String>	m_wordSearchTree;
};
