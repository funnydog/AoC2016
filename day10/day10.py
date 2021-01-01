#!/usr/bin/env python3

from collections import defaultdict
import re

pat = re.compile(
    r"value ([0-9]+) goes to bot ([0-9]+)" +
    r"|bot ([0-9]+) gives low to (bot|output) ([0-9]+) and high to (bot|output) ([0-9]+)"
)

class Bot(object):
    def __init__(self):
        self.values = []
        self.low = None
        self.high = None
        self.olow = None
        self.ohigh = None
        self.discovered = False
        self.depends = []

    def add(self, value):
        self.values.append(value)

    def execute(self, bots, output):
        a, b = self.values
        if b < a:
            a, b = b, a
        if not self.low is None:
            bots[self.low].add(a)
        if not self.olow is None:
            output[self.olow] = a
        if not self.high is None:
            bots[self.high].add(b)
        if not self.ohigh is None:
            output[self.ohigh] = b

def load_bots(txt):
    bots = defaultdict(lambda: Bot())
    for row in txt.split("\n"):
        m = pat.search(row)
        if not m:
            continue
        g = m.groups()
        if g[0] and g[1]:
            value = int(g[0])
            botnum = int(g[1])
            bots[botnum].add(value)
        elif g[2] and g[3] and g[4] and g[5] and g[6]:
            botnum = int(g[2])
            low = int(g[4])
            if g[3] == "bot":
                bots[low].depends.append(botnum)
                bots[botnum].low = low
            else:
                bots[botnum].olow = low
            high = int(g[6])
            if g[5] == "bot":
                bots[high].depends.append(botnum)
                bots[botnum].high = high
            else:
                bots[botnum].ohigh = high
    return bots

def dfs(bots, num, stack):
    bot = bots[num]
    bot.discovered = True
    for d in bot.depends:
        if bots[d].discovered:
            continue

        dfs(bots, d, stack)

    stack.append(num)

def execute(bots):
    # reset
    maxout = 0
    for i, b in bots.items():
        b.discovered = False
        if b.olow and maxout < b.olow:
            maxout = b.olow
        if b.ohigh and maxout < b.ohigh:
            maxout = b.ohigh

    # topological sort
    path = []
    for i, bot in bots.items():
        if not bot.discovered:
            dfs(bots, i, path)

    # execute the bots in order
    out = [0] * (maxout+1)
    for i in path:
        bots[i].execute(bots, out)

    return out

def find_bot(bots, a, b):
    if a > b:
        a, b = b, a

    for num, bot in bots.items():
        if len(bot.values) == 2:
            x, y = bot.values
            if y < x:
                x, y = y, x
            if a == x and b == y:
                return num

    return None

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

    bots = load_bots(txt)
    output = execute(bots)
    print("Part1:", find_bot(bots, 61, 17))
    print("Part2:", output[0]*output[1]*output[2])
