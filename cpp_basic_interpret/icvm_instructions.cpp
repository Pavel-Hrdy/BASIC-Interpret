#include <string>
#include "icvm_instructions.h"
#include "icvm.h"
#include "exceptions.h"
#include "lexer.h"

//Loads value of variable which name is on the top of the stack to the top of stack.
void LoadVariable::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	bool doesExist;

	StackItem name = icvm->PopItem();

	if (name.GetType() != ItemType::String) throw TypeMismatchException();

	std::string value = icvm->ReturnValueOfVariable(name.GetContent(), doesExist);

	if (doesExist) {
		TypeOfVariable variableType = icvm->ReturnTypeOfVariable(name.GetContent(), doesExist);

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
			throw VariableNotFoundException(name.GetContent());
		}
	}
	else {
		throw VariableNotFoundException(name.GetContent());
	}
}

//Loads constant to the top of the stack. 
//'arg' is in format "type"content
// type is either I - int, R - real, S - string, E - end
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
		itemType = ItemType::EndArray;
		break;
	}
	case 'L': {
		itemType = ItemType::EndList;
		break;
	}
	default: {
		throw UnknownTypeOfConstantException();
	}
	}
	StackItem item(itemType, content);
	ICVM::GetInstance()->AddStackItem(item);
}
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


//Takes value and name from stack (in that order) and then saves value to the variable.
void SaveToVariable::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	bool doesExist;

	StackItem value = icvm->PopItem();

	StackItem name = icvm->PopItem();

	TypeOfVariable type = icvm->ReturnTypeOfVariable(name.GetContent(), doesExist);
	if (doesExist) {
		if (type == TypeOfVariable::String) { //We want to save to string variable - top of the stack has to be string
			if (value.GetType() != ItemType::String) throw TypeMismatchException();
		}
		else { //Otherwise, type of variable depends on the top of the stack - but it can't be string
			if (value.GetType() == ItemType::String) throw TypeMismatchException();
		}

		icvm->UpdateVariable(name.GetContent(), value.GetContent(), (TypeOfVariable)value.GetType());
	}
	else {
		throw VariableNotFoundException(name.GetContent());
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
	std::vector<std::string> outputVector;
	try {
		while (true) {
			current = icvm->PopItem();
			if (current.GetType() == ItemType::EndList)
				break;
			outputVector.insert(outputVector.begin(), current.GetContent());
		}

		for (size_t i = 0; i < outputVector.size(); i++) {
			std::cout << outputVector[i] << ' ';
		}

		std::cout << std::endl;
	}
	catch (EmptyStackException) {
		std::string x = "Line " + std::to_string(icvm->ICVMLineToNormalLine()) + " - wrong use of PRINT command.";
		std::cout << x;
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
			if (currType == ItemType::EndList) break;
			else icvm->PushToDataStack(currentItem);
		}
		icvm->RestoreDataStack();
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

			if (current.GetType() == ItemType::EndList) break;
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
		if (current.GetType() == ItemType::EndArray) break;

		if (current.GetType() == ItemType::Int) {
			indices.insert(indices.begin(), std::stoi(current.GetContent()));
		}
		else throw TypeMismatchException();
	}

	StackItem name = icvm->PopItem();

	if (name.GetType() != ItemType::String) throw InvalidSyntaxException(icvm->ICVMLineToNormalLine());

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

	StackItem value = icvm->PopItem();

	StackItem current;

	while (true) {
		current = icvm->PopItem();
		if (current.GetType() == ItemType::EndArray) break;

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
	icvm->AddStackItem(value);
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
	icvm->AddStackItem(returnItem);
}

void Add::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
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
	icvm->AddStackItem(returnItem);
}

void Sub::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
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
	icvm->AddStackItem(returnItem);
}

void Div::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	std::string returnContent;

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
	icvm->AddStackItem(returnItem);
}

void Mul::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
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
	icvm->AddStackItem(returnItem);
}

void Less::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
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
	icvm->AddStackItem(returnItem);
}

void Greater::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	std::string returnContent;
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
	icvm->AddStackItem(returnItem);
}

void LessEq::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
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
	icvm->AddStackItem(returnItem);
}

void GreaterEq::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	std::string returnContent;
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
	icvm->AddStackItem(returnItem);
}

void Eq::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
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
	icvm->AddStackItem(returnItem);
}

void NotEq::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
	std::string returnContent;
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
	icvm->AddStackItem(returnItem);
}

void Exp::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem firstOp = icvm->PopItem();
	StackItem secondOp = icvm->PopItem();
	ItemType returnType;
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
	icvm->AddStackItem(returnItem);
}

void End_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	icvm->End();
}

void Read_Function::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	StackItem currentItem;
	ItemType currType;
	try {
		while (true) {
			currentItem = icvm->PopItem();
			currType = currentItem.GetType();
			if (currType == ItemType::EndList) break;

			StackItem value = icvm->PopDataItem();

			icvm->UpdateVariable(currentItem.GetContent(), value.GetContent(), (TypeOfVariable)value.GetType());

		}
	}
	catch (EmptyStackException) {
		std::string x = "Line " + std::to_string(icvm->ICVMLineToNormalLine()) + " - wrong use of READ command.";
		std::cout << x;
	}
}

void LoadArrayVariableName::Execute()
{
	std::vector<uint32_t> indices;
	ICVM* icvm = ICVM::GetInstance();
	StackItem current;

	while (true) {
		current = icvm->PopItem();
		if (current.GetType() == ItemType::EndList) break;

		if (current.GetType() == ItemType::Int) {
			indices.insert(indices.begin(), std::stoi(current.GetContent()));
		}
		else throw TypeMismatchException();
	}

	StackItem name = icvm->PopItem();

	if (name.GetType() != ItemType::String) throw InvalidSyntaxException(icvm->ICVMLineToNormalLine());

	std::string varName = name.GetContent();
	varName += '(';
	for (size_t i = 0; i < indices.size(); i++) {
		varName += (char)indices[i];
		if (i != indices.size() - 1)varName += ',';
	}
	varName += ')';
	StackItem x(ItemType::String, varName);
	icvm->AddStackItem(x);
}

void SaveToNewVariable::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	bool doesExist;

	StackItem value = icvm->PopItem();

	StackItem name = icvm->PopItem();

	TypeOfVariable type = icvm->ReturnTypeOfVariable(name.GetContent(), doesExist);
	if (doesExist) {
		if (type == TypeOfVariable::String) { //We want to save to string variable - top of the stack has to be string
			if (value.GetType() != ItemType::String) throw TypeMismatchException();
		}
		else { //Otherwise, type of variable depends on the top of the stack - but it can't be string
			if (value.GetType() == ItemType::String) throw TypeMismatchException();
		}

		icvm->UpdateVariable(name.GetContent(), value.GetContent(), (TypeOfVariable)value.GetType());
	}
	else {
		icvm->UpdateVariable(name.GetContent(), value.GetContent(), (TypeOfVariable)value.GetType());
	}
}
//na stacku m�me n�zvy prom�nn�ch - v�echny popneme do vectoru, pak postupn� pro ka�dej zavol�me std::cin
//pak hod�me prom�nnou zp�tky na stack, za t�m se po�le hodnota ze vstupu, pak u� jen SaveToNewVariable
void Input_Function::Execute()
{
	ICVM* icvm = ICVM::GetInstance();
	std::vector<StackItem> vars;
	StackItem currentItem;
	ItemType currType;
	try {
		while (true) {
			currentItem = icvm->PopItem();
			currType = currentItem.GetType();
			if (currType == ItemType::EndList) break;
			vars.insert(vars.begin(), currentItem);
		}
		std::string input;

		for (size_t i = 0; i < vars.size(); i++) {
			std::cin >> input;
			Lexer l(input);
			Token x = l.GetNextToken();

			if ((x.GetTokenType()->Type() != TType::Int) && (x.GetTokenType()->Type() != TType::Real) && (x.GetTokenType()->Type() != TType::String)) {
				throw WrongInputException();
			}
			else {
				StackItem value((ItemType)x.GetTokenType()->Type(), x.GetContent());

				icvm->AddStackItem(vars[i]);
				icvm->AddStackItem(value);

				SaveToNewVariable x; x.Execute();
			}
		}
	}
	catch (EmptyStackException) {
		std::string x = "Line " + std::to_string(icvm->ICVMLineToNormalLine()) + " - wrong use of INPUT command.";
		std::cout << x;
	}
}

void Pop_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	icvm->PopAddress();
}

void Restore_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	icvm->RestoreDataStack();
}
//na z�sobn�ku jsou hodnoty v tomto po�ad� - max, start, varName
//push varname
//push start
//saveto varname
//jump to there
//push 1
//load varName
//ADD
//saveto varname
//there
//load varname
//push max
//eq
//push jumplinenumber = zjistit ��slo ��dku v kodu+10, pak se pod�vat do codetoinstruct mapy a to je ono
//jumpif 
void For::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem maxValueTemp = icvm->PopItem();
	StackItem maxValue(maxValueTemp.GetType(), std::to_string(std::stoi(maxValueTemp.GetContent()) + 1));
	StackItem startValue = icvm->PopItem();
	StackItem varName = icvm->PopItem();

	size_t currIP = icvm->GetIP();
	//icvm->RemoveInstruction(currIP-1);
	size_t startIndex = icvm->ICVMLineToNormalLine() + 10;
	size_t offset = 20;
	//currIP--;
	icvm->RecalculateLineNumMapping(startIndex, offset);

	//Jump here when popped from address stack
	LoadConstant loadJumpConst("\"I\"" + std::to_string(currIP + 7));
	std::unique_ptr<Instruction> instrLoadJumpConst = std::make_unique<LoadConstant>(loadJumpConst);
	icvm->AddInstructionAtIndex(std::move(instrLoadJumpConst), currIP);

	Jump jumpToStart;
	std::unique_ptr<Instruction> instrJumpToStart = std::make_unique<Jump>(jumpToStart);
	icvm->AddInstructionAtIndex(std::move(instrJumpToStart), currIP + 1);
	//Start - init var
	LoadConstant loadNameConst("\"S\"" + varName.GetContent());
	std::unique_ptr<Instruction> instrLoadNameConst = std::make_unique<LoadConstant>(loadNameConst);
	icvm->AddInstructionAtIndex(std::move(instrLoadNameConst), currIP + 2);

	LoadConstant loadStart("\"I\"" + startValue.GetContent());
	std::unique_ptr<Instruction> instrLoadStart = std::make_unique<LoadConstant>(loadStart);
	icvm->AddInstructionAtIndex(std::move(instrLoadStart), currIP + 3);

	SaveToNewVariable save;
	std::unique_ptr<Instruction> instrSave = std::make_unique<SaveToNewVariable>(save);
	icvm->AddInstructionAtIndex(std::move(instrSave), currIP + 4);

	LoadConstant loadJumpAddr("\"I\"" + std::to_string(currIP + 13));
	std::unique_ptr<Instruction> instrJumpAddr = std::make_unique<LoadConstant>(loadJumpAddr);
	icvm->AddInstructionAtIndex(std::move(instrJumpAddr), currIP + 5);

	Jump jumpTo;
	std::unique_ptr<Instruction> instrJumpTo = std::make_unique<Jump>(jumpTo);
	icvm->AddInstructionAtIndex(std::move(instrJumpTo), currIP + 6);

	std::unique_ptr<Instruction> instrLoadNameConst1 = std::make_unique<LoadConstant>(loadNameConst);
	icvm->AddInstructionAtIndex(std::move(instrLoadNameConst1), currIP + 7);

	LoadConstant loadOne("\"I\"1");
	std::unique_ptr<Instruction> instrLoadOne = std::make_unique<LoadConstant>(loadOne);
	icvm->AddInstructionAtIndex(std::move(instrLoadOne), currIP + 8);

	std::unique_ptr<Instruction> instrLoadNameConst3 = std::make_unique<LoadConstant>(loadNameConst);
	icvm->AddInstructionAtIndex(std::move(instrLoadNameConst3), currIP + 9);

	LoadVariable loadVar;
	std::unique_ptr<Instruction> instrVar = std::make_unique<LoadVariable>(loadVar);
	icvm->AddInstructionAtIndex(std::move(instrVar), currIP + 10);

	Add add;
	std::unique_ptr<Instruction> instrAdd = std::make_unique<Add>(add);
	icvm->AddInstructionAtIndex(std::move(instrAdd), currIP + 11);

	std::unique_ptr<Instruction> instrSave1 = std::make_unique<SaveToNewVariable>(save);
	icvm->AddInstructionAtIndex(std::move(instrSave1), currIP + 12);

	LoadConstant loadMaxConst("\"I\"" + maxValue.GetContent());
	std::unique_ptr<Instruction> instrLoadMax = std::make_unique<LoadConstant>(loadMaxConst);
	icvm->AddInstructionAtIndex(std::move(instrLoadMax), currIP + 13);

	std::unique_ptr<Instruction> instrLoadNameConst4 = std::make_unique<LoadConstant>(loadNameConst);
	icvm->AddInstructionAtIndex(std::move(instrLoadNameConst4), currIP + 14);
	std::unique_ptr<Instruction> instrVar1 = std::make_unique<LoadVariable>(loadVar);
	icvm->AddInstructionAtIndex(std::move(instrVar1), currIP + 15);

	Eq eq;
	std::unique_ptr<Instruction> instrEq = std::make_unique<Eq>(eq);
	icvm->AddInstructionAtIndex(std::move(instrEq), currIP + 16);

	int32_t currentLineNum = icvm->ICVMLineToNormalLine();
	int32_t nextLineNum = icvm->forNextPairs[currentLineNum] + 10;

	int32_t jumpNum = icvm->NormalLineToICVM(nextLineNum);

	LoadConstant loadJumpNum("\"I\"" + std::to_string(jumpNum));
	std::unique_ptr<Instruction> instrLoadJumpNum = std::make_unique<LoadConstant>(loadJumpNum);
	icvm->AddInstructionAtIndex(std::move(instrLoadJumpNum), currIP + 17);

	Jumpif jumpIf;
	std::unique_ptr<Instruction> instrJumpif = std::make_unique<Jumpif>(jumpIf);
	icvm->AddInstructionAtIndex(std::move(instrJumpif), currIP + 18);

	LoadConstant loadReturnNum("\"I\"" + std::to_string(currIP + 7));
	std::unique_ptr<Instruction> instrLoadReturnNum = std::make_unique<LoadConstant>(loadReturnNum);
	icvm->AddInstructionAtIndex(std::move(instrLoadReturnNum), currIP + 19);

	LoadToAddressStack loadToAddress;
	std::unique_ptr<Instruction> instrLoadToAddress = std::make_unique<LoadToAddressStack>(loadToAddress);
	icvm->AddInstructionAtIndex(std::move(instrLoadToAddress), currIP + 20);

	icvm->ChangeIP(currIP + 2);
}

void LoadToAddressStack::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem address = icvm->PopItem(ItemType::Int);

	icvm->PushAddress(std::stoi(address.GetContent()));
}

void PopAddressStack::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	size_t address = icvm->PopAddress();

	StackItem x(ItemType::Int, std::to_string(address));

	icvm->AddStackItem(x);
}

void CodeLineNumberToICVMLineNumber::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	StackItem codeLN = icvm->PopItem(ItemType::Int);

	int32_t intCodeLn = std::stoi(codeLN.GetContent());
	int32_t icvmLine = icvm->NormalLineToICVM(intCodeLn);

	StackItem icvmLN(ItemType::Int, std::to_string(icvmLine));
	icvm->AddStackItem(icvmLN);
}
//max start var
void CreateFor::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem maxStackItem = icvm->PopItem(ItemType::Int);
	StackItem startStackItem = icvm->PopItem(ItemType::Int);
	StackItem varStackItem = icvm->PopItem(ItemType::String);
	ForInfo x(varStackItem.GetContent(), std::stoi(startStackItem.GetContent()) - 1, std::stoi(maxStackItem.GetContent()));
	icvm->forInfoStack.push(x);
}

void DeleteFor::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	if (icvm->forInfoStack.empty()) throw EmptyStackException();
	icvm->forInfoStack.pop();
	icvm->PopAddress();
}

void GetForInfo::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	if (icvm->forInfoStack.empty()) throw EmptyStackException();
	if (argument == "N") {
		StackItem name(ItemType::String, icvm->forInfoStack.top().VarName);
		icvm->AddStackItem(name);
	}
	else if (argument == "S") {
		StackItem start(ItemType::Int, std::to_string(icvm->forInfoStack.top().Start));
		icvm->AddStackItem(start);
	}
	else if (argument == "M") {
		StackItem max(ItemType::Int, std::to_string(icvm->forInfoStack.top().Max));
		icvm->AddStackItem(max);
	}
	else throw InvalidSyntaxException(icvm->ICVMLineToNormalLine());
}

void GetLineNumberAfterNEXT::Execute()
{
	ICVM * icvm = ICVM::GetInstance();
	int32_t forLN = std::stoi(argument);

	auto it = icvm->forNextPairs.find(forLN);

	if (it == icvm->forNextPairs.end()) throw InvalidSyntaxException(icvm->ICVMLineToNormalLine());
	else {
		StackItem addr(ItemType::Int, std::to_string((icvm->forNextPairs[forLN])+10));
		icvm->AddStackItem(addr);
	}

}

void Abs_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem num = icvm->PopItem();
	ItemType type = num.GetType();
	if ((num.GetType() != ItemType::Int) && (num.GetType() != ItemType::Real)) throw TypeMismatchException();

	double absNum = abs(std::stod(num.GetContent()));
	
	StackItem returnItem(type, std::to_string(absNum));
	icvm->AddStackItem(returnItem);
}

void Asc_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem item = icvm->PopItem();
	ItemType type = item.GetType();
	if (item.GetType() != ItemType::String) throw TypeMismatchException();

	char c = item.GetContent()[0];

	StackItem returnItem(ItemType::Int, std::to_string((int)c));
	icvm->AddStackItem(returnItem);
}

void Atn_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem num = icvm->PopItem();
	ItemType type = num.GetType();
	if ((num.GetType() != ItemType::Int) && (num.GetType() != ItemType::Real)) throw TypeMismatchException();

	double absNum = atan(std::stod(num.GetContent()));

	StackItem returnItem(ItemType::Real, std::to_string(absNum));
	icvm->AddStackItem(returnItem);
}

void Chr_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem num = icvm->PopItem();
	ItemType type = num.GetType();
	if (num.GetType() != ItemType::Int) throw TypeMismatchException();

	char c = (char)(std::stoi(num.GetContent()));
	std::string x = "";
	x += c;

	StackItem returnItem(ItemType::String, x);
	icvm->AddStackItem(returnItem);
}

void Clog_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem num = icvm->PopItem();
	ItemType type = num.GetType();
	if ((num.GetType() != ItemType::Int) && (num.GetType() != ItemType::Real)) throw TypeMismatchException();

	double returnNum = log10(std::stod(num.GetContent()));

	StackItem returnItem(ItemType::Real, std::to_string(returnNum));
	icvm->AddStackItem(returnItem);
}

void Cos_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem num = icvm->PopItem();
	ItemType type = num.GetType();
	if ((num.GetType() != ItemType::Int) && (num.GetType() != ItemType::Real)) throw TypeMismatchException();

	double returnNum = cos(std::stod(num.GetContent()));

	StackItem returnItem(ItemType::Real, std::to_string(returnNum));
	icvm->AddStackItem(returnItem);
}

void Int_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem num = icvm->PopItem();
	ItemType type = num.GetType();
	if ((num.GetType() != ItemType::Int) && (num.GetType() != ItemType::Real)) throw TypeMismatchException();

	int returnNum = int(std::stoi(num.GetContent()));

	StackItem returnItem(ItemType::Int, std::to_string(returnNum));
	icvm->AddStackItem(returnItem);
}

void Len_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem item = icvm->PopItem();
	ItemType type = item.GetType();
	if (item.GetType() != ItemType::String) throw TypeMismatchException();

	int returnNum = item.GetContent().length();

	StackItem returnItem(ItemType::Int, std::to_string(returnNum));
	icvm->AddStackItem(returnItem);
}

void Log_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem num = icvm->PopItem();
	ItemType type = num.GetType();
	if ((num.GetType() != ItemType::Int) && (num.GetType() != ItemType::Real)) throw TypeMismatchException();

	double returnNum = log(std::stod(num.GetContent()));

	StackItem returnItem(ItemType::Real, std::to_string(returnNum));
	icvm->AddStackItem(returnItem);
}

void Rnd_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem num = icvm->PopItem();

	double returnNum = rand() % 1000000;
	returnNum /= 1000000;

	StackItem returnItem(ItemType::Real, std::to_string(returnNum));
	icvm->AddStackItem(returnItem);
}

void Sgn_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem num = icvm->PopItem();
	ItemType type = num.GetType();
	if ((num.GetType() != ItemType::Int) && (num.GetType() != ItemType::Real)) throw TypeMismatchException();

	double returnNum1 = std::stod(num.GetContent());
	int returnNum;

	if (returnNum1 > 0) {
		returnNum = 1;
	}
	else if (returnNum1 < 0) {
		returnNum = -1;
	}else returnNum = 0;

	StackItem returnItem(ItemType::Int, std::to_string(returnNum));
	icvm->AddStackItem(returnItem);
}

void Sin_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem num = icvm->PopItem();
	ItemType type = num.GetType();
	if ((num.GetType() != ItemType::Int) && (num.GetType() != ItemType::Real)) throw TypeMismatchException();

	double returnNum = sin(std::stod(num.GetContent()));

	StackItem returnItem(ItemType::Real, std::to_string(returnNum));
	icvm->AddStackItem(returnItem);
}

void Sqr_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem num = icvm->PopItem();
	ItemType type = num.GetType();
	if ((num.GetType() != ItemType::Int) && (num.GetType() != ItemType::Real)) throw TypeMismatchException();

	double returnNum = sqrt(std::stod(num.GetContent()));

	StackItem returnItem(ItemType::Real, std::to_string(returnNum));
	icvm->AddStackItem(returnItem);
}

void Str_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem num = icvm->PopItem();
	ItemType type = num.GetType();
	if ((num.GetType() != ItemType::Int) && (num.GetType() != ItemType::Real)) throw TypeMismatchException();


	StackItem returnItem(ItemType::String, num.GetContent()	);
	icvm->AddStackItem(returnItem);
}

void Val_Function::Execute()
{
	ICVM * icvm = ICVM::GetInstance();

	StackItem item = icvm->PopItem();
	ItemType type = item.GetType();
	if (item.GetType() != ItemType::String) throw TypeMismatchException();

	StackItem returnItem(ItemType::Real, item.GetContent());
	icvm->AddStackItem(returnItem);
}
