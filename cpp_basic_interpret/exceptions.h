/**************************
	Exception declarations
	Pavel Hrdý
**************************/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <string>

class ParserException : public std::exception {
public:
	const size_t lineNumber;
	virtual const char* what() const noexcept = 0;
	ParserException(size_t line) : lineNumber(line){}
};

class InvalidSyntaxException : public ParserException {
public:
	virtual const char* what() const noexcept;
	InvalidSyntaxException(size_t line) :ParserException(line) {}
};


class ICVMException : public std::exception {
public:
	size_t lineNumber = 0;
	virtual const char* what() const noexcept = 0;
};

class VariableNotFoundException : public ICVMException {
public:
	std::string name;
	VariableNotFoundException(size_t line, std::string _name) { lineNumber = line; name = _name; };
	virtual const char* what() const noexcept;
};

class UnknownTypeOfConstantException : public ICVMException {
public:
	virtual const char* what() const noexcept;
	UnknownTypeOfConstantException(size_t line)
	{
		lineNumber = line;
	}
};

class WrongInputException : public ICVMException {
public:
	virtual const char* what() const noexcept;
	WrongInputException(size_t line)
	{
		lineNumber = line;
	}
};

class TypeMismatchException : public ICVMException {
public:
	virtual const char* what() const noexcept;
	TypeMismatchException(size_t line)
	{
		lineNumber = line;
	}
};

class EmptyStackException : public ICVMException {
public:
	virtual const char* what() const noexcept;
	EmptyStackException(size_t line)
	{
		lineNumber = line;
	}
};

class DivideByZeroException : public ICVMException {
public:
	virtual const char* what() const noexcept;
	DivideByZeroException(size_t line)
	{
		lineNumber = line;
	}
};

class CodeToInstructionTranslationException : public ICVMException {
public:
	virtual const char* what() const noexcept;
	CodeToInstructionTranslationException(size_t line)
	{
		lineNumber = line;
	}
};

class LexerException : public std::exception
{
public:
	int lineNumber;
	virtual const char* what() const noexcept = 0;
	LexerException(size_t line);
};

class NewlineInStringException : public LexerException {
public:
	virtual const char* what() const noexcept;
	NewlineInStringException(size_t line) :LexerException(line) {}
};

class UnknownCharacterException : public LexerException {
public:
	virtual const char* what() const noexcept;
	UnknownCharacterException(size_t line) :LexerException(line) {}
};

class StringNotTerminatedException : public LexerException {
public:
	virtual const char* what() const noexcept;
	StringNotTerminatedException(size_t line) :LexerException(line) {}
};

class WrongLineNumberException : public LexerException {
public:
	virtual const char* what() const noexcept;
	WrongLineNumberException(size_t line) :LexerException(line) {}
};

class LineNumberNotFoundException : public LexerException {
public:
	virtual const char* what() const noexcept;
	LineNumberNotFoundException(size_t line) :LexerException(line) {}
};
#endif

