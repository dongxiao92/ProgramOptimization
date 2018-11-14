import sys
import numpy as np

def check_graph(path):
    #1. check no duplicate edges
    edges=set([])
    with open(path) as f:
        lines = f.readlines()
    for line in lines[2:]:
        s1, s2 = line.strip().split(" ")
        s1 = int(s1)
        s2 = int(s2)
        if (s1, s2) in edges:
            print("duplicate edge")
            continue
        edges.add((s1,s2))
    print("total %d edges"%(len(edges)))
    # check valid indirect graph, for edge(s1, s2), there must be (s2, s1)
    vton={}
    for (s1, s2) in edges:
        if not s1 in vton:
            vton[s1] = set([])
        vton[s1].add(s2)
    for (s1, s2) in edges:
        if not s1 in vton[s2]:
            print("no edge for (%d, %d)"%(s2, s1))
        if not s2 in vton[s1]:
            print("no edge for (%d, %d)"%(s1, s2))

if __name__ == '__main__':
    check_graph(sys.argv[1])
