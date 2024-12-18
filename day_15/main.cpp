#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::unordered_map<char, std::vector<int>> dir = {
	{'^', {-1, 0}}, {'v', {1, 0}}, {'<', {0, -1}}, {'>', {0, 1}}};
int n, m;
long long part_1 = 0, part_2 = 0;

int valid(const std::vector<std::string> &map, int x, int y) {
	if (x < 0 || x >= n || y < 0 || y >= m || map[x][y] == '#') {
		return -1;
	}

	return map[x][y] == 'O' ? 0 : 1;
}

void box_coordinates(std::vector<std::string> map, const std::string &moves) {
	int x = -1, y = -1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (map[i][j] == '@') {
				x = i, y = j;
				break;
			}
		}

		if (x >= 0 || y >= 0) {
			break;
		}
	}

	for (char m : moves) {
		auto d = dir[m];
		int a = x + d[0], b = y + d[1];
		int v = valid(map, a, b);
		if (v == -1) {
			continue;
		}

		if (v == 1) {
			map[a][b] = '@';
			map[x][y] = '.';

			x = a, y = b;

			continue;
		}

		int p = a + d[0], q = b + d[1];
		v = valid(map, p, q);
		while (v == 0) {
			p += d[0], q += d[1];
			v = valid(map, p, q);
		}

		if (v == -1) {
			continue;
		}

		map[a][b] = '@';
		map[x][y] = '.';
		map[p][q] = 'O';

		x = a, y = b;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (map[i][j] == 'O') {
				part_1 += i * 100 + j;
			}
		}
	}
}

bool vertical_move(std::vector<std::string> &map, char m, int x, int y) {
	std::vector<std::unordered_set<int>> rows;
	int cur_x = x;
	rows.push_back({y});
	while (!rows.back().empty()) {
		std::unordered_set<int> next_bfs;
		cur_x += dir[m][0];
		for (auto cur_y : rows.back()) {
			switch (map[cur_x][cur_y]) {
			case '[':
				next_bfs.insert(cur_y);
				next_bfs.insert(cur_y + 1);
				break;

			case ']':
				next_bfs.insert(cur_y);
				next_bfs.insert(cur_y - 1);
				break;

			case '#':
				return false;
			}
		}

		if (next_bfs.empty()) {
			break;
		}

		rows.push_back(next_bfs);
	}

	while (cur_x != x) {
		auto boxes = rows.back();
		rows.pop_back();
		for (auto cur_y : boxes) {
			map[cur_x][cur_y]			  = map[cur_x - dir[m][0]][cur_y];
			map[cur_x - dir[m][0]][cur_y] = '.';
		}

		cur_x -= dir[m][0];
	}

	return true;
}

void larger_boxes(const std::vector<std::string> &map,
				  const std::string &moves) {
	std::vector<std::string> resized;
	for (const auto &ln : map) {
		std::string new_ln;
		for (char o : ln) {
			switch (o) {
			case '#':
				new_ln += "##";
				break;
			case 'O':
				new_ln += "[]";
				break;
			case '.':
				new_ln += "..";
				break;
			case '@':
				new_ln += "@.";
				break;
			}
		}

		resized.push_back(new_ln);
	}

	m *= 2;

	int x = -1, y = -1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (resized[i][j] == '@') {
				x = i, y = j;
				break;
			}
		}

		if (x >= 0 || y >= 0) {
			break;
		}
	}

	for (char m : moves) {
		if (m == '<' || m == '>') {
			int b = y + dir[m][1];
			while (resized[x][b] == '[' || resized[x][b] == ']') {
				b += dir[m][1];
			}

			if (resized[x][b] == '#') {
				continue;
			}

			for (int j = b; j != y; j -= dir[m][1]) {
				resized[x][j] = resized[x][j - dir[m][1]];
			}

			resized[x][y] = '.';
			y			  = y + dir[m][1];
		} else if (vertical_move(resized, m, x, y)) {
			x += dir[m][0];
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (resized[i][j] == '[') {
				part_2 += i * 100 + j;
			}
		}
	}
}

int main(int argc, char *argv[]) {
	std::fstream f("input.txt", std::ios::in);

	std::vector<std::string> map;

	std::string ln;
	while (std::getline(f, ln)) {
		if (ln.empty()) {
			break;
		}

		map.push_back(ln);
	}

	std::string moves;
	while (std::getline(f, ln)) {
		moves += ln;
	}

	f.close();

	n = map.size(), m = map[0].length();

	auto start = std::chrono::high_resolution_clock::now();

	box_coordinates(map, moves);
	larger_boxes(map, moves);

	auto stop = std::chrono::high_resolution_clock::now();
	auto dur =
		std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	printf("%ld microseconds\n", dur.count());

	std::cout << part_1 << std::endl;
	std::cout << part_2 << std::endl;

	return 0;
}
