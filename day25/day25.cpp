#include <fstream>
#include <vector>
#include <fmt/format.h>

using namespace std;

struct Operand
{
	enum { REG, LIT } type;
	int64_t value;

	int64_t get(int64_t regs[]) const noexcept
	{
		return type == REG ? regs[value] : value;
	}
};

istream& operator>>(istream& in, Operand& o)
{
	string s;
	if (in >> s)
	{
		if ("abcd"s.find(s[0]) != string::npos)
		{
			o.type = Operand::REG;
			o.value = s[0]-'a';
		}
		else
		{
			o.type = Operand::LIT;
			o.value = stoi(s);
		}
	}
	return in;
}

struct Instruction
{
	enum { CPY, INC, DEC, JNZ, OUT } opcode;
	Operand op1;
	Operand op2;
};

istream& operator>>(istream& in, Instruction& i)
{
	string s;
	in >> s;
	if (s == "cpy")
	{
		i.opcode = Instruction::CPY;
		in >> i.op1;
		in >> i.op2;
	}
	else if (s == "inc")
	{
		i.opcode = Instruction::INC;
		in >> i.op1;
	}
	else if (s == "dec")
	{
		i.opcode = Instruction::DEC;
		in >> i.op1;
	}
	else if (s == "jnz")
	{
		i.opcode = Instruction::JNZ;
		in >> i.op1;
		in >> i.op2;
	}
	else if (s == "out")
	{
		i.opcode = Instruction::OUT;
		in >> i.op1;
	}
	else
	{
		in.setstate(ios::failbit);
	}
	return in;
}

struct Computer
{
	int64_t registers[4];

	bool execute(const vector<Instruction>& program, int64_t init) noexcept
	{
		registers[0] = init;
		fill(registers+1, registers+4, 0);
		vector<Computer> stack;
		int64_t last = -1;
		int64_t ip = 0;
		while (0 <= ip && (size_t)ip < program.size())
		{
			auto& instr = program[ip];
			switch (instr.opcode)
			{
			case Instruction::CPY:
				if (instr.op2.type == Operand::REG)
				{
					registers[instr.op2.value] = instr.op1.get(registers);
				}
				ip++;
				break;

			case Instruction::INC:
				if (instr.op1.type == Operand::REG)
				{
					registers[instr.op1.value]++;
				}
				ip++;
				break;

			case Instruction::DEC:
				if (instr.op1.type == Operand::REG)
				{
					registers[instr.op1.value]--;
				}
				ip++;
				break;

			case Instruction::JNZ:
				if (instr.op1.get(registers))
				{
					ip += instr.op2.get(registers);
				}
				else
				{
					ip++;
				}
				break;

			case Instruction::OUT:
			{
				int64_t value = instr.op1.get(registers);
				if (value == last)
				{
					return false;
				}
				last = value;

				while (!stack.empty())
				{
					auto& back = stack.back();
					if (back == *this)
					{
						return true;
					}

					if (back > *this)
					{
						stack.pop_back();
					}
					else
					{
						break;
					}
				}
				stack.push_back(*this);
				ip++;
				break;
			}

			default:
				abort();
			}
		}
		return false;
	}

	bool operator==(const Computer& c) const noexcept
	{
		for (int i = 0; i < 4; i++)
		{
			if (registers[i] != c.registers[i])
				return false;
		}
		return true;
	}

	bool operator>(const Computer& c) const noexcept
	{
		for (int i = 0; i < 4; i++)
		{
			if (registers[i]>c.registers[i])
				return true;
			else if (registers[i]<c.registers[i])
				return false;
		}
		return true;
	}
};

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

	vector<Instruction> program;
	Instruction instr;
	while (input >> instr)
	{
		program.emplace_back(move(instr));
	}
	input.close();

	Computer c;
	int64_t i;
	for (i = 0; !c.execute(program, i); i++)
	{
	}
	fmt::print("Part1: {}\n", i);
	return 0;
}
