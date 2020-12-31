#include <cassert>
#include <fstream>
#include <iostream>
#include <regex>
#include <fmt/format.h>

using namespace std;

static const int WIDTH = 50;
static const int HEIGHT = 6;

static const regex rect("rect (\\d+)x(\\d+)");
static const regex rotrow("rotate row y=(\\d+) by (\\d+)");
static const regex rotcol("rotate column x=(\\d+) by (\\d+)");

struct Instruction
{
	enum { RECT, ROTR, ROTC } type;
	int arg1;
	int arg2;
};

istream& operator>>(istream& in, Instruction& i)
{
	string line;
	if (!getline(in, line))
	{
		return in;
	}
	smatch sm;
	if (regex_match(line, sm, rect))
	{
		i.type = Instruction::RECT;
	}
	else if (regex_match(line, sm, rotrow))
	{
		i.type = Instruction::ROTR;
	}
	else if (regex_match(line, sm, rotcol))
	{
		i.type = Instruction::ROTC;
	}
	else
	{
		in.setstate(ios::failbit);
		return in;
	}
	i.arg1 = stoi(sm[1]);
	i.arg2 = stoi(sm[2]);
	return in;
}

struct Screen
{
	char pixel[HEIGHT][WIDTH];

	Screen() : pixel{} {}

	void rect(size_t a, size_t b)
	{
		assert(a < WIDTH);
		assert(b < HEIGHT);
		for (size_t y = 0; y < b; y++)
		{
			for (size_t x = 0; x < a; x++)
			{
				pixel[y][x] = 1;
			}
		}
	}

	void rotate_row(size_t row, size_t b)
	{
		assert(row < HEIGHT);
		while (b-->0)
		{
			char end = pixel[row][WIDTH-1];
			for (size_t i = WIDTH-1; i > 0; i--)
			{
				pixel[row][i] = pixel[row][i-1];
			}
			pixel[row][0] = end;
		}
	}

	void rotate_col(size_t col, size_t b)
	{
		assert(col < WIDTH);
		while (b-->0)
		{
			char end = pixel[HEIGHT-1][col];
			for (size_t i = HEIGHT-1; i > 0; i--)
			{
				pixel[i][col] = pixel[i-1][col];
			}
			pixel[0][col] = end;
		}
	}

	void execute(const Instruction& inst)
	{
		switch(inst.type)
		{
		case Instruction::RECT: rect(inst.arg1, inst.arg2); break;
		case Instruction::ROTR: rotate_row(inst.arg1, inst.arg2); break;
		case Instruction::ROTC: rotate_col(inst.arg1, inst.arg2); break;
		}
	}

	size_t lit() const noexcept
	{
		return count(pixel[0], pixel[0]+WIDTH*HEIGHT, 1);
	}
};

ostream& operator<<(ostream& out, const Screen& s)
{
	for (size_t y = 0; y < HEIGHT; y++)
	{
		for (size_t x = 0; x < WIDTH; x++)
		{
			out << (s.pixel[y][x] ? '#' : ' ');
		}
		out << endl;
	}
	return out;
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
	Screen s;
	Instruction in;
	while (input>>in)
	{
		s.execute(in);
	}
	input.close();

	cout << fmt::format("Part1: {}", s.lit()) << endl
	     << "Part2:" << endl << s;
	return 0;
}
