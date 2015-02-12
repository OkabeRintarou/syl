/**
* @file ConsoleCBL.cpp
* @brief 
*   A Simple command-based language,its syntax is as follows:
*   ----command-----------parameter--------description-----------------------------------
*   |  PrintString        string           output specific string
*   |  PrintStringLoop    string,integer   output specific string and the number of times
*   |  Newline            none             output a blank line
*   |  WaitForKeyPress    none             be suspended until a key is pressed
*   -------------------------------------------------------------------------------------
* @author sylcoder
* @version 1.0
* @date 2015/2/12
*/

#include <cctype>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <exception>

class file_open_failed_exception :public std::exception
{
public:
	file_open_failed_exception(const std::string & msg)
		:exception(msg.c_str()){}
	file_open_failed_exception(const char * msg)
		:exception(msg){}
};

class unknow_command_exception :public std::exception
{
public:
	unknow_command_exception(const std::string & msg)
		:exception(msg.c_str()){}
	unknow_command_exception(const char * msg)
		:exception(msg){}
};

class ConsoleCBL
{
public:
	ConsoleCBL();
	void LoadScript(const std::string & filename);
	void RunScript();
private:
	void Advance();
	char Peek();
	void Preprocess();
	void HandleCommand();
	void HandleStringLiteral();
	void HandleIntegerLiteral();
	bool IsInteger(const std::string & str);
	void HandleLine();
private:
	static std::vector<std::string> command_;
private:
	std::fstream input_;
	std::vector<std::vector<std::string>> buffer_;
	std::vector<std::string> chain_;
	std::string dummy_;
	char cur_char_;
};

std::vector<std::string> ConsoleCBL::command_ =
{
	"PrintString",
	"PrintStringLoop",
	"Newline",
	"WaitForKeyPress"
};

ConsoleCBL::ConsoleCBL()
{
	dummy_.resize(256);
	dummy_.clear();
	cur_char_ = -1;
}

void ConsoleCBL::Advance()
{
	cur_char_ = input_.get();
}

char ConsoleCBL::Peek()
{
	return input_.peek();
}

bool ConsoleCBL::IsInteger(const std::string & str)
{
	for (const char & c : str){
		if (!std::isdigit(c)){
			return false;
		}
	}
	return true;
}

void ConsoleCBL::Preprocess()
{
	if (cur_char_ != '\n'){
		while (std::isspace(cur_char_)){
			Advance();
		}
	}
}

void ConsoleCBL::HandleCommand()
{
	while (cur_char_ != '\n' && std::isalpha(cur_char_)){
		dummy_.push_back(cur_char_);
		Advance();
	}

	if (std::find(command_.begin(), command_.end(), dummy_) == command_.end()){
		throw unknow_command_exception{ std::string("Unknow command ") + dummy_ };
	}
	chain_.push_back(dummy_);
	dummy_.clear();
}

void ConsoleCBL::HandleStringLiteral()
{
	Advance(); // eat '"'
	while (cur_char_ != '"'){
		dummy_.push_back(cur_char_);
		Advance();
	}
	chain_.push_back(dummy_);
	dummy_.clear();

	Advance(); // eat '"'
}

void ConsoleCBL::HandleIntegerLiteral()
{
	while (std::isdigit(cur_char_))
	{
		dummy_.push_back(cur_char_);
		Advance();
	}
	chain_.push_back(dummy_);
	dummy_.clear();

}

void ConsoleCBL::HandleLine()
{
	Preprocess();
	HandleCommand();

	while (cur_char_ != '\n'){
		Preprocess();
		HandleStringLiteral();
		Preprocess();
		HandleIntegerLiteral();
	}
	buffer_.push_back(chain_);
	chain_.clear();
	Advance(); // eat '\n'
}

void ConsoleCBL::LoadScript(const std::string & filename)
{
	input_.open(filename);
	if (input_.fail()){
		throw file_open_failed_exception{ std::string("open file ") + filename + " failed." };
	}

	std::string buf;
	buf.resize(256);

	Advance();

	while (true){
		if (input_.eof()){
			break;
		}
		else{
			HandleLine();
		}
	}

	input_.close();
}

void ConsoleCBL::RunScript()
{
	for (auto & chain : buffer_){
		if (chain.at(0) == "PrintString"){
			std::cout << chain.at(1) << std::endl;
		}
		else if (chain.at(0) == "PrintStringLoop"){
			if (IsInteger(chain.at(2))){
				int loop = std::stol(chain.at(2), 0, 10);
				for (int i = 0; i < loop; ++i){
					std::cout << i << ": " << chain.at(1) << std::endl;
				}
			}
		}
		else if (chain.at(0) == "Newline"){
			std::cout << std::endl;
		}
		else if (chain.at(0) == "WaitForKeyPress"){
			std::getchar();
		}
	}
}

int main(int argc, char * argv[])
{
	ConsoleCBL demo;
	try{
		demo.LoadScript("demo.cbl");
	}
	catch (file_open_failed_exception & e){
		std::cerr << e.what() << std::endl;
	}
	catch (unknow_command_exception & e){
		std::cerr << e.what() << std::endl;
	}
	demo.RunScript();
	return 0;
}