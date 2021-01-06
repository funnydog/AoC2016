#!/usr/bin/env python3

import sys

def new_row(row):
    nr = []
    left = False
    center = row[0] == "^"
    for i in range(len(row)-1):
        right = row[i+1] == "^"
        if left != right:
            nr.append("^")
        else:
            nr.append(".")
        left = center
        center = right

    nr.append(" ")
    return "".join(nr)

def count_safe(initial, size):
    row = initial + " "
    v = row.count(".")
    for i in range(size-1):
        row = new_row(row)
        v += row.count(".")
    return v

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            initial = f.readline().strip()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    print("Part1:", count_safe(initial, 40))
    print("Part2:", count_safe(initial, 400000))
