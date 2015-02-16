#ifndef XASM_SCANNER_H_
#define XASM_SCANNER_H_

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
	private:
		inline void Advance();
		inline char Peek();
		void Preprocess();
		void SkipWithoutNewline();
		void HandleComment();
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
		std::fstream input_;
		std::string buffer_;
		std::string filename_;

		TokenLocation loc_;
		Token token_;
		std::string iden_;
		int int_val_;
		float float_val_;

		long line_;
		long column_;
		char cur_char_;
	};
}

#endif