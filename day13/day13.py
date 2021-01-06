#!/usr/bin/env python3

import sys

def is_wall(x, y, salt):
    v = x * x + 3 * x + 2 * x * y + y + y * y + salt

    # compute the parity
    v ^= v>>1
    v ^= v>>2
    v ^= v>>4
    v ^= v>>8
    v ^= v>>16
    return v & 1

def distance(salt, start, end):
    queue = []
    dist = {}
    queue.append(start)
    dist[start] = 0
    while queue:
        pos = queue.pop(0)
        if pos == end:
            break

        for dx, dy in ((0, -1), (1, 0), (0, 1), (-1, 0)):
            nx = pos[0] + dx
            ny = pos[1] + dy
            if nx < 0 or ny < 0:
                continue

            if not is_wall(nx, ny, salt) and (nx, ny) not in dist:
                dist[nx, ny] = dist[pos]+1
                queue.append((nx, ny))

    return dist[end]

def locations(salt, start, steps):
    queue = []
    visited = set()
    queue.append((start[0], start[1], steps))
    visited.add(start)
    while queue and steps:
        x, y, steps = queue.pop(0)
        if steps == 0:
            break

        for dx, dy in ((0, -1), (1, 0), (0, 1), (-1, 0)):
            nx = x + dx
            ny = y + dy
            if nx < 0 or ny < 0:
                continue

            if not is_wall(nx, ny, salt) and (nx, ny) not in visited:
                visited.add((nx, ny))
                queue.append((nx, ny, steps-1))

    return len(visited)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            salt = int(f.readline())
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    print("Part1:", distance(salt, (1,1), (31,39)))
    print("Part2:", locations(salt, (1,1), 50))
