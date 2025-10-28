#include <sstream>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "types.h"
#include "LogFlags.hxx"

string str(string s) {
    return s;
}
string str(int n) {
    return std::to_string(n);
}
string str(char c) {
    string thing = "";
    thing += c;
    return thing;
}
/* https://stackoverflow.com/a/236803 */
template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        result = item;
    }
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems{};
    split(s, delim, std::back_inserter(elems));
    return elems;
}



vector<string> slice(const vector<string> arr, const int startInclusive, const int endExclusive) {

    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "-- begin slice debug logging --" << std::endl;
    }
    if(startInclusive == endExclusive && CHPatterns_LOG_LEVEL >= CHPatternsLogLevelWarn) {
        std::cout << "WARN: startInclusive is equal to endExclusive" << std::endl;
    }
    if(CHPatterns_LOG_LEVEL > CHPatternsLogLevelDebug) {
        std::cout << "startInclusive: " << startInclusive << ", endExclusive: " << endExclusive << std::endl;
    }


    int arrSize = arr.size();
    int actualEndExclusive = std::min(arrSize, endExclusive);
    int resultSize = actualEndExclusive - startInclusive;
    vector<string> result;
    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "arr size: " << arrSize << std::endl;
        std::cout << "actualEndExclusive: " << actualEndExclusive << std::endl;
        std::cout << "result size: " << resultSize << std::endl;
        std::cout << "intial result capacity: " << result.capacity() << std::endl;
    }
    result.reserve(resultSize);
    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "reserved storage for " << resultSize << " elements" << std::endl;
    }
    int newResultCapacity = result.capacity();
    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "new result capacity: " << newResultCapacity << std::endl;
    };
    if(newResultCapacity < resultSize) {
        if(CHPatterns_LOG_LEVEL > CHPatternsLogLevelNone) {
            std::cout << "newResultCapacity < resultSize, shouldn't happen" << std::endl;
        }
        std::exit(1);
    }
    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "entering loop" << std::endl;
    }
    for(int i = startInclusive, j = 0; i < actualEndExclusive; i++, j++) {
        if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
            std::cout << "iteration #" << j << ": " << std::endl;
            std::cout << "assigning result[" << j << "] value \"" << arr[i] << "\" at arr[" << i << "]" << std::endl;
        }
        result.push_back(arr[i]);
        if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
            std::cout << "assigned value to result[" << j << "]" << std::endl;
        }
    }
    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "finished loop, returning result with size " << (result.size()) << std::endl;
        std::cout << "result[0]: " << result[0] << std::endl;
        std::cout << "-- end slice debug logging --" << std::endl;
    }
    return result;
}

string substring(const string s, const int start, const int end) {
    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "substringing from " << start << " to " << end << " for string \"" << s << "\"" << std::endl;
    }
    string result = "";
    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "created result" << std::endl;
    }
    int slength = s.length();
    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "slength: " << slength << std::endl;
    }
    int loopcount = std::min<int>(slength,end);
    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "loopcount: " << loopcount << std::endl;
        int x = start;
        std::cout << "set x" << std::endl;
        if(x < loopcount) {
            std::cout << "x lt loopcount" << std::endl;
        } else if(x > loopcount) {
            std::cout << "x gt loopcount" << std::endl;
        } else if(x == loopcount) {
            std::cout << "x eq loopcount" << std::endl;
        } else {
            std::cout << "x neq loopcount (wait, how did we reach this code?)" << std::endl;
        }
        std::cout << "compared x and loopcount" << std::endl;
        x++;
        std::cout << "x++'d" << std::endl;
    }
    const string* pointyS = &s;
    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "set pointyS" << std::endl;
        std::cout << "entering loop" << std::endl;
    }
    for(int i = start; i < loopcount; i++) {
        if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
            std::cout << "in loop" << std::endl;
            std::cout << "setting nextChar to index" << i << std::endl;
        }
        const string nextChar = pointyS[i];
        if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
            std::cout << "set nextChar" << std::endl;
        }
        result = result + nextChar;
        if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
            std::cout << "added nextChar '" << nextChar << "' to result" << std::endl;
        }
    }
    return result;
}

// string join(const string arr[]*) {
//     string result = "";
//     for(uint i = 0; i < arr->length; i++) {
//         result += arr[i];
//     }
//     return result;
// }
string join(const vector<string> vec, const string &sep = ",") {
    string result = "";
    for(int i = 0; i < vec.size(); i++) {
        result += vec[i];
        if((i+1) < vec.size()) {
            result += sep;
        }
    }
    return result;
}
string u8ToString(u8 n) {
    // int iN = ((int)n);
    // return ("" + iN);
    return std::to_string((unsigned)n);
}
u8 parseU8(const string s) {
    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "-- begin parseU8 debug logging --" << std::endl;
        std::cout << "s: \"" << s << "\"" << std::endl;
    }
    u8 result = 0;
    if(s.length() > 3) {
        result = 255;
    } else if(s.length() > 0) {
        for(u8 i = 0; i < s.length(); i++) {
            const char* digitPtr = &s[i];
            char digit = (*digitPtr);
            u8 parsed = ((u8)digit)-48;
            u8 zerocount = s.length() - i - 1;
            u8 powedZerocount = pow(10, zerocount);
            u8 thingyToAdd = ((parsed * powedZerocount));
            if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
                string stringedParsed = u8ToString(parsed);
                string stringedThingyToAdd = u8ToString(thingyToAdd);
                int iThingyToAdd = ((int)(((int)parsed) * powedZerocount));
                string stringedIThingyToAdd = u8ToString(iThingyToAdd);
                std::cout << "digit #" << ((int)i) << ": " << digit << std::endl;
                std::cout << "  as u8: " << stringedParsed << std::endl;
                std::cout << "  zerocount: " << str(zerocount) << std::endl;
                std::cout << "  powedZerocount: " << str(powedZerocount) << std::endl;
                std::cout << "  thingyToAdd: " << stringedThingyToAdd << std::endl;
                std::cout << "  iThingyToAdd: " << stringedIThingyToAdd << std::endl;
            }
            string stringedPreviousResult = u8ToString(result);
            result += thingyToAdd;
            string stringedResult = u8ToString(result);
            if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
                std::cout << "  updated result from \"" << stringedPreviousResult << "\" to \"" << stringedResult << "\"" << std::endl;
            }
        }
    }
    if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelDebug) {
        std::cout << "-- end parseU8 debug logging --" << std::endl;
    }
    return result;
}
bool containsKey(map<string, string> map, string key) {
    if (map.count(key) > 0) {
        return true;
    }
    return false;
}
void logStrings(vector<string> arg, const string &sep = ",", const string &pfx = "[", const string &sfx = "]") {
    std::cout << pfx << join(arg, sep) << sfx << std::endl;
}
