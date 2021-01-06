#!/usr/bin/env python

import sys

def reverse(a):
    lst = []
    for c in reversed(a):
        lst.append(c == "0" and "1" or "0")
    return "".join(lst)

def step(a):
    return a + "0" + reverse(a)

def rchksum(a, pos, size):
    if size == 1:
        return a[pos]
    ca = rchksum(a, pos, size//2)
    cb = rchksum(a, pos+size//2, size//2);
    return ca == cb and "1" or "0"

def find_chksum(initial, size):
    a = ""
    i = len(initial)
    while i < size:
        i = i * 2 + 1
        a = step(a)

    sz, block_size = size, 1
    while sz % 2 == 0:
        block_size *= 2
        sz = sz // 2

    lst = []
    rev = reverse(initial)
    mystr = initial
    pos = 0
    while size:
        while len(mystr) < block_size:
            mystr += a[pos]
            mystr += rev
            rev, initial = initial, rev
            pos += 1

        lst.append(rchksum(mystr, 0, block_size))
        mystr = mystr[block_size:]
        size -= block_size

    return "".join(lst)

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

    print("Part1:", find_chksum(initial, 272))
    print("Part2:", find_chksum(initial, 35651584))
