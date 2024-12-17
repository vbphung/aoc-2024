#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#define N 103
#define M 101
#define T 100
#define T_LONG 10'000
const int N_MID = N / 2, M_MID = M / 2;
long long part_1 = 1;

void safety_factor(const std::vector<std::vector<int>> &rs) {
	std::vector<int> qs(4, 0);
	for (const auto &r : rs) {
		int m = ((r[0] + r[2] * T) % M + M) % M;
		int n = ((r[1] + r[3] * T) % N + N) % N;
		if (m < M_MID && n < N_MID) {
			qs[0]++;
		} else if (m < M_MID && n > N_MID) {
			qs[1]++;
		} else if (m > M_MID && n < N_MID) {
			qs[2]++;
		} else if (m > M_MID && n > N_MID) {
			qs[3]++;
		}
	}

	for (auto q : qs) {
		part_1 *= q;
	}
}

void chrismas_tree(const std::vector<std::vector<int>> &rs) {
	for (int t = 1; t <= T_LONG; t++) {

		std::vector<std::vector<int>> grid(N, std::vector<int>(M, 0));
		for (const auto &r : rs) {
			int m = ((r[0] + r[2] * t) % M + M) % M;
			int n = ((r[1] + r[3] * t) % N + N) % N;
			grid[n][m]++;
		}

		int max_row = 0;
		for (int i = 0; i < N; i++) {
			int row = 0;
			for (int j = 0; j < M; j++) {
				if (grid[i][j] >= 1) {
					row++;
				} else {
					max_row = std::max(max_row, row);
					row		= 0;
				}
			}
		}

		int max_col = 0;
		for (int j = 0; j < M; j++) {
			int col = 0;
			for (int i = 0; i < N; i++) {
				if (grid[i][j] >= 1) {
					col++;
				} else {
					max_col = std::max(max_col, col);
					col		= 0;
				}
			}
		}

		if (max_row < 10 || max_col < 10) {
			continue;
		}

		std::cout << t << std::endl;

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				std::cout << (grid[i][j] >= 1 ? '#' : '.');
			}

			std::cout << std::endl;
		}

		break;
	}
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt", std::ios::in);

	std::vector<std::vector<int>> rs;

	std::regex pattern(R"(\b(\d+),(\d+)\s+v=(-?\d+),(-?\d+))");
	std::smatch match;

	std::string ln;
	while (std::getline(f, ln)) {
		if (std::regex_search(ln, match, pattern)) {
			std::vector<int> r;
			for (int i = 1; i <= 4; i++) {
				r.push_back(std::stoi(match[i].str()));
			}

			rs.push_back(r);
		}
	}

	f.close();

	auto start = std::chrono::high_resolution_clock::now();

	safety_factor(rs);
	chrismas_tree(rs);

	auto stop = std::chrono::high_resolution_clock::now();
	auto dur =
		std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	printf("%ld microseconds\n", dur.count());

	std::cout << part_1 << std::endl;

	return 0;
}
