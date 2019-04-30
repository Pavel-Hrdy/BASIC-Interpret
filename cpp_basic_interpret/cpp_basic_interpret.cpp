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
//- Sémantické akce zbytku funkcí + matematické funkce (ty by měly bejt ez) - DONE
//- Mapování řádků mezi ICVM a kódem - DONE
//- Samotné natažení kódu ze souboru - to by mělo bejt ez
//- Chybová hlášení
//- Funkce v expressionu - udělá se funkce, která podle jména vrátí příslušnou instanci funkce

int main()
{
	std::map<int, int> bla;


	std::string code = "10 X = SGN(5)";
	std::vector<TType> types;
	Lexer l(code);
	Parser p(l);
	p.Parse();
	ICVM* icvm = ICVM::GetInstance();
	icvm->ExecuteAll();
}