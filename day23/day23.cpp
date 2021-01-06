#include <cassert>
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
	enum { CPY, INC, DEC, JNZ, TGL } opcode;
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
	else if (s == "tgl")
	{
		i.opcode = Instruction::TGL;
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

	void reset() noexcept
	{
		fill(registers, registers + 4, 0);
	}

	int64_t execute(vector<Instruction> program, bool patch = false) noexcept
	{
		int64_t ip = 0;
		while (0 <= ip && (size_t)ip < program.size())
		{
			if (patch && ip == 3)
			{
				int a = program[3].op2.value;
				int b = program[4].op1.value;
				int c = program[4].op2.value;
				int d = program[2].op2.value;
				registers[a] = registers[b] * registers[d];
				registers[c] = registers[d] = 0;
				ip += 7;
				continue;
			}

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

			case Instruction::TGL:
			{
				int64_t addr = ip + instr.op1.get(registers);
				if (0 <= addr && (size_t)addr < program.size())
				{
					Instruction& other = program[addr];
					switch (other.opcode)
					{
					case Instruction::INC:
						other.opcode = Instruction::DEC;
						break;
					case Instruction::DEC:
					case Instruction::TGL:
						other.opcode = Instruction::INC;
						break;
					case Instruction::JNZ:
						other.opcode = Instruction::CPY;
						break;
					case Instruction::CPY:
						other.opcode = Instruction::JNZ;
					}
				}
				ip++;
				break;
			}

			default:
				abort();
			}
		}
		return registers[0];
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
	c.registers[0] = 7;
	fmt::print("Part1: {}\n", c.execute(program));
	c.reset();
	c.registers[0] = 12;
	fmt::print("Part2: {}\n", c.execute(program, true));

	return 0;
}
