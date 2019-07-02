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

TypeOfVariable ICVM::ReturnTypeOfArrayVariable(const std::string & nameOfVariable, bool & doesExist) const
{
	auto it = arrayTypes.find(nameOfVariable);
	doesExist = false;
	if (it != arrayTypes.end()) {
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
	if (stack.size() == 0) throw EmptyStackException(ICVMLineToNormalLine());
	StackItem item = stack.top();
	stack.pop();
	if (item.GetType() != type) throw TypeMismatchException(ICVMLineToNormalLine());
	else return item;
}

StackItem ICVM::PopItem()
{
	if (stack.size() == 0) throw EmptyStackException(ICVMLineToNormalLine());
	StackItem item = stack.top();
	stack.pop();
	return item;
}

StackItem ICVM::PopDataItem()
{
	ICVM* icvm = ICVM::GetInstance();
	if (currentDataStack.size() == 0) throw EmptyStackException(icvm->ICVMLineToNormalLine());
	StackItem item = currentDataStack.front();
	currentDataStack.pop();
	return item;
}

TypeOfVariable ICVM::ReturnTypeOfVarOnTopofStack() const
{
	ICVM* icvm = ICVM::GetInstance();
	if (stack.size() == 0) throw EmptyStackException(icvm->ICVMLineToNormalLine());
	return (TypeOfVariable)stack.top().GetType();
}

//Updates value of a variable in map
void ICVM::UpdateVariable(const std::string & nameOfVar, const std::string & newContent, TypeOfVariable newType)
{
	ICVM* icvm = ICVM::GetInstance();
	if ((newType == TypeOfVariable::String) && (nameOfVar[nameOfVar.size() - 1] != '$')) throw TypeMismatchException(icvm->ICVMLineToNormalLine());
	if ((newType != TypeOfVariable::String) && (nameOfVar[nameOfVar.size() - 1] == '$')) throw TypeMismatchException(icvm->ICVMLineToNormalLine());

	auto it = variables.find(nameOfVar);
	if (it != variables.end()) {
		variables[nameOfVar] = newContent;
		variablesTypes[nameOfVar] = newType;
	}
	else {
		AddVariable(nameOfVar, newType);
		variables[nameOfVar] = newContent;
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

//Adds array variable to ICVM
void ICVM::AddArray(const std::string & nameOfArray, TypeOfVariable type, const std::vector<uint32_t> dimensions)
{
	auto it = arrayTypes.find(nameOfArray);
	if (it == arrayTypes.end()) {
		arrayTypes.emplace(nameOfArray,type);
		arrayDimensions.emplace(nameOfArray, dimensions);
	}
	else {
		arrayTypes[nameOfArray] = type;
		arrayDimensions[nameOfArray] = dimensions;
	}
}

//Changes instruction pointer - newIP is line number from original code, so it has to be translated to instruction's line number
void ICVM::ChangeIP(int32_t newIP)
{
	instructionPointer = newIP;
}

int32_t ICVM::GetIP() const
{
	return instructionPointer;
}

//Adds new instruction to ICVM
void ICVM::AddInstruction(std::unique_ptr<Instruction> instr)
{
	instructions.push_back(std::move(instr));
}

void ICVM::AddInstructionAtIndex(std::unique_ptr<Instruction> instr, uint32_t index)
{
	instructions.insert(instructions.begin() + index, std::move(instr));
}

//Executes instruction at instruction pointer and increments it.
//Returns false if there are not any other instructions to execute.
bool ICVM::ExecuteInstruction()
{
	if ((instructionPointer >= 0) && ((size_t)instructionPointer < instructions.size())) {
		Instruction * instr = instructions[instructionPointer].get();
		instructionPointer++;
		instr->Execute();
		return true;
	}
	return false;
}

void ICVM::CopyToStack(std::stack<StackItem> s)
{
	while (!s.empty()) {
		stack.push(s.top());
		s.pop();
	}
}

void ICVM::PushToDataStack(const StackItem item)
{
	dataStack.push(item);
}

void ICVM::RestoreDataStack()
{
	currentDataStack = dataStack;
}

void ICVM::RemoveInstruction(size_t index)
{
	instructions.erase(instructions.begin() + index);
}

void ICVM::PushAddress(size_t value)
{
	addressStack.push(value);
}

size_t ICVM::PopAddress()
{
	ICVM* icvm = ICVM::GetInstance();
	if (addressStack.empty()) throw EmptyStackException(icvm->ICVMLineToNormalLine());
	size_t value = addressStack.top();
	addressStack.pop();
	return value;
}

void ICVM::AddNewLineNumber(int32_t codeLineNumber)
{
	codeToInstructionMapping.emplace(codeLineNumber, instructions.size());
}

void ICVM::ExecuteAll()
{
	while (ExecuteInstruction());
}

void ICVM::End()
{
	instructionPointer = (int32_t)instructions.size();
}

int32_t ICVM::ICVMLineToNormalLine(int32_t icvmLine) 
{
	ICVM* icvm = ICVM::GetInstance();
	for (size_t i = 10; i <= codeToInstructionMapping.size() * 10; i += 10) {
		if (codeToInstructionMapping[(int32_t)i] > icvmLine) return (int32_t)(i - 10);
		else if (codeToInstructionMapping[(int32_t)i] == icvmLine) return (int32_t)i;
	}
	throw CodeToInstructionTranslationException(icvm->ICVMLineToNormalLine());
}

int32_t ICVM::ICVMLineToNormalLine()
{
	return ICVMLineToNormalLine(instructionPointer);
}

int32_t ICVM::NormalLineToICVM(int32_t normalLine) 
{
	auto it = codeToInstructionMapping.find((int32_t)normalLine);
	if (it == codeToInstructionMapping.end()) return -1;
	int32_t x = codeToInstructionMapping[(int32_t)normalLine];
	return x;
}

