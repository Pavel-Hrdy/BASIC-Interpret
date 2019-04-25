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
	std::string code = "10 PRINT (2*3)+8";
	std::vector<TType> types;
	Lexer l(code);
	Parser p(l);
	p.Parse();
	ICVM* icvm = ICVM::GetInstance();
	icvm->ExecuteAll();
}