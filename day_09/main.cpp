#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

long long filesystem_checksum(const std::vector<int> &ns) {
	std::vector<int> v;
	for (int i = 0; i < ns.size(); i++) {
		if (i % 2 == 1) {
			v.insert(v.end(), ns[i], -1);
		} else {
			v.insert(v.end(), ns[i], i / 2);
		}
	}

	int i = 0, j = v.size() - 1;
	while (i < j) {
		if (v[i] == -1 && v[j] != -1) {
			std::swap(v[i], v[j]);
			continue;
		}

		if (v[i] != -1) {
			i++;
		}

		if (v[j] == -1) {
			j--;
		}
	}

	long long ans = 0;
	for (int i = 0; i < v.size() && v[i] != -1; i++) {
		ans += i * v[i];
	}

	return ans;
}

long long chunk_filesystem_checksum(const std::vector<int> &ns) {
	std::unordered_map<int, int> sz, pos;
	int j = 0, l = 0;
	for (int i = 0; i < ns.size(); i++) {
		int k = i % 2 == 1 ? -(i / 2 + 1) : i / 2;
		if (i % 2 == 0) {
			j = k;
		}

		sz[k]  = ns[i];
		pos[k] = l;
		l += ns[i];
	}

	while (j > 0) {
		for (int i = 1; i <= j; i++) {
			if (sz[-i] < sz[j]) {
				continue;
			}

			sz[-i] -= sz[j];
			pos[j] = pos[-i];
			pos[-i] += sz[j];

			break;
		}

		j--;
	}

	long long ans = 0;
	for (const auto &[n, p] : pos) {
		if (n < 0) {
			continue;
		}

		for (int i = 0; i < sz[n]; i++) {
			ans += n * (p + i);
		}
	}

	return ans;
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt");

	std::vector<int> ns;
	std::string ln;
	std::getline(f, ln);
	for (char c : ln) {
		ns.push_back(c - '0');
	}

	f.close();

	long long part1 = filesystem_checksum(ns);
	long long part2 = chunk_filesystem_checksum(ns);

	std::cout << part1 << std::endl;
	std::cout << part2 << std::endl;

	return 0;
}
