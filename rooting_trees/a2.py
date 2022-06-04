'''
    Gaurav Laxmandas
    Nathani
    gauravla
'''

import re
import time
import sys
from pyspark import SparkContext
from pyspark import SparkConf

def mapper1(edge):
    edge = edge.split()
    if edge[0] == edge[1]: yield (int(edge[0]), [int(edge[0]), int(edge[1])])
    else:
        yield (int(edge[0]), [int(edge[0]), int(edge[1])])
        yield (int(edge[1]), [int(edge[0]), int(edge[1])])

def mapper(edge):
    if edge[0] == edge[1]: yield (int(edge[0]), [int(edge[0]), int(edge[1])])
    else:
        yield (int(edge[0]), [int(edge[0]), int(edge[1])])
        yield (int(edge[1]), [int(edge[0]), int(edge[1])])

def reducer(edges):
    V = edges[0]
    E = edges[1]
    if len(E) == 1: return ()
    for e in E:
        if e[0] == V: 
            par = e[1]
            break
    if V == par: yield (V, V)
    for e in E:
        if e[0] != V: yield (e[0], par)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Wrong Input!")
        print("Usage: a2.py <path_to_infile> <path_to_outdir>")
        sys.exit(1)

    APP_NAME = 'FindRoots'
    conf = SparkConf().setAppName(APP_NAME)
    start = time.time()
    sc = SparkContext(conf=conf)

    mapped_edges = sc.textFile(sys.argv[1]).flatMap(mapper1,preservesPartitioning=True).groupByKey().flatMap(reducer)
    c_me = mapped_edges.cache()
    prev_count = c_me.values().sum()
    while True:
        mapped_edges = c_me.flatMap(mapper,preservesPartitioning=True).groupByKey().flatMap(reducer)
        c_me.unpersist()
        c_me = mapped_edges.cache()
        curr_count = c_me.values().sum()
        if curr_count == prev_count:
            c_me.saveAsTextFile(sys.argv[2])
            break
        prev_count = curr_count

    sc.stop()
    end = time.time()

    print(round((end-start)/60,2),"m")
