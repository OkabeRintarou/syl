#include <cassert>
#include <cctype>
#include <stack>
#include "Error.h"
#include "BTParser.h"

IntegerType parseInteger(char * & str)
{
	assert(str != 0);
	if (*str != 'i'){
		THROW_BT_EXCEPTION( "must start with 'i'");
	}

	IntegerType::value_type val = 0;
	++str;
	while (std::isdigit(*str)){
		val = (val * 10) + (*str - '0');
		++str;
	}

	if (*str++ != 'e'){
		THROW_BT_EXCEPTION("must end with 'e'");
	}

	return IntegerType(val);
}

StringType parseString(char * &str)
{
	size_t len = 0;
	while (std::isdigit(*str)){
		len = (len * 10) + (*str - '0');
		++str;
	}

	if (*str != ':'){
		THROW_BT_EXCEPTION("':' expected");
	}

	++str;
	std::string str_val;
	str_val.reserve(len);
	for (size_t i = 0; i < len; ++i){
		str_val.push_back(*str++);
	}

	return StringType(str_val);
}

ListType parseList(char * & str,ListType & lst,DictionaryType & d)
{
	if (*str != 'l'){
		THROW_BT_EXCEPTION("must start with 'l'");
	}

	std::stack<char> stak;
	stak.push(*str);

	++str;
	char ch = *str;
	BaseType * new_list_item = 0;
	
	while (stak.size() == 1){

		if ('i' == ch){
			new_list_item = new IntegerType(parseInteger(str));
		}
		else if ('l' == ch){
			//stak.push(ch);
			new_list_item = new ListType(parseList(str, lst, d));
		}
		else if ('d' == ch){
			//stak.push(ch);
			new_list_item = new DictionaryType(parseDictionary(str, lst, d));
		}
		else if (std::isdigit(ch)){
			new_list_item = new StringType(parseString(str));
		}
		else{
			THROW_BT_EXCEPTION("parse failed");
		}

		lst.push_item(new_list_item);

		/*if (ch == 'i' || ch == 'l' || ch == 'd'){
			if (*str != 'e'){
			THROW_BT_EXCEPTION("must end with 'e'");
			}
			stak.pop();
			}

			if (stak.size() == 1 && *str == 'e'){
			stak.pop();
			break;
			}*/

		if (*str == 'e'){
			++str;
			stak.pop();
		}

	}
	

	return lst;
}

DictionaryType parseDictionary(char *& str,ListType & lst,DictionaryType & d)
{
	if (*str != 'd'){
		THROW_BT_EXCEPTION("must start with 'd'");
	}

	std::stack<char> stak;
	stak.push(*str);

	++str;

	while (stak.size() == 1){

		StringType key = parseString(str);
		const std::string  key_str = key.getValue();

		BaseType * val_ptr = nullptr;
		char ch = *str;
		if (ch == 'i'){
			val_ptr = new IntegerType(parseInteger(str));
		}
		else if (ch == 'l'){
			//stak.push(ch);
			val_ptr = new ListType(parseList(str, lst, d));
		}
		else if (ch == 'd'){
			//stak.push(ch);
			val_ptr = new DictionaryType(parseDictionary(str, lst, d));
		}
		else if (std::isdigit(ch)){
			val_ptr = new StringType(parseString(str));
		}
		else{
			THROW_BT_EXCEPTION("parse failed");
		}

		d.push_item(key_str, val_ptr);


		if (*str == 'e'){
			++str;
			stak.pop();
		}
	}
	

	return d;
}