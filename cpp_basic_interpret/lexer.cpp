/************************************
Lexical analysis of BASIC language
Pavel Hrdý
************************************/

#include <iostream>
#include <algorithm>
#include <string>
#include "token.h"
#include "lexer.h"
#include "exceptions.h"

//Peeks in text and returns char at current position.
//If position is higher than text's length, returns NUL char
char Lexer::Peek() const {
	if (position < text.length())
		return text[position];

	else return 0;
}

//Returns char at current position, increments position.
//If position is higher than text's length, returns NUL char
char Lexer::Read() {
	if (position < text.length()) {
		char returnC = text[position];
		position++;
		return returnC;
	}
	else return 0;
}

//Reads and returns identifier
//Identifier can be: anything which starts with alphabetic char and consists of alphanumeric chars and can end with $ 
std::string Lexer::ReadIdentifier() {
	std::string buffer = "";

	buffer += Read(); //Char at current position is surely alphabetic.
	char currentChar;
	while (position < text.length()) {
		currentChar = Peek();
		if ((isalnum(currentChar) || currentChar == '$')) {
			buffer += Read();
		}
		else break;
	}

	return buffer;
}

std::string Lexer::ReadString() {
	std::string buffer = "";
	bool hasStringEnd = false;
	Read(); //Char at current position is surely '"'. '"' is not part of returned string.
	char currentChar;
	while (position < text.length()) {
		currentChar = Peek();
		if (currentChar == '\n') {
			throw NewlineInStringException(lineNumber); // New line can't be in string
		}
		else if (currentChar != '"') {
			buffer += Read();
		}
		else if (currentChar == '\\') { // If there is an escape sequence, throw '\' away and then read next char
			Read();
			buffer += Read();
		}
		else if (currentChar == '"') {
			Read(); // End of string
			hasStringEnd = true;
			break;
		}
	}
	if (!hasStringEnd) throw StringNotTerminatedException(lineNumber);
	return buffer;
}

std::string Lexer::ReadNumber(bool& isRealNumber) {
	std::string buffer = "";
	bool isReal = false;
	buffer += Read(); //Char at current position is surely a digit.
	char currentChar;
	while (position < text.length()) {
		currentChar = Peek();

		if (isdigit(currentChar)) {
			buffer += Read();
		}
		else if ((currentChar == '.') && (!isReal)) {
			buffer += Read();
			isReal = true;
		}
		else break;
	}
	isRealNumber = isReal;
	return buffer;
}

std::unique_ptr<TokenType> Lexer::DetermineTypeOfIdentifier(const std::string& input) const {
	std::unique_ptr<TokenType> returnPointer;
	std::string lowerString = input;
	std::transform(lowerString.begin(), lowerString.end(), lowerString.begin(), ::tolower);

	if (lowerString == "not") {
		NotOp_T x;
		returnPointer = std::make_unique<NotOp_T>(x);
	}
	else if (lowerString == "and") {
		AndOp_T x;
		returnPointer = std::make_unique<AndOp_T>(x);
	}
	else if (lowerString == "or") {
		OrOp_T x;
		returnPointer = std::make_unique<OrOp_T>(x);
	}
	else if (lowerString == "for") {
		For_T x;
		returnPointer = std::make_unique<For_T>(x);
	}
	else if (lowerString == "gosub") {
		Gosub_T x;
		returnPointer = std::make_unique<Gosub_T>(x);
	}
	else if (lowerString == "goto") {
		Goto_T x;
		returnPointer = std::make_unique<Goto_T>(x);
	}
	else if (lowerString == "if") {
		If_T x;
		returnPointer = std::make_unique<If_T>(x);
	}
	else if (lowerString == "next") {
		Next_T x;
		returnPointer = std::make_unique<Next_T>(x);
	}
	else if (lowerString == "on") {
		On_T x;
		returnPointer = std::make_unique<On_T>(x);
	}
	else if (lowerString == "rem") {
		Rem_T x;
		returnPointer = std::make_unique<Rem_T>(x);
	}
	else if (lowerString == "return") {
		Return_T x;
		returnPointer = std::make_unique<Return_T>(x);
	}
	else if (lowerString == "step") {
		Step_T x;
		returnPointer = std::make_unique<Step_T>(x);
	}
	else if (lowerString == "then") {
		Then_T x;
		returnPointer = std::make_unique<Then_T>(x);
	}
	else if (lowerString == "to") {
		To_T x;
		returnPointer = std::make_unique<To_T>(x);
	}
	//Functions
	else if (lowerString == "abs") {
		Abs_F_T x;
		returnPointer = std::make_unique<Abs_F_T>(x);
	}
	else if (lowerString == "asc") {
		Asc_F_T x;
		returnPointer = std::make_unique<Asc_F_T>(x);
	}
	else if (lowerString == "atn") {
		Atn_F_T x;
		returnPointer = std::make_unique<Atn_F_T>(x);
	}
	else if (lowerString == "chr$") {
		Chr_F_T x;
		returnPointer = std::make_unique<Chr_F_T>(x);
	}
	else if (lowerString == "clog") {
		Clog_F_T x;
		returnPointer = std::make_unique<Clog_F_T>(x);
	}
	else if (lowerString == "cos") {
		Cos_F_T x;
		returnPointer = std::make_unique<Cos_F_T>(x);
	}
	else if (lowerString == "data") {
		Data_F_T x;
		returnPointer = std::make_unique<Data_F_T>(x);
	}
	else if (lowerString == "dim") {
		Dim_F_T x;
		returnPointer = std::make_unique<Dim_F_T>(x);
	}
	else if (lowerString == "end") {
		End_F_T x;
		returnPointer = std::make_unique<End_F_T>(x);
	}
	else if (lowerString == "exp") {
		Exp_F_T x;
		returnPointer = std::make_unique<Exp_F_T>(x);
	}
	else if (lowerString == "input") {
		Input_F_T x;
		returnPointer = std::make_unique<Input_F_T>(x);
	}
	else if (lowerString == "int") {
		Int_F_T x;
		returnPointer = std::make_unique<Int_F_T>(x);
	}
	else if (lowerString == "len") {
		Len_F_T x;
		returnPointer = std::make_unique<Len_F_T>(x);
	}
	else if (lowerString == "let") {
		Let_F_T x;
		returnPointer = std::make_unique<Let_F_T>(x);
	}
	else if (lowerString == "log") {
		Log_F_T x;
		returnPointer = std::make_unique<Log_F_T>(x);
	}
	else if (lowerString == "pop") {
		Pop_F_T x;
		returnPointer = std::make_unique<Pop_F_T>(x);
	}
	else if (lowerString == "print") {
		Print_F_T x;
		returnPointer = std::make_unique<Print_F_T>(x);
	}
	else if (lowerString == "read") {
		Read_F_T x;
		returnPointer = std::make_unique<Read_F_T>(x);
	}
	else if (lowerString == "restore") {
		Restore_F_T x;
		returnPointer = std::make_unique<Restore_F_T>(x);
	}
	else if (lowerString == "return") {
		Return_T x;
		returnPointer = std::make_unique<Return_T>(x);
	}
	else if (lowerString == "rnd") {
		Rnd_F_T x;
		returnPointer = std::make_unique<Rnd_F_T>(x);
	}
	else if (lowerString == "sgn") {
		Sgn_F_T x;
		returnPointer = std::make_unique<Sgn_F_T>(x);
	}
	else if (lowerString == "sin") {
		Sin_F_T x;
		returnPointer = std::make_unique<Sin_F_T>(x);
	}
	else if (lowerString == "sqr") {
		Sqr_F_T x;
		returnPointer = std::make_unique<Sqr_F_T>(x);
	}
	else if (lowerString == "str$") {
		Str_F_T x;
		returnPointer = std::make_unique<Str_F_T>(x);
	}
	else if (lowerString == "val") {
		Val_F_T x;
		returnPointer = std::make_unique<Val_F_T>(x);
	}
	else if (lowerString[lowerString.length() - 1] == '$') {
		StringVariable_T x;
		returnPointer = std::make_unique<StringVariable_T>(x);
	}
	else {
		Variable_T x;
		returnPointer = std::make_unique<Variable_T>(x);
	}

	return returnPointer;
}

//Creates new lexer.
//input - whole code
Lexer::Lexer(const std::string& input) {
	text = input;
}

Token Lexer::GetToken()
{
	Token t = GetNextToken();
	if (t.GetTokenType()->Type() == TType::EndOfCode) return t;
	if (newlineFlag) {
		if ((t.GetTokenType()->Type() == TType::Int)) { //Next token has to be line number
			newlineFlag = false;
			if ((std::stoi(t.GetContent()) == (int)lineNumber)) {
				return t;
			}
			else {
				throw WrongLineNumberException(t.GetLineNumber());
			}
		}
		else if ((t.GetTokenType()->Type() == TType::NewLine)) {
			return t;
		}
		else {
			throw LineNumberNotFoundException(t.GetLineNumber());
		}
	}
	else {
		return t;
	}
}


//Gets next token from code
Token Lexer::GetNextToken() {
	std::string content = "";

	while (true) {
		char currentChar = Peek();
		//End of code
		if (currentChar == 0) {
			EndOfCode_T x;
			std::unique_ptr<TokenType> y = std::make_unique<EndOfCode_T>(x);
			return Token(std::move(y), "", lineNumber);
		}

		//Is alphanumeric -> token is identifier 
		if (isalpha(currentChar)) {
			content = ReadIdentifier();
			std::unique_ptr<TokenType> y = DetermineTypeOfIdentifier(content);
			return Token(std::move(y), content, lineNumber);
		}

		//Is a digit -> token is a number
		else if (isdigit(currentChar)) {
			bool isReal;
			content = ReadNumber(isReal);
			std::unique_ptr<TokenType> y;
			if (isReal) {
				Real_T x;
				y = std::make_unique<Real_T>(x);
			}
			else {
				Int_T x;
				y = std::make_unique<Int_T>(x);
			}

			return Token(std::move(y), content, lineNumber);
		}
		//Is a " -> token is a string
		else if (currentChar == '"') {
			content = ReadString();
			String_T x;
			std::unique_ptr<TokenType> y = std::make_unique<String_T>(x);

			return Token(std::move(y), content, lineNumber);
		}
		else if (currentChar == '(') {
			Read();
			LeftPar_T x;
			std::unique_ptr<TokenType> y = std::make_unique<LeftPar_T>(x);

			return Token(std::move(y), content, lineNumber);
		}
		else if (currentChar == ')') {
			Read();
			RightPar_T x;
			std::unique_ptr<TokenType> y = std::make_unique<RightPar_T>(x);

			return Token(std::move(y), content, lineNumber);
		}
		else if (currentChar == ':') {
			Read();
			Colon_T x;
			std::unique_ptr<TokenType> y = std::make_unique<Colon_T>(x);

			return Token(std::move(y), content, lineNumber);
		}
		else if (currentChar == '~') {
			Read();
			UnaryMinusOp_T x;
			std::unique_ptr<TokenType> y = std::make_unique<UnaryMinusOp_T>(x);

			return Token(std::move(y), content, lineNumber);
		}
		else if (currentChar == ',') {
			Read();
			Comma_T x;
			std::unique_ptr<TokenType> y = std::make_unique<Comma_T>(x);

			return Token(std::move(y), content, lineNumber);
		}
		else if (currentChar == ';') {
			Read();
			Semicolon_T x;
			std::unique_ptr<TokenType> y = std::make_unique<Semicolon_T>(x);

			return Token(std::move(y), content, lineNumber);
		}
		else if (currentChar == '+') {
			Read();
			PlusMinusOp_T x(SignAddType::Add);
			std::unique_ptr<TokenType> y = std::make_unique<PlusMinusOp_T>(x);

			return Token(std::move(y), "+", lineNumber);
		}
		else if (currentChar == '-') {
			Read();
			PlusMinusOp_T x(SignAddType::Sub);
			std::unique_ptr<TokenType> y = std::make_unique<PlusMinusOp_T>(x);

			return Token(std::move(y), "-", lineNumber);
		}
		else if (currentChar == '*') {
			Read();
			MulDivOp_T x(MulType::Mul);
			std::unique_ptr<TokenType> y = std::make_unique<MulDivOp_T>(x);

			return Token(std::move(y), "*", lineNumber);
		}
		else if (currentChar == '/') {
			Read();
			MulDivOp_T x(MulType::Div);
			std::unique_ptr<TokenType> y = std::make_unique<MulDivOp_T>(x);

			return Token(std::move(y), "/", lineNumber);
		}
		else if (currentChar == '^') {
			Read();
			ExpOp_T x;
			std::unique_ptr<TokenType> y = std::make_unique<ExpOp_T>(x);

			return Token(std::move(y), "^", lineNumber);
		}
		else if (currentChar == '<') {
			Read();
			content = "<";
			std::unique_ptr<TokenType> y;
			if (Peek() == '=') {
				Read();
				RelOp_T x(RelType::LessEq);
				y = std::make_unique<RelOp_T>(x);
				content = "<=";
			}
			else if (Peek() == '>') {
				Read();
				RelOp_T x(RelType::NotEq);
				y = std::make_unique<RelOp_T>(x);
				content = "<>";
			}
			else {
				RelOp_T x(RelType::Less);
				y = std::make_unique<RelOp_T>(x);
			}

			return Token(std::move(y), content, lineNumber);
		}
		else if (currentChar == '>') {
			Read();
			content = ">";
			std::unique_ptr<TokenType> y;
			if (Peek() == '=') {
				Read();
				RelOp_T x(RelType::GreaterEq);
				y = std::make_unique<RelOp_T>(x);
				content = ">=";
			}
			else {
				RelOp_T x(RelType::Greater);
				y = std::make_unique<RelOp_T>(x);
			}

			return Token(std::move(y), content, lineNumber);
		}
		else if (currentChar == '=') {
			Read();
			RelOp_T x(RelType::Eq);
			std::unique_ptr<TokenType> y = std::make_unique<RelOp_T>(x);

			return Token(std::move(y), "=", lineNumber);
		}
		else if (currentChar == '\n') {
			lineNumber += 10;
			newlineFlag = true;
			Read();

			NewLine_T x;
			std::unique_ptr<TokenType> y = std::make_unique<NewLine_T>(x);
			return Token(std::move(y), content, lineNumber);
		}
		else if (isspace(currentChar)) {
			Read();
		}
		else {
			throw UnknownCharacterException(lineNumber); // Unknown char
		}
	}
	//These lines are here to suppress warning
	std::unique_ptr<TokenType> x;
	return Token(std::move(x), "", lineNumber);
	//-----------------------------------------
}

uint32_t Lexer::ReturnLineNumber() const
{
	return lineNumber;
}
