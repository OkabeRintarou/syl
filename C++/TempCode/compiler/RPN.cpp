#include <cctype>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <algorithm>
#include <functional>

enum TokenType
{
	TT_OPERATOR = 1,
	TT_NUMBER = 2,
};

struct Token
{
	TokenType type;
	union{
		const char * str;
		mutable int value;
	};
};

static std::map<char, int> g_OpPriority;
static std::map<char, std::function<int(int, int)>> g_OpFunc;

static void SplitTokens(const char * src, std::vector<Token> & tokens)
{
	while (true){
		while (std::isspace(*src))++src;
		if (*src == '\0')break;

		if (std::isdigit(*src)){
			Token t = { TT_NUMBER, src };
			tokens.push_back(t);
			while (std::isdigit(*++src));
		}
		else{
			if (*src == '(' || *src == ')' || g_OpPriority.count(*src) > 0){
				Token t = { TT_OPERATOR, src++ };
				tokens.push_back(t);
			}
			else{
				assert(0);
			}
		}
	}
}

static void DumpTokens(const std::vector<Token> & tokens)
{
	using std::cout;
	using std::endl;
	for (int i = 0; i < (int)tokens.size(); ++i){
		if (tokens[i].type == TT_NUMBER){
			cout << std::atoi(tokens[i].str) << ' ';
		}
		else if (tokens[i].type == TT_OPERATOR){
			cout << tokens[i].str[0] << ' ';
		}
		else{
			assert(0);
		}
	}
	cout << endl;
}

static int Compute(const char * exp)
{
	std::vector<Token> tokens;
	SplitTokens(exp, tokens);
	std::stack<Token> stackOp, stackRPN;

	for (int i = 0; i < (int)tokens.size(); ++i){
		const Token & t = tokens[i];
		if (t.type == TT_NUMBER){
			t.value = std::atoi(t.str);
			stackRPN.push(t);
		}
		else if (t.type == TT_OPERATOR){
			if (t.str[0] == '('){
				stackOp.push(t);
			}
			else if (t.str[0] == ')'){
				while (!stackOp.empty() && stackOp.top().str[0] != '('){
					stackRPN.push(stackOp.top());
					stackOp.pop();
				}
				stackOp.pop();
			}
			else{
				if (!stackOp.empty() && stackOp.top().str[0] == '('){
					stackOp.push(t);
				}
				else{
					while (!stackOp.empty() &&
						(g_OpPriority[stackOp.top().str[0]] >= g_OpPriority[t.str[0]] &&
						stackOp.top().str[0] != '(')){

						stackRPN.push(stackOp.top());
						stackOp.pop();
					}
					stackOp.push(t);
				}
			}
		}
		else{
			assert(0);
		}
	}
	
	while (!stackOp.empty()){
		stackRPN.push(stackOp.top());
		stackOp.pop();
	}

	std::stack<Token> stackTmp;
	while (!stackRPN.empty()){
		stackTmp.push(stackRPN.top());
		stackRPN.pop();
	}

	std::stack<int> stackValue;

	while (!stackTmp.empty()){
		const Token & t = stackTmp.top();
		if (t.type == TT_NUMBER){
			stackValue.push(t.value);
		}
		else if (t.type == TT_OPERATOR){
			assert(stackValue.size() >= 2);
			int a = stackValue.top();
			stackValue.pop();
			int b = stackValue.top();
			stackValue.pop();
			if (t.str[0] == '-' || t.str[0] == '/' || t.str[0] == '!'){
				std::swap(a, b);
			}
			int c = g_OpFunc[t.str[0]](a, b);
			stackValue.push(c);
		}
		stackTmp.pop();
	}

	assert(stackValue.size() == 1);
	return stackValue.top();
	
}
int main(int argc, char * argv[])
{
	g_OpPriority['+'] = g_OpPriority['-'] = 1;
	g_OpPriority['*'] = g_OpPriority['/'] = 2;
	g_OpPriority['!'] = 3;

	g_OpFunc['+'] = [](int a, int b){return a + b; };
	g_OpFunc['-'] = [](int a, int b){return a - b; };
	g_OpFunc['*'] = [](int a, int b){return a * b; };
	g_OpFunc['/'] = [](int a, int b){return a / b; };
	g_OpFunc['!'] = [](int a, int b){return int(std::powf(float(a), float(b))); };

	assert(Compute("23 *5") == 115);
	assert(Compute("23 / 5") == 4);
	assert(Compute("4 *(1 + 2)") == 12);
	assert(Compute("((10 - 3) / 2) *(8 - (3 + 6))") == -3);
	assert(Compute("2!5") == 32);
	assert(Compute("2!3+2!(2+2)") == 24);
	
	std::string str;
	while (std::getline(std::cin, str)){
		std::cout << str << " = " << Compute(str.c_str()) << std::endl;
	}
	return 0;
}

