#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

def getGrid(lines):
    return [[c for c in line] for line in lines]

def traverseGrid(grid, covered, pos, vel):
    while True:
        # seeing if this position is already covered
        (row, col) = pos
        (vY, vX) = vel
        if vel in covered[row][col]:
            break
        else:
            covered[row][col].append(vel)

        # parsing command at current position
        command = grid[row][col]
        if command == '\\':
            vel = (vX, vY)
        elif command == '/':
            vel = (-vX, -vY)
        elif command == '|':
            if vX != 0:
                traverseGrid(grid, covered, pos, (1, 0))
                vel = (-1, 0)
        elif command == '-':
            if vY != 0:
                traverseGrid(grid, covered, pos, (0, 1))
                vel = (0, -1)

        # moving cursor
        (newVY, newVX) = vel
        pos = (row + newVY, col + newVX)

        # if oob, this cursor dies; exit
        (newRow, newCol) = pos
        if (newRow < 0 or newRow >= len(grid)) or (newCol < 0 or newCol >= len(grid[0])):
            break

def getEnergyFor(grid, startPos, startVel):
    covered = [[[] for col in row] for row in grid]
    traverseGrid(grid, covered, startPos, startVel)

    total = 0
    for row in covered:
        for col in row:
            if len(col) > 0:
                total += 1

    return total

def partOne(lines):
    grid = getGrid(lines)

    return getEnergyFor(grid, (0, 0), (0, 1))

def partTwo(lines):
    grid = getGrid(lines)

    # getting all start positions and velocities
    starts = []
    vels = []

    for row in range(0, len(grid)):
        # left
        starts.append((row, 0))
        vels.append((0, 1))

        # right
        starts.append((row, len(grid[0]) - 1))
        vels.append((0, -1))

    for col in range(0, len(grid[0])):
        # top
        starts.append((0, col))
        vels.append((1, 0))

        # bottom
        starts.append((len(grid) - 1, col))
        vels.append((-1, 0))

    # finding best start position and velocity
    maxEnergy = 0
    for (start, vel) in zip(starts, vels):
        energy = getEnergyFor(grid, start, vel)
        if (energy > maxEnergy):
            maxEnergy = energy

    return maxEnergy

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
