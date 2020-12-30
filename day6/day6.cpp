#include <cctype>
#include <fstream>
#include <vector>

#include <fmt/format.h>

using namespace std;

static pair<string,string> error_correct(const vector<string>& messages)
{
	string most, least;
	for (size_t col = 0; col < messages[0].size(); col++)
	{
		vector<pair<int,int>> freq(26);
		for (auto& msg: messages)
		{
			int i = msg[col];
			if (isalpha(i))
			{
				freq[i-'a'].first++;
				freq[i-'a'].second = i;
			}
		}
		sort(freq.begin(), freq.end(), [](const auto &a, const auto& b) {
			return a.first > b.first;
		});
		most.push_back(freq.front().second);
		while (freq.back().first == 0)
		{
			freq.pop_back();
		}
		least.push_back(freq.back().second);
	}
	return make_pair(move(most),move(least));
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

	vector<string> messages;
	string message;
	while (getline(input, message))
	{
		messages.emplace_back(message);
	}
	input.close();

	auto [most, least] = error_correct(messages);
	fmt::print("Part1: {}\nPart2: {}\n", most, least);

	return 0;
}
