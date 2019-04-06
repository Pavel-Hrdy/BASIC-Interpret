#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include "..\cpp_basic_interpret\lexer.h"
#include "..\cpp_basic_interpret\lexer.cpp"
#include "..\cpp_basic_interpret\token.h"
#include "..\cpp_basic_interpret\token.cpp"
#include "..\cpp_basic_interpret\exceptions.cpp"
#include "..\cpp_basic_interpret\exceptions.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace basic_interpret_tests
{

	TEST_CLASS(LexerTests)
	{
	public:

		TEST_METHOD(EmptyCodeToken)
		{
			std::string code = "";

			Lexer l(code);
			Token t = l.GetNextToken();
			TokenType * token = t.GetTokenType();

			Assert::AreEqual((int)TType::EndOfCode, (int)token->Type());
		}

		TEST_METHOD(StringVariableToken)
		{
			std::string code = "test$";

			Lexer l(code);
			Token t = l.GetNextToken();
			TokenType * token = t.GetTokenType();

			Assert::AreEqual((int)TType::StringVariable, (int)token->Type());
		}

		TEST_METHOD(VariableToken)
		{
			std::string code = "test";

			Lexer l(code);
			Token t = l.GetNextToken();
			TokenType * token = t.GetTokenType();

			Assert::AreEqual((int)TType::Variable, (int)token->Type());
		}

		TEST_METHOD(IntToken)
		{
			std::string code = "50";

			Lexer l(code);
			Token t = l.GetNextToken();
			TokenType * token = t.GetTokenType();

			Assert::AreEqual((int)TType::Int, (int)token->Type());
		}

		TEST_METHOD(Real)
		{
			std::string code = "50.86";

			Lexer l(code);
			Token t = l.GetNextToken();
			TokenType * token = t.GetTokenType();

			Assert::AreEqual((int)TType::Real, (int)token->Type());
		}

		TEST_METHOD(RealToken)
		{
			std::string code = "50.86";

			Lexer l(code);
			Token t = l.GetNextToken();
			TokenType * token = t.GetTokenType();

			Assert::AreEqual((int)TType::Real, (int)token->Type());
		}

		TEST_METHOD(RealToken_TestParse)
		{
			std::string code = "50.86";

			Lexer l(code);
			Token t = l.GetNextToken();

			Assert::AreEqual(50.86, std::stod(t.GetContent()));
		}

		TEST_METHOD(ComplexTest_CorrectInput)
		{
			std::string code = "PRINT X,Y,Z,A$\n100 PRINT\"THE VALUE OF X IS \";X";
			std::vector<TType> types;
			Lexer l(code);

			while (true) {
				Token t = l.GetNextToken();
				if (t.GetTokenType()->Type() == TType::EndOfCode) break;
				types.push_back(t.GetTokenType()->Type());
			}
			Assert::IsTrue(types.size() == 13);
			Assert::IsTrue(types[0] == TType::Print_F);
			Assert::IsTrue(types[1] == TType::Variable);
			Assert::IsTrue(types[2] == TType::Comma);
			Assert::IsTrue(types[3] == TType::Variable);
			Assert::IsTrue(types[4] == TType::Comma);
			Assert::IsTrue(types[5] == TType::Variable);
			Assert::IsTrue(types[6] == TType::Comma);
			Assert::IsTrue(types[7] == TType::StringVariable);
			Assert::IsTrue(types[8] == TType::Int);
			Assert::IsTrue(types[9] == TType::Print_F);
			Assert::IsTrue(types[10] == TType::String);
			Assert::IsTrue(types[11] == TType::Semicolon);
			Assert::IsTrue(types[12] == TType::Variable);
		}

		TEST_METHOD(ComplexTest_UnknownCharacter)
		{
			std::string code = "PRINT X,Y,Z,A$\n100 PRINT\"THE VALUE OF X IS \";X????";
			std::vector<TType> types;
			Lexer l(code);

			try {
				while (true) {
					Token t = l.GetNextToken();
					if (t.GetTokenType()->Type() == TType::EndOfCode) { break; };
					types.push_back(t.GetTokenType()->Type());
				}
			}
			catch(UnknownCharacterException){
				Assert::IsTrue(true);
			}
			catch (std::exception) {
				Assert::IsFalse(true);
			}
			
		}

		TEST_METHOD(ComplexTest_NewlineInString)
		{
			std::string code = "PRINT X,Y,Z,A$\n100 PRINT\"THE VALUE \nOF X IS \";X";
			std::vector<TType> types;
			Lexer l(code);

			try {
				while (true) {
					Token t = l.GetNextToken();
					if (t.GetTokenType()->Type() == TType::EndOfCode) break;
					types.push_back(t.GetTokenType()->Type());
				}
			}
			catch (NewlineInStringException) {
				Assert::IsTrue(true);
			}
			catch (std::exception) {
				Assert::IsFalse(true);
			}

		}

		TEST_METHOD(ComplexTest_StringNotTerminated)
		{
			std::string code = "PRINT X,Y,Z,A$\n100 PRINT\"THE VALUE OF X IS ;X";
			std::vector<TType> types;
			Lexer l(code);

			try {
				while (true) {
					Token t = l.GetNextToken();
					if (t.GetTokenType()->Type() == TType::EndOfCode) break;
					types.push_back(t.GetTokenType()->Type());
				}
			}
			catch (StringNotTerminatedException) {
				Assert::IsTrue(true);
			}
			catch (std::exception) {
				Assert::IsFalse(true);
			}

		}
	};
}