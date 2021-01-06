#include <fstream>
#include <vector>
#include <fmt/format.h>

using namespace std;

static size_t find_lowest(const vector<pair<size_t, size_t>>& blacklist)
{
	size_t i = 0;
	for (auto [low, high]: blacklist)
	{
		if (i < low)
		{
			break;
		}
		else if (i <= high)
		{
			i = high+1;
		}
		else
		{
		}
	}
	return i;
}

static size_t find_available(const vector<pair<size_t, size_t>>& blacklist, size_t size)
{
	size_t i = 0;
	size_t available = 0;
	for (auto [low, high]: blacklist)
	{
		if (i < low)
		{
			available += low - i;
			i = high + 1;
		}
		else if (i <= high)
		{
			i = high + 1;
		}
		else
		{
		}
	}
	if (i < size)
	{
		available += size - i;
	}
	return available;
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

	vector<pair<size_t,size_t>> blacklist;
	while (true)
	{
		size_t low, high;
		input >> low;
		input.get();
		input >> high;
		if (!input)
		{
			break;
		}
		blacklist.emplace_back(make_pair(low, high));
	}
	input.close();

	sort(blacklist.begin(), blacklist.end());

	fmt::print("Part1: {}\n", find_lowest(blacklist));
	fmt::print("Part2: {}\n", find_available(blacklist, 1ULL<<32));
	return 0;
}
