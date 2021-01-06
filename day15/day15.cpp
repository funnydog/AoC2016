#include <fstream>
#include <vector>
#include <regex>
#include <fmt/format.h>

using namespace std;

struct Disc
{
	size_t period;
	size_t position;
};

istream& operator>>(istream& in, Disc& d)
{
	static const regex pat("Disc #\\d+ has (\\d+) positions; at time=(\\d+), it is at position (\\d+).");
	string line;
	if (getline(in, line))
	{
		smatch sm;
		if (regex_match(line, sm, pat))
		{
			d.period = stoi(sm[1]);
			d.position = (d.period + stoi(sm[3]) - stoi(sm[2])) % d.period;
		}
		else
		{
			in.setstate(ios::failbit);
		}
	}
	return in;
}

static size_t sieve(const vector<Disc>& discs)
{
	// translate the problem into the cinese remainder problem
	// by computing the remainders
	vector<Disc> tmp(discs.size());
	for (size_t i = 0; i < discs.size(); i++)
	{
		size_t d = discs[i].period;
		size_t p = discs[i].position;
		tmp[i].period = d;
		// NOTE: this is needed to get positive remainders of
		// negative dividends
		tmp[i].position = (d-(i+p+1)%d)%d;
	}

	// sort in reverse to speed up the sieve
	sort(tmp.begin(), tmp.end(), [](const auto& a, const auto& b){
		return a.period > b.period;
	});

	// compute the sieve
	size_t p = 1;
	size_t x = 0;
	for (auto& d: tmp)
	{
		while ((x % d.period) != d.position)
		{
			x += p;
		}
		p *= d.period;
	}
	return x;
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
	vector<Disc> discs;
	Disc disc;
	while (input>>disc)
	{
		discs.emplace_back(move(disc));
	}
	input.close();

	fmt::print("Part1: {}\n", sieve(discs));
	discs.emplace_back(Disc{11, 0});
	fmt::print("Part2: {}\n", sieve(discs));
	return 0;
}
