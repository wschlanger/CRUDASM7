// common_test.cpp
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "../common/include/ixbasic.h"
#include "../common/include/types.h"
#include <iostream>

int main(int argc, const char **argv)
{
	if(sizeof(U8) != 8 || sizeof(S8) != 8)
	{
		std::cout << "\nERROR!!! sizeof(U8) is not 8 or sizeof(S8) is not 8!" << std::endl;
		std::cout << "Please edit common/include/types.h for your platform" << std::endl;
		std::cout << "before continuing." << std::endl;
		std::cout << std::endl;
		return 1;
	}
	return 0;
}

