#include <chrono>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

int dirs[4][2]	 = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
long long part_1 = 0, part_2 = 0;
int lx, ly, sx = -1, sy, ex = -1, ey;

std::vector<std::vector<int>>
shortest_paths(const std::vector<std::string> &map) {
	std::vector<std::vector<int>> dst(lx, std::vector<int>(ly, INT_MAX));
	dst[sx][sy] = 0;

	std::priority_queue<std::tuple<int, int, int>,
						std::vector<std::tuple<int, int, int>>, std::greater<>>
		q;
	q.push({0, sx, sy});

	while (!q.empty()) {
		auto [d, x, y] = q.top();
		q.pop();

		for (const auto &dir : dirs) {
			int vx = x + dir[0], vy = y + dir[1];
			if (vx < 0 || vy < 0 || vx >= lx || vy >= ly ||
				map[vx][vy] == '#' || dst[vx][vy] != INT_MAX) {
				continue;
			}

			dst[vx][vy] = dst[x][y] + 1;
			q.push({dst[vx][vy], vx, vy});
		}
	}

	return dst;
}

long long cheat(const std::vector<std::string> &map,
				const std::vector<std::vector<int>> &dst, int chances) {
	long long ans = 0;

	for (int x = 0; x < lx; x++) {
		for (int y = 0; y < ly; y++) {
			if (map[x][y] == '#') {
				continue;
			}

			for (int r = 2; r <= chances; r++) {
				for (int rx = 0; rx <= r; rx++) {
					int ry = r - rx;

					std::set<std::pair<int, int>> vs = {{x + rx, y + ry},
														{x + rx, y - ry},
														{x - rx, y + ry},
														{x - rx, y - ry}};

					for (const auto &[vx, vy] : vs) {
						if (vx < 0 || vy < 0 || vx >= lx || vy >= ly ||
							map[vx][vy] == '#') {
							continue;
						}

						if (dst[x][y] - dst[vx][vy] >= 100 + r) {
							ans++;
						}
					}
				}
			}
		}
	}

	return ans;
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
		}

		if (sx != -1 && ex != -1) {
			break;
		}
	}

	auto start = std::chrono::high_resolution_clock::now();

	auto dst = shortest_paths(map);
	part_1	 = cheat(map, dst, 2);
	part_2	 = cheat(map, dst, 20);

	auto stop = std::chrono::high_resolution_clock::now();
	auto dur =
		std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	printf("%ld microseconds\n", dur.count());

	printf("%lld\n", part_1);
	printf("%lld\n", part_2);

	return 0;
}
