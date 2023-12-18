// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
using namespace std;

// Our Files
#include "inputparse.h"

int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

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

vector<int> getNumericDigits(const string &line) {
    regex digitRegex("\\d");
    smatch digitMatch;
    vector<int> result;

    string curr = line;

    while (regex_search(curr, digitMatch, digitRegex)) {
        string num = digitMatch.str();
        curr = digitMatch.suffix();
        result.push_back(stoi(num));
    }

    return result;
}

map<string,int> digitMap = {
    {"one",1},
    {"two",2},
    {"three",3},
    {"four",4},
    {"five",5},
    {"six",6},
    {"seven",7},
    {"eight",8},
    {"nine",9}
};

vector<int> getStringyDigits(const string &line) {
    regex digitRegex("\\d|one|two|three|four|five|six|seven|eight|nine");
    smatch digitMatch;
    vector<int> result;

    string curr = line;

    while (regex_search(curr, digitMatch, digitRegex)) {
        string num = digitMatch.str();
        curr = curr.substr(digitMatch.position(0) + 1);
        if ((int)num[0] < (int)'a') {
            result.push_back(stoi(num));
        } else {
            result.push_back(digitMap[num]);
        }
    }

    return result;
}

int partOne(const vector<string> &input) {
    int sum = 0;
    for (string line : input) {
        vector<int> digits = getNumericDigits(line);
        sum += (10 * digits[0]) + digits[digits.size() - 1];
    }
    return sum;
}

int partTwo(const vector<string> &input) {
    int sum = 0;
    for (string line : input) {
        vector<int> digits = getStringyDigits(line);
        sum += (10 * digits[0]) + digits[digits.size() - 1];
    }
    return sum;
}
