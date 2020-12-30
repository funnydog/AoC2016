#!/usr/bin/env python3

def decode(txt):
    rows = txt.split("\n")
    length = len(rows[0])
    most = []
    least = []
    for j in range(length):
        freq = {}
        for row in rows:
            freq[row[j]] = freq.get(row[j], 0) + 1

        lst = sorted(freq.items(), key = lambda x: (-x[1], x[0]))
        most.append(lst[0][0])
        least.append(lst[-1][0])

    return "".join(most), "".join(least)

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

    part1, part2 = decode(txt)
    print("Part1:", part1)
    print("Part2:", part2)
