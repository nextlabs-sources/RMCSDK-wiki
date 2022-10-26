#include "stdafx.h"
#include "SDLInc.h"
#include <direct.h>  
#include <io.h>  
#include <tlhelp32.h>

#define BUILTIN_RIGHT_VIEW                      0x0000000000000001ULL
#define BUILTIN_RIGHT_EDIT                      0x0000000000000002ULL
#define BUILTIN_RIGHT_PRINT                     0x0000000000000004ULL
#define BUILTIN_RIGHT_CLIPBOARD                 0x0000000000000008ULL
#define BUILTIN_RIGHT_SAVEAS                    0x0000000000000010ULL
#define BUILTIN_RIGHT_DECRYPT                   0x0000000000000020ULL
#define BUILTIN_RIGHT_SCREENCAP                 0x0000000000000040ULL
#define BUILTIN_RIGHT_SEND                      0x0000000000000080ULL
#define BUILTIN_RIGHT_CLASSIFY                  0x0000000000000100ULL
#define BUILTIN_RIGHT_SHARE                     0x0000000000000200ULL
#define BUILTIN_RIGHT_DOWNLOAD                  0x0000000000000400ULL
#define BUILTIN_OBRIGHT_WATERMARK               0x0000000040000000ULL

void checkSDWLResult(SDWLResult res)
{
	//EXPECT_EQ(0, res.GetCode());  //Write assertion outside, for print out the specific error location.

	if (res.GetCode() != 0)
	{
		if (res.GetCode() >= 0xF000)
		{
			cout << "- Failed with RMS code: [" << res.GetCode() - 0xF000 << "] - " << res.GetMsg() << "." << endl;
			//check the error message same as the one in json file.
			//		// using 3rd party Jsoncpp to read and get json value
			//		Json::Value jroot;
			//		jroot = ParseJsonFile(jsonFile);
			//		string status, message;
			//
			//		//Read root node: status and message
			//		status = jroot["statusCode"].asString();
			//		message = jroot["message"].asString();
			//
			//		// using atoi function to transfer string to const char* 
			//		EXPECT_EQ(atoi(status.c_str()), ret.GetCode() - 0xF000);
			//		EXPECT_STREQ(message.c_str(), ret.GetMessage());
			//
		}
		else
		{
			cout << "- Failed. (" << res.GetCode()<< ": "<< res.GetMsg() <<")"<<endl;
		}
		//cout << "Error details:" << endl;
		//cout << "   GetFunction(): " << res.GetFunction() << endl;
		//cout << "   GetCode(): " << res.GetCode() << endl;
		//cout << "   GetMsg(): " << res.GetMsg() << endl;
		//cout << "   GetFile(): " << res.GetFile() << endl;
		//cout << "   GetLine(): " << res.GetLine() << endl;
	}
	else
	{
		cout << " - OK." << endl;
	}
}

void checkResult(SDWLResult res, int expected)
{
	EXPECT_EQ(expected, res.GetCode());

	if (res.GetCode() != 0)
	{
		if (res.GetCode() >= 0xF000)
		{
			LOG(INFO) << "- Failed with RMS code: [" << res.GetCode() - 0xF000 << "] - " << res.GetMsg() << "." << endl;
		}
		else
		{
			LOG(INFO) << "- Failed. (" << res.GetCode() << ": " << res.GetMsg() << ")" << endl;
		}
	}
	else
	{
		LOG(INFO) << " - OK." << endl;
	}
}

bool checkFileExist(string filefolder, string filename)
{
	string filePath = filefolder + "\\" + filename;
	FILE* file = fopen(filePath.c_str(), "rb");
	if (file)
	{
		int size = filelength(fileno(file));
		fclose(file);

		if (size == 0)
		{
			cout << filePath << " - size is 0" << endl;;
			return false;
		}
		else
		{
			cout << filePath << " - is existing." << " File size: " << size << endl;
			return true;
		}
	}
	else
	{
		cout << filePath << " - Not exist" << endl;;
		return false;
	}
}

//bool checkFileExist(string filefolder, string filename)
//{
//	string filePath = filefolder + "\\" + filename;
//
//	ifstream fin(filePath);
//
//	if (!fin)
//	{
//		cout << filePath << " - Not exist" << endl;
//		return false;
//	}
//
//	fin.seekg(0, ios::end);
//	streampos ps = fin.tellg();
//	cout << filePath << " - is saved.";
//	cout << " File size: " << ps << endl;
//
//	fin.close();
//	return true;
//}

void deleteFile(string filefolder, string filename)
{
	string filePath = filefolder + "\\" + filename;
	//DeleteFile((LPCTSTR)filePath.c_str());
	if (remove(filePath.c_str()) == 0)
	{
		cout << filePath << " - Delete success" << endl;
	}
	else
	{
		cout << filePath << " - Delete failed!!!. [Please check]" << endl;
	}

	//? file can be delete, however, check under the folder, file still exist. Debug mode no this issue

}

void getLastCreatedNXL(string path, string &lastCreatedNXLFile)
{
	//lastCreatedNXLFile: save the lasted created NXL file (Path+Name)

	//long   hFile = 0;
	intptr_t hFile = 0;
	int i = 0;
	__time64_t time;
	string::size_type idx;
	string fn = "";

	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				;
			}
			else
			{
				fn = fileinfo.name;
				idx = fn.find(".nxl");
				if (i == 0 && idx != string::npos)
				{
					lastCreatedNXLFile = p.assign(path).append("\\").append(fileinfo.name);
					time = fileinfo.time_create;
					i++;
				}
				else if (idx != string::npos)
				{
					if (fileinfo.time_create > time)
					{
						lastCreatedNXLFile = p.assign(path).append("\\").append(fileinfo.name);
						time = fileinfo.time_create;
					}
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);

		if (i == 0)
		{
			cout << "No NXL found in this folder, please check if it protected." << endl;
		}
		else
		{
			cout << "NXL file: " << lastCreatedNXLFile << ", created at: " << time << endl;
		}
	}
}

void createFolder(const string& path)
{
	LOG(INFO) << "createFolder:" << path;
	string t = "md \"" + path + "\"";
	system(t.c_str());
}

std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

string readFileContent(string fileDir, string fileName)
{
	string content = "";
	LOG(INFO) << "readFileContent";
	HANDLE hFile = CreateFile(s2ws(fileDir+"\\"+fileName).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		LOG(INFO)<< "OpenFile error.";
	}
	else {
		int file_size = 0;
		file_size = GetFileSize(hFile, NULL);
		char *buff;
		buff = (char*)malloc(file_size);
		DWORD dwRead;
		if (!ReadFile(hFile, buff, file_size, &dwRead, NULL))
		{
			LOG(INFO) << "ReadFile error.";
		}
		buff[file_size] = '\0';
		CloseHandle(hFile);
		content = buff;
	}
	return content;
}

string readFileBeginningChar(string file, int charNum)
{
	try {
		cout << "Read file>>>";
		ifstream infile;
		infile.open(file.data());
		string content = "";

		if (infile.is_open()) {

			infile >> noskipws;
			int i = 0;
			char c;

			while (!infile.eof() && i < charNum)
			{
				i++;
				infile >> c;
				content.push_back(c);
			}
			infile.close();
			cout << content << endl;
		}
		else
		{
			cout << "the test file cannot open, please check." << endl;
			throw "Test file cannot open.";
		}

		return content;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

string getTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
	return tmp;
}

void getAllFiles(string cate_dir, vector<string>& files)
{
	_finddata_t file;
	long long lf;
	if ((lf = _findfirst(cate_dir.c_str(), &file)) == -1) {
		cout << cate_dir << " not found!!!" << endl;
	}
	else {
		while (_findnext(lf, &file) == 0) {
			//cout<<file.name<<endl;  
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			files.push_back(file.name);
		}
	}
	_findclose(lf);
}

bool file_exist(string file_path) {
	fstream _file;
	_file.open(file_path, ios::in);
	if (_file) {
		cout << "File exist!" << endl;
		return true;
	}
	else {
		return false;
	}
}

void edit_file(string file_path, string content) {
	ofstream outfile;
	outfile.open(file_path, ios::in | ios::out | ios::binary);
	if (outfile.is_open()) {
		outfile << content;
		cout << "Edited finished!" << endl;
	}
	else
	{
		cout << "Open file failed." << endl;
	}
}

string read_filecontent(string file_path) {
	int charNum = 35;
	cout << "Read file>>>";
	ifstream infile;
	infile.open(file_path, ios::in);
	string content = "";

	if (infile.is_open()) {

		infile >> noskipws;
		int i = 0;
		char c;

		while (!infile.eof() && i < charNum)
		{
			i++;
			infile >> c;
			if (infile.eof())
				break;
			content.push_back(c);
		}
		infile.close();
		cout << content << endl;
		return content;
	}
	else
	{
		cout << "[ERROR]: the test file cannot open, please check." << endl;
		return content;
	}
}

BOOL FindProcessPid(LPCSTR ProcessName, DWORD& dwPid)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return(FALSE);
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(FALSE);
	}

	BOOL    bRet = FALSE;
	do
	{
		string processname = ws2s(pe32.szExeFile);
		const CHAR * test = processname.data();
		if (!strcmp(ProcessName, test))
		{
			dwPid = pe32.th32ProcessID;
			bRet = TRUE;
			break;
		}

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return bRet;
}

string GetFileShortName(wstring longFilePath) {
	LPCTSTR lpFileName = longFilePath.c_str();
	long     length = 0;
	TCHAR*   buffer = NULL;
	length = GetShortPathName(lpFileName, NULL, 0);
	buffer = new TCHAR[length];

	length = GetShortPathName(lpFileName, buffer, length);
	//_tprintf(TEXT("long name = %s shortname = %s"), lpFileName, buffer);

	int iLen = WideCharToMultiByte(CP_ACP, 0, buffer, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen * sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, buffer, -1, chRtn, iLen, NULL, NULL);
	std::string str(chRtn);

	delete[] buffer;
	return str;
}

void string_replace(std::string &strBig, const std::string &strsrc, const std::string &strdst)
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}

uint64_t getRightsValue(string rights_csv)
{
	uint64_t rights_value = 0x0000000000000000ULL;;
	vector<string> r;
	splitString(rights_csv, r, "|");
	for (vector<string>::size_type i = 0; i != r.size(); ++i)
	{
		cout << r[i];
		string r1;
		transform(r[i].begin(), r[i].end(), back_inserter(r1), ::tolower);
		cout << "(" << r1 << ")" << " ";
		if (r1 == "view")
		{
			rights_value = BUILTIN_RIGHT_VIEW;
		}
		else if (r1 == "edit")
		{
			rights_value = rights_value | BUILTIN_RIGHT_EDIT;
		}
		else if (r1 == "print")
		{
			rights_value = rights_value | BUILTIN_RIGHT_PRINT;
		}
		else if (r1 == "copycontent")
		{
			rights_value = rights_value | BUILTIN_RIGHT_CLIPBOARD;
		}
		else if (r1 == "saveas")
		{
			rights_value = rights_value | BUILTIN_RIGHT_SAVEAS;
		}
		else if (r1 == "decrypt")
		{
			rights_value = rights_value | BUILTIN_RIGHT_DECRYPT;
		}
		else if (r1 == "sc")
		{
			rights_value = rights_value | BUILTIN_RIGHT_SCREENCAP;
		}
		else if (r1 == "send")
		{
			rights_value = rights_value | BUILTIN_RIGHT_SEND;
		}
		else if (r1 == "classify")
		{
			rights_value = rights_value | BUILTIN_RIGHT_CLASSIFY;
		}
		else if (r1 == "share")
		{
			rights_value = rights_value | BUILTIN_RIGHT_SHARE;

		}
		else if (r1 == "download")
		{
			rights_value = rights_value | BUILTIN_RIGHT_DOWNLOAD;
		}
		else if (r1 == "watermark")
		{
			rights_value = rights_value | BUILTIN_OBRIGHT_WATERMARK;
		}
	}
	cout << "rights_value: " << rights_value << endl;

	return rights_value;
}

bool checkRights(int rights, string rights_check) {
	uint64_t rights_value = 0x0000000000000000ULL;;
	vector<string> r;

	std::transform(rights_check.begin(), rights_check.end(), rights_check.begin(), ::tolower);
	if (rights_check == "view")
	{
		rights_value = rights & BUILTIN_RIGHT_VIEW;
		if (rights_value) {
			return true;
		}
	}
	else if (rights_check == "edit")
	{
		rights_value = rights & BUILTIN_RIGHT_EDIT;
		if (rights_value) {
			return true;
		}
	}
	else if (rights_check == "print")
	{
		rights_value = rights & BUILTIN_RIGHT_PRINT;
		if (rights_value) {
			return true;
		}
	}
	else if (rights_check == "copycontent")
	{
		rights_value = rights & BUILTIN_RIGHT_CLIPBOARD;
		if (rights_value) {
			return true;
		}
	}
	else if (rights_check == "saveas")
	{
		rights_value = rights & BUILTIN_RIGHT_SAVEAS;
		if (rights_value) {
			return true;
		}
	}
	else if (rights_check == "decrypt")
	{
		rights_value = rights & BUILTIN_RIGHT_DECRYPT;
		if (rights_value) {
			return true;
		}
	}
	else if (rights_check == "sc")
	{
		rights_value = rights & BUILTIN_RIGHT_SCREENCAP;
		if (rights_value) {
			return true;
		}
	}
	else if (rights_check == "send")
	{
		rights_value = rights & BUILTIN_RIGHT_SEND;
		if (rights_value) {
			return true;
		}
	}
	else if (rights_check == "classify")
	{
		rights_value = rights & BUILTIN_RIGHT_CLASSIFY;
		if (rights_value) {
			return true;
		}
	}
	else if (rights_check == "share")
	{
		rights_value = rights & BUILTIN_RIGHT_SHARE;
		if (rights_value) {
			return true;
		}

	}
	else if (rights_check == "download")
	{
		rights_value = rights & BUILTIN_RIGHT_DOWNLOAD;
		if (rights_value) {
			return true;
		}
	}
	else if (rights_check == "watermark")
	{
		rights_value = rights & BUILTIN_OBRIGHT_WATERMARK;
		if (rights_value) {
			return true;
		}
	}

	return false;
}

vector<SDRmFileRight> getRightsString(int rights_value)
{
	vector<SDRmFileRight> rights_return;
	if ((rights_value&BUILTIN_RIGHT_VIEW)) {
		rights_return.push_back(RIGHT_VIEW);
	}
	else if ((rights_value&BUILTIN_RIGHT_EDIT))
	{
		rights_return.push_back(RIGHT_VIEW);
	}
	else if ((rights_value&BUILTIN_RIGHT_PRINT))
	{
		rights_return.push_back(RIGHT_PRINT);
	}
	else if ((rights_value&BUILTIN_RIGHT_CLIPBOARD))
	{
		rights_return.push_back(RIGHT_CLIPBOARD);
	}
	else if ((rights_value&BUILTIN_RIGHT_SAVEAS))
	{
		rights_return.push_back(RIGHT_SAVEAS);
	}
	else if ((rights_value&BUILTIN_RIGHT_DECRYPT))
	{
		rights_return.push_back(RIGHT_DECRYPT);
	}
	else if ((rights_value&BUILTIN_RIGHT_SCREENCAP))
	{
		rights_return.push_back(RIGHT_SCREENCAPTURE);
	}
	else if ((rights_value&BUILTIN_RIGHT_SEND))
	{
		rights_return.push_back(RIGHT_SEND);
	}
	else if ((rights_value&BUILTIN_RIGHT_CLASSIFY))
	{
		rights_return.push_back(RIGHT_CLASSIFY);
	}
	else if (BUILTIN_RIGHT_SHARE)
	{
		rights_return.push_back(RIGHT_SHARE);
	}
	else if (BUILTIN_RIGHT_CLASSIFY)
	{
		rights_return.push_back(RIGHT_CLASSIFY);
	}
	else if (BUILTIN_OBRIGHT_WATERMARK)
	{
		rights_return.push_back(RIGHT_WATERMARK);
	}
	return rights_return;
}

bool is_projectowner(int pro_id, string email) {
	bool list_result = GetProjectListResponseData("1", "100", "name", "all");
	Json::Value jroot;
	jroot = ParseJsonFile(projectlist_return);
	int project_count = jroot["results"]["detail"].size();
	for (int i = 0; i < project_count; i++) {
		int p_id = jroot["results"]["detail"][i]["id"].asInt();
		if (p_id == pro_id) {
			string email_jroot = jroot["results"]["detail"][i]["owner"]["email"].asString();
			if (email_jroot == email) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			continue;
		}
	}
	return false;
}

bool is_projectmember(int pro_id, string email) {
	bool member_list = Project_MemberListResponseData(to_string(pro_id));
	Json::Value jroot;
	jroot = ParseJsonFile(projectmemberlist_return);
	int member_count = jroot["results"]["detail"]["members"].size();
	for (int i = 0; i < member_count; i++) {
		string email_jroot = jroot["results"]["detail"]["members"][i]["email"].asString();
		if (email_jroot == email) {
			return true;
		}
	}
	return false;
}

int rights_changeToValue(vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks) {
	int count = rightsAndWatermarks.size();
	int rights_value = 0;

	for (int i = 0; i < count; i++)
	{
		rights_value = rights_value + rightsAndWatermarks[i].first;
	}
	return rights_value;
}

int rights_changeToValue2(vector<SDRmFileRight> filerights) {
	int count = filerights.size();
	int rights_value = 0;

	for (int i = 0; i < count; i++)
	{
		rights_value = rights_value + filerights[i];
	}
	return rights_value;
}
