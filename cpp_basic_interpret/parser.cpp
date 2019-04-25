#include <stack>
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
	if (CurrentTokenType() != TType::Int) { return false; }
	else { Eat(TType::Int); }

	if (!Parse_Statements()) { return false; }

	if (CurrentTokenType() == TType::NewLine) {
		Eat(TType::NewLine);
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
| FOR ID '=' <Expression> TO <Expression> STEP Integer
| GOTO <Expression>
| GOSUB <Expression>
| POP
| RESTORE <Expression>
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
| RUN
| STOP
| Remark*/
bool Parser::Parse_Statement()
{
	ICVM* icvm = ICVM::GetInstance();
	std::string varName;
	//| FOR ID '=' <Expression> TO <Expression>
	//| FOR ID '=' <Expression> TO <Expression> STEP Integer
	if (CurrentTokenType() == TType::For) {
		if (!Parse_ID(varName)) { return false; }
		if (CurrentTokenType() != TType::RelOp) { return false; }
		RelOp_T* x = dynamic_cast<RelOp_T*>(CurrentToken.GetTokenType());
		if (x->type != RelType::Eq) return false;

		Eat(TType::RelOp);

		if (!Parse_Expression()) return false;
		if (CurrentTokenType() != TType::To) { return false; }

		Eat(TType::To);

		if (!Parse_Expression()) return false;

		if (CurrentTokenType() == TType::Step) {
			Eat(TType::Step);
			if (CurrentTokenType() != TType::Int) { return false; }
			Eat(TType::Int);
		}
		/*Semantic actions*/


		return true;
	}
	//| GOTO <Expression>
	else if (CurrentTokenType() == TType::Goto) {
		Eat(TType::Goto);

		if (!Parse_Expression()) return false;
		/*Semantic actions*/

		return true;
	}
	//| GOSUB <Expression>
	else if (CurrentTokenType() == TType::Gosub) {
		Eat(TType::Gosub);

		if (!Parse_Expression()) return false;
		/*Semantic actions*/

		return true;
	}
	//| IF <Expression> THEN <Statements>
	else if (CurrentTokenType() == TType::If) {
		Eat(TType::If);
		if (!Parse_Expression()) { return false; }

		if (CurrentTokenType() != TType::Then) { return false; }
		Eat(TType::Then);

		if (!Parse_Statements()) { return false; }
		/*Semantic actions*/


		return true;
	}
	//| ID '=' <Expression>
	else if (Parse_ID(varName)) {
		if (CurrentTokenType() != TType::RelOp) { return false; }
		RelOp_T* x = dynamic_cast<RelOp_T*>(CurrentToken.GetTokenType());
		if (x->type != RelType::Eq) return false;
		Eat(TType::RelOp);

		if (!Parse_Expression()) { return false; }

		/*Semantic actions*/


		return true;
	}
	//| NEXT ID
	else if (CurrentTokenType() == TType::Next) {
		Eat(TType::Next);

		if (!Parse_ID(varName)) { return false; }

		/*Semantic actions*/

		return true;
	}
	//| RETURN
	else if (CurrentTokenType() == TType::Return) {
		Eat(TType::Return);

		/*Semantic actions*/


		return true;
	}
	//| <Function> '(' <Expression> ')'
	else if (CurrentTokenType() == TType::Function) {
		Token y = CurrentToken;
		Function_T* x = dynamic_cast<Function_T*>(y.GetTokenType());

		//| DATA <Constant List>
		if (x->FuncType() == FunctionType::Data) {
			x->SemanticAction();
			Eat(TType::Function);
			StackItem end(ItemType::End, "");
			icvm->AddStackItem(end);
			if (!Parse_ConstantList()) return false;

			return true;
		}	//| DIM Variable '(' <Integer List> ')' Variable 
		else if (x->FuncType() == FunctionType::Dim) {
			Eat(TType::Function);

			if (CurrentTokenType() != TType::Variable) { return false; }
			StackItem name(ItemType::String, CurrentToken.GetContent());
			Eat(TType::Variable);
			icvm->AddStackItem(name);

			if (CurrentTokenType() != TType::LeftPar) { return false; }
			Eat(TType::LeftPar);
			StackItem end(ItemType::End, "");
			icvm->AddStackItem(end);

			if (!Parse_IntegerList()) { return false; }

			if (CurrentTokenType() != TType::RightPar) { return false; }

			Eat(TType::RightPar);

			if (CurrentTokenType() == TType::Variable) {
				if ((CurrentToken.GetContent() == "integer") || (CurrentToken.GetContent() == "real")) {
					StackItem type(ItemType::String, CurrentToken.GetContent());
					Eat(TType::Variable);
					icvm->AddStackItem(type);
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
			StackItem end(ItemType::End, "");
			icvm->AddStackItem(end);
			if (!Parse_PrintList()) { return false; }

			/*Semantic actions*/
			x->SemanticAction();
			return true;
		}
		//END 
		else if (x->FuncType() == FunctionType::End) {
			Eat(TType::Function);
			/*Semantic actions*/

			return true;
		}
		//| READ <ID List>
		else if (x->FuncType() == FunctionType::Read) {
			Eat(TType::Function);

			if (!Parse_IDList()) { return false; }

			return true;
		}
		//| LET ID '=' <Expression>
		else if (x->FuncType() == FunctionType::Let) {
			Eat(TType::Function);
			if (!Parse_ID(varName)) { return false; }

			if (CurrentTokenType() != TType::RelOp) { return false; }
			RelOp_T* x = dynamic_cast<RelOp_T*>(CurrentToken.GetTokenType());
			if (x->type != RelType::Eq) return false;
			Eat(TType::RelOp);

			if (!Parse_Expression()) { return false; }

			/*Semantic actions*/


			return true;
		}
		//| INPUT <ID List>
		else if (x->FuncType() == FunctionType::Input) {
			Eat(TType::Function);

			if (!Parse_IDList()) { return false; }

			/*Semantic actions*/

			return true;
		}
		//| POP
		else if (x->FuncType() == FunctionType::Pop) {
			Eat(TType::Function);
			/*Semantic actions*/

			return true;
		}
		//| RESTORE <Expression>
		//| RESTORE
		else if (x->FuncType() == FunctionType::Restore) {
			Eat(TType::Function);
			if (Parse_Expression()) {
				/*Semantic actions with expression*/
			}
			else {
				/*Semantic actions without expression*/

			}

			return true;
		}

		if (CurrentTokenType() != TType::LeftPar) { return false; }
		Eat(TType::LeftPar);
		if (!Parse_Expression()) { return false; }
		if (CurrentTokenType() != TType::RightPar) { return false; }
		Eat(TType::RightPar);
		/*Semantic actions*/

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
bool Parser::Parse_ID(std::string & varName)
{
	ICVM* icvm = ICVM::GetInstance();

	if (CurrentTokenType() == TType::StringVariable) {
		StackItem name(ItemType::String, CurrentToken.GetContent());
		varName = CurrentToken.GetContent();
		icvm->AddStackItem(name);

		LoadVariable instr;
		std::unique_ptr<Instruction> instrPtr = std::make_unique<LoadVariable>(instr);
		icvm->AddInstruction(std::move(instrPtr));

		Eat(TType::StringVariable);

		/*Semantic action*/


		return true;
	}
	else if (CurrentTokenType() == TType::Variable) {
		StackItem name(ItemType::String, CurrentToken.GetContent());
		varName = CurrentToken.GetContent();
		icvm->AddStackItem(name);

		Eat(TType::Variable);

		if (CurrentTokenType() == TType::LeftPar) {
			Eat(TType::LeftPar);

			StackItem end(ItemType::End, "");
			icvm->AddStackItem(end);

			if (!Parse_ExpressionList()) return false;
			if (CurrentTokenType() != TType::RightPar) return false;
			Eat(TType::RightPar);

			LoadArrayVariable instr;
			std::unique_ptr<Instruction> instrPtr = std::make_unique<LoadArrayVariable>(instr);
			icvm->AddInstruction(std::move(instrPtr));

			return true;
		}

		LoadVariable instr;
		std::unique_ptr<Instruction> instrPtr = std::make_unique<LoadVariable>(instr);
		icvm->AddInstruction(std::move(instrPtr));


		return true;
	}

	return false;
}
/*
<Integer List>    ::= Integer ',' <Integer List>
					| Integer
DONE
*/
bool Parser::Parse_IntegerList()
{
	ICVM* icvm = ICVM::GetInstance();
	if (CurrentTokenType() == TType::Int) {
		StackItem x(ItemType::Int, CurrentToken.GetContent());
		icvm->AddStackItem(x);
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
/*
<Expression>  ::= <And Exp> OR <Expression>
				| <And Exp>
*/
/*
bool Parser::Parse_Expression()
{
	ICVM* icvm = ICVM::GetInstance();

	if (!Parse_AndExp()) return false;

	if (CurrentTokenType() == TType::OrOp) {
		Eat(TType::OrOp);
		if (!Parse_Expression()) return false;
		/*Semantic actions

		Or instr;
		std::unique_ptr<Instruction> instrPtr = std::make_unique<Or>(instr);
		icvm->AddInstruction(std::move(instrPtr));

		return true;
	}
	/*Semantic actions



	return true;
}*/
//povolené tokeny: RelOp, PlusMinusOp, MulOp, UnaryMinus, Exp
/*
pravidla -
- pokud narazíme na číslo, další token musí být operátor
- závorky musí pasovat - uděláme counter
- pokud narazíme na Variable, musíme se podívat, zda další znak není závorka - pokud ano, je to pole


*/
bool Parser::Parse_Expression() {
	std::vector<ExprToken> tokens;
	bool foundVariable = false;;
	bool foundOp = false;;
	int parCounter = 0;
	while (true) {
		if ((foundVariable) && (CurrentTokenType() != TType::AndOp) && (CurrentTokenType() != TType::ExpOp) // There has to be an op after number
			&& (CurrentTokenType() != TType::MulDivOp) && (CurrentTokenType() != TType::NotOp) && (CurrentTokenType() != TType::OrOp)
			&& (CurrentTokenType() != TType::PlusMinusOp) && (CurrentTokenType() != TType::RelOp)) return false;
		else foundVariable = false;

		if ((foundOp) && (CurrentTokenType() != TType::Int) && (CurrentTokenType() != TType::Real) && (CurrentTokenType() != TType::Variable) // There has to be a number or left parenthesis after op 
			&& (CurrentTokenType() != TType::StringVariable) && (CurrentTokenType() != TType::LeftPar)) return false;
		else foundOp = false;

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
			bool foundNum = false;

			if (CurrentTokenType() == TType::LeftPar) { // It's array variable
				varName += '('; Eat(TType::LeftPar);

				while (true) {
					if (CurrentTokenType() == TType::Int) {
						foundNum = true;
						varName += CurrentToken.GetContent();
						Eat(TType::Int);
					}
					if (CurrentTokenType() == TType::Comma) {
						Eat(TType::Comma);
						varName += ',';
					}
					else if (CurrentTokenType() == TType::RightPar) {
						Eat(TType::RightPar);
						varName += ')';
						break;
					}
					else return false;
				}
				if (!foundNum) return false;

				tokens.emplace_back(ExprToken(ExprTokenType::ArrayVariable, varName));
			}
			else
				tokens.emplace_back(ExprToken(ExprTokenType::Variable, varName));

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
			tokens.emplace_back(ExprToken(ExprTokenType::RightPar, CurrentToken.GetContent()));
			Eat(TType::RightPar);
			parCounter--;
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
	}

	if (parCounter != 0) return false;
}


/*
<Constant List>   ::= <Constant> ',' <Constant List>
					| <Constant>
*/
bool Parser::Parse_ConstantList()
{
	ICVM* icvm = ICVM::GetInstance();

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
	if (!Parse_ID(varName)) return false;
	if (CurrentTokenType() == TType::Comma) {
		Eat(TType::Comma);
		if (!Parse_IDList()) return false;
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
	if (Parse_Expression()) {

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
<And Exp>     ::= <Not Exp> AND <And Exp>
				| <Not Exp>
*/
bool Parser::Parse_AndExp()
{
	ICVM* icvm = ICVM::GetInstance();

	if (!Parse_NotExp()) return false;
	if (CurrentTokenType() == TType::AndOp) {
		Eat(TType::AndOp);

		if (!Parse_AndExp()) return false;

		/*Semantic actions*/
		And instr;
		std::unique_ptr<Instruction> instrPtr = std::make_unique<And>(instr);
		icvm->AddInstruction(std::move(instrPtr));

		return true;
	}
	/*Semantic actions*/

	return true;
}

/*
<Not Exp>     ::= NOT <Compare Exp>
				| <Compare Exp>
*/
bool Parser::Parse_NotExp()
{
	ICVM* icvm = ICVM::GetInstance();
	if (CurrentTokenType() == TType::NotOp) {
		Eat(TType::NotOp);

		if (!Parse_CompareExp()) return false;

		Not instr;
		std::unique_ptr<Instruction> instrPtr = std::make_unique<Not>(instr);
		icvm->AddInstruction(std::move(instrPtr));

		return true;
	}
	if (!Parse_CompareExp()) return false;

	/*Semantic actions*/

	return true;
}

/*
<Add Exp>     ::= <Mult Exp> '+' <Add Exp>
				| <Mult Exp> '-' <Add Exp>
				| <Mult Exp>
*/
bool Parser::Parse_AddExp()
{
	ICVM* icvm = ICVM::GetInstance();

	if (!Parse_MultExp()) return false;

	if (CurrentTokenType() == TType::PlusMinusOp) {
		Token y = CurrentToken;
		PlusMinusOp_T* x = dynamic_cast<PlusMinusOp_T*>(y.GetTokenType());
		Eat(TType::PlusMinusOp);
		if (!Parse_AddExp()) return false;
		std::unique_ptr<Instruction> instrPtr;
		/*Semantic actions*/

		switch (x->type) {
		case SignAddType::Add: {
			Add instr;
			instrPtr = std::make_unique<Add>(instr);
			break;
		}
		case SignAddType::Sub: {
			Sub instr;
			instrPtr = std::make_unique<Sub>(instr);
			break;
		}
		}

		icvm->AddInstruction(std::move(instrPtr));

		return true;
	}
	/*Semantic actions*/


	return true;
}
/*
<Mult Exp>    ::= <Negate Exp> '*' <Mult Exp>
				| <Negate Exp> '/' <Mult Exp>
				| <Negate Exp>
				*/
bool Parser::Parse_MultExp()
{
	ICVM* icvm = ICVM::GetInstance();

	if (!Parse_NegateExp()) return false;

	if (CurrentTokenType() == TType::MulDivOp) {
		Token y = CurrentToken;
		MulDivOp_T* x = dynamic_cast<MulDivOp_T*>(y.GetTokenType());
		Eat(TType::MulDivOp);
		if (!Parse_MultExp()) return false;

		/*Semantic actions*/
		std::unique_ptr<Instruction> instrPtr;

		switch (x->type) {
		case MulType::Div: {
			Div instr;
			instrPtr = std::make_unique<Div>(instr);
			break;
		}
		case MulType::Mul: {
			Mul instr;
			instrPtr = std::make_unique<Mul>(instr);
			break;
		}
		}

		icvm->AddInstruction(std::move(instrPtr));
		return true;
	}
	/*Semantic actions*/


	return true;
}

/*
<Compare Exp> ::= <Add Exp> '='  <Compare Exp>
				| <Add Exp> '<>' <Compare Exp>
				| <Add Exp> '>'  <Compare Exp>
				| <Add Exp> '>=' <Compare Exp>
				| <Add Exp> '<'  <Compare Exp>
				| <Add Exp> '<=' <Compare Exp>
				| <Add Exp>
*/
bool Parser::Parse_CompareExp()
{
	ICVM* icvm = ICVM::GetInstance();

	if (!Parse_AddExp()) return false;

	if (CurrentTokenType() == TType::RelOp) {
		Token y = CurrentToken;
		RelOp_T* x = dynamic_cast<RelOp_T*>(y.GetTokenType());

		Eat(TType::RelOp);
		if (!Parse_CompareExp()) return false;
		std::unique_ptr<Instruction> instrPtr;
		/*Semantic actions*/

		switch (x->type) {
		case RelType::Eq: {
			Eq instr;
			instrPtr = std::make_unique<Eq>(instr);
			break;
		}
		case RelType::NotEq: {
			NotEq instr;
			instrPtr = std::make_unique<NotEq>(instr);
			break;
		}
		case RelType::Less: {
			Less instr;
			instrPtr = std::make_unique<Less>(instr);
			break;
		}
		case RelType::LessEq: {
			LessEq instr;
			instrPtr = std::make_unique<LessEq>(instr);
			break;
		}
		case RelType::Greater: {
			Greater instr;
			instrPtr = std::make_unique<Greater>(instr);
			break;
		}
		case RelType::GreaterEq: {
			GreaterEq instr;
			instrPtr = std::make_unique<GreaterEq>(instr);
			break;
		}

		}
		icvm->AddInstruction(std::move(instrPtr));

		return true;
	}
	/*Semantic actions*/


	return true;
}
/*
<Negate Exp>  ::= '-' <Power Exp>
				| <Power Exp>
*/
bool Parser::Parse_NegateExp()
{
	ICVM* icvm = ICVM::GetInstance();

	if (CurrentTokenType() == TType::PlusMinusOp) {
		PlusMinusOp_T* x = dynamic_cast<PlusMinusOp_T*>(CurrentToken.GetTokenType());
		if (x->type != SignAddType::Sub) return false;

		Eat(TType::PlusMinusOp);

		UnaryMinus instr;
		std::unique_ptr<Instruction> instrPtr = std::make_unique<UnaryMinus>(instr);
		icvm->AddInstruction(std::move(instrPtr));
	}
	if (!Parse_PowerExp())return false;
	/*Semantic actions*/

	return true;
}
/*
<Power Exp>	  ::= <Value> <Power Exp2>

*/
bool Parser::Parse_PowerExp()
{
	if (!Parse_Value()) return false;
	if (!Parse_PowerExp2()) return false;

	/*Semantic Actions*/

	return true;
}

/*
<Power Exp2>  ::= '^' <Value> <Power Exp2>
				|
*/
bool Parser::Parse_PowerExp2() {
	ICVM* icvm = ICVM::GetInstance();
	if (CurrentTokenType() == TType::ExpOp) {
		if (!Parse_Value()) return false;
		if (!Parse_PowerExp2()) return false;

		/*Semantic actions*/
		Exp instr;
		std::unique_ptr<Instruction> instrPtr = std::make_unique<Exp>(instr);
		icvm->AddInstruction(std::move(instrPtr));
		return true;
	}

	/*Semantic actions*/

	return true;
}

/*
<Expression List> ::= <Expression> ',' <Expression List>
					| <Expression>
*/
bool Parser::Parse_ExpressionList()
{
	if (!Parse_Expression()) return false;
	if (CurrentTokenType() == TType::Comma) {
		if (!Parse_ExpressionList()) return false;
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
DONE*/
bool Parser::Parse_Constant()
{
	ICVM* icvm = ICVM::GetInstance();
	if (CurrentTokenType() == TType::Int) {

		/*Semantic actions*/
		StackItem x(ItemType::Int, CurrentToken.GetContent());
		icvm->AddStackItem(x);
		Eat(TType::Int);
		return true;
	}
	else if (CurrentTokenType() == TType::Real) {
		/*Semantic actions*/
		StackItem x(ItemType::Real, CurrentToken.GetContent());
		icvm->AddStackItem(x);
		Eat(TType::Real);
		return true;
	}
	else if (CurrentTokenType() == TType::String) {
		/*Semantic actions*/
		StackItem x(ItemType::String, CurrentToken.GetContent());
		icvm->AddStackItem(x);
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
	bool doesExist = true;
	if (CurrentTokenType() == TType::LeftPar) {
		Eat(TType::LeftPar);
		if (!Parse_Expression()) return false;
		if (CurrentTokenType() != TType::RightPar) throw InvalidSyntaxException(icvm->ICVMLineToNormalLine());
		Eat(TType::RightPar);
		/*Semantic actions*/

		return true;
	}
	else if (Parse_ID(varName)) {
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
		throw InvalidSyntaxException(CurrentToken.GetLineNumber());
	}
}
