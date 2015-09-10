#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

int char2int(char ch)
{
	ch = tolower(ch);
	if(ch >= '0' && ch <= '9')return ch - '0';
	if(ch >= 'a' && ch <= 'f')return ch - 'a' + 10;
	return -1;
}

void  error_input()
{
	cerr << "input error" << endl;
	exit(1);
}

int main(int argc,char * argv[])
{
	int n = 1;

	if(argc == 2){
		if(strlen(argv[1]) == 2){
			if(argv[1][0] == '-' && isdigit(argv[1][1])){
				n = argv[1][1] - '0';
			}
		}
	}

	string out;

	for(string line;getline(cin,line);){
		for(int i = 0; i < (int)line.size();){
			if(isalnum(line[i])){
				if(i + 1 == line.size()){
					error_input();
				}
				out += static_cast<char>((char2int(line[i]) << 4) + char2int(line[i + 1]));
				i += 2;
			}
			else if(isspace(line[i])){
				++i;
				continue;
			}
			else {
				error_input();
			}
		}
	}
	
	for(int i = 0; i < n;++i)cout << out;

	return 0;
}
