#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

# gets a nested array of characters from the input lines
def getGrid(lines):
    return [[c for c in line] for line in lines]

# slides a single tile in a given direction in the grid
def slideTile(grid, posX, posY, dirX, dirY):
    currX = posX
    currY = posY
    while True:
        nextX = currX + dirX
        nextY = currY + dirY

        if (nextY < 0 or nextY >= len(grid)) or (nextX < 0 or nextX >= len(grid[0])) or (grid[nextY][nextX] != '.'):
            break

        currX = nextX
        currY = nextY

    if (currX != posX) or (currY != posY):
        grid[currY][currX] = grid[posY][posX]
        grid[posY][posX] = '.'

# slides the whole grid in a certain direction
def slideGrid(grid, dirX, dirY):
    xRange = range(0, len(grid[0]))
    yRange = range(0, len(grid))
    if (dirX >= 0):
        xRange = range(len(grid[0]) - 1, -1, -1)
    if (dirY >= 0):
        yRange = range(len(grid) - 1, -1, -1)

    for y in yRange:
        for x in xRange:
            if grid[y][x] == 'O':
                slideTile(grid, x, y, dirX, dirY)

# prints the grid to stdout, line by line
def printGrid(grid):
    for row in grid:
        print(''.join(row))

# gets the load on the northern side
def getLoad(grid):
    load = 0
    for y in range(0, len(grid)):
        for x in range(0, len(grid[0])):
            if grid[y][x] == 'O':
                load += len(grid) - y

    return load

# does a single "cycle" of the grid (slide up, left, down, right)
def cycleGrid(grid):
    slideGrid(grid, 0, -1)
    slideGrid(grid, -1, 0)
    slideGrid(grid, 0, 1)
    slideGrid(grid, 1, 0)

def partOne(lines):
    grid = getGrid(lines)

    slideGrid(grid, 0, -1)

    return getLoad(grid)

def partTwo(lines):
    gridOne = getGrid(lines)
    gridTwo = getGrid(lines)

    stepsOne = 0
    stepsTwo = 0
    while (True):
        cycleGrid(gridOne)
        stepsOne = stepsOne + 1

        cycleGrid(gridTwo)
        cycleGrid(gridTwo)
        stepsTwo = stepsTwo + 2

        if (gridOne == gridTwo):
            # found a cycle! first get the cycle length
            cycleLen = 0
            while (True):
                cycleGrid(gridTwo)
                cycleLen = cycleLen + 1
                if (gridOne == gridTwo):
                    break

            # now, use that to find where in the cycle 1000000000 lies
            cyclePos = (1000000000 - stepsOne) % cycleLen

            # step to that position in the cycle
            for i in range(0, cyclePos):
                cycleGrid(gridOne)

            # return the load
            return getLoad(gridOne)

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
