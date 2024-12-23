#include <chrono>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

#define MEM_SPACE 70
#define COUNTED_BYTES 1024

const int SPACE = MEM_SPACE + 1;
int dirs[4][2]	= {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
std::vector<std::vector<char>> map;
long long part_1		= 0;
std::vector<int> part_2 = {-1, -1};

int shortest_path(const std::vector<std::vector<char>> &map) {
	std::vector<std::vector<bool>> vst(SPACE, std::vector<bool>(SPACE, false));
	std::vector<std::vector<int>> dst(SPACE, std::vector<int>(SPACE, INT_MAX));
	dst[0][0] = 0;

	std::priority_queue<std::tuple<int, int, int>,
						std::vector<std::tuple<int, int, int>>, std::greater<>>
		q;
	q.push({0, 0, 0});

	while (!q.empty()) {
		auto [d, x, y] = q.top();
		q.pop();

		if (vst[x][y]) {
			continue;
		}

		vst[x][y] = true;

		for (const auto &dir : dirs) {
			int vx = x + dir[0], vy = y + dir[1];
			if (vx < 0 || vy < 0 || vx >= SPACE || vy >= SPACE ||
				map[vx][vy] == '#') {
				continue;
			}

			int vd = dst[x][y] + 1;

			if (dst[vx][vy] > vd) {
				dst[vx][vy] = vd;
				q.push({dst[vx][vy], vx, vy});
			}
		}
	}

	return dst[MEM_SPACE][MEM_SPACE];
}

void minimum_steps(const std::vector<std::vector<int>> &pos) {
	map = std::vector<std::vector<char>>(SPACE, std::vector<char>(SPACE, '.'));
	for (int i = 0; i < COUNTED_BYTES; i++) {
		int x = pos[i][0], y = pos[i][1];
		map[x][y] = '#';
	}

	part_1 = shortest_path(map);
}

void chase_falls(const std::vector<std::vector<int>> &pos) {
	for (int i = COUNTED_BYTES; i < pos.size(); i++) {
		int x = pos[i][0], y = pos[i][1];
		map[x][y] = '#';
		if (shortest_path(map) == INT_MAX) {
			part_2 = {x, y};
			return;
		}
	}
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt", std::ios::in);

	std::vector<std::vector<int>> pos;

	std::string ln;
	while (std::getline(f, ln)) {
		int comma = ln.find_first_of(',');
		int y	  = std::stoi(ln.substr(0, comma));
		int x	  = std::stoi(ln.substr(comma + 1));

		pos.push_back({x, y});
	}

	f.close();

	auto start = std::chrono::high_resolution_clock::now();

	minimum_steps(pos);
	chase_falls(pos);

	auto stop = std::chrono::high_resolution_clock::now();
	auto dur =
		std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	printf("%ld microseconds\n", dur.count());

	printf("%lld\n", part_1);
	printf("%d,%d\n", part_2[1], part_2[0]);

	return 0;
}
