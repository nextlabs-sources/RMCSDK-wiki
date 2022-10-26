#include "stdafx.h"
#include "rmccore\restful\rmuser.h"


string Trim(string& str)
{
	str.erase(0, str.find_first_not_of(" \t\r\n"));
	str.erase(str.find_last_not_of(" \t\r\n") + 1);
	return str;
}

vector<vector<string>> readCSV(string CSVfile)
{
	vector<vector<string>> csv;
	ifstream fin(CSVfile);
	string line;
	int i = 0;
	while (getline(fin, line)) {
		//cout << line << endl;
		if (i != 0) {
			istringstream sin(line);
			vector<string> fields;
			string field;
			while (getline(sin, field, ',')) {
				fields.push_back(field);
			}
			csv.push_back(fields);
			//cout << csv[i][0] << "\t" << csv[i][1] << endl;
		}
		i++;
	}
	return csv;
}

void splitString(const string& sourceString, vector<string>& result_string, const string& split)
{
	string::size_type pos1, pos2;
	pos2 = sourceString.find(split);
	pos1 = 0;
	while (string::npos != pos2)
	{
		result_string.push_back(sourceString.substr(pos1, pos2 - pos1));

		pos1 = pos2 + split.size();
		pos2 = sourceString.find(split, pos1);
	}
	if (pos1 != sourceString.length())
		result_string.push_back(sourceString.substr(pos1));
}

int string2Int(string s)
{
	char* end;
	int i = static_cast<int>(strtol(s.c_str(), &end, 10));
	return i;
}

string& replace_all(string& str, const string& old_value, const string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}


int standard_to_stamp(const char *str_time)
{
	struct tm stm;
	int iY, iM, iD, iH, iMin, iS;

	memset(&stm, 0, sizeof(stm));
	iY = atoi(str_time);
	iM = atoi(str_time + 5);
	iD = atoi(str_time + 8);
	iH = atoi(str_time + 11);
	iMin = atoi(str_time + 14);
	iS = atoi(str_time + 17);

	stm.tm_year = iY - 1900;
	stm.tm_mon = iM - 1;
	stm.tm_mday = iD;
	stm.tm_hour = iH;
	stm.tm_min = iMin;
	stm.tm_sec = iS;

	printf("%d-%0d-%0d %0d:%0d:%0d\n", iY, iM, iD, iH, iMin, iS);
	return (int)mktime(&stm);
}


// Using 3rd Jsoncpp to parse json file
Json::Value ParseJsonFile(std::string filename)
{
	Json::Reader reader;
	Json::Value value;
	//Json::Value::iterator iter;
	//Json::Value::Members members;
	std::ifstream ifs;
	ifs.open(filename, std::ios::binary);
	if (reader.parse(ifs, value)) {
		/*members = value.getMemberNames();
		for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it) {
		string name = *it;
		string value_str = value[name].asString();
		}*/
		return value;
	}
}

//Export file
void ExportToFile(string filename, string content) {
	string exportfilename = "ex_";
	exportfilename += filename;
	ofstream SaveFile(default_generate_file_path + exportfilename);
	SaveFile << content;
	SaveFile.close();
}

std::vector<std::string> splitWithStl(const std::string &str, const std::string &pattern)
{
	std::vector<std::string> resVec;

	if ("" == str)
	{
		return resVec;
	}
	std::string strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != std::string::npos)
	{
		std::string x = strs.substr(0, pos);
		resVec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return resVec;
}

int const_str_int(string str) {
	string a = str;
	int res;
	stringstream ss;
	ss << a;
	ss >> res;
	return res;
}
