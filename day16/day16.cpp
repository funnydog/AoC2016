#include <cassert>
#include <fstream>
#include <fmt/format.h>

using namespace std;

static string reverse(const string& a)
{
	string r;
	for (auto it = a.rbegin(); it != a.rend(); ++it)
	{
		r.push_back(*it == '0' ? '1' : '0');
	}
	return r;
}

static string step(const string& a)
{
	return a + '0' + reverse(a);
}

static char rchksum(const string& a, size_t pos, size_t size)
{
	if (size == 1)
	{
		return a[pos];
	}
	char ca = rchksum(a, pos, size/2);
	char cb = rchksum(a, pos+size/2, size/2);
	if (ca == cb)
	{
		return '1';
	}
	else
	{
		return '0';
	}
}

static string find_chksum(const string& initial, size_t size)
{
	// this find the divisors between initial and reverse
	string a = "";
	size_t i = initial.size();
	while (i < size)
	{
		a = step(a);
		i = i * 2 + 1;
	}

	// compute the block size to get the 1 digit of checksum
	size_t block = 1;
	i = size;
	while (i % 2 == 0)
	{
		i /= 2;
		block *= 2;
	}

	// compute the checksum
	string chk;
	string ini(initial);
	string rev(reverse(initial));
	string blk(initial);
	size_t apos = 0;
	while (i-->0)
	{
		while (blk.size() < block)
		{
			blk.push_back(a[apos++]);
			blk += rev;
			swap(ini, rev);
		}
		chk.push_back(rchksum(blk, 0, block));
		blk.erase(blk.begin(), blk.begin()+block);
	}
	return chk;
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
	getline(input, initial);
	input.close();
	if (input.fail())
	{
		fmt::print(stderr, "Cannot parse the data\n");
		return 1;
	}

	fmt::print("Part1: {}\n", find_chksum(initial, 272));
	fmt::print("Part2: {}\n", find_chksum(initial, 35651584));
	return 0;
}
