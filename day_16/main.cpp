#include <chrono>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct pos {
	int x, y;
	char dir;
};

std::unordered_map<char, std::vector<int>> dirs = {
	{'^', {-1, 0}}, {'v', {1, 0}}, {'<', {0, -1}}, {'>', {0, 1}}};
std::unordered_map<char, int> dir_index = {
	{'^', 0}, {'v', 1}, {'<', 2}, {'>', 3}};

std::vector<std::vector<std::vector<int>>> scores;
std::vector<std::vector<std::vector<std::vector<pos>>>> trace;
int lx, ly, sx = -1, sy, ex = -1, ey;
long long part_1 = INT_MAX, part_2 = 0;

void shortest_path(const std::vector<std::string> &map, int x, int y, char dir,
				   int cur_score) {
	auto pos_hash = [](const pos &p) {
		return p.y + 1000 * p.x + 1000'000 * p.dir;
	};
	auto pos_eq = [](const pos &p1, const pos &p2) {
		return p1.x == p2.x && p1.y == p2.y && p1.dir == p2.dir;
	};
	std::unordered_set<pos, decltype(pos_hash), decltype(pos_eq)> visited(
		0, pos_hash, pos_eq);

	scores = std::vector<std::vector<std::vector<int>>>(
		lx, std::vector<std::vector<int>>(ly, std::vector<int>(4, INT_MAX)));

	std::priority_queue<std::tuple<int, int, int, char>,
						std::vector<std::tuple<int, int, int, char>>,
						std::greater<>>
		pq;

	trace = std::vector<std::vector<std::vector<std::vector<pos>>>>(
		lx, std::vector<std::vector<std::vector<pos>>>(
				ly, std::vector<std::vector<pos>>(4)));

	scores[x][y][dir_index[dir]] = 0;
	pq.push({0, x, y, dir});

	while (!pq.empty()) {
		auto [u_score, ux, uy, ud] = pq.top();
		pq.pop();

		pos u = {ux, uy, ud};
		if (visited.find(u) != visited.end()) {
			continue;
		}

		visited.insert(u);

		for (auto &[d, vec] : dirs) {
			int vx = ux + vec[0], vy = uy + vec[1];
			if (vx < 0 || vx >= lx || vy < 0 || vy >= ly ||
				map[vx][vy] == '#') {
				continue;
			}

			int v_score = u_score + 1 + (ud != d) * 1000;

			if (v_score < scores[vx][vy][dir_index[d]]) {
				scores[vx][vy][dir_index[d]] = v_score;
				trace[vx][vy][dir_index[d]]	 = {u};
				pq.push({v_score, vx, vy, d});
			} else if (v_score == scores[vx][vy][dir_index[d]]) {
				trace[vx][vy][dir_index[d]].push_back(u);
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		if (scores[ex][ey][i] < part_1) {
			part_1 = scores[ex][ey][i];
		}
	}
}

inline int pos_key(int x, int y) { return x + 1000 * y; }

void trace_shortest_paths(std::vector<std::string> &map) {
	std::unordered_set<int> tiles;
	tiles.insert(pos_key(ex, ey));
	tiles.insert(pos_key(sx, sy));

	std::vector<pos> next;
	for (int k = 0; k < 4; k++) {
		if (scores[ex][ey][k] == part_1) {
			auto vs = trace[ex][ey][k];
			for (const auto &v : vs) {
				if (v.x == sx && v.y == sy) {
					continue;
				}

				tiles.insert(pos_key(v.x, v.y));
				next.push_back(v);
			}
		}
	}

	while (!next.empty()) {
		auto u = next.back();
		next.pop_back();

		auto vs = trace[u.x][u.y][dir_index[u.dir]];
		for (const auto &v : vs) {
			if (v.x == sx && v.y == sy) {
				continue;
			}

			tiles.insert(pos_key(v.x, v.y));
			next.push_back(v);
		}
	}

	part_2 = tiles.size();

	for (const auto &n : tiles) {
		int x = n % 1000, y = n / 1000;
		map[x][y] = 'O';
	}
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt", std::ios::in);

	std::vector<std::string> map;

	std::string ln;
	while (std::getline(f, ln)) {
		map.push_back(ln);
	}

	f.close();

	lx = map.size(), ly = map[0].length();
	for (int i = 0; i < lx; i++) {
		for (int j = 0; j < ly; j++) {
			if (map[i][j] == 'S') {
				sx = i, sy = j;
			} else if (map[i][j] == 'E') {
				ex = i, ey = j;
			}

			if (sx >= 0 && ex >= 0) {
				break;
			}
		}
	}

	auto start = std::chrono::high_resolution_clock::now();

	shortest_path(map, sx, sy, '>', 0);
	trace_shortest_paths(map);

	auto stop = std::chrono::high_resolution_clock::now();
	auto dur =
		std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	for (int i = 0; i < lx; i++) {
		for (int j = 0; j < ly; j++) {
			std::cout << map[i][j];
		}

		std::cout << std::endl;
	}

	printf("%ld microseconds\n", dur.count());

	printf("%lld\n", part_1);
	printf("%lld\n", part_2);

	return 0;
}
