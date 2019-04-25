#include <string>
#include <algorithm>
#include "icvm_instructions.h"
#include "icvm.h"
#include "exceptions.h"


//Loads value of variable which name is on the top of the stack to the top of stack.
void LoadVariable::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	bool doesExist;

	StackItem name = icvm->PopItem();

	std::string value = icvm->ReturnValueOfVariable(name.GetContent(), doesExist);

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
			throw VariableNotFoundException(argument);
		}
	}
	else {
		throw VariableNotFoundException(argument);
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
/*
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
*/
//Takes boolean int number from the top of the stack, negates it and puts it back to the stack.
void Not::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	TypeOfVariable type = icvm->ReturnTypeOfVarOnTopofStack();
	StackItem intItem;

	if ((type == TypeOfVariable::Int) || (type == TypeOfVariable::Real)) intItem = icvm->PopItem();
	else throw TypeMismatchException();

	int outputContent = !(std::stoi(intItem.GetContent()));
	StackItem newItem(ItemType::Int, std::to_string(outputContent));

	icvm->AddStackItem(newItem);
}

//Logical and
void And::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	TypeOfVariable type = icvm->ReturnTypeOfVarOnTopofStack();
	StackItem firstOperand, secondOperand;
	if ((type == TypeOfVariable::Int) || (type == TypeOfVariable::Real)) firstOperand = icvm->PopItem();
	else throw TypeMismatchException();

	if ((type == TypeOfVariable::Int) || (type == TypeOfVariable::Real)) secondOperand = icvm->PopItem();
	else throw TypeMismatchException();

	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(firstOpValue && secondOpValue));
	icvm->AddStackItem(newItem);
}

//Logical or
void Or::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	TypeOfVariable type = icvm->ReturnTypeOfVarOnTopofStack();
	StackItem firstOperand, secondOperand;
	if ((type == TypeOfVariable::Int) || (type == TypeOfVariable::Real)) firstOperand = icvm->PopItem();
	else throw TypeMismatchException();

	if ((type == TypeOfVariable::Int) || (type == TypeOfVariable::Real)) secondOperand = icvm->PopItem();
	else throw TypeMismatchException();

	int firstOpValue = std::stoi(firstOperand.GetContent());
	int secondOpValue = std::stoi(secondOperand.GetContent());

	StackItem newItem(ItemType::Int, std::to_string(firstOpValue || secondOpValue));
	icvm->AddStackItem(newItem);
}
/*
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

//Takes name of variable and value from stack (in that order) and then saves value to the variable.
void SaveToVariable::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	bool doesExist;

	StackItem name = icvm->PopItem();

	TypeOfVariable type = icvm->ReturnTypeOfVariable(name.GetContent(), doesExist);
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
		throw VariableNotFoundException(argument);
	}
}

//Pops int from stack, takes it as a line number in code and jumps to it
void Jump::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem item = icvm->PopItem(ItemType::Int);
	icvm->ChangeIP(std::stoi(item.GetContent()));
}

//Pops int from stack, takes it as a line number in code. Then takes another int from stack, if it's 1, then jumps to line number.
void Jumpif::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
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
	ICVM* icvm = ICVM::GetInstance();
	StackItem current;
	try {
		while (true) {
			current = icvm->PopItem();
			if (current.GetType() != ItemType::End)
				std::cout << current.GetContent() << ' ';
			else break;
		}
	}
	catch (EmptyStackException) {
		std::string x = "Line " + std::to_string(icvm->ICVMLineToNormalLine()) + " - wrong use of PRINT command.";
	}
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
	ICVM* icvm = ICVM::GetInstance();
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
	ICVM* icvm = ICVM::GetInstance();
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
				dimensions.insert(dimensions.begin(), std::stoi(current.GetContent()));
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

//Takes indices from stack and returns value of array variable indexed by those indices
void LoadArrayVariable::Execute()
{
	std::vector<uint32_t> indices;
	ICVM* icvm = ICVM::GetInstance();
	StackItem current;

	while (true) {
		current = icvm->PopItem();
		if (current.GetType() == ItemType::End) break;

		if (current.GetType() == ItemType::Int) {
			indices.insert(indices.begin(), std::stoi(current.GetContent()));
		}
		else throw TypeMismatchException();
	}

	StackItem name = icvm->PopItem();

	if (current.GetType() != ItemType::String) throw InvalidSyntaxException(icvm->ICVMLineToNormalLine());

	std::string varName = name.GetContent();
	varName += '(';
	for (size_t i = 0; i < indices.size(); i++) {
		varName += indices[i];
		if (i != indices.size() - 1)varName += ',';
	}
	varName += ')';
	StackItem x(ItemType::String, varName);
	icvm->AddStackItem(x);
	LoadVariable ld;
	ld.Execute();
}

void SaveToArrayVariable::Execute()
{
	std::vector<uint32_t> indices;
	ICVM* icvm = ICVM::GetInstance();
	StackItem current;

	while (true) {
		current = icvm->PopItem();
		if (current.GetType() == ItemType::End) break;

		if (current.GetType() == ItemType::Int) {
			indices.insert(indices.begin(), std::stoi(current.GetContent()));
		}
		else throw TypeMismatchException();
	}

	StackItem name = icvm->PopItem();

	if (current.GetType() != ItemType::String) throw InvalidSyntaxException(icvm->ICVMLineToNormalLine());

	std::string varName = name.GetContent();
	varName += '(';
	for (size_t i = 0; i < indices.size(); i++) {
		varName += indices[i];
		if (i != indices.size() - 1)varName += ',';
	}
	varName += ')';
	StackItem x(ItemType::String, varName);
	icvm->AddStackItem(x);
	SaveToVariable ld;
	ld.Execute();
}

void UnaryMinus::Execute()
{
	ICVM* icvm = ICVM::GetInstance();

	TypeOfVariable type = icvm->ReturnTypeOfVarOnTopofStack();
	StackItem item = icvm->PopItem();
	ItemType returnType;
	std::string returnContent;
	switch (type) {
	case TypeOfVariable::Int: {
		returnType = ItemType::Int;
		returnContent = std::to_string(-1 * std::stoi(item.GetContent()));
		break;
	}
	case TypeOfVariable::Real: {
		returnType = ItemType::Real;
		returnContent = std::to_string(-1 * std::stod(item.GetContent()));
		break;
	}
	default: {
		throw TypeMismatchException();
		break;
	}
	}
	StackItem returnItem(returnType, returnContent);
	icvm->AddStackItem(returnItem, item.GetPrecedence());
}

void Add::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	uint32_t minPrec = std::min(firstOp.GetPrecedence(), secondOp.GetPrecedence());
	std::string returnContent;
	if ((firstOp.GetType() == ItemType::String) && (firstOp.GetType() == secondOp.GetType())) { //Add strings
		returnType = firstOp.GetType();
		returnContent = firstOp.GetContent() + secondOp.GetContent();
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Int)) { //Add ints
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stoi(firstOp.GetContent()) + std::stoi(secondOp.GetContent()));
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Real)) { //Add reals
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stod(firstOp.GetContent()) + std::stod(secondOp.GetContent()));
	}
	else if ((firstOp.GetType() == ItemType::Real) || (secondOp.GetType() == ItemType::Real)) { //Add int and real
		returnType = ItemType::Real;
		returnContent = std::to_string(std::stod(firstOp.GetContent()) + std::stod(secondOp.GetContent()));
	}
	else { throw TypeMismatchException(); }

	StackItem returnItem(returnType, returnContent);
	icvm->AddStackItem(returnItem, minPrec);
}

void Sub::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	uint32_t minPrec = std::min(firstOp.GetPrecedence(), secondOp.GetPrecedence());
	std::string returnContent;
	if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Int)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stoi(secondOp.GetContent()) - std::stoi(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Real)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stod(secondOp.GetContent()) - std::stod(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == ItemType::Real) || (secondOp.GetType() == ItemType::Real)) {
		returnType = ItemType::Real;
		returnContent = std::to_string(std::stod(secondOp.GetContent()) - std::stod(firstOp.GetContent()));
	}
	else { throw TypeMismatchException(); }

	StackItem returnItem(returnType, returnContent);
	icvm->AddStackItem(returnItem, minPrec);
}

void Div::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	size_t firstIndex, secondIndex;
	StackItem firstOp = icvm->PopItem(firstIndex);
	StackItem secondOp = icvm->PopItem(secondIndex);
	uint32_t minPrec = std::min(firstOp.GetPrecedence(), secondOp.GetPrecedence());
	ItemType returnType;
	std::string returnContent;

	StackItem temp;

	if (firstIndex <= secondIndex + 1) {
		temp = firstOp;
		firstOp = secondOp;
		secondOp = temp;
	}

	if ((firstOp.GetType() != ItemType::String) && (std::stod(firstOp.GetContent()) == 0))throw DivideByZeroException();

	if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Int)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stoi(secondOp.GetContent()) / std::stoi(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Real)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stod(secondOp.GetContent()) / std::stod(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == ItemType::Real) || (secondOp.GetType() == ItemType::Real)) {
		returnType = ItemType::Real;
		returnContent = std::to_string(std::stod(secondOp.GetContent()) / std::stod(firstOp.GetContent()));
	}
	else { throw TypeMismatchException(); }

	StackItem returnItem(returnType, returnContent);
	icvm->AddStackItem(returnItem, minPrec);
}

void Mul::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	uint32_t minPrec = std::min(firstOp.GetPrecedence(), secondOp.GetPrecedence());
	std::string returnContent;
	if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Int)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stoi(secondOp.GetContent()) * std::stoi(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Real)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stod(secondOp.GetContent()) * std::stod(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == ItemType::Real) || (secondOp.GetType() == ItemType::Real)) {
		returnType = ItemType::Real;
		returnContent = std::to_string(std::stod(secondOp.GetContent()) * std::stod(firstOp.GetContent()));
	}
	else { throw TypeMismatchException(); }

	StackItem returnItem(returnType, returnContent);
	icvm->AddStackItem(returnItem, minPrec);
}

void Less::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	uint32_t minPrec = std::min(firstOp.GetPrecedence(), secondOp.GetPrecedence());
	std::string returnContent;
	if ((firstOp.GetType() == ItemType::String) && (firstOp.GetType() == secondOp.GetType())) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(secondOp.GetContent() < firstOp.GetContent());
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Int)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stoi(secondOp.GetContent()) < std::stoi(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Real)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stod(secondOp.GetContent()) < std::stod(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == ItemType::Real) || (secondOp.GetType() == ItemType::Real)) {
		returnType = ItemType::Real;
		returnContent = std::to_string(std::stod(secondOp.GetContent()) < std::stod(firstOp.GetContent()));
	}
	else { throw TypeMismatchException(); }

	StackItem returnItem(returnType, returnContent);
	icvm->AddStackItem(returnItem, minPrec);
}

void Greater::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	std::string returnContent;
	uint32_t minPrec = std::min(firstOp.GetPrecedence(), secondOp.GetPrecedence());
	if ((firstOp.GetType() == ItemType::String) && (firstOp.GetType() == secondOp.GetType())) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(secondOp.GetContent() > firstOp.GetContent());
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Int)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stoi(secondOp.GetContent()) > std::stoi(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Real)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stod(secondOp.GetContent()) > std::stod(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == ItemType::Real) || (secondOp.GetType() == ItemType::Real)) {
		returnType = ItemType::Real;
		returnContent = std::to_string(std::stod(secondOp.GetContent()) > std::stod(firstOp.GetContent()));
	}
	else { throw TypeMismatchException(); }

	StackItem returnItem(returnType, returnContent);
	icvm->AddStackItem(returnItem, minPrec);
}

void LessEq::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	uint32_t minPrec = std::min(firstOp.GetPrecedence(), secondOp.GetPrecedence());
	std::string returnContent;
	if ((firstOp.GetType() == ItemType::String) && (firstOp.GetType() == secondOp.GetType())) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(secondOp.GetContent() <= firstOp.GetContent());
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Int)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stoi(secondOp.GetContent()) <= std::stoi(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Real)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stod(secondOp.GetContent()) <= std::stod(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == ItemType::Real) || (secondOp.GetType() == ItemType::Real)) {
		returnType = ItemType::Real;
		returnContent = std::to_string(std::stod(secondOp.GetContent()) <= std::stod(firstOp.GetContent()));
	}
	else { throw TypeMismatchException(); }

	StackItem returnItem(returnType, returnContent);
	icvm->AddStackItem(returnItem, minPrec);
}

void GreaterEq::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	std::string returnContent;
	uint32_t minPrec = std::min(firstOp.GetPrecedence(), secondOp.GetPrecedence());

	if ((firstOp.GetType() == ItemType::String) && (firstOp.GetType() == secondOp.GetType())) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(secondOp.GetContent() >= firstOp.GetContent());
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Int)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stoi(secondOp.GetContent()) >= std::stoi(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Real)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stod(secondOp.GetContent()) >= std::stod(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == ItemType::Real) || (secondOp.GetType() == ItemType::Real)) {
		returnType = ItemType::Real;
		returnContent = std::to_string(std::stod(secondOp.GetContent()) >= std::stod(firstOp.GetContent()));
	}
	else { throw TypeMismatchException(); }

	StackItem returnItem(returnType, returnContent);
	icvm->AddStackItem(returnItem, minPrec);
}

void Eq::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	uint32_t minPrec = std::min(firstOp.GetPrecedence(), secondOp.GetPrecedence());
	std::string returnContent;
	if ((firstOp.GetType() == ItemType::String) && (firstOp.GetType() == secondOp.GetType())) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(secondOp.GetContent() == firstOp.GetContent());
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Int)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stoi(secondOp.GetContent()) == std::stoi(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Real)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stod(secondOp.GetContent()) == std::stod(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == ItemType::Real) || (secondOp.GetType() == ItemType::Real)) {
		returnType = ItemType::Real;
		returnContent = std::to_string(std::stod(secondOp.GetContent()) == std::stod(firstOp.GetContent()));
	}
	else { throw TypeMismatchException(); }

	StackItem returnItem(returnType, returnContent);
	icvm->AddStackItem(returnItem, minPrec);
}

void NotEq::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	std::string returnContent;
	uint32_t minPrec = std::min(firstOp.GetPrecedence(), secondOp.GetPrecedence());
	if ((firstOp.GetType() == ItemType::String) && (firstOp.GetType() == secondOp.GetType())) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(secondOp.GetContent() != firstOp.GetContent());
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Int)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stoi(secondOp.GetContent()) != std::stoi(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Real)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(std::stod(secondOp.GetContent()) != std::stod(firstOp.GetContent()));
	}
	else if ((firstOp.GetType() == ItemType::Real) || (secondOp.GetType() == ItemType::Real)) {
		returnType = ItemType::Real;
		returnContent = std::to_string(std::stod(secondOp.GetContent()) != std::stod(firstOp.GetContent()));
	}
	else { throw TypeMismatchException(); }

	StackItem returnItem(returnType, returnContent);
	icvm->AddStackItem(returnItem, minPrec);
}

void Exp::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	uint32_t minPrec = std::min(firstOp.GetPrecedence(), secondOp.GetPrecedence());
	std::string returnContent;
	if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Int)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(pow(std::stoi(secondOp.GetContent()), std::stoi(firstOp.GetContent())));
	}
	else if ((firstOp.GetType() == secondOp.GetType()) && (firstOp.GetType() == ItemType::Real)) {
		returnType = firstOp.GetType();
		returnContent = std::to_string(pow(std::stod(secondOp.GetContent()), std::stod(firstOp.GetContent())));
	}
	else if ((firstOp.GetType() == ItemType::Real) || (secondOp.GetType() == ItemType::Real)) {
		returnType = ItemType::Real;
		returnContent = std::to_string(pow(std::stod(secondOp.GetContent()), std::stod(firstOp.GetContent())));
	}
	else { throw TypeMismatchException(); }

	StackItem returnItem(returnType, returnContent);
	icvm->AddStackItem(returnItem, minPrec);
}
