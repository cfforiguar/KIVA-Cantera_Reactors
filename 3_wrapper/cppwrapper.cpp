#include <iostream>

extern "C"
void wrapper_c_(float *ins, float *outs)
{
	float test;
	test=*ins+*ins;
	//outs=test;
	outs=&test;
	std::cout << ins << std::endl;
	std::cout << test << std::endl;
	std::cout << "C++ wrapper" << std::endl;
}
