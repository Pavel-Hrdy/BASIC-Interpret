// cpp_basic_interpret.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <typeinfo>
#include <vector>
#include "lexer.h"
#include "exceptions.h"
#include "icvm.h"
#include "parser.h"

//Todo 
//- znak tilda pro unární minus 
//- Parse_Expression přijme všechny znaky, které se mohou nacházet ve výrazu
//- udělá z nich postfixovou notaci. 
//- To se pak může vysypat do stacku a instrukcí ICVM a bude to šlapat
//- Sémantické akce zbytku funkcí + matematické funkce (ty by měly bejt ez)
//- Mapování řádků mezi ICVM a kódem
//- Samotné natažení kódu ze souboru - to by mělo bejt ez
//- Chybová hlášení

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