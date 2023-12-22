#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

import functools

def getBricks(lines):
    bricks = []
    for line in lines:
        lineSpl = line.split('~')
        cornerOne = map(int, lineSpl[0].split(','))
        cornerTwo = map(int, lineSpl[1].split(','))
        bricks.append(tuple(cornerOne) + tuple(cornerTwo))
    return bricks

def lowerCompare(brickA, brickB):
    (_, _, az1, _, _, _) = brickA
    (_, _, bz1, _, _, _) = brickB
    return az1 - bz1

def upperCompare(brickA, brickB):
    (_, _, _, _, _, az2) = brickA
    (_, _, _, _, _, bz2) = brickB
    return az2 - bz2

def overlappingXY(brickA, brickB):
    (ax1, ay1, _, ax2, ay2, _) = brickA
    (bx1, by1, _, bx2, by2, _) = brickB

    overlapX = not (ax2 < bx1 or ax1 > bx2)
    overlapY = not (ay2 < by1 or ay1 > by2)
    return overlapX and overlapY

def getClosestBricksBelow(bricks, brick):
    sortedUpper = sorted(bricks, key=functools.cmp_to_key(upperCompare), reverse=True)
    below = []
    collideHeight = -1
    (_, _, lowerZ, _, _, _) = brick
    for currBrick in sortedUpper:
        (_, _, _, _, _, upperZ) = currBrick
        if upperZ >= lowerZ:
            continue
        elif upperZ < collideHeight:
            break
        elif overlappingXY(brick, currBrick):
            below.append(currBrick)
            collideHeight = upperZ
    return below

def getCollapsed(bricks):
    sortedLower = sorted(bricks, key=functools.cmp_to_key(lowerCompare))
    fallenBricks = []
    totalFell = 0
    for brick in sortedLower:
        closestBelow = getClosestBricksBelow(fallenBricks, brick)
        lowerZ = 1
        if len(closestBelow):
            (_, _, _, _, _, closestZ) = closestBelow[0]
            lowerZ = closestZ + 1
        (x1, y1, z1, x2, y2, z2) = brick
        fallenBricks.append((x1, y1, lowerZ, x2, y2, z2 - z1 + lowerZ))
        fallenBricks = sorted(fallenBricks, key=functools.cmp_to_key(upperCompare), reverse = True)
        if z1 != lowerZ:
            totalFell += 1
    return (fallenBricks, totalFell)

def partOne(lines):
    bricks = getBricks(lines)
    (collapsedBricks, _) = getCollapsed(bricks)

    directSupports = set()
    for brick in collapsedBricks:
        closestBelow = getClosestBricksBelow(collapsedBricks, brick)
        if len(closestBelow) == 1:
            directSupports.add(closestBelow[0])

    return len(bricks) - len(directSupports)

def partTwo(lines):
    bricks = getBricks(lines)
    (collapsedBricks, _) = getCollapsed(bricks)

    directSupports = set()

    total = 0
    for i, brick in enumerate(collapsedBricks):
        print(i)
        closestBelow = getClosestBricksBelow(collapsedBricks, brick)
        if len(closestBelow) == 1:
            directSupports.add(closestBelow[0])

        if brick in directSupports:
            bricksWithout = [b for b in collapsedBricks if b != brick]
            (_, fell) = getCollapsed(bricksWithout)
            total += fell

    return total

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
