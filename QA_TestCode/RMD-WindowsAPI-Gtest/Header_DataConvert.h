#pragma once

string Trim(string& str);
vector<vector<string>> readCSV(string CSVfile);

const std::string ReadFromFile(std::string filename);
Json::Value ParseJsonFile(std::string filename);


std::string WChar2Ansi(LPCWSTR pwszSrc);
string ws2s(const wstring& inputws);
std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);
std::wstring s2ws(const string& s);

char *int_to_hex(unsigned long data);

void splitString(const string& sourceString, vector<string>& result_string, const string& split);
int string2Int(string s);
uint64_t stringToUINT64(const std::string s);
void replaceA2B(string &S, string A, string B);

//MD5 related:
/* typedef a 32 bit type */
typedef unsigned long int UINT4;

/* Data structure for MD5 (Message Digest) computation */
typedef struct {
	UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
	UINT4 buf[4];                                    /* scratch buffer */
	unsigned char in[64];                              /* input buffer */
	unsigned char digest[16];     /* actual digest after MD5Final call */
} MD5_CTX;

void MD5Init(MD5_CTX *mdContext);
void MD5Update(MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen);
void MD5Final(MD5_CTX *mdContext);

void md5(string& str);
string md5_s(string str);
//End - MD5 -


string patchTagString(string & tags_csv);

string getFileNameFromPath(string filePath);
std::string getPathFromFullPath(std::string strFullName);
bool getParentdir(string testPath, string &parentDir);
void StringToLower(std::string &str);
void writecode4CreateFile();

std::string UTF8_To_string(const std::string & str);