#!/usr/bin/env python3

import hashlib

def open_sesame(salt):
    hexs = "0123456789abcdef"
    door1, door2 = [' '] * 8, [' '] * 8
    size1, size2 = 0, 0
    counter = 0
    while size1 < 8 or size2 < 8:
        generator = "{}{}".format(salt, counter)
        counter += 1
        hash = hashlib.md5(generator.encode("ascii")).hexdigest()
        if hash[:5] != "00000":
            continue

        if size1 < 8:
            door1[size1] = hash[5]
            size1 += 1

        if size2 < 8:
            pos = hexs.find(hash[5])
            if 0 <= pos < 8 and door2[pos] == ' ':
                door2[pos] = hash[6]
                size2 += 1

    return "".join(door1), "".join(door2)

import sys
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file = sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            txt = f.read().strip()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    door1, door2 = open_sesame(txt)
    print("Part1:", door1)
    print("Part2:", door2)
