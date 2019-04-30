/*******************************
	Exceptions implementation
	Pavel Hrdý
*******************************/


#include "exceptions.h"
#include <string>

const char * NewlineInStringException::what() const noexcept
{
	std::string x = "[Error] Line " + std::to_string(lineNumber) + ": String cannot contain new line";
	return x.c_str();
}

const char * UnknownCharacterException::what() const noexcept
{
	std::string x = "[Error] Line " + std::to_string(lineNumber) + ": Unknown character";
	return x.c_str();
}

const char * StringNotTerminatedException::what() const noexcept
{
	std::string x = "[Error] Line " + std::to_string(lineNumber) + ": String is not terminated";
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
	std::string x = "[Error] Line " + std::to_string(lineNumber) + ": Unknown type of constant";
	return x.c_str();
}

const char * TypeMismatchException::what() const noexcept
{
	std::string x = "[Error] Line " + std::to_string(lineNumber) + ": Expected different type";
	return x.c_str();
}

const char * EmptyStackException::what() const noexcept
{
	std::string x = "[Error] Line " + std::to_string(lineNumber) + ": Empty stack";
	return x.c_str();
}

const char * DivideByZeroException::what() const noexcept
{
	std::string x = "[Error] Line " + std::to_string(lineNumber) + ": Division by zero";
	return x.c_str();
}

const char * CodeToInstructionTranslationException::what() const noexcept
{
	std::string x = "[Error] Line " + std::to_string(lineNumber) + ": Code line number was not found";
	return x.c_str();
}

const char * WrongLineNumberException::what() const noexcept
{
	std::string x = "[Error] Line " + std::to_string(lineNumber) + ": Wrong line number";
	return x.c_str();
}

const char * LineNumberNotFoundException::what() const noexcept
{
	std::string x = "[Error] Line " + std::to_string(lineNumber) + ": Line number is missing";
	return x.c_str();
}

const char * InvalidSyntaxException::what() const noexcept
{
	std::string x = "[Error] Line " + std::to_string(lineNumber) + ": Invalid syntax";
	return x.c_str();
}

const char * WrongInputException::what() const noexcept
{
	std::string x = "[Error] Input can be only int, real or string";
	return x.c_str();
}
