#!/usr/bin/env python3

import re
import sys

pat = re.compile(r"\/dev\/grid\/node-x(\d+)-y(\d+)\s+(\d+)T\s+(\d+)T\s+(\d+)T\s+(\d+)%")

class Node(object):
    def __init__(self, x, y, used, available):
        self.pos = (x, y)
        self.used = used
        self.available = available
        self.movable = False

    def __repr__(self):
        return repr(self.pos)

def parse_nodes(txt):
    nmap = {}
    for line in txt.splitlines():
        m = pat.search(line)
        if m:
            x, y, _, used, available, _ = map(int, [m[i] for i in range(1,7)])
            nmap[x, y] = Node(x, y, used, available)

    return nmap

def find_viable_nodes(nmap):
    nodes = list(nmap.values())
    viable = 0
    for i, a in enumerate(nodes):
        for j in range(i):
            b = nodes[j]
            if 0 < a.used <= b.available:
                viable += 1
                a.movable = True
            if 0 < b.used <= a.available:
                viable += 1
                b.movable = True

    return viable

def bfs(nmap, start, end):
    parent = {}
    queue = []
    parent[start] = None
    queue.append(start)
    while queue:
        pos = queue.pop(0)
        if pos == end:
            break

        for dx, dy in ((0,-1),(1,0),(0,1),(-1,0)):
            np = (pos[0]+dx, pos[1]+dy)
            if np not in nmap or not nmap[np].movable:
                continue

            if np in parent:
                continue

            parent[np] = pos
            queue.append(np)

    return parent

def count_moving_steps(nmap):
    # find the empty node
    empty = None
    for pos, node in nmap.items():
        if node.used == 0:
            empty = pos
            break
    nmap[empty].movable = True

    # find the position of the goal
    end = (0, 0)
    for npos, node in nmap.items():
        if npos[1] == 0 and npos[0] > end[0]:
            end = npos

    # find the path from the starting node to the goal
    path = bfs(nmap, (0,0), end)
    # NOTE: set the goal as non movable to treat it as a wall for the
    # empty node
    nmap[end].movable = False

    # find the position to move the empty node to
    steps = 0
    p = path[end]
    while p:
        epath = bfs(nmap, p, empty)
        n = epath[empty]
        while n:
            nmap[empty], nmap[n] = nmap[n], nmap[empty]
            empty = n
            n = epath[empty]
            steps += 1

        nmap[end], nmap[p] = nmap[p], nmap[end]
        empty = end
        end = p
        p = path[p]
        steps += 1

    return steps

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            txt = f.read()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    nmap = parse_nodes(txt)
    print("Part1:", find_viable_nodes(nmap))
    print("Part2:", count_moving_steps(nmap))
