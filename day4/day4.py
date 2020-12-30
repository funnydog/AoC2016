#!/usr/bin/env python3

import re
from string import ascii_lowercase as letters

pat = re.compile(r"(.*)-([0-9]+)\[(.*)\]")

def checksum(enc_name):
    lst = [[0, x] for x in letters]
    for l in enc_name:
        idx = letters.find(l)
        if idx >= 0:
            lst[idx][0] += 1

    lst.sort(key = lambda x: (-x[0], x[1]))
    return "".join(letter for freq, letter in lst)[:5]

def decrypt(txt):
    count, secret = 0, None
    for room in txt.splitlines():
        m = pat.match(room)
        if not m:
            continue

        if checksum(m[1]) != m[3]:
            continue

        rot = int(m[2])
        count += rot

        if not secret is None:
            continue

        lst = []
        for letter in m[1]:
            if letter == "-":
                lst.append(" ")
            else:
                lst.append(letters[(letters.find(letter) + rot) % 26])
        decrypted = "".join(lst)
        if decrypted.startswith("northpole"):
            secret = rot

    return count, secret

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

    count, secret = decrypt(txt)
    print("Part1:", count)
    print("Part2:", secret)
