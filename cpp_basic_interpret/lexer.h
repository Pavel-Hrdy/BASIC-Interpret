#include "token.h"

#ifndef LEXER
#define LEXER

class Lexer {
private:
	std::string text;
	uint32_t position = 0;
	uint32_t lineNumber = 10;
	char Peek();
	char Read();
	std::string ReadIdentifier();
	std::string ReadString();
	std::string ReadNumber(bool & isRealNumber);
	std::unique_ptr<TokenType> DetermineTypeOfIdentifier(const std::string & input);
public:
	Lexer(const std::string & input);
	Token GetNextToken();
};

#endif
