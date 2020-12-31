#!/usr/bin/env python3

def declen(txt):
    delta = 0
    state = 0
    length, mult, start = 0, 0, 0
    i = 0
    end = len(txt)
    while i < end:
        c = txt[i]
        if state == 0:
            if c == "(":
                length = 0
                start = i
                state = 1
            i += 1
        elif state == 1:
            if c != "x":
                length = length * 10 + int(c)
            else:
                mult = 0
                state = 2
            i += 1
        else:
            if c != ")":
                mult = mult * 10 + int(c)
                i += 1
            else:
                delta -= (length + i + 1 - start)
                delta += mult * length
                state = 0
                i += length

    return end + delta

def declen2(txt, begin = None, end = None):
    begin = begin or 0
    end = end or len(txt) - begin

    delta = 0
    state = 0
    rulestart, mult, length = 0, 0, 0
    i = begin
    while i < end:
        c = txt[i]
        if state == 0:
            if c == "(":
                length = 0
                rulestart = i
                state = 1
            i += 1
        elif state == 1:
            if c != "x":
                length = length * 10 + int(c)
            else:
                mult = 0
                state = 2
            i += 1
        elif state == 2:
            if c != ")":
                mult = mult * 10 + int(c)
                i += 1
            else:
                delta -= (length + i + 1 - rulestart)
                delta += mult * declen2(txt, i+1, length+i+1)
                state = 0
                i += length

    return end - begin + delta

# assert declen("ADVENT") == 6
# assert declen("A(1x5)BC") == 7
# assert declen("(3x3)XYZ") == 9
# assert declen("A(2x2)BCD(2x2)EFG") == 11
# assert declen("(6x1)(1x3)A") == 6
# assert declen("X(8x2)(3x3)ABCY") == 18

# assert declen2("(3x3)XYZ") == 9
# assert declen2("X(8x2)(3x3)ABCY") == 20
# assert declen2("(27x12)(20x12)(13x14)(7x10)(1x12)A") == 241920
# assert declen2("(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN") == 445

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

    print("Part1:", declen(txt))
    print("Part2:", declen2(txt))
