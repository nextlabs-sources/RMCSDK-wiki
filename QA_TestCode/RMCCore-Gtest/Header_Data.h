#pragma once

string Trim(string& str);
vector<vector<string>> readCSV(string CSVfile);

void splitString(const string& sourceString, vector<string>& result_string, const string& split);

int string2Int(string s);

string& replace_all(string& str, const string& old_value, const string& new_value);

int standard_to_stamp(const char *str_time);

Json::Value ParseJsonFile(std::string filename);

void ExportToFile(std::string filename, std::string content);
std::vector<std::string> splitWithStl(const std::string &str, const std::string &pattern);
int const_str_int(std::string str);