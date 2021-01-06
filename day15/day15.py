#!/usr/bin/env python3

import re
import sys

pat = re.compile(r"Disc #\d+ has (\d+) positions; at time=(\d+), it is at position (\d+).")

def parse_discs(txt):
    discs = []
    i = 1
    for line in txt.strip().split("\n"):
        m = pat.match(line)
        if m:
            mod = int(m[1])
            pos = (int(m[3]) - int(m[2])) % mod
            discs.append((mod, pos))
            i += 1
    return discs

def sieve(discs):
    lst = []
    for i, (d, p) in enumerate(discs):
        lst.append((d, (d - i - p - 1) % d))

    lst.sort(key = lambda x: -x[0])
    p, x = 1, 0
    for n, a in lst:
        while x % n != a:
            x += p
        p *= n
    return x

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            discs = parse_discs(f.read())
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    print("Part1:", sieve(discs))
    discs.append((11, 0))
    print("Part2:", sieve(discs))
