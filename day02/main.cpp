// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Our Files
#include "inputparse.h"

const int MAX_RED = 12;
const int MAX_GREEN = 13;
const int MAX_BLUE = 14;

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

bool isPossible(const string &game) {
    const string gameContent = split(game, ": ")[1];
    const vector<string> rounds = split(gameContent, "; ");

    for (const string& round : rounds) {
        const vector<string> drawings = split(round, ", ");

        for (const string& drawing : drawings) {
            const vector<string> parts = split(drawing, " ");
            const int num = stoi(parts[0]);
            const string color = parts[1];

            if (color == "red" && num > MAX_RED) {
                return false;
            } else if (color == "green" && num > MAX_GREEN) {
                return false;
            } else if (color == "blue" && num > MAX_BLUE) {
                return false;
            }
        }
    }

    return true;
}

int getPower(const string &game) {
    int minRed = 0;
    int minGreen = 0;
    int minBlue = 0;

    const string gameContent = split(game, ": ")[1];
    const vector<string> rounds = split(gameContent, "; ");

    for (const string& round : rounds) {
        const vector<string> drawings = split(round, ", ");

        for (const string& drawing : drawings) {
            const vector<string> parts = split(drawing, " ");
            const int num = stoi(parts[0]);
            const string color = parts[1];

            if (color == "red" && num > minRed) {
                minRed = num;
            } else if (color == "green" && num > minGreen) {
                minGreen = num;
            } else if (color == "blue" && num > minBlue) {
                minBlue = num;
            }
        }
    }

    return minRed * minGreen * minBlue;
}

int partOne(const vector<string> &input) {
    int sum = 0;

    for (unsigned int i = 0; i < input.size(); i++) {
        const string game = input[i];
        if (isPossible(game)) {
            sum += (i + 1);
        }
    }

    return sum;
}

int partTwo(const vector<string> &input) {
    int sum = 0;

    for (const string& game : input) {
        sum += getPower(game);
    }

    return sum;
}
