#include <fstream>
#include <vector>

#include <fmt/format.h>

using namespace std;

static bool is_abba(const string& str, size_t begin, size_t end)
{
	if (end > str.size())
	{
		end = str.size();
	}
	for (size_t i = begin+3; i < end; i++)
	{
		if (str[i-3] == str[i]
		    && str[i-2] == str[i-1]
		    && str[i] != str[i-1])
		{
			return true;
		}
	}
	return false;
}

static bool supports_tls(const string& ip)
{
	size_t pos = 0;
	size_t next;
	enum {OUT, IN} state = OUT;
	bool ret = false;
	do
	{
		if (state == OUT)
		{
			next = ip.find('[', pos);
			ret = ret || is_abba(ip, pos, next);
			state = IN;
		}
		else
		{
			next = ip.find(']', pos);
			if (is_abba(ip, pos, next))
			{
				return false;
			}
			state = OUT;
		}
		pos = next + 1;
	} while (next != string::npos);

	return ret;
}

static size_t count_tls(const vector<string>& ips)
{
	return count_if(
		ips.begin(),
		ips.end(),
		[](const auto& ip) {
			return supports_tls(ip);
		});
}

static void find_aba(const string& str, size_t begin, size_t end, vector<pair<char,char>>& aba)
{
	if (end > str.size())
	{
		end = str.size();
	}
	for (size_t i = begin+2; i < end; i++)
	{
		if (str[i] == str[i-2] && str[i] != str[i-1])
		{
			aba.emplace_back(make_pair(str[i], str[i-1]));
		}
	}
}

static bool supports_ssl(const string& ip)
{
	size_t pos = 0;
	size_t next;
	enum {OUT, IN} state = OUT;
	vector<pair<char,char>> aba;
	vector<pair<char,char>> bab;
	do
	{
		if (state == OUT)
		{
			next = ip.find('[', pos);
			find_aba(ip, pos, next, aba);
			state = IN;
		}
		else
		{
			next = ip.find(']', pos);
			find_aba(ip, pos, next, bab);
			state = OUT;
		}
		pos = next + 1;
	} while (next != string::npos);

	for (auto [a1, a2]: aba)
	{
		for(auto [b1, b2]: bab)
		{
			if (a1 == b2 && a2 == b1)
			{
				return true;
			}
		}
	}
	return false;
}

static size_t count_ssl(const vector<string>& ips)
{
	return count_if(
		ips.begin(),
		ips.end(),
		[](const auto& ip) {
			return supports_ssl(ip);
		});
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
	vector<string> ips;
	string ip;
	while (getline(input, ip))
	{
		ips.emplace_back(move(ip));
	}
	input.close();

	fmt::print("Part1: {}\nPart2: {}\n", count_tls(ips), count_ssl(ips));
	return 0;
}
