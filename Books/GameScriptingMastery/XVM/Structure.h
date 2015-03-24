#ifndef _XVM_STRUCTURE_H_
#define _XVM_STRUCTURE_H_

#include <vector>

namespace xvm
{
	enum OpType
	{
		OP_TYPE_INT,
		OP_TYPE_FLOAT,
		OP_TYPE_STRING_INDEX,
		OP_TYPE_ABS_STACK_INDEX,
		OP_TYPE_REL_STACK_INDEX,
		OP_TYPE_INSTR_INDEX,
		OP_TYPE_FUNC_INDEX,
		OP_TYPE_HOST_API_CALL_INDEX,
		OP_TYPE_REG,
		OP_TYPE_STRING_LITERAL,
	};

	struct Value
	{
		int Type;
		union{
			int IntLiteral;
			float FloatLiteral;
			char * StringLiteral;
			int StackIndex;
			int InstrIndex;
			int FuncIndex;
			int HostAPICallIndex;
			int Reg;
		};

		int OffsetIndex;
	};

	struct Instruction
	{
		int OpCode;
		int OpCount;
		std::vector<Value> OpList;
	};

	struct InstructionStream
	{
		std::vector<Instruction> Instructions;
		int Size; // ָ������
		int CurrInstr;
	};

	struct RuntimeStack
	{
		std::vector<Value> Elements;
		int Size;
		int TopIndex;
		int FrameIndex;
	};

	struct Function
	{
		int EntryPoint;
		int ParamCount;
		int LocalDataSize;
		int StackFrameSize;
	};

	struct Script
	{
		// ͷ����
		int GlobalDataSize; //�ű�ȫ�����ݴ�С

		// ����
		int IsMainFuncPresent;
		int MainFuncIndex;
		int IsPaused;
		int PausedEndTime;

		// �Ĵ���
		Value _RetVal;

		// �ű�����
		InstructionStream InstrStream;
		RuntimeStack Stack;
		std::vector<Function> FunctionTable;
	};
}
#endif