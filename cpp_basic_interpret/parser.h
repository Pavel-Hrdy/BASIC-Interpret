/*
Parser header file
Pavel Hrdý
*/


#ifndef  PARSER_H
#define PARSER_H

#include "lexer.h"
#include "icvm.h"

enum class ExprTokenType{Variable,ArrayVariable,StringVariable,Int,Real,String,RelOp,MulDivOp,AddSubOp,UnaryMinusOp,ExpOp,
						LeftPar,RightPar,Error, AndOp, OrOp, NotOp, EndList,EndArray, ArrayComma, Function};

class ExprToken {
private:
	ExprTokenType _type = ExprTokenType::Error;
	std::string _content = "";
public:
	ExprToken(ExprTokenType type, const std::string & content)
	{
		_type = type;
		_content = content;
	}

	ExprTokenType GetType() const { return _type; }
	std::string GetContent()const { return _content; }
};

class Parser {
private:
	Lexer lexer;
	Token CurrentToken;

	void Eat(TType type);
	bool Parse_Lines();
	bool Parse_Statements();
	bool Parse_Statement();


	bool Parse_ID();
	bool Parse_ID_NameOnStack();
	bool Parse_IntegerList();
	bool Parse_Expression(bool isArray);
	void AddPostfixToICVM(std::vector<ExprToken>& postfix, size_t startIndex, size_t & endIndex);
	bool Parse_InfixExpression(std::vector<ExprToken>& exprTokens, bool isArrayIndex);
	bool Parse_ConstantList();
	bool Parse_IDList();
	bool Parse_IDList_NamesOnStack();
	bool Parse_PrintList();
	bool Parse_Remark();
	bool Parse_ExpressionList(bool isArray);
	bool Parse_Constant();
	bool Parse_Value();

	TType CurrentTokenType();
public:
	Parser(Lexer & l) {
		lexer = l;
		CurrentToken = lexer.GetToken();
	}
	void Parse();
};

#endif // ! PARSER_H
