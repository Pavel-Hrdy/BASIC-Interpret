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

	if (CurrentTokenType() == TType::Colon) {
		Eat(TType::Colon);
		if (!Parse_Statements()) return false;
	}

	return true;
}

/*
<Statement>   :: =
| DATA <Constant List>
| DIM ID '(' <Integer List> ')'
| <Function> '(' <Expression> ')'
| END
| FOR ID '=' <Expression> TO <Expression>
| FOR ID '=' <Expression> TO <Expression> STEP Integer
| GOTO <Expression>
| GOSUB <Expression>
| ON <Expression> GOTO <Value List>
| ON <Expression> GOSUB <Value List>
| POP
| RESTORE <Expression>
| RESTORE
| IF <Expression> THEN <Statements>
| INPUT <ID List>
| LET ID '=' <Expression>
| ID '=' <Expression>
| NEXT ID
| POKE <Value List>
| PRINT <Print list>
| READ <ID List>
| RETURN
| RESTORE
| RUN
| STOP
| Remark*/
bool Parser::Parse_Statement()
{
	//| DATA <Constant List>
	if (CurrentTokenType() == TType::Data_F) {
		Eat(TType::Data_F);
		if (!Parse_ConstantList()) return false;
		/*Semantic actions*/

		return true;
	}
	//| DIM ID '(' <Integer List> ')'
	else if (CurrentTokenType() == TType::Dim_F) {
		Eat(TType::Dim_F);
		if (!Parse_ID()) { return false; }
		if (CurrentTokenType() != TType::LeftPar) { return false; }
		Eat(TType::LeftPar);
		if (!Parse_IntegerList()) { return false; }
		if (CurrentTokenType() != TType::RightPar) { return false; }
		Eat(TType::RightPar);
		/*Semantic actions*/

		return true;
	}
	//| <Function> '(' <Expression> ')'
	else if (Parse_Functions()) {
		if (CurrentTokenType() != TType::LeftPar) { return false; }
		Eat(TType::LeftPar);
		if (!Parse_Expression()) { return false; }
		if (CurrentTokenType() != TType::RightPar) { return false; }
		Eat(TType::RightPar);
		/*Semantic actions*/

		return true;
	}
	//END 
	else if (CurrentTokenType() == TType::End_F) {
		Eat(TType::End_F);
		/*Semantic actions*/

		return true;
	}
	//| FOR ID '=' <Expression> TO <Expression>
	//| FOR ID '=' <Expression> TO <Expression> STEP Integer
	else if (CurrentTokenType() == TType::For) {
		if (!Parse_ID()) { return false; }
		if (CurrentTokenType() != TType::RelOp) { return false; }
		RelOp_T * x = dynamic_cast<RelOp_T*>(CurrentToken.GetTokenType());
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
	//| ON <Expression> GOTO <Value List>
	//| ON <Expression> GOSUB <Value List>
	else if (CurrentTokenType() == TType::On) {
		Eat(TType::On);

		if (!Parse_Expression()) return false;
		if (CurrentTokenType() == TType::Goto) {
			Eat(TType::Goto);
		}
		else if (CurrentTokenType() == TType::Gosub) {
			Eat(TType::Gosub);
		}
		else return false;

		if (!Parse_ValueList()) return false;
		/*Semantic actions*/

		return true;
	}
	//| POP
	else if (CurrentTokenType() == TType::Pop_F) {
		Eat(TType::Pop_F);
		/*Semantic actions*/

		return true;
	}
	//| RESTORE <Expression>
	//| RESTORE
	else if (CurrentTokenType() == TType::Restore_F) {
		Eat(TType::Restore_F);
		if (Parse_Expression()) {
			/*Semantic actions with expression*/
		}
		else {
			/*Semantic actions without expression*/

		}

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
	//| INPUT <ID List>
	else if (CurrentTokenType() == TType::Input_F) {
		Eat(TType::Input_F);

		if (!Parse_IDList()) { return false; }

		/*Semantic actions*/

		return true;
	}
	//| LET ID '=' <Expression>
	else if (CurrentTokenType() == TType::Let_F) {
		Eat(TType::Let_F);
		if (!Parse_ID()) { return false; }

		if (CurrentTokenType() != TType::RelOp) { return false; }
		RelOp_T * x = dynamic_cast<RelOp_T*>(CurrentToken.GetTokenType());
		if (x->type != RelType::Eq) return false;
		Eat(TType::RelOp);

		if (!Parse_Expression()) { return false; }

		/*Semantic actions*/


		return true;
	}
	//| ID '=' <Expression>
	else if (Parse_ID()) {
		if (CurrentTokenType() != TType::RelOp) { return false; }
		RelOp_T * x = dynamic_cast<RelOp_T*>(CurrentToken.GetTokenType());
		if (x->type != RelType::Eq) return false;
		Eat(TType::RelOp);

		if (!Parse_Expression()) { return false; }

		/*Semantic actions*/


		return true;
	}
	//| NEXT ID
	else if (CurrentTokenType() == TType::Next) {
		Eat(TType::Next);

		if (!Parse_ID()) { return false; }

		/*Semantic actions*/

		return true;
	}
	//| PRINT <Print list>
	else if (CurrentTokenType() == TType::Print_F) {
		Eat(TType::Print_F);

		if (!Parse_PrintList()) { return false; }

		/*Semantic actions*/

		return true;
	}
	//| READ <ID List>
	else if (CurrentTokenType() == TType::Read_F) {
		Eat(TType::Read_F);

		if (!Parse_IDList()) { return false; }

		return true;
	}
	//| RETURN
	else if (CurrentTokenType() == TType::Return) {
		Eat(TType::Return);

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
*/
bool Parser::Parse_ID()
{
	if (CurrentTokenType() == TType::StringVariable) {
		Eat(TType::StringVariable);

		/*Semantic action*/


		return true;
	}
	else if (CurrentTokenType() == TType::Variable) {
		Eat(TType::Variable);

		/*Semantic action*/


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
	if (CurrentTokenType() == TType::Int) {
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

bool Parser::Parse_Functions()
{
	return false;
}

bool Parser::Parse_ConstantList()
{
	return false;
}

bool Parser::Parse_ValueList()
{
	return false;
}

bool Parser::Parse_IDList()
{
	return false;
}

bool Parser::Parse_PrintList()
{
	return false;
}

bool Parser::Parse_Remark()
{
	return false;
}

bool Parser::Parse_AndExp()
{
	return false;
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
