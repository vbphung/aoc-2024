#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#define ADDITION 10000000000000

long long part_1 = 0, part_2 = 0;

void fewest_tokens(long long ax, long long ay, long long bx, long long by,
				   long long px, long long py, long long &ans) {
	auto det = ax * by - ay * bx;
	if (det == 0) {
		return;
	}

	auto cofactor_a = (px * by - py * bx), cofactor_b = (ax * py - ay * px);
	if (cofactor_a % det != 0 || cofactor_b % det != 0) {
		return;
	}

	auto a = cofactor_a / det, b = cofactor_b / det;
	ans += 3 * a + b;
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt", std::ios::in);

	std::regex a_regex(R"(Button A: X\+(\d+), Y\+(\d+))"),
		b_regex(R"(Button B: X\+(\d+), Y\+(\d+))"),
		prize_regex(R"(Prize: X=(\d+), Y=(\d+))");
	std::smatch match;

	std::string ln;
	while (std::getline(f, ln)) {
		std::regex_search(ln, match, a_regex);
		int ax = std::stoi(match[1].str()), ay = std::stoi(match[2].str());

		std::getline(f, ln);
		std::regex_search(ln, match, b_regex);
		int bx = std::stoi(match[1].str()), by = std::stoi(match[2].str());

		std::getline(f, ln);
		std::regex_search(ln, match, prize_regex);
		int px = std::stoi(match[1].str()), py = std::stoi(match[2].str());

		fewest_tokens(ax, ay, bx, by, px, py, part_1);
		fewest_tokens(ax, ay, bx, by, px + ADDITION, py + ADDITION, part_2);

		std::getline(f, ln);
	}

	f.close();

	std::cout << part_1 << std::endl;
	std::cout << part_2 << std::endl;

	return 0;
}
