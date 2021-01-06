#!/usr/bin/env python3

import heapq
import sys

def parse_input(txt):
    names = {}
    floors = []
    for line in txt.strip().split("\n"):
        if not line:
            continue
        _, cont = line.split(" contains ")
        word = None
        g, m = 0, 0
        for element in cont.split(" "):
            if element in ("a", "and", "nothing", "relevant."):
                continue
            elif element.startswith("generator"):
                g |= 1<<names[word];
            elif element.startswith("microchip"):
                m |= 1<<names[word];
            else:
                word = element.split("-")[0]
                if not word in names:
                    names[word] = len(names)

        floors.append((g, m))

    return tuple(floors)

def add_elements(floors, couples):
    mask = (1<<couples)-1
    x = 0
    for g, m in floors:
        x |= g
        x |= m

    i = 0
    while x:
        x >>= 1
        i += 1

    floors = list(floors)
    floors[0] = (floors[0][0] | mask<<i, floors[0][1] | mask<<i)
    return tuple(floors)

def is_safe(g, m):
    return g == 0 or m == 0 or (g & m) == m

def bits(i):
    lst = []
    j = 0
    while i:
        if i & 1:
            lst.append(1<<j)
        i >>= 1
        j += 1
    return lst

def candidates(state):
    lst = []
    pos, floors = state
    for move in (1, -1):
        npos = pos+move
        if not (0 <= npos < 4):
            continue

        # find the movable items
        safe = []
        cg, cm = floors[pos]
        ng, nm = floors[npos]
        bg = bits(cg)
        bm = bits(cm)

        # generators
        for i, g1 in enumerate(bg):
            # single generator
            if is_safe(cg & ~g1, cm) and is_safe(ng | g1, nm):
                safe.append((g1, 0))

            # couple of generators
            for j in range(i):
                g2 = g1 | bg[j]
                if is_safe(cg & ~g2, cm) and is_safe(ng | g2, nm):
                    safe.append((g2, 0))

        # microchips
        for i, m1 in enumerate(bm):
            # single microchip
            if is_safe(cg, cm & ~m1) and is_safe(ng, nm | m1):
                safe.append((0, m1))

            # couple of microchips
            for j in range(i):
                m2 = m1 | bm[j]
                if is_safe(cg, cm & ~m2) and is_safe(ng, nm | m2):
                    safe.append((0, m2))

        # microchip + generator
        for g1 in bg:
            for m1 in bm:
                if is_safe(g1, m1) and is_safe(cg & ~g1, cm & ~m1) and is_safe(ng|g1, nm|m1):
                    safe.append((g1, m1))

        lst.append((npos, safe))

    return lst

def priority(cost, state):
    s = 0
    for i in range(3,-1,-1):
        s *= 10
        for x in state[1][i]:
            while x:
                x &= x-1
                s += 1

    return cost*1000 - s

def search(state):
    queue = []
    hint = priority(0, state)
    heapq.heappush(queue, (hint, state))
    cost = {}
    cost[state] = 0
    while queue:
        _, state = heapq.heappop(queue)
        old_cost = cost[state]
        pos, floors = state
        if pos == 3 and all(f == (0,0) for f in floors[:-1]):
            return cost[state]

        new_floors = list(floors)
        for npos, safe in candidates(state):
            for ng, nm in safe:
                new_floors[npos] = (floors[npos][0] | ng, floors[npos][1] | nm)
                new_floors[pos] = (floors[pos][0] & ~ng, floors[pos][1] & ~nm)

                new_state = (npos, tuple(new_floors))
                new_cost = old_cost + 1
                if not new_state in cost or new_cost < cost[new_state]:
                    cost[new_state] = new_cost
                    hint = priority(new_cost, new_state)
                    heapq.heappush(queue, (hint, new_state))

            new_floors[npos] = floors[npos]

    return None

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

    f = parse_input(txt)
    print("Part1:", search((0, f)))

    f = add_elements(f, 2)
    print("Part2:", search((0, f)))

