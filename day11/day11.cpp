#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include <fmt/format.h>

using namespace std;

struct Candidate
{
	int pos;
	unsigned g;
	unsigned m;
};

static bool is_safe(int g, int m)
{
	return g == 0 || m == 0 || (g&m) == m;
}

struct State
{
	int pos;
	unsigned floor[4][2];
	int prio;

	bool operator==(const State& other) const noexcept
	{
		if (pos != other.pos)
		{
			return false;
		}

		for (size_t i = 0; i < 4; i++)
		{
			if (floor[i][0] != other.floor[i][0]
			    || floor[i][1] != other.floor[i][1])
			{
				return false;
			}
		}
		return true;
	}

	void add_elements_at(int idx, int count)
	{
		unsigned x = 0;
		for (auto &f: floor)
		{
			x |= f[0];
			x |= f[1];
		}
		int p;
		for (p = 0; x; p++)
		{
			x = x & (x-1);
		}
		unsigned mask = (1U<<count)-1;
		floor[idx][0] |= mask<<p;
		floor[idx][1] |= mask<<p;
	}

	bool is_optimal() const noexcept
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (floor[i][0] || floor[i][1])
			{
				return false;
			}
		}
		return true;
	}

	void set_priority(int cost) noexcept
	{
		int s = 0;
		for (int i = 3; i >= 0; i--)
		{
			s *= 10;
			for (int j = 0; j < 2; j++)
			{
				int n = floor[i][j];
				while (n)
				{
					n = n & (n-1);
					s++;
				}
			}
		}
		prio = s - cost * 1000;
	}

	void get_candidates(vector<Candidate>& candidates) const noexcept
	{
		static const int dir[] = {1, -1};
		candidates.clear();
		for (auto d: dir)
		{
			int npos = pos + d;
			if (npos < 0 || npos >= 4)
			{
				continue;
			}

			unsigned cg = floor[pos][0];
		        unsigned cm = floor[pos][1];
			unsigned ng = floor[npos][0];
			unsigned nm = floor[npos][1];

			// generators
			for (unsigned i = 0, x = cg; x; i++, x>>=1)
			{
				// one generator
				unsigned g1 = cg & (1U<<i);
				if (!g1) continue;

				if (is_safe(cg & ~g1, cm) && is_safe(ng|g1, nm))
				{
					candidates.emplace_back(Candidate{npos, g1, 0});
				}
				// two generators
				for (unsigned j = 0; j < i; j++)
				{
					unsigned g2 = cg & (1U<<j);
					if (!g2) continue;

					g2 |= g1;
					if (is_safe(cg & ~g2, cm) && is_safe(ng|g2, nm))
					{
						candidates.emplace_back(Candidate{npos, g2, 0});
					}
				}
			}

			// microchips
			for (unsigned i = 0, x = cm; x; i++, x>>=1)
			{
				// one microchip
				unsigned m1 = cm & (1U<<i);
				if (!m1) continue;

				if (is_safe(cg, cm & ~m1) && is_safe(ng, nm|m1))
				{
					candidates.emplace_back(Candidate{npos, 0, m1});
				}
				// two microchips
				for (unsigned j = 0; j < i; j++)
				{
					unsigned m2 = cm & (1U<<j);
					if (!m2) continue;

					m2 |= m1;
					if (is_safe(cg, cm & ~m2) && is_safe(ng, nm|m2))
					{
						candidates.emplace_back(Candidate{npos, 0, m2});
					}
				}
			}

			// microchip + generator
			for (unsigned i = 0, x = cg; x; i++, x >>= 1)
			{
				unsigned g1 = cg & (1U<<i);
				if (!g1) continue;

				for (unsigned j = 0, y = cm; y; j++, y >>= 1)
				{
					unsigned m1 = cm & (1U<<j);
					if (!m1) continue;

					if (is_safe(g1, m1)
					    && is_safe(cg & ~g1, cm & ~m1)
					    && is_safe(ng|g1, nm|m1))
					{
						candidates.emplace_back(Candidate{npos, g1, m1});
					}
				}
			}
		}
	}
};

bool operator<(const State& a, const State& b)
{
	return a.prio < b.prio;
}

namespace std
{
	template<>
	struct hash<State>
	{
		size_t operator()(const State &s) const noexcept
		{
			size_t h = 0;
			for (auto& f: s.floor)
			{
				h = (h << 8) ^ (f[0]<<4) ^ (f[1]);
			}
			return (h<<2) ^ s.pos;
		}
	};
}

istream& operator>>(istream& input, State& s)
{
	unordered_map<string,size_t> names;
	string line;
	size_t index = 0;
	while (getline(input, line))
	{
		size_t pos = line.find(" contains ");
		if (pos == string::npos)
		{
			continue;
		}
		pos += 10;
		size_t next;
		string name;
		do
		{
			next = line.find(' ', pos);
			string w = line.substr(pos, next-pos);
			if (w == "a" || w == "and" || w == "nothing" || w == "relevant.")
			{
				// ignore
			}
			else if (w.rfind("generator", 0) == 0)
			{
				s.floor[index][0] |= 1<<names[name];
			}
			else if (w.rfind("microchip", 0) == 0)
			{
				s.floor[index][1] |= 1<<names[name];
			}
			else
			{
				size_t end = w.find('-');
				name = w.substr(0, end);
				if (names.find(name) == names.end())
				{
					names[name] = names.size();
				}
			}
			pos = next + 1;
		} while (next != string::npos);
		index++;
	}
	s.pos = s.prio = 0;
	return input;
}

ostream& operator<<(ostream& out, const State& s)
{
	for (size_t i = 0; i < 4; i++)
	{
		out << fmt::format("Floor: {}, G = {:02x},  M = {:02x}",
				   i+1, s.floor[i][0], s.floor[i][1]) << endl;
	}
	return out;
}

static size_t search(State state)
{
	vector<Candidate> candidates;
	vector<State> queue;
	unordered_map<State, size_t> cost;
	cost[state] = 0;
	queue.emplace_back(move(state));
	while (!queue.empty())
	{
		pop_heap(queue.begin(), queue.end());
		state = queue.back();
		queue.pop_back();
		size_t oldcost = cost[state];

		if (state.is_optimal())
		{
			return cost[state];
		}

		state.get_candidates(candidates);
		for (auto [npos, g, m]: candidates)
		{
			int opos = state.pos;
			state.floor[npos][0] |= g;
			state.floor[npos][1] |= m;
			state.floor[opos][0] &= ~g;
			state.floor[opos][1] &= ~m;
			state.pos = npos;

			size_t newcost = oldcost + 1;
			if (cost.find(state) == cost.end() || newcost < cost[state])
			{
				state.set_priority(newcost);
				cost[state] = newcost;
				queue.push_back(state);
				push_heap(queue.begin(), queue.end());
			}

			state.pos = opos;
			state.floor[npos][0] &= ~g;
			state.floor[npos][1] &= ~m;
			state.floor[opos][0] |= g;
			state.floor[opos][1] |= m;
		}
	}
	return -1;
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
	State s{};
	input>>s;
	input.close();

	fmt::print(stdout, "Part1: {}\n", search(s));

	s.add_elements_at(0, 2);
	fmt::print(stdout, "Part2: {}\n", search(s));

	return 0;
}
