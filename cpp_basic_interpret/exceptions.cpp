/*******************************
	Exceptions implementation
	Pavel Hrdý
*******************************/


#include "exceptions.h"
#include <string>

const char * NewlineInStringException::what() const noexcept
{
	std::string x = "[Lexer Error] Line " + std::to_string(lineNumber) + ": String cannot contain new line";
	return x.c_str();
}

const char * UnknownCharacterException::what() const noexcept
{
	std::string x = "[Lexer Error] Line " + std::to_string(lineNumber) + ": Unknown character";
	return x.c_str();
}

const char * StringNotTerminated::what() const noexcept
{
	std::string x = "[Lexer Error] Line " + std::to_string(lineNumber) + ": String is not terminated";
	return x.c_str();
}

InterpreterException::InterpreterException(int line)
{
	lineNumber = line;
}
