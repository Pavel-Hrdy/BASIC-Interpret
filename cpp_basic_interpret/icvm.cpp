#include "icvm.h"                                                                                                                                             
#include "exceptions.h"

ICVM* ICVM::instance;

std::string ICVM::ReturnValueOfVariable(const std::string & nameOfVariable, bool & doesExist) const
{
	auto it = variables.find(nameOfVariable);
	doesExist = false;
	if (it != variables.end()) {
		doesExist = true;
		return it->second;
	}
	else return "";
}

TypeOfVariable ICVM::ReturnTypeOfVariable(const std::string & nameOfVariable, bool & doesExist) const
{
	auto it = variablesTypes.find(nameOfVariable);
	doesExist = false;
	if (it != variablesTypes.end()) {
		doesExist = true;
		return it->second;
	}
	else return TypeOfVariable::Error;
}

void ICVM::AddStackItem(const StackItem item)
{
	stack.push(item);
}

StackItem ICVM::PopItem(ItemType type)
{
	if (stack.size() == 0) throw EmptyStackException();
	StackItem item = stack.top();
	stack.pop();
	if (item.GetType() != type) throw TypeMismatchException();
	else return item;
}

StackItem ICVM::PopItem()
{
	if (stack.size() == 0) throw EmptyStackException();
	StackItem item = stack.top();
	stack.pop();
	return item;
}

TypeOfVariable ICVM::ReturnTypeOfVarOnTopofStack() const
{
	if (stack.size() == 0) throw EmptyStackException();
	return (TypeOfVariable)stack.top().GetType();
}

//Updates value of a variable in map
void ICVM::UpdateVariable(const std::string & nameOfVar, const std::string & newContent, TypeOfVariable newType)
{
	auto it = variables.find(nameOfVar);
	if (it != variables.end()) {
		variables[nameOfVar] = newContent;
		variablesTypes[nameOfVar] = newType;
	}
}

//Adds new variable to map
void ICVM::AddVariable(const std::string & nameOfVar, TypeOfVariable type)
{
	auto it = variables.find(nameOfVar);
	if (it == variables.end()) {
		variables.emplace(nameOfVar, "");
		variablesTypes.emplace(nameOfVar, type);
	}
}

//Changes instruction pointer - newIP is line number from original code, so it has to be translated to instruction's line number
void ICVM::ChangeIP(uint32_t newIP)
{
	auto it = codeToInstructionMapping.find(newIP);
	if (it != codeToInstructionMapping.end()) {
		instructionPointer = codeToInstructionMapping[newIP];
	}
	else {
		throw CodeToInstructionTranslationException();
	}
}

//Adds new instruction to ICVM
void ICVM::AddInstruction(std::unique_ptr<Instruction> instr)
{
	instructions.push_back(std::move(instr));
}

void ICVM::ExecuteInstruction()
{
	if (instructions.size() > 0) {
		Instruction * instr = instructions[instructions.size() - 1].get();
		instr->Execute();
		instructions.pop_back();
	}
}
