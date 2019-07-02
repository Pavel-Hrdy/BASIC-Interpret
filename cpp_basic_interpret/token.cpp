/********************************
Token procedures implementation
Pavel Hrdý
********************************/


#include <iostream>
#include "token.h"
#include "icvm.h"

MulDivOp_T::MulDivOp_T(const MulType _type) {
	type = _type;
}

TType MulDivOp_T::Type()
{
	return TType::MulDivOp;
}

PlusMinusOp_T::PlusMinusOp_T(const SignAddType _type) {
	type = _type;
}

TType PlusMinusOp_T::Type()
{
	return TType::PlusMinusOp;
}

RelOp_T::RelOp_T(const RelType _type) {
	type = _type;
}

TType RelOp_T::Type()
{
	return TType::RelOp;
}

void Abs_F_T::SemanticAction() {
	ICVM * icvm = ICVM::GetInstance();
	Abs_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Abs_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Abs_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Asc_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Asc_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Asc_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Asc_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Atn_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Atn_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Atn_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Atn_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Chr_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Chr_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Chr_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Chr_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Clog_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Clog_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Clog_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Clog_F_T::Type() noexcept
{
	return Function_T::Type();
}


void Cos_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Cos_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Cos_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Cos_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Data_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	std::unique_ptr<Instruction> instr = std::make_unique<Data_Function>();
	icvm->AddInstruction(std::move(instr));
}

TType Data_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Dim_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Dim_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Dim_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Dim_F_T::Type() noexcept
{
	return Function_T::Type();
}

void End_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();

	LoadConstant x("\"I\"-1");
	std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(x);
	icvm->AddInstruction(std::move(instr));

	Jump jump;
	std::unique_ptr<Instruction> instr1 = std::make_unique<Jump>(jump);
	icvm->AddInstruction(std::move(instr1));
}

TType End_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Exp_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	End_Function x;
	std::unique_ptr<Instruction> instrPtr = std::make_unique<End_Function>(x);
	icvm->AddInstruction(std::move(instrPtr));
}

TType Exp_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Input_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Input_Function x;
	std::unique_ptr<Instruction> instrPtr = std::make_unique<Input_Function>(x);
	icvm->AddInstruction(std::move(instrPtr));
}


TType Input_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Int_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Int_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Int_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Int_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Len_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Len_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Len_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Len_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Let_F_T::SemanticAction()
{
}

TType Let_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Log_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Log_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Log_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Log_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Pop_F_T::SemanticAction()
{
	ICVM* icvm = ICVM::GetInstance();
	Pop_Function instr;
	std::unique_ptr<Instruction> instrPtr = std::make_unique<Pop_Function>(instr);
	icvm->AddInstruction(std::move(instrPtr));
}

TType Pop_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Print_F_T::SemanticAction()
{
	ICVM* icvm = ICVM::GetInstance();
	Print instr;
	std::unique_ptr<Instruction> instrPtr = std::make_unique<Print>(instr);
	icvm->AddInstruction(std::move(instrPtr));
}

TType Print_F_T::Type() noexcept
{
	return Function_T::Type();
}


void Read_F_T::SemanticAction()
{
	ICVM* icvm = ICVM::GetInstance();
	Read_Function instr;
	std::unique_ptr<Instruction> instrPtr = std::make_unique<Read_Function>(instr);
	icvm->AddInstruction(std::move(instrPtr));
}

TType Read_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Restore_F_T::SemanticAction()
{
	ICVM* icvm = ICVM::GetInstance();
	Restore_Function instr;
	std::unique_ptr<Instruction> instrPtr = std::make_unique<Restore_Function>(instr);
	icvm->AddInstruction(std::move(instrPtr));
}

TType Restore_F_T::Type() noexcept
{
	return Function_T::Type();
}


void Rnd_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Rnd_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Rnd_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Rnd_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Sgn_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Sgn_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Sgn_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Sgn_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Sin_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Sin_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Sin_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Sin_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Sqr_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Sqr_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Sqr_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Sqr_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Str_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Str_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Str_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Str_F_T::Type() noexcept
{
	return Function_T::Type();
}

void Val_F_T::SemanticAction()
{
	ICVM * icvm = ICVM::GetInstance();
	Val_Function x;
	std::unique_ptr<Instruction> instr = std::make_unique<Val_Function>(x);
	icvm->AddInstruction(std::move(instr));
}

TType Val_F_T::Type() noexcept
{
	return Function_T::Type();
}

TType Variable_T::Type()
{
	return TType::Variable;
}

TType StringVariable_T::Type()
{
	return TType::StringVariable;
}

TType LeftPar_T::Type()
{
	return TType::LeftPar;
}

TType RightPar_T::Type()
{
	return TType::RightPar;
}

TType Colon_T::Type()
{
	return TType::Colon;
}

TType String_T::Type()
{
	return TType::String;
}

TType EndOfCode_T::Type()
{
	return TType::EndOfCode;
}

TType Int_T::Type()
{
	return TType::Int;
}

TType Real_T::Type()
{
	return TType::Real;
}

TType ExpOp_T::Type()
{
	return TType::ExpOp;
}

TType NotOp_T::Type()
{
	return TType::NotOp;
}

TType AndOp_T::Type()
{
	return TType::AndOp;
}

TType OrOp_T::Type()
{
	return TType::OrOp;
}

TType For_T::Type()
{
	return TType::For;
}

TType Gosub_T::Type()
{
	return TType::Gosub;
}

TType Goto_T::Type()
{
	return TType::Goto;
}

TType If_T::Type()
{
	return TType::If;
}

TType Next_T::Type()
{
	return TType::Next;
}

TType On_T::Type()
{
	return TType::On;
}

TType Rem_T::Type()
{
	return TType::Rem;
}

TType Return_T::Type()
{
	return TType::Return;
}

TType NewLine_T::Type()
{
	return TType::NewLine;
}

TType Step_T::Type()
{
	return TType::Step;
}

TType Then_T::Type()
{
	return TType::Then;
}

TType To_T::Type()
{
	return TType::To;
}

TType Comma_T::Type()
{
	return TType::Comma;
}

TType Semicolon_T::Type()
{
	return TType::Semicolon;
}

Token::Token(std::unique_ptr<TokenType> _token, const std::string & _content, uint32_t _lineNumber) {
	token = std::move(_token);
	content = _content;
	lineNumber = _lineNumber;
}
/*
Token::Token(const Token& t) {
	
}
*/
TokenType * Token::GetTokenType() {
	return token.get();
}

std::string Token::GetContent()
{
	return content;
}

uint32_t Token::GetLineNumber()
{
	return lineNumber;
}

TType UnaryMinusOp_T::Type()
{
	return TType::UnaryMinusOp;
}
