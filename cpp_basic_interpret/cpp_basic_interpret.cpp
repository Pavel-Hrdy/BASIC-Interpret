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
	
		std::string code = "10 PRINT X,Y,Z,A$\n20 100 PRINT\"THE VALUE OF X IS \";X";
		std::vector<TType> types;
		Lexer l(code);

		while (true) {
			Token t = l.GetToken();
			if (t.GetTokenType()->Type() == TType::EndOfCode) break;
			types.push_back(t.GetTokenType()->Type());
		}
}
