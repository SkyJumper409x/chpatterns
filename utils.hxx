#pragma once
# include "types.h"

string str(string s);
string str(int n);
string str(char c);
vector<string> split(const string &s, char delim);
vector<string> slice(const vector<string>  arr, const int start, const int end);
string substring(const string s, const int start, const int end);
string join(const vector<string> vec, const string &sep = ",");
string u8ToString(u8 n);
int parseU8(const string s);
bool containsKey(map<string, string> map, string key);
void logStrings(vector<string> arg, const string &sep = ",", const string &pfx = "[", const string &sfx = "]");
