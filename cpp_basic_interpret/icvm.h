/**************************************************************
	Intermediate code virtual machine procedures declarations
	Pavel Hrdý
**************************************************************/

/*TODO:
Instrukce stroje:
1) load(jméno promìnné) - hodí na zásobník obsah promìnné - DONE
2) load konstanty - hodí na zásobník konstantu - DONE
3) pøevod real -> int - DONE
4) pøevod int -> real - DONE
5) unární minus - DONE
6) not - DONE
7) and - DONE
8) or - DONE
9) souèet, dìlení, souèin, rozdíl pro inty, realy - DONE
10) porovnání ==, =>, =< apod. na int, realy, stringy - DONE
11) slouèení øetìzcù -
12) znièení záznamu na vrcholu - DONE
13) save(jméno promìnné) - uloží vrchol zásobníku do promìnné - DONE
14) jump, jumpif - DONE
15) funkce print, read - DONE
16) práce s polem - ukládání do pole, load z pole - teoreticky DONE, pole se bude brát jako klasická promìnná
17) práce se stringem - splitting

Princip fungování - ICVM bude obsahovat stack a pak seznam instrukcí, které bude sekvenènì vykonávat.
Bude potøeba i mapování z èíslování øádkù v kódu do èislování intermediate kódu.
Samotné instrukce budou øešeny pravdìpodobnì hierarchií tøíd.
*/

#ifndef ICVM_H
#define ICVM_H

#include <stack>
#include <vector>
#include <map>
#include <memory>
#include "icvm_instructions.h"

enum class ItemType { Int, Real, String, Address, End };
enum class TypeOfVariable { Int, Real, String, Address, Error };

class StackItem {
private:
	ItemType type;
	std::string content;
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
	ICVM() {}
	std::stack<StackItem> stack;
	std::map<std::string, std::string> variables;
	std::map<std::string, std::vector<uint32_t>> arrayDimensions;
	std::map<std::string, TypeOfVariable> variablesTypes;
	std::map<uint32_t, uint32_t> codeToInstructionMapping;
	std::vector<std::unique_ptr<Instruction>> instructions;
	std::stack<StackItem> dataStack;
	uint32_t instructionPointer = 0;
public:
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
	void AddStackItem(const StackItem item);
	StackItem PopItem(ItemType type);
	StackItem PopItem();
	TypeOfVariable ReturnTypeOfVarOnTopofStack() const;
	void UpdateVariable(const std::string & nameOfVar, const std::string & newContent, TypeOfVariable newType);
	void AddVariable(const std::string & nameOfVar, TypeOfVariable type);
	void ChangeIP(uint32_t newIP);
	void AddInstruction(std::unique_ptr<Instruction> instr);
	bool ExecuteInstruction();
	void CopyToStack(const std::stack<StackItem> s);
	void PushToDataStack(const StackItem item);
	uint32_t ICVMLineToNormalLine();
};



#endif
