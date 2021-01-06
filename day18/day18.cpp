#include <cassert>
#include <fstream>
#include <fmt/format.h>

using namespace std;

static size_t count_open_tiles(string row, size_t rows)
{
	assert(rows>0);
	assert(row.size()>0);

	size_t len = row.size();
	row.push_back(' ');
	size_t c = count(row.begin(), row.end(), '.');
	string newrow(row);
	while (--rows>0)
	{
		bool left = false;
		bool center = row[0] == '^';
		for (size_t i = 0; i < len; i++)
		{
			bool right = row[i+1] == '^';
			if (left ^ right)
			{
				newrow[i] = '^';
			}
			else
			{
				c++;
				newrow[i] = '.';
			}
			left = center;
			center = right;
		}
		swap(row, newrow);
	}
	return c;
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

	string initial;
	input>>initial;
	input.close();
	if (input.fail())
	{
		fmt::print(stderr, "Cannot parse the data\n");
		return 1;
	}

	fmt::print("Part1: {}\n", count_open_tiles(initial, 40));
	fmt::print("Part2: {}\n", count_open_tiles(initial, 400000));
	return 0;
}
