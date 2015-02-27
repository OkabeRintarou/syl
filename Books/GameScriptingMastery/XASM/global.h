/**
* @file global.h
* @brief
*  Some useful global declarations, structure, etc
* @author sylcoder
* @version 1.0
* @date 2015/2/16
*/

#ifndef XASM_GLOBAL_H_
#define XASM_GLOBAL_H_

#include <string>
#include <vector>
#include "Structure.h"

namespace xasm
{
	const int VERSION_MAJOR = 1;
	const int VERSION_MINOR = 0;

	const int MAX_INSTR_LOOKUP_COUNT = 256;

	const std::string ERROR_MSG_LOCAL_SETSTACKSIZE =
		"SetStackSize can only appear in the global scope";
	const std::string ERROR_MSG_INVALID_STACK_SIZE =
		"Invalid stack size";
	const std::string ERROR_MSG_MULTIPLE_SETSTACKSIZES =
		"Multiple instances of SetStackSize illegal";
	const std::string ERROR_MSG_IDENT_EXPECTED =
		"Identifier expected";
	const std::string ERROR_MSG_INVALID_ARRAY_SIZE =
		"Invalid array size";
	const std::string ERROR_MSG_IDENT_REDEFINITION =
		"Undefined identifier";
	const std::string ERROR_MSG_UNDEFINED_IDENT =
		"Undefined identifier";
	const std::string ERROR_MSG_NESTED_FUNC =
		"Nested functions illegal";
	const std::string ERROR_MSG_FUNC_REDEFINITION =
		"Function redefinition";
	const std::string ERROR_MSG_UNDEFINED_FUNC =
		"Undefined function";
	const std::string ERROR_MSG_GLOBAL_PARAM =
		"Parameters can only appear inside functions";
	const std::string ERROR_MSG_MAIN_PARAM =
		"_Main() function cannot accept parameters";
	const std::string ERROR_MSG_GLOBAL_LINE_LABEL =
		"Line labels can only appear inside functions";
	const std::string ERROR_MSG_LINE_LABEL_REDEFINITION =
		"Line label redefinition";
	const std::string ERROR_MSG_UNDEFINED_LINE_LABEL =
		"Undefined line label";
	const std::string ERROR_MSG_GLOBAL_INSTR =
		"Instructions can only appear inside functions";
	const std::string ERROR_MSG_INVALID_INSTR =
		"Invalid instruction";
	const std::string ERROR_MSG_INVALID_OP =
		"Invalid operand";
	const std::string ERROR_MSG_INVALID_STRING =
		"Invalid string";
	const std::string ERROR_MSG_INVALID_ARRAY_NOT_INDEXED =
		"Arrays must be indexed";
	const std::string ERROR_MSG_INVALID_ARRAY =
		"Invalid array";
	const std::string ERROR_MSG_INVALID_ARRAY_INDEX =
		"Invalid array index";
	const std::string ERROR_MSG_OPEN_BRACKET_EXPECTED =
		"[ expected";
	const std::string ERROR_MSG_CLOSE_BRACKET_EXPECTED =
		"] expected";
	const std::string ERROR_MSG_OPEN_BRACE_EXPECTED =
		"{ expected";
	const std::string ERROR_MSG_CLOSE_BRACE_EXPECTED =
		"} expected";
	const std::string ERROR_MSG_COMMA_EXPECTED =
		", expected";
	const std::string ERROR_MSG_INVALID_INPUT =
		"invalid input";

	const std::string MAIN_FUNC_NAME =
		"_main";
	const std::string XSE_ID_STRING =
		"XSE0";
}

#endif