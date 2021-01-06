#include <deque>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <fmt/format.h>

using namespace std;

struct Pos
{
	int x;
	int y;
};

bool operator==(const Pos& a, const Pos& b)
{
	return a.x == b.x && a.y == b.y;
}

struct PosHash
{
	size_t operator()(const Pos& p) const noexcept
	{
		return (hash<int>{}(p.x)<<4) ^ hash<int>{}(p.y);
	}
};

struct Node
{
	Pos pos;
	int used;
	int avail;
	bool movable;
};

using NodeMap = unordered_map<Pos, Node, PosHash>;
using Path = unordered_map<Pos, Pos, PosHash>;

static NodeMap load(istream& input)
{
	NodeMap rv;
	static const regex pat("\\/dev\\/grid\\/node-x(\\d+)-y(\\d+)\\s+\\d+T\\s+(\\d+)T\\s+(\\d+)T\\s+\\d+%");
	string line;
	while (getline(input, line))
	{
		smatch sm;
		Node n{};
		if (regex_match(line, sm, pat))
		{
			n.pos.x = stoi(sm[1]);
			n.pos.y = stoi(sm[2]);
			n.used = stoi(sm[3]);
			n.avail = stoi(sm[4]);
			n.movable = false;
		}
		rv[n.pos] = n;
	}
	return rv;
}

static size_t viable_nodes(NodeMap& nmap)
{
	size_t viable = 0;
	for (auto a = nmap.begin(); a != nmap.end(); ++a)
	{
		for (auto b = nmap.begin(); b != a; ++b)
		{
			if (0 < a->second.used && a->second.used <= b->second.avail)
			{
				a->second.movable = true;
				viable++;
			}
			if (0 < b->second.used && b->second.used <= a->second.avail)
			{
				b->second.movable = true;
				viable++;
			}
		}
	}
	return viable;
}

static void bfs(const NodeMap& map, const Pos& start, const Pos& end, Path& path)
{
	static const int dx[] = {0, 1, 0, -1};
	static const int dy[] = {-1, 0, 1, 0};
	path.clear();
	deque<Pos> queue;
	queue.push_back(start);
	path[start] = Pos{-1,-1};
	while (!queue.empty())
	{
		Pos pos = queue.front();
		queue.pop_front();

		for (int i = 0; i < 4; i++)
		{
			Pos npos{pos.x+dx[i], pos.y+dy[i]};
			auto it = map.find(npos);
			if (it == map.end() || !it->second.movable)
			{
				continue;
			}

			if (path.find(npos) != path.end())
			{
				continue;
			}

			path[npos] = pos;
			queue.push_back(npos);
		}
	}
}

static size_t count_moving_steps(NodeMap& map)
{
	// find the empty node
	Pos empty{};
	for (auto& [pos, node]: map)
	{
		if (node.used == 0)
		{
			node.movable = true;
			empty = pos;
			break;
		}
	}

	// find the goal position
	Pos goal{};
	for (auto& [pos, node]: map)
	{
		if (pos.y == 0 && goal.x < pos.x)
		{
			goal.x = pos.x;
		}
	}

	// find the path from the start to the goal
	Pos start{0, 0};
	Path goal_path;
	bfs(map, start, goal, goal_path);

	// set the path as not movable to avoid the empty node to move
	// into it
	map[goal].movable = false;

	// now move the goal along the path using the empty node as a
	// useful aid
	size_t steps = 0;
	Pos p = goal_path[goal];
	while (!(p == Pos{-1, -1}))
	{
		Path empty_path;
		bfs(map, p, empty, empty_path);
		Pos next = empty_path[empty];
		while (!(next == Pos{-1,-1}))
		{
			swap(map[empty], map[next]);
			empty = next;
			next = empty_path[next];
			steps++;
		}

		swap(map[goal], map[p]);
		empty = goal;
		goal = p;
		p = goal_path[p];
		steps++;
	}
	return steps;
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

	NodeMap map = load(input);
	input.close();

	fmt::print("Part1: {}\n", viable_nodes(map));
	fmt::print("Part2: {}\n", count_moving_steps(map));
	return 0;
}
