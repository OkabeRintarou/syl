#ifndef XASM_EXCEPTION_H_
#define XASM_EXCEPTION_H_

#include <string>
#include <exception>

namespace xasm
{
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

	class parse_exception :public std::exception
	{
	public:
		parse_exception(const std::string & msg)
			:exception(msg.c_str()){}
		parse_exception(const char * msg)
			:exception(msg){}
	};
}

#endif
