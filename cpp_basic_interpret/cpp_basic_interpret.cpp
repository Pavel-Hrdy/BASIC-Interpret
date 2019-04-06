// cpp_basic_interpret.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <typeinfo>
#include <vector>
#include "lexer.h"
#include "exceptions.h"

int main()
{
	int x = 1;
	if (x) {
		std::cout << true;
	}
	x--;
	if (x) {
		std::cout << true;
	}
	x = 2;
	if (x) {
		std::cout << true;
	}
}
