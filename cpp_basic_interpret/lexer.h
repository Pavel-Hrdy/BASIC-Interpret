#ifndef LEXER
#define LEXER
#include "token.h"

class Lexer {
private:
	std::string text;
	uint32_t position = 0;
	uint32_t lineNumber = 10;
	bool newlineFlag = true;
	char Peek();
	char Read();
	std::string ReadIdentifier();
	std::string ReadString();
	std::string ReadNumber(bool & isRealNumber);
	std::unique_ptr<TokenType> DetermineTypeOfIdentifier(const std::string & input);
public:
	Lexer(const std::string & input);
	Lexer(){}
	Token GetToken();
	Token GetNextToken();
	uint32_t ReturnLineNumber();
};

#endif
