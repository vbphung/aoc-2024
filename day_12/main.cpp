#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::vector<int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
int n, m;
long long part_1 = 0, part_2 = 0;

void count(const std::vector<std::vector<bool>> &region, bool &fence,
		   long long &ans, int i, int j, int x, int y) {
	if (!region[i][j]) {
		fence = false;
		return;
	} else if (x < 0 || x >= n || y < 0 || y >= m || !region[x][y]) {
		if (!fence) {
			fence = true, ans++;
		}
	} else {
		fence = false;
	}
}

long long count_sides(const std::vector<std::vector<bool>> &region) {
	long long ans = 0;
	bool fence	  = false;

	for (int i = 0; i < n; i++) {
		bool fence = false;
		for (int j = 0; j < m; j++) {
			count(region, fence, ans, i, j, i - 1, j);
		}

		fence = false;
		for (int j = 0; j < m; j++) {
			count(region, fence, ans, i, j, i + 1, j);
		}
	}

	for (int j = 0; j < m; j++) {
		bool fence = false;
		for (int i = 0; i < n; i++) {
			count(region, fence, ans, i, j, i, j - 1);
		}

		fence = false;
		for (int i = 0; i < n; i++) {
			count(region, fence, ans, i, j, i, j + 1);
		}
	}

	return ans;
}

void total_price_dfs(const std::vector<std::string> &mp,
					 std::vector<std::vector<bool>> &explored,
					 std::vector<std::vector<bool>> &region, long long &area,
					 long long &perimeter, int i, int j) {
	if (explored[i][j]) {
		return;
	}

	explored[i][j] = true;
	region[i][j]   = true;
	area++;

	perimeter += 4;
	for (int k = 0; k < 4; k++) {
		int x = i + dirs[k][0], y = j + dirs[k][1];
		if (x >= 0 && x < n && y >= 0 && y < m && mp[x][y] == mp[i][j] &&
			explored[x][y]) {
			perimeter -= 2;
		}
	}

	for (int k = 0; k < 4; k++) {
		int x = i + dirs[k][0], y = j + dirs[k][1];
		if (x >= 0 && x < n && y >= 0 && y < m && mp[x][y] == mp[i][j]) {
			total_price_dfs(mp, explored, region, area, perimeter, x, y);
		}
	}
}

void total_price(const std::vector<std::string> &mp) {
	std::vector<std::vector<bool>> explored(n, std::vector<bool>(m, false));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			long long area = 0, perimeter = 0;
			std::vector<std::vector<bool>> region(n,
												  std::vector<bool>(m, false));
			total_price_dfs(mp, explored, region, area, perimeter, i, j);
			if (area == 0) {
				continue;
			}

			part_1 += area * perimeter;

			long long sides = count_sides(region);
			part_2 += area * sides;
		}
	}
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt");

	std::vector<std::string> mp;
	std::string ln;
	while (std::getline(f, ln)) {
		mp.push_back(ln);
	}

	f.close();

	n = mp.size(), m = mp[0].size();

	auto start = std::chrono::high_resolution_clock::now();

	total_price(mp);

	auto stop = std::chrono::high_resolution_clock::now();
	auto dur =
		std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	printf("%ld microseconds\n", dur.count());

	std::cout << part_1 << std::endl;
	std::cout << part_2 << std::endl;

	return 0;
}
