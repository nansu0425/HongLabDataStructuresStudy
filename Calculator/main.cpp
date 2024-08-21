#include <iostream>
#include "Calculator.h"

int main()
{
	Calculator cal;
	cal.SetExpression("- 5 +     (4 -    9.3 / 3)* 10");
	
	cal.PrintInfixExpression();
	std::cout << "= " << cal.EvaluateExpression() << std::endl;
	
	return 0;
}
