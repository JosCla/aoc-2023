#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

import networkx as nx

def getGraph(lines):
    graph = nx.Graph()
    for line in lines:
        lineSpl = line.split(": ")
        source = lineSpl[0]
        dests = lineSpl[1].split(' ')

        for dest in dests:
            graph.add_edge(source, dest, capacity=1)
    return graph

def getSeparatestGroups(graph):
    start = ""
    for node in graph:
        if start == "":
            start = node
        else:
            cut, partition = nx.minimum_cut(graph, start, node)
            if cut == 3:
                return partition

# below: my initial attempt, before just pulling in networkx.
'''
def getGraph(lines):
    graph = {}
    for line in lines:
        lineSpl = line.split(": ")
        source = lineSpl[0]
        dests = lineSpl[1].split(' ')

        if not source in graph:
            graph[source] = []
        for dest in dests:
            if not dest in graph:
                graph[dest] = []
            graph[source].append(dest)
            graph[dest].append(source)
    return graph

def getConnectivity(graph, group1, group2):
    connectEdges = []
    for elem in group1:
        for dest in graph[elem]:
            if dest in group2:
                connectEdges.append((elem, dest))
    return connectEdges

def getSeparatestGroups(graph):
    group1 = set()
    group2 = set()
    for key in graph:
        if len(group1) > int(len(graph) / 2):
            group2.add(key)
        else:
            group1.add(key)

    connectedness = len(graph)**2
    while True:
        bestCon = len(getConnectivity(graph, group1, group2))
        bestMove = ""

        keys1 = [key for key in group1]
        keys2 = [key for key in group2]
        for key in keys1:
            group1.remove(key)
            group2.add(key)

            currCon = len(getConnectivity(graph, group1, group2))
            if currCon < bestCon:
                bestCon = currCon
                bestMove = key

            group2.remove(key)
            group1.add(key)

        for key in keys2:
            group2.remove(key)
            group1.add(key)

            currCon = len(getConnectivity(graph, group1, group2))
            if currCon < bestCon:
                bestCon = currCon
                bestMove = key

            group1.remove(key)
            group2.add(key)

        if bestCon < connectedness:
            connectedness = bestCon
            print(connectedness)
            if bestMove in group1:
                group1.remove(bestMove)
                group2.add(bestMove)
            else:
                group2.remove(bestMove)
                group1.add(bestMove)
        else:
            return (group1, group2)
'''

def partOne(lines):
    graph = getGraph(lines)
    g1, g2 = getSeparatestGroups(graph)
    return len(g1) * len(g2)

def partTwo(lines):
    return "merry christmas!"

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
