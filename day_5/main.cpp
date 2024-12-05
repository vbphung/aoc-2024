#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

bool ok(const std::vector<std::unordered_set<int>> &rules,
		const std::vector<int> &update) {
	for (int i = 1; i < update.size(); i++) {
		for (int j = 0; j < i; j++) {
			if (rules[update[i]].find(update[j]) != rules[update[i]].end()) {
				return false;
			}
		}
	}

	return true;
}

std::vector<int> fix(const std::vector<std::unordered_set<int>> &rules,
					 const std::vector<int> &update) {
	std::vector<int> fixed = update;

	for (int i = 1; i < fixed.size(); i++) {
		int j = i;
		for (int k = i - 1; k >= 0; k--) {
			if (rules[fixed[j]].find(fixed[k]) != rules[fixed[j]].end()) {
				std::swap(fixed[j], fixed[k]);
				j = k;
			}
		}
	}

	return fixed;
}

int main() {
	std::fstream f("input.txt");
	std::string ln;
	long part1 = 0, part2 = 0;

	std::vector<std::unordered_set<int>> rules(100, std::unordered_set<int>());
	while (std::getline(f, ln)) {
		if (ln.empty()) {
			break;
		}

		int u = std::stoi(ln.substr(0, 2));
		int v = std::stoi(ln.substr(3));
		rules[u].insert(v);
	}

	while (std::getline(f, ln)) {
		std::stringstream s(ln);
		std::string n;
		std::vector<int> update;
		while (std::getline(s, n, ',')) {
			update.push_back(std::stoi(n));
		}

		if (ok(rules, update)) {
			part1 += update[update.size() / 2];
		} else {
			auto fixed = fix(rules, update);
			part2 += fixed[fixed.size() / 2];
		}
	}

	f.close();

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	return 0;
}
