#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

import numpy as np

def getHailstones(lines):
    hailstones = []
    for line in lines:
        [pos, vel] = line.split(" @ ")
        posTup = tuple(map(int, pos.split(", ")))
        velTup = tuple(map(int, vel.split(", ")))
        hailstones.append((posTup, velTup))
    return hailstones

def getPathCollision(stoneA, stoneB):
    ((posXA, posYA, _), (velXA, velYA, _)) = stoneA
    ((posXB, posYB, _), (velXB, velYB, _)) = stoneB

    A = np.array([
        [0, 1, -velYA, 0],
        [1, 0, -velXA, 0],
        [0, 1, 0, -velYB],
        [1, 0, 0, -velXB]
    ])
    b = np.array([posYA, posXA, posYB, posXB])

    try:
        soln = np.linalg.solve(A, b)
        [colX, colY, colT1, colT2] = soln
        if colT1 >= 0 and colT2 >= 0:
            return (colX, colY)
        else:
            return None
    except:
        return None

def getPosAt(stone, time):
    ((posX, posY, posZ), (velX, velY, velZ)) = stone
    return (posX + (velX * time), posY + (velY * time), posZ + (velZ * time))

def getThrowStone(stoneA, tA, stoneB, tB):
    ((posXA, posYA, posZA), (velXA, velYA, velZA)) = stoneA
    ((posXB, posYB, posZB), (velXB, velYB, velZB)) = stoneB

    (tAPosX, tAPosY, tAPosZ) = getPosAt(stoneA, tA)
    (tBPosX, tBPosY, tBPosZ) = getPosAt(stoneB, tB)

    velX = (tBPosX - tAPosX) / (tB - tA)
    velY = (tBPosY - tAPosY) / (tB - tA)
    velZ = (tBPosZ - tAPosZ) / (tB - tA)
    posX = tAPosX - (tA * velX)
    posY = tAPosY - (tA * velY)
    posZ = tAPosZ - (tA * velZ)
    return ((posX, posY, posZ), (velX, velY, velZ))

def getCost(stoneA, tA, stoneB, tB, stoneC, tC):
    predictStone = getThrowStone(stoneA, tA, stoneB, tB)
    (predX, predY, predZ) = getPosAt(predictStone, tC)
    (realX, realY, realZ) = getPosAt(stoneC, tC)

    return (predX - realX)**2 + (predY - realY)**2 + (predZ - realZ)**2

def optimalCost(center, step, numSteps, stoneA, stoneB, stoneC):
    (cTA, cTB, cTC) = center
    bestCost = 10**100
    bestCenter = (0, 0, 0)
    for tADiff in range(-step * numSteps, step * numSteps + 1, step):
        for tBDiff in range(-step * numSteps, step * numSteps + 1, step):
            for tCDiff in range(-step * numSteps, step * numSteps + 1, step):
                tA = cTA + tADiff
                tB = cTB + tBDiff
                tC = cTC + tCDiff
                if tA == tB or tB == tC or tA == tC:
                    continue

                cost = getCost(stoneA, tA, stoneB, tB, stoneC, tC)
                cost += getCost(stoneB, tB, stoneC, tC, stoneA, tA)
                cost += getCost(stoneC, tC, stoneA, tA, stoneB, tB)
                # print(tA, tB, tC, cost)
                if cost < bestCost:
                    bestCost = cost
                    bestCenter = (tA, tB, tC)
    return bestCenter

def partOne(lines):
    rangeMin = 200000000000000
    rangeMax = 400000000000000
    if len(lines) == 5:
        rangeMin = 7
        rangeMax = 27

    hailstones = getHailstones(lines)
    totalCollisions = 0
    for i in range(0, len(hailstones)):
        for j in range(i+1, len(hailstones)):
            collision = getPathCollision(hailstones[i], hailstones[j])
            if collision != None:
                (colPX, colPY) = collision
                if (colPX >= rangeMin and colPX <= rangeMax) and (colPY >= rangeMin and colPY <= rangeMax):
                    totalCollisions += 1

    return totalCollisions

def partTwo(lines):
    hailstones = getHailstones(lines)
    center = (0, 0, 0)
    base = 2
    maxPower = 64
    rangeLen = 10

    indA = 0
    indB = 10
    indC = 200

    for radix in range(maxPower, -1, -1):
        center = optimalCost(center, base**radix, rangeLen, hailstones[indA], hailstones[indB], hailstones[indC])

    tA, tB, tC = center
    if tA == tB or tB == tC or tA == tC:
        print("cost: indeterminate")
    else:
        cost = getCost(hailstones[indA], tA, hailstones[indB], tB, hailstones[indC], tC)
        cost += getCost(hailstones[indB], tB, hailstones[indC], tC, hailstones[indA], tA)
        cost += getCost(hailstones[indC], tC, hailstones[indA], tA, hailstones[indB], tB)
        print("cost:", cost)
        
    ((cX, cY, cZ), _) = getThrowStone(hailstones[indA], tA, hailstones[indB], tB)
    return int(cX + cY + cZ)

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
