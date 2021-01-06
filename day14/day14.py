#!/usr/bin/env python3

import hashlib
import sys

def generate(salt, index, stretch):
    # generate a key
    h = hashlib.md5()
    data = salt + str(index)
    h.update(bytes(data, "ascii"))
    digest = h.hexdigest()
    for i in range(stretch):
        h = hashlib.md5()
        h.update(bytes(digest, "ascii"))
        digest = h.hexdigest()
    return digest

def last_index(salt, stretch):
    potential = {}
    key_count, key_index = 0, None
    i = 0
    while key_count < 64:
        digest = generate(salt, i, stretch)

        # NOTE: look for a repetition of 5
        cnt = 1
        last = digest[0]
        for v in digest[1:]:
            if v == last:
                cnt += 1
            else:
                cnt = 1
            if cnt == 5:
                # NOTE: potential key becomes real key
                for j in potential.get(last):
                    if i - j <= 1000:
                        key_index = j
                        key_count += 1
                        if key_count == 64:
                            break
                del potential[last]
            last = v

        # NOTE: check if a key has a triplet
        cnt = 1
        last = digest[0]
        for v in digest[1:]:
            if v == last:
                cnt += 1
            else:
                cnt = 1
            if cnt == 3:
                # NOTE: key becomes potential key
                potential[last] = potential.get(last, []) + [i]
                break
            last = v

        i+= 1

    return key_index

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            salt = f.read().strip()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    print("Part1:", last_index(salt, 0))
    print("Part2:", last_index(salt, 2016))
