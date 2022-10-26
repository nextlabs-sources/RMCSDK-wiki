#include "stdafx.h"

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

const std::string ReadFromFile(std::string filename)
{
	std::string retstr;

	FILE * testfile;
	fopen_s(&testfile, filename.c_str(), "r");
	if (testfile == NULL) {
		return retstr;
	}

	char buffer[257];
	//memset(buffer, 0, sizeof(buffer));
	while (!feof(testfile)) {
		memset(buffer, 0, sizeof(buffer));
		fread(buffer, sizeof(char), sizeof(buffer) - 1, testfile);
		retstr += buffer;
	}
	fclose(testfile);

	return retstr;
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


//Converting a WChar string to a Ansi string  
std::string WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

	if (nLen <= 0) return std::string("");

	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;

	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;
}

string ws2s(const wstring& inputws) { return WChar2Ansi(inputws.c_str()); }

//Converting a Ansi string to WChar string  

std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen)

{
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if (nSize <= 0) return NULL;

	WCHAR *pwszDst = new WCHAR[nSize + 1];
	if (NULL == pwszDst) return NULL;

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;

	if (pwszDst[0] == 0xFEFF) // skip Oxfeff  
		for (int i = 0; i < nSize; i++)
			pwszDst[i] = pwszDst[i + 1];

	wstring wcharString(pwszDst);
	delete pwszDst;

	return wcharString;
}

std::wstring s2ws(const string& s)
{ 
	if (s == "")
		cout << "The input string is Empty, please check!!!" << endl;
	return Ansi2WChar(s.c_str(), s.size()); 
}

char *int_to_hex(unsigned long data)
{
	int bit_num = sizeof(unsigned long) * 8;
	char *p_hex = new char[sizeof(unsigned long) * 8 / 4 + 3];
	p_hex[0] = '0';
	p_hex[1] = 'x';
	p_hex[bit_num / 4 + 2] = '\0';
	char *p_tmp = p_hex + 2;
	for (unsigned int i = 0; i < bit_num / 4; ++i)
	{
		p_tmp[i] = data << (4 * i) >> (bit_num - 4);
		if (p_tmp[i] >= 0 && p_tmp[i] <= 9)
			p_tmp[i] += '0';
		else if (p_tmp[i] >= 10 && p_tmp[i] <= 15)
			p_tmp[i] = p_tmp[i] - 10 + 'A';
	}
	return p_hex;
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

uint64_t stringToUINT64(const std::string s)
{
	std::stringstream a;
	a << s;
	UINT64 ret = 0;
	a >> ret;
	return ret;
}

void replaceA2B(string &S, string A, string B)
{
	std::size_t found = S.find(A);
	while (std::string::npos != found) {
		S.replace(found, A.length(), B);
		found = S.find(A, found + 1);
	}
}

//MD5
/* forward declaration */
static void Transform();
static void TransformMD5(UINT4 *buf, UINT4 *in);

static unsigned char PADDING[64] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* F, G and H are basic MD5 functions: selection, majority, parity */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z))) 

/* ROTATE_LEFT rotates x left n bits */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4 */
/* Rotation is separate from addition to prevent recomputation */
#define FF(a, b, c, d, x, s, ac) \
  {(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) \
	  {(a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
	  }
#define HH(a, b, c, d, x, s, ac) \
	  {(a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
	  }
#define II(a, b, c, d, x, s, ac) \
	  {(a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
	  }

void MD5Init(MD5_CTX *mdContext)
{
	mdContext->i[0] = mdContext->i[1] = (UINT4)0;

	/* Load magic initialization constants.
	*/
	mdContext->buf[0] = (UINT4)0x67452301;
	mdContext->buf[1] = (UINT4)0xefcdab89;
	mdContext->buf[2] = (UINT4)0x98badcfe;
	mdContext->buf[3] = (UINT4)0x10325476;
}

void MD5Update(MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen)
{
	UINT4 in[16];
	int mdi;
	unsigned int i, ii;

	/* compute number of bytes mod 64 */
	mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	/* update number of bits */
	if ((mdContext->i[0] + ((UINT4)inLen << 3)) < mdContext->i[0])
		mdContext->i[1]++;
	mdContext->i[0] += ((UINT4)inLen << 3);
	mdContext->i[1] += ((UINT4)inLen >> 29);

	while (inLen--) {
		/* add new character to buffer, increment mdi */
		mdContext->in[mdi++] = *inBuf++;

		/* transform if necessary */
		if (mdi == 0x40) {
			for (i = 0, ii = 0; i < 16; i++, ii += 4)
				in[i] = (((UINT4)mdContext->in[ii + 3]) << 24) |
				(((UINT4)mdContext->in[ii + 2]) << 16) |
				(((UINT4)mdContext->in[ii + 1]) << 8) |
				((UINT4)mdContext->in[ii]);
			TransformMD5(mdContext->buf, in);
			mdi = 0;
		}
	}
}

void MD5Final(MD5_CTX *mdContext)

{
	UINT4 in[16];
	int mdi;
	unsigned int i, ii;
	unsigned int padLen;

	/* save number of bits */
	in[14] = mdContext->i[0];
	in[15] = mdContext->i[1];

	/* compute number of bytes mod 64 */
	mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	/* pad out to 56 mod 64 */
	padLen = (mdi < 56) ? (56 - mdi) : (120 - mdi);
	MD5Update(mdContext, PADDING, padLen);

	/* append length in bits and transform */
	for (i = 0, ii = 0; i < 14; i++, ii += 4)
		in[i] = (((UINT4)mdContext->in[ii + 3]) << 24) |
		(((UINT4)mdContext->in[ii + 2]) << 16) |
		(((UINT4)mdContext->in[ii + 1]) << 8) |
		((UINT4)mdContext->in[ii]);
	TransformMD5(mdContext->buf, in);

	/* store buffer in digest */
	for (i = 0, ii = 0; i < 4; i++, ii += 4) {
		mdContext->digest[ii] = (unsigned char)(mdContext->buf[i] & 0xFF);
		mdContext->digest[ii + 1] =
			(unsigned char)((mdContext->buf[i] >> 8) & 0xFF);
		mdContext->digest[ii + 2] =
			(unsigned char)((mdContext->buf[i] >> 16) & 0xFF);
		mdContext->digest[ii + 3] =
			(unsigned char)((mdContext->buf[i] >> 24) & 0xFF);
	}
}

/* Basic MD5 step. Transform buf based on in.
*/
static void TransformMD5(UINT4 *buf, UINT4 *in)
{
	UINT4 a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
	FF(a, b, c, d, in[0], S11, 3614090360); /* 1 */
	FF(d, a, b, c, in[1], S12, 3905402710); /* 2 */
	FF(c, d, a, b, in[2], S13, 606105819); /* 3 */
	FF(b, c, d, a, in[3], S14, 3250441966); /* 4 */
	FF(a, b, c, d, in[4], S11, 4118548399); /* 5 */
	FF(d, a, b, c, in[5], S12, 1200080426); /* 6 */
	FF(c, d, a, b, in[6], S13, 2821735955); /* 7 */
	FF(b, c, d, a, in[7], S14, 4249261313); /* 8 */
	FF(a, b, c, d, in[8], S11, 1770035416); /* 9 */
	FF(d, a, b, c, in[9], S12, 2336552879); /* 10 */
	FF(c, d, a, b, in[10], S13, 4294925233); /* 11 */
	FF(b, c, d, a, in[11], S14, 2304563134); /* 12 */
	FF(a, b, c, d, in[12], S11, 1804603682); /* 13 */
	FF(d, a, b, c, in[13], S12, 4254626195); /* 14 */
	FF(c, d, a, b, in[14], S13, 2792965006); /* 15 */
	FF(b, c, d, a, in[15], S14, 1236535329); /* 16 */

											 /* Round 2 */
#define S21 5
#define S22 9
#define S23 14
#define S24 20
	GG(a, b, c, d, in[1], S21, 4129170786); /* 17 */
	GG(d, a, b, c, in[6], S22, 3225465664); /* 18 */
	GG(c, d, a, b, in[11], S23, 643717713); /* 19 */
	GG(b, c, d, a, in[0], S24, 3921069994); /* 20 */
	GG(a, b, c, d, in[5], S21, 3593408605); /* 21 */
	GG(d, a, b, c, in[10], S22, 38016083); /* 22 */
	GG(c, d, a, b, in[15], S23, 3634488961); /* 23 */
	GG(b, c, d, a, in[4], S24, 3889429448); /* 24 */
	GG(a, b, c, d, in[9], S21, 568446438); /* 25 */
	GG(d, a, b, c, in[14], S22, 3275163606); /* 26 */
	GG(c, d, a, b, in[3], S23, 4107603335); /* 27 */
	GG(b, c, d, a, in[8], S24, 1163531501); /* 28 */
	GG(a, b, c, d, in[13], S21, 2850285829); /* 29 */
	GG(d, a, b, c, in[2], S22, 4243563512); /* 30 */
	GG(c, d, a, b, in[7], S23, 1735328473); /* 31 */
	GG(b, c, d, a, in[12], S24, 2368359562); /* 32 */

											 /* Round 3 */
#define S31 4
#define S32 11
#define S33 16
#define S34 23
	HH(a, b, c, d, in[5], S31, 4294588738); /* 33 */
	HH(d, a, b, c, in[8], S32, 2272392833); /* 34 */
	HH(c, d, a, b, in[11], S33, 1839030562); /* 35 */
	HH(b, c, d, a, in[14], S34, 4259657740); /* 36 */
	HH(a, b, c, d, in[1], S31, 2763975236); /* 37 */
	HH(d, a, b, c, in[4], S32, 1272893353); /* 38 */
	HH(c, d, a, b, in[7], S33, 4139469664); /* 39 */
	HH(b, c, d, a, in[10], S34, 3200236656); /* 40 */
	HH(a, b, c, d, in[13], S31, 681279174); /* 41 */
	HH(d, a, b, c, in[0], S32, 3936430074); /* 42 */
	HH(c, d, a, b, in[3], S33, 3572445317); /* 43 */
	HH(b, c, d, a, in[6], S34, 76029189); /* 44 */
	HH(a, b, c, d, in[9], S31, 3654602809); /* 45 */
	HH(d, a, b, c, in[12], S32, 3873151461); /* 46 */
	HH(c, d, a, b, in[15], S33, 530742520); /* 47 */
	HH(b, c, d, a, in[2], S34, 3299628645); /* 48 */

											/* Round 4 */
#define S41 6
#define S42 10
#define S43 15
#define S44 21
	II(a, b, c, d, in[0], S41, 4096336452); /* 49 */
	II(d, a, b, c, in[7], S42, 1126891415); /* 50 */
	II(c, d, a, b, in[14], S43, 2878612391); /* 51 */
	II(b, c, d, a, in[5], S44, 4237533241); /* 52 */
	II(a, b, c, d, in[12], S41, 1700485571); /* 53 */
	II(d, a, b, c, in[3], S42, 2399980690); /* 54 */
	II(c, d, a, b, in[10], S43, 4293915773); /* 55 */
	II(b, c, d, a, in[1], S44, 2240044497); /* 56 */
	II(a, b, c, d, in[8], S41, 1873313359); /* 57 */
	II(d, a, b, c, in[15], S42, 4264355552); /* 58 */
	II(c, d, a, b, in[6], S43, 2734768916); /* 59 */
	II(b, c, d, a, in[13], S44, 1309151649); /* 60 */
	II(a, b, c, d, in[4], S41, 4149444226); /* 61 */
	II(d, a, b, c, in[11], S42, 3174756917); /* 62 */
	II(c, d, a, b, in[2], S43, 718787259); /* 63 */
	II(b, c, d, a, in[9], S44, 3951481745); /* 64 */

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

/*** End of md5 **/

void md5(string& str)
{
	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];

	MD5Init(&mdContext);
	MD5Update(&mdContext, (unsigned char*)const_cast<char*>(str.c_str()), str.size());
	MD5Final(&mdContext);

	string md5;
	char buf[3];
	for (int i = 0; i < 16; i++)
	{
		sprintf(buf, "%02x", mdContext.digest[i]);
		md5.append(buf);
	}
	str= md5;
}
string md5_s(string str)
{
	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];

	MD5Init(&mdContext);
	MD5Update(&mdContext, (unsigned char*)const_cast<char*>(str.c_str()), str.size());
	MD5Final(&mdContext);

	string md5;
	char buf[3];
	for (int i = 0; i < 16; i++)
	{
		sprintf(buf, "%02x", mdContext.digest[i]);
		md5.append(buf);
	}
	return md5;
}

//change tags string from: ITAR|TAA-01|TAA-02;BAFA|01|02
//Into this format: {"ITAR":["TAA-01","TAA-02"],"BAFA":["01","02"]}
//This is for read tag string from CSV file. 
string patchTagString(string & tags_csv)
{
	//for tag
	//std::string tags = "{\"itar\":[\"TAA-02\"]}";
	string tags = "";
	vector<string> tag_n;
	vector<string> tdetail;
	splitString(tags_csv, tag_n, ";");
	int i = 0;
	int j = 0;
	for each(string tn in tag_n)
	{
		if (i == 0)
		{
			tags = "{";
		}
		else
		{
			tags = tags + ",";
		}
		tdetail.clear();
		splitString(tn, tdetail, "|");
		j = 0;
		for each(string t in tdetail)
		{
			if (j == 0)
			{
				tags = tags + "\"" + t + "\":[";
			}
			else if (j == 1)
			{
				tags = tags + "\"" + t + "\"";
			}
			else
			{
				tags = tags + ",\"" + t + "\"";
			}
			j++;
		}
		tags = tags + "]";

		i++;
	}
	if (tags.size() != 0)
		tags = tags + "}";

	return tags;
}

string getFileNameFromPath(string filePath)
{
	if (filePath.empty())
	{
		return "";
	}
	replaceA2B(filePath, "/", "\\");
	int pos = filePath.find_last_of('\\');
	string s(filePath.substr(pos + 1));
	return s;
}

std::string getPathFromFullPath(std::string strFullName)
{
	if (strFullName.empty())
	{
		return "";
	}
	replaceA2B(strFullName, "/", "\\");

	int pos = strFullName.find_last_of('\\');
	string s(strFullName.substr(pos + 1));

	return strFullName.substr(0, strFullName.length() - s.length());
}

bool getParentdir(string testPath, string &parentDir)
{
	if (testPath.empty())
	{
		return 0;
	}
	replaceA2B(testPath, "/", "\\");

	int pos = testPath.find_last_of('\\');
	if(testPath.size()==pos+1|| -1==pos)
	{
		return 0;
	}
	parentDir= testPath.substr(0, pos);
	return 1;
}

void StringToLower(std::string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

//output a file which inlcuding code for Windows API CreateFile, with differernt para from CSV
void writecode4CreateFile()
{
	string codeFile = "C:\\SDK test data\\autoData\\CreateFile.txt";
	for each(vector<string> csv in csv_WinAPI)
	{
		if (csv[0] == "1")
		{
			replaceA2B(csv[2], "\\", "/");
			replaceA2B(csv[2], "/", "\\\\");
			string code = "cout << endl << \"--------CreateFile - " + csv[1] + " ----------\" << endl;\nhFile = CreateFile(L\""
				+ csv[2] + "\", " + csv[3] + +", " + csv[4] + ", " + csv[5] + ", " + csv[6] + ", " + csv[7] + ", " + csv[8] + ");\nif (hFile == INVALID_HANDLE_VALUE){	cout << \"CreateFile have error.\" << endl;}\nelse { cout << \"CreateFile -ok.\" << endl;}\nCloseHandle(hFile);";
			
			ofstream SaveFile(codeFile, ios::app);
			SaveFile << code;
			SaveFile.close();
		}
	}
}

std::string UTF8_To_string(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}
