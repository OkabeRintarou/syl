#include <iostream>
#include "Scanner.h"
#include "Token.h"

using namespace xasm;

int main()
{
	Scanner scanner;
	scanner.LoadFile("test_0.xasm");
	Token token = scanner.GetNextToken();
	while (token.GetType() != TokenType::TOKEN_TYPE_END_OF_FILE){
		TokenType type = token.GetType();

		if (type == TokenType::TOKEN_TYPE_NEWLINE){
			token = scanner.GetNextToken();
			continue;
		}
		else if (type == TokenType::TOKEN_TYPE_INT){
			std::cout << token.GetInteger() << std::endl;
		}
		else if (type == TokenType::TOKEN_TYPE_FLOAT){
			std::cout << token.GetFloat() << std::endl;
		}
		else{
			std::cout <<":"<< token.GetString() << std::endl;
		}
		
		token = scanner.GetNextToken();
	}
	return 0;
}