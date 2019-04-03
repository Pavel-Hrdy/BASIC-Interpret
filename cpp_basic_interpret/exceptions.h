#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>

class InterpreterException : public std::exception
{
	
public:
	int lineNumber;
	virtual const char* what() const noexcept = 0;
	InterpreterException(int line);
};

class NewlineInStringException : public InterpreterException {
public:
	virtual const char* what() const noexcept;
	NewlineInStringException(int line) :InterpreterException(line) {}
};

class UnknownCharacterException : public InterpreterException {
public:
	virtual const char* what() const noexcept;
	UnknownCharacterException(int line) :InterpreterException(line) {}
};

class StringNotTerminated : public InterpreterException {
public:
	virtual const char* what() const noexcept;
	StringNotTerminated(int line) :InterpreterException(line) {}
};
#endif

