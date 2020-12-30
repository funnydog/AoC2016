#include <fstream>
#include <fmt/format.h>

using namespace std;

static const char keypad1[][7] = {
	{ 0,   0,   0,   0, 0, 0, 0 },
	{ 0, '1', '2', '3', 0, 0, 0 },
	{ 0, '4', '5', '6', 0, 0, 0 },
	{ 0, '7', '8', '9', 0, 0, 0 },
	{ 0,   0,   0,   0, 0, 0, 0 },
};

static const char keypad2[][7] = {
	{ 0,   0,   0,   0,   0,   0, 0 },
	{ 0,   0,   0, '1',   0,   0, 0 },
	{ 0,   0, '2', '3', '4',   0, 0 },
	{ 0, '5', '6', '7', '8', '9', 0 },
	{ 0,   0, 'A', 'B', 'C',   0, 0 },
	{ 0,   0,   0, 'D',   0,   0, 0 },
	{ 0,   0,   0,   0,   0,   0, 0 },
};

static string decode(const vector<string>& instr, const char keypad[][7], int x, int y)
{
	string rv;
	for (auto& s: instr)
	{
		for (auto c: s)
		{
			int nx = x;
			int ny = y;
			switch (c)
			{
			case 'U': ny--;	break;
			case 'L': nx--; break;
			case 'R': nx++; break;
			case 'D': ny++; break;
			default:
				continue;
			}
			if (keypad[ny][nx])
			{
				x = nx;
				y = ny;
			}
		}
		rv.push_back(keypad[y][x]);
	}
	return rv;
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

	vector<string> lines;
	string line;
	while (getline(input, line))
	{
		lines.emplace_back(move(line));
	}
	input.close();

	fmt::print(stderr, "Part1: {}\n", decode(lines, keypad1, 2, 2));
	fmt::print(stderr, "Part2: {}\n", decode(lines, keypad2, 1, 3));
	return 0;
}
