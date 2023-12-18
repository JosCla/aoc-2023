#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

def getSteps(lines):
    return lines[0].split(',')

def getHash(step):
    total = 0
    for c in step:
        total += ord(c)
        total *= 17
        total %= 256
    return total

def partOne(lines):
    return sum(getHash(step) for step in getSteps(lines))

def partTwo(lines):
    boxes = [[] for _ in range(0,256)]

    # doing steps
    for step in getSteps(lines):
        eqSplit = step.split('=')
        if len(eqSplit) == 2:
            # this is an 'equals' instruction
            label = eqSplit[0]
            box = getHash(label)
            focalLen = int(eqSplit[1])

            found = False
            for i in range(0, len(boxes[box])):
                (currLabel, currFocal) = boxes[box][i]
                if label == currLabel:
                    boxes[box][i] = (label, focalLen)
                    found = True
                    break
            if not found:
                boxes[box].append((label, focalLen))
        else:
            # this is a 'minus' instruction
            label = step[:-1]
            box = getHash(label)

            boxes[box] = [(l, f) for (l, f) in boxes[box] if l != label]

    # calculating focusing power
    total = 0
    for (boxNum, box) in enumerate(boxes):
        for (lensNum, (label, focus)) in enumerate(box):
            total += (boxNum + 1) * (lensNum + 1) * focus

    return total

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
