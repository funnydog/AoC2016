#include <algorithm>
#include <fstream>
#include <vector>

#include <fmt/format.h>

using namespace std;

struct Triplet
{
	int a, b, c;

};

static istream& operator>>(istream& in, Triplet& t)
{
	in >> t.a >> t.b >> t.c;
	return in;
}

static bool is_valid(int a, int b, int c)
{
	if (a > b)
	{
		swap(a, b);
	}
	if (a > c)
	{
		swap(a, c);
	}
	if (b > c)
	{
		swap(b, c);
	}
	return a + b > c;
}

static int count_horizontally(const vector<Triplet>& list)
{
	return count_if(list.begin(), list.end(), [](const auto& t) {
		return is_valid(t.a, t.b, t.c);
	});
}

static int count_vertically(const vector<Triplet>& list)
{
	size_t count = 0;
	for (size_t y = 0; y < list.size(); y += 3)
	{
		count += is_valid(list[y+0].a, list[y+1].a, list[y+2].a);
		count += is_valid(list[y+0].b, list[y+1].b, list[y+2].b);
		count += is_valid(list[y+0].c, list[y+1].c, list[y+2].c);
	}
	return count;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fmt::print(stderr, "Usage: {} <filename\n", argv[0]);
		return 1;
	}

	ifstream input(argv[1]);
	if (!input)
	{
		fmt::print("Cannot open {}\n", argv[1]);
		return 1;
	}

	vector<Triplet> list;
	Triplet t;
	while (input>>t)
	{
		list.emplace_back(move(t));
	}
	input.close();

	fmt::print("Part1: {}\n", count_horizontally(list));
	fmt::print("Part2: {}\n", count_vertically(list));
	return 0;
}
