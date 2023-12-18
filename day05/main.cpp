// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Our Files
#include "inputparse.h"

#define llint long long int
#define uint unsigned int

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

struct MapTriplet {
    llint srcStart;
    llint destStart;
    llint length;

    MapTriplet(llint a, llint b, llint c) {
        destStart = a;
        srcStart = b;
        length = c;
    }

    bool doesConvert(llint src) {
        return (src >= srcStart && src < (srcStart + length));
    }

    llint convert(llint src) {
        return destStart + (src - srcStart);
    }

    pair< vector< pair<llint, llint> >, vector< pair<llint, llint> > > convertRange(pair<llint, llint> range) {
        llint srcEnd = srcStart + length;
        llint rangeStart = range.first;
        llint rangeLen = range.second;
        llint rangeEnd = rangeStart + rangeLen;
        // cout << "range: [" << rangeStart << "," << rangeEnd << ")" << endl;
        // cout << "map  : [" << srcStart << "," << srcEnd << ")" << endl;

        pair< vector< pair<llint, llint> >, vector< pair<llint, llint> > > res;
        if (rangeStart >= srcStart && rangeStart < srcEnd) {
            if (rangeEnd <= srcEnd) {
                // range entirely contained
                // cout << "  range cont" << endl;
                res.first.push_back({
                    convert(rangeStart),
                    rangeLen
                });

                // cout << res.first.at(0).first << "," << res.first.at(0).second << endl;
            } else {
                // first half of range contained
                // cout << "  range first part" << endl;
                llint amtContained = srcEnd - rangeStart;
                res.first.push_back({
                    convert(rangeStart),
                    amtContained
                });
                res.second.push_back({
                    srcEnd,
                    rangeLen - amtContained
                });

                // cout << res.first.at(0).first << "," << res.first.at(0).second << endl;
                // cout << res.second.at(0).first << "," << res.second.at(0).second << endl;
            }
        } else if (rangeEnd > srcStart && rangeEnd <= srcEnd) {
            // second half of range contained
            // cout << "  range second part" << endl;
            llint amtContained = rangeEnd - srcStart;
            res.first.push_back({
                destStart,
                amtContained
            });
            res.second.push_back({
                rangeStart,
                rangeLen - amtContained
            });

            // cout << res.first.at(0).first << "," << res.first.at(0).second << endl;
            // cout << res.second.at(0).first << "," << res.second.at(0).second << endl;
        } else if (rangeStart < srcStart && rangeEnd > srcEnd) {
            // internal part of range contained
            // cout << "  range internal" << endl;
            res.first.push_back({
                destStart,
                length
            });
            res.second.push_back({
                rangeStart,
                srcStart - rangeStart
            });
            res.second.push_back({
                srcEnd,
                rangeEnd - srcEnd
            });

            // cout << res.first.at(0).first << "," << res.first.at(0).second << endl;
            // cout << res.second.at(0).first << "," << res.second.at(0).second << endl;
            // cout << res.second.at(1).first << "," << res.second.at(1).second << endl;
        } else {
            // none of range contained
            // cout << "  range separate" << endl;
            res.second.push_back(range);
            // cout << res.second.at(0).first << "," << res.second.at(0).second << endl;
        }

        return res;
    }
};

struct SeedRange {
    vector< pair<llint, llint> > ranges;

    SeedRange(llint start, llint len) {
        ranges.push_back({start, len});
    }

    void convert(vector<MapTriplet> map) {
        vector< pair<llint, llint> > nextRanges;

        for (pair<llint, llint> range : ranges) {
            vector< pair<llint, llint> > toConv;
            toConv.push_back(range);

            for (MapTriplet mt : map) {
                vector< pair<llint, llint> > nextConv;
                for (pair<llint, llint> convRange : toConv) {
                    auto convertRes = mt.convertRange(convRange);
                    for (auto converted : convertRes.first) {
                        nextRanges.push_back(converted);
                    }
                    for (auto unconverted : convertRes.second) {
                        nextConv.push_back(unconverted);
                    }
                }
                toConv = nextConv;
            }

            for (pair<llint, llint> unConv : toConv) {
                nextRanges.push_back(unConv);
            }
        }

        ranges = nextRanges;
    }

    llint getMinSeed() {
        llint min = ranges.at(0).first;
        for (uint i = 1; i < ranges.size(); i++) {
            if (ranges.at(i).first < min) {
                min = ranges.at(i).first;
            }
        }
        return min;
    }
};

// gets all the seeds
vector<llint> getSeeds(const vector<string> &input) {
    vector<string> lineOne = split(input.at(0), " ");
    vector<llint> res;

    for (uint i = 1; i < lineOne.size(); i++) {
        res.push_back(stoll(lineOne.at(i)));
    }

    return res;
}

// returns a vector of conversion maps, each being a vector of MapTriplets
vector< vector<MapTriplet> > getMaps(const vector<string> &input) {
    vector< vector<MapTriplet> > res;
    res.push_back({});

    for (uint i = 3; i < input.size(); i++) {
        string line = input.at(i);

        if (line.size() == 0) {
            res.push_back({});
            i++;
            continue;
        }

        vector<string> lineSplit = split(line, " ");
        MapTriplet mt(
            stoll(lineSplit.at(0)),
            stoll(lineSplit.at(1)),
            stoll(lineSplit.at(2))
        );
        res.at(res.size() - 1).push_back(mt);
    }

    return res;
}

llint partOne(const vector<string> &input) {
    vector<llint> seeds = getSeeds(input);
    vector< vector<MapTriplet> > maps = getMaps(input);
    vector<llint> convSeeds;

    for (llint seed : seeds) {
        llint curr = seed;

        for (vector<MapTriplet> map : maps) {
            for (MapTriplet conv : map) {
                if (conv.doesConvert(curr)) {
                    curr = conv.convert(curr);
                    break;
                }
            }
        }

        convSeeds.push_back(curr);
    }

    llint minSeed = convSeeds.at(0);
    for (uint i = 1; i < convSeeds.size(); i++) {
        if (convSeeds.at(i) < minSeed) {
            minSeed = convSeeds.at(i);
        }
    }

    return minSeed;
}

llint partTwo(const vector<string> &input) {
    vector<llint> seeds = getSeeds(input);
    vector< vector<MapTriplet> > maps = getMaps(input);

    vector<SeedRange> ranges;
    for (uint i = 0; i < seeds.size(); i += 2) {
        ranges.push_back({seeds.at(i), seeds.at(i+1)});
    }

    for (vector<MapTriplet> map : maps) {
        for (uint i = 0; i < ranges.size(); i++) {
           ranges.at(i).convert(map);
        }
    }

    llint min = ranges.at(0).getMinSeed();
    for (uint i = 1; i < ranges.size(); i++) {
        llint currMin = ranges.at(i).getMinSeed();
        if (currMin < min) {
            min = currMin;
        }
    }

	return min;
}
