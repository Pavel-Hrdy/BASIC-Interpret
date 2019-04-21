/**************************************************************
	Intermediate code virtual machine procedures declarations
	Pavel Hrd�
**************************************************************/

/*TODO:
Instrukce stroje:
1) load(jm�no prom�nn�) - hod� na z�sobn�k obsah prom�nn� - DONE
2) load konstanty - hod� na z�sobn�k konstantu - DONE
3) p�evod real -> int - DONE
4) p�evod int -> real - DONE
5) un�rn� minus - DONE
6) not - DONE
7) and - DONE
8) or - DONE
9) sou�et, d�len�, sou�in, rozd�l pro inty, realy - DONE
10) porovn�n� ==, =>, =< apod. na int, realy, stringy - DONE
11) slou�en� �et�zc� -
12) zni�en� z�znamu na vrcholu - DONE
13) save(jm�no prom�nn�) - ulo�� vrchol z�sobn�ku do prom�nn� - DONE
14) jump, jumpif - DONE
15) funkce print, read - DONE
16) pr�ce s polem - ukl�d�n� do pole, load z pole - teoreticky DONE, pole se bude br�t jako klasick� prom�nn�
17) pr�ce se stringem - splitting

Princip fungov�n� - ICVM bude obsahovat stack a pak seznam instrukc�, kter� bude sekven�n� vykon�vat.
Bude pot�eba i mapov�n� z ��slov�n� ��dk� v k�du do �islov�n� intermediate k�du.
Samotn� instrukce budou �e�eny pravd�podobn� hierarchi� t��d.
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
