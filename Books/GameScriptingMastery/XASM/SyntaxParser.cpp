#include <cctype>
#include <cassert>
#include <iostream>
#include <algorithm>
#include "SyntaxParser.h"
#include "Exception.h"
#include "global.h"

namespace xasm
{
	SyntaxParser::SyntaxParser()
	{
		is_func_active_ = false;
		is_set_stacksize_found = false;

		curr_func_index = 0;
		curr_func_param_count_ = 0;
		curr_func_local_data_size_ = 0;
		instr_stream_size = 0;
		curr_func_index = 0;
		curr_inst_index = 0;
		tokens_.resize(256);
		tokens_.clear();

		InitInstrTable();
	}

	void SyntaxParser::BeginParse(const std::string & filename)
	{
		scanner_.LoadFile(filename);
		Token token = scanner_.GetNextToken();
		while (token.GetType() != TokenType::TOKEN_TYPE_END_OF_FILE){
			tokens_.push_back(token);
			token = scanner_.GetNextToken();
		}
		tokens_.push_back(Token(TokenType::TOKEN_TYPE_END_OF_FILE));
		itr_ = tokens_.begin();
		scanner_.CloseFile();
	}

	void SyntaxParser::Advance()
	{
		curr_token_ = *itr_;
		++itr_;
	}

	void SyntaxParser::ResetLexer()
	{
		itr_ = tokens_.begin();
	}

	const Token & SyntaxParser::GetCurrentToken()const
	{
		return curr_token_;
	}

	Token SyntaxParser::Peek()const
	{
		if (itr_ >= tokens_.end()){
			throw parse_exception{ "Already arrive end of token." };
		}
		return *(itr_);
	}

	void SyntaxParser::Parse()
	{
		Token token;

		// Fisrt scan
		while (true){
			Advance();
			token = GetCurrentToken();

			if (token.GetType() == TokenType::TOKEN_TYPE_END_OF_FILE){
				break;;
			}

			switch (token.GetType())
			{
			case TokenType::TOKEN_TYPE_SETSTACKSIZE:
				HandleSetStackSize();
				break;
				// Var/Var []
			case TokenType::TOKEN_TYPE_VAR:
				HandleVar();
				break;
				// Func
			case TokenType::TOKEN_TYPE_FUNC:
				HandleFunc();
				break;
			case TokenType::TOKEN_TYPE_CLOSE_BRACE:
				HandleCloseBrace();
				break;
				// Param
			case TokenType::TOKEN_TYPE_PARAM:
				HandleParam();
				break;
				// Instructions
			case TokenType::TOKEN_TYPE_INSTR:
				HandleInstr();
				break;
			case TokenType::TOKEN_TYPE_IDENT:
				HandleIdent();
				break;
			default:
				break;
			}
		}

		ResetLexer();
		table_.InstructionTable.resize(this->instr_stream_size);

		// Second scan
		while (true){
			Advance();
			token = GetCurrentToken();

			if (token.GetType() == TokenType::TOKEN_TYPE_END_OF_FILE){
				break;
			}

			switch (token.GetType()){
				case TokenType::TOKEN_TYPE_FUNC:
					HandleFunc2();
					break;
				case TokenType::TOKEN_TYPE_CLOSE_BRACE:
					HandleCloseBrace2();
					break;
				case TokenType::TOKEN_TYPE_INSTR:
					HandleInstr2();
					break;
				case TokenType::TOKEN_TYPE_PARAM:
					HandleParam2();
					break;

				default:
					break;
			}
		}
	}

	void SyntaxParser::BuildXSE()
	{
		std::string filename = scanner_.GetFileName();
		std::string outputname = std::string(filename.begin(),filename.end() - 5) + ".xse";

		std::ofstream file;
		
		file.open(outputname.c_str(), std::ios::binary | std::ios::out | std::ios::trunc | std::ios::app);
		if (file.fail()){
			throw file_open_failed_exception{ outputname + "open failed." };
		}

		file.write(XSE_ID_STRING.c_str(), strlen(XSE_ID_STRING.c_str()));
		file.write((char*)VERSION_MAJOR, sizeof(VERSION_MAJOR));
		file.write((char*)VERSION_MINOR, sizeof(VERSION_MINOR));

	}

	void SyntaxParser::HandleSetStackSize()
	{
		if (is_func_active_){
			ExitOnCodeError(ERROR_MSG_LOCAL_SETSTACKSIZE);
		}

		if (is_set_stacksize_found){
			ExitOnCodeError(ERROR_MSG_MULTIPLE_SETSTACKSIZES);
		}

		Advance();
		if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_INT){
			ExitOnCodeError(ERROR_MSG_INVALID_STACK_SIZE);
		}

		header_.StackSize = GetCurrentToken().GetInteger();
		is_set_stacksize_found = true;
	}

	void SyntaxParser::HandleVar()
	{
		Advance();
		if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_IDENT){
			ExitOnCodeError(ERROR_MSG_IDENT_EXPECTED);
		}

		std::string ident = GetCurrentToken().GetString();
		int iSize = 1;
		TokenType type = Peek().GetType();
		if (Peek().GetType() == TokenType::TOKEN_TYPE_OPEN_BRACKET){
			Advance(); // eat '['
			Advance();
			if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_INT){
				ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_INDEX);
			}

			iSize = GetCurrentToken().GetInteger();
			// Make sure the size is valid
			if (iSize <= 0){
				ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_INDEX);
			}

			Advance();
			if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_CLOSE_BRACKET){
				ExitOnCodeError(ERROR_MSG_CLOSE_BRACKET_EXPECTED);
			}
		}

		int iStackIndex;

		if (is_func_active_){//local variable
			iStackIndex = -(curr_func_local_data_size_ + 2);
		}
		else{//global
			iStackIndex = header_.GlobalDataSize;
		}

		if (AddSymbol(ident, iSize, iStackIndex, curr_func_index) == -1){
			ExitOnCodeError(ERROR_MSG_IDENT_REDEFINITION);
		}

		if (is_func_active_){
			curr_func_local_data_size_ += iSize;
		}
		else{
			header_.GlobalDataSize += iSize;
		}

	}

	void SyntaxParser::HandleFunc()
	{
		// make sure we aren't in a function already
		if (is_func_active_){
			ExitOnCodeError(ERROR_MSG_NESTED_FUNC);
		}

		Advance();
		if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_IDENT){
			ExitOnCodeError(ERROR_MSG_IDENT_EXPECTED);
		}

		std::string funcName = GetCurrentToken().GetString();

		int iEntryPoint = instr_stream_size;
		int iFuncIndex = table_.FuncTable.size();

		FuncNode newNode;
		newNode.Name = funcName;
		newNode.EntryPoint = iEntryPoint;
		newNode.Index = iFuncIndex;

		auto & FuncTable = table_.FuncTable;
		if (std::find_if(FuncTable.begin(), FuncTable.end(), [&](const FuncNode & node)->bool
		{
			return newNode.Name == node.Name;
		}) != FuncTable.end()){
			ExitOnCodeError(ERROR_MSG_FUNC_REDEFINITION);
		}

		table_.FuncTable.push_back(newNode);

		if (funcName == MAIN_FUNC_NAME){
			header_.IsMainFuncPresent = 1;
			header_.MainFuncIndex = iFuncIndex;
		}

		is_func_active_ = true;
		curr_func_name_ = funcName;
		curr_func_index = iFuncIndex;
		curr_func_param_count_ = 0;
		curr_func_local_data_size_ = 0;

		Advance();
		while (GetCurrentToken().GetType() == TokenType::TOKEN_TYPE_NEWLINE){
			Advance();
		}

		if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_OPEN_BRACE){
			ExitOnCodeError(ERROR_MSG_CLOSE_BRACE_EXPECTED);
		}

		// All functions are automatically appended with Ret, so increment the
		// required size of the instruction stream
		++instr_stream_size;
	}

	void SyntaxParser::HandleFunc2()
	{
		Advance();
		std::string name = GetCurrentToken().GetString();
		curr_func_node_ = *GetFuncByName(name);
		curr_func_index = curr_func_node_.Index;

		is_func_active_ = true;
		curr_func_param_count_ = 0;

		Advance();
		while (GetCurrentToken().GetType() == TokenType::TOKEN_TYPE_NEWLINE){
			Advance();
		}
	}

	void SyntaxParser::HandleCloseBrace()
	{
		if (!is_func_active_){
			ExitOnCodeError(ERROR_MSG_CLOSE_BRACE_EXPECTED);
		}

		std::string funcName = curr_func_name_;
		auto & FuncTable = table_.FuncTable;
		auto & itr = std::find_if(FuncTable.begin(), FuncTable.end(), [&](const FuncNode & func)->bool
		{
			return funcName == func.Name;
		});

		assert(itr != FuncTable.end());
		itr->ParamCount = curr_func_param_count_;
		itr->LocalDataSize = curr_func_local_data_size_;

		is_func_active_ = false;
	}

	void SyntaxParser::HandleCloseBrace2()
	{
		is_func_active_ = false;

		if (curr_func_node_.Name == MAIN_FUNC_NAME){
			table_.InstructionTable[curr_inst_index].Mnemonic = "ret";
			table_.InstructionTable[curr_inst_index].OpCode = INSTR_RET;
			table_.InstructionTable[curr_inst_index].OpCount = 1;

			Operation op;
			op.Type = OP_TYPE_INT;
			op.IntLiteral = 0;
			table_.InstructionTable[curr_inst_index].OpList.push_back(op);
		}
		else{
			table_.InstructionTable[curr_inst_index].Mnemonic = "ret";
			table_.InstructionTable[curr_inst_index].OpCode = INSTR_RET;
			table_.InstructionTable[curr_inst_index].OpCount = 0;
		}

		++curr_func_index;
		++curr_inst_index;
	}

	void SyntaxParser::HandleParam()
	{
		if (!is_func_active_){
			ExitOnCodeError(ERROR_MSG_GLOBAL_PARAM);
		}

		if (curr_func_name_ == MAIN_FUNC_NAME){
			ExitOnCodeError(ERROR_MSG_MAIN_PARAM);
		}

		Advance();
		if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_IDENT){
			ExitOnCodeError(ERROR_MSG_IDENT_EXPECTED);
		}

		++curr_func_param_count_;
	}

	void SyntaxParser::HandleParam2()
	{
		Advance();
		if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_IDENT){
			ExitOnCodeError(ERROR_MSG_IDENT_EXPECTED);
		}

		std::string name = GetCurrentToken().GetString();
		int iStackIndex = -(2 + curr_func_node_.LocalDataSize + 1 + curr_func_param_count_);
		if (AddSymbol(name, 1, iStackIndex, curr_func_index) == -1){
			ExitOnCodeError(ERROR_MSG_IDENT_REDEFINITION);
		}

		++curr_func_param_count_;
	}

	void SyntaxParser::HandleInstr()
	{
		if (!is_func_active_){
			ExitOnCodeError(ERROR_MSG_GLOBAL_INSTR);
		}

		++instr_stream_size;

		Advance();
		while (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_NEWLINE){
			Advance();
		}
	}

	void SyntaxParser::HandleInstr2()
	{
		curr_inst_lookup = *GetInstrLookupByName(GetCurrentToken().GetString());

		table_.InstructionTable[curr_inst_index].Mnemonic = curr_inst_lookup.Mnemonic;
		table_.InstructionTable[curr_inst_index].OpCode = curr_inst_lookup.OpCode;
		table_.InstructionTable[curr_inst_index].OpCount = curr_inst_lookup.OpCount;

		table_.InstructionTable[curr_inst_index].OpList.resize(curr_inst_lookup.OpCount);
		table_.InstructionTable[curr_inst_index].OpList.clear();

		Operation op;
		for (int i = 0; i < curr_inst_lookup.OpCount; ++i){

			OpFlagType CurrOpTypes = curr_inst_lookup.OpList[i];

			Advance();
			Token InitOpToken = GetCurrentToken();
			switch (InitOpToken.GetType())
			{
			case TokenType::TOKEN_TYPE_INT:
				if (CurrOpTypes & OP_FLAG_TYPE_INT){
					op.Type = OP_TYPE_INT;
					op.IntLiteral = InitOpToken.GetInteger();
				}
				else{
					ExitOnCodeError(ERROR_MSG_INVALID_OP);
				}
				break;
			case TokenType::TOKEN_TYPE_FLOAT:
				if (CurrOpTypes & OP_FLAG_TYPE_FLOAT){
					op.Type = OP_TYPE_FLOAT;
					op.FloatLiteral = InitOpToken.GetFloat();
				}
				else{
					ExitOnCodeError(ERROR_MSG_INVALID_OP);
				}
				break;
			case TokenType::TOKEN_TYPE_STRING:
				if (CurrOpTypes & OP_FLAG_TYPE_STRING){
					// Convert empty strings to the integer value zero
					if (InitOpToken.GetString().empty()){
						op.Type = OP_TYPE_INT;
						op.IntLiteral = 0;
					}
					else{
						op.Type = OP_TYPE_STRING_INDEX;
						op.StringTableIndex = table_.StringTable.size();
						table_.StringTable.push_back(InitOpToken.GetString());

					}
				}
				else{
					ExitOnCodeError(ERROR_MSG_INVALID_OP);
				}
				break;
			case TokenType::TOKEN_TYPE_REG_RETVAL:
				if (CurrOpTypes & OP_FLAG_TYPE_REG){
					op.Type = OP_TYPE_REG;
					op.Reg = 0;
				}
				else{
					ExitOnCodeError(ERROR_MSG_INVALID_OP);
				}
				break;
			case TokenType::TOKEN_TYPE_IDENT:
				if (CurrOpTypes & OP_FLAG_TYPE_MEM_REF){
					std::string name = InitOpToken.GetString();
					SymbolNode * node = GetSymbolByName(name, curr_func_index);
					if (!node){
						ExitOnCodeError(ERROR_MSG_UNDEFINED_IDENT);
					}


					int iBaseIndex = node->StackIndex;
					// not array
					if (Peek().GetType() != TokenType::TOKEN_TYPE_OPEN_BRACKET){
						if (node->Size > 1){
							ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_NOT_INDEXED);
						}
						op.Type = OP_TYPE_ABS_STACK_INDEX;
						op.IntLiteral = iBaseIndex;
					}
					else{// array!
						if (node->Size <= 1){
							ExitOnCodeError(ERROR_MSG_INVALID_ARRAY);
						}
						Advance(); 
						if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_OPEN_BRACKET){
							ExitOnCodeError(ERROR_MSG_OPEN_BRACKET_EXPECTED);
						}

						Advance(); // eat '['
						Token IndexToken = GetCurrentToken();
						TokenType type = IndexToken.GetType();
						if (type == TokenType::TOKEN_TYPE_INT){
							int iOffsetIndex = IndexToken.GetInteger();
							op.Type = OP_TYPE_ABS_STACK_INDEX;
							op.StackIndex = iOffsetIndex + iBaseIndex;
						}
						else if (type == TokenType::TOKEN_TYPE_IDENT){
							std::string name = IndexToken.GetString();
							SymbolNode * node = GetSymbolByName(name, curr_func_index);
							if (!node){
								ExitOnCodeError(ERROR_MSG_UNDEFINED_IDENT);
							}
							if (node->Size < 1){
								ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_INDEX);
							}

							int iOffsetIndex = node->StackIndex;

							op.Type = OP_TYPE_REL_STACK_INDEX;
							op.StackIndex = iBaseIndex;
							op.OffsetIndex = iOffsetIndex;
						}
						else{
							ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_INDEX);
						}

						Advance();
						if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_CLOSE_BRACKET){
							ExitOnCodeError(ERROR_MSG_CLOSE_BRACKET_EXPECTED);
						}
					}
				}
				
				// Parse a line label
				if (CurrOpTypes & OP_FLAG_TYPE_LINE_LABEL){
					
					std::string name = InitOpToken.GetString();

					LabelNode * labelNode = GetLabelByName(name, curr_func_index);
					if (!labelNode){
						ExitOnCodeError(ERROR_MSG_UNDEFINED_LINE_LABEL);
					}

					op.Type = OP_TYPE_INSTR_INDEX;
					op.InstrIndex = labelNode->TargetIndex;
				}

				// Parse a function name
				if (CurrOpTypes & OP_FLAG_TYPE_FUNC_NAME){
					std::string name = InitOpToken.GetString();
					FuncNode * funcNode = GetFuncByName(name);
					if (!funcNode){
						ExitOnCodeError(ERROR_MSG_UNDEFINED_FUNC);
					}
					op.Type = OP_TYPE_FUNC_INDEX;
					op.FuncIndex = funcNode->Index;
				}

				// Parse a host API call
				if (CurrOpTypes & OP_FLAG_TYPE_HOST_API_CALL){
					//TODO: how to add host call?
					;
				}
				break;
			default:
				ExitOnCodeError(ERROR_MSG_INVALID_OP);
				break;
			}

			
			if (i < curr_inst_lookup.OpCount - 1){
				Advance();
				if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_COMMA){
					ExitOnCodeError(ERROR_MSG_COMMA_EXPECTED);
				}
			}

			table_.InstructionTable[curr_inst_index].OpList.push_back(op);
		}// end for

		Advance();
		if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_NEWLINE){
			ExitOnCodeError(ERROR_MSG_INVALID_INPUT);
		}

		++curr_inst_index;
	}

	// line labels
	void SyntaxParser::HandleIdent()
	{
		std::string name = GetCurrentToken().GetString();

		Advance();
		if (GetCurrentToken().GetType() != TokenType::TOKEN_TYPE_COLON){
			ExitOnCodeError(ERROR_MSG_INVALID_INSTR);
		}

		if (!is_func_active_){
			ExitOnCodeError(ERROR_MSG_GLOBAL_LINE_LABEL);
		}

		
		int iTargetIndex = instr_stream_size;
		int iFuncIndex = curr_func_index;

		LabelNode newNode;
		auto & LabelTable = table_.LableTable;
		newNode.Name = name;
		newNode.Index = LabelTable.size();
		newNode.FuncIndex = iFuncIndex;
		newNode.TargetIndex = iTargetIndex;
		if (std::find_if(LabelTable.begin(), LabelTable.end(), [&](const LabelNode & label)->bool{
			return name == label.Name;
		} ) != LabelTable.end()){
			ExitOnCodeError(ERROR_MSG_LINE_LABEL_REDEFINITION);
		}

		table_.LableTable.push_back(newNode);
	}

	int SyntaxParser::AddSymbol(std::string name, int iSize, int iStackIndex, int iCurrFuncIndex)
	{
		if (AlreadyFindSymbol(name, iCurrFuncIndex)){
			return -1;
		}

		SymbolNode newNode;
		newNode.Name = name;
		newNode.StackIndex = iStackIndex;
		newNode.FuncIndex = iCurrFuncIndex;
		newNode.Size = iSize;
		newNode.Index = table_.SymbolTable.size();

		table_.SymbolTable.push_back(newNode);

		return newNode.Index;
	}

	int SyntaxParser::AlreadyFindSymbol(const std::string & ident, int FuncIndex)
	{
		auto & SymbolTable = table_.SymbolTable;
		return std::find_if(SymbolTable.begin(), SymbolTable.end(), [&](const SymbolNode & symbol)->bool{
			return (ident == symbol.Name) && (FuncIndex == symbol.FuncIndex || symbol.StackIndex >= 0);
		}) != SymbolTable.end();
	}

	void SyntaxParser::StringToLower(std::string & str)
	{
		std::transform(str.begin(), str.end(), str.begin(), std::tolower);
	}
	
	void SyntaxParser::ExitOnCodeError(const std::string & line)
	{

		const Token & token = GetCurrentToken();
		const TokenLocation & loc = token.GetTokenLocation();

		int iLine = loc.GetLine();
		int iColumn = loc.GetColumn();

		std::string currLine = scanner_.GetLineString(iLine);
		std::string filename = scanner_.GetTokenLocation().GetFileName();
	
		std::cout << filename << " Line:" << iLine 
			<< " Column:" << iColumn << std::endl;
		
		const char * pstr = currLine.c_str();
		while (std::isspace(*pstr)){
			iColumn--;
			pstr++;
		}
		while (*pstr != '\0'){
			std::cout << *pstr;
			pstr++;
		}

		std::cout << '\n';
		for (int i = 0; i < iColumn - 1; ++i){
			std::cout << ' ';
		}
		std::cout << '^' << std::endl;
		std::cout << line << std::endl;

		std::cout << "parser failed.\nExit....\nBye!" << std::endl;

		exit(EXIT_FAILURE);
	}

	void SyntaxParser::InitInstrTable()
	{
		int iInstrIndex;

		// Mov   Destincation,Source

		iInstrIndex = AddInstrLookup("mov", INSTR_MOV, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Add	Destincation,Source

		iInstrIndex = AddInstrLookup("add", INSTR_ADD, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Sub  Destincation,Source

		iInstrIndex = AddInstrLookup("sub", INSTR_SUB, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_REG);

		// Mul  Destination,Source
		iInstrIndex = AddInstrLookup("mul", INSTR_MUL, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Div  Destination,Source
		iInstrIndex = AddInstrLookup("div", INSTR_DIV, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Mod  Destination,Source
		iInstrIndex = AddInstrLookup("mod", INSTR_MOD, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Exp  Destination,Source
		iInstrIndex = AddInstrLookup("exp", INSTR_EXP, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Neg Destincation
		iInstrIndex = AddInstrLookup("neg", INSTR_NEG, 1);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		//  Inc Destincation
		iInstrIndex = AddInstrLookup("inc", INSTR_INC, 1);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Dec Destincation
		iInstrIndex = AddInstrLookup("dec", INSTR_DEC, 1);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// And Destination,Source
		iInstrIndex = AddInstrLookup("and", INSTR_AND, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Or  Destination, Source
		iInstrIndex = AddInstrLookup("or", INSTR_OR, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Xor Destination, Source
		iInstrIndex = AddInstrLookup("xor", INSTR_XOR, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Not Destination
		iInstrIndex = AddInstrLookup("not", INSTR_NOT, 1);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// ShL Destination, ShiftCount
		iInstrIndex = AddInstrLookup("shl", INSTR_SHL, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// ShR Destination, ShiftCount
		iInstrIndex = AddInstrLookup("slr", INSTR_SHR, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Concat String0,String1
		iInstrIndex = AddInstrLookup("concat", INSTR_CONCAT, 2);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_REG);

		// GetChar  Destination,Source,Index
		iInstrIndex = AddInstrLookup("getchar", INSTR_GETCHAR, 3);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG |
			OP_FLAG_TYPE_STRING);
		SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG |
			OP_FLAG_TYPE_INT);

		// SetChar  Destination,Index,Source
		iInstrIndex = AddInstrLookup("setchar", INSTR_SETCHAR, 3);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG |
			OP_FLAG_TYPE_INT);
		SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG |
			OP_FLAG_TYPE_STRING);

		// Jmp Label
		iInstrIndex = AddInstrLookup("jmp", INSTR_JMP, 1);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_LINE_LABEL);

		// JE  Op0,Op1,Label
		iInstrIndex = AddInstrLookup("je", INSTR_JE, 3);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

		// JNE Op0, Op1, Label
		iInstrIndex = AddInstrLookup("jne", INSTR_JNE, 3);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

		// JG  Op0, Op1, Label
		iInstrIndex = AddInstrLookup("jg", INSTR_JG, 3);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

		// JL  Op0, Op1, Label
		iInstrIndex = AddInstrLookup("jl", INSTR_JL, 3);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

		// JGE Op0, Op1, Label
		iInstrIndex = AddInstrLookup("jge", INSTR_JGE, 3);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

		// JLE Op0, Op1, Label
		iInstrIndex = AddInstrLookup("jle", INSTR_JLE, 3);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);


		// Push Source
		iInstrIndex = AddInstrLookup("push", INSTR_PUSH, 1);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Pop Destination
		iInstrIndex = AddInstrLookup("pop", INSTR_POP, 1);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Call FunctionName
		iInstrIndex = AddInstrLookup("call", INSTR_CALL, 1);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_FUNC_NAME);

		// Ret
		iInstrIndex = AddInstrLookup("ret", INSTR_RET, 0);

		// CallHost FunctioName
		iInstrIndex = AddInstrLookup("callhost", INSTR_CALLHOST, 1);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_HOST_API_CALL);

		// Pause Duration
		iInstrIndex = AddInstrLookup("pause", INSTR_PAUSE, 1);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_TYPE_REG);

		// Exit Code
		iInstrIndex = AddInstrLookup("exit", INSTR_EXIT, 1);
		SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_TYPE_REG);
	}

	int SyntaxParser::AddInstrLookup(const std::string & name, InstrType OpCode, int OpCount)
	{
		static int iInstrIndex = 0;

		if (iInstrIndex >= MAX_INSTR_LOOKUP_COUNT){
			return -1;
		}

		table_.InstrLookupTable.at(iInstrIndex).Mnemonic = name;
		table_.InstrLookupTable.at(iInstrIndex).OpCode = OpCode;
		table_.InstrLookupTable.at(iInstrIndex).OpCount = OpCount;
		table_.InstrLookupTable.at(iInstrIndex).OpList.resize(OpCount);

		return iInstrIndex++;
	}

	void SyntaxParser::SetOpType(int iInstrIndex, int iOpIndex, OpFlagType iOpType)
	{
		table_.InstrLookupTable.at(iInstrIndex).OpList.at(iOpIndex) = iOpType;
	}

	SymbolNode * SyntaxParser::GetSymbolByName(const std::string & name, int iFuncIndex)
	{
		auto & symbolTable = table_.SymbolTable;
		auto itr = std::find_if(symbolTable.begin(), symbolTable.end(),
			[&](const SymbolNode & symbolNode){
			return symbolNode.Name == name && symbolNode.FuncIndex == iFuncIndex;
			}
		);

		// NOT local variable
		if (itr == symbolTable.end()){
			auto itr2 = std::find_if(symbolTable.begin(), symbolTable.end(),
				[&](const SymbolNode & symbolNode)
				{
					return symbolNode.Name == name && symbolNode.StackIndex >= 0;
				});
			// if it is global variable?
			if (itr2 == symbolTable.end()){
				return nullptr;
			}
			else{
				return &(*itr2);
			}
		}
		else{
			return &(*itr);
		}
	}

	LabelNode * SyntaxParser::GetLabelByName(const std::string & name, int iFuncIndex)
	{
		auto labelTable = table_.LableTable;
		auto itr = std::find_if(labelTable.begin(), labelTable.end(),
			[&](const LabelNode & labelNode){
			return labelNode.Name == name && labelNode.FuncIndex == iFuncIndex;
			}
		);

		if (itr == labelTable.end()){
			return nullptr;
		}
		else{
			return &(*itr);
		}
	}

	FuncNode * SyntaxParser::GetFuncByName(const std::string & name)
	{
		auto & funcTable = table_.FuncTable;
		auto itr = std::find_if(funcTable.begin(), funcTable.end(),
			[&](const FuncNode & funcNode)
			{
				return funcNode.Name == name;
			}
		);

		if (itr == funcTable.end()){
			return nullptr;
		}
		else{
			return &(*itr);
		}
	}

	InstrLookup * SyntaxParser::GetInstrLookupByName(const std::string & name)
	{
		auto  & instrLookupTable = table_.InstrLookupTable;
		auto itr = std::find_if(instrLookupTable.begin(), instrLookupTable.end(),
			[&](const InstrLookup & instrLookup){
				return instrLookup.Mnemonic == name;
			}
		);

		if (itr == instrLookupTable.end()){
			return nullptr;
		}
		else{
			return  &(*itr);
		}
	}
}