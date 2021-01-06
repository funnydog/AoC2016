#!/usr/bin/env python3

import hashlib
import sys

DIRECTIONS = (("U",0, -1), ("D", 0, 1), ("L", -1, 0), ("R", 1, 0))

def doors_status(passcode, path):
    h = hashlib.md5()
    data = passcode + path
    h.update(bytes(data, 'ascii'))
    digest = h.hexdigest()

    status = []
    for i in range(4):
        status.append(digest[i] in "bcdef")

    return status

def neighbors(passcode, pos):
    path, x, y = pos
    doors = doors_status(passcode, path)

    lst = []
    for i, (l, dx, dy) in enumerate(DIRECTIONS):
        nx = x + dx
        ny = y + dy
        if doors[i] and 0 <= nx < 4 and 0 <= ny < 4:
            lst.append((path+l, nx, ny))

    return lst

def shortest_path(passcode):
    queue = []
    cost = {}
    start = ("", 0, 0)
    queue.append(start)
    cost[start] = 0
    while queue:
        p = queue.pop(0)
        if p[1] == 3 and p[2] == 3:
            return p[0]

        for n in neighbors(passcode, p):
            ncost = cost[p] + 1
            if n not in cost or cost[n] > ncost:
                cost[n] = ncost
                queue.append(n)

    return ""

def longest_path_len(passcode):
    queue = []
    cost = {}
    start = ("", 0, 0)
    queue.append(start)
    cost[start] = 0
    longest = 0
    while queue:
        p = queue.pop(0)
        if p[1] == 3 and p[2] == 3:
            longest = max(longest, len(p[0]))
            continue

        for n in neighbors(passcode, p):
            ncost = cost[p] + 1
            if n not in cost or cost[n] < ncost:
                cost[n] = ncost
                queue.append(n)

    return longest

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            passcode = f.readline().strip()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    print("Part1:", shortest_path(passcode))
    print("Part2:", longest_path_len(passcode))
