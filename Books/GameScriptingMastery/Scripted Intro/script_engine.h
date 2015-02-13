/**
* @file script_engine.h
* @brief
*   A Simple command-based language,its syntax is as follows:
*   ----command-----------parameter--------description-----------------------------------
*   |  DrawBitmap         string           show specific bitmap on screen
*   |  PlaySound          string           play specific music
*   |  Pause              integer          pause specific time
*   |  WaitForKeyPress    none             be suspended until a key is pressed
*   |  FoldCloseEffectX   none             complete orizontally "fold close" effect
*   |  FoldCloseEffectY   none             complete vertically "fold close" effect
*   |  Exit               none             exit
*   -------------------------------------------------------------------------------------
* @author sylcoder
* @version 1.0
* @date 2015/2/12
*/

#ifndef SCRIPT_ENGINE_H_
#define SCRIPT_ENGINE_H_

#include <fstream>
#include <vector>
#include <string>

#include "cbl_token.h"

namespace cbl{
	class ScriptCommand
	{
	public:
		ScriptCommand()
			:command_type_(CommandType::COMMAND_UNKNOW)
		{}
		explicit ScriptCommand(CommandType type)
			:command_type_(type){}
		ScriptCommand(CommandType type, const std::vector<Token> tokens)
			:command_type_(type), tokens_(tokens){}
		CommandType GetCommandType()const{ return command_type_; }
		std::vector<Token> & GetCommandParameters(){ return tokens_; }
		void Clear();
		explicit operator bool()const{ return CommandType::COMMAND_UNKNOW != command_type_; }
	private:
		CommandType command_type_;
		std::vector<Token> tokens_;
	};

	class ScriptEngine
	{
	public:
		ScriptEngine();
		void LoadScript(const std::string & filename);
		void UnloadScript();
		ScriptCommand GetNextCommand();
	private:
		void SkipWhiteSpace();
		void SkipWhiteSpaceWithoutNewline();
		inline void Advance();
		inline char Peek();

		void HandleCommand();
		void HandleStringLiteral();
		void HandleIntegerLiteral();
		inline bool Lookup(const std::string & str);
	private:
		static std::vector<std::string> lookup_;
	private:
		std::fstream input_;
		char cur_char_;
		std::string buffer_;
	};
}
#endif