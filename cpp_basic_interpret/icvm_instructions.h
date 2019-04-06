#ifndef ICVM_INSTRUCTIONS_H
#define ICVM_INSTRUCTIONS_H

#include <iostream>

class Instruction {
	virtual void Execute(const std::string & arg) = 0;
};

class LoadVariable :public Instruction {
	virtual void Execute(const std::string & arg) override;
};

class LoadConstant : public Instruction {
	virtual void Execute(const std::string & arg) override;
};

class SaveToVariable :public Instruction {
	virtual void Execute(const std::string & arg) override;
};

class IntToReal : public Instruction {
	virtual void Execute(const std::string & arg) override;
};

class RealToInt : public Instruction {
	virtual void Execute(const std::string & arg) override;
};

class UnaryMinusReal : public Instruction {
	virtual void Execute(const std::string & arg) override;
};

class UnaryMinusInt : public Instruction {
	virtual void Execute(const std::string & arg) override;
};

class Not : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class And : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class Or : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class AddInt : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class SubInt : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class DivInt : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class MulInt : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class AddReal : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class SubReal : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class DivReal : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class MulReal : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class LessInt : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class GreaterInt : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class LessEqInt : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class GreaterEqInt : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class EqInt : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class NotEqInt : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class LessReal : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class GreaterReal : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class LessEqReal : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class GreaterEqReal : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class EqReal : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class NotEqReal : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class LessString : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class GreaterString : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class LessEqString : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class GreaterEqString : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class EqString : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class NotEqString : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class Jump : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class Jumpif : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class Pop : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class Print : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

class Read : public Instruction {
	virtual void Execute(const std::string & arg)override;
};

#endif
