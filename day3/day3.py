#!/usr/bin/python3

def is_valid(a, b, c):
    if a > b:
        a, b = b, a
    if a > c:
        a, c = c, a
    if b > c:
        b, c = c, b
    return a + b > c

def count_by_row(txt):
    count = 0
    for row in txt.splitlines():
        a, b, c = [int(x) for x in row.split()]
        if is_valid(a, b, c):
            count += 1
    return count

def count_by_column(txt):
    rows = txt.splitlines()
    count = 0
    for y in range(0, len(rows), 3):
        nums = []
        for j in range(y, y+3):
            nums.append(tuple(map(int, rows[j].split())))
        for x in range(3):
            if is_valid(nums[0][x], nums[1][x], nums[2][x]):
                count += 1

    return count

assert is_valid(25, 10, 5) == False

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

    print("Part1:", count_by_row(txt))
    print("Part2:", count_by_column(txt))
