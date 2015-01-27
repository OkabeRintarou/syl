#include <iostream>
#include <string>

using namespace std;

class parse_result
{
public:
	parse_result() :
		result_(0),parse_error_(false){}
	void set_result(int result)
	{
		result_ = result;
	}
	int get_result()const{ return result_; }
	void set_error(){ parse_error_ = true; }
	void clear_error(){ parse_error_ = false; }
	bool is_error()const{return parse_error_;}
private:
	int result_;
	bool parse_error_;
};

class manual_parse
{
public:
	manual_parse(string text)
		:input_(text), itr_(input_.begin()){}
	parse_result parse();
private:
	parse_result get_number();
	bool next_not_space_is(char c);
	char next_operator();
private:
	string input_;
	string::iterator itr_;
};

char manual_parse::next_operator()
{
	while ((itr_ != input_.end() && *itr_ == ' ')){
		itr_++;
	}

	if ((itr_ != input_.end()) &&
		((*itr_ == '+') || (*itr_ == '-') || (*itr_ == '*') || (*itr_ == '/'))
		){

		return *itr_++;
	}
	else{
		return 0;
	}
}

bool manual_parse::next_not_space_is(char c)
{
	while (true){
		if (itr_ != input_.end() && *itr_  == ' '){
			itr_++;
		}
		else{
			break;
		}
	}

	return (itr_ != input_.end()) && (c == *itr_++);
}

parse_result manual_parse::get_number()
{
	parse_result result;
	bool got_number = false;

	string::iterator old = itr_;

	while (*itr_ == ' ')itr_++;

	while (true){
		char c = *itr_;
		itr_++;
		if (itr_ == input_.end()) break;
		if ('0' <= c && c <= '9'){
			result.set_result(result.get_result() * 10 + (c - '0'));
			got_number = true;
		}
		else{
			break;
		}
	}

	if (got_number == false){
		itr_ = old;
		result.set_error();
	}
	return result;
}

parse_result manual_parse::parse()
{
	
	parse_result result = get_number();
	if (result.is_error()){
		result.clear_error();
		if (next_not_space_is('(')){

			char op = 0;
			if ((op = next_operator()) != 0){ // (+ - * /)

				parse_result left = parse();
				if (left.is_error())return left;
				parse_result right = parse();
				if (right.is_error()) return right;

				switch (op){
				case '+':
					result.set_result(left.get_result() + right.get_result());
					break;
				case '-':
					result.set_result(left.get_result() - right.get_result());
					break;
				case '*':
					result.set_result(left.get_result()  * right.get_result());
					break;
				case '/':
					result.set_result(left.get_result() / right.get_result());
					break;
				default:
					break;
				}
				
			}
			else{
				result = parse();
			}

			if (itr_ != input_.end() && *itr_ != ')'){
				cout << "Can.t found match ')'" << endl;
			}
		}
	}

	return result;
}

int main(int argc, char * argv[])
{
	string buffer;
	while (true){
		cout << ">";
		getline(cin,buffer);
		manual_parse parser(buffer);
		parse_result result = parser.parse();
		cout << result.get_result() << endl;
	}
	return 0;
}