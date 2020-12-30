#!/usr/bin/env python3

def add(a, b):
    return (a[0]+b[0], a[1]+b[1])

def xmul(a, b):
    return (a[0]*b[0]-a[1]*b[1], a[0]*b[1]+a[1]*b[0])

def manhattan(a):
    return abs(a[0]) + abs(a[1])

def walk(cmds):
    pos = (0, 0)
    dir = (0, -1)
    for cmd in cmds.split(", "):
        if cmd[0] == "R":
            dir = xmul(dir, (0, 1))
        else:
            dir = xmul(dir, (0, -1))

        for i in range(int(cmd[1:])):
            pos = add(pos, dir)

    return manhattan(pos)

def twice(cmds):
    map = {}
    pos = (0, 0)
    dir = (0, -1)
    for cmd in cmds.split(", "):
        if cmd[0] == "R":
            dir = xmul(dir, (0, 1))
        else:
            dir = xmul(dir, (0, -1))

        for i in range(int(cmd[1:])):
            pos = add(pos, dir)
            if pos in map:
                return manhattan(pos)
            map[pos] = 1


assert walk("R2, L3") == 5
assert walk("R2, R2, R2") == 2
assert walk("R5, L5, R5, R3") == 12
assert twice("R8, R4, R4, R8") == 4

import sys
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            txt = f.read().strip()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    print("Part1:", walk(txt))
    print("Part2:", twice(txt))
