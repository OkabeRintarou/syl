#ifndef _ERROR_H_
#define _ERROR_H_

#include <exception>
#include <stdexcept>
#include <string>

#define _STRING(X)  #X
#define STRING(X) _STRING(X)
#define FILE_LINE "[" __FILE__ ":" STRING(__LINE__) " ]"
#define THROW_BT_EXCEPTION(MSG) do{\
	throw BTPaserException((std::string(FILE_LINE) + MSG).c_str()); }while (0); 

class BTPaserException :public std::runtime_error
{
public:
	explicit BTPaserException(const std::string & msg)
		:std::runtime_error(msg){}
};

#endif