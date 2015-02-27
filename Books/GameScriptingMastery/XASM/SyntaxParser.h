

#ifndef XASM_SYNTAXPARSER_H_
#define XASM_SYNTAXPARSER_H_

#include <vector>
#include "Token.h"
#include "Scanner.h"
#include "Structure.h"

namespace xasm
{
	class SyntaxParser
	{
	public:
		SyntaxParser();
		void BeginParse(const std::string & filename);
		void Parse();
		void BuildXSE();
	private:
		inline Token Peek()const;
		void ExitOnCodeError(const std::string & line);
		void HandleSetStackSize();
		void HandleVar();
		void HandleFunc();
		void HandleFunc2();
		void HandleCloseBrace();
		void HandleCloseBrace2();
		void HandleParam();
		void HandleParam2();
		void HandleInstr();
		void HandleInstr2();
		void HandleIdent();
		inline void Advance();
		inline const Token & GetCurrentToken()const;
		inline void ResetLexer();

		void InitInstrTable();
		int AddInstrLookup(const std::string & name, InstrType OpCode, int OpCount);
		void SetOpType(int iInstrIndex, int iOpIndex, OpFlagType iOpType);

		int AddSymbol(std::string name, int iSize, int iStackIndex, int iCurrFuncIndex);
		int AlreadyFindSymbol(const std::string & ident,int FuncIndex);
		void StringToLower(std::string & str);

		//FuncNode & GetFuncNodeByName(const std::string & name);
		InstrLookup * GetInstrLookupByName(const std::string & name);
		SymbolNode * GetSymbolByName(const std::string & name,int iFuncIndex);
		LabelNode * GetLabelByName(const std::string & name, int iFuncIndex);
		FuncNode * GetFuncByName(const std::string & name);
	private:
		Scanner scanner_;
		std::vector<Token> tokens_;
		Token curr_token_;
		std::vector<Token>::iterator itr_;

		LookupTable table_;

		InstrLookup curr_instr_;
		ScriptHeader header_;

		FuncNode curr_func_node_;
		InstrLookup curr_inst_lookup;
		std::string curr_func_name_;
		int curr_func_index;
		int curr_func_param_count_;
		int curr_func_local_data_size_;
		int curr_inst_index;
		int instr_stream_size;
		bool is_func_active_;
		bool is_set_stacksize_found;
	};
}

#endif