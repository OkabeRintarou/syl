#ifndef XASM_SCANNER_H_
#define XASM_SCANNER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Token.h"

namespace xasm
{
	class Scanner
	{
	public:
		Scanner();
		~Scanner();
		void LoadFile(const std::string & filename);
		void CloseFile();
		std::string GetString()const{ return iden_; }
		int GetIntegerValue()const{ return int_val_; }
		float GetFloatValue()const{ return float_val_; }
		Token GetNextToken();
		Token GetToken()const{ return token_; }
		TokenLocation GetTokenLocation(){ return token_.GetTokenLocation(); }
		const std::string & GetCurrentLine()const{ return strings_.at(line_ - 1); }
		const std::string GetLineString(int line)const{ return strings_.at(line - 1); }
		const std::string & GetFileName()const{ return filename_; }
	private:
		inline void Advance();
		inline char Peek();
		void Preprocess();
		void SkipWithoutNewline();
		void HandleComment();
		Token HandleNewline();
		Token HandleColon();
		Token HandleOpenBracket();
		Token HandleCloseBracket();
		Token HandleOpenBrace();
		Token HandleCloseBrace();
		Token HandleComma();
		Token HandleStringLiteral();
		Token HandleDigitLiteral();
		Token HandleIdentifier();
		inline bool IsEscapeCharacter();
		Token MakeToken(TokenType type, const TokenLocation & loc, std::string & str);
		Token MakeToken(TokenType type, const TokenLocation & loc, int val);
		Token MakeToken(TokenType type, const TokenLocation & loc, float val);
		inline void ToLower();
	private:
		static std::vector<std::string> instructions;
	private:
		std::vector<std::string> strings_;
		std::fstream input_;
		std::string buffer_;
		std::string filename_;

		Token token_;
		std::string iden_;
		int int_val_;
		float float_val_;

		long line_;
		long column_;
		long prev_line_;
		long prev_column_;
		char cur_char_;
	};
}

#endif