#ifndef XASM_XASM_H_
#define XASM_XASM_H_

#include "Structure.h"

namespace xasm
{
	class XASM
	{
	private:
		void InitInstrTable();
		int AddInstrLookup(const std::string & name, InstrType OpCode, int OpCount);
		void SetOpType(int iInstrIndex, int iOpIndex, OpType iOpType);
	private:
		LookupTable table_;
	};
}

#endif
