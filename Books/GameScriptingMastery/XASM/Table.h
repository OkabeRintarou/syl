#ifndef XASM_TABLE_H_
#define XASM_TABLE_H_

#include <vector>
#include <string>
#include "Structure.h"

namespace xasm
{
	class Table
	{
	private:
		std::vector<std::string> string_table_;
		std::vector<FuncNode*> function_table_;

	};
}

#endif