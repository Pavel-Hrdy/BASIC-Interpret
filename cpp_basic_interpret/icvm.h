/**************************************************************
	Intermediate code virtual machine procedures declarations
	Pavel Hrdý
**************************************************************/

#ifndef ICVM_H
#define ICVM_H

#include <deque>
#include <stack>
#include <vector>
#include <map>
#include <queue>
#include <memory>
#include "icvm_instructions.h"

enum class ItemType { Int, Real, String, Address, EndList, EndArray };
enum class TypeOfVariable { Int, Real, String, Address,Error };

class ForInfo {
public:
	std::string VarName;
	int Start;
	int Max;
	ForInfo(const std::string & varName, int start, int max)
	{
		VarName = varName;
		Start = start;
		Max = max;
	}
};

class StackItem {
private:
	ItemType type = ItemType::EndList;
	std::string content = "";
public:
	StackItem(ItemType _type, const std::string & _content) {
		type = _type;
		content = _content;
	}
	StackItem() {}
	ItemType GetType() const {
		return type;
	}
	std::string GetContent() const {
		return content;
	}
};



class ICVM {
private:
	static ICVM *instance;
	std::stack<StackItem> stack;
	std::stack<size_t> addressStack;
	std::map<std::string, std::string> variables;
	std::map<std::string, TypeOfVariable> variablesTypes;
	std::map<std::string, TypeOfVariable> arrayTypes;
	std::map<std::string, std::vector<uint32_t>> arrayDimensions;
	std::vector<std::unique_ptr<Instruction>> instructions;
	std::queue<StackItem> dataStack;
	std::queue<StackItem> currentDataStack;
	int32_t instructionPointer = 0;

	ICVM() {
		codeToInstructionMapping.emplace(10, 0);
	}
public:
	std::stack<uint32_t> forStack;
	std::stack<ForInfo> forInfoStack;
	std::map<int32_t, int32_t> codeToInstructionMapping;
	std::map<int32_t, int32_t> forNextPairs;
	ICVM(ICVM const&) = delete;
	void operator=(ICVM const&) = delete;


	static ICVM * GetInstance() {
		if (!instance) {
			instance = new ICVM;
		}
		return instance;
	}

	void Reset() {
		if (instance) delete instance;
		instance = new ICVM;
	}

	std::string ReturnValueOfVariable(const std::string & nameOfVariable, bool & doesExist) const;
	TypeOfVariable ReturnTypeOfVariable(const std::string & nameOfVariable, bool & doesExist) const;
	TypeOfVariable ReturnTypeOfArrayVariable(const std::string & nameOfVariable, bool & doesExist) const;
	void AddStackItem(const StackItem item);
	StackItem PopItem(ItemType type);
	StackItem PopItem();
	StackItem PopDataItem();
	TypeOfVariable ReturnTypeOfVarOnTopofStack() const;
	void UpdateVariable(const std::string & nameOfVar, const std::string & newContent, TypeOfVariable newType);
	void AddVariable(const std::string & nameOfVar, TypeOfVariable type);
	void AddArray(const std::string & nameOfArray, TypeOfVariable type, const std::vector<uint32_t> dimensions);
	void ChangeIP(int32_t newIP);
	int32_t GetIP();
	void AddInstruction(std::unique_ptr<Instruction> instr);
	void AddInstructionAtIndex(std::unique_ptr<Instruction> instr, uint32_t index);
	bool ExecuteInstruction();
	void CopyToStack(const std::stack<StackItem> s);
	void PushToDataStack(const StackItem item);
	void RestoreDataStack();
	void RemoveInstruction(size_t index);
	void PushAddress(size_t value);
	size_t PopAddress();
	void AddNewLineNumber(int32_t codeLineNumber);
	void ExecuteAll();
	void End();
	size_t InstructionCount(){ return instructions.size(); }
	int32_t ICVMLineToNormalLine(int32_t icvmLine);
	int32_t ICVMLineToNormalLine();
	int32_t NormalLineToICVM(int32_t normalLine);

};



#endif
