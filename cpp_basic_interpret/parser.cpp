#include "parser.h"
#include "exceptions.h"

//Checks if type of current's token is equal to parameter 'type'. 
//If yes, then get new token.
//Otherwise throws exception.
void Parser::Eat(TType type)
{
	if (CurrentToken.GetTokenType()->Type() == type) {

	}
	else {
		throw InvalidSyntaxException(CurrentToken.GetLineNumber());
	}
}
