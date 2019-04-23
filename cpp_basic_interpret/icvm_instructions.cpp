#include <string>
#include "icvm_instructions.h"
#include "icvm.h"
#include "exceptions.h"


//Loads value of variable named 'optionalArg' to the top of stack.
void LoadVariable::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	bool doesExist;
	std::string value = icvm->ReturnValueOfVariable(argument, doesExist);

	if (doesExist) {
		TypeOfVariable variableType = icvm->ReturnTypeOfVariable(argument, doesExist);

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
void LoadConstant::Execute()
{
	char type = argument[1];
	std::string content = argument.substr(3);
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
void IntToReal::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem intItem = icvm->PopItem(ItemType::Int);
	StackItem realItem(ItemType::Real, intItem.GetContent());

	icvm->AddStackItem(realItem);
}

//Takes real which is on the top of the stack and converts it to int.
void RealToInt::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem realItem = icvm->PopItem(ItemType::Real);
	double realContent = std::stod(realItem.GetContent());
	std::string convertedToInt = std::to_string(static_cast<int>(realContent));
	StackItem intItem(ItemType::Int, convertedToInt);

	icvm->AddStackItem(intItem);
}

//Takes real number from the top of the stack, multiplies its value by -1 and returns it to the stack.
void UnaryMinusReal::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem realItem = icvm->PopItem(ItemType::Real);
	double outputContent = std::stod(realItem.GetContent())*-1;
	StackItem newItem(ItemType::Real, std::to_string(outputContent));

	icvm->AddStackItem(newItem);
}

//Takes int number from the top of the stack, multiplies its value by -1 and returns it to the stack.
void UnaryMinusInt::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem intItem = icvm->PopItem(ItemType::Int);
	int outputContent = std::stoi(intItem.GetContent())*-1;
	StackItem newItem(ItemType::Int, std::to_string(outputContent));

	icvm->AddStackItem(newItem);
}

//Takes boolean int number from the top of the stack, negates it and puts it back to the stack.
void Not::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem intItem = icvm->PopItem(ItemType::Int);
	int outputContent = !(std::stoi(intItem.GetContent()));
	StackItem newItem(ItemType::Int, std::to_string(outputContent));

	icvm->AddStackItem(newItem);
}

//Logical and
void And::Execute()
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
void Or::Execute()
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
void AddInt::Execute()
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
void SubInt::Execute()
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
void DivInt::Execute()
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
void MulInt::Execute()
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
void AddReal::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(firstOpValue + secondOpValue));
	icvm->AddStackItem(newItem);
}

void SubReal::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue - firstOpValue));
	icvm->AddStackItem(newItem);
}

void DivReal::Execute()
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

void MulReal::Execute()
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
void LessInt::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue < firstOpValue));
	icvm->AddStackItem(newItem);
}

void GreaterInt::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue > firstOpValue));
	icvm->AddStackItem(newItem);
}

void LessEqInt::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue <= firstOpValue));
	icvm->AddStackItem(newItem);
}

void GreaterEqInt::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue >= firstOpValue));
	icvm->AddStackItem(newItem);
}

void EqInt::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Int);
	StackItem secondOperand = icvm->PopItem(ItemType::Int);
	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(secondOpValue == firstOpValue));
	icvm->AddStackItem(newItem);
}

void NotEqInt::Execute()
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
void LessReal::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue < firstOpValue));
	icvm->AddStackItem(newItem);
}

void GreaterReal::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue > firstOpValue));
	icvm->AddStackItem(newItem);
}

void LessEqReal::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue <= firstOpValue));
	icvm->AddStackItem(newItem);
}

void GreaterEqReal::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue >= firstOpValue));
	icvm->AddStackItem(newItem);
}

void EqReal::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::Real);
	StackItem secondOperand = icvm->PopItem(ItemType::Real);
	double firstOpValue = std::stod(firstOperand.GetContent());
	double secondOpValue = std::stod(secondOperand.GetContent());

	StackItem newItem(ItemType::Real, std::to_string(secondOpValue == firstOpValue));
	icvm->AddStackItem(newItem);
}

void NotEqReal::Execute()
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
void LessString::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::String);
	StackItem secondOperand = icvm->PopItem(ItemType::String);
	std::string firstOpValue = firstOperand.GetContent();
	std::string secondOpValue = secondOperand.GetContent();

	StackItem newItem(ItemType::String, std::to_string(secondOpValue < firstOpValue));
	icvm->AddStackItem(newItem);
}

void GreaterString::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::String);
	StackItem secondOperand = icvm->PopItem(ItemType::String);
	std::string firstOpValue = firstOperand.GetContent();
	std::string secondOpValue = secondOperand.GetContent();

	StackItem newItem(ItemType::String, std::to_string(secondOpValue > firstOpValue));
	icvm->AddStackItem(newItem);
}

void LessEqString::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::String);
	StackItem secondOperand = icvm->PopItem(ItemType::String);
	std::string firstOpValue = firstOperand.GetContent();
	std::string secondOpValue = secondOperand.GetContent();

	StackItem newItem(ItemType::String, std::to_string(secondOpValue <= firstOpValue));
	icvm->AddStackItem(newItem);
}

void GreaterEqString::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::String);
	StackItem secondOperand = icvm->PopItem(ItemType::String);
	std::string firstOpValue = firstOperand.GetContent();
	std::string secondOpValue = secondOperand.GetContent();

	StackItem newItem(ItemType::String, std::to_string(secondOpValue >= firstOpValue));
	icvm->AddStackItem(newItem);
}

void EqString::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem firstOperand = icvm->PopItem(ItemType::String);
	StackItem secondOperand = icvm->PopItem(ItemType::String);
	std::string firstOpValue = firstOperand.GetContent();
	std::string secondOpValue = secondOperand.GetContent();

	StackItem newItem(ItemType::String, std::to_string(secondOpValue == firstOpValue));
	icvm->AddStackItem(newItem);
}

void NotEqString::Execute()
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
void ConcatString::Execute()
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
void SaveToVariable::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	bool doesExist;
	TypeOfVariable type = icvm->ReturnTypeOfVariable(argument, doesExist);
	StackItem item;
	if (doesExist) {
		if (type == TypeOfVariable::String) { //We want to save to string variable - top of the stack has to be string
			item = icvm->PopItem(ItemType::String);
		}
		else { //Otherwise, type of variable depends on the top of the stack - but it can't be string
			item = icvm->PopItem();
			if (item.GetType() == ItemType::String) throw TypeMismatchException();
		}

		icvm->UpdateVariable(argument, item.GetContent(), (TypeOfVariable)item.GetType());
	}
	else {
		throw VariableNotFoundException();
	}
}

//Pops int from stack, takes it as a line number in code and jumps to it
void Jump::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem item = icvm->PopItem(ItemType::Int);
	icvm->ChangeIP(std::stoi(item.GetContent()));
}

//Pops int from stack, takes it as a line number in code. Then takes another int from stack, if it's 1, then jumps to line number.
void Jumpif::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem address = icvm->PopItem(ItemType::Int);
	StackItem cond = icvm->PopItem(ItemType::Int);
	if (std::stoi(cond.GetContent())) {
		icvm->ChangeIP(std::stoi(address.GetContent()));
	}
}

//Pops item from stack
void Pop::Execute()
{
	ICVM::GetInstance()->PopItem();
}

//Prints all items until it reaches the END StackItem
void Print::Execute()
{
	StackItem item = ICVM::GetInstance()->PopItem();
	std::cout << item.GetContent();
}

//Reads line from standard input and is pushed on stack as a string 
void Read::Execute()
{
	std::string input;
	std::getline(std::cin, input);
	StackItem newItem(ItemType::String, input);
	ICVM::GetInstance()->AddStackItem(newItem);
}

void Data_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem currentItem;
	ItemType currType;
	try {
		while (true) {
			currentItem = icvm->PopItem();
			currType = currentItem.GetType();
			if (currType == ItemType::End) break;
			else icvm->PushToDataStack(currentItem);
		}
	}
	catch (EmptyStackException) {
		std::string x = "Line " + std::to_string(icvm->ICVMLineToNormalLine()) + " - wrong use of DATA command.";
		std::cout << x;
	}
}

void Dim_Function::Execute() {
	ICVM * icvm = ICVM::GetInstance();
	try {
		StackItem type = icvm->PopItem();

		if (type.GetType() != ItemType::String) goto error;

		std::string typeS = type.GetContent();
		std::vector<uint32_t> dimensions;
		TypeOfVariable typeOfArray;

		if (typeS == "integer") typeOfArray = TypeOfVariable::Int;
		else if (typeS == "real") typeOfArray = TypeOfVariable::Real;
		else goto error;

		StackItem current;
		while (true) {
			current = icvm->PopItem();

			if (current.GetType() == ItemType::End) break;
			else if (current.GetType() == ItemType::Int) {
				dimensions.insert(dimensions.begin(),std::stoi(current.GetContent()));
			}
			else goto error;
		}

		StackItem id = icvm->PopItem();

		if (id.GetType() != ItemType::String) goto error;

		icvm->AddArray(id.GetContent(), typeOfArray, dimensions);
	}
	catch (EmptyStackException) {
		goto error;
	}
	return;

error:
	std::string x = "Line " + std::to_string(icvm->ICVMLineToNormalLine()) + " - wrong use of DIM command.";
	std::cout << x;
}