#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int N, M;

bool valid_pos(int x, int y) { return x >= 0 && x < N && y >= 0 && y < M; }

int count_antinodes(
	std::vector<std::string> m,
	const std::unordered_map<char, std::vector<std::vector<int>>> &mp) {

	int ans = 0;

	for (const auto &[_, as] : mp) {
		for (int i = 0; i < as.size(); i++) {
			for (int j = i + 1; j < as.size(); j++) {
				int dx = as[i][0] - as[j][0], dy = as[i][1] - as[j][1];

				int nx = as[i][0] + dx, ny = as[i][1] + dy;
				if (valid_pos(nx, ny) && m[nx][ny] != '#') {
					ans++, m[nx][ny] = '#';
				}

				nx = as[j][0] - dx, ny = as[j][1] - dy;
				if (valid_pos(nx, ny) && m[nx][ny] != '#') {
					ans++, m[nx][ny] = '#';
				}
			}
		}
	}

	return ans;
}

int count_antinodes_extended(
	std::vector<std::string> m,
	const std::unordered_map<char, std::vector<std::vector<int>>> &mp) {

	int ans = 0;

	for (const auto &[_, as] : mp) {
		ans += as.size();

		for (int i = 0; i < as.size(); i++) {
			for (int j = i + 1; j < as.size(); j++) {
				int dx = as[i][0] - as[j][0], dy = as[i][1] - as[j][1];

				int nx = as[i][0] + dx, ny = as[i][1] + dy;
				while (valid_pos(nx, ny)) {
					if (m[nx][ny] == '.') {
						ans++, m[nx][ny] = '#';
					}

					nx += dx, ny += dy;
				}

				nx = as[j][0] - dx, ny = as[j][1] - dy;
				while (valid_pos(nx, ny)) {
					if (m[nx][ny] == '.') {
						ans++, m[nx][ny] = '#';
					}

					nx -= dx, ny -= dy;
				}
			}
		}
	}

	for (const auto &ln : m) {
		std::cout << ln << std::endl;
	}

	return ans;
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt");
	std::string ln;

	std::vector<std::string> m;
	int part1 = 0, part2 = 0;
	while (std::getline(f, ln)) {
		m.push_back(ln);
	}
	f.close();

	N = m.size();
	M = m[0].length();

	std::unordered_map<char, std::vector<std::vector<int>>> mp;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (m[i][j] != '.') {
				mp[m[i][j]].push_back({i, j});
			}
		}
	}

	part1 = count_antinodes(m, mp);
	part2 = count_antinodes_extended(m, mp);

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	return 0;
}
