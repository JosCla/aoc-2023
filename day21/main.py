#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

def findStart(lines):
    for row, line in enumerate(lines):
        for col, c in enumerate(line):
            if c == 'S':
                return (row, col)

def getAfterNSteps(lines, startPos, maxSteps):
    discovered = [[-1 for c in line] for line in lines]
    (sRow, sCol) = startPos
    discovered[sRow][sCol] = 0

    lastPositions = set([startPos])
    for step in range(1, maxSteps + 1):
        nextPositions = set()
        for posRow, posCol in lastPositions:
            for dirVecRow, dirVecCol in [(0, 1), (1, 0), (0, -1), (-1, 0)]:
                nextRow = dirVecRow + posRow
                nextCol = dirVecCol + posCol
                if (nextRow < 0 or nextRow >= len(lines)) or (nextCol < 0 or nextCol >= len(lines[0])):
                    continue
                if discovered[nextRow][nextCol] != -1:
                    continue

                if lines[nextRow][nextCol] != '#':
                    nextPositions.add((nextRow, nextCol))
                    discovered[nextRow][nextCol] = step

        lastPositions = nextPositions

    maxParity = maxSteps % 2
    total = 0
    for line in discovered:
        for n in line:
            if n != -1 and n % 2 == maxParity:
                total += 1
    return total

def findConvergents(lines, startPos):
    convEven = getAfterNSteps(lines, startPos, 500)
    convOdd = getAfterNSteps(lines, startPos, 501)

    return (convEven, convOdd)

def partOne(lines):
    maxSteps = 64
    if len(lines) == 11:
        maxSteps = 6

    return getAfterNSteps(lines, findStart(lines), maxSteps)

def partTwo(lines):
    maxSteps = 26501365
    if len(lines) == 11:
        return 1

    # these represent the number of squares covered after taking an infinite
    # but even/odd number of steps from the start point, within a single grid
    convEven, convOdd = findConvergents(lines, findStart(lines))

    # step 1: add every square we reach as if it fits the convergent
    totalOdd = 1 # middle square is odd
    totalEven = 0

    steps = int(len(lines) / 2) + 1
    layer = 0
    while steps <= maxSteps:
        if layer % 2 == 0:
            totalEven += (layer + 1) * 4
        else:
            totalOdd += (layer + 1) * 4
        layer += 1
        steps += len(lines)

    total = (totalEven * convEven) + (totalOdd * convOdd)

    # step 2: look at tips of diamond, correct for having fewer steps
    orthoStarts = [
        (0, int(len(lines) / 2)),
        (int(len(lines) / 2), 0),
        (len(lines) - 1, int(len(lines) / 2)),
        (int(len(lines) / 2), len(lines) - 1)
    ]
    for start in orthoStarts:
        currSteps = steps - len(lines)
        currLayer = layer - 1

        while True:
            stepsLeft = maxSteps - currSteps
            # print(stepsLeft)
            count = getAfterNSteps(lines, start, stepsLeft)
            # print(count)

            if currLayer % 2 == 0:
                diff = count - convEven
                if diff == 0:
                    break
                else:
                    total += diff
            else:
                diff = count - convOdd
                if diff == 0:
                    break
                else:
                    total += diff

            currSteps -= len(lines)
            currLayer -= 1

    # step 3: look at the long diagonal edges, correct for having fewer steps
    diagStarts = [
        (0, 0),
        (0, len(lines) - 1),
        (len(lines) - 1, 0),
        (len(lines) - 1, len(lines) - 1)
    ]
    for start in diagStarts:
        currSteps = steps - len(lines) + int(len(lines) / 2) + 1
        currLayer = layer

        while True:
            stepsLeft = maxSteps - currSteps
            # print(stepsLeft)
            count = getAfterNSteps(lines, start, stepsLeft)
            # print(count)

            diff = count

            if currLayer % 2 == 0:
                if currLayer != layer:
                    diff -= convEven
                if diff == 0:
                    break
                else:
                    total += diff * currLayer
            else:
                if currLayer != layer:
                    diff -= convOdd
                if diff == 0:
                    break
                else:
                    total += diff * currLayer

            currSteps -= len(lines)
            currLayer -= 1

    return total

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
