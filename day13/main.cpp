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

vector< vector<string> > getImages(const vector<string> &input) {
    vector< vector<string> > allImages;
    vector<string> currImage;
    for (string line : input) {
        if (line.empty()) {
            allImages.push_back(currImage);
            currImage.clear();
        } else {
            currImage.push_back(line);
        }
    }
    allImages.push_back(currImage);

    return allImages;
}

/*
bool rowEqual(const vector<string> &image, uint row1, uint row2) {
    return image.at(row1) == image.at(row2);
}

bool colEqual(const vector<string> &image, uint col1, uint col2) {
    for (uint row = 0; row < image.size(); row++) {
        if (image.at(row).at(col1) != image.at(row).at(col2)) {return false;}
    }
    return true;
}

// returns a vector of horiz mirrors' rows.
// a mirror between rows n and n+1 will return n+1
vector<uint> getHorizMirrors(const vector<string> &image) {
    vector<uint> mirrors;

    for (uint row = 1; row < image.size(); row++) {
        bool isMirror = true;

        uint upperRow = row - 1;
        uint lowerRow = row;
        while (true) {
            if (!rowEqual(image, upperRow, lowerRow)) {
                isMirror = false;
                break;
            }

            if (upperRow == 0 || lowerRow == image.size() - 1) {break;}

            upperRow--;
            lowerRow++;
        }

        if (isMirror) {mirrors.push_back(row);}
    }

    return mirrors;
}

// returns a vector of vert mirrors' cols.
// a mirror between cols n and n+1 will return n+1
vector<uint> getVertMirrors(const vector<string> &image) {
    vector<uint> mirrors;

    for (uint col = 1; col < image.at(0).size(); col++) {
        bool isMirror = true;

        uint upperCol = col - 1;
        uint lowerCol = col;
        while (true) {
            if (!colEqual(image, upperCol, lowerCol)) {
                isMirror = false;
                break;
            }

            if (upperCol == 0 || lowerCol == image.at(0).size() - 1) {break;}

            upperCol--;
            lowerCol++;
        }

        if (isMirror) {mirrors.push_back(col);}
    }

    return mirrors;
}
*/

uint rowDiff(const vector<string> &image, uint row1, uint row2) {
    uint diff = 0;
    for (uint col = 0; col < image.at(0).size(); col++) {
        if (image.at(row1).at(col) != image.at(row2).at(col)) {diff++;}
    }
    return diff;
}

uint colDiff(const vector<string> &image, uint col1, uint col2) {
    uint diff = 0;
    for (uint row = 0; row < image.size(); row++) {
        if (image.at(row).at(col1) != image.at(row).at(col2)) {diff++;}
    }
    return diff;
}

llint getScoreForDiff(const vector<string> &image, uint targetDiff) {
    // first looking for horizontal mirror
    for (uint row = 1; row < image.size(); row++) {
        uint diff = 0;

        uint upperRow = row - 1;
        uint lowerRow = row;
        while (true) {
            diff += rowDiff(image, upperRow, lowerRow);

            if (upperRow == 0 || lowerRow == image.size() - 1) {break;}

            upperRow--;
            lowerRow++;
        }

        if (diff == targetDiff) {return 100 * row;}
    }

    // then looking for vertical mirror
    for (uint col = 1; col < image.at(0).size(); col++) {
        uint diff = 0;

        uint upperCol = col - 1;
        uint lowerCol = col;
        while (true) {
            diff += colDiff(image, upperCol, lowerCol);

            if (upperCol == 0 || lowerCol == image.at(0).size() - 1) {break;}

            upperCol--;
            lowerCol++;
        }

        if (diff == targetDiff) {return col;}
    }

    // should never happen
    cerr << "found no mirror with diff " << targetDiff << endl;
    return -1;
}

llint partOne(const vector<string> &input) {
    vector< vector<string> > images = getImages(input);
    llint total = 0;

    for (auto image : images) {
        total += getScoreForDiff(image, 0);
    }

    return total;
}

llint partTwo(const vector<string> &input) {
    vector< vector<string> > images = getImages(input);
    llint total = 0;

    for (auto image : images) {
        total += getScoreForDiff(image, 1);
    }

    return total;
}
