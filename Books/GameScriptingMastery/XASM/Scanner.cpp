#include <cctype>
#include <algorithm>
#include "Token.h"
#include "Scanner.h"
#include "Exception.h"

namespace xasm
{
	std::vector<std::string> Scanner::instructions = 
	{
		"mov", "add", "sub", "mul", "div",
		"mod", "exp", "neg", "inc", "dec",
		"and", "or", "xor", "not", "shl",
		"shr", "concat", "getchar", "setchar",
		"jmp", "je", "jne", "jg", "jge", "jle",
		"push", "pop", "call", "ret", "callhost",
		"pause", "exit"
	};

	Scanner::Scanner()
	{
		line_ = 1;
		column_ = 0;
		prev_line_ = 1;
		prev_column_ = 0;
	}

	Scanner::~Scanner()
	{
		if (input_.is_open()){
			input_.close();
		}
		filename_.clear();
	}

	void Scanner::LoadFile(const std::string & filename)
	{
		filename_ = filename;
		input_.open(filename);
		if (input_.fail()){
			throw xasm::file_open_failed_exception{ std::string("open file ") + filename + " failed." };
		}
		std::string line;
		while (!input_.eof()){
			std::getline(input_, line);
			strings_.push_back(line);
		}
		CloseFile();
		input_.open(filename);
		if (input_.fail()){
			throw xasm::file_open_failed_exception{ std::string("open file ") + filename + " failed." };
		}

		/*std::getline(input_, line_buffer_);
		line_buffer_.push_back('\n');*/
		Advance();
	}

	void Scanner::CloseFile()
	{
		if (input_.is_open()){
			input_.close();
		}
	}

	void Scanner::Advance()
	{
		cur_char_ = input_.get();
		if (cur_char_ == '\n'){
			prev_line_ = line_;
			prev_column_ = column_;
			++line_;
			column_ = 0;
		}
		else{
			prev_column_ = column_;
			column_++;
		}
	}

	
	char Scanner::Peek()
	{
		return input_.peek();
	}

	void Scanner::Preprocess()
	{
		do 
		{
			SkipWithoutNewline();
			HandleComment();
		} while (cur_char_ !='\n' && std::isspace(cur_char_));
	}
	
	void Scanner::SkipWithoutNewline()
	{
		while (cur_char_ != '\n' && std::isspace(cur_char_)){
			Advance();
		}
	}

	void Scanner::HandleComment()
	{
		if (cur_char_ == ';'){
			while (cur_char_ != '\n'){
				Advance();
			}
		}
	}

	Token Scanner::GetNextToken()
	{
		Preprocess();

		TokenLocation loc(filename_, line_, column_);

		if (input_.eof()){
			return MakeToken(TokenType::TOKEN_TYPE_END_OF_FILE,loc,-1);
		}
		if (cur_char_ == '\n'){
			return HandleNewline();
		}
		if (cur_char_ == ':'){
			return HandleColon();
		}
		if (cur_char_ == '['){
			return HandleOpenBracket();
		}
		if (cur_char_ == ']'){
			return HandleCloseBracket();
		}
		if (cur_char_ == ','){
			return HandleComma();
		}
		if (cur_char_ == '{'){
			return HandleOpenBrace();
		}
		if (cur_char_ == '}'){
			return HandleCloseBrace();
		}

		if (cur_char_ == '"'){
			return HandleStringLiteral();
		}
		else if (std::isalpha(cur_char_) || cur_char_ == '_'){
			return HandleIdentifier();
		}
		else if (std::isdigit(cur_char_)){
			return HandleDigitLiteral();
		}

		return Token(TokenType::TOKEN_TYPE_INVALID);
	}

	Token Scanner::HandleNewline()
	{
		TokenLocation loc(filename_, prev_line_, prev_column_);

		Advance(); // eat '\n'
		buffer_.push_back('\n');
		return MakeToken(TokenType::TOKEN_TYPE_NEWLINE, loc, buffer_);
	}

	Token Scanner::HandleColon()
	{
		TokenLocation loc(filename_, line_, column_);

		Advance(); // eat ':'
		buffer_.push_back(':');
		return MakeToken(TokenType::TOKEN_TYPE_COLON, loc, buffer_);
	}

	Token Scanner::HandleOpenBracket()
	{
		TokenLocation loc(filename_, line_, column_);

		Advance(); // eat '['
		buffer_.push_back('[');
		return MakeToken(TokenType::TOKEN_TYPE_OPEN_BRACKET, loc, buffer_);
	}

	Token Scanner::HandleCloseBracket()
	{
		TokenLocation loc(filename_, line_, column_);

		Advance(); // eat ']'
		buffer_.push_back(']');
		return MakeToken(TokenType::TOKEN_TYPE_CLOSE_BRACKET, loc, buffer_);
	}

	Token Scanner::HandleOpenBrace()
	{
		TokenLocation loc(filename_, line_, column_);

		Advance(); // eat '{'
		buffer_.push_back('{');
		return MakeToken(TokenType::TOKEN_TYPE_OPEN_BRACE, loc, buffer_);
	}

	Token Scanner::HandleCloseBrace()
	{
		TokenLocation loc(filename_, line_, column_);

		Advance(); // eat '}'
		buffer_.push_back('}');
		return MakeToken(TokenType::TOKEN_TYPE_CLOSE_BRACE, loc, buffer_);
	}

	Token Scanner::HandleComma()
	{
		TokenLocation loc(filename_, line_, column_);

		Advance(); // eat ','
		buffer_.push_back(',');
		return MakeToken(TokenType::TOKEN_TYPE_COMMA, loc, buffer_);
	}

	Token Scanner::HandleStringLiteral()
	{
		TokenLocation loc(filename_, line_, column_);

		Advance(); // eat '"'
		while (cur_char_ != '"'){
			if (cur_char_ == '\\'){
				Advance();
				if (IsEscapeCharacter()){
					buffer_.push_back(cur_char_);
					Advance();
				}
			}
			else{
				buffer_.push_back(cur_char_);
				Advance();
			}
			
		}
		Advance(); // eat '"'

		return MakeToken(TokenType::TOKEN_TYPE_STRING, loc, buffer_);
	}

	Token Scanner::HandleIdentifier()
	{
		TokenLocation loc(filename_, line_, column_);

		buffer_.push_back(cur_char_);
		Advance();

		while (std::isalnum(cur_char_) || cur_char_ == '_')
		{
			buffer_.push_back(cur_char_);
			Advance();
		}

		ToLower();

		if (instructions.end() != std::find(instructions.begin(), instructions.end(), buffer_)){
			return MakeToken(TokenType::TOKEN_TYPE_INSTR, loc, buffer_);
		}
		else if (buffer_ == "var"){
			return MakeToken(TokenType::TOKEN_TYPE_VAR,loc,buffer_);
		}
		else if (buffer_ == "func"){
			return MakeToken(TokenType::TOKEN_TYPE_FUNC, loc, buffer_);
		}
		else if (buffer_ == "param"){
			return MakeToken(TokenType::TOKEN_TYPE_PARAM, loc, buffer_);
		}
		else if (buffer_ == "_retval"){
			return MakeToken(TokenType::TOKEN_TYPE_REG_RETVAL, loc, buffer_);
		}
		else if (buffer_ == "setstacksize"){
			return MakeToken(TokenType::TOKEN_TYPE_SETSTACKSIZE, loc, buffer_);
		}
		else {
			return MakeToken(TokenType::TOKEN_TYPE_IDENT, loc, buffer_);
		}
	}

	Token Scanner::HandleDigitLiteral()
	{
		TokenLocation loc(filename_, line_, column_);

		buffer_.push_back(cur_char_);
		Advance();

		while (std::isdigit(cur_char_)){
			buffer_.push_back(cur_char_);
			Advance();
		}
		if (cur_char_ == '.'){
			buffer_.push_back(cur_char_);
			Advance();
			while (std::isdigit(cur_char_)){
				buffer_.push_back(cur_char_);
				Advance();
			}
			float val = std::stof(buffer_);
			return  MakeToken(TokenType::TOKEN_TYPE_FLOAT, loc, val);
		}
		else{
			int val = std::stol(buffer_, 0, 10);
			return MakeToken(TokenType::TOKEN_TYPE_INT, loc, val);
		}
	}

	bool Scanner::IsEscapeCharacter()
	{
		return cur_char_ == '\"' ||
			cur_char_ == '\\' ||
			cur_char_ == 'n';
	}

	Token  Scanner::MakeToken(TokenType type, const TokenLocation & loc, std::string & str)
	{
		token_ = Token(type, loc, str);
		buffer_.clear();
		return token_;
	}

	Token Scanner::MakeToken(TokenType type, const TokenLocation & loc, int val)
	{
		token_ = Token(type, loc, val);
		buffer_.clear();
		return token_;
	}

	Token Scanner::MakeToken(TokenType type, const TokenLocation & loc, float val)
	{
		token_ = Token(type, loc, val);
		buffer_.clear();
		return token_;
	}

	void Scanner::ToLower()
	{
		std::transform(buffer_.begin(), buffer_.end(), buffer_.begin(), std::tolower);
	}
}