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

const char * StringNotTerminatedException::what() const noexcept
{
	std::string x = "[Lexer Error] Line " + std::to_string(lineNumber) + ": String is not terminated";
	return x.c_str();
}

LexerException::LexerException(int line)
{
	lineNumber = line;
}

const char * VariableNotFoundException::what() const noexcept
{
	std::string x = "[Error] Variable  " + name + "wasn't found";
	return x.c_str();
}

const char * UnknownTypeOfConstantException::what() const noexcept
{
	return "[ICVM Error] Unknown type of constant";
}

const char * TypeMismatchException::what() const noexcept
{
	return "[ICVM Error] Expected different type on top of the stack";
}

const char * EmptyStackException::what() const noexcept
{
	return "[ICVM Error] Stack is empty";
}

const char * DivideByZeroException::what() const noexcept
{
	return "[ICVM Error] Divide by zero";
}

const char * CodeToInstructionTranslationException::what() const noexcept
{
	return "[ICVM Error] Code line number wasn't found";
}

const char * WrongLineNumberException::what() const noexcept
{
	std::string x = "[Lexer Error] Line " + std::to_string(lineNumber) + ": Wrong line number";
	return x.c_str();
}

const char * LineNumberNotFoundException::what() const noexcept
{
	std::string x = "[Lexer Error] Line " + std::to_string(lineNumber) + ": Line number is missing";
	return x.c_str();
}

const char * InvalidSyntaxException::what() const noexcept
{
	std::string x = "[Parser Error] Line " + std::to_string(lineNumber) + ": Invalid syntax";
	return x.c_str();
}

const char * WrongInputException::what() const noexcept
{
	std::string x = "[Input Error] Input can be only int, real or string";
	return x.c_str();
}
