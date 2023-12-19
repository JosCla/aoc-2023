#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

import copy

def getWorkflows(lines):
    workflows = {}
    for line in lines:
        if len(line) == 0:
            return workflows

        lineSplit = line.split('{')
        name = lineSplit[0]

        instrs = lineSplit[1][:-1].split(',')
        workflows[name] = instrs

def getParts(lines):
    parts = []
    seenEmpty = False
    for line in lines:
        if len(line) == 0:
            seenEmpty = True
            continue
        elif not seenEmpty:
            continue

        part = {}
        for category in line[1:-1].split(','):
            catSplit = category.split('=')
            part[catSplit[0]] = int(catSplit[1])
        parts.append(part)

    return parts

# returns True if accepted, False if rejected
def processPart(workflows, part):
    name = 'in'
    while True:
        workflow = workflows[name]
        for instr in workflow:
            if instr == 'A':
                # auto-accept
                return True
            elif instr == 'R':
                # auto-reject
                return False

            colonSplit = instr.split(':')
            if len(colonSplit) == 1:
                # jump to workflow
                name = instr
                break

            condTrue = False
            gtSplit = colonSplit[0].split('>')
            if len(gtSplit) == 1:
                # less than
                ltSplit = colonSplit[0].split('<')
                condTrue = part[ltSplit[0]] < int(ltSplit[1])
            else:
                # greater than
                condTrue = part[gtSplit[0]] > int(gtSplit[1])

            if condTrue:
                if colonSplit[1] == 'A':
                    return True
                elif colonSplit[1] == 'R':
                    return False
                else:
                    name = colonSplit[1]
                    break

def totalAcceptedIn(workflows, name, instrNum, ranges):
    while True:
        workflow = workflows[name]
        for i in range(instrNum, len(workflow)):
            instr = workflow[i]
            if instr == 'A':
                # auto-accept
                return getRangesSize(ranges)
            elif instr == 'R':
                # auto-reject
                return 0

            colonSplit = instr.split(':')
            if len(colonSplit) == 1:
                # jump to workflow
                name = instr
                instrNum = 0
                break

            gtSplit = colonSplit[0].split('>')
            if len(gtSplit) == 1:
                # less than
                ltSplit = colonSplit[0].split('<')
                bound = int(ltSplit[1])
                (r1, r2) = ranges[ltSplit[0]]

                total = 0

                # true branch
                if r1 < bound:
                    rangesTrue = copy.deepcopy(ranges)
                    # rangesTrue = [key: value for key, value in ranges.items()]
                    rangesTrue[ltSplit[0]] = (r1, bound - 1)

                    if colonSplit[1] == 'A':
                        total += getRangesSize(rangesTrue)
                    elif colonSplit[1] == 'R':
                        total += 0
                    else:
                        total += totalAcceptedIn(workflows, colonSplit[1], 0, rangesTrue)

                # false branch
                if r2 >= bound:
                    rangesFalse = copy.deepcopy(ranges)
                    rangesFalse[ltSplit[0]] = (bound, r2)

                    total += totalAcceptedIn(workflows, name, i + 1, rangesFalse)
                
                return total
            else:
                # greater than
                bound = int(gtSplit[1])
                (r1, r2) = ranges[gtSplit[0]]

                total = 0

                # true branch
                if r2 > bound:
                    rangesTrue = copy.deepcopy(ranges)
                    rangesTrue[gtSplit[0]] = (bound + 1, r2)

                    if colonSplit[1] == 'A':
                        total += getRangesSize(rangesTrue)
                    elif colonSplit[1] == 'R':
                        total += 0
                    else:
                        total += totalAcceptedIn(workflows, colonSplit[1], 0, rangesTrue)

                # false branch
                if r1 <= bound:
                    rangesFalse = copy.deepcopy(ranges)
                    rangesFalse[gtSplit[0]] = (r1, bound)

                    total += totalAcceptedIn(workflows, name, i + 1, rangesFalse)
                
                return total

def getRangesSize(ranges):
    total = 1
    for currRange in ranges.values():
        (r1, r2) = currRange
        total *= r2 - r1 + 1
    return total

def getValue(part):
    return part['x'] + part['m'] + part['a'] + part['s']

def partOne(lines):
    parts = getParts(lines)
    workflows = getWorkflows(lines)

    total = 0
    for part in parts:
        if processPart(workflows, part):
            total += getValue(part)
        
    return total

def partTwo(lines):
    workflows = getWorkflows(lines)
    ranges = {
        'x': (1, 4000),
        'm': (1, 4000),
        'a': (1, 4000),
        's': (1, 4000),
    }

    return totalAcceptedIn(workflows, 'in', 0, ranges)

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
