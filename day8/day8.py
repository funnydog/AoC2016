#!/usr/bin/env python3

import re

pat = re.compile(r"rect ([0-9]+)x([0-9]+)|rotate column x=([0-9]+) by ([0-9]+)|rotate row y=([0-9]+) by ([0-9]+)")

class Screen(object):
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.pixel = [[0 for x in range(width)] for y in range(height)]

    def rect(self, a, b):
        for y in range(b):
            for x in range(a):
                self.pixel[y][x] = 1

    def rotate_row(self, a, b):
        for i in range(b):
            right = self.pixel[a][-1]
            for x in range(self.width-1, 0, -1):
                self.pixel[a][x] = self.pixel[a][x-1]
            self.pixel[a][0] = right

    def rotate_col(self, a, b):
        for i in range(b):
            bottom = self.pixel[-1][a]
            for y in range(self.height-1, 0, -1):
                self.pixel[y][a] = self.pixel[y-1][a]
            self.pixel[0][a] = bottom

    def commands(self, txt):
        for row in txt.split("\n"):
            m = pat.search(row)
            if not m:
                continue
            g = m.groups()
            if g[0] and g[1]:
                self.rect(int(g[0]), int(g[1]))
            elif g[2] and g[3]:
                self.rotate_col(int(g[2]), int(g[3]))
            elif g[4] and g[5]:
                self.rotate_row(int(g[4]), int(g[5]))
            else:
                pass

    def lit(self):
        count = 0
        for row in self.pixel:
            count += sum(row)
        return count

    def __str__(self):
        lst = []
        for y, row in enumerate(self.pixel):
            lst.append("".join(v and "#" or " " for v in row))
        return "\n".join(lst)

import sys

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file = sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            txt = f.read().strip()
    except:
        print("Cannot open {}".format(sys.argv[1]), file = sys.stderr)
        sys.exit(1)

    s = Screen(50,6)
    s.commands(txt)
    print("Part1:", s.lit())
    print("Part2:")
    print(s)
