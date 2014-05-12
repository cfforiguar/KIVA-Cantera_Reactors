#include <iostream>

extern "C"
void wrapper_c_(float *ins, float *outs)
{
	std::cout << "C++ wrapper" << std::endl;
}
