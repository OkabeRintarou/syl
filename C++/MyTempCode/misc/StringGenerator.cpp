#include <chrono>
#include <random>
#include <string>
#include "StringGenerator.h"

std::string RandomString(size_t length)
{
	static const std::string alphanums =
		"0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	static std::mt19937 rg(static_cast<unsigned int>(
		std::chrono::system_clock::now().time_since_epoch().count()));
	static std::uniform_int_distribution<> pick(0, alphanums.size() - 1);

	std::string s;

	s.reserve(length);

	while (length--)
		s += alphanums[pick(rg)];

	return s;
}