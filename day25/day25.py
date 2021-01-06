#!/usr/bin/env python3

import sys

class Instruction(object):
    def __init__(self, op, arg1, arg2):
        self.op = op
        self.arg1 = arg1
        self.arg2 = arg2

    def __repr__(self):
        return "{} {} {}".format(self.op, self.arg1, self.arg2)

class Computer(object):
    def __init__(self):
        self.registers = [0] * 4
        self.ip = 0

    def get(self, addr):
        index = "abcd".find(addr)
        if index < 0:
            return int(addr)
        return self.registers[index]

    def set(self, addr, value):
        index = "abcd".find(addr)
        assert(index >= 0)
        self.registers[index] = value

    def load(self, txt):
        self.program = []
        for line in txt.splitlines():
            lst = line.split(" ")
            if len(lst) < 3:
                lst.append("")
            self.program.append(Instruction(*lst))

    def execute(self, init):
        self.ip = 0
        self.registers = [init, 0, 0, 0]
        last_output = None
        stack = []
        while 0 <= self.ip < len(self.program):
            inst = self.program[self.ip]
            if inst.op == "cpy":
                self.set(inst.arg2, self.get(inst.arg1))
                self.ip += 1
            elif inst.op == "inc":
                self.set(inst.arg1, self.get(inst.arg1)+1)
                self.ip += 1
            elif inst.op == "dec":
                self.set(inst.arg1, self.get(inst.arg1)-1)
                self.ip += 1
            elif inst.op == "jnz":
                if self.get(inst.arg1):
                    self.ip += self.get(inst.arg2)
                else:
                    self.ip += 1
            elif inst.op == "tgl":
                addr = self.ip + self.get(inst.arg1)
                if 0 <= addr < len(self.program):
                    other = self.program[self.ip + self.get(inst.arg1)]
                    if other.op == "inc":
                        other.op = "dec"
                    elif other.op == "dec" or other.op == "tgl":
                        other.op = "inc"
                    elif other.op == "jnz":
                        other.op = "cpy"
                    else:
                        other.op = "jnz"
                self.ip += 1
            elif inst.op == "out":
                value = self.get(inst.arg1)
                if value == last_output:
                    return False
                last_output = value

                # cycle detection
                cur = tuple(self.registers)
                while stack:
                    if stack[-1] == cur:
                        # NOTE: cycle detected
                        return True
                    if stack[-1] > cur:
                        stack.pop()
                    else:
                        break

                stack.append(cur)
                self.ip += 1
            else:
                break

        return self.get("a")

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

    c = Computer()
    c.load(txt)
    i = 0
    while not c.execute(i):
        i += 1

    print("Part1:", i)
