#ifndef XASM_STRUCTURE_H_
#define XASM_STRUCTURE_H_

#include <string>

namespace xasm
{
	class FuncNode
	{
	public:

	private:
		int index_;
		std::string name_;
		int entry_point_;
		int param_count_;
		int local_data_size_;
	};

	class SymbolNode
	{
	private:
		int index_;
		std::string name_;
		int size_;
		int stack_index_;
		int func_index_;
	};

	class LabelNode
	{
	private:
		int index_;
		std::string name_;
		int target_index_;
		int func_index_;
	};

	class InstrLookup
	{
	private:
		std::string name_;
		int op_code_;
		int op_count_;

	};
}

#endif