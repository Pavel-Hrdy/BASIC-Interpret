/*
Tokens:
GRAMMAR:
"Case Sensitive" = False
"Start Symbol"   = <Lines>

{String Chars} = {Printable} - ["]
{WS}           = {Whitespace} - {CR} - {LF}

Whitespace     = {WS}+

<Remark>         = REM {Cokoliv až na NewLine a Colon}

<ID>            ::= StringVariable
				| Variable

<Lines>       ::= Int <Statements> NewLine <Lines>
				| Int <Statements> NewLine
				| Int <Statements>

<Statements>  ::= <Statement> ':' <Statements>
				| <Statement>

<Statement>   ::=
				| DATA <Constant List>
				| DIM ID '(' <Integer List> ')'
				| <Function> '(' <Expression> ')'
				| END
				| FOR ID '=' <Expression> TO <Expression>
				| FOR ID '=' <Expression> TO <Expression> STEP Integer
				| GOTO <Expression>
				| GOSUB <Expression>
				| POP
				| RESTORE <Expression>
				| RESTORE
				| IF <Expression> THEN <Statements>
				| INPUT <ID List>
				| LET ID '=' <Expression>
				| ID '=' <Expression>
				| NEXT ID
				| PRINT <Print list>
				| READ <ID List>
				| RETURN
				| RUN
				| STOP
				| Remark

<ID List>  ::= ID ',' <ID List>
			 | ID

<Constant List>   ::= <Constant> ',' <Constant List>
					| <Constant>

<Integer List>    ::= Integer ',' <Integer List>
					| Integer

<Expression List> ::= <Expression> ',' <Expression List>
					| <Expression>

<Print List>      ::= <Expression> ',' <Print List>
					| <Expression>
					|

<Expression>  ::= <And Exp> OR <Expression>
				| <And Exp>

<And Exp>     ::= <Not Exp> AND <And Exp>
				| <Not Exp>

<Not Exp>     ::= NOT <Compare Exp>
				| <Compare Exp>

<Compare Exp> ::= <Add Exp> '='  <Compare Exp>
				| <Add Exp> '<>' <Compare Exp>
				| <Add Exp> '>'  <Compare Exp>
				| <Add Exp> '>=' <Compare Exp>
				| <Add Exp> '<'  <Compare Exp>
				| <Add Exp> '<=' <Compare Exp>
				| <Add Exp>

<Add Exp>     ::= <Mult Exp> '+' <Add Exp>
				| <Mult Exp> '-' <Add Exp>
				| <Mult Exp>

<Mult Exp>    ::= <Negate Exp> '*' <Mult Exp>
				| <Negate Exp> '/' <Mult Exp>
				| <Negate Exp>

<Negate Exp>  ::= '-' <Power Exp>
				| <Power Exp>

<Power Exp>   ::= <Power Exp> '^' <Value>
				| <Value> <Power Exp2>

<Power Exp>	  ::= <Value> <Power Exp2>

<Power Exp2>  ::= '^' <Value> <Power Exp2>
				| 

<Value>       ::= '(' <Expression> ')'
				| ID
				| <Constant>

<Constant> ::= Int
			 | String
			 | Real


*/

#ifndef  PARSER_H
#define PARSER_H

#include "lexer.h"
#include "icvm.h"

enum class ExprTokenType{Variable,ArrayVariable,StringVariable,Int,Real,String,RelOp,MulDivOp,AddSubOp,UnaryMinusOp,ExpOp,
						LeftPar,RightPar,Error, AndOp, OrOp, NotOp, EndList,EndArray, ArrayComma};

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
	bool Parse_AndExp();
	bool Parse_NotExp();
	bool Parse_AddExp();
	bool Parse_MultExp();
	bool Parse_CompareExp();
	bool Parse_NegateExp();
	bool Parse_PowerExp();
	bool Parse_PowerExp2();
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
