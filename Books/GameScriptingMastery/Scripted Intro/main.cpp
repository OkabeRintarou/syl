#include "script_engine.h"
#include <iostream>

using namespace cbl;

int main(int argc,char * argv[])
{
	ScriptEngine script;
	script.LoadScript("intro.cbl");
	while (ScriptCommand command = script.GetNextCommand()){
		CommandType type = command.GetCommandType();

		for (Token & itr : command.GetCommandParameters()){

			CommandType type = command.GetCommandType();

			if (itr.GetTokenType() == TokenType::TOKEN_STRING){
				std::cout << itr.GetString() << std::endl;
			}
			else if (itr.GetTokenType() == TokenType::TOKEN_INTEGER){
				std::cout << itr.GetValue() << std::endl;
			}
		}
	}
	return 0;
}