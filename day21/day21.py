#!/usr/bin/env python3

import re
import sys
from itertools import permutations

def swap_pos(lst, m):
    x, y = int(m[1]), int(m[2])
    lst[x], lst[y] = lst[y], lst[x]

def swap_letter(lst, m):
    x, y = m[1], m[2]
    for i in range(len(lst)):
        if lst[i] == x:
            lst[i] = y
        elif lst[i] == y:
            lst[i] = x

def rotate_dir(lst, m):
    d, steps = m[1], int(m[2])
    if d == "right":
        for i in range(steps):
            lst.insert(0, lst.pop())
    else:
        for i in range(steps):
            lst.append(lst.pop(0))

def rotate_pos(lst, m):
    letter = m[1]
    idx = lst.index(letter)
    if idx >= 4:
        idx += 1
    idx += 1
    rotate_dir(lst, ["", "right", idx])

def reverse(lst, m):
    x, y = int(m[1]), int(m[2])
    if x > y:
        x, y = y, x
    while x < y:
        lst[x], lst[y] = lst[y], lst[x]
        x += 1
        y -= 1

def move(lst, m):
    x, y = int(m[1]), int(m[2])
    lst.insert(y, lst.pop(x))

ops = [
    (re.compile(r"swap position (\d+) with position (\d+)"), swap_pos),
    (re.compile(r"swap letter ([a-z]) with letter ([a-z])"), swap_letter),
    (re.compile(r"rotate (left|right) (\d+) steps?"), rotate_dir),
    (re.compile(r"rotate based on position of letter ([a-z])"), rotate_pos),
    (re.compile(r"reverse positions (\d+) through (\d+)"), reverse),
    (re.compile(r"move position (\d+) to position (\d+)"), move),
]

def scramble(password, instructions):
    lst = list(password)
    for ins in instructions:
        for op, fun in ops:
            m = op.match(ins)
            if m:
                fun(lst, m)
                break
        else:
            print(ins, "not matched!")
            exit(1)

    return "".join(lst)

def descramble(password, instructions):
    for i in permutations(password):
        if scramble(i, instructions) == password:
            return "".join(i)

    return ""

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            instructions = f.read().strip().splitlines()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    print("Part1:", scramble("abcdefgh", instructions))
    print("Part2:", descramble("fbgdceah", instructions))
