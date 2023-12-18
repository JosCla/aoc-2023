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

const uint NUM_RACES = 4;

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

vector< pair<llint, llint> > getRaces(const vector<string> &input) {
    regex spaceReg("\\s+");
    vector<string> times = splitReg(input.at(0), spaceReg);
    vector<string> dists = splitReg(input.at(1), spaceReg);

    vector< pair<llint, llint> > res;
    for (uint i = 1; i <= NUM_RACES; i++) {
        res.push_back({
            stoll(times.at(i)),
            stoll(dists.at(i))
        });
    }

    return res;
}

llint totalPossibilities(const pair<llint, llint> &race) {
    llint time = race.first;
    llint dist = race.second;

    llint possibilities = 0;
    for (llint i = 0; i <= time; i++) {
        if ((i * (time - i)) > dist) {
            possibilities++;
        }
    }

    return possibilities;
}

pair<llint, llint> getRace(const vector<string> &input) {
    llint time = stoll(split(ignoreChars(input.at(0), " "), ":").at(1));
    llint dist = stoll(split(ignoreChars(input.at(1), " "), ":").at(1));

    return {time, dist};
}

llint partOne(const vector<string> &input) {
    llint res = 1;
    vector< pair<llint, llint> > races = getRaces(input);

    for (auto race : races) {
        res *= totalPossibilities(race);
    }

	return res;
}

llint partTwo(const vector<string> &input) {
	return totalPossibilities(getRace(input));
}
