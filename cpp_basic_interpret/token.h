/***************************
Lexical tokens declarations
Pavel Hrdý
****************************/


#ifndef TOKEN_H
#define TOKEN_H

#include <memory>
#include <string>

enum class RelType { Less, LessEq, Greater, GreaterEq, Eq, NotEq };
enum class SignAddType { Add, Sub };
enum class MulType { Mul, Div };
enum class TType { UnaryMinusOp, Function, Comma, Semicolon, On, For, Gosub, Goto, If, Next, Real, ExpOp, NotOp, AndOp, OrOp, Int, MulDivOp, PlusMinusOp, RelOp, To, Then, Step, Return, Rem, Variable, StringVariable, EndOfCode, LeftPar, RightPar, Colon, String, NewLine };
enum class FunctionType {
	Sin, Str, Trap, Val, Return, Rnd, Sgn, Sqr, Pop, Print, Rad, Read, Restore, Len, Let, Log, Dim, End, Abs, Exp, Input, Int, Clr, Cos, Data, Deg, Asc, Atn, Chr, Clog
};

class TokenType {
public:
	virtual TType Type() = 0;
};

class Variable_T : public TokenType {
public:
	virtual TType Type() override;
};

class StringVariable_T :public Variable_T {
public:
	virtual TType Type() override;
};

class EndOfCode_T : public TokenType {
public:
	virtual TType Type() override;
};

class NewLine_T : public TokenType {
public:
	virtual TType Type() override;
};


class Comma_T : public TokenType {
public:
	virtual TType Type() override;
};

class LeftPar_T : public TokenType {
public:
	virtual TType Type() override;
};

class RightPar_T : public TokenType {
public:
	virtual TType Type() override;
};

class Semicolon_T : public TokenType {
public:
	virtual TType Type() override;
};

class Colon_T : public TokenType {
public:
	virtual TType Type() override;
};

class String_T : public TokenType {
public:
	virtual TType Type() override;
};

class Int_T : public TokenType {
public:
	virtual TType Type() override;
};

class Real_T : public TokenType {
public:
	virtual TType Type() override;
};

class PlusMinusOp_T : public TokenType {
public:
	SignAddType type;
	PlusMinusOp_T(const SignAddType _type);
	virtual TType Type() override;
};

class UnaryMinusOp_T : public TokenType {
public:
	virtual TType Type() override;
};

class MulDivOp_T : public TokenType {
public:
	MulType type;
	MulDivOp_T(const MulType _type);
	virtual TType Type() override;
};

class ExpOp_T : public TokenType {
public:
	virtual TType Type() override;
};

class RelOp_T : public TokenType {
public:
	RelType type;
	RelOp_T(const RelType _type);
	virtual TType Type() override;
};

class NotOp_T : public TokenType {
public:
	virtual TType Type() override;
};

class AndOp_T : public TokenType {
public:
	virtual TType Type() override;
};

class OrOp_T : public TokenType {
public:
	virtual TType Type() override;
};

class For_T : public TokenType {
public:
	virtual TType Type() override;
};

class Gosub_T : public TokenType {
public:
	virtual TType Type() override;
};

class Goto_T : public TokenType {
public:
	virtual TType Type() override;
};

class If_T : public TokenType {
public:
	virtual TType Type() override;
};

class Next_T : public TokenType {
public:
	virtual TType Type() override;
};

class On_T : public TokenType {
public:
	virtual TType Type() override;
};

class Rem_T : public TokenType {
public:
	virtual TType Type() override;
};

class Return_T : public TokenType {
public:
	virtual TType Type() override;
};

class Step_T : public TokenType {
public:
	virtual TType Type() override;
};

class Then_T : public TokenType {
public:
	virtual TType Type() override;
};

class To_T : public TokenType {
public:
	virtual TType Type() override;
};

class Function_T : public TokenType {
public:
	virtual void SemanticAction() = 0;
	virtual TType Type() { return TType::Function; }
	virtual FunctionType FuncType() = 0;
};

class Abs_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Abs; }
};

class Asc_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Asc; }
};

class Atn_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Atn; }
};

class Chr_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Chr; }
};

class Clog_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Clog; }
};

class Clr_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Clr; }
};

class Cos_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Cos; }
};

class Data_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Data; }
};

class Deg_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Deg; }
};

class Dim_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Dim; }
};

class End_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::End; }
};

class Exp_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Exp; }
};

class Input_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Input; }
};

class Int_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Int; }
};

class Len_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Len; }
};

class Let_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Let; }
};

class Log_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Log; }
};

class Pop_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Pop; }
};

class Print_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Print; }
};

class Rad_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Rad; }
};

class Read_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Read; }
};

class Restore_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Restore; }
};

class Rnd_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Rnd; }
};

class Sgn_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Sgn; }
};

class Sin_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Sin; }
};

class Sqr_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Sqr; }
};

class Str_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Str; }
};

class Trap_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Trap; }
};

class Val_F_T : public Function_T {
public:
	virtual void SemanticAction() override;
	virtual TType Type() override;
	virtual FunctionType FuncType() { return FunctionType::Val; }
};

struct Token {
private:
	std::shared_ptr<TokenType> token;
	std::string content;
	uint32_t lineNumber;
public:
	Token() {}
	Token(std::unique_ptr<TokenType> _token, const std::string& _content, uint32_t _lineNumber);


	TokenType* GetTokenType();
	std::string GetContent();
	uint32_t GetLineNumber();
};



#endif
