// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <utility>
#include <map>
#include <set>
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

vector< pair<char, pair<int, int> > > getSymbolsNearby(const vector<string> &input, int row, int col) {
    vector< pair<char, pair<int, int> > > res;

    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (i < 0 || i >= input.size() || j < 0 || j >= input[0].size()) {
                continue;
            }

            char curr = input.at(i).at(j);

            if (curr == '.')
                continue;
            if ((int)curr >= (int)'0' && (int)curr <= (int)'9')
                continue;

            res.push_back({curr, {i, j}});
        }
    }

    return res;
}

int partOne(const vector<string> &input) {
    int sum = 0;

    const unsigned int height = input.size();
    const unsigned int width = input[0].size();

    int num = 0;
    bool parsingNum = false;
    bool foundSymbol = false;

    for (unsigned int row = 0; row < height; row++) {
        for (unsigned int col = 0; col < width; col++) {
            char curr = input.at(row).at(col);
            if ((int)curr >= (int)'0' && (int)curr <= (int)'9') {
                // adding digit to total
                num *= 10;
                num += (int)curr - (int)'0';
                parsingNum = true;

                // seeing if a symbol is nearby
                if (getSymbolsNearby(input, row, col).size() > 0) {
                    foundSymbol = true;
                }
            } else if (parsingNum) {
                parsingNum = false;

                if (foundSymbol) {
                    sum += num;
                }

                foundSymbol = false;
                num = 0;
            }
        }
    }

	return sum;
}

int partTwo(const vector<string> &input) {
    const unsigned int height = input.size();
    const unsigned int width = input[0].size();

    int num = 0;
    bool parsingNum = false;
    set< pair<int, int> > gearLocs;

    map< pair<int, int>, vector<int> > gearVals;

    for (unsigned int row = 0; row < height; row++) {
        for (unsigned int col = 0; col < width; col++) {
            char curr = input.at(row).at(col);
            if ((int)curr >= (int)'0' && (int)curr <= (int)'9') {
                // adding digit to total
                num *= 10;
                num += (int)curr - (int)'0';
                parsingNum = true;

                // seeing if a gear is nearby
                vector< pair<char, pair<int, int> > > symbols = getSymbolsNearby(input, row, col);
                for (auto symbol : symbols) {
                    if (symbol.first == '*') {
                        gearLocs.insert(symbol.second);
                    }
                }
            } else if (parsingNum) {
                for (auto gearLoc : gearLocs) {
                    gearVals[gearLoc].push_back(num);
                }

                parsingNum = false;
                num = 0;
                gearLocs.clear();
            }
        }
    }

    int sum = 0;

    for (auto gearVal : gearVals) {
        vector<int> nums = gearVal.second;

        if (nums.size() == 2) {
            sum += (nums.at(0) * nums.at(1));
        }
    }

	return sum;
}
