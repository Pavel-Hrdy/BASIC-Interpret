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
#include "..\cpp_basic_interpret\icvm.h"
#include "..\cpp_basic_interpret\icvm.cpp"
#include "..\cpp_basic_interpret\parser.h"
#include "..\cpp_basic_interpret\parser.cpp"
#include "..\cpp_basic_interpret\icvm_instructions.h"
#include "..\cpp_basic_interpret\icvm_instructions.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace basic_interpret_tests
{

	TEST_CLASS(LexerTests)
	{
	public:

		TEST_METHOD(EmptyCodeToken)
		{
			std::string code = "10";
			Lexer l(code);
			Token lineNumber = l.GetToken();
			Token t = l.GetToken();
			TokenType * token = t.GetTokenType();

			Assert::AreEqual((int)TType::Int, (int)lineNumber.GetTokenType()->Type());
			Assert::AreEqual((int)TType::EndOfCode, (int)token->Type());
		}

		TEST_METHOD(StringVariableToken)
		{
			std::string code = "10 test$";

			Lexer l(code);
			Token lineNumber = l.GetToken();
			Token t = l.GetToken();
			TokenType * token = t.GetTokenType();

			Assert::AreEqual((int)TType::Int, (int)lineNumber.GetTokenType()->Type());
			Assert::AreEqual((int)TType::StringVariable, (int)token->Type());
		}

		TEST_METHOD(VariableToken)
		{
			std::string code = "10 test";

			Lexer l(code);
			Token lineNumber = l.GetToken();
			Token t = l.GetToken();
			TokenType * token = t.GetTokenType();

			Assert::AreEqual((int)TType::Int, (int)lineNumber.GetTokenType()->Type());
			Assert::AreEqual((int)TType::Variable, (int)token->Type());
		}

		TEST_METHOD(IntToken)
		{
			std::string code = "10 50";

			Lexer l(code);
			Token lineNumber = l.GetToken();
			Token t = l.GetToken();
			TokenType * token = t.GetTokenType();

			Assert::AreEqual((int)TType::Int, (int)lineNumber.GetTokenType()->Type());
			Assert::AreEqual((int)TType::Int, (int)token->Type());
		}

		TEST_METHOD(Real)
		{
			std::string code = "10 50.86";

			Lexer l(code);
			Token lineNumber = l.GetToken();
			Token t = l.GetToken();
			TokenType * token = t.GetTokenType();

			Assert::AreEqual((int)TType::Int, (int)lineNumber.GetTokenType()->Type());
			Assert::AreEqual((int)TType::Real, (int)token->Type());
		}

		TEST_METHOD(RealToken)
		{
			std::string code = "10 50.86";

			Lexer l(code);
			Token lineNumber = l.GetToken();
			Token t = l.GetToken();
			TokenType * token = t.GetTokenType();

			Assert::AreEqual((int)TType::Int, (int)lineNumber.GetTokenType()->Type());
			Assert::AreEqual((int)TType::Real, (int)token->Type());
		}

		TEST_METHOD(RealToken_TestParse)
		{
			std::string code = "10 50.86";

			Lexer l(code);
			Token lineNumber = l.GetToken();
			Token t = l.GetToken();

			Assert::AreEqual((int)TType::Int, (int)lineNumber.GetTokenType()->Type());
			Assert::AreEqual(50.86, std::stod(t.GetContent()));
		}

		TEST_METHOD(ComplexTest_CorrectInput)
		{
			std::string code = "10 PRINT X,Y,Z,A$\n20 100 PRINT\"THE VALUE OF X IS \";X";
			std::vector<TType> types;
			Lexer l(code);

			while (true) {
				Token t = l.GetToken();
				if (t.GetTokenType()->Type() == TType::EndOfCode) break;
				types.push_back(t.GetTokenType()->Type());
			}
			Assert::IsTrue(types.size() == 16);
			Assert::IsTrue(types[0] == TType::Int);
			Assert::IsTrue(types[1] == TType::Function);
			Assert::IsTrue(types[2] == TType::Variable);
			Assert::IsTrue(types[3] == TType::Comma);
			Assert::IsTrue(types[4] == TType::Variable);
			Assert::IsTrue(types[5] == TType::Comma);
			Assert::IsTrue(types[6] == TType::Variable);
			Assert::IsTrue(types[7] == TType::Comma);
			Assert::IsTrue(types[8] == TType::StringVariable);
			Assert::IsTrue(types[9] == TType::NewLine);
			Assert::IsTrue(types[10] == TType::Int);
			Assert::IsTrue(types[11] == TType::Int);
			Assert::IsTrue(types[12] == TType::Function);
			Assert::IsTrue(types[13] == TType::String);
			Assert::IsTrue(types[14] == TType::Semicolon);
			Assert::IsTrue(types[15] == TType::Variable);
		}

		TEST_METHOD(ComplexTest_UnknownCharacter)
		{
			std::string code = "10 PRINT X,Y,Z,A$\n20 100 PRINT\"THE VALUE OF X IS \";X????";
			std::vector<TType> types;
			Lexer l(code);

			try {
				while (true) {
					Token t = l.GetToken();
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
			std::string code = "10 PRINT X,Y,Z,A$\n20 100 PRINT\"THE VALUE \nOF X IS \";X";
			std::vector<TType> types;
			Lexer l(code);

			try {
				while (true) {
					Token t = l.GetToken();
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
			std::string code = "10 PRINT X,Y,Z,A$\n20 100 PRINT\"THE VALUE OF X IS ;X";
			std::vector<TType> types;
			Lexer l(code);

			try {
				while (true) {
					Token t = l.GetToken();
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

		TEST_METHOD(MissingLineNumberAtTheBeginning) {
			std::string code = "PRINT X,Y,Z,A$\n20 100 PRINT\"THE VALUE OF X IS ;X";
			Lexer l(code);
			try {
				l.GetToken();
			}
			catch (LineNumberNotFoundException) {
				Assert::IsTrue(true);
			}
			catch (std::exception) { Assert::IsFalse(true); }
		}

		TEST_METHOD(WrongLineNumberAtTheBeginning) {
			std::string code = "20 PRINT X,Y,Z,A$\n20 100 PRINT\"THE VALUE OF X IS ;X";
			Lexer l(code);
			try {
				l.GetToken();
			}
			catch (WrongLineNumberException) {
				Assert::IsTrue(true);
			}
			catch (std::exception) { Assert::IsFalse(true); }
		}
	};
}