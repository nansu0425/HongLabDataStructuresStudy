#include "EnglishDictionary.h"

#include <iostream>

int main()
{
	EnglishDictionary enDict;
	enDict.setWordSearchTree("eng_dic.txt");

	String word;

	while (std::cin.good() == true)
	{
		std::cout << "\nInput a word: ";
		word.getLine(std::cin);

		std::cout << "Meaning: " << enDict.search(word) << std::endl;
	}

	return 0;
}
