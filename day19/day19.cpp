#include <fstream>
#include <memory>
#include <fmt/format.h>

using namespace std;

static unsigned steal_next(unsigned elfcount)
{
	unique_ptr<unsigned[]> next(new unsigned[elfcount+1]);

	for (unsigned i = 1; i < elfcount; i++)
	{
		next[i] = i + 1;
	}
	next[elfcount] = 1;

	unsigned cur = 1;
	while (cur != next[cur])
	{
		// steal from left
		cur = next[cur] = next[next[cur]];
	}

	return cur;
}

static unsigned steal_opposite(unsigned elfcount)
{
	unique_ptr<unsigned[]> next(new unsigned[elfcount+1]);

	for (unsigned i = 1; i < elfcount; i++)
	{
		next[i] = i + 1;
	}
	next[elfcount] = 1;

	unsigned cur = 1;
	unsigned prev = elfcount/2;	// NOTE: element before the opposite
	while (cur != next[cur])
	{
		// steal from the opposite
		next[prev] = next[next[prev]];
		elfcount--;

		// go to the elf at the left
		cur = next[cur];

		// advance the opposite only if there is an even
		// number of elements
		if ((elfcount & 1) == 0)
		{
			prev = next[prev];
		}
	}

	return cur;
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

	unsigned elfcount;
	input >> elfcount;
	input.close();
	if (input.fail())
	{
		fmt::print(stderr, "Cannot parse the data\n");
		return 1;
	}

	fmt::print("Part1: {}\n", steal_next(elfcount));
	fmt::print("Part2: {}\n", steal_opposite(elfcount));
	return 0;
}
