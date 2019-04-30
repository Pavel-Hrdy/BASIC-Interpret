// cpp_basic_interpret.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <typeinfo>
#include <vector>
#include <fstream>
#include <map>
#include "lexer.h"
#include "exceptions.h"
#include "icvm.h"
#include "parser.h"

//Todo 
//- Parse_Expression přijme všechny znaky, které se mohou nacházet ve výrazu - DONE
//- výraz, který určuje index v ArrayVariable je Expression - DONE
//- udělá z nich postfixovou notaci. - DONE
//- To se pak může vysypat do stacku a instrukcí ICVM a bude to šlapat - možná DONE
//- Sémantické akce zbytku funkcí + matematické funkce (ty by měly bejt ez) - DONE
//- Mapování řádků mezi ICVM a kódem - DONE
//- Samotné natažení kódu ze souboru - to by mělo bejt ez
//- Chybová hlášení
//- Funkce v expressionu - udělá se funkce, která podle jména vrátí příslušnou instanci funkce - DONE

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cout << "Usage: PROGRAM_NAME <filename>\n";
		return -1;
	}
	std::string line;
	std::string wholeFile="";
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

	Lexer l(wholeFile);
	Parser p(l);
	p.Parse();
	ICVM* icvm = ICVM::GetInstance();
	icvm->ExecuteAll();
}