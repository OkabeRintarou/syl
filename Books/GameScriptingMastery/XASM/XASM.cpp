#include <iostream>
#include "Scanner.h"
#include "Token.h"
#include "SyntaxParser.h"
#include "XASM.h"

using namespace xasm;

int main()
{
	/*Scanner scanner;
	scanner.LoadFile("test_0.xasm");
	Token token = scanner.GetNextToken();
	while (token.GetType() != TokenType::TOKEN_TYPE_END_OF_FILE){
		TokenType type = token.GetType();

		if (type == TokenType::TOKEN_TYPE_NEWLINE){
			std::cout << "newline" << scanner.GetTokenLocation().GetLine() << "," << scanner.GetTokenLocation().GetColumn() << std::endl;
			token = scanner.GetNextToken();
			continue;
		}
		else if (type == TokenType::TOKEN_TYPE_INT){
			std::cout << token.GetInteger() << "  " << scanner.GetTokenLocation().GetLine() << "," << scanner.GetTokenLocation().GetColumn() << std::endl;
		}
		else if (type == TokenType::TOKEN_TYPE_FLOAT){
			std::cout << token.GetFloat() << std::endl;
		}
		else if (type == TokenType::TOKEN_TYPE_COMMA){
			std::cout << "," << std::endl;
		}
		else if (type == TokenType::TOKEN_TYPE_OPEN_BRACKET){
			std::cout << '[' << std::endl;
		}
		else if (type == TokenType::TOKEN_TYPE_CLOSE_BRACKET){
			std::cout << ']' << std::endl;
		}
		else if (type == TokenType::TOKEN_TYPE_CLOSE_BRACE){
			std::cout << '}' << std::endl;
		}
		else if (type == TokenType::TOKEN_TYPE_OPEN_BRACE){
			std::cout << '{' << std::endl;
		}
		else{
			std::cout << token.GetString() << std::endl;
		}

		token = scanner.GetNextToken();
	}*/

	xasm::SyntaxParser parser;
	parser.BeginParse("test_0.xasm");
	parser.Parse();
	parser.BuildXSE();

	return 0;
}