#ifndef CBL_EXCEPTION_H_
#define CBL_EXCEPTION_H_

#include <exception>
#include <string>

namespace cbl{
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

	class window_create_failed_exception :public std::exception
	{
	public:
		window_create_failed_exception(const std::string & msg)
			:exception(msg.c_str()){}
		window_create_failed_exception(const char * msg)
			:exception(msg){}
	};

	class renderer_create_failed_exception :public std::exception
	{
	public:
		renderer_create_failed_exception(const std::string & msg)
			:exception(msg.c_str()){}
		renderer_create_failed_exception(const char * msg)
			:exception(msg){}
	};
}
#endif