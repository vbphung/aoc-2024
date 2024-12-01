#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <unordered_map>
#include <vector>

void part_1(std::vector<int> x, std::vector<int> y) {
	std::sort(x.begin(), x.end());
	std::sort(y.begin(), y.end());
	long ans = 0;
	for (int i = 0; i < x.size(); i++) {
		ans += std::abs(x[i] - y[i]);
	}
	std::cout << ans << std::endl;
}

void part_2(std::vector<int> x, std::vector<int> y) {
	std::unordered_map<int, int> mp;
	for (auto n : y) {
		mp[n]++;
	}
	long ans = 0;
	for (auto n : x) {
		ans += n * mp[n];
	}
	std::cout << ans << std::endl;
}

int main() {
	std::fstream f("day_1.txt");
	int a, b;
	std::vector<int> x, y;
	while (f >> a >> b) {
		x.push_back(a), y.push_back(b);
	}
	f.close();
	part_1(x, y);
	part_2(x, y);
	return 0;
}
