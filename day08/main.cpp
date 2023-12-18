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

string getInstrs(const vector<string> &input) {
    return input.at(0);
}

map<string, pair<string, string> > getMap(const vector<string> &input) {
    map<string, pair<string, string> > res;

    for (uint i = 2; i < input.size(); i++) {
        string line = input.at(i);

        vector<string> mainSplit = split(line, " = ");
        string key = mainSplit.at(0);

        vector<string> pathSplit = split(mainSplit.at(1).substr(1, 8), ", ");
        pair<string, string> vals = {pathSplit.at(0), pathSplit.at(1)};

        res.insert({key, vals});
    }

    return res;
}

string getNext(
    const string& curr,
    const string& instrs,
    const llint steps,
    map<string, pair<string, string> >& lrMap
) {
    char instr = instrs.at(steps % instrs.size());
    if (instr == 'L') {
        return lrMap[curr].first;
    } else {
        return lrMap[curr].second;
    }
}

llint gcd(llint n1, llint n2) {
    while (true) {
        while (n1 > n2) {
            n1 -= n2;
        }
        while (n2 > n1) {
            n2 -= n1;
        }

        if (n1 == n2) {return n1;}
    }
}

llint lcm(llint n1, llint n2) {
    return (n1 * n2) / gcd(n1, n2);
}

llint partOne(const vector<string> &input) {
    string instrs = getInstrs(input);
    auto lrMap = getMap(input);

    llint steps = 0;
    string curr = "AAA";
    while (true) {
        if (curr == "ZZZ") {break;}

        curr = getNext(curr, instrs, steps, lrMap);
        steps++;
    }

	return steps;
}

llint partTwo(const vector<string> &input) {
    string instrs = getInstrs(input);
    auto lrMap = getMap(input);

    vector<string> currs;
    for (auto entry : lrMap) {
        if (entry.first.at(2) == 'A') {
            currs.push_back(entry.first);
        }
    }

    // just gonna assume all the cycles are 0-based.
    // and gonna assume each A only goes through one Z.
    // edit: yeah it worked lol. bad problem though.
    llint res = 1;

    for (string baseCurr : currs) {
        llint steps = 0;
        string curr = baseCurr;

        while (true) {
            if (curr.at(2) == 'Z') {break;}

            curr = getNext(curr, instrs, steps, lrMap);
            steps++;
        }

        res = lcm(res, steps);
    }

	return res;
}
