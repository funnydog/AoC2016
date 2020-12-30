#include <fstream>
#include <unordered_map>

#include <fmt/format.h>

using namespace std;

struct Pos
{
	int x;
	int y;

	size_t manhattan() const noexcept
	{
		return (x>0?x:-x) + (y>0?y:-y);
	}
};

static bool operator==(Pos a, Pos b)
{
	return a.x == b.x && a.y == b.y;
}

static Pos operator+(Pos a, Pos b)
{
	return Pos{a.x+b.x, a.y+b.y};
}

static Pos xmul(Pos a, Pos b)
{
	return Pos{a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x};
}

struct HashPos
{
	size_t operator()(const Pos& p) const noexcept
	{
		return (hash<int>{}(p.x)<<4)^(hash<int>{}(p.y));
	}
};

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

	string directions;
	getline(input, directions);
	input.close();
	if(input.fail())
	{
		fmt::print(stderr, "Cannot parse the data\n");
		return 1;
	}

	unordered_map<Pos, bool, HashPos> visited;
	int twice = 0;
	Pos cur{0, 0};
	Pos dir{0, -1};
	visited[cur] = true;
	size_t pos = 0;
	size_t next;
	do
	{
		next = directions.find(' ', pos);
		int v = directions[pos];
		int l = stoi(directions.substr(pos+1, next-1));
		dir = xmul(dir, v == 'R' ? Pos{0,1} : Pos{0,-1});
		while (l-->0)
		{
			cur = cur + dir;
			if (twice == 0 && visited.find(cur) != visited.end())
			{
				twice = cur.manhattan();
			}
			visited[cur] = true;
		}
		pos = next + 1;
	} while (next != string::npos);

	fmt::print("Part1: {}\nPart2: {}\n", cur.manhattan(), twice);
	return 0;
}
