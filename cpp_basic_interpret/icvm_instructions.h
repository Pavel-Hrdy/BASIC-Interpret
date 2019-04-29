#ifndef ICVM_INSTRUCTIONS_H
#define ICVM_INSTRUCTIONS_H

#include <iostream>

class Instruction {
public:
	virtual void Execute() = 0;
	std::string argument;

	Instruction(){}
	Instruction(const std::string & arg) { argument = arg; }
};

class LoadArrayVariable :public Instruction {
public:
	virtual void Execute() override;
};

class LoadToAddressStack :public Instruction {
public:
	virtual void Execute() override;
};

class PopAddressStack :public Instruction {
public:
	virtual void Execute() override;
};

class LoadArrayVariableName :public Instruction {
public:
	virtual void Execute() override;
};

class SaveToArrayVariable :public Instruction {
public:
	virtual void Execute() override;
};


class LoadVariable :public Instruction {
public:
	virtual void Execute() override;
};

class LoadConstant : public Instruction {
public:
	virtual void Execute() override;
	LoadConstant(const std::string & input) :Instruction(input){}
};

class SaveToNewVariable :public Instruction {
public:
	virtual void Execute() override;
};


class SaveToVariable :public Instruction {
public:
	virtual void Execute() override;
};
/*
class IntToReal : public Instruction {
public:
	virtual void Execute() override;
};

class RealToInt : public Instruction {
public:
	virtual void Execute() override;
};

class UnaryMinusReal : public Instruction {
public:
	virtual void Execute() override;
};

class UnaryMinusInt : public Instruction {
public:
	virtual void Execute() override;
};
*/

class UnaryMinus : public Instruction {
public:
	virtual void Execute() override;
};

class Not : public Instruction {
public:
	virtual void Execute()override;
};

class And : public Instruction {
public:
	virtual void Execute()override;
};

class Or : public Instruction {
public:
	virtual void Execute()override;
};
/*
class AddInt : public Instruction {
public:
	virtual void Execute()override;
};

class SubInt : public Instruction {
public:
	virtual void Execute()override;
};

class DivInt : public Instruction {
public:
	virtual void Execute()override;
};

class MulInt : public Instruction {
public:
	virtual void Execute()override;
};

class AddReal : public Instruction {
public:
	virtual void Execute()override;
};

class SubReal : public Instruction {
public:
	virtual void Execute()override;
};

class DivReal : public Instruction {
public:
	virtual void Execute()override;
};

class MulReal : public Instruction {
public:
	virtual void Execute()override;
};
*/

class Add : public Instruction {
public:
	virtual void Execute()override;
};

class Sub : public Instruction {
public:
	virtual void Execute()override;
};

class Div : public Instruction {
public:
	virtual void Execute()override;
};

class Mul : public Instruction {
public:
	virtual void Execute()override;
};

/*
class LessInt : public Instruction {
public:
	virtual void Execute()override;
};

class GreaterInt : public Instruction {
public:
	virtual void Execute()override;
};

class LessEqInt : public Instruction {
public:
	virtual void Execute()override;
};

class GreaterEqInt : public Instruction {
public:
	virtual void Execute()override;
};

class EqInt : public Instruction {
public:
	virtual void Execute()override;
};

class NotEqInt : public Instruction {
public:
	virtual void Execute()override;
};

class LessReal : public Instruction {
public:
	virtual void Execute()override;
};

class GreaterReal : public Instruction {
public:
	virtual void Execute()override;
};

class LessEqReal : public Instruction {
public:
	virtual void Execute()override;
};

class GreaterEqReal : public Instruction {
public:
	virtual void Execute()override;
};

class EqReal : public Instruction {
public:
	virtual void Execute()override;
};

class NotEqReal : public Instruction {
public:
	virtual void Execute()override;
};
*/

class Less : public Instruction {
public:
	virtual void Execute()override;
};

class Greater : public Instruction {
public:
	virtual void Execute()override;
};

class LessEq : public Instruction {
public:
	virtual void Execute()override;
};

class GreaterEq : public Instruction {
public:
	virtual void Execute()override;
};

class Eq : public Instruction {
public:
	virtual void Execute()override;
};

class NotEq : public Instruction {
public:
	virtual void Execute()override;
};

class Exp : public Instruction {
public:
	virtual void Execute()override;
};
/*
class LessString : public Instruction {
public:
	virtual void Execute()override;
};

class GreaterString : public Instruction {
public:
	virtual void Execute()override;
};

class LessEqString : public Instruction {
public:
	virtual void Execute()override;
};

class GreaterEqString : public Instruction {
public:
	virtual void Execute()override;
};

class EqString : public Instruction {
public:
	virtual void Execute()override;
};

class NotEqString : public Instruction {
public:
	virtual void Execute()override;
};
*/
class Jump : public Instruction {
public:
	virtual void Execute()override;
};

class Jumpif : public Instruction {
public:
	virtual void Execute()override;
};

class Pop : public Instruction {
public:
	virtual void Execute()override;
};

class Print : public Instruction {
public:
	virtual void Execute()override;
};

class Read : public Instruction {
public:
	virtual void Execute()override;
};

class Data_Function : public Instruction {
public:
	virtual void Execute()override;
};

class Dim_Function : public Instruction {
public:
	virtual void Execute()override;
};

class End_Function : public Instruction {
public:
	virtual void Execute()override;
};

class Read_Function : public Instruction {
public:
	virtual void Execute()override;
};

class Input_Function : public Instruction {
public:
	virtual void Execute()override;
};

class Pop_Function : public Instruction {
public:
	virtual void Execute()override;
};

class Restore_Function : public Instruction {
public:
	virtual void Execute()override;
};

class CodeLineNumberToICVMLineNumber : public Instruction {
public:
	virtual void Execute()override;
};

class CreateFor : public Instruction {
public:
	virtual void Execute() override;
};

class DeleteFor : public Instruction {
public:
	virtual void Execute() override;
};

class GetForInfo : public Instruction {
public:
	GetForInfo(const std::string & input) :Instruction(input) {}
	virtual void Execute() override;
};

class GetLineNumberAfterNEXT :public Instruction {
public:
	GetLineNumberAfterNEXT(const std::string & input) :Instruction(input) {}
	virtual void Execute() override;
};

class For : public Instruction {
public:
	virtual void Execute()override;
};

class Abs_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Asc_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Atn_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Chr_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Clog_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Cos_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Int_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Len_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Log_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Rnd_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Sgn_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Sin_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Sqr_Function :public Instruction {
public:
	virtual void Execute()override;
};
class Str_Function :public Instruction {
public:
	virtual void Execute()override;
};

class Val_Function :public Instruction {
public:
	virtual void Execute()override;
};
#endif
