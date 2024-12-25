#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#define ll unsigned long long int

ll part_1 = 0, part_2 = 0;
std::unordered_map<char, std::pair<int, int>> dirs = {
	{'^', {-1, 0}}, {'v', {1, 0}}, {'<', {0, -1}}, {'>', {0, 1}}};
std::unordered_map<char, std::pair<int, int>> numeric_kb = {
	{'7', {0, 0}}, {'8', {0, 1}}, {'9', {0, 2}}, {'4', {1, 0}},
	{'5', {1, 1}}, {'6', {1, 2}}, {'1', {2, 0}}, {'2', {2, 1}},
	{'3', {2, 2}}, {'0', {3, 1}}, {'A', {3, 2}}};
std::unordered_map<char, std::pair<int, int>> remote_control = {
	{'^', {0, 1}}, {'A', {0, 2}}, {'<', {1, 0}}, {'v', {1, 1}}, {'>', {1, 2}}};
std::unordered_map<std::string, std::unordered_map<int, ll>> cost_cache;

bool valid_pos(const std::unordered_map<char, std::pair<int, int>> &kb, int x,
			   int y) {
	for (const auto &[c, p] : kb) {
		if (p.first == x && p.second == y) {
			return true;
		}
	}

	return false;
}

bool valid_command(const std::unordered_map<char, std::pair<int, int>> &kb,
				   const std::string &moves, int x, int y) {
	for (auto c : moves) {
		auto [dx, dy] = dirs[c];
		x += dx, y += dy;
		if (!valid_pos(kb, x, y)) {
			return false;
		}
	}

	return true;
}

ll command_cost(const std::unordered_map<char, std::pair<int, int>> &kb,
				const std::string &code, int lv) {
	if (lv <= 0) {
		return code.length();
	}

	if (cost_cache.find(code) != cost_cache.end() &&
		cost_cache[code].find(lv) != cost_cache[code].end()) {
		return cost_cache[code][lv];
	}

	ll ans = 0;

	auto [x, y] = kb.at('A');
	for (auto c : code) {
		auto [vx, vy] = kb.at(c);
		auto dx = vx - x, dy = vy - y;

		std::string cur;
		cur.append(std::abs(dx), dx > 0 ? 'v' : '^');
		cur.append(std::abs(dy), dy > 0 ? '>' : '<');

		ll best_cost = ULLONG_MAX;

		std::sort(cur.begin(), cur.end());
		do {
			if (!valid_command(kb, cur, x, y)) {
				continue;
			}

			auto cost = command_cost(remote_control, cur + "A", lv - 1);
			best_cost = std::min(best_cost, cost);
		} while (std::next_permutation(cur.begin(), cur.end()));

		ans += best_cost;
		x = vx, y = vy;
	}

	return cost_cache[code][lv] = ans;
}

void complexity(const std::vector<std::string> &codes) {
	for (const auto &code : codes) {
		auto numeric = std::stoi(code.substr(0, code.length() - 1));

		auto cmd_cost = command_cost(numeric_kb, code, 3);
		part_1 += numeric * cmd_cost;

		cmd_cost = command_cost(numeric_kb, code, 26);
		part_2 += numeric * cmd_cost;
	}
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt", std::ios::in);

	std::vector<std::string> codes;

	std::string ln;
	while (std::getline(f, ln)) {
		codes.push_back(ln);
	}

	f.close();

	auto start = std::chrono::high_resolution_clock::now();

	complexity(codes);

	auto stop = std::chrono::high_resolution_clock::now();
	auto dur =
		std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	printf("%ld microseconds\n", dur.count());

	printf("%lld\n", part_1);
	printf("%lld\n", part_2);

	return 0;
}
