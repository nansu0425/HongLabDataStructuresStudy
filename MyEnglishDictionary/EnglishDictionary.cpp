#include "EnglishDictionary.h"

#include <fstream>
#include <utility>
#include <iostream>

void EnglishDictionary::setWordSearchTree(const char* pFileName)
{
	std::ifstream file(pFileName);

	if (file.is_open() == false)
	{
		return;
	}

	std::cout << "Making word search tree..." << std::endl;

	while (file.good() == true)
	{
		String word;
		String meaning;

		word.getLine(file);
		meaning.getLine(file);

		std::cout << "Word: " << word << std::endl;
		std::cout << "Meaning: " << meaning << std::endl;

		m_wordSearchTree.insert(Pair<String, String>{ std::move(word), std::move(meaning) });
	}

	std::cout << "Completed!" << std::endl;

	file.close();
}
