#include <cctype>
#include <fstream>
#include <regex>
#include <vector>

#include <fmt/format.h>

using namespace std;

static string checksum(const string& name)
{
	vector<pair<int, int>> freq(26);
	for (auto c: name)
	{
		if (isalpha(c))
		{
			freq[tolower(c) - 'a'].first++;
			freq[tolower(c) - 'a'].second = c;
		}
	}

	sort(freq.begin(), freq.end(), [](const auto& a, const auto& b) {
		if (a.first != b.first)
		{
			return a.first > b.first;
		}
		else
		{
			return a.second < b.second;
		}
	});

	string rv;
	for (size_t i = 0; i < 5; i++)
	{
		rv.push_back(freq[i].second);
	}
	return rv;
}

static pair<int,int> decrypt(const vector<string>& rooms)
{
	static const regex pattern("(.*)-(\\d+)\\[(.*)\\]");

	pair<int,int> rv{0, -1};
	for (auto& room: rooms)
	{
		smatch sm;
		if (!regex_match(room, sm, pattern))
		{
			continue;
		}

		if (checksum(sm[1]) == sm[3])
		{
			rv.first += stoi(sm[2]);
		}

		if (rv.second != -1)
		{
			continue;
		}

		int rot = stoi(sm[2]);
		string tmp;
		for (auto c: sm[1].str())
		{
			if (c == '-')
			{
				tmp.push_back(' ');
			}
			else
			{
				tmp.push_back((c-'a' + rot) % 26 + 'a');
			}
		}
		if (tmp.rfind("northpole", 0) == 0)
		{
			rv.second = rot;
		}
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

	vector<string> rooms;
	string room;
	while (getline(input, room))
	{
		rooms.emplace_back(move(room));
	}
	input.close();

	auto [sum, secret] = decrypt(rooms);

	fmt::print("Part1: {}\nPart2: {}\n", sum, secret);
	return 0;
}
