#ifndef XASM_STRUCTURE_H_
#define XASM_STRUCTURE_H_

#include <string>
#include <vector>
#include "global.h"

namespace xasm
{
	using OpFlagType = int;
#define		OP_FLAG_TYPE_INT			 1
#define		OP_FLAG_TYPE_FLOAT			 2
#define		OP_FLAG_TYPE_STRING			 4
#define		OP_FLAG_TYPE_MEM_REF		 8
#define		OP_FLAG_TYPE_LINE_LABEL		 16
#define		OP_FLAG_TYPE_FUNC_NAME		 32
#define		OP_FLAG_TYPE_HOST_API_CALL	 64
#define		OP_FLAG_TYPE_REG			 128
	/*enum OpFlagType
	{
		OP_FLAG_TYPE_INT           = 1,
		OP_FLAG_TYPE_FLOAT         = 2,
		OP_FLAG_TYPE_STRING        = 4,
		OP_FLAG_TYPE_MEM_REF	   = 8,
		OP_FLAG_TYPE_LINE_LABEL    = 16,
		OP_FLAG_TYPE_FUNC_NAME     = 32,
		OP_FLAG_TYPE_HOST_API_CALL = 64,
		OP_FLAG_TYPE_REG           = 128
	};*/

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
		OP_TYPE_REG
	};

	enum InstrType
	{
		INSTR_MOV,

		INSTR_ADD,
		INSTR_SUB,
		INSTR_MUL,
		INSTR_DIV,
		INSTR_MOD,
		INSTR_EXP,
		INSTR_NEG,
		INSTR_INC,
		INSTR_DEC,

		INSTR_AND,
		INSTR_OR,
		INSTR_XOR,
		INSTR_NOT,
		INSTR_SHL,
		INSTR_SHR,

		INSTR_CONCAT,
		INSTR_GETCHAR,
		INSTR_SETCHAR,

		INSTR_JMP,
		INSTR_JE,
		INSTR_JNE,
		INSTR_JG,
		INSTR_JL,
		INSTR_JGE,
		INSTR_JLE,

		INSTR_PUSH,
		INSTR_POP,

		INSTR_CALL,
		INSTR_RET,
		INSTR_CALLHOST,

		INSTR_PAUSE,
		INSTR_EXIT
	};

	struct ScriptHeader
	{
	public:
		ScriptHeader()
			:StackSize(0), GlobalDataSize(0), IsMainFuncPresent(0), MainFuncIndex(-1){}
		int StackSize;
		int GlobalDataSize;
		int IsMainFuncPresent;
		int MainFuncIndex;
	};

	// A function table node
	struct FuncNode
	{
		std::string Name;
		int Index;
		int EntryPoint;
		int ParamCount;
		int LocalDataSize;
	};

	// A symbol table node
	struct SymbolNode
	{
		std::string Name;
		int Index;
		int Size;				// Size(1 for variables,N for arrays)
		int StackIndex;			// The stack index to which the symbol
		int FuncIndex;			// Function in which the symbol resides
	};

	struct LabelNode
	{
		std::string Name;
		int Index;
		int TargetIndex;		// Index of the target instruction
		int FuncIndex;
	};

	/* lookup table item */
	struct InstrLookup
	{
		std::string Mnemonic;
		InstrType OpCode;
		int OpCount;
		std::vector<OpFlagType> OpList;
	};

	/* assemble instruction stream  */
	// operation
	struct Operation
	{
		int Type;
		union
		{
			int IntLiteral;
			float FloatLiteral;
			int StringTableIndex;
			int StackIndex;
			int InstrIndex;
			int FuncIndex;
			int HostAPICallIndex;
			int Reg;
		};
		int OffsetIndex;
	};

	// instruction
	struct Instruction
	{
		std::string Mnemonic;
		InstrType OpCode;
		int OpCount;
		std::vector<Operation> OpList;
	};

	// lookup table
	struct LookupTable
	{
		LookupTable()
		{
			InstrLookupTable.resize(MAX_INSTR_LOOKUP_COUNT);
		}
		std::vector<InstrLookup> InstrLookupTable;
		std::vector<Instruction> InstructionTable;
		std::vector<LabelNode> LableTable;
		std::vector<SymbolNode> SymbolTable;
		std::vector<FuncNode> FuncTable;
		std::vector<std::string> StringTable;
	};	
}

#endif