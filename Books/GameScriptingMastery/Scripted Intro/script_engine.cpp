#include <cctype>
#include <cassert>
#include <iostream>
#include <map>
#include <algorithm>
#include "script_engine.h"
#include "cbl_exception.h"

namespace cbl{
	/* ScriptCommand class implement */
	void ScriptCommand::Clear()
	{
		this->command_type_ = CommandType::COMMAND_UNKNOW;
		this->tokens_.clear();
	}

	/* ScriptEngine class implement */
	std::vector<std::string> ScriptEngine::lookup_ = 
	{
		"DrawBitmap", "PlaySound", "Pause", "WaitForKeyPress",
		"FoldCloseEffectX", "FoldCloseEffectY", "Exit"
	};

	ScriptEngine::ScriptEngine()
	{
		buffer_.resize(256);
		buffer_.clear();
	}

	void ScriptEngine::SkipWhiteSpace()
	{
		while (std::isspace(cur_char_)){
			Advance();
		}
	}

	void ScriptEngine::SkipWhiteSpaceWithoutNewline()
	{
		while (cur_char_ != '\n' && std::isspace(cur_char_)){
			Advance();
		}
	}

	void ScriptEngine::Advance()
	{
		cur_char_ = input_.get();
	}

	char ScriptEngine::Peek()
	{
		return input_.peek();
	}

	void ScriptEngine::LoadScript(const std::string & filename)
	{
		input_.open(filename);
		if (input_.fail()){
			throw file_open_failed_exception{ filename + std::string(" open failed.") };
		}

		Advance();
	}

	void ScriptEngine::UnloadScript()
	{
		if (input_.is_open()){
			input_.close();
		}
	}

	bool ScriptEngine::Lookup(const std::string & str)
	{
		return std::find(lookup_.begin(), lookup_.end(), str) != lookup_.end();
	}

	void ScriptEngine::HandleCommand()
	{
		while (std::isalpha(cur_char_)){
			buffer_.push_back(cur_char_);
			Advance();
		}

		if (!Lookup(buffer_)){
			throw unknow_command_exception{ std::string("unknow command : ") + buffer_ };
		}
	}

	void ScriptEngine::HandleStringLiteral()
	{	
		Advance(); // eat '"'
		
		while (cur_char_ != '"'){
			buffer_.push_back(cur_char_);
			Advance();
		}
				
		Advance(); // eat '"'
	}

	void ScriptEngine::HandleIntegerLiteral()
	{
		while (std::isdigit(cur_char_)){
			buffer_.push_back(cur_char_);
			Advance();
		}
	}

	ScriptCommand ScriptEngine::GetNextCommand()
	{
		static std::map<std::string,CommandType> commandMap = 
		{
			{ "DrawBitmap"         , CommandType::COMMAND_DRAWBITMAP },
			{ "PlaySound"          , CommandType::COMMAND_PLAYSOUND },
			{ "Pause"              , CommandType::COMMAND_PAUSE },
			{ "WaitForKeyPress"    , CommandType::COMMAND_WAITFORKEYPRESS },
			{ "FoldCloseEffectX"   , CommandType::COMMAND_FOLDCLOSEEFFECTX },
			{ "FoldCloseEffectY"   , CommandType::COMMAND_FOLDCLOSEEFFECTY },
			{ "Exit"               , CommandType::COMMAND_EXIT }
		};

		if (input_.eof()){
			return ScriptCommand(CommandType::COMMAND_UNKNOW);
		}

		CommandType type;
		std::vector<Token> tokens;

		SkipWhiteSpaceWithoutNewline();
		HandleCommand();
		type = commandMap[buffer_];
		buffer_.clear();

		while (cur_char_ != '\n'){
			SkipWhiteSpaceWithoutNewline();
			if (cur_char_ == '"'){
				HandleStringLiteral();
				if (!buffer_.empty()){
					tokens.push_back(Token(TokenType::TOKEN_STRING, buffer_));
					buffer_.clear();
				}
				
			}
			
			SkipWhiteSpaceWithoutNewline();
			if (std::isdigit(cur_char_)){
				HandleIntegerLiteral();
				if (!buffer_.empty()){
					tokens.push_back(Token(TokenType::TOKEN_INTEGER, std::stol(buffer_, 0, 10)));
					buffer_.clear();
				}		
			}	
		}

		Advance(); // eat '\n'
		return ScriptCommand(type, tokens);
	}
}