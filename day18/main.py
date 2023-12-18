#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

def dirStrToInts(dirStr):
    dirDict = {
        'U': (0, -1),
        'D': (0, 1),
        'L': (-1, 0),
        'R': (1, 0)
    }
    return dirDict[dirStr]

def toCommands(lines):
    res = []
    for line in lines:
        lineSplit = line.split(' ')
        numDir = dirStrToInts(lineSplit[0])
        res.append((numDir, int(lineSplit[1])))
    return res

def toColorCommands(lines):
    res = []
    dirs = [(1, 0), (0, 1), (-1, 0), (0, -1)]
    for line in lines:
        lineSplit = line.split(' ')
        color = lineSplit[2][2:-1]
        colorDir = dirs[int(color[5])]
        colorNum = int(color[0:5], 16)
        res.append((colorDir, colorNum))

    return res

# gets all points of a 1D border of the shape
# (assumes the input winds clockwise. TODO: generalize to CCW, too)
def getBorderPoints(cmds):
    (prevDir, _) = cmds[len(cmds) - 1]
    posX = 0
    posY = 0

    points = []

    for cmd in cmds:
        (currDir, currNum) = cmd

        (dirX, dirY) = currDir
        (prevDX, prevDY) = prevDir
        if dirX == -prevDY and dirY == prevDX:
            # turned CW
            if dirX == -1:
                points.append((posX + 1, posY + 1))
            elif dirX == 1:
                points.append((posX, posY))
            elif dirY == -1:
                points.append((posX, posY + 1))
            elif dirY == 1:
                points.append((posX + 1, posY))
        else:
            # turned CCW
            if dirX == -1:
                points.append((posX, posY + 1))
            elif dirX == 1:
                points.append((posX + 1, posY))
            elif dirY == -1:
                points.append((posX, posY))
            elif dirY == 1:
                points.append((posX + 1, posY + 1))

        posX += dirX * currNum
        posY += dirY * currNum
        prevDir = currDir

    return points

# given a list of border points, returns a list of horiz segments for points
# each consists of a seg srart and end X, and a sorted array of y-coordinates
#   where horiz lines pass through this segment
# assumes the first command is a horiz line (TODO: generalize to vert lines)
def getHorizontalSegments(borderPoints):
    allX = []
    for point in borderPoints:
        (pX, pY) = point
        allX.append(pX)
    sortedX = sorted(list(dict.fromkeys(allX)))

    segArrs = [[] for x in sortedX]
    segArrs.pop()
    for p in range(0, len(borderPoints), 2):
        (p1X, p1Y) = borderPoints[p]
        (p2X, p2Y) = borderPoints[p + 1]
        lowX = min(p1X, p2X)
        highX = max(p1X, p2X)
        for i in range(0, len(sortedX)):
            x = sortedX[i]
            if x >= lowX and x < highX:
                segArrs[i].append(p1Y)

    res = []
    for i in range(0, len(sortedX) - 1):
        sortedSegs = sorted(segArrs[i])
        res.append((sortedX[i], sortedX[i + 1], sortedSegs))

    return res

# gets the area contained by the given commands
def getContainedArea(cmds):
    borderPoints = getBorderPoints(cmds)
    segs = getHorizontalSegments(borderPoints)

    total = 0
    for seg in segs:
        (startX, endX, heights) = seg
        segLen = endX - startX
        coveredHeight = 0
        for i in range(0, len(heights), 2):
            coveredHeight += heights[i + 1] - heights[i]
        total += segLen * coveredHeight

    return total

def partOne(lines):
    cmds = toCommands(lines)
    return getContainedArea(cmds)

def partTwo(lines):
    cmds = toColorCommands(lines)
    return getContainedArea(cmds)

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
