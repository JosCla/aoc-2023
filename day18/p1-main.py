#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

def toCommands(lines):
    res = []
    for line in lines:
        lineSplit = line.split(' ')
        res.append((lineSplit[0], int(lineSplit[1]), lineSplit[2][2:-1]))
    return res

def dirStrToInts(dirStr):
    dirDict = {
        'U': (0, -1),
        'D': (0, 1),
        'L': (-1, 0),
        'R': (1, 0)
    }
    return dirDict[dirStr]

def isClockwise(cmds):
    turningNumber = 0
    (lastDir, _, _) = cmds[len(cmds) - 1]
    currDir = dirStrToInts(lastDir)
    for cmd in cmds:
        (nextDirStr, _, _) = cmd
        nextDir = dirStrToInts(nextDirStr)

        (currDirX, currDirY) = currDir
        (nextDirX, nextDirY) = nextDir
        if nextDirX == -currDirY and nextDirY == currDirX:
            turningNumber -= 1
        else:
            turningNumber += 1

        currDir = nextDir

    return turningNumber < 0

def initGrid(cmds):
    currX = 0
    currY = 0
    minX = 0
    minY = 0
    maxX = 0
    maxY = 0

    for cmd in cmds:
        (dirStr, num, _) = cmd
        (dirX, dirY) = dirStrToInts(dirStr)
        currX += dirX * num
        currY += dirY * num

        if currX < minX:
            minX = currX
        if currX > maxX:
            maxX = currX
        if currY < minY:
            minY = currY
        if currY > maxY:
            maxY = currY

    width = maxX - minX + 1
    height = maxY - minY + 1
    start = (-minX, -minY)
    grid = [[[] for col in range(0, width)] for row in range(0, height)]

    return (start, grid)

def digEdge(start, grid, cmds):
    (posX, posY) = start
    totalDug = 0

    for cmd in cmds:
        (dirStr, num, color) = cmd
        (dirX, dirY) = dirStrToInts(dirStr)

        grid[posY][posX].append(dirStr + color)
        for i in range(0, num):
            posX += dirX
            posY += dirY
            grid[posY][posX].append(dirStr + color)
            totalDug += 1

    return totalDug

def printGrid(grid):
    for row in grid:
        rowArr = []
        print(''.join(['#' if len(col) > 0 else '.' for col in row]))

def fillInside(grid, start, cmds):
    # finding an inside tile
    (startX, startY) = start
    (dirStr, _, _) = cmds[0]
    (dirX, dirY) = dirStrToInts(dirStr)
    startX += dirX
    startY += dirY
    if isClockwise(cmds):
        (dirX, dirY) = (-dirY, dirX)
    else:
        (dirX, dirY) = (dirY, -dirX)
    startX += dirX
    startY += dirY

    # flood-filling from that tile
    toSearch = [(startX, startY)]
    grid[startY][startX].append("I")
    totalDug = 1
    searchDirs = [(1, 0), (-1, 0), (0, 1), (0, -1)]
    while len(toSearch) > 0:
        (currX, currY) = toSearch.pop()
        for searchDir in searchDirs:
            (dirX, dirY) = searchDir
            nextX = currX + dirX
            nextY = currY + dirY
            if len(grid[nextY][nextX]) == 0:
                toSearch.append((nextX, nextY))
                grid[nextY][nextX].append("I")
                totalDug += 1

    return totalDug
    
def partOne(lines):
    cmds = toCommands(lines)
    (start, grid) = initGrid(cmds)

    totalDug = 0
    totalDug += digEdge(start, grid, cmds)
    totalDug += fillInside(grid, start, cmds)

    return totalDug

def partTwo(lines):
    return 1

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
