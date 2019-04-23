// cpp_basic_interpret.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <typeinfo>
#include <vector>
#include "lexer.h"
#include "exceptions.h"
#include "icvm.h"
#include "parser.h"

int main()
{
	std::string x = "1.5";
	int y = std::stoi(x);


	std::string code = "10 DIM X(2,3,4) real";
	std::vector<TType> types;
	Lexer l(code);
	Parser p(l);
	p.Parse();
	ICVM* icvm = ICVM::GetInstance();
	icvm->ExecuteInstruction();

}