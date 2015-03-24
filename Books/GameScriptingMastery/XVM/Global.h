#ifndef _XVM_GLOBAL_H_
#define _XVM_GLOBAL_H_

#include <string>

namespace xvm
{
	const int VERSION_MAJOR = 1;
	const int VERSION_MINOR = 0;

	const int MAX_INSTR_LOOKUP_COUNT = 256;
	const int DEFAULT_STACKSIZE = 512;

	const std::string MAIN_FUNC_NAME =
		"_main";
	const std::string XSE_ID_STRING =
		"XSE0";

	enum LoadState
	{
		LOAD_ERROR_INVALID_XSE,
		LOAD_ERROR_UNSUPPORTED_VERSION,
		LOAD_OK,
	};
}
#endif