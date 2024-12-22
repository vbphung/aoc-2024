#include <chrono>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

long long part_1 = 0, part_2 = 0;

long long ways(const std::unordered_set<std::string> &pats,
			   std::unordered_map<std::string, long long> &memo,
			   std::string des) {
	if (memo.find(des) != memo.end()) {
		return memo[des];
	}

	if (pats.find(des) != pats.end()) {
		memo[des]++;
	}

	for (int i = 1; i < des.length(); i++) {
		auto pat = des.substr(0, i);
		if (pats.find(pat) != pats.end()) {
			auto rest = des.substr(i);
			memo[des] += ways(pats, memo, rest);
		}
	}

	return memo[des];
}

void possibles(const std::unordered_set<std::string> &pats,
			   const std::vector<std::string> &des) {
	std::unordered_map<std::string, long long> memo;
	for (const auto &d : des) {
		auto res = ways(pats, memo, d);
		if (res > 0) {
			part_1++;
		}

		part_2 += res;
	}
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt", std::ios::in);

	std::string ln;

	std::unordered_set<std::string> pats;
	std::vector<std::string> des;

	std::getline(f, ln);
	const char *del = ", ";
	char *tok		= strtok(&ln[0], del);
	while (tok) {
		pats.insert(tok);
		tok = strtok(NULL, del);
	}

	std::getline(f, ln);

	while (std::getline(f, ln)) {
		des.push_back(ln);
	}

	f.close();

	auto start = std::chrono::high_resolution_clock::now();

	possibles(pats, des);

	auto stop = std::chrono::high_resolution_clock::now();
	auto dur =
		std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	printf("%ld microseconds\n", dur.count());

	printf("%lld\n", part_1);
	printf("%lld\n", part_2);

	return 0;
}
