#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

std::vector<std::vector<int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
int n, m;
long long part1 = 0, part2 = 0;

std::string encode_pos(int i, int j) {
	return std::to_string(i) + "/" + std::to_string(j);
}

void trailheads_scores_dfs(
	const std::vector<std::vector<int>> &mp,
	std::vector<std::vector<std::unordered_set<std::string>>> &ends,
	std::vector<std::vector<int>> &trails,
	std::vector<std::vector<bool>> &explored, int i, int j) {

	if (explored[i][j]) {
		return;
	}

	explored[i][j] = true;

	if (mp[i][j] == 9) {
		ends[i][j].insert(encode_pos(i, j));
		trails[i][j]++;

		return;
	}

	for (const auto &dir : dirs) {
		int x = i + dir[0], y = j + dir[1];
		if (x < 0 || x >= n || y < 0 || y >= m || mp[x][y] - mp[i][j] != 1) {
			continue;
		}

		trailheads_scores_dfs(mp, ends, trails, explored, x, y);

		for (const auto &pos : ends[x][y]) {
			ends[i][j].insert(pos);
		}

		trails[i][j] += trails[x][y];
	}
}

void trailheads_scores(const std::vector<std::vector<int>> &mp) {
	std::vector<std::vector<std::unordered_set<std::string>>> ends(
		n, std::vector<std::unordered_set<std::string>>(m));
	std::vector<std::vector<int>> trails(n, std::vector<int>(m, 0));
	std::vector<std::vector<bool>> explored(n, std::vector<bool>(m, false));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (mp[i][j] != 0) {
				continue;
			}

			trailheads_scores_dfs(mp, ends, trails, explored, i, j);
			part1 += ends[i][j].size();
			part2 += trails[i][j];
		}
	}
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt");

	std::vector<std::vector<int>> mp;
	std::string ln;
	while (std::getline(f, ln)) {
		std::vector<int> v(ln.length());
		for (int i = 0; i < ln.length(); i++) {
			v[i] = (ln[i] - '0');
		}

		mp.push_back(v);
	}

	f.close();

	n = mp.size(), m = mp[0].size();

	auto start = std::chrono::high_resolution_clock::now();

	trailheads_scores(mp);

	auto stop = std::chrono::high_resolution_clock::now();
	auto dur =
		std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	printf("%ld microseconds\n", dur.count());

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	return 0;
}
