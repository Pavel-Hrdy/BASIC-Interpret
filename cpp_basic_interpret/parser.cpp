/*
Parser source file
Pavel Hrdý
*/

#include <stack>
#include <functional>
#include "parser.h"
#include "exceptions.h"

//Checks if type of current's token is equal to parameter 'type'. 
//If yes, then get new token.
//Otherwise throws exception.
void Parser::Eat(TType type)
{
	if (CurrentTokenType() == type) {
		CurrentToken = lexer.GetToken();
	}
	else {
		throw InvalidSyntaxException(CurrentToken.GetLineNumber());
	}
}

/*<Lines>  :: = Int <Statements> NewLine <Lines>
			| Int <Statements> NewLine
			| Int <Statements> EndOfCode
*/
bool Parser::Parse_Lines()
{
	if (CurrentTokenType() == TType::EndOfCode) { return true; }
	if (CurrentTokenType() != TType::Int) { return false; }
	else { Eat(TType::Int); }

	if (!Parse_Statements()) { return false; }

	if (CurrentTokenType() == TType::NewLine) {
		Eat(TType::NewLine);

		ICVM* icvm = ICVM::GetInstance();
		icvm->AddNewLineNumber(lexer.ReturnLineNumber());

		if (!Parse_Lines()) { return false; }
	}
	else if (CurrentTokenType() == TType::EndOfCode) return true;
	else return false;

	return true;
}

/*<Statements>  :: = <Statement> ':' <Statements>
| <Statement>
*/
bool Parser::Parse_Statements()
{
	if (!Parse_Statement()) { return false; }

	/*Semantic actions */

	if (CurrentTokenType() == TType::Colon) {
		Eat(TType::Colon);
		if (!Parse_Statements()) return false;
	}

	return true;
}

/*
<Statement>   :: =
| DATA <Constant List>
| DIM ID '(' <Integer List> ')' Variable
| <Function> '(' <Expression> ')'
| END
| FOR ID '=' <Expression> TO <Expression>
| GOTO <Expression>
| GOSUB <Expression>
| POP
| RESTORE
| IF <Expression> THEN <Statements>
| INPUT <ID List>
| LET ID '=' <Expression>
| ID '=' <Expression>
| NEXT ID
| PRINT <Print list>
| READ <ID List>
| RETURN
| RESTORE
| Remark*/
bool Parser::Parse_Statement()
{
	ICVM* icvm = ICVM::GetInstance();
	std::string varName;
	//| FOR ID '=' <Expression> TO <Expression>
	if (CurrentTokenType() == TType::For) {
		int currentLine = lexer.ReturnLineNumber();
		icvm->forStack.push(currentLine);
		Eat(TType::For);
		if (!Parse_ID_NameOnStack()) { return false; }
		if (CurrentTokenType() != TType::RelOp) { return false; }
		RelOp_T* x = dynamic_cast<RelOp_T*>(CurrentToken.GetTokenType());
		if (x->type != RelType::Eq) return false;
		Eat(TType::RelOp);
		if (!Parse_Expression(false)) return false;
		if (CurrentTokenType() != TType::To) { return false; }
		Eat(TType::To);
		if (!Parse_Expression(false)) return false;

		CreateFor c;
		std::unique_ptr<Instruction> instrCreate = std::make_unique<CreateFor>(c);
		icvm->AddInstruction(std::move(instrCreate));

		//Save start value to ID
		GetForInfo name("N");
		std::unique_ptr<Instruction> getNameInstr = std::make_unique<GetForInfo>(name);
		icvm->AddInstruction(std::move(getNameInstr));

		GetForInfo startValue("S");
		std::unique_ptr<Instruction> getStartValInstr = std::make_unique<GetForInfo>(startValue);
		icvm->AddInstruction(std::move(getStartValInstr));

		SaveToNewVariable saveToVar;
		std::unique_ptr<Instruction> saveNameInstr = std::make_unique<SaveToNewVariable>(saveToVar);
		icvm->AddInstruction(std::move(saveNameInstr));

		//Add 1 to FOR Var and save back to var
		std::unique_ptr<Instruction> getNameInstr1 = std::make_unique<GetForInfo>(name);
		icvm->AddInstruction(std::move(getNameInstr1));

		size_t jumpBackAddress = icvm->InstructionCount() - (size_t)1;

		LoadConstant address("\"I\"" + std::to_string(jumpBackAddress));
		std::unique_ptr<Instruction> ldAddrInstr = std::make_unique<LoadConstant>(address);
		icvm->AddInstruction(std::move(ldAddrInstr));

		LoadToAddressStack ldAddress;
		std::unique_ptr<Instruction> ldAddressInstr = std::make_unique<LoadToAddressStack>(ldAddress);
		icvm->AddInstruction(std::move(ldAddressInstr));

		LoadConstant one("\"I\"1");
		std::unique_ptr<Instruction> ldOneInstr = std::make_unique<LoadConstant>(one);
		icvm->AddInstruction(std::move(ldOneInstr));

		std::unique_ptr<Instruction> getNameInstr2 = std::make_unique<GetForInfo>(name);
		icvm->AddInstruction(std::move(getNameInstr2));

		LoadVariable ldVar;
		std::unique_ptr<Instruction> ldVarInstr = std::make_unique<LoadVariable>(ldVar);
		icvm->AddInstruction(std::move(ldVarInstr));

		Add add;
		std::unique_ptr<Instruction> addInstr = std::make_unique<Add>(add);
		icvm->AddInstruction(std::move(addInstr));

		std::unique_ptr<Instruction> saveNameInstr1 = std::make_unique<SaveToNewVariable>(saveToVar);
		icvm->AddInstruction(std::move(saveNameInstr1));
		//Compare with max value
		GetForInfo maxValue("M");
		std::unique_ptr<Instruction> getMaxValInstr = std::make_unique<GetForInfo>(maxValue);
		icvm->AddInstruction(std::move(getMaxValInstr));

		std::unique_ptr<Instruction> getNameInstr3 = std::make_unique<GetForInfo>(name);
		icvm->AddInstruction(std::move(getNameInstr3));

		std::unique_ptr<Instruction> ldVarInstr1 = std::make_unique<LoadVariable>(ldVar);
		icvm->AddInstruction(std::move(ldVarInstr1));

		GreaterEq greaterEq;
		std::unique_ptr<Instruction> greaterEqInstr = std::make_unique<GreaterEq>(greaterEq);
		icvm->AddInstruction(std::move(greaterEqInstr));
		//If max >= currValue, jump to next line
		LoadConstant loadNextLine("\"I\"" + std::to_string(currentLine + 10));
		std::unique_ptr<Instruction> loadNextLineInstr = std::make_unique<LoadConstant>(loadNextLine);
		icvm->AddInstruction(std::move(loadNextLineInstr));

		CodeLineNumberToICVMLineNumber convert;
		std::unique_ptr<Instruction> convertInst = std::make_unique<CodeLineNumberToICVMLineNumber>(convert);
		icvm->AddInstruction(std::move(convertInst));

		Jumpif jmpIf;
		std::unique_ptr<Instruction> jumpIfInstr = std::make_unique<Jumpif>(jmpIf);
		icvm->AddInstruction(std::move(jumpIfInstr));

		//If currValue > max, jump to next line after NEXT line, but delete FOR info from stack before that
		GetLineNumberAfterNEXT getLineNumAfterNext(std::to_string(currentLine));
		std::unique_ptr<Instruction> getLineNumAfterNextInstr = std::make_unique<GetLineNumberAfterNEXT>(getLineNumAfterNext);
		icvm->AddInstruction(std::move(getLineNumAfterNextInstr));

		std::unique_ptr<Instruction> convertInst1 = std::make_unique<CodeLineNumberToICVMLineNumber>(convert);
		icvm->AddInstruction(std::move(convertInst1));

		DeleteFor delFor;
		std::unique_ptr<Instruction> delForInstr = std::make_unique<DeleteFor>(delFor);
		icvm->AddInstruction(std::move(delForInstr));

		Jump jmp;
		std::unique_ptr<Instruction> jumpInstr = std::make_unique<Jump>(jmp);
		icvm->AddInstruction(std::move(jumpInstr));

		return true;
	}
	//| GOTO <Expression>
	else if (CurrentTokenType() == TType::Goto) {
		Eat(TType::Goto);

		if (!Parse_Expression(false)) return false;
		/*Semantic actions*/

		CodeLineNumberToICVMLineNumber x;
		std::unique_ptr<Instruction> instrCodeToICVM = std::make_unique< CodeLineNumberToICVMLineNumber>(x);
		icvm->AddInstruction(std::move(instrCodeToICVM));

		Jump jump;
		std::unique_ptr<Instruction> jumpInstr = std::make_unique<Jump>(jump);
		icvm->AddInstruction(std::move(jumpInstr));

		return true;
	}
	//| GOSUB <Expression>
	else if (CurrentTokenType() == TType::Gosub) {
		int nextLine = CurrentToken.GetLineNumber() + 10;
		Eat(TType::Gosub);

		if (!Parse_Expression(false)) return false;
		/*Semantic actions*/
		LoadConstant ldConst("\"I\"" + std::to_string(nextLine));
		std::unique_ptr<Instruction> ldConstInstr = std::make_unique<LoadConstant>(ldConst);
		icvm->AddInstruction(std::move(ldConstInstr));

		CodeLineNumberToICVMLineNumber x;
		std::unique_ptr<Instruction> instrCodeToICVM = std::make_unique< CodeLineNumberToICVMLineNumber>(x);
		icvm->AddInstruction(std::move(instrCodeToICVM));

		LoadToAddressStack ldToAddrStack;
		std::unique_ptr<Instruction> ldConstAddrInstr = std::make_unique<LoadToAddressStack>(ldToAddrStack);
		icvm->AddInstruction(std::move(ldConstAddrInstr));

		std::unique_ptr<Instruction> instrCodeToICVM1 = std::make_unique< CodeLineNumberToICVMLineNumber>(x);
		icvm->AddInstruction(std::move(instrCodeToICVM1));

		Jump jump;
		std::unique_ptr<Instruction> jumpInstr = std::make_unique<Jump>(jump);
		icvm->AddInstruction(std::move(jumpInstr));

		return true;
	}
	//| IF <Expression> THEN <Statement>
	else if (CurrentTokenType() == TType::If) {
		Eat(TType::If);
		if (!Parse_Expression(false)) { return false; }

		Not n;
		std::unique_ptr<Instruction> notInstr = std::make_unique<Not>(n);
		icvm->AddInstruction(std::move(notInstr));

		int nextLine = CurrentToken.GetLineNumber() + 10;

		LoadConstant ldConst("\"I\"" + std::to_string(nextLine));
		std::unique_ptr<Instruction> ldConstInstr = std::make_unique<LoadConstant>(ldConst);
		icvm->AddInstruction(std::move(ldConstInstr));

		CodeLineNumberToICVMLineNumber c;
		std::unique_ptr<Instruction> convertInstr = std::make_unique<CodeLineNumberToICVMLineNumber>(c);
		icvm->AddInstruction(std::move(convertInstr));

		Jumpif jmp;
		std::unique_ptr<Instruction> jumpifInstr = std::make_unique<Jumpif>(jmp);
		icvm->AddInstruction(std::move(jumpifInstr));

		if (CurrentTokenType() != TType::Then) { return false; }
		Eat(TType::Then);

		if (!Parse_Statement()) { return false; }
		/*Semantic actions*/


		return true;
	}
	//| ID '=' <Expression>
	else if (Parse_ID_NameOnStack()) {
		if (CurrentTokenType() != TType::RelOp) { return false; }
		RelOp_T* x = dynamic_cast<RelOp_T*>(CurrentToken.GetTokenType());
		if (x->type != RelType::Eq) return false;
		Eat(TType::RelOp);

		if (!Parse_Expression(false)) { return false; }

		/*Semantic actions*/
		SaveToNewVariable instr;
		std::unique_ptr<Instruction> instrPtr = std::make_unique<SaveToNewVariable>(instr);
		icvm->AddInstruction(std::move(instrPtr));

		return true;
	}
	//| NEXT 
	else if (CurrentTokenType() == TType::Next) {
		icvm->forNextPairs.emplace(icvm->forStack.top(), lexer.ReturnLineNumber());
		icvm->forStack.pop();

		Eat(TType::Next);

		/*Semantic actions*/
		PopAddressStack instr;
		std::unique_ptr<Instruction> instrPtr = std::make_unique<PopAddressStack>(instr);
		icvm->AddInstruction(std::move(instrPtr));

		Jump instrJump;
		std::unique_ptr<Instruction> instrJumpPtr = std::make_unique<Jump>(instrJump);
		icvm->AddInstruction(std::move(instrJumpPtr));

		return true;
	}
	//| RETURN
	else if (CurrentTokenType() == TType::Return) {
		Eat(TType::Return);

		/*Semantic actions*/
		PopAddressStack popAddrStack;
		std::unique_ptr<Instruction> ldConstAddrInstr = std::make_unique<PopAddressStack>(popAddrStack);
		icvm->AddInstruction(std::move(ldConstAddrInstr));

		Jump instrJump;
		std::unique_ptr<Instruction> instrJumpPtr = std::make_unique<Jump>(instrJump);
		icvm->AddInstruction(std::move(instrJumpPtr));

		return true;
	}
	//| <Function> '(' <Expression> ')'
	else if (CurrentTokenType() == TType::Function) {
		Token y = CurrentToken;
		Function_T* x = dynamic_cast<Function_T*>(y.GetTokenType());

		//| DATA <Constant List>
		if (x->FuncType() == FunctionType::Data) {
			Eat(TType::Function);

			LoadConstant loadConst("\"L\"");
			std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(loadConst);
			icvm->AddInstruction(std::move(instr));

			if (!Parse_ConstantList()) return false;

			x->SemanticAction();
			return true;
		}	//| DIM Variable '(' <Integer List> ')' Variable 
		else if (x->FuncType() == FunctionType::Dim) {
			Eat(TType::Function);

			if (CurrentTokenType() != TType::Variable) { return false; }

			LoadConstant loadConst("\"S\"" + CurrentToken.GetContent());
			std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(loadConst);
			icvm->AddInstruction(std::move(instr));

			Eat(TType::Variable);

			if (CurrentTokenType() != TType::LeftPar) { return false; }
			Eat(TType::LeftPar);

			LoadConstant loadConst1("\"L\"");
			std::unique_ptr<Instruction> instr1 = std::make_unique<LoadConstant>(loadConst1);
			icvm->AddInstruction(std::move(instr1));

			if (!Parse_IntegerList()) { return false; }

			if (CurrentTokenType() != TType::RightPar) { return false; }

			Eat(TType::RightPar);

			if (CurrentTokenType() == TType::Variable) {
				std::string currString = CurrentToken.GetContent();
				std::string lowerString = "";

				for (size_t i = 0; i < currString.length(); i++) {
					lowerString += static_cast<char>(currString[i]);
				}

				if ((lowerString == "integer") || (lowerString == "real")) {

					LoadConstant loadConst2("\"S\"" + lowerString);
					std::unique_ptr<Instruction> instr2 = std::make_unique<LoadConstant>(loadConst2);
					icvm->AddInstruction(std::move(instr2));

					Eat(TType::Variable);
				}
				else return false;
			}
			else return false;
			x->SemanticAction();

			return true;
		}
		//| PRINT <Print list>
		else if (x->FuncType() == FunctionType::Print) {
			Eat(TType::Function);

			LoadConstant loadConst("\"L\"");
			std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(loadConst);
			icvm->AddInstruction(std::move(instr));

			if (!Parse_PrintList()) { return false; }

			/*Semantic actions*/
			x->SemanticAction();
			return true;
		}
		//END 
		else if (x->FuncType() == FunctionType::End) {
			Eat(TType::Function);
			/*Semantic actions*/
			x->SemanticAction();
			return true;
		}
		//| READ <ID List>
		else if (x->FuncType() == FunctionType::Read) {
			Eat(TType::Function);

			LoadConstant loadConst("\"L\"");
			std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(loadConst);
			icvm->AddInstruction(std::move(instr));

			if (!Parse_IDList_NamesOnStack()) { return false; }

			x->SemanticAction();

			return true;
		}
		//| LET ID '=' <Expression>
		else if (x->FuncType() == FunctionType::Let) {
			Eat(TType::Function);
			if (!Parse_ID_NameOnStack()) { return false; }

			if (CurrentTokenType() != TType::RelOp) { return false; }
			RelOp_T* x1 = dynamic_cast<RelOp_T*>(CurrentToken.GetTokenType());
			if (x1->type != RelType::Eq) return false;
			Eat(TType::RelOp);

			if (!Parse_Expression(false)) { return false; }

			/*Semantic actions*/
			SaveToNewVariable instr;
			std::unique_ptr<Instruction> instrPtr = std::make_unique<SaveToNewVariable>(instr);
			icvm->AddInstruction(std::move(instrPtr));

			return true;
		}
		//| INPUT <ID List>
		else if (x->FuncType() == FunctionType::Input) {
			Eat(TType::Function);

			LoadConstant loadConst("\"L\"");
			std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(loadConst);
			icvm->AddInstruction(std::move(instr));

			if (!Parse_IDList_NamesOnStack()) { return false; }

			/*Semantic actions*/
			x->SemanticAction();
			return true;
		}
		//| POP
		else if (x->FuncType() == FunctionType::Pop) {
			Eat(TType::Function);
			/*Semantic actions*/
			x->SemanticAction();
			return true;
		}
		//| RESTORE
		else if (x->FuncType() == FunctionType::Restore) {
			Eat(TType::Function);

			x->SemanticAction();
			return true;
		}

		if (CurrentTokenType() != TType::LeftPar) { return false; }
		Eat(TType::LeftPar);
		if (!Parse_Expression(false)) { return false; }
		if (CurrentTokenType() != TType::RightPar) { return false; }
		Eat(TType::RightPar);
		/*Semantic actions*/
		x->SemanticAction();
		return true;
	}
	//Remark
	else if (Parse_Remark()) {

		/*Semantic actions*/

		return true;
	}
	return false;
}

/*<ID>:: = StringVariable
		  | Variable
		  | Variable '(' <Expression List> ')'
Puts value of that variable on top of the stack.
*/
bool Parser::Parse_ID()
{
	ICVM* icvm = ICVM::GetInstance();

	if (CurrentTokenType() == TType::StringVariable) {
		LoadConstant loadConst("\"S\"" + CurrentToken.GetContent());
		std::unique_ptr<Instruction> instrPtr = std::make_unique<LoadConstant>(loadConst);
		icvm->AddInstruction(std::move(instrPtr));

		LoadVariable instr;
		std::unique_ptr<Instruction> instrPtr1 = std::make_unique<LoadVariable>(instr);
		icvm->AddInstruction(std::move(instrPtr1));

		Eat(TType::StringVariable);

		/*Semantic action*/


		return true;
	}
	else if (CurrentTokenType() == TType::Variable) {
		LoadConstant loadConst("\"S\"" + CurrentToken.GetContent());
		std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(loadConst);
		icvm->AddInstruction(std::move(instr));

		Eat(TType::Variable);

		if (CurrentTokenType() == TType::LeftPar) {
			Eat(TType::LeftPar);

			LoadConstant loadConst1("\"L\"");
			std::unique_ptr<Instruction> instr1 = std::make_unique<LoadConstant>(loadConst1);
			icvm->AddInstruction(std::move(instr1));

			if (!Parse_ExpressionList(true)) return false;
			if (CurrentTokenType() != TType::RightPar) return false;
			Eat(TType::RightPar);

			LoadArrayVariable instrLoadArrayVar;
			std::unique_ptr<Instruction> instrPtr = std::make_unique<LoadArrayVariable>(instrLoadArrayVar);
			icvm->AddInstruction(std::move(instrPtr));

			return true;
		}

		LoadVariable instr1;
		std::unique_ptr<Instruction> instrPtr = std::make_unique<LoadVariable>(instr1);
		icvm->AddInstruction(std::move(instrPtr));


		return true;
	}

	return false;
}

bool Parser::Parse_ID_NameOnStack()
{
	ICVM* icvm = ICVM::GetInstance();

	if (CurrentTokenType() == TType::StringVariable) {
		LoadConstant loadConst("\"S\"" + CurrentToken.GetContent());
		std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(loadConst);
		icvm->AddInstruction(std::move(instr));

		Eat(TType::StringVariable);

		/*Semantic action*/


		return true;
	}
	else if (CurrentTokenType() == TType::Variable) {
		LoadConstant loadConst("\"S\"" + CurrentToken.GetContent());
		std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(loadConst);
		icvm->AddInstruction(std::move(instr));

		Eat(TType::Variable);

		if (CurrentTokenType() == TType::LeftPar) {
			Eat(TType::LeftPar);

			LoadConstant loadConst1("\"L\"");
			std::unique_ptr<Instruction> instr2 = std::make_unique<LoadConstant>(loadConst1);
			icvm->AddInstruction(std::move(instr2));

			if (!Parse_ExpressionList(true)) return false;
			if (CurrentTokenType() != TType::RightPar) return false;
			Eat(TType::RightPar);

			LoadArrayVariableName instr1;
			std::unique_ptr<Instruction> instrPtr = std::make_unique<LoadArrayVariableName>(instr1);
			icvm->AddInstruction(std::move(instrPtr));

			return true;
		}

		return true;
	}

	return false;
}

/*
<Integer List>    ::= Integer ',' <Integer List>
					| Integer
*/
bool Parser::Parse_IntegerList()
{
	ICVM* icvm = ICVM::GetInstance();
	if (CurrentTokenType() == TType::Int) {

		LoadConstant loadConst("\"I\"" + CurrentToken.GetContent());
		std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(loadConst);
		icvm->AddInstruction(std::move(instr));

		Eat(TType::Int);
		if (CurrentTokenType() == TType::Comma) {
			Eat(TType::Comma);
			if (!Parse_IntegerList()) return false;
		}

		/*Semantic action*/


		return true;
	}

	return false;
}

uint32_t ReturnPrecedenceOfOp(ExprTokenType input) {
	switch (input) {
	case ExprTokenType::ExpOp: {
		return 8;
	}
	case ExprTokenType::UnaryMinusOp: {
		return 7;
	}
	case ExprTokenType::MulDivOp: {
		return 6;
	}
	case ExprTokenType::AddSubOp: {
		return 5;
	}
	case ExprTokenType::RelOp: {
		return 4;
	}
	case ExprTokenType::NotOp: {
		return 3;
	}
	case ExprTokenType::AndOp: {
		return 2;
	}
	case ExprTokenType::OrOp: {
		return 1;
	}
	default: {
		return 0;
	}
	}
}

//Takes vector of tokens in infix and converts them to postfix notation
std::vector<ExprToken> ConvertInfixToPostfix(const std::vector<ExprToken>& infixTokens, size_t startIndex, size_t& endIndex) {
	std::stack<ExprToken> operators;
	std::vector<ExprToken> output;

	for (size_t i = startIndex; i < infixTokens.size(); i++) {
		endIndex = i;
		if ((infixTokens[i].GetType() == ExprTokenType::Int) || (infixTokens[i].GetType() == ExprTokenType::Real) ||
			(infixTokens[i].GetType() == ExprTokenType::Variable) || (infixTokens[i].GetType() == ExprTokenType::StringVariable)
			|| (infixTokens[i].GetType() == ExprTokenType::String) || (infixTokens[i].GetType() == ExprTokenType::Function)) {
			output.push_back(infixTokens[i]);
		}
		else if (infixTokens[i].GetType() == ExprTokenType::EndArray) {
			output.emplace_back(ExprToken(ExprTokenType::EndArray, ""));
			break;
		}
		else if (infixTokens[i].GetType() == ExprTokenType::ArrayComma) {
			continue;
		}
		else if (infixTokens[i].GetType() == ExprTokenType::LeftPar) {
			operators.push(infixTokens[i]);
		}
		else if (infixTokens[i].GetType() == ExprTokenType::RightPar) {
			while ((operators.size() > 0) && (operators.top().GetType() != ExprTokenType::LeftPar)) {
				output.push_back(operators.top());
				operators.pop();
			}
			operators.pop();
		}
		else if (infixTokens[i].GetType() == ExprTokenType::ArrayVariable) {
			size_t endI = 0;
			std::vector<ExprToken> indexPostfix;
			output.push_back(infixTokens[i]);
			indexPostfix = ConvertInfixToPostfix(infixTokens, i + 1, endI);
			i = endI;

			for (size_t j = 0; j < indexPostfix.size(); j++) {
				output.push_back(indexPostfix[j]);
			}
		}
		else //It's operator
		{
			while ((operators.size() > 0) && (ReturnPrecedenceOfOp(operators.top().GetType()) > ReturnPrecedenceOfOp(infixTokens[i].GetType()))) {
				output.push_back(operators.top());
				operators.pop();
			}
			operators.push(infixTokens[i]);
		}
	}

	while (!operators.empty()) {
		output.push_back(operators.top());
		operators.pop();
	}

	return output;
}


bool Parser::Parse_Expression(bool isArray) {

	std::vector<ExprToken> infixTokens;
	std::vector<ExprToken> postfix;
	size_t temp;
	if (!Parse_InfixExpression(infixTokens, isArray)) return false;
	postfix = ConvertInfixToPostfix(infixTokens, 0, temp);
	AddPostfixToICVM(postfix, 0, temp);
	return true;
}

void CallRightFunction(FunctionType type) {
	switch (type) {
	case FunctionType::Abs: {
		Abs_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Asc: {
		Asc_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Atn: {
		Atn_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Chr: {
		Chr_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Clog: {
		Clog_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Cos: {
		Cos_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Exp: {
		Exp_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Int: {
		Int_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Len: {
		Len_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Log: {
		Log_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Rnd: {
		Rnd_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Sgn: {
		Sgn_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Sin: {
		Sin_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Sqr: {
		Sqr_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Str: {
		Str_F_T x;
		x.SemanticAction();
		break;
	}
	case FunctionType::Val: {
		Val_F_T x;
		x.SemanticAction();
		break;
	}
	}
}

//Adds instructions to ICVM, depending on input vector of postfix notation
void Parser::AddPostfixToICVM(std::vector<ExprToken>& postfix, size_t startIndex, size_t& endIndex) {
	ICVM* icvm = ICVM::GetInstance();

	for (size_t i = startIndex; i < postfix.size(); i++) {
		endIndex = i;
		switch (postfix[i].GetType()) {
		case ExprTokenType::Int: {
			LoadConstant instr("\"I\"" + postfix[i].GetContent());
			std::unique_ptr<Instruction> instrPtr = std::make_unique<LoadConstant>(instr);
			icvm->AddInstruction(std::move(instrPtr));
			break;
		}
		case ExprTokenType::Real: {
			LoadConstant instr("\"R\"" + postfix[i].GetContent());
			std::unique_ptr<Instruction> instrPtr = std::make_unique<LoadConstant>(instr);
			icvm->AddInstruction(std::move(instrPtr));
			break;
		}
		case ExprTokenType::String: {
			LoadConstant instr("\"S\"" + postfix[i].GetContent());
			std::unique_ptr<Instruction> instrPtr = std::make_unique<LoadConstant>(instr);
			icvm->AddInstruction(std::move(instrPtr));
			break;
		}
		case ExprTokenType::Function: {
			FunctionType funcType = (FunctionType)std::stoi(postfix[i].GetContent());
			CallRightFunction(funcType);
			break;
		}
		case ExprTokenType::Variable: {
			LoadConstant loadConst("\"S\"" + postfix[i].GetContent());
			std::unique_ptr<Instruction> instrLoadConstPtr = std::make_unique<LoadConstant>(loadConst);
			icvm->AddInstruction(std::move(instrLoadConstPtr));

			LoadVariable loadVar;
			std::unique_ptr<Instruction> instrLoadVarPtr = std::make_unique<LoadVariable>(loadVar);
			icvm->AddInstruction(std::move(instrLoadVarPtr));
			break;
		}
		case ExprTokenType::StringVariable: {
			LoadConstant loadConst("\"S\"" + postfix[i].GetContent());
			std::unique_ptr<Instruction> instrLoadConstPtr = std::make_unique<LoadConstant>(loadConst);
			icvm->AddInstruction(std::move(instrLoadConstPtr));

			LoadVariable loadVar;
			std::unique_ptr<Instruction> instrLoadVarPtr = std::make_unique<LoadVariable>(loadVar);
			icvm->AddInstruction(std::move(instrLoadVarPtr));
			break;
		}
		case ExprTokenType::EndArray: {
			return;
		}
		case ExprTokenType::ArrayVariable: {
			size_t endI;

			LoadConstant loadName("\"S\"" + postfix[i].GetContent());
			std::unique_ptr<Instruction> instrLoadNamePtr = std::make_unique<LoadConstant>(loadName);
			icvm->AddInstruction(std::move(instrLoadNamePtr));

			LoadConstant loadEnd("\"A\" ");
			std::unique_ptr<Instruction> instrEndPtr = std::make_unique<LoadConstant>(loadEnd);
			icvm->AddInstruction(std::move(instrEndPtr));

			AddPostfixToICVM(postfix, i + 1, endI);
			i = endI;

			LoadArrayVariable loadArrayVar;
			std::unique_ptr<Instruction> loadArrayVarPtr = std::make_unique<LoadArrayVariable>(loadArrayVar);
			icvm->AddInstruction(std::move(loadArrayVarPtr));
			break;
		}
		case ExprTokenType::AddSubOp: {
			std::string op = postfix[i].GetContent();
			std::unique_ptr<Instruction> opPtr;
			if (op == "+") {
				Add op1;
				opPtr = std::make_unique<Add>(op1);
			}
			else if (op == "-") {
				Sub op2;
				opPtr = std::make_unique<Sub>(op2);
			}
			icvm->AddInstruction(std::move(opPtr));
			break;
		}
		case ExprTokenType::AndOp: {
			And op;
			std::unique_ptr<Instruction> opPtr = std::make_unique<And>(op);
			icvm->AddInstruction(std::move(opPtr));
			break;
		}
		case ExprTokenType::ExpOp: {
			Exp op;
			std::unique_ptr<Instruction> opPtr = std::make_unique<Exp>(op);
			icvm->AddInstruction(std::move(opPtr));
			break;
		}
		case ExprTokenType::MulDivOp: {
			std::string op = postfix[i].GetContent();
			std::unique_ptr<Instruction> opPtr;
			if (op == "*") {
				Mul op1;
				opPtr = std::make_unique<Mul>(op1);
			}
			else if (op == "/") {
				Div op2;
				opPtr = std::make_unique<Div>(op2);
			}
			icvm->AddInstruction(std::move(opPtr));
			break;
		}
		case ExprTokenType::NotOp: {
			Not op;
			std::unique_ptr<Instruction> opPtr = std::make_unique<Not>(op);
			icvm->AddInstruction(std::move(opPtr));
			break;
		}
		case ExprTokenType::OrOp: {
			Or op;
			std::unique_ptr<Instruction> opPtr = std::make_unique<Or>(op);
			icvm->AddInstruction(std::move(opPtr));
			break;
		}
		case ExprTokenType::RelOp: {
			std::string op = postfix[i].GetContent();
			std::unique_ptr<Instruction> opPtr;
			if (op == "=") {
				Eq op1;
				opPtr = std::make_unique<Eq>(op1);
			}
			else if (op == "<>") {
				NotEq op2;
				opPtr = std::make_unique<NotEq>(op2);
			}
			else if (op == ">=") {
				GreaterEq op3;
				opPtr = std::make_unique<GreaterEq>(op3);
			}
			else if (op == "<=") {
				LessEq op4;
				opPtr = std::make_unique<LessEq>(op4);
			}
			else if (op == ">") {
				Greater op5;
				opPtr = std::make_unique<Greater>(op5);
			}
			else if (op == "<") {
				Less op6;
				opPtr = std::make_unique<Less>(op6);
			}
			icvm->AddInstruction(std::move(opPtr));
			break;
		}
		case ExprTokenType::UnaryMinusOp: {
			UnaryMinus op;
			std::unique_ptr<Instruction> opPtr = std::make_unique<UnaryMinus>(op);
			icvm->AddInstruction(std::move(opPtr));
			break;
		}
		}
	}
}

//Parses infix expression and returns boolean if parsing was successful.
//Also returns vector of ExprTokens which represents infix notation.
bool Parser::Parse_InfixExpression(std::vector<ExprToken>& exprTokens, bool isInParenthesis) {
	std::vector<ExprToken> tokens;
	bool foundVariable = false;;
	bool foundOp = false;;
	int parCounter = 0;
	while (true) {
		if (CurrentTokenType() == TType::Int) {
			foundVariable = true;
			tokens.emplace_back(ExprToken(ExprTokenType::Int, CurrentToken.GetContent()));
			Eat(TType::Int);
		}
		else if (CurrentTokenType() == TType::Real) {
			foundVariable = true;
			tokens.emplace_back(ExprToken(ExprTokenType::Real, CurrentToken.GetContent()));
			Eat(TType::Real);
		}
		else if (CurrentTokenType() == TType::String) {
			foundVariable = true;
			tokens.emplace_back(ExprToken(ExprTokenType::String, CurrentToken.GetContent()));
			Eat(TType::String);
		}
		else if (CurrentTokenType() == TType::Variable) {
			foundVariable = true;
			std::string varName = CurrentToken.GetContent();
			Eat(TType::Variable);

			if (CurrentTokenType() == TType::LeftPar) { // It's array variable
				Eat(TType::LeftPar);
				std::vector<ExprToken> indices;
				while (true) {
					if (!Parse_InfixExpression(indices, true)) {
						return false;
					}
					if (CurrentTokenType() == TType::Comma) {
						indices.emplace_back(ExprToken(ExprTokenType::ArrayComma, ""));
						Eat(TType::Comma);
					}
					else if (CurrentTokenType() == TType::RightPar) {
						Eat(TType::RightPar);
						break;
					}
					else return false;
				}
				tokens.emplace_back(ExprToken(ExprTokenType::ArrayVariable, varName));

				//Copy indices to tokens vector
				for (size_t i = 0; i < indices.size(); i++) {
					tokens.push_back(indices[i]);
				}
				//Signalise end of indices
				tokens.emplace_back(ExprToken(ExprTokenType::EndArray, ""));

			}
			else
				tokens.emplace_back(ExprToken(ExprTokenType::Variable, varName));

		}
		else if (CurrentTokenType() == TType::Function) {
			Token x = CurrentToken;
			Function_T* y = dynamic_cast<Function_T*>(x.GetTokenType());
			FunctionType funcType = y->FuncType();
			Eat(TType::Function);
			std::vector<ExprToken> funcArgument;

			if (!Parse_InfixExpression(funcArgument, false))
				return false;

			for (size_t i = 0; i < funcArgument.size(); i++) {
				tokens.push_back(funcArgument[i]);
			}

			tokens.emplace_back(ExprToken(ExprTokenType::Function, std::to_string((int)funcType)));

		}
		else if (CurrentTokenType() == TType::StringVariable) {
			foundVariable = true;
			tokens.emplace_back(ExprToken(ExprTokenType::StringVariable, CurrentToken.GetContent()));
			Eat(TType::StringVariable);
		}
		else if (CurrentTokenType() == TType::LeftPar) {
			tokens.emplace_back(ExprToken(ExprTokenType::LeftPar, CurrentToken.GetContent()));
			Eat(TType::LeftPar);
			parCounter++;
		}
		else if (CurrentTokenType() == TType::RightPar) {
			if (isInParenthesis && (parCounter == 0)) {
				break;
			}
			else {
				tokens.emplace_back(ExprToken(ExprTokenType::RightPar, CurrentToken.GetContent()));
				Eat(TType::RightPar);
				parCounter--;
			}
		}
		else if (CurrentTokenType() == TType::AndOp) {
			foundOp = true;
			tokens.emplace_back(ExprToken(ExprTokenType::AndOp, CurrentToken.GetContent()));
			Eat(TType::AndOp);
		}
		else if (CurrentTokenType() == TType::ExpOp) {
			foundOp = true;
			tokens.emplace_back(ExprToken(ExprTokenType::ExpOp, CurrentToken.GetContent()));
			Eat(TType::ExpOp);
		}
		else if (CurrentTokenType() == TType::MulDivOp) {
			foundOp = true;
			tokens.emplace_back(ExprToken(ExprTokenType::MulDivOp, CurrentToken.GetContent()));
			Eat(TType::MulDivOp);
		}
		else if (CurrentTokenType() == TType::NotOp) {
			foundOp = true;
			tokens.emplace_back(ExprToken(ExprTokenType::NotOp, CurrentToken.GetContent()));
			Eat(TType::NotOp);
		}
		else if (CurrentTokenType() == TType::OrOp) {
			foundOp = true;
			tokens.emplace_back(ExprToken(ExprTokenType::OrOp, CurrentToken.GetContent()));
			Eat(TType::OrOp);
		}
		else if (CurrentTokenType() == TType::PlusMinusOp) {
			foundOp = true;
			tokens.emplace_back(ExprToken(ExprTokenType::AddSubOp, CurrentToken.GetContent()));
			Eat(TType::PlusMinusOp);
		}
		else if (CurrentTokenType() == TType::RelOp) {
			foundOp = true;
			tokens.emplace_back(ExprToken(ExprTokenType::RelOp, CurrentToken.GetContent()));
			Eat(TType::RelOp);
		}
		else if (CurrentTokenType() == TType::UnaryMinusOp) {
			foundOp = true;
			tokens.emplace_back(ExprToken(ExprTokenType::UnaryMinusOp, CurrentToken.GetContent()));
			Eat(TType::UnaryMinusOp);
		}
		else break;
	}

	if (parCounter != 0) return false;

	for (size_t i = 0; i < tokens.size(); i++) {
		exprTokens.push_back(tokens[i]);
	}
	return true;
}


/*
<Constant List>   ::= <Constant> ',' <Constant List>
					| <Constant>
*/
bool Parser::Parse_ConstantList()
{
	if (!Parse_Constant()) return false;
	if (CurrentTokenType() == TType::Comma) {
		Eat(TType::Comma);
		if (!Parse_ConstantList()) return false;

		return true;
	}

	return true;
}
/*
<ID List>  ::= ID ',' <ID List>
			 | ID
*/
bool Parser::Parse_IDList()
{
	std::string varName;
	if (!Parse_ID()) return false;
	if (CurrentTokenType() == TType::Comma) {
		Eat(TType::Comma);
		if (!Parse_IDList()) return false;
		/*Semantic actions*/

		return true;
	}

	/*Semantic actions*/

	return true;
}

bool Parser::Parse_IDList_NamesOnStack()
{
	if (!Parse_ID_NameOnStack()) return false;
	if (CurrentTokenType() == TType::Comma) {
		Eat(TType::Comma);
		if (!Parse_IDList_NamesOnStack()) return false;
		/*Semantic actions*/

		return true;
	}

	/*Semantic actions*/

	return true;
}


/*
<Print List>      ::= <Expression> ',' <Print List>
					| <Expression>
					|
*/
bool Parser::Parse_PrintList()
{
	if (Parse_Expression(false)) {

		if (CurrentTokenType() == TType::Comma) {
			Eat(TType::Comma);
			if (!Parse_PrintList()) return false;

			/*Semantic actions*/

		}
		/*Semantic actions*/


		return true;
	}
	else {
		/*Semantic actions*/

		return true;
	}
}
//<Remark> = REM{ Cokoliv až na NewLine a Colon }
bool Parser::Parse_Remark()
{
	if (CurrentTokenType() != TType::Rem) return false;
	Eat(TType::Rem);

	while ((CurrentTokenType() != TType::Colon) && (CurrentTokenType() != TType::NewLine))
	{
		Eat(CurrentTokenType());
	}

	return true;
}

/*
<Expression List> ::= <Expression> ',' <Expression List>
					| <Expression>
*/
bool Parser::Parse_ExpressionList(bool isArray)
{
	if (!Parse_Expression(isArray)) return false;
	if (CurrentTokenType() == TType::Comma) {
		if (!Parse_ExpressionList(isArray)) return false;
		/*Semantics actions*/

		return true;
	}
	/*Semantics action*/


	return true;
}
/*
<Constant> ::= Int
			 | String
			 | Real
*/
bool Parser::Parse_Constant()
{
	ICVM* icvm = ICVM::GetInstance();
	if (CurrentTokenType() == TType::Int) {

		/*Semantic actions*/

		LoadConstant loadConst("\"I\"" + CurrentToken.GetContent());
		std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(loadConst);
		icvm->AddInstruction(std::move(instr));

		Eat(TType::Int);
		return true;
	}
	else if (CurrentTokenType() == TType::Real) {
		/*Semantic actions*/
		LoadConstant loadConst("\"R\"" + CurrentToken.GetContent());
		std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(loadConst);
		icvm->AddInstruction(std::move(instr));
		Eat(TType::Real);
		return true;
	}
	else if (CurrentTokenType() == TType::String) {
		/*Semantic actions*/
		LoadConstant loadConst("\"S\"" + CurrentToken.GetContent());
		std::unique_ptr<Instruction> instr = std::make_unique<LoadConstant>(loadConst);
		icvm->AddInstruction(std::move(instr));
		Eat(TType::String);
		return true;
	}
	return false;
}
/*
<Value>       ::= '(' <Expression> ')'
				| ID
				| <Constant>
*/
bool Parser::Parse_Value()
{
	std::string varName;
	ICVM* icvm = ICVM::GetInstance();
	if (CurrentTokenType() == TType::LeftPar) {
		Eat(TType::LeftPar);
		if (!Parse_Expression(true)) return false;
		if (CurrentTokenType() != TType::RightPar) throw InvalidSyntaxException(icvm->ICVMLineToNormalLine());
		Eat(TType::RightPar);
		/*Semantic actions*/

		return true;
	}
	else if (Parse_ID()) {
		/*Semantic actions*/

		return true;
	}
	else if (Parse_Constant()) {
		/*Semantic actions*/

		return true;
	}
	return true;
}


TType Parser::CurrentTokenType()
{
	return CurrentToken.GetTokenType()->Type();
}

void Parser::Parse()
{
	if (!Parse_Lines()) {
		throw InvalidSyntaxException(CurrentToken.GetLineNumber() - 10 > 0 ? CurrentToken.GetLineNumber() - 10 : 10);
	}
}
