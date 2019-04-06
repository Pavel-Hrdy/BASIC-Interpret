/********************************
Token procedures implementation
Pavel Hrdý
********************************/


#include <iostream>
#include "token.h"

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

}

TType Abs_F_T::Type()
{
	return TType::Abs_F;
}

void Asc_F_T::SemanticAction()
{
}

TType Asc_F_T::Type()
{
	return TType::Asc_F;
}

void Atn_F_T::SemanticAction()
{
}

TType Atn_F_T::Type()
{
	return TType::Atn_F;
}

void Chr_F_T::SemanticAction()
{
}

TType Chr_F_T::Type()
{
	return TType::Chr_F;
}

void Clog_F_T::SemanticAction()
{
}

TType Clog_F_T::Type()
{
	return TType::Clog_F;
}

void Clr_F_T::SemanticAction()
{
}

TType Clr_F_T::Type()
{
	return TType::Clr_F;
}

void Cos_F_T::SemanticAction()
{
}

TType Cos_F_T::Type()
{
	return TType::Cos_F;
}

void Data_F_T::SemanticAction()
{
}

TType Data_F_T::Type()
{
	return TType::Data_F;
}

void Deg_F_T::SemanticAction()
{
}

TType Deg_F_T::Type()
{
	return TType::Deg_F;
}

void Dim_F_T::SemanticAction()
{
}

TType Dim_F_T::Type()
{
	return TType::Dim_F;
}

void End_F_T::SemanticAction()
{
}

TType End_F_T::Type()
{
	return TType::End_F;
}

void Exp_F_T::SemanticAction()
{
}

TType Exp_F_T::Type()
{
	return TType::Exp_F;
}

void Input_F_T::SemanticAction()
{
}

TType Input_F_T::Type()
{
	return TType::Input_F;
}

void Int_F_T::SemanticAction()
{
}

TType Int_F_T::Type()
{
	return TType::Int_F;
}

void Len_F_T::SemanticAction()
{
}

TType Len_F_T::Type()
{
	return TType::Len_F;
}

void Let_F_T::SemanticAction()
{
}

TType Let_F_T::Type()
{
	return TType::Let_F;
}

void Log_F_T::SemanticAction()
{
}

TType Log_F_T::Type()
{
	return TType::Log_F;
}

void Pop_F_T::SemanticAction()
{
}

TType Pop_F_T::Type()
{
	return TType::Pop_F;
}

void Print_F_T::SemanticAction()
{
}

TType Print_F_T::Type()
{
	return TType::Print_F;
}

void Rad_F_T::SemanticAction()
{
}

TType Rad_F_T::Type()
{
	return TType::Rad_F;
}

void Read_F_T::SemanticAction()
{
}

TType Read_F_T::Type()
{
	return TType::Read_F;
}

void Restore_F_T::SemanticAction()
{
}

TType Restore_F_T::Type()
{
	return TType::Restore_F;
}

void Return_F_T::SemanticAction()
{
}

TType Return_F_T::Type()
{
	return TType::Return_F;
}

void Rnd_F_T::SemanticAction()
{
}

TType Rnd_F_T::Type()
{
	return TType::Rnd_F;
}

void Sgn_F_T::SemanticAction()
{
}

TType Sgn_F_T::Type()
{
	return TType::Sgn_F;
}

void Sin_F_T::SemanticAction()
{
}

TType Sin_F_T::Type()
{
	return TType::Sin_F;
}

void Sqr_F_T::SemanticAction()
{
}

TType Sqr_F_T::Type()
{
	return TType::Sqr_F;
}

void Str_F_T::SemanticAction()
{
}

TType Str_F_T::Type()
{
	return TType::Str_F;
}

void Trap_F_T::SemanticAction()
{
}

TType Trap_F_T::Type()
{
	return TType::Trap_F;
}

void Val_F_T::SemanticAction()
{
}

TType Val_F_T::Type()
{
	return TType::Val_F;
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

TokenType * Token::GetTokenType() {
	return token.get();
}

std::string Token::GetContent()
{
	return content;
}


