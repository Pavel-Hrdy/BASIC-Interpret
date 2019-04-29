// cpp_basic_interpret.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <typeinfo>
#include <vector>
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
//- Sémantické akce zbytku funkcí + matematické funkce (ty by měly bejt ez)
//- Mapování řádků mezi ICVM a kódem
//- Samotné natažení kódu ze souboru - to by mělo bejt ez
//- Chybová hlášení

int main()
{
	std::map<int, int> bla;
	int x = bla[0];

	std::string code = "10 X=2:IF X=3 THEN X=3\n20 PRINT X";
	std::vector<TType> types;
	Lexer l(code);
	Parser p(l);
	p.Parse();
	ICVM* icvm = ICVM::GetInstance();
	icvm->ExecuteAll();
}