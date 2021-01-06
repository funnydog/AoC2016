#!/usr/bin/env python3

import sys

class Instruction(object):
    def __init__(self, txt):
        lst = txt.split(" ")
        self.op = lst[0]
        if self.op in ("inc", "dec"):
            self.arg1, self.arg2 = lst[1], None
        else:
            self.arg1, self.arg2  = lst[1], lst[2]

class Computer(object):
    def __init__(self):
        self.reset()

    def resolve(self, arg):
        value = self.register.get(arg)
        if value is None:
            return int(arg)
        return value

    def reset(self):
        self.register = {v: 0 for v in "abcd"}

    def execute(self, program):
        ip = 0
        while 0 <= ip < len(program):
            i = program[ip]
            if i.op == "cpy":
                self.register[i.arg2] = self.resolve(i.arg1)
                ip += 1
            elif i.op == "inc":
                self.register[i.arg1] += 1
                ip += 1
            elif i.op == "dec":
                self.register[i.arg1] -= 1
                ip += 1
            elif i.op == "jnz":
                if self.resolve(i.arg1):
                    ip += self.resolve(i.arg2)
                else:
                    ip += 1
            else:
                break

        return self.resolve("a")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            txt = f.read()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    program = [Instruction(line) for line in txt.splitlines()]
    c = Computer()
    print("Part1:", c.execute(program))
    c.reset()
    c.register["c"] = 1
    print("Part2:", c.execute(program))
