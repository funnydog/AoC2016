#include <climits>
#include <fstream>
#include <deque>
#include <vector>
#include <unordered_map>

#include <fmt/format.h>

using namespace std;

struct Pos
{
	int x;
	int y;

	bool operator==(const Pos& p) const noexcept
	{
		return x == p.x && y == p.y;
	}
};

struct PosHash
{
	size_t operator()(const Pos& p) const noexcept
	{
		return (hash<int>{}(p.x)<<5)^(hash<int>{}(p.y));
	}
};

struct Location
{
	int value;
	Pos pos;
};

static int distance(const vector<string>& map, Pos start, Pos end)
{
	static const int dx[] = {0, 1, 0, -1};
	static const int dy[] = {-1, 0, 1, 0};
	unordered_map<Pos, int, PosHash> cost;
	deque<Pos> queue;

	queue.push_back(start);
	cost[start] = 0;
	while (!queue.empty())
	{
		Pos pos = queue.front();
		if (pos == end)
		{
			break;
		}
		queue.pop_front();
		for (int i = 0; i < 4; i++)
		{
			Pos np{pos.x+dx[i], pos.y+dy[i]};
			if (map[np.y][np.x] == '#')
			{
				continue;
			}
			if (cost.find(np) != cost.end())
			{
				continue;
			}
			cost[np] = cost[pos] + 1;
			queue.push_back(np);
		}
	}
	return cost[end];
}

static pair<int, int> solve(const vector<string>& map)
{
	// find the locs in the map
	vector<Location> locs;
	for (size_t y = 0; y < map.size(); y++)
	{
		for (size_t x = 0; x < map[y].size(); x++)
		{
			switch (map[y][x])
			{
			case '#':
			case '.':
				break;
			default:
				locs.emplace_back(
					Location{map[y][x], Pos{(int)x, (int)y}}
					);
				break;
			}
		}
	}
	sort(locs.begin(), locs.end(), [](const auto& a, const auto& b) {
		return a.value < b. value;
	});

	// for each location couple find the distance
	vector<vector<int>> dist(locs.size());
	for (auto& d: dist)
	{
		d.resize(locs.size());
	}
	for (size_t i = 0; i < locs.size(); i++)
	{
		for (size_t j = 0; j < i; j++)
		{
			dist[j][i] = dist[i][j] = distance(
				map, locs[i].pos, locs[j].pos);
		}
	}

	// find the length
	vector<int> indices;
	for (size_t i = 1; i < locs.size(); i++)
	{
		indices.push_back(i);
	}

	int part1 = INT_MAX;
	int part2 = INT_MAX;
	do
	{
		int len = dist[0][indices[0]];
		for (size_t i = 1; i < indices.size(); i++)
		{
			len += dist[indices[i-1]][indices[i]];
		}
		if (part1 > len)
		{
			part1 = len;
		}
		len += dist[0][indices.back()];
		if (part2 > len)
		{
			part2 = len;
		}
	} while (next_permutation(indices.begin(), indices.end()));

	return make_pair(part1, part2);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fmt::print(stderr, "Usage: {} <filename>\n", argv[0]);
		return 1;
	}

	ifstream input(argv[1]);
	if (!input)
	{
		fmt::print(stderr, "Cannot open {}\n", argv[1]);
		return 1;
	}

	vector<string> map;
	string line;
	while (getline(input, line))
	{
		map.emplace_back(move(line));
	}
	input.close();

	auto [part1, part2] = solve(map);
	fmt::print("Part1: {}\nPart2: {}\n", part1, part2);
	return 0;
}
