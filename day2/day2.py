#!/usr/bin/env python3

kp1 = (
    ( "", "", "", "", "" ),
    ( "", "1", "2", "3", "" ),
    ( "", "4", "5", "6", "" ),
    ( "", "7", "8", "9", "" ),
    ( "", "", "", "", "" ),
)

kp2 = (
    ( "", "", "", "", "", "", "" ),
    ( "", "", "", "1", "", "", "", ),
    ( "", "", "2", "3", "4", "", "", "" ),
    ( "", "5", "6", "7", "8", "9", "" ),
    ( "", "", "A", "B", "C", "", "" ),
    ( "", "", "", "D", "", "", "" ),
    ( "", "", "", "", "", "", "" ),
)

def decode(txt, keypad, x, y):
    nums = []
    for row in txt.split("\n"):
        for c in row:
            if c == "U":
                if keypad[y-1][x]:
                    y -= 1
            elif c == "D":
                if keypad[y+1][x]:
                    y += 1
            elif c == "L":
                if keypad[y][x-1]:
                    x -= 1
            elif c == "R":
                if keypad[y][x+1]:
                    x += 1
        nums.append(keypad[y][x])

    return "".join(nums)

# txt ="""ULL
# RRDDD
# LURDL
# UUUUD"""
# assert decode(txt, kp1, 2, 2) == "1985"
# assert decode(txt, kp2, 1, 3) == "5DB3"

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

    print("Part1:", decode(txt, kp1, 2, 2))
    print("Part2:", decode(txt, kp2, 1, 3))
