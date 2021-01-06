#!/usr/bin/env python3

import sys

def steal_left(elfcount):
    next = [None] + [0] * elfcount
    for i in range(1, elfcount):
        next[i] = i + 1
    next[elfcount] = 1

    cur = 1
    while cur != next[cur]:
        # current elf steals the gifts on the elf in its left
        next[cur] = next[next[cur]]
        cur = next[cur]

    return cur

def steal_opposite(elfcount):
    next = [None] + [0] * elfcount
    for i in range(1, elfcount):
        next[i] = i + 1
    next[elfcount] = 1

    cur = 1
    prev = cur + elfcount//2 - 1 # NOTE: element before the opposite
    while cur != next[cur]:
        # steal from the opposite
        next[prev] = next[next[prev]]
        elfcount -= 1

        # go to the elf at the left
        cur = next[cur]

        # advance the opposite only if there is an even number of
        # elements
        if elfcount & 1 == 0:
            prev = next[prev]

    return cur

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            elfcount = int(f.readline().strip())
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    print("Part1:", steal_left(elfcount))
    print("Part2:", steal_opposite(elfcount))
