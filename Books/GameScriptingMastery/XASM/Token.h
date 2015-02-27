#ifndef XASM_TOKEN_H_
#define XASM_TOKEN_H_

#include <vector>

namespace xasm
{
	enum class TokenType
	{
		TOKEN_TYPE_INT,             // integer literal:1,2,3
		TOKEN_TYPE_FLOAT,			// float literal:3.14
		TOKEN_TYPE_STRING,			// string literal:"hello,world"
		TOKEN_TYPE_QUOTE,			// " 
		TOKEN_TYPE_IDENT,           // identifier
		TOKEN_TYPE_COLON,			// :
		TOKEN_TYPE_OPEN_BRACKET,    // [
		TOKEN_TYPE_CLOSE_BRACKET,   // ]
		TOKEN_TYPE_COMMA,			// ,
		TOKEN_TYPE_OPEN_BRACE,		// {
		TOKEN_TYPE_CLOSE_BRACE,		// }
		TOKEN_TYPE_NEWLINE,			// '\n'
		TOKEN_TYPE_INSTR,			// instruction
		TOKEN_TYPE_SETSTACKSIZE,	// SetStackSize indicator
		TOKEN_TYPE_VAR,				// Var indicator
		TOKEN_TYPE_FUNC,			// Func indicator
		TOKEN_TYPE_PARAM,			// Param indicator
		TOKEN_TYPE_REG_RETVAL,		// _RetVal register
		TOKEN_TYPE_INVALID,			// invalid indicator
		TOKEN_TYPE_END_OF_FILE      // end of file
	};

	class TokenLocation
	{
	public:
		TokenLocation()
			:line_(-1), column_(-1){}
		TokenLocation(const std::string & file, int line, int column)
			:file_(file), line_(line), column_(column){}
		const std::string & GetFileName()const{ return file_; }
		std::string GetFileName(){ return file_; }
		long GetLine()const{ return line_; }
		long GetLine(){ return line_; }
		long GetColumn()const{ return column_; }
		long GetColumn(){ return column_; }
	private:
		std::string file_;
		long line_;
		long column_;
	};

	class Token
	{
	public:
		Token(TokenType type = TokenType::TOKEN_TYPE_INVALID)
			:type_(type){}
		Token(TokenType type, TokenLocation loc, std::string & str)
			:type_(type), loc_(loc), str_(str){}
		Token(TokenType type, TokenLocation loc, int val)
			:type_(type), loc_(loc), int_val_(val){}
		Token(TokenType type,TokenLocation loc,float val)
			:type_(type), loc_(loc), float_val_(val){}
		const TokenLocation & GetTokenLocation()const{ return loc_; }
		TokenLocation GetTokenLocation(){ return loc_; }
		TokenType GetType()const{ return type_;}
		std::string GetString()const{ return str_; }
		int GetInteger()const{ return int_val_; }
		float GetFloat()const{ return float_val_; }
	private:
		TokenType type_;
		TokenLocation loc_;
		std::string str_;
		int int_val_;
		float float_val_;
	};
}
#endif