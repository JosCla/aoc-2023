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

class Universe {
public:
    Universe(const vector<string> &input) {
        vector<uint> colCounts(input.at(0).size(), 0);
        for (uint rowInd = 0; rowInd < input.size(); rowInd++) {
            string row = input.at(rowInd);
            uint rowCount = 0;

            for (uint colInd = 0; colInd < row.size(); colInd++) {
                if (row.at(colInd) == '#') {
                    rowCount++;
                    colCounts.at(colInd)++;
                    galaxyPoses.push_back({rowInd, colInd});
                }
            }

            if (rowCount == 0) {
                emptyRows.push_back(rowInd);
            }
        }

        for (uint colInd = 0; colInd < colCounts.size(); colInd++) {
            if (colCounts.at(colInd) == 0) {
                emptyCols.push_back(colInd);
            }
        }
    }

    llint sumAllDists(llint expandFactor = 1) {
        llint sum = 0;
        for (uint i = 0; i < galaxyPoses.size(); i++) {
            for (uint j = (i + 1); j < galaxyPoses.size(); j++) {
                sum += getDist(galaxyPoses.at(i), galaxyPoses.at(j), expandFactor);
            }
        }
        return sum;
    }

    llint getDist(pair<uint, uint> p1, pair<uint, uint> p2, llint expandFactor) {
        llint dist = 0;

        uint minRow = min(p1.first, p2.first);
        uint maxRow = max(p1.first, p2.first);
        uint minCol = min(p1.second, p2.second);
        uint maxCol = max(p1.second, p2.second);

        dist += (maxRow - minRow);
        dist += (maxCol - minCol);

        for (uint emptyRow : emptyRows) {
            if (emptyRow > minRow && emptyRow < maxRow) {
                dist += expandFactor;
            }
        }
        for (uint emptyCol : emptyCols) {
            if (emptyCol > minCol && emptyCol < maxCol) {
                dist += expandFactor;
            }
        }

        return dist;
    }

private:
    vector< pair<uint, uint> > galaxyPoses;
    vector<uint> emptyRows;
    vector<uint> emptyCols;
};

llint partOne(const vector<string> &input) {
    Universe u(input);
    return u.sumAllDists();
}

llint partTwo(const vector<string> &input) {
    Universe u(input);
    return u.sumAllDists(999999);
}
