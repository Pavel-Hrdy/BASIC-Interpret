/**************************
	Exception declarations
	Pavel Hrdý
**************************/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>

class ParserException : public std::exception {
public:
	int lineNumber;
	virtual const char* what() const noexcept = 0;
	ParserException(int line) { lineNumber = line; }
};

class InvalidSyntaxException : public ParserException {
public:
	virtual const char* what() const noexcept;
	InvalidSyntaxException(int line) :ParserException(line) {}
};


class ICVMException : public std::exception {
public:
	virtual const char* what() const noexcept = 0;
};

class VariableNotFoundException : public ICVMException {
public:
	virtual const char* what() const noexcept;
};

class UnknownTypeOfConstantException : public ICVMException {
public:
	virtual const char* what() const noexcept;
};

class TypeMismatchException : public ICVMException {
public:
	virtual const char* what() const noexcept;
};

class EmptyStackException : public ICVMException {
public:
	virtual const char* what() const noexcept;
};

class DivideByZeroException : public ICVMException {
public:
	virtual const char* what() const noexcept;
};

class CodeToInstructionTranslationException : public ICVMException {
public:
	virtual const char* what() const noexcept;
};

class LexerException : public std::exception
{
public:
	int lineNumber;
	virtual const char* what() const noexcept = 0;
	LexerException(int line);
};

class NewlineInStringException : public LexerException {
public:
	virtual const char* what() const noexcept;
	NewlineInStringException(int line) :LexerException(line) {}
};

class UnknownCharacterException : public LexerException {
public:
	virtual const char* what() const noexcept;
	UnknownCharacterException(int line) :LexerException(line) {}
};

class StringNotTerminatedException : public LexerException {
public:
	virtual const char* what() const noexcept;
	StringNotTerminatedException(int line) :LexerException(line) {}
};

class WrongLineNumberException : public LexerException {
public:
	virtual const char* what() const noexcept;
	WrongLineNumberException(int line) :LexerException(line) {}
};

class LineNumberNotFoundException : public LexerException {
public:
	virtual const char* what() const noexcept;
	LineNumberNotFoundException(int line) :LexerException(line) {}
};
#endif

