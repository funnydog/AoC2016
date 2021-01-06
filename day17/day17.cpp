#include <fstream>
#include <deque>
#include <unordered_map>
#include <vector>
#include <fmt/format.h>

#include <openssl/md5.h>

using namespace std;

struct Position
{
	string path;
	int x;
	int y;
	bool open[4];

	Position() : path(""), x(0), y(0), open{} {}
	Position(const string& path, int x, int y) : path(path), x(x), y(y), open{} {}

	bool operator==(const Position& other) const noexcept
	{
		return path == other.path
			&& x == other.x
			&& y == other.y;
	}

	void compute_open(const string& passcode) noexcept
	{
		MD5_CTX c;
		unsigned char buffer[MD5_DIGEST_LENGTH];
		MD5_Init(&c);
		MD5_Update(&c, passcode.c_str(), passcode.size());
		MD5_Update(&c, path.c_str(), path.size());
		MD5_Final(buffer, &c);
		unsigned val = (unsigned)buffer[1] | ((unsigned)buffer[0] << 8);
		for (size_t i = 0; i < 4; i++)
		{
			open[i] = (val&0xf000) >= 0xb000;
			val <<= 4;
		}
	}
};

struct PositionHash
{
	size_t operator()(const Position& p) const noexcept
	{
		return hash<string>{}(p.path) ^ (hash<int>{}(p.x)<<4)
					     ^ (hash<int>{}(p.y)<<2);
	}
};

static void find_neighbours(const Position& pos, vector<Position>& out)
{
	static const char letters[] = {'U', 'D', 'L', 'R'};
	static const int dx[] = {0, 0, -1, 1};
	static const int dy[] = {-1, 1, 0, 0};
	out.clear();
	for (size_t i = 0; i < 4; i++)
	{
		if (!pos.open[i])
		{
			continue;
		}
		int nx = pos.x + dx[i];
		int ny = pos.y + dy[i];
		if (0 <= nx && nx < 4 && 0 <= ny && ny < 4)
		{
			out.emplace_back(
				Position(pos.path+letters[i], nx, ny)
				);
		}
	}
}

static string shortest_path(const string& passcode)
{
	vector<Position> neighbours(4);
	deque<Position> queue;
	unordered_map<Position, int, PositionHash> cost;
	Position pos;
	cost[pos] = 0;
	queue.emplace_back(move(pos));
	while (!queue.empty())
	{
		pos = queue.front();
		queue.pop_front();
		if (pos.x == 3 && pos.y == 3)
		{
			return pos.path;
		}

		pos.compute_open(passcode);
		find_neighbours(pos, neighbours);
		for (auto& npos: neighbours)
		{
			if (cost.find(npos) != cost.end()
			    && cost[pos] < cost[npos])
			{
				continue;
			}

			cost[npos] = cost[pos]+1;
			queue.emplace_back(move(npos));
		}
	}
	return "";
}

static size_t longest_path_len(const string& passcode)
{
	size_t longest = 0;
	vector<Position> neighbours(4);
	deque<Position> queue;
	unordered_map<Position, int, PositionHash> cost;
	Position pos;
	cost[pos] = 0;
	queue.emplace_back(move(pos));
	while (!queue.empty())
	{
		pos = queue.front();
		queue.pop_front();
		if (pos.x == 3 && pos.y == 3)
		{
			if (longest < pos.path.size())
			{
				longest = pos.path.size();
			}
			continue;
		}

		pos.compute_open(passcode);
		find_neighbours(pos, neighbours);
		for (auto& npos: neighbours)
		{
			if (cost.find(npos) != cost.end()
			    && cost[pos] > cost[npos])
			{
				continue;
			}

			cost[npos] = cost[pos]+1;
			queue.emplace_back(move(npos));
		}
	}
	return longest;
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

	string passcode;
	input >> passcode;
	input.close();
	if (input.fail())
	{
		fmt::print(stderr, "Cannot parse the data\n");
		return 1;
	}

	fmt::print("Part1: {}\n", shortest_path(passcode));
	fmt::print("Part2: {}\n", longest_path_len(passcode));
	return 0;
}
