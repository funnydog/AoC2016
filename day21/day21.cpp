#include <algorithm>
#include <fstream>
#include <regex>

#include <fmt/format.h>

using namespace std;

struct Instruction
{
	enum { SWP, SWL, ROT, ROP, REV, MOV, CNT } type;
	int a, b;

	void execute(string &str) const noexcept
	{
		switch(type)
		{
		case SWP:
			swap(str[a], str[b]);
			break;

		case SWL:
			for (auto& l: str)
			{
				if (l == a)
				{
					l = b;
				}
				else if (l == b)
				{
					l = a;
				}
			}
			break;

		case ROT:
			if (a)
			{
				// LEFT
				for (int i = 0; i < b; i++)
				{
					int v = str.front();
					str.erase(0, 1);
					str.push_back(v);
				}
			}
			else
			{
				// RIGHT
				for (int i = 0; i < b; i++)
				{
					int v = str.back();
					str.pop_back();
					str.insert(0, 1, v);
				}
			}
			break;

		case ROP: {
			size_t tmp = str.find(a);
			if (tmp != string::npos)
			{
				if (tmp >= 4)
				{
					tmp++;
				}
				tmp++;
				while (tmp-->0)
				{
					int v = str.back();
					str.pop_back();
					str.insert(0, 1, v);
				}
			}
			break;
		}

		case REV:
			for (size_t x = a, y = b; x < y; x++, y--)
			{
				swap(str[x], str[y]);
			}
			break;

		case MOV: {
			int tmp = str[a];
			str.erase(a, 1);
			str.insert(b, 1, tmp);
			break;
		}

		default:
			break;
		}
	}
};

istream& operator>>(istream& input, Instruction &i)
{
	static const regex rlst[] = {
		regex("swap position (\\d+) with position (\\d+)"),
		regex("swap letter ([a-z]) with letter ([a-z])"),
		regex("rotate (left|right) (\\d+) steps?"),
		regex("rotate based on position of letter ([a-z])"),
		regex("reverse positions (\\d+) through (\\d+)"),
		regex("move position (\\d+) to position (\\d+)"),
	};

	string line;
	if (!getline(input, line))
	{
		return input;
	}

	smatch sm;
	int j;
	for (j = 0; j < Instruction::CNT; j++)
	{
		if (regex_match(line, sm, rlst[j]))
		{
			break;
		}
	}
	i.type = static_cast<decltype(i.type)>(j);
	switch (j)
	{
	case Instruction::SWP:
	case Instruction::REV:
	case Instruction::MOV:
		i.a = stoi(sm[1]);
		i.b = stoi(sm[2]);
		break;

	case Instruction::SWL:
		i.a = sm[1].str()[0];
		i.b = sm[2].str()[0];
		break;

	case Instruction::ROT:
		i.a = sm[1] == "left";
		i.b = stoi(sm[2]);
		break;

	case Instruction::ROP:
		i.a = sm[1].str()[0];
		break;

	default:
		input.setstate(ios::failbit);
		return input;
	}
	return input;
}

static string scramble(string password, const vector<Instruction>& instructions)
{
	for (auto& i: instructions)
	{
		i.execute(password);
	}
	return password;
}

static string descramble(string password, const vector<Instruction>& instructions)
{
	string cur(password);
	sort(cur.begin(), cur.end());
	do
	{
		if (password == scramble(cur, instructions))
		{
			return cur;
		}
	} while (next_permutation(cur.begin(), cur.end()));

	return "";
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

	vector<Instruction> instructions;
	Instruction inst;
	while (input>>inst)
	{
		instructions.emplace_back(move(inst));
	}
	input.close();

	fmt::print("Part1: {}\n", scramble("abcdefgh", instructions));
	fmt::print("Part2: {}\n", descramble("fbgdceah", instructions));
	return 0;
}
