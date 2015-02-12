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
}
#endif