// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
using namespace std;

// Our Files
#include "inputparse.h"

#define uint unsigned int
#define llint long long int

const uint NUM_REPS = 5;

llint partOne(const vector<string> &input);
llint partTwo(const vector<string> &input);

int main() {
	vector<string> inputLines = readFileToLines("input");
	if (inputLines.empty()) {
		return 1;
	}
	inputLines.pop_back();

	cout << "Part One: " << partOne(inputLines) << endl;
	cout << "Part Two: " << partTwo(inputLines) << endl;

	return 0;
}

string getLine(const string &inputLine) {
    return split(inputLine, " ").at(0);
}

vector<uint> getGroups(const string &inputLine) {
    string numListStr = split(inputLine, " ").at(1);
    vector<string> numListStrs = split(numListStr, ",");
    return vecStrToUInt(numListStrs);
}

class Line {
public:
    Line(string line, vector<uint> groups) {
        this->line = line;
        this->groups = groups;
    }

    llint numPossibilities() {
        // creating DP table
        vector< vector<llint> > dp;
        for (uint a = 0; a <= groups.size(); a++) {
            vector<llint> curr(line.size() + 1, -1);
            dp.push_back(curr);
        }

        // base cases
        bool seenHash = false;
        for (uint b = 0; b <= line.size(); b++) {
            if (b > 0 && line.at(b - 1) == '#') {seenHash = true;}

            if (seenHash) {
                dp.at(0).at(b) = 0;
            } else {
                dp.at(0).at(b) = 1;
            }
        }

        for (uint a = 1; a <= groups.size(); a++) {
            dp.at(a).at(0) = 0;
        }

        // recursive cases
        for (uint a = 1; a <= groups.size(); a++) {
            uint lastGroup = groups.at(a - 1);
            for (uint b = 1; b <= line.size(); b++) {
                if (b < lastGroup) {
                    dp.at(a).at(b) = 0;
                    continue;
                }

                llint total = 0;
                for (int s = b - lastGroup; s >= 0; s--) {
                    if (isValidPlacement(s, lastGroup, b)) {
                        if (s > 0) {
                            total += dp.at(a - 1).at(s - 1);
                        } else {
                            total += dp.at(a - 1).at(0);
                        }
                    }
                }
                dp.at(a).at(b) = total;
            }
        }

        return dp.at(groups.size()).at(line.size());
    }

private:
    string line;
    vector<uint> groups;

    bool isValidPlacement(uint pos, uint size, uint lineEnd) {
        // if empty space in range, fail
        for (uint i = 0; i < size; i++) {
            if (line.at(pos + i) == '.') {return false;}
        }

        // if hash directly before, fail
        if (pos > 0 && line.at(pos - 1) == '#') {return false;}

        // if leaves trailing hashes, fail
        uint end = pos + size;
        for (uint i = end; i < lineEnd; i++) {
            if (line.at(i) == '#') {return false;}
        }

        return true;
    }
};

llint partOne(const vector<string> &input) {
    llint total = 0;
    for (string inputLine : input) {
        string lineStr = getLine(inputLine);
        vector<uint> groups = getGroups(inputLine);
        Line line(lineStr, groups);

        llint numPoss = line.numPossibilities();
        total += numPoss;

        // cout << inputLine << ": " << numPoss << endl;
    }

    return total;
}

llint partTwo(const vector<string> &input) {
    llint total = 0;
    for (string inputLine : input) {
        string lineStr = getLine(inputLine);
        vector<uint> groups = getGroups(inputLine);

        string actualLineStr = "";
        vector<uint> actualGroups;
        for (uint i = 1; i <= NUM_REPS; i++) {
            actualLineStr += lineStr;
            if (i < NUM_REPS) {actualLineStr += '?';}

            for (uint group : groups) {
                actualGroups.push_back(group);
            }
        }

        Line line(actualLineStr, actualGroups);

        llint numPoss = line.numPossibilities();
        total += numPoss;

        // cout << inputLine << ": " << numPoss << endl;
    }

    return total;
}
