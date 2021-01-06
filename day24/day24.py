#!/usr/bin/env python3

from itertools import permutations
import sys

def distance(mymap, start, end):
    distance = {}
    queue = []
    distance[start] = 0
    queue.append(start)
    while queue:
        pos = queue.pop(0)
        if pos == end:
            break

        for dx, dy in ((0,-1),(1,0),(0,1),(-1,0)):
            np = (pos[0]+dx, pos[1]+dy)
            if mymap[np[1]][np[0]] == "#":
                continue

            if np in distance:
                continue

            distance[np] = distance[pos] + 1
            queue.append(np)

    return distance[end]

def solve(mymap):
    locations = []
    for y, row in enumerate(mymap):
        for x, value in enumerate(row):
            if value != "#" and value != ".":
                locations.append((value, (x, y)))

    locations.sort()

    dist = {}
    for i, (loc1, pos1) in enumerate(locations):
        for j in range(i):
            loc2, pos2 = locations[j]
            dist[i, j] = dist[j, i] = distance(mymap, pos1, pos2)

    part1 = 1e12
    part2 = 1e12
    for path in permutations(range(1, len(locations))):
        length = dist[0, path[0]]
        for x in range(len(path)-1):
            length += dist[path[x], path[x+1]]
        if part1 > length:
            part1 = length
        length += dist[0, path[-1]]
        if part2 > length:
            part2 = length

    return part1, part2

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

    mymap = [list(x) for x in txt.split("\n")]
    part1, part2 = solve(mymap)
    print("Part1:", part1)
    print("Part2:", part2)

