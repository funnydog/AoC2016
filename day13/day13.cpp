#include <fstream>
#include <deque>
#include <unordered_map>
#include <fmt/format.h>

using namespace std;

struct Vec
{
	int x;
	int y;
	int steps;

	Vec(int x, int y) : x(x), y(y), steps(0) {}
	Vec(int x, int y, int s) : x(x), y(y), steps(s) {}

	bool operator==(const Vec& other) const noexcept
	{
		return x == other.x && y == other.y;
	}
};

struct HashVec
{
	size_t operator()(const Vec& v) const
	{
		return (hash<int>{}(v.x)<4) ^ (hash<int>{}(v.y));
	}
};


static int is_wall(Vec v, int salt)
{
	int p = v.x * v.x + 3 * v.x  + 2 * v.x * v.y + v.y + v.y * v.y + salt;
	p ^= p>>1;
	p ^= p>>2;
	p ^= p>>4;
	p ^= p>>8;
	p ^= p>>16;
	return p & 1;
}

static const int dx[] = {0, 1, 0, -1};
static const int dy[] = {-1, 0, 1, 0};

static int distance(Vec start, Vec end, int salt)
{
	deque<Vec> queue;
	unordered_map<Vec, int, HashVec> dist;

	queue.push_back(start);
	dist[start] = 0;
	while (!queue.empty())
	{
		Vec pos = queue.front();
		queue.pop_front();
		if (pos == end)
		{
			break;
		}
		for (size_t i = 0; i < 4; i++)
		{
			Vec npos{pos.x+dx[i], pos.y+dy[i]};
			if (npos.x < 0 || npos.y < 0
			    || is_wall(npos, salt)
			    || dist.find(npos) != dist.end())
			{
				continue;
			}

			dist[npos] = dist[pos] + 1;
			queue.push_back(npos);
		}

	}
	if (queue.empty())
	{
		return -1;
	}
	return dist[end];
}

static int reachable(Vec start, int steps, int salt)
{
	deque<Vec> queue;
	unordered_map<Vec, int, HashVec> dist;

	start.steps = steps;
	queue.push_back(start);
	dist[start] = 0;
	while (!queue.empty())
	{
		Vec pos = queue.front();
		queue.pop_front();
		if (pos.steps == 0)
		{
			break;
		}
		for (size_t i = 0; i < 4; i++)
		{
			Vec npos{pos.x+dx[i], pos.y+dy[i], pos.steps-1};
			if (npos.x < 0 || npos.y < 0
			    || is_wall(npos, salt)
			    || dist.find(npos) != dist.end())
			{
				continue;
			}

			dist[npos] = dist[pos] + 1;
			queue.push_back(npos);
		}

	}
	if (queue.empty())
	{
		return -1;
	}
	return dist.size();
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

	int salt;
	input >> salt;
	input.close();

	if (input.fail())
	{
		fmt::print(stderr, "Cannot read the salt\n");
		return 1;
	}

	fmt::print("Part1: {}\n", distance(Vec{1,1}, Vec{31,39}, salt));
	fmt::print("Part2: {}\n", reachable(Vec{1, 1}, 50, salt));
	return 0;
}
