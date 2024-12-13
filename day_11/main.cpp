#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

long long part_1 = 0, part_2 = 0;

int log10(long long n) {
	int l = 0;
	while (n > 0) {
		n /= 10, l++;
	}

	return l;
}

long long stones_after_blinks(
	std::unordered_map<long long, std::pmr::unordered_map<int, long long>>
		&cache,
	long long n, int lv) {
	if (lv == 0) {
		return 1;
	}

	if (cache.find(n) != cache.end() && cache[n].find(lv) != cache[n].end()) {
		return cache[n][lv];
	}

	if (n == 0) {
		return cache[n][lv] = stones_after_blinks(cache, 1, lv - 1);
	}

	auto l = log10(n);
	if (l % 2 == 0) {
		long long p			= std::pow(10, l / 2);
		return cache[n][lv] = stones_after_blinks(cache, n % p, lv - 1) +
							  stones_after_blinks(cache, n / p, lv - 1);
	}

	return cache[n][lv] = stones_after_blinks(cache, n * 2024, lv - 1);
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt", std::ios::in);

	std::vector<long long> v;
	std::string ln;
	if (std::getline(f, ln)) {
		std::stringstream s(ln);
		long long n;
		while (s >> n) {
			v.push_back(n);
		}
	} else {
		exit(1);
	}

	f.close();

	std::unordered_map<long long, std::pmr::unordered_map<int, long long>>
		cache;

	auto start = std::chrono::high_resolution_clock::now();

	for (auto n : v) {
		part_1 += stones_after_blinks(cache, n, 25);
	}

	for (auto n : v) {
		part_2 += stones_after_blinks(cache, n, 75);
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto dur =
		std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	printf("%ld microseconds\n", dur.count());

	std::cout << part_1 << std::endl;
	std::cout << part_2 << std::endl;

	return 0;
}
