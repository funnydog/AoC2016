#include <fstream>
#include <sstream>
#include <unordered_map>
#include <fmt/format.h>

#include <openssl/md5.h>

using namespace std;

static string genhash(const string& salt, unsigned index, unsigned stretch)
{
	unsigned char buffer[MD5_DIGEST_LENGTH];
	string data = fmt::format("{}{}", salt, index);
	do
	{
		MD5_CTX c;
		MD5_Init(&c);
		MD5_Update(&c, data.c_str(), data.size());
		MD5_Final(buffer, &c);

		data = fmt::format("{:02x}", fmt::join(buffer, buffer+MD5_DIGEST_LENGTH, ""));
	} while (stretch-->0);
	return data;
}

static unsigned last_index(const string& salt, unsigned stretch)
{
	static const string hex = "0123456789abcdef";
	vector<int> potential[16];
	unsigned count = 0;
	unsigned last_key = 0;
	for (unsigned i = 0; ; i++)
	{
		string digest = genhash(salt, i, stretch);

		// look for a repetition of 5 values
		int cnt = 1;
		int last = digest[0];
		for (size_t j = 1; j < digest.size(); j++)
		{
			cnt = (digest[j] == last) ? (cnt+1) : 1;
			last = digest[j];
			if (cnt == 5)
			{
				auto& lst = potential[hex.find(last)];
				for (auto k: lst)
				{
					if (i-k <= 1000)
					{
						last_key = k;
						count++;
						if (count == 64)
						{
							goto out;
						}
					}
				}
				lst.clear();
			}
		}

		// look for the first repetition of 3 values
		cnt = 1;
		last = digest[0];
		for (size_t j = 1; j < digest.size(); j++)
		{
			cnt = (digest[j] == last) ? (cnt+1) : 1;
			last = digest[j];
			if (cnt == 3)
			{
				potential[hex.find(last)].push_back(i);
				break;
			}
		}
	}
out:
	return last_key;
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

	string salt;
	input >> salt;
	input.close();
	if (input.fail())
	{
		fmt::print(stderr, "Cannot parse the data\n");
		return 1;
	}

	fmt::print("Part1: {}\n", last_index(salt, 0));
	fmt::print("Part2: {}\n", last_index(salt, 2016));
	return 0;
}
