#include "icvm.h"                                                                                                                                             
#include "exceptions.h"

ICVM* ICVM::instance;

std::string ICVM::ReturnValueOfVariable(const std::string& nameOfVariable, bool& doesExist) const
{
	auto it = variables.find(nameOfVariable);
	doesExist = false;
	if (it != variables.end()) {
		doesExist = true;
		return it->second;
	}
	else return "";
}

TypeOfVariable ICVM::ReturnTypeOfVariable(const std::string& nameOfVariable, bool& doesExist) const
{
	auto it = variablesTypes.find(nameOfVariable);
	doesExist = false;
	if (it != variablesTypes.end()) {
		doesExist = true;
		return it->second;
	}
	else return TypeOfVariable::Error;
}

TypeOfVariable ICVM::ReturnTypeOfArrayVariable(const std::string& nameOfVariable, bool& doesExist) const
{
	auto it = arrayTypes.find(nameOfVariable);
	doesExist = false;
	if (it != arrayTypes.end()) {
		doesExist = true;
		return it->second;
	}
	else return TypeOfVariable::Error;
}

void ICVM::AddStackItem(StackItem item)
{
	item.SetPrecedence(currentPrecedence);
	stack.push_back(item);
}

void ICVM::AddStackItem(StackItem item, uint32_t prec)
{
	item.SetPrecedence(prec);
	stack.push_back(item);
}


size_t ICVM::IndexOfItemWithHighestPrecedence() {
	size_t currentIndex = 0;
	int highestPrecedence = -1;

	for (size_t i = stack.size() - 1; i > 0; i--) {
		if ((int)stack[i].GetPrecedence() > highestPrecedence) {
			highestPrecedence = stack[i].GetPrecedence();
			currentIndex = i;
		}
	}

	return currentIndex;
}


StackItem ICVM::PopItem(ItemType type)
{
	if (stack.size() == 0) throw EmptyStackException();
	StackItem item = PopItem();
	if (item.GetType() != type) throw TypeMismatchException();
	else return item;
}

StackItem ICVM::PopItem()
{
	if (stack.size() == 0) throw EmptyStackException();
	size_t index = IndexOfItemWithHighestPrecedence();
	StackItem item = stack[index];
	stack.erase(stack.begin() + index);
	return item;
}

StackItem ICVM::PopItem(size_t & index)
{
	if (stack.size() == 0) throw EmptyStackException();
	index = IndexOfItemWithHighestPrecedence();
	StackItem item = stack[index];
	stack.erase(stack.begin() + index);
	return item;
}

TypeOfVariable ICVM::ReturnTypeOfVarOnTopofStack() const
{
	if (stack.size() == 0) throw EmptyStackException();
	return (TypeOfVariable)stack.back().GetType();
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

void ICVM::AddArray(const std::string & nameOfArray, TypeOfVariable type, const std::vector<uint32_t> dimensions)
{
	auto it = arrayTypes.find(nameOfArray);
	if (it == arrayTypes.end()) {
		arrayTypes.emplace(nameOfArray, type);
		arrayDimensions.emplace(nameOfArray, dimensions);
	}
	else {
		arrayTypes[nameOfArray] = type;
		arrayDimensions[nameOfArray] = dimensions;
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

//Executes instruction at instruction pointer and increments it.
//Returns false if there are not any other instructions to execute.
bool ICVM::ExecuteInstruction()
{
	if ((instructions.size() > 0) && (instructionPointer < instructions.size())) {
		Instruction* instr = instructions[instructionPointer].get();
		instr->Execute();
		instructionPointer++;
		return true;
	}
	return false;
}

void ICVM::CopyToStack(std::stack<StackItem> s)
{
	while (!s.empty()) {
		stack.push_back(s.top());
		s.pop();
	}
}

void ICVM::PushToDataStack(const StackItem item)
{
	dataStack.push(item);
}

void ICVM::ExecuteAll()
{
	while (ExecuteInstruction());
}

uint32_t ICVM::ICVMLineToNormalLine()
{
	throw 1;
}
