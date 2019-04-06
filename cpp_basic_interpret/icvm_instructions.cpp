#include <string>
#include "icvm_instructions.h"
#include "icvm.h"
#include "exceptions.h"


//Loads value of variable named 'optionalArg' to the top of stack.
void LoadVariable::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	bool doesExist;
	std::string value = icvm->ReturnValueOfVariable(arg, doesExist);

	if (doesExist) {
		TypeOfVariable variableType = icvm->ReturnTypeOfVariable(arg, doesExist);

		if (variableType != TypeOfVariable::Error) {
			ItemType itemType;
			if (variableType == TypeOfVariable::Int) { // If it is a number 
				itemType = ItemType::Int;
			}
			else if (variableType == TypeOfVariable::Real) {
				itemType = ItemType::Real;
			}
			else { //otherwise it's a string
				itemType = ItemType::String;
			}
			StackItem stackItem(itemType, value);
			icvm->AddStackItem(stackItem);
		}
		else {
			throw new VariableNotFoundException;
		}
	}
	else {
		throw new VariableNotFoundException;
	}
}

//Loads constant to the top of the stack. 
//'arg' is in format "type"content
// type is either I - int, R - real, S - string, A - address
void LoadConstant::Execute(const std::string & arg)
{
	char type = arg[1];
	std::string content = arg.substr(3);
	ItemType itemType;

	switch (type) {
	case 'I': {
		itemType = ItemType::Int;
		break;
	}
	case 'R': {
		itemType = ItemType::Real;
		break;
	}
	case 'S': {
		itemType = ItemType::String;
		break;
	}
	case 'A': {
		itemType = ItemType::Address;
		break;
	}
	default: {
		throw UnknownTypeOfConstantException();
	}
	}
	StackItem item(itemType, content);
	ICVM::GetInstance()->AddStackItem(item);
}

//Takes int which is on the top of the stack and converts it to real.
void IntToReal::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem intItem = icvm->PopItem(ItemType::Int);
	StackItem realItem(ItemType::Real, intItem.GetContent());

	icvm->AddStackItem(realItem);
}

//Takes real which is on the top of the stack and converts it to int.
void RealToInt::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem realItem = icvm->PopItem(ItemType::Real);
	double realContent = std::stod(realItem.GetContent());
	std::string convertedToInt = std::to_string(static_cast<int>(realContent));
	StackItem intItem(ItemType::Int, convertedToInt);

	icvm->AddStackItem(intItem);
}

//Takes real number from the top of the stack, multiplies its value by -1 and returns it to the stack.
void UnaryMinusReal::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem realItem = icvm->PopItem(ItemType::Real);
	double outputContent = std::stod(realItem.GetContent())*-1;
	StackItem newItem(ItemType::Real, std::to_string(outputContent));

	icvm->AddStackItem(newItem);
}

//Takes int number from the top of the stack, multiplies its value by -1 and returns it to the stack.
void UnaryMinusInt::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem intItem = icvm->PopItem(ItemType::Int);
	int outputContent = std::stoi(intItem.GetContent())*-1;
	StackItem newItem(ItemType::Int, std::to_string(outputContent));

	icvm->AddStackItem(newItem);
}

//Takes boolean int number from the top of the stack, negates it and puts it back to the stack.
void Not::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem intItem = icvm->PopItem(ItemType::Int);
	int outputContent = !(std::stoi(intItem.GetContent()));
	StackItem newItem(ItemType::Int, std::to_string(outputContent));

	icvm->AddStackItem(newItem);
}

//Logical and
void And::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(firstOpValue && secondOpValue));
	icvm->AddStackItem(newItem);
}

//Logical or
void Or::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(firstOpValue || secondOpValue));
	icvm->AddStackItem(newItem);
}

//Adds two integers and returns the result to the stack
void AddInt::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(firstOpValue + secondOpValue));
	icvm->AddStackItem(newItem);
}

//Subtracts two integers
void SubInt::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue - firstOpValue));
	icvm->AddStackItem(newItem);
}

//Divides two integers
void DivInt::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	if (firstOpValue == 0) throw DivideByZeroException();

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue / firstOpValue));
	icvm->AddStackItem(newItem);
}

//Multiplies two integers
void MulInt::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(firstOpValue * secondOpValue));
	icvm->AddStackItem(newItem);
}

//Arithmetic operations for real numbers
void AddReal::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(firstOpValue + secondOpValue));
	icvm->AddStackItem(newItem);
}

void SubReal::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue - firstOpValue));
	icvm->AddStackItem(newItem);
}

void DivReal::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	if (firstOpValue == 0) throw DivideByZeroException();

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue / firstOpValue));
	icvm->AddStackItem(newItem);
}

void MulReal::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(firstOpValue * secondOpValue));
	icvm->AddStackItem(newItem);
}

//Relational operators for integers
void LessInt::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue < firstOpValue));
	icvm->AddStackItem(newItem);
}

void GreaterInt::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue > firstOpValue));
	icvm->AddStackItem(newItem);
}

void LessEqInt::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue <= firstOpValue));
	icvm->AddStackItem(newItem);
}

void GreaterEqInt::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue >= firstOpValue));
	icvm->AddStackItem(newItem);
}

void EqInt::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue == firstOpValue));
	icvm->AddStackItem(newItem);
}

void NotEqInt::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue != firstOpValue));
	icvm->AddStackItem(newItem);
}

//Relational operators for real numbers
void LessReal::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue < firstOpValue));
	icvm->AddStackItem(newItem);
}

void GreaterReal::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue > firstOpValue));
	icvm->AddStackItem(newItem);
}

void LessEqReal::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue <= firstOpValue));
	icvm->AddStackItem(newItem);
}

void GreaterEqReal::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue >= firstOpValue));
	icvm->AddStackItem(newItem);
}

void EqReal::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue == firstOpValue));
	icvm->AddStackItem(newItem);
}

void NotEqReal::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue != firstOpValue));
	icvm->AddStackItem(newItem);
}

//Relational operators for strings
void LessString::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::String);
	StackItem secondOperand = icvm->PopItem(ItemType::String);
	std::string firstOpValue = firstOperand.GetContent();
	std::string secondOpValue = secondOperand.GetContent();

	StackItem newItem(ItemType::String, std::to_string(secondOpValue < firstOpValue));
	icvm->AddStackItem(newItem);
}

void GreaterString::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::String);
	StackItem secondOperand = icvm->PopItem(ItemType::String);
	std::string firstOpValue = firstOperand.GetContent();
	std::string secondOpValue = secondOperand.GetContent();

	StackItem newItem(ItemType::String, std::to_string(secondOpValue > firstOpValue));
	icvm->AddStackItem(newItem);
}

void LessEqString::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::String);
	StackItem secondOperand = icvm->PopItem(ItemType::String);
	std::string firstOpValue = firstOperand.GetContent();
	std::string secondOpValue = secondOperand.GetContent();

	StackItem newItem(ItemType::String, std::to_string(secondOpValue <= firstOpValue));
	icvm->AddStackItem(newItem);
}

void GreaterEqString::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::String);
	StackItem secondOperand = icvm->PopItem(ItemType::String);
	std::string firstOpValue = firstOperand.GetContent();
	std::string secondOpValue = secondOperand.GetContent();

	StackItem newItem(ItemType::String, std::to_string(secondOpValue >= firstOpValue));
	icvm->AddStackItem(newItem);
}

void EqString::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::String);
	StackItem secondOperand = icvm->PopItem(ItemType::String);
	std::string firstOpValue = firstOperand.GetContent();
	std::string secondOpValue = secondOperand.GetContent();

	StackItem newItem(ItemType::String, std::to_string(secondOpValue == firstOpValue));
	icvm->AddStackItem(newItem);
}

void NotEqString::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::String);
	StackItem secondOperand = icvm->PopItem(ItemType::String);
	std::string firstOpValue = firstOperand.GetContent();
	std::string secondOpValue = secondOperand.GetContent();

	StackItem newItem(ItemType::String, std::to_string(secondOpValue != firstOpValue));
	icvm->AddStackItem(newItem);
}
/*
void ConcatString::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::String);
	StackItem secondOperand = icvm->PopItem(ItemType::String);
	std::string firstOpValue = firstOperand.GetContent();
	std::string secondOpValue = secondOperand.GetContent();

	StackItem newItem(ItemType::String, std::to_string(secondOpValue + firstOpValue));
	icvm->AddStackItem(newItem);
}*/

//Takes value from the top of the stack and saves it to variable named "arg"
void SaveToVariable::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	bool doesExist;
	TypeOfVariable type = icvm->ReturnTypeOfVariable(arg, doesExist);
	StackItem item;
	if (doesExist) {
		if (type == TypeOfVariable::String) { //We want to save to string variable - top of the stack has to be string
			item = icvm->PopItem(ItemType::String);
		}
		else { //Otherwise, type of variable depends on the top of the stack - but it can't be string
			item = icvm->PopItem();
			if (item.GetType() == ItemType::String) throw TypeMismatchException();
		}

		icvm->UpdateVariable(arg, item.GetContent(),(TypeOfVariable)item.GetType());
	}
	else {
		throw VariableNotFoundException();
	}
}

//Pops int from stack, takes it as a line number in code and jumps to it
void Jump::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem item = icvm->PopItem(ItemType::Int);
	icvm->ChangeIP(std::stoi(item.GetContent()));
}

//Pops int from stack, takes it as a line number in code. Then takes another int from stack, if it's 1, then jumps to line number.
void Jumpif::Execute(const std::string & arg)
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem address = icvm->PopItem(ItemType::Int);
	StackItem cond = icvm->PopItem(ItemType::Int);
	if (std::stoi(cond.GetContent())) {
		icvm->ChangeIP(std::stoi(address.GetContent()));
	}
}

//Pops item from stack
void Pop::Execute(const std::string & arg)
{
	ICVM::GetInstance()->PopItem();
}

//Prints item which is on top of stack to the standard output
void Print::Execute(const std::string & arg)
{
	StackItem item = ICVM::GetInstance()->PopItem();
	std::cout << item.GetContent();
}

//Reads line from standard input and is pushed on stack as a string 
void Read::Execute(const std::string & arg)
{
	std::string input;
	std::getline(std::cin, input);
	StackItem newItem(ItemType::String, input);
	ICVM::GetInstance()->AddStackItem(newItem);
}
