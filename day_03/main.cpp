#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

long mul(const std::string &ln) {
	long ans	  = 0;
	std::string s = ln;
	std::regex pattern(R"(mul\((\d+),(\d+)\))");
	std::smatch match;

	while (std::regex_search(s, match, pattern)) {
		int a = std::stoi(match[1].str()), b = std::stoi(match[2].str());
		ans += a * b;
		s = match.suffix().str();
	}

	return ans;
}

bool enabled = true;

long mul_with_instructions(const std::string &ln) {
	long ans	  = 0;
	std::string s = ln;
	std::regex pattern(R"(mul\((\d+),(\d+)\)|don't\(\)|do\(\))");
	std::smatch match;

	while (std::regex_search(s, match, pattern)) {
		if (match[0].str() == "do()") {
			enabled = true;
		} else if (match[0].str() == "don't()") {
			enabled = false;
		} else if (enabled) {
			int a = std::stoi(match[1].str()), b = std::stoi(match[2].str());
			ans += a * b;
		}

		s = match.suffix().str();
	}

	return ans;
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt");
	std::string ln;
	long part1 = 0, part2 = 0;
	while (std::getline(f, ln)) {
		part1 += mul(ln);
		part2 += mul_with_instructions(ln);
	}
	f.close();
	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;
	return 0;
}
