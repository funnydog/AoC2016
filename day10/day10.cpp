#include <cassert>
#include <fstream>
#include <regex>
#include <vector>

#include <fmt/format.h>

using namespace std;

struct Bots;

struct Output
{
	enum { NONE, BOT, OUT } type;
	size_t value;
};

struct Bot
{
	int chips[2];
	size_t ccount;

	Output low;
	Output high;

	size_t adj[2];
	size_t acount;

	bool visited;

	void add_chip(int chip)
	{
		assert(ccount<2);
		chips[ccount++] = chip;
	}

	void add_dependency(size_t bot)
	{
		assert(acount<2);
		adj[acount++] = bot;
	}

	void execute(vector<Bot>& bots, vector<int>& out)
	{
		assert(ccount == 2);
		int min = chips[0];
		int max = chips[1];
		if (max < min)
		{
			swap(max, min);
		}
		if (low.type == Output::BOT)
		{
			bots[low.value].add_chip(min);
		}
		else if (low.type == Output::OUT)
		{
			out[low.value] = min;
		}
		if (high.type == Output::BOT)
		{
			bots[high.value].add_chip(max);
		}
		else if (high.type == Output::OUT)
		{
			out[high.value] = max;
		}
	}
};

static vector<Bot> load(istream& input)
{
	static const regex value("value (\\d+) goes to bot (\\d+)");
	static const regex bot("bot (\\d+) gives low to (bot|output) (\\d+) and high to (bot|output) (\\d+)");
	vector<Bot> bots;
	string line;
	while (getline(input, line))
	{
		smatch sm;
		if (regex_match(line, sm, value))
		{
			size_t i = stoull(sm[2]);
			if (bots.size() <= i)
			{
				bots.resize(i+1);
			}

			bots[i].add_chip(stoi(sm[1]));
		}
		else if (regex_match(line, sm, bot))
		{
			size_t src = stoull(sm[1]);
			size_t low = stoull(sm[3]);
			size_t high = stoull(sm[5]);

			// resize the vector if needed
			size_t max = src;
			if (max < low) max = low;
			if (max < high) max = high;
			if (bots.size() <= max)
			{
				bots.resize(max+1);
			}

			// set the values and add the dependencies
			if (sm[2] == "bot")
			{
				bots[src].low.type = Output::BOT;
				bots[low].add_dependency(src);
			}
			else
			{
				bots[src].low.type = Output::OUT;
			}
			bots[src].low.value = low;

			if (sm[4] == "bot")
			{
				bots[src].high.type = Output::BOT;
				bots[high].add_dependency(src);
			}
			else
			{
				bots[src].high.type = Output::OUT;
			}
			bots[src].high.value = high;
		}
	}
	return bots;
}

static void dfs(vector<Bot>& bots, size_t cur, vector<size_t>& path)
{
	auto& bot = bots[cur];
	bot.visited = true;
	for (size_t i = 0; i < bot.acount; i++)
	{
		size_t pos = bot.adj[i];
		if (bots[pos].visited)
		{
			continue;
		}
		dfs(bots, pos, path);
	}
	path.push_back(cur);
}

static vector<int> execute(vector<Bot>& bots)
{
	// reset
	size_t maxout = 0;
	for (auto& b: bots)
	{
		b.visited = false;
		if (b.low.type == Output::OUT && maxout < b.low.value)
		{
			maxout = b.low.value;
		}
		else if (b.high.type == Output::OUT && maxout < b.high.value)
		{
			maxout = b.high.value;
		}
	}
	vector<size_t> path;

	// topological sort
	for (size_t i = 0; i < bots.size(); i++)
	{
		if (!bots[i].visited)
		{
			dfs(bots, i, path);
		}
	}

	// execute the logic for each bot in order
	vector<int> output(maxout+1, -1);
	for (auto i: path)
	{
		bots[i].execute(bots, output);
	}

	return output;
}

static size_t find_bot(const vector<Bot>& bots, int low, int high)
{
	if (high < low)
	{
		swap(high, low);
	}
	for (size_t i = 0; i < bots.size(); i++)
	{
		int min = bots[i].chips[0];
		int max = bots[i].chips[1];
		if (max < min)
		{
			swap(max, min);
		}
		if (min == low && max == high)
		{
			return i;
		}
	}
	return -1;
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
		fmt::print(stderr, "Cannot open {}\n", argv[1]);
		return 1;
	}
	vector<Bot> bots = load(input);
	input.close();

	vector<int> output = execute(bots);
	fmt::print(
		"Part1: {}\nPart2: {}\n",
		find_bot(bots, 61, 17),
		output[0] * output[1] * output[2]);
	return 0;
}
