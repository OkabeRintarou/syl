#include <cassert>
#include <iostream>
#include <fstream>
#include <exception>
#include "XVM.h"
#include "Global.h"

namespace xvm
{
	class file_open_failed_exception :public std::exception{};

	LoadState XVM::LoadScript(const std::string & filename)
	{
		std::fstream file;
		file.open(filename, std::ios::in | std::ios::binary);
		if (file.fail()){
			throw file_open_failed_exception();
		}

		char * IDString = 0;
		IDString = new char[5];

		file.read((char*)IDString, sizeof(char)* 4);
		IDString[4] = 0;
		if (strcmp(XSE_ID_STRING.c_str(), IDString) != 0){
			return LOAD_ERROR_INVALID_XSE;
		}

		delete[] IDString;

		// 读取脚本的版本
		char minor, major;
		file.read((char*)&major, sizeof(major));
		file.read((char*)&minor, sizeof(minor));

		// 验证版本
		if (major != VERSION_MAJOR || minor != VERSION_MINOR){
			return LOAD_ERROR_UNSUPPORTED_VERSION;
		}

		// 读取堆栈大小
		script_.Stack.Size = 0;
		file.read((char*)&script_.Stack.Size, sizeof(script_.Stack.Size));
		assert(script_.Stack.Size != 0);

		this->script_.Stack.Elements.reserve(script_.Stack.Size);

		// 读取全局数据大小
		file.read((char*)&this->script_.GlobalDataSize, sizeof(this->script_.GlobalDataSize));
		// _Main()是否存在
		char isMain = 0;
		file.read((char*)&isMain, sizeof(isMain));
		this->script_.IsMainFuncPresent = static_cast<int>(isMain);
		// MainIndex
		file.read((char*)&this->script_.MainFuncIndex, sizeof(this->script_.MainFuncIndex));

		// 读取指令流
		file.read((char*)&this->script_.InstrStream.Size, sizeof(this->script_.InstrStream.Size));

		this->script_.InstrStream.Instructions.reserve(this->script_.InstrStream.Size);
		int iInstrSize = this->script_.InstrStream.Size;
		for (int i = 0; i < iInstrSize; ++i){
			Instruction instr;
			// 操作码(2字节)
			short sOpCode;
			file.read((char*)&sOpCode, sizeof(sOpCode));
			instr.OpCode = static_cast<int>(sOpCode);
			// 操作数个数(1字节)
			char cOpCount;
			file.read((char*)&cOpCount, sizeof(cOpCount));
			instr.OpCount = static_cast<int>(cOpCount);

			int iOpCount = static_cast<int>(cOpCount);
			instr.OpList.reserve(iOpCount);
			// 操作数
			for (int j = 0; j < iOpCount; ++j){

				char cOpType;
				Value value;
				file.read((char*)&cOpType, sizeof(cOpType));
				value.Type = cOpType;
				value.OffsetIndex = 0;
				switch (cOpType)
				{
				case OP_TYPE_INT:
					file.read((char*)&value.IntLiteral, sizeof(value.IntLiteral));
					break;
				case OP_TYPE_FLOAT:
					file.read((char*)&value.FloatLiteral, sizeof(value.FloatLiteral));
					break;
				case OP_TYPE_STRING_INDEX:
					file.read((char*)&value.IntLiteral, sizeof(value.IntLiteral));
					break;
				case OP_TYPE_ABS_STACK_INDEX:
					file.read((char*)&value.StackIndex, sizeof(value.StackIndex));
					break;
				case OP_TYPE_REL_STACK_INDEX:
					file.read((char*)&value.StackIndex, sizeof(value.StackIndex));
					file.read((char*)&value.OffsetIndex, sizeof(value.OffsetIndex));
					break;
				case OP_TYPE_INSTR_INDEX:
					file.read((char*)&value.InstrIndex, sizeof(value.InstrIndex));
					break;
				case OP_TYPE_FUNC_INDEX:
					file.read((char*)&value.FuncIndex, sizeof(value.FuncIndex));
					break;
				case OP_TYPE_HOST_API_CALL_INDEX:
					file.read((char*)&value.HostAPICallIndex, sizeof(value.HostAPICallIndex));
					break;;
				case OP_TYPE_REG:
					file.read((char*)&value.Reg, sizeof(value.Reg));
					break;
				default:
					assert("unknow instruction!!!");
					break;
				}

				instr.OpList.push_back(value);
			}//读取操作数完毕

			this->script_.InstrStream.Instructions.push_back(instr);
		}// 读取指令流完毕

		// 字符串表
		int iStrSize;
		file.read((char*)&iStrSize, sizeof(iStrSize));
		char ** ppStringTable = new char*[iStrSize];
		for (int i = 0; i < iStrSize; ++i){
			int iStrLength;
			file.read((char*)&iStrLength, sizeof(iStrLength));
			ppStringTable[i] = new char[iStrLength + 1];
			file.read((char*)ppStringTable[i], iStrLength * sizeof(char));
			ppStringTable[i][iStrLength] = '\0';
		}


		for (int i = 0; i < this->script_.InstrStream.Size; ++i){
			int iOpCount = this->script_.InstrStream.Instructions[i].OpCount;

			// 对每个操作数做循环
			for (int j = 0; j < iOpCount; ++j){
				if (OpType::OP_TYPE_STRING_INDEX == this->script_.InstrStream.Instructions[i].OpList[j].Type){
					int iStrIndex = this->script_.InstrStream.Instructions[i].OpList[j].IntLiteral;
					int iStrLength = strlen(ppStringTable[iStrIndex]);
					this->script_.InstrStream.Instructions[i].OpList[j].StringLiteral = new char[iStrLength + 1];
					strcpy(this->script_.InstrStream.Instructions[i].OpList[j].StringLiteral, ppStringTable[iStrIndex]);
					this->script_.InstrStream.Instructions[i].OpList[j].StringLiteral[iStrLength] = '\0';
					this->script_.InstrStream.Instructions[i].OpList[j].Type = OpType::OP_TYPE_STRING_LITERAL;
				}
			}
		}

		// 释放字符串表
		for (int i = 0; i < iStrSize; ++i){
			delete[] ppStringTable[i];
		}
		delete[] ppStringTable;

		// 函数表
		int iFuncSize;
		file.read((char*)&iFuncSize, sizeof(iFuncSize));
		this->script_.FunctionTable.reserve(iFuncSize);
		for (int i = 0; i < iFuncSize; ++i){
			Function func;
			// Entry(4 bytes)
			file.read((char*)&func.EntryPoint, sizeof(func.EntryPoint));
			char cParamCount;
			file.read((char*)&cParamCount, sizeof(cParamCount));
			func.ParamCount = static_cast<int>(cParamCount);

			// Local DataSize
			file.read((char*)&func.LocalDataSize, sizeof(func.LocalDataSize));
			func.StackFrameSize = 1 + func.LocalDataSize + func.ParamCount;

			this->script_.FunctionTable.push_back(func);
		}

		// TODO:HOST API CALL READ

		//////////////////////////////////////////////////////////////////////

		return LOAD_OK;
	}

	void XVM::UnLoadScript()
	{
		auto & Instructions = this->script_.InstrStream.Instructions;
		int  iInstrCount = Instructions.size();
	
		for (int i = 0; i < iInstrCount; ++i){
			int iOpCount = Instructions[0].OpCount;

			for (int j = 0; j < iOpCount; ++j){
				if (Instructions[i].OpList[j].Type == OpType::OP_TYPE_STRING_LITERAL){
					delete[] Instructions[i].OpList[j].StringLiteral;
				}
			}
		}
	}

	int XVM::GetOpType(int OpIndex)
	{
		int iCurrInsr = this->script_.InstrStream.CurrInstr;
		return this->script_.InstrStream.Instructions[iCurrInsr].OpList[OpIndex].Type;
	}

	int XVM::GetOpAsInt(int OpIndex)
	{
		int iCurrInsr = this->script_.InstrStream.CurrInstr;
		return this->script_.InstrStream.Instructions[iCurrInsr].OpList[OpIndex].IntLiteral;
	}

	float XVM::GetOpAsFloat(int OpIndex)
	{
		int iCurrInsr = this->script_.InstrStream.CurrInstr;
		return this->script_.InstrStream.Instructions[iCurrInsr].OpList[OpIndex].FloatLiteral;
	}


	Value XVM::ResolveOpValue(int OpIndex)
	{
		int iCurrInsr = this->script_.InstrStream.CurrInstr;
		Value OpValue = this->script_.InstrStream.Instructions[iCurrInsr].OpList[OpIndex];

		switch (OpValue.Type){
		case OpType::OP_TYPE_REL_STACK_INDEX:
		case OpType::OP_TYPE_ABS_STACK_INDEX:
			int iAbsIndex = ResolveOpStackIndex(OpIndex);
			return GetStackValue(iAbsIndex);
		case OpType::OP_TYPE_REG:
			return this->script_._RetVal;
		default:
			return OpValue;
		}
	}
}