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

enum Dir {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

Dir opposite(Dir dir) {
    switch (dir) {
        case UP: return DOWN;
        case DOWN: return UP;
        case LEFT: return RIGHT;
        case RIGHT: return LEFT;
    }
    return UP;
}

Dir turnRight(Dir dir) {
    switch (dir) {
        case UP: return RIGHT;
        case RIGHT: return DOWN;
        case DOWN: return LEFT;
        case LEFT: return UP;
    }
    return UP;
}

Dir turnLeft(Dir dir) {
    return opposite(turnRight(dir));
}

vector< vector<char> > getMaze(const vector<string> &input) {
    vector< vector<char> > res;
    for (string line : input) {
        vector<char> curr;
        for (char c : line) {
            curr.push_back(c);
        }
        res.push_back(curr);
    }
    return res;
}

vector<Dir> getDirs(char tile) {
    switch (tile) {
        case '.': return {};
        case '|': return {UP, DOWN};
        case '-': return {LEFT, RIGHT};
        case 'L': return {UP, RIGHT};
        case 'J': return {UP, LEFT};
        case '7': return {DOWN, LEFT};
        case 'F': return {DOWN, RIGHT};
    }
    return {};
}

bool canGoDir(char tile, Dir dir) {
    vector<Dir> dirs = getDirs(tile);
    return find(dirs.begin(), dirs.end(), dir) != dirs.end();
}

pair<uint, uint> goDir(pair<uint, uint> pos, Dir dir) {
    switch (dir) {
        case UP: return {pos.first - 1, pos.second};
        case DOWN: return {pos.first + 1, pos.second};
        case LEFT: return {pos.first, pos.second - 1};
        case RIGHT: return {pos.first, pos.second + 1};
    }
    return pos;
}

// also replaces the start position with the relevant pipe
pair<uint, uint> getStart(vector< vector<char> > &maze) {
    for (uint row = 0; row < maze.size(); row++) {
        for (uint col = 0; col < maze.size(); col++) {
            if (maze.at(row).at(col) == 'S') {
                if (canGoDir(maze.at(row - 1).at(col), DOWN)) {
                    if (canGoDir(maze.at(row).at(col - 1), RIGHT)) {
                        maze.at(row).at(col) = 'J';
                    } else if (canGoDir(maze.at(row).at(col + 1), LEFT)) {
                        maze.at(row).at(col) = 'L';
                    } else {
                        maze.at(row).at(col) = '|';
                    }
                } else if (canGoDir(maze.at(row + 1).at(col), UP)) {
                    if (canGoDir(maze.at(row).at(col - 1), RIGHT)) {
                        maze.at(row).at(col) = '7';
                    } else {
                        maze.at(row).at(col) = 'F';
                    }
                } else {
                    maze.at(row).at(col) = '-';
                }

                return {row, col};
            }
        }
    }

    cerr << "found no start" << endl;
    return {0, 0};
}

// flood fills the discovered map from a certain position
// returns the number of tiles filled
llint floodFillFrom(
    vector< vector<bool> > &discovered,
    pair<uint, uint> start,
    bool &edge
) {
    // if the start is already discovered, just leave now
    if (discovered.at(start.first).at(start.second)) {return 0;}

    // mark curr pos as discovered
    discovered.at(start.first).at(start.second) = true;

    // else flood fill in all four directions
    llint total = 0;

    if (start.first > 0)
        total += floodFillFrom(discovered, goDir(start, UP), edge);
    else
        edge = true;

    if (start.first < discovered.size() - 1)
        total += floodFillFrom(discovered, goDir(start, DOWN), edge);
    else
        edge = true;

    if (start.second > 0)
        total += floodFillFrom(discovered, goDir(start, LEFT), edge);
    else
        edge = true;

    if (start.second < discovered.at(0).size() - 1)
        total += floodFillFrom(discovered, goDir(start, RIGHT), edge);
    else
        edge = true;

    return 1 + total;
}

llint partOne(const vector<string> &input) {
    vector< vector<char> > maze = getMaze(input);
    pair<uint, uint> start = getStart(maze);

    llint steps = 1;
    Dir lastDir = getDirs(maze.at(start.first).at(start.second)).at(0);
    pair<uint, uint> pos = goDir(start, lastDir);

    while (true) {
        if (pos == start) {break;}

        vector<Dir> dirs = getDirs(maze.at(pos.first).at(pos.second));
        if (opposite(dirs.at(0)) == lastDir) {
            lastDir = dirs.at(1);
        } else {
            lastDir = dirs.at(0);
        }

        pos = goDir(pos, lastDir);
        steps++;
    }

    return steps / 2;
}

llint partTwo(const vector<string> &input) {
    vector< vector<char> > maze = getMaze(input);
    pair<uint, uint> start = getStart(maze);

    vector< vector<bool> > discovered;
    for (uint row = 0; row < maze.size(); row++) {
        vector<bool> curr;
        for (uint col = 0; col < maze.size(); col++) {
            curr.push_back(false);
        }
        discovered.push_back(curr);
    }
    discovered.at(start.first).at(start.second) = true;

    // traversal 1: mark the loop as a "border wall"
    Dir lastDir = getDirs(maze.at(start.first).at(start.second)).at(0);
    pair<uint, uint> pos = goDir(start, lastDir);

    while (true) {
        discovered.at(pos.first).at(pos.second) = true;

        if (pos == start) {break;}

        vector<Dir> dirs = getDirs(maze.at(pos.first).at(pos.second));
        if (opposite(dirs.at(0)) == lastDir) {
            lastDir = dirs.at(1);
        } else {
            lastDir = dirs.at(0);
        }

        pos = goDir(pos, lastDir);
    }

    // traversal 2: go through the loop again, flood filling to the left
    //  and right of the loop
    lastDir = getDirs(maze.at(start.first).at(start.second)).at(0);
    pos = goDir(start, lastDir);

    llint leftTotal = 0;
    llint rightTotal = 0;
    bool edgeLeft = false;
    bool edgeRight = false;

    while (true) {
        if (pos == start) {break;}

        vector<Dir> dirs = getDirs(maze.at(pos.first).at(pos.second));
        if (opposite(dirs.at(0)) == lastDir) {
            lastDir = dirs.at(1);
        } else {
            lastDir = dirs.at(0);
        }

        bool foundEdgeLeft = false;
        llint leftRes = floodFillFrom(discovered, goDir(pos, turnLeft(lastDir)), foundEdgeLeft);
        leftTotal += leftRes;
        edgeLeft = edgeLeft || foundEdgeLeft;

        bool foundEdgeRight = false;
        llint rightRes = floodFillFrom(discovered, goDir(pos, turnRight(lastDir)), foundEdgeRight);
        rightTotal += rightRes;
        edgeRight = edgeRight || foundEdgeRight;

        pos = goDir(pos, lastDir);

        foundEdgeLeft = false;
        leftRes = floodFillFrom(discovered, goDir(pos, turnLeft(lastDir)), foundEdgeLeft);
        leftTotal += leftRes;
        edgeLeft = edgeLeft || foundEdgeLeft;

        foundEdgeRight = false;
        rightRes = floodFillFrom(discovered, goDir(pos, turnRight(lastDir)), foundEdgeRight);
        rightTotal += rightRes;
        edgeRight = edgeRight || foundEdgeRight;
    }

    if (edgeRight) {
        return leftTotal;
    } else {
        return rightTotal;
    }
}
