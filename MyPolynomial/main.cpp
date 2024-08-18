#include <iostream>
#include "Polynomial.h"

int main()
{
	Polynomial poly1;
	poly1.AddTerm(5, 2);
	poly1.AddTerm(-3, 5);
	poly1.AddTerm(6, 1);
	poly1.AddTerm(8, 0);
	poly1.AddTerm(1, 100);
	poly1.AddTerm(10, 1000);
	std::cout << poly1 << std::endl;

	Polynomial poly2;
	poly2.AddTerm(-6, 3);
	poly2.AddTerm(13, 7);
	poly2.AddTerm(-8, 1);
	poly2.AddTerm(6, 0);
	poly2.AddTerm(10, 32);
	std::cout << poly2 << std::endl;

	std::cout << poly1.AddPolynomial(poly2) << std::endl;

	return 0;
}
