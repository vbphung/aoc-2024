#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

bool check_safe(const std::vector<int> &v) {
	if (v.size() <= 1) {
		return true;
	}

	int inc = 0;
	for (int i = 1; i < v.size(); i++) {
		int d = v[i] - v[i - 1];
		if (d * inc < 0 || std::abs(d) < 1 || std::abs(d) > 3) {
			return false;
		}
		inc = d;
	}

	return true;
}

bool check_safe_without_index(const std::vector<int> &v, int i) {
	std::vector<int> tol = v;
	tol.erase(tol.begin() + i);
	return check_safe(tol);
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt");
	std::string ln;
	int part1 = 0, part2 = 0;
	while (std::getline(f, ln)) {
		std::istringstream s(ln);
		std::vector<int> v;
		int n;
		while (s >> n) {
			v.push_back(n);
		}
		if (check_safe(v)) {
			part1++, part2++;
		} else {
			for (int j = 0; j < v.size(); j++) {
				if (check_safe_without_index(v, j)) {
					part2++;
					break;
				}
			}
		}
	}
	f.close();
	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;
	return 0;
}
