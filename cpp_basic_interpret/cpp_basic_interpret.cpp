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
	std::ifstream file;
	
	file.open(argv[1]);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			wholeFile += line + '\n';
		}
		file.close();
	}
	else { std::cout << "Unable to open file " << argv[1]; return -1; }
	
	try {
		Lexer l(wholeFile);
		Parser p(l);
		p.Parse();
		ICVM* icvm = ICVM::GetInstance();
		icvm->ExecuteAll();
	}
	catch(VariableNotFoundException e){
		std::cout << "[Error] Line " + std::to_string(e.lineNumber) + ": Variable " +e.name + " wasn't found";
	}
	catch (NewlineInStringException e) {
		std::cout << "[Error] Line " + std::to_string(e.lineNumber) + ": String cannot contain new line";
	}
	catch (UnknownCharacterException e) {
		std::cout << "[Error] Line " + std::to_string(e.lineNumber) + ": Unknown character";
	}
	catch (StringNotTerminatedException e) {
		std::cout << "[Error] Line " + std::to_string(e.lineNumber) + ": String is not terminated";
	}
	catch (UnknownTypeOfConstantException e) {
		std::cout << "[Error] Line " + std::to_string(e.lineNumber) + ": Unknown type of constant";
	}
	catch (TypeMismatchException e) {
		std::cout << "[Error] Line " + std::to_string(e.lineNumber) + ": Expected different type";
	}
	catch (EmptyStackException e) {
		std::cout << "[Error] Line " + std::to_string(e.lineNumber) + ": Empty stack";
	}
	catch (DivideByZeroException e) {
		std::cout << "[Error] Line " + std::to_string(e.lineNumber) + ": Division by zero";
	}
	catch (CodeToInstructionTranslationException e) {
		std::cout << "[Error] Line " + std::to_string(e.lineNumber) + ": Code line number was not found";
	}
	catch (WrongLineNumberException e) {
		std::cout << "[Error] Line " + std::to_string(e.lineNumber) + ": Wrong line number";
	}
	catch (LineNumberNotFoundException e) {
		std::cout << "[Error] Line " + std::to_string(e.lineNumber) + ": Line number is missing";
	}
	catch (InvalidSyntaxException e) {
		std::cout << "[Error] Line " + std::to_string(e.lineNumber) + ": Invalid syntax";
	}
	catch (WrongInputException e) {
		std::cout << "[Error] Input can be only int, real or string";
	}


}