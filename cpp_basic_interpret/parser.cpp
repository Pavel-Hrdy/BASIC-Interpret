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
		if (!Parse_Lines()) { return true; }
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
	ICVM * icvm = ICVM::GetInstance();
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
		Function_T * x = dynamic_cast<Function_T*>(y.GetTokenType());

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
	ICVM * icvm = ICVM::GetInstance();

	if (CurrentTokenType() == TType::StringVariable) {
		StackItem name(ItemType::String, CurrentToken.GetContent());
		varName = CurrentToken.GetContent();
		icvm->AddStackItem(name);

		LoadVariable instr();
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

			LoadArrayVariable instr();
			std::unique_ptr<Instruction> instrPtr = std::make_unique<LoadArrayVariable>(instr);
			icvm->AddInstruction(std::move(instrPtr));

			return true;
		}

		LoadVariable instr();
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
	ICVM * icvm = ICVM::GetInstance();
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
bool Parser::Parse_Expression()
{
	if (!Parse_AndExp()) return false;

	if (CurrentTokenType() == TType::OrOp) {
		Eat(TType::OrOp);
		if (!Parse_Expression()) return false;
		/*Semantic actions*/


		return true;
	}
	/*Semantic actions*/



	return true;
}
/*
<Constant List>   ::= <Constant> ',' <Constant List>
					| <Constant>
*/
bool Parser::Parse_ConstantList()
{
	ICVM * icvm = ICVM::GetInstance();
	ItemType temp;

	if (!Parse_Constant(temp)) return false;
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
	if (!Parse_NotExp()) return false;
	if (CurrentTokenType() == TType::AndOp) {
		Eat(TType::AndOp);

		if (!Parse_AndExp()) return false;

		/*Semantic actions*/


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
	if (CurrentTokenType() == TType::NotOp) {
		Eat(TType::NotOp);

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
	if (!Parse_MultExp()) return false;

	if (CurrentTokenType() == TType::PlusMinusOp) {
		Eat(TType::PlusMinusOp);
		if (!Parse_AddExp()) return false;

		/*Semantic actions*/

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
	if (!Parse_NegateExp()) return false;

	if (CurrentTokenType() == TType::MulDivOp) {
		Eat(TType::MulDivOp);
		if (!Parse_MultExp()) return false;

		/*Semantic actions*/

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
	if (!Parse_AddExp()) return false;

	if (CurrentTokenType() == TType::RelOp) {
		Eat(TType::RelOp);
		if (!Parse_CompareExp()) return false;

		/*Semantic actions*/

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
	if (CurrentTokenType() == TType::PlusMinusOp) {
		PlusMinusOp_T* x = dynamic_cast<PlusMinusOp_T*>(CurrentToken.GetTokenType());
		if (x->type != SignAddType::Sub) return false;

		Eat(TType::PlusMinusOp);

	}
	if (!Parse_PowerExp())return false;
	/*Semantic actions*/

	return true;
}
/*
<Power Exp>	  ::= <Value> <Power Exp2>

*/
bool Parser::Parse_PowerExp(ItemType & type)
{
	if (!Parse_Value(type)) return false;
	if (!Parse_PowerExp2(type)) return false;

	/*Semantic Actions*/

	return true;
}

/*
<Power Exp2>  ::= '^' <Value> <Power Exp2>
				|
*/
bool Parser::Parse_PowerExp2(ItemType & type) {
	ItemType valueType;
	ItemType powerExp2Type;
	if (CurrentTokenType() == TType::ExpOp) {
		if (!Parse_Value(valueType)) return false;
		if (!Parse_PowerExp2(powerExp2Type)) return false;

		/*Semantic actions*/
		type = DecideType(valueType, powerExp2Type);
		return true;
	}

	type = ItemType::Int;
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
bool Parser::Parse_Constant(ItemType & type)
{
	ICVM * icvm = ICVM::GetInstance();
	if (CurrentTokenType() == TType::Int) {

		/*Semantic actions*/
		StackItem x(ItemType::Int, CurrentToken.GetContent());
		icvm->AddStackItem(x);
		Eat(TType::Int);
		type = ItemType::Int;
		return true;
	}
	else if (CurrentTokenType() == TType::Real) {
		/*Semantic actions*/
		StackItem x(ItemType::Real, CurrentToken.GetContent());
		icvm->AddStackItem(x);
		Eat(TType::Real);
		type = ItemType::Real;
		return true;
	}
	else if (CurrentTokenType() == TType::String) {
		/*Semantic actions*/
		StackItem x(ItemType::String, CurrentToken.GetContent());
		icvm->AddStackItem(x);
		Eat(TType::String);
		type = ItemType::String;
		return true;
	}
	return false;
}
/*
<Value>       ::= '(' <Expression> ')'
				| ID
				| <Constant>
*/
bool Parser::Parse_Value(ItemType & type)
{
	std::string varName;
	ICVM * icvm = ICVM::GetInstance();
	bool doesExist = true;
	if (CurrentTokenType() == TType::LeftPar) {
		Eat(TType::LeftPar);
		if (!Parse_Expression(type)) return false;
		if (CurrentTokenType() != TType::RightPar) return false;
		Eat(TType::RightPar);
		/*Semantic actions*/

		return true;
	}
	else if (Parse_ID(varName)) {
		TypeOfVariable typeVar = icvm->ReturnTypeOfVariable(varName, doesExist);
		if (!doesExist) throw VariableNotFoundException(varName);
		type = (ItemType)typeVar;
		/*Semantic actions*/

		return true;
	}
	else if (Parse_Constant(type)) {
		/*Semantic actions*/

		return true;
	}
	return true;
}

//If arguments are equal, returns their type. 
//If one of the arguments is Real, returns Real
//Otherwise returns End as error type
ItemType Parser::DecideType(const ItemType first, const ItemType second)
{
	if (first == second) { return first; }
	else if ((first == ItemType::Real) || (second == ItemType::Real)) return ItemType::Real;
	else return ItemType::End;
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
