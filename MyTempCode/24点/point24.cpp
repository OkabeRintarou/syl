#include <cassert>
#include <cctype>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <functional>

void Point24(std::vector<double> & a, std::vector<std::string> & exps, int len)
{
	assert(len >= 1);
	static const double epsilon = 0.00001;
	static const int target_point = 24;
	static std::unordered_map<std::string, bool> result_set;
	static std::map<std::string, std::function<double(double,double)>> ops =
	{
		{ "+", [](double a, double b)->double{return a + b; } } ,
		{ "-", [](double a, double b)->double{return a - b; } } ,
		{ "*", [](double a, double b)->double{return a * b; } } ,
		{ "/", [](double a, double b)->double{return a / b; } } 
	};

	static decltype(ops) ops_uncomm = { { "-", ops["-"] }, { "/", ops["/"] } };

	
	if (len == 1 &&
		std::abs(a[0] - target_point) < epsilon &&
		result_set.find(exps[0]) == result_set.end())
	{
		std::cout << exps[0] << std::endl;
		result_set[exps[0]] = true;
	}

	for (int i = 0; i < len - 1; i++){
		for (int j = i + 1; j < len; j++){
			// 处理所有运算
			for (auto & op:ops){
				
				double ai = a[i], aj = a[j];
				std::string expi = exps[i], expj = exps[j];

				a[i] = op.second(ai, aj);
				exps[i] = std::string(std::string("(") + exps[i] + op.first + exps[j] + std::string(")"));
				a[j] = a[len - 1], exps[j] = exps[len - 1];

				Point24(a, exps, len - 1);

				a[i] = ai, exps[i] = expi;
				a[j] = aj, exps[j] = expj;
			}
		}
	}

	// len == 2
	for (int i = 0; i < len - 1; i++){
		for (int j = i + 1; j < len; j++){
			// 处理非交换律的
			for (auto & op : ops_uncomm){
				double ai = a[i], aj = a[j];
				std::string expi = exps[i], expj = exps[j];

				a[i] = op.second(a[j], a[i]);
				exps[i] = std::string(std::string("(") + exps[j] + op.first + exps[i] + std::string(")"));

        		a[j] = a[len - 1], exps[j] = exps[len - 1];

				Point24(a, exps, len - 1);

				a[i] = ai, exps[i] = expi;
				a[j] = aj, exps[j] = expj;
			}
		}
	}
}

int main(int argc, char * argv[])
{
	std::vector<double> a;
	a.reserve(4);
	std::vector<std::string> exps;
	exps.reserve(4);

	for (int i = 0; i < 4; i++){
		double input;
		std::cin >> input;
		a.push_back(input);
		exps.push_back(std::to_string(static_cast<int>(input)));
	}

	Point24(a, exps, 4);

	return 0;
}