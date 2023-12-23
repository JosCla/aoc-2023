#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

dirs = [(0, 1), (1, 0), (-1, 0), (0, -1)]
arrows = {
    '^': (-1, 0),
    'v': (1, 0),
    '<': (0, -1),
    '>': (0, 1)
}

def getGrid(lines):
    return [[c for c in line] for line in lines]

def printGrid(grid):
    for row in grid:
        print(''.join(row))

def getStartPos(grid):
    return (0, 1)

def getEndPos(grid):
    return (len(grid) - 1, len(grid[0]) - 2)

# locates the nodes, also replaces all arrows with nothing and the central node
# tiles with stars
def locateNodes(grid):
    nodes = {}

    # firstly, the start and end nodes
    startRow, startCol = getStartPos(grid)
    endRow, endCol = getEndPos(grid)
    grid[startRow][startCol] = '*'
    grid[endRow][endCol] = '*'
    nodes[(startRow, startCol)] = {(1, 0): (True, None, None)}
    nodes[(endRow, endCol)] = {(-1, 0): (False, None, None)}

    # then finding all the internal nodes
    for (row, line) in enumerate(grid):
        for (col, c) in enumerate(line):
            if row == 0 or row == len(grid) - 1:
                continue
            if col == 0 or col == len(grid[0]) - 1:
                continue
            if c != '.':
                continue

            allArrows = True
            for dirRow, dirCol in dirs:
                if grid[row + dirRow][col + dirCol] == '.':
                    allArrows = False
                    break

            if not allArrows:
                continue

            node = {}
            for currDir in dirs:
                dirRow, dirCol = currDir
                dirChar = grid[row + dirRow][col + dirCol]
                if dirChar != '#':
                    isOut = currDir == arrows[dirChar]
                    node[currDir] = (isOut, None, None)
                    grid[row + dirRow][col + dirCol] = '.'
            nodes[(row, col)] = node
            grid[row][col] = '*'

    return nodes

# given a start node and direction, pathfinds along to the next node
def pathfindToNode(grid, start, startDir):
    discovered = [[False for c in row] for row in grid]
    startRow, startCol = start
    startDirRow, startDirCol = startDir

    discovered[startRow][startCol] = True
    row = startRow + startDirRow
    col = startCol + startDirCol
    discovered[row][col] = True
    steps = 1

    while True:
        if grid[row][col] == '*':
            return (steps, (row, col))

        for currDir in dirs:
            dirRow, dirCol = currDir
            nextRow = row + dirRow
            nextCol = col + dirCol
            dirChar = grid[nextRow][nextCol]

            if dirChar != '#' and not discovered[nextRow][nextCol]:
                row = nextRow
                col = nextCol
                discovered[row][col] = True
                break

        steps += 1

# gets all the nodes in the grid, which are the arrow junctions.
# maps from their positions to a map, which maps each exit direction to whether
# it's outgoing, how many steps it takes, and which node it leads to
def getNodes(grid):
    # step 1: locating the nodes
    nodes = locateNodes(grid)

    # step 2: pathfind between the nodes
    for nodePos in nodes:
        nodeRow, nodeCol = nodePos
        nodeExits = nodes[nodePos]
        for exitDir in nodeExits:
            exitDirRow, exitDirCol = exitDir
            isOut, _, _ = nodeExits[exitDir]

            steps, receivingNode = pathfindToNode(grid, nodePos, exitDir)
            nodes[nodePos][exitDir] = (isOut, steps, receivingNode)

    '''
    for node in nodes:
        print(node)
        print(nodes[node])
    printGrid(grid)
    '''
    return nodes

def findMaxDistTo(nodes, startNode, visitStack, endNode, listenDirs = True):
    # print(visitStack)
    if startNode == endNode:
        return (0, True)

    maxDist = 0
    anyMadeToEnd = False
    currNode = nodes[startNode]
    for outDir in currNode:
        isOut, steps, receivingNode = currNode[outDir]
        if listenDirs and (not isOut):
            continue
        if receivingNode in visitStack:
            continue

        nextVisitStack = visitStack[:] + [receivingNode]
        (currMax, madeToEnd) = findMaxDistTo(nodes, receivingNode, nextVisitStack, endNode, listenDirs)
        currMax += steps

        if currMax > maxDist and madeToEnd:
            maxDist = currMax
            anyMadeToEnd = True

    return (maxDist, anyMadeToEnd)

def partOne(lines):
    grid = getGrid(lines)
    nodes = getNodes(grid)

    startPos = getStartPos(grid)
    endPos = getEndPos(grid)

    maxDist, anyMadeToEnd = findMaxDistTo(nodes, startPos, [startPos], endPos)
    return maxDist

def partTwo(lines):
    grid = getGrid(lines)
    nodes = getNodes(grid)

    startPos = getStartPos(grid)
    endPos = getEndPos(grid)

    maxDist, anyMadeToEnd = findMaxDistTo(nodes, startPos, [startPos], endPos, False)
    return maxDist

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
