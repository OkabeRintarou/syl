#ifndef CBL_TOKEN_H_
#define CBL_TOKEN_H_

#include <string>

namespace cbl{
	enum class CommandType
	{
		COMMAND_UNKNOW,
		COMMAND_DRAWBITMAP,
		COMMAND_PLAYSOUND,
		COMMAND_PAUSE,
		COMMAND_WAITFORKEYPRESS,
		COMMAND_FOLDCLOSEEFFECTX,
		COMMAND_FOLDCLOSEEFFECTY,
		COMMAND_EXIT
	};

	enum class TokenType
	{
		TOKEN_UNKNOW,
		TOKEN_STRING,
		TOKEN_INTEGER,
		TOKEN_END_OF_FILE
	};

	class Token
	{
	public:
		Token(TokenType type, std::string str)
			:type_(type), str_(str), integer_(-1){}
		Token(TokenType type, int value)
			:type_(type), integer_(value){}
		const std::string & GetString()const{ return str_; }
		const int GetValue()const{ return integer_; }
		const TokenType & GetTokenType()const{ return type_; }
	private:
		TokenType type_;
		std::string str_;
		int integer_;
	};

}
#endif