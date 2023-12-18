#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

import heapq

def toGrid(lines):
    return [[int(c) for c in line] for line in lines]

# converts a direction to an array index from 0 to 3
def dirToIndex(direction):
    (dirX, dirY) = direction
    return int((dirX + (3 * dirY) + 3) / 2)

def turnLeft(direction):
    (dirX, dirY) = direction
    return (-dirY, dirX)

def turnRight(direction):
    (dirX, dirY) = direction
    return (dirY, -dirX)

def alreadyBetter(best, loss, pos, direction, straight):
    (posX, posY) = pos
    dirIndex = dirToIndex(direction)
    bestLoss = best[posY][posX][dirIndex][straight]
    if bestLoss != -1 and bestLoss <= loss:
        return True
    else:
        best[posY][posX][dirIndex][straight] = loss
        return False

def pathfind(grid, minStraight, maxStraight):
    # each elem in queue is (heat loss, pos, dir, squares left in straight)
    queue = []
    heapq.heappush(queue, (grid[0][1], (1, 0), (1, 0), 0))
    heapq.heappush(queue, (grid[1][0], (0, 1), (0, 1), 0))

    best = [[[[-1 for straight in range(0, maxStraight)] for direction in range(0, 4)] for col in row] for row in grid]

    while len(queue) > 0:
        curr = heapq.heappop(queue)
        # print(curr)
        (loss, pos, direction, straight) = curr
        (posX, posY) = pos

        # seeing if we're at the end
        if posY == (len(grid) - 1) and posX == (len(grid[0]) - 1) and (straight + 1 >= minStraight):
            return loss

        # seeing if we've already gotten here with less loss
        if alreadyBetter(best, loss, pos, direction, straight):
            continue

        # discovering next steps, adding to priority queue
        toDiscover = []
        if (straight + 1) < maxStraight:
            toDiscover.append((direction, straight + 1))
        if (straight + 1) >= minStraight:
            toDiscover.append((turnLeft(direction), 0))
            toDiscover.append((turnRight(direction), 0))
        for discover in toDiscover:
            (nextDir, nextStraight) = discover
            (dirX, dirY) = nextDir
            nextX = posX + dirX
            nextY = posY + dirY

            if (nextX >= 0 and nextX < len(grid[0])) and (nextY >= 0 and nextY < len(grid)):
                nextLoss = loss + grid[nextY][nextX]
                heapq.heappush(queue, (nextLoss, (nextX, nextY), nextDir, nextStraight))

def partOne(lines):
    grid = toGrid(lines)
    return pathfind(grid, 0, 3)

def partTwo(lines):
    grid = toGrid(lines)
    return pathfind(grid, 4, 10)

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
