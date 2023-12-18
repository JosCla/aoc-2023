// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
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

vector<int> spacedStrsToNums(const string &spacedStrs) {
    const vector<string> units = split(spacedStrs, " ");

    vector<int> res;
    for (string unit : units) {
        if (unit.length() > 0) {
            res.push_back(stoi(unit));
        }
    }

    return res;
}

vector<int> getWinningNums(const string &line) {
    const string card = split(line, ": ")[1];
    const string winsRaw = split(card, " | ")[0];

    return spacedStrsToNums(winsRaw);
}

vector<int> getDrawnNums(const string &line) {
    const string card = split(line, ": ")[1];
    const string drawnRaw = split(card, " | ")[1];

    return spacedStrsToNums(drawnRaw);
}

int partOne(const vector<string> &input) {
    int total = 0;

    for (const string& card : input) {
        vector<int> winning = getWinningNums(card);
        vector<int> drawn = getDrawnNums(card);

        int found = 0;
        for (int win : winning) {
            for (int draw : drawn) {
                if (win == draw) {
                    found++;
                    break;
                }
            }
        }

        int score = 0;
        if (found > 0) {
            score = 1;
            for (int i = 1; i < found; i++) {
                score *= 2;
            }
        }

        total += score;
    }

    return total;
}

int partTwo(const vector<string> &input) {
    int total = 0;

    vector<int> cardCounts(input.size(), 1);

    for (unsigned int cardNum = 0; cardNum < input.size(); cardNum++) {
        const string& card = input.at(cardNum);
        vector<int> winning = getWinningNums(card);
        vector<int> drawn = getDrawnNums(card);

        int found = 0;
        for (int win : winning) {
            for (int draw : drawn) {
                if (win == draw) {
                    found++;
                    break;
                }
            }
        }

        for (int i = 1; i <= found; i++) {
            cardCounts.at(cardNum + i) += cardCounts.at(cardNum);
        }

        total += cardCounts.at(cardNum);
    }

    return total;
}
