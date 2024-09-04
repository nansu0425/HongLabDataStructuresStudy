#pragma once

#include "String.h"
#include "BinarySearchTree.h"

class EnglishDictionary
{
public:
	void	setWordSearchTree(const char* fileName);

private:
	BinarySearchTree<String, String>	m_wordSearchTree;
};
