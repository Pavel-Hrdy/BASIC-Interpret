/***************************
Lexical tokens definitons
Pavel Hrdý
****************************/


#ifndef TOKEN_H
#define TOKEN_H

enum class RelType { Less, LessEq, Greater, GreaterEq, Eq, NotEq };
enum class SignAddType { Add, Sub };
enum class MulType { Mul, Div };
enum class TType{Comma,Semicolon,On,For,Gosub,Goto,If,Next,Real,ExpOp,NotOp,AndOp,OrOp,Sin_F,Str_F,Trap_F,Val_F,Int,Return_F,Rnd_F,Sgn_F,Sqr_F,Pop_F,Print_F,Rad_F,Read_F,Restore_F,Len_F,Let_F,Log_F,Dim_F,End_F,Exp_F,Input_F,Int_F,Clr_F,Cos_F,Data_F,Deg_F,Asc_F,Atn_F,Chr_F,Clog_F,MulDivOp,PlusMinusOp,RelOp,Abs_F,To,Then,Step,Return,Rem,Variable,StringVariable,EndOfCode,LeftPar,RightPar,Colon,String,};

class TokenType {
public:
	virtual TType Type()=0;
};

class Variable_T : public TokenType {
	virtual TType Type() override;
};

class StringVariable_T :public Variable_T {
	virtual TType Type() override;
};

class EndOfCode_T : public TokenType {
	virtual TType Type() override;
};

class Comma_T : public TokenType {
	virtual TType Type() override;
};

class LeftPar_T : public TokenType {
	virtual TType Type() override;
};

class RightPar_T : public TokenType {
	virtual TType Type() override;
};

class Semicolon_T : public TokenType {
	virtual TType Type() override;
};

class Colon_T : public TokenType {
	virtual TType Type() override;
};

class String_T : public TokenType {
	virtual TType Type() override;
};

class Int_T : public TokenType {
	virtual TType Type() override;
};

class Real_T : public TokenType {
	virtual TType Type() override;
};

class PlusMinusOp_T : public TokenType {
public:
	SignAddType type;
	PlusMinusOp_T(const SignAddType _type);
	virtual TType Type() override;
};

class MulDivOp_T : public TokenType {
public:
	MulType type;
	MulDivOp_T(const MulType _type);
	virtual TType Type() override;
};

class ExpOp_T : public TokenType {
	virtual TType Type() override;
};

class RelOp_T : public TokenType {
public:
	RelType type;
	RelOp_T(const RelType _type);
	virtual TType Type() override;
};

class NotOp_T : public TokenType {
	virtual TType Type() override;
};

class AndOp_T : public TokenType {
	virtual TType Type() override;
};

class OrOp_T : public TokenType {
	virtual TType Type() override;
};

class For_T : public TokenType {
	virtual TType Type() override;
};

class Gosub_T : public TokenType {
	virtual TType Type() override;
};

class Goto_T : public TokenType {
	virtual TType Type() override;
};

class If_T : public TokenType {
	virtual TType Type() override;
};

class Next_T : public TokenType {
	virtual TType Type() override;
};

class On_T : public TokenType {
	virtual TType Type() override;
};

class Rem_T : public TokenType {
	virtual TType Type() override;
};

class Return_T : public TokenType {
	virtual TType Type() override;
};

class Step_T : public TokenType {
	virtual TType Type() override;
};

class Then_T : public TokenType {
	virtual TType Type() override;
};

class To_T : public TokenType {
	virtual TType Type() override;
};

class Function_T : public TokenType {
	virtual void SemanticAction()=0;
};

class Abs_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Asc_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Atn_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Chr_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Clog_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Clr_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Cos_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Data_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Deg_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Dim_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class End_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Exp_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Input_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Int_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Len_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Let_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Log_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Pop_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Print_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Rad_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Read_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Restore_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Return_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Rnd_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Sgn_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Sin_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Sqr_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Str_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Trap_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

class Val_F_T : public Function_T {
	virtual void SemanticAction() override;
	virtual TType Type() override;
};

struct Token {
private:
	std::unique_ptr<TokenType> token;
	std::string content;
public:
	Token(std::unique_ptr<TokenType> _token, const std::string & _content);
	TokenType * GetTokenType();
	std::string GetContent();
};



#endif
