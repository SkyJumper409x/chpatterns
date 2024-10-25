#pragma once
std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::string> slice(const std::vector<string>  arr, const int start, const int end);
string substring(const string s, const int start, const int end);
string join(const std::vector<string> vec);
string u8ToString(u8 n);
int parseU8(const string s);
bool containsKey(std::map<string, string> map, string key);
