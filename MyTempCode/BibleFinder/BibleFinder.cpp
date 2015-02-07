/************************************************************************/
/*  Open bible.txt and find text you input                                                                     */
/************************************************************************/

#include <cctype>

#include <iostream>
#include <fstream>
#include <string>

class BibleFinder
{
public:
	BibleFinder(std::string file)
		:cur_char_(-1),line_(1),column_(0)
	{
		file_.open(file);
		if (!file_.is_open()){
			std::cout << "file open failed.\n";
		}
	}
	void FindString(const std::string & str);
	
private:
	void Advance();
	char Peek();
	void Preprocess();
private:
	std::fstream file_;
	char cur_char_;
	int line_;
	int column_;
};

void BibleFinder::Advance()
{
	cur_char_ = file_.get();
	column_++;

	if (cur_char_ == '\n'){
		line_++;
		column_ = 0;
	}
}

char BibleFinder::Peek()
{
	return file_.peek();
}

void BibleFinder::Preprocess()
{
	Advance();
	while (true){
		if (std::isspace(cur_char_)){
			Advance();
		}
		else{
			break;
		}
	}
}

void BibleFinder::FindString(const std::string & str)
{
	
	int index = 0;

	while (!file_.eof()){
		Preprocess();
		if (cur_char_ == str.at(index)){
			index++;
			int line = line_;
			int columu = column_;

			while (true){
				Advance();
				if (index == str.size()){
					if (std::isspace(cur_char_)){
						std::cout << "第" << line << "行,第" << columu << "列\n";
						index = 0;
						break;
					}
					else{
						Advance();
						while (!std::isspace(cur_char_)){
							Advance();
						}
						index = 0;
						break;
					}		
				}
				else if (cur_char_ == str.at(index)){
					index++;
				}
				else{
					index = 0;
					break;
				}
			}
		}
		else{
			while (true){
				Advance();
				if (file_.eof()){
					break;
				}
				if (std::isspace(cur_char_)){
					break;
				}
			}
		}
	}
}

int main()
{
	BibleFinder finder("bible.txt");
	std::string tofind;

	std::cout << "ready>";
	std::cin >> tofind;
	finder.FindString(tofind);

	return 0;
}