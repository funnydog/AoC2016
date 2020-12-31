#!/usr/bin/env python3

import re
import sys

pat = re.compile(r"([a-z]+)|\[([a-z]+)\]")

def is_abba(sub):
    for i in range(len(sub)-3):
        if sub[i] == sub[i+3] and sub[i+1] == sub[i+2] \
           and sub[i] != sub[i+1]:
            return True

    return False

def supports_tls(sequence):
    found = False
    for a, b in pat.findall(sequence):
        if a and is_abba(a):
            found = True
        elif b and is_abba(b):
            return False

    return found

def find_aba(sub):
    lst = []
    for i in range(len(sub)-2):
        if sub[i] != sub[i+1] and sub[i] == sub[i+2]:
            lst.append(sub[i:i+3])
    return lst

def supports_ssl(sequence):
    aba = []
    bab = []
    for a, b in pat.findall(sequence):
        if a:
            aba.extend(find_aba(a))
        else:
            for b in find_aba(b):
                bab.append(b[1]+b[0]+b[1])

    for a in aba:
        if a in bab:
            return True
    return False

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

    tls = 0
    ssl = 0
    for row in txt.splitlines():
        if supports_tls(row):
            tls += 1
        if supports_ssl(row):
            ssl += 1

    print("Part1:", tls)
    print("Part2:", ssl)
