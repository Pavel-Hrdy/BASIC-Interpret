/*
Entry point
Pavel Hrdý
*/

#include <iostream>
#include <fstream>
#include "lexer.h"
#include "exceptions.h"
#include "icvm.h"
#include "parser.h"

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cout << "Usage: PROGRAM_NAME <filename>\n";
		return -1;
	}
	std::string line;
	std::string wholeFile = "";
	std::ifstream file(argv[1]);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			wholeFile += line + '\n';
		}
		file.close();
	}
	else std::cout << "Unable to open file " << argv[1];
	try {
		Lexer l(wholeFile);
		Parser p(l);
		p.Parse();
		ICVM* icvm = ICVM::GetInstance();
		icvm->ExecuteAll();
	}
	catch(std::exception e){
		std::cout << e.what();
	}
}