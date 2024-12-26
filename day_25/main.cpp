#include <chrono>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define ll long long
#define M 7
#define N 5

ll part_1 = 0, part_2 = 0;
int L, T[N]			  = {5, 5, 5, 5, 5};
std::vector<std::vector<int>> locks, keys;

void cal_key_joins(const std::vector<std::vector<std::string>> &schematics) {
	for (int i = 0; i < L; i++) {
		std::vector<int> join(N, -1);
		for (int j = 0; j < M; j++) {
			for (int k = 0; k < N; k++) {
				if (schematics[i][j][k] == '#') {
					join[k]++;
				}
			}
		}

		if (schematics[i][0][0] == '#') {
			locks.push_back(join);
		} else {
			keys.push_back(join);
		}
	}
}

bool fit_pair(const std::vector<int> &l, const std::vector<int> &k) {
	for (int i = 0; i < N; i++) {
		if (l[i] + k[i] > T[i]) {
			return false;
		}
	}
	return true;
}

void fit_pairs() {
	for (const auto &l : locks) {
		for (const auto &k : keys) {
			if (fit_pair(l, k)) {
				part_1++;
			}
		}
	}
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt", std::ios::in);

	std::vector<std::vector<std::string>> schematics;

	std::string ln;
	while (true) {
		std::vector<std::string> lock;
		for (int i = 0; i < M; i++) {
			std::getline(f, ln);
			lock.push_back(ln);
		}

		schematics.push_back(lock);

		if (!std::getline(f, ln)) {
			break;
		}
	}

	f.close();

	L = schematics.size();

	auto start = std::chrono::high_resolution_clock::now();

	cal_key_joins(schematics);
	fit_pairs();

	auto stop = std::chrono::high_resolution_clock::now();
	auto dur =
		std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	printf("%ld microseconds\n", dur.count());
	printf("%lld\n", part_1);
	printf("%lld\n", part_2);

	return 0;
}
