#!/bin/python3

import sys
lines = []
for line in sys.stdin:
    lines.append(line[:-1])

class Module:
    def __init__(self, args):
        self.mods = args
        self.inputs = {}

    def getMods(self):
        return self.mods

    def addInput(self, sender):
        self.inputs[sender] = False

    def receivePulse(self, sender, isHigh):
        pass

    def broadcast(self, isHigh):
        return [(m, isHigh) for m in self.mods]

class Broadcast(Module):
    def __init__(self, args):
        super().__init__(args)

    def receivePulse(self, sender, isHigh):
        super().receivePulse(sender, isHigh)
        return self.broadcast(isHigh)

class FlipFlop(Module):
    def __init__(self, args):
        super().__init__(args)
        self.isHigh = False

    def receivePulse(self, sender, isHigh):
        super().receivePulse(sender, isHigh)
        if not isHigh:
            self.isHigh = not self.isHigh
            return self.broadcast(self.isHigh)
        else:
            return []

class Conjunction(Module):
    def __init__(self, args):
        super().__init__(args)

    def receivePulse(self, sender, isHigh):
        super().receivePulse(sender, isHigh)
        self.inputs[sender] = isHigh

        for lastPulse in self.inputs.values():
            if not lastPulse:
                return self.broadcast(True)
        return self.broadcast(False)

def getModule(line):
    name = line.split(" -> ")[0][1:]
    args = line.split(" -> ")[1].split(", ")
    if line[0] == 'b':
        return ("broadcaster", Broadcast(args))
    elif line[0] == '%':
        return (name, FlipFlop(args))
    elif line[0] == '&':
        return (name, Conjunction(args))

def getModules(lines):
    # getting modules
    modTuples = [getModule(line) for line in lines]
    mods = {name: val for name, val in modTuples}

    # hooking up inputs
    for key, val in mods.items():
        outputMods = val.getMods()
        for outputMod in outputMods:
            if outputMod in mods:
                mods[outputMod].addInput(key)

    return mods

def pressButton(mods, lookFor = ""):
    totalLow = 0
    totalHigh = 0
    lookSentHigh = False

    pulses = [("button", "broadcaster", False)]

    while len(pulses) > 0:
        nextPulses = []
        for pulse in pulses:
            (sender, receiver, isHigh) = pulse

            if isHigh:
                totalHigh += 1
            else:
                totalLow += 1

            if receiver in mods:
                res = mods[receiver].receivePulse(sender, isHigh)
                for nextRecv, nextIsHigh in res:
                    nextPulses.append((receiver, nextRecv, nextIsHigh))
                    if receiver == lookFor and nextIsHigh:
                        lookSentHigh = True

        pulses = nextPulses

    return (totalLow, totalHigh, lookSentHigh)

def partOne(lines):
    mods = getModules(lines)

    totalLow = 0
    totalHigh = 0

    for i in range(0, 1000):
        (currLow, currHigh, _) = pressButton(mods)
        totalLow += currLow
        totalHigh += currHigh

    return totalLow * totalHigh

def gcd(n1, n2):
    while n1 != n2:
        while n1 > n2:
            n1 -= n2
        (n1, n2) = (n2, n1)
    return n1

def lcm(n1, n2):
    return int(n1 * n2 / gcd(n1, n2))

def partTwo(lines):
    mods = getModules(lines)

    # finding conjunction just before rx
    rxConj = ""
    for key, val in mods.items():
        if "rx" in val.getMods():
            rxConj = key
            break

    # finding conjunctions to look for
    toLook = []
    for key, val in mods.items():
        if rxConj in val.getMods():
            toLook.append(key)

    # finding cycle sizes of those conjunctions
    res = 1
    for look in toLook:
        mods = getModules(lines)

        if not look in mods:
            break

        for i in range(1, 1000000000):
            (_, _, sent) = pressButton(mods, look)
            if sent:
                res = lcm(res, i)
                break

    return res

print("Part One:", partOne(lines))
print("Part Two:", partTwo(lines))
