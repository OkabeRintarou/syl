#include <cassert>
#include <cctype>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Error.h"
#include "BitTorrent.h"
#include "BTParser.h"

BitTorrent::BitTorrent(std::string filename)
{
	std::fstream in(filename.c_str(),std::ios::in | std::ios::binary | std::ios::ate);
	char * contents = 0;
	
	if (in.is_open()){

		size_t size = in.tellg();
		contents = new char[size + 1];
		in.seekg(0, std::ios::beg);
		in.read(contents, size);
		contents[size] = 0;
		in.close();
	}

	ListType lst;
	DictionaryType dic;

	if (contents != 0){
		char * to_delete = contents;
		parseDictionary(contents, lst, dic);
		delete [] to_delete;
	}
	

	for (size_t i = 0; i < dic.getItems().size(); ++i){
		const auto & item = dic.getItems().at(i);
		auto  key = item.first;
		const auto & value = item.second;

		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
		if (key == "announce"){
			assert(value->getType() == BType::STRING);
			this->announce_ = static_cast<StringType*>(value)->getValue();
		}
		else if (key == "announce-list"){
			assert(value->getType() == BType::LIST);
			auto & items = static_cast<ListType*>(value)->getItems();
			for (size_t j = 0; j < items.size(); ++j){
				auto & item = items[j];

				if (item->getType() == BType::STRING){
					const std::string & name = static_cast<StringType*>(item)->getValue();
					this->announce_list_.push_back(name);
				}
				
			}
		}
		else if (key == "comment"){
			assert(value->getType() == BType::STRING);
			this->comment_ = this->comment_utf_8_ =
				static_cast<StringType*>(value)->getValue();
		}
		else if (key == "creation date"){
			assert(value->getType() == BType::INTEGER);
			this->creation_date_ = static_cast<IntegerType*>(value)->getValue();
		}
		else if (key == "length"){
			assert(value->getType() == BType::INTEGER);
			this->info_.length = static_cast<IntegerType*>(value)->getValue();
		}
		else if (key == "name"){
			assert(value->getType() == BType::STRING);
			this->info_.name = this->info_.name_utf_8 =
				static_cast<StringType*>(value)->getValue();
		}
		else if (key == "piece length"){
			assert(value->getType() == BType::INTEGER);
			this->piece_length_ = static_cast<IntegerType*>(value)->getValue();
		}
		else if (key == "pieces"){
			assert(value->getType() == BType::STRING);
			this->pieces_ = static_cast<StringType*>(value)->getValue();
		}
		else if (key == "info"){
			// do nothing
		}
		else if (key == "httpseeds"){
			// do nothing
		}
		else if (key == "files"){
			THROW_BT_EXCEPTION("currrent don't support multi files");
		}
		else{
			THROW_BT_EXCEPTION("unexpected token");
		}
		
	}

	this->encoding_ = "utf-8";
}