/*
Tokens: Comma,Semicolon,On,For,Gosub,Goto,If,Next,Real,ExpOp,NotOp,AndOp,OrOp,Sin_F,Str_F,Trap_F,Val_F,Int,Return_F,
Rnd_F,Sgn_F,Sqr_F,Pop_F,Print_F,Rad_F,Read_F,Restore_F,Len_F,Let_F,Log_F,Dim_F,End_F,Exp_F,Input_F,Int_F,Clr_F,Cos_F,
Data_F,Deg_F,Asc_F,Atn_F,Chr_F,Clog_F,MulDivOp,PlusMinusOp,RelOp,Abs_F,To,Then,Step,Return,Rem,Variable,
StringVariable,EndOfCode,LeftPar,RightPar,Colon,String, NewLine
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
				| END
				| FOR ID '=' <Expression> TO <Expression>
				| FOR ID '=' <Expression> TO <Expression> STEP Integer
				| GOTO <Expression>
				| GOSUB <Expression>
				| IF <Expression> THEN <Statement>
				| INPUT <ID List>
				| INPUT '#' Integer ',' <ID List>
				| LET ID '=' <Expression>
				| ID '=' <Expression>
				| NEXT <ID List>
				| POKE <Value List>
				| PRINT <Print list>
				| PRINT '#' Integer ',' <Print List>
				| READ <ID List>
				| RETURN
				| RESTORE
				| RUN
				| STOP
				| Remark

<ID List>  ::= ID ',' <ID List>
			 | ID

<Value List>      ::= <Value> ',' <Value List>
					| <Value>

<Constant List>   ::= <Constant> ',' <Constant List>
					| <Constant>

<Integer List>    ::= Integer ',' <Integer List>
					| Integer

<Expression List> ::= <Expression> ',' <Expression List>
					| <Expression>

<Print List>      ::= <Expression> ';' <Print List>
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
				| <Value>

<Value>       ::= '(' <Expression> ')'
				| ID
				| ID '(' <Expression List> ')'
				| <Constant>

<Constant> ::= Int
			 | String
			 | Real


*/

#ifndef  PARSER_H
#define PARSER_H

#include "lexer.h"

class Parser {
private:
	Lexer lexer;
	Token CurrentToken;


	void Eat(TType type);
	bool Parse_Lines();
	bool Parse_Statements();
	TType CurrentTokenType();
public:
	Parser(Lexer & l) {
		lexer = l;
		CurrentToken = lexer.GetToken();
	}
	void Parse();
};

#endif // ! PARSER_H
