// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Our Files
#include "inputparse.h"

#define uint unsigned int
#define llint long long int

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

llint getNext(const vector<llint>& seq) {
    // base case: all nums are zero; return zero
    bool allZero = true;
    for (llint num : seq) {
        if (num != 0) {
            allZero = false;
            break;
        }
    }
    if (allZero) {return 0;}

    // recursive case: get diffs, find next in diffs, add to final
    vector<llint> diffs;
    for (uint i = 1; i < seq.size(); i++) {
        diffs.push_back(seq.at(i) - seq.at(i - 1));
    }

    llint diffsNext = getNext(diffs);

    return seq.at(seq.size() - 1) + diffsNext;
}

llint getPrev(const vector<llint>& seq) {
    // base case: all nums are zero; return zero
    bool allZero = true;
    for (llint num : seq) {
        if (num != 0) {
            allZero = false;
            break;
        }
    }
    if (allZero) {return 0;}

    // recursive case: get diffs, find prev in diffs, add to first
    vector<llint> diffs;
    for (uint i = 1; i < seq.size(); i++) {
        diffs.push_back(seq.at(i) - seq.at(i - 1));
    }

    llint diffsPrev = getPrev(diffs);

    return seq.at(0) - diffsPrev;
}

llint partOne(const vector<string> &input) {
    llint total = 0;
    for (string line : input) {
        vector<llint> nums = vecStrToLLInt(split(line, " "));
        total += getNext(nums);
    }

    return total;
}

llint partTwo(const vector<string> &input) {
    llint total = 0;
    for (string line : input) {
        vector<llint> nums = vecStrToLLInt(split(line, " "));
        total += getPrev(nums);
    }

    return total;
}
