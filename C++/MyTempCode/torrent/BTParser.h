#ifndef _BTPARSER_H_
#define _BTPARSER_H_

#include <string>
#include <vector>

enum BType
{
	INTEGER,STRING,LIST,DICTIONARY
};

class BaseType
{
public:
	BaseType(BType type)
		:btype_(type){}
	void setType(BType type){ btype_ = type; }
	BType getType()const{ return btype_; }
private:
	BType btype_;

};

class IntegerType :public BaseType
{
public:
	typedef long value_type;
	IntegerType(value_type n)
		:BaseType(INTEGER),integer_(n){}
	value_type getValue()const{ return integer_; }
	void setValue(const value_type & val){ integer_ = val; }
private:
	value_type integer_;
};

class StringType :public BaseType
{
public:
	typedef std::string value_type;
public:
	StringType(const std::string &str)
		:BaseType(STRING),string_(str)
	{

	}

	value_type getValue()const{ return string_; }
	void setValue(const value_type & val){ string_ = val; }
private:
	std::string string_;
};

class ListType :public BaseType
{
public:
	typedef BaseType item_type;
	typedef std::vector<item_type*>::iterator iterator;
	typedef std::vector<iterator*>::const_iterator const_iterator;
public:
	ListType()
		:BaseType(LIST)
	{

	}

	~ListType()
	{
		/*for (size_t i = 0; i < vec_.size(); ++i){
			delete vec_[i];
		}*/
	}

	void push_item(item_type * item){ vec_.push_back(item); }
	bool empty()const{ return vec_.empty(); }
	iterator begin(){ return vec_.begin(); }
	iterator end(){ return vec_.end(); }
	const std::vector<item_type*>& getItems()const{ return vec_; }
private:
	std::vector<item_type *> vec_;
};

class DictionaryType :public BaseType
{
public:
	typedef std::string key_type;
public:
	DictionaryType()
		:BaseType(DICTIONARY)
	{
		
	}

	~DictionaryType()
	{
		/*for (size_t i = 0; i < vec_.size(); ++i){
			const key_type & key = vec_[i].first;
			const BaseType * value = vec_[i].second;

			delete value;
		}

		vec_.clear();*/
	}

	void push_item(const std::pair<key_type, BaseType*> & item)
	{
		vec_.push_back(item);
	}

	void push_item(const std::string & key,BaseType * val)
	{
		vec_.push_back(std::make_pair(key, val));
	}

	const std::vector<std::pair<key_type, BaseType*>> & getItems()const{ return vec_; }
private:
	std::vector<std::pair<key_type, BaseType*>> vec_;
};


IntegerType parseInteger(char * &);
StringType parseString(char *&);
ListType parseList(char * &,ListType &,DictionaryType &);
DictionaryType parseDictionary(char *&,ListType &,DictionaryType &);

class BTParser
{
private:
	
};

#endif