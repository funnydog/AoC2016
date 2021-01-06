#!/usr/bin/env python3

def find_lowest(blacklist):
    i = 0
    for low, high in blacklist:
        if i < low:
            break
        elif i <= high:
            i = high + 1
        else:
            pass
    return i

def find_available(blacklist, size):
    available = 0
    i = 0
    for low, high in blacklist:
        if i < low:
            available += low - i
            i = high + 1
        elif i <= high:
            i = high + 1
        else:
            pass

    if i < size:
        available += size- i

    return available

import sys
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    with open(sys.argv[1], "rt") as f:
        blacklist = []
        for line in f:
            blacklist.append(tuple(map(int, line.split("-"))))

    blacklist.sort()
    print("Part1:", find_lowest(blacklist))
    print("Part2:", find_available(blacklist, 2**32))
