#include <fstream>
#include <fmt/format.h>

#include <openssl/md5.h>

using namespace std;

static pair<string,string> open_sesame(const string& salt)
{
	string door1(8, ' ');
	string door2(8, ' ');
	unsigned char digest[MD5_DIGEST_LENGTH];
	static const char *hex = "0123456789abcdef";

	size_t size1 = 0;
	size_t size2 = 0;
	for (size_t counter = 0; size1 < 8 || size2 < 8; counter++)
	{
		string data = fmt::format("{}{}", salt, counter);
		MD5_CTX c;
		MD5_Init(&c);
		MD5_Update(&c, data.c_str(), data.size());
		MD5_Final(digest, &c);

		if (((digest[2]&0xf0)|digest[1]|digest[0]) != 0)
		{
			continue;
		}
		if (size1 < 8)
		{
			door1[size1] = hex[digest[2]&0xf];
			size1++;
		}
		if (size2 < 8)
		{
			size_t i = digest[2]&0xf;
			if (i < 8 && door2[i] == ' ')
			{
				door2[i] = hex[digest[3]>>4];
				size2++;
			}
		}
	}
	return make_pair(move(door1), move(door2));
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
	getline(input, salt);
	input.close();
	if (input.fail())
	{
		fmt::print("Cannot parse the data\n");
		return 1;
	}

	auto [door1, door2] = open_sesame(salt);
	fmt::print("Part1: {}\nPart2: {}\n", door1, door2);
	return 0;
}
