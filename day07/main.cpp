// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <algorithm>
using namespace std;

// Our Files
#include "inputparse.h"

#define uint unsigned int

const uint NUM_CARDS = 5;

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

// returns 1 if c1>c2, -1 if c1<c2, 0 if equal
const vector<char> strengths1 = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
const vector<char> strengths2 = {'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J'};
int compareCards(char c1, char c2, bool isTwo) {
    const vector<char> &strengths = (isTwo ? strengths2 : strengths1);

    for (char s : strengths) {
        if (c1 == s && c2 == s) {return 0;}
        if (c1 == s) {return 1;}
        if (c2 == s) {return -1;}
    }

    return 0;
}

class Hand {
public:
    Hand(const string& cards, int bet, bool isTwo = false) {
        this->isTwo = isTwo;
        this->cards = cards;
        this->bet = bet;

        score();
    }

    void score() {
        map<char, int> counts;
        int jokers = 0;

        for (char c : cards) {
            if (isTwo && c == 'J') {
                jokers++;
            } else {
                counts[c]++;
            }
        }

        for (pair<char, int> count : counts) {
            sortedCounts.push_back({count.second, count.first});
        }

        if (sortedCounts.size() == 0) {
            // special case: all jokers!!!
            sortedCounts.push_back({0, 'J'});
        }

        sort(sortedCounts.begin(), sortedCounts.end());
        reverse(sortedCounts.begin(), sortedCounts.end());

        sortedCounts.at(0).first += jokers;
    }

    bool operator <(Hand& otherHand) {
        // first comparing hand type
        auto thisCounts = getSortedCounts();
        auto otherCounts = otherHand.getSortedCounts();
        uint elemsToCompare = min(thisCounts.size(), otherCounts.size());

        for (uint i = 0; i < elemsToCompare; i++) {
            if (thisCounts.at(i).first < otherCounts.at(i).first) {
                return true;
            } else if (thisCounts.at(i).first > otherCounts.at(i).first) {
                return false;
            }
        }

        // then if no conclusion has been reached, comparing card strengths
        string thisCards = getCards();
        string otherCards = otherHand.getCards();
        for (uint i = 0; i < NUM_CARDS; i++) {
            int comp = compareCards(thisCards.at(i), otherCards.at(i), isTwo);
            if (comp == -1) {return true;}
            if (comp == 1) {return false;}
        }

        cout << "equal hands: " << thisCards << "," << otherCards << endl;
        return false;
    }

    string getCards() {return cards;}
    int getBet() {return bet;}
    vector< pair<int, char> > getSortedCounts() {return sortedCounts;}

private:
    bool isTwo;
    string cards;
    int bet;

    vector< pair<int, char> > sortedCounts;
};

int getWinnings(const vector<string> &input, bool isTwo = false) {
    vector<Hand> hands;
	for (string line : input) {
        auto parts = split(line, " ");
        Hand h(parts.at(0), stoi(parts.at(1)), isTwo);
        hands.push_back(h);
    }

    sort(hands.begin(), hands.end());

    int sum = 0;
    for (uint i = 0; i < hands.size(); i++) {
        sum += (i + 1) * hands.at(i).getBet();
    }

    return sum;
}

int partOne(const vector<string> &input) {
    return getWinnings(input);
}

int partTwo(const vector<string> &input) {
	return getWinnings(input, true);
}
