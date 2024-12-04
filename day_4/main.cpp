#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

long count_xmas(const std::vector<std::string> &v) {
	long ans						   = 0;
	std::vector<std::vector<int>> dirs = {{0, 1}, {1, 0},  {-1, 0}, {0, -1},
										  {1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
	std::string xmas				   = "MAS";
	int n = v.size(), m = v[0].length();

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (v[i][j] != 'X') {
				continue;
			}

			for (const auto &d : dirs) {
				std::string s;
				int x = i, y = j;

				for (int k = 1; k < 4; k++) {
					x += d[0];
					y += d[1];
					if (x >= 0 && x < n && y >= 0 && y < m) {
						s += v[x][y];
					} else {
						break;
					}
				}

				if (s == xmas) {
					ans++;
				}
			}
		}
	}

	return ans;
}

long count_x_mas(const std::vector<std::string> &v) {
	long ans	   = 0;
	std::string ms = "MS", sm = "SM";
	int n = v.size(), m = v[0].length();

	auto is_valid_index = [n, m](int x, int y) -> bool {
		return x >= 0 && x < n && y >= 0 && y < m;
	};

	std::vector<std::vector<int>> dirs = {{1, 1}, {-1, -1}, {-1, 1}, {1, -1}};

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (v[i][j] != 'A') {
				continue;
			}

			std::string a, b;

			for (int k = 0; k < 2; k++) {
				int x = i + dirs[k][0], y = j + dirs[k][1];
				if (is_valid_index(x, y)) {
					a += v[x][y];
				}
			}

			if (a != ms && a != sm) {
				continue;
			}

			for (int k = 2; k < 4; k++) {
				int x = i + dirs[k][0], y = j + dirs[k][1];
				if (is_valid_index(x, y)) {
					b += v[x][y];
				}
			}

			if (b == ms || b == sm) {
				ans++;
			}
		}
	}

	return ans;
}

int main() {
	std::fstream f("input.txt");

	std::string ln;
	long part1 = 0, part2 = 0;
	std::vector<std::string> v;

	while (std::getline(f, ln)) {
		v.push_back(ln);
	}
	f.close();

	part1 = count_xmas(v);
	part2 = count_x_mas(v);

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	return 0;
}
