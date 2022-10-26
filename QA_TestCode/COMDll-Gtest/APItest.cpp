#include "pch.h"
#include <iostream>
#include <comdef.h>
#include "INxrcom.h"

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <tlhelp32.h>
#include <string>

#import "nxrmcom.tlb" named_guids raw_interfaces_only  


//// {9B6E1D13-F007-4031-8F4F-61038FD01EE1}
//extern "C" const GUID CLSID_Nxrcom =
//{ 0x9b6e1d13, 0xf007, 0x4031,
//{ 0x8f, 0x4f, 0x61, 0x3, 0x8f, 0xd0, 0x1e, 0xe1 } };
//
//// {6285D0A5-0871-48B7-A86D-033980D54558}
//extern "C" const GUID IID_Nxrcom =
//{ 0x6285d0a5, 0x871, 0x48b7,
//{ 0xa8, 0x6d, 0x3, 0x39, 0x80, 0xd5, 0x45, 0x58 } };

string Trim(string& str)
{
	str.erase(0, str.find_first_not_of("\""));
	str.erase(str.find_last_not_of("\"") + 1);
	return str;
}
std::wstring StringToWString(const std::string& str)
{
	setlocale(LC_ALL, "chs");
	const char* point_to_source = str.c_str();
	size_t new_size = str.size() + 1;
	wchar_t *point_to_destination = new wchar_t[new_size];
	wmemset(point_to_destination, 0, new_size);
	mbstowcs(point_to_destination, point_to_source, new_size);
	std::wstring result = point_to_destination;
	delete[]point_to_destination;
	setlocale(LC_ALL, "C");
	return result;
}

std::string WStringToString(const std::wstring &wstr)
{
	std::string str;
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');
	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);
	if (nResult == 0)
	{
		return "";
	}
	return str;
}

std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	pos = str.find(pattern, 0);
	std::string s = str.substr(0, pos);
	result.push_back(s);
	std::string s2 = str.substr(pos + 1, str.size());
	result.push_back(s2);
	return result;
}

void splitString1(const string& sourceString, vector<string>& result_string, const string& split)
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

string patchTagString1(string & tags_csv)
{
	//for tag
	//std::string tags = "{\"itar\":[\"TAA-02\"]}";
	string tags = "";
	vector<string> tag_n;
	vector<string> tdetail;
	splitString1(tags_csv, tag_n, ";");
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
		splitString1(tn, tdetail, "|");
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

vector<SDRmFileRight> Tick_Rights1(string rights_str) {

	std::vector<SDRmFileRight> rights;
	vector<string> r;
	splitString1(rights_str, r, "|");

	for (vector<string>::size_type i = 0; i != r.size(); ++i)
	{
		if (r[i] == "view")
		{
			rights.push_back(RIGHT_VIEW);
		}
		else if (r[i] == "edit")
		{
			rights.push_back(RIGHT_EDIT);
		}
		else if (r[i] == "print")
		{
			rights.push_back(RIGHT_PRINT);
		}
		else if (r[i] == "copycontent")
		{
			rights.push_back(RIGHT_CLIPBOARD);
		}
		else if (r[i] == "saveas")
		{
			rights.push_back(RIGHT_SAVEAS);
		}
		else if (r[i] == "decrypt")
		{
			rights.push_back(RIGHT_DECRYPT);
		}
		else if (r[i] == "sc")
		{
			rights.push_back(RIGHT_SCREENCAPTURE);
		}
		else if (r[i] == "send")
		{
			rights.push_back(RIGHT_SEND);
		}
		else if (r[i] == "classify")
		{
			rights.push_back(RIGHT_CLASSIFY);
		}
		else if (r[i] == "share")
		{
			rights.push_back(RIGHT_SHARE);
		}
		else if (r[i] == "download")
		{
			rights.push_back(RIGHT_DOWNLOAD);
		}
		else if (r[i] == "watermark")
		{
			rights.push_back(RIGHT_WATERMARK);
		}
	}
	return rights;
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

const vector<vector<string>> csv_nxrcom_GetRights = readCSV("C:\\SDK test data\\nxrcomGetRights.csv");

TEST(SampleCode, Test) {
	//init
	CoInitialize(NULL);

	HRESULT hResult;

	nxrmcomLib::ICnxrmcomPtr nxrmcomPtr(__uuidof(nxrmcomLib::Cnxrmcom));

	//GetRights
	std::wstring nxlFilePath = L"C:\\Users\\jrzhou\\Desktop\\Export\\2Dnormal-2019-08-22-17-02-15.pdf.nxl";

	std::wstring riths;

	BSTR _nxlFilePath = SysAllocStringLen(nxlFilePath.data(), nxlFilePath.size());

	BSTR _riths = SysAllocStringLen(riths.data(), riths.size());

	hResult = nxrmcomPtr->GetRights(_nxlFilePath, &_riths);

	std::wstring strRignts(_riths);

	std::wcout << strRignts << std::endl;

	std::cout << hResult << std::endl;

	//ProtectFile
	std::wstring  filePath = L"C:\\Users\\jrzhou\\Desktop\\TestFiles\\doc\\E4 transcript.docx";

	std::wstring  tags = L"{\"Public\": [\"Public\"],\"Restricted\" : [\"Restricted\"],\"Confidential\" : [\"Confidential\"],\"Internal\" : [\"Internal\"],\"Sensitivity\" : [\"Internal or Internal Use\"]}";

	std::wstring newNxlFilePath;

	std::wstring DestinationFolder = L"C:\\Users\\sfeng\\Desktop\\protect file";

	BSTR _filePath = SysAllocStringLen(filePath.data(), filePath.size());

	BSTR _tags = SysAllocStringLen(tags.data(), tags.size());

	BSTR _DestinationFolder = SysAllocStringLen(DestinationFolder.data(), DestinationFolder.size());

	BSTR _newNxlFilePath = SysAllocStringLen(newNxlFilePath.data(), newNxlFilePath.size());

	hResult = nxrmcomPtr->ProtectFile(_filePath, _tags, _DestinationFolder, &_newNxlFilePath);

	std::wstring StrNewNxlFilePath(_newNxlFilePath);

	std::wcout << StrNewNxlFilePath << std::endl;

	std::cout << hResult << std::endl;

	//ViewFile
	int option = 0;

	std::wstring OutputJson;
	BSTR _OutputJson = SysAllocStringLen(OutputJson.data(), OutputJson.size());

	hResult = nxrmcomPtr->ViewFile(_nxlFilePath, option, &_OutputJson);

	std::cout << hResult << std::endl;

	CoUninitialize();

	getchar();

}

class nxrcomGetFileRightsTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(nxrcomGetFileRightsTest, nxrcomGetFileRights) {
	//init
	CoInitialize(NULL);

	HRESULT hResult;

	nxrmcomLib::ICnxrmcomPtr nxrmcomPtr(__uuidof(nxrmcomLib::Cnxrmcom));

	vector<string> csv = GetParam();

	string file_path = csv[3];
	wstring file_pathw = StringToWString(file_path);
	int expect_rsult = atoi(csv[4].c_str());


	std::wstring nxlFilePath = file_pathw;

	std::wstring riths;

	BSTR _nxlFilePath = SysAllocStringLen(nxlFilePath.data(), nxlFilePath.size());

	BSTR _riths = SysAllocStringLen(riths.data(), riths.size());

	hResult = nxrmcomPtr->GetRights(_nxlFilePath, &_riths);

	long pdwWin32 = HRESULT_CODE(hResult);
	int Actual_Result = (int)pdwWin32;

	EXPECT_EQ(expect_rsult, Actual_Result);

	std::wstring strRignts(_riths);

	wstring str = (_bstr_t)_riths;

	std::wcout << "strRights: " << strRignts << std::endl;

	std::wcout << "strRights1: " << str << std::endl;

	std::cout << "hResult: " << hResult << std::endl;
	cout << "protect return code: " << Actual_Result << endl;

	CoUninitialize();

	//getchar();

}
INSTANTIATE_TEST_CASE_P(nxrcomGetFileRightsTesting, nxrcomGetFileRightsTest, testing::ValuesIn(csv_nxrcom_GetRights));

//class nxrcomGetFileRightsTest2 :public::testing::TestWithParam<vector<string>> {};
//TEST_P(nxrcomGetFileRightsTest2, nxrcomGetFileRights2) {
//	//init
//	CoInitialize(NULL);
//
//	HRESULT hResult;
//
//	nxrmcomLib::ICnxrmcomPtr nxrmcomPtr(__uuidof(nxrmcomLib::Cnxrmcom));
//
//	vector<string> csv = GetParam();
//
//	string file_path = csv[3];
//	wstring file_pathw = StringToWString(file_path);
//	int expect_rsult = atoi(csv[4].c_str());
//
//
//	std::wstring nxlFilePath = file_pathw;
//
//	std::wstring riths;
//
//	BSTR _nxlFilePath = SysAllocStringLen(nxlFilePath.data(), nxlFilePath.size());
//
//	BSTR _riths = SysAllocStringLen(riths.data(), riths.size());
//
//	hResult = GetRights2(NxlFilePath,Rights)
//
//	long pdwWin32 = HRESULT_CODE(hResult);
//	int Actual_Result = (int)pdwWin32;
//
//	EXPECT_EQ(expect_rsult, Actual_Result);
//
//	std::wstring strRignts(_riths);
//
//	std::wcout << "strRights: " << strRignts << std::endl;
//
//	std::cout << "hResult: " << hResult << std::endl;
//	cout << "protect return code: " << pdwWin32 << endl;
//
//	CoUninitialize();
//
//	getchar();
//
//}
//INSTANTIATE_TEST_CASE_P(nxrcomGetFileRightsTesting2, nxrcomGetFileRightsTest2, testing::ValuesIn(csv_nxrcom_GetRights));

const vector<vector<string>> csv_nxrcom_Protect = readCSV("C:\\SDK test data\\nxrcomProtect.csv");
class nxrcomProtectFileTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(nxrcomProtectFileTest, nxrcomProtectFile) {
	

	//init
	CoInitialize(NULL);

	HRESULT hResult;

	nxrmcomLib::ICnxrmcomPtr nxrmcomPtr(__uuidof(nxrmcomLib::Cnxrmcom));

	vector<string> csv = GetParam();

	string file_path = csv[4];
	wstring file_pathw = StringToWString(file_path);
	string csv_tags = csv[5];
	string csv_rights = csv[6];
	int rights_return_code = atoi(csv[7].c_str());
	int expect_rsult = atoi(csv[8].c_str());
	int is_run = atoi(csv[1].c_str());
	if (is_run == 1) {
		wstring use_tags = StringToWString(patchTagString1(csv_tags));
		wcout << "use_tags: " << use_tags << endl;

		std::wstring  filePath = file_pathw;

		std::wstring newNxlFilePath;

		std::wstring DestinationFolder = L"C:\\Users\\sfeng\\Desktop\\protect2";

		BSTR _filePath = SysAllocStringLen(filePath.data(), filePath.size());

		BSTR _tags = SysAllocStringLen(use_tags.data(), use_tags.size());

		BSTR _newNxlFilePath = SysAllocStringLen(newNxlFilePath.data(), newNxlFilePath.size());

		BSTR _DestinationFolder = SysAllocStringLen(DestinationFolder.data(), DestinationFolder.size());

		hResult = nxrmcomPtr->ProtectFile(_filePath, _tags, _DestinationFolder, &_newNxlFilePath);

		long pdwWin32 = HRESULT_CODE(hResult);

		int Actual_Result = (int)pdwWin32;

		ASSERT_EQ(Actual_Result, 0);
		std::wstring StrNewNxlFilePath(_newNxlFilePath);

		std::wcout << "protect API return: " << StrNewNxlFilePath << std::endl;

		std::cout << "hResult: " << hResult << std::endl;

		vector<string> r;
		splitString1(WStringToString(StrNewNxlFilePath), r, ",");

		vector<string> r2;
		splitString1(r[1], r2, "\"");

		cout << "r2:" << Trim(r2[3]) << endl;

		std::wstring newNxlFilePath2 = StringToWString(Trim(r2[3]));
		BSTR _newNxlFilePath2 = SysAllocStringLen(newNxlFilePath2.data(), newNxlFilePath2.size());

		vector<string> r3;
		splitString1(WStringToString(StrNewNxlFilePath), r3, ",");
		vector<string> r4;
		splitString1(r3[0], r4, ":");

		int code = atoi(r4[1].c_str());
		cout << "code: " << code << endl;		

		if (code == 0)
		{
			std::wstring riths;

			BSTR _riths = SysAllocStringLen(riths.data(), riths.size());
			
			hResult = nxrmcomPtr->GetRights(_newNxlFilePath2, &_riths);

			long pdwWin321 = HRESULT_CODE(hResult);

			int Actual_Result2 = (int)pdwWin321;

			ASSERT_EQ(Actual_Result2, 0);
			if (Actual_Result2 == 0) {

				std::wstring wstrRights(_riths);
				wcout << "rights: " << wstrRights << endl;
				string strRights = WStringToString(wstrRights);
				transform(strRights.begin(), strRights.end(), strRights.begin(), ::tolower);

				vector<string> r3;
				splitString1(strRights, r3, ",");

				vector<string> r4;
				splitString1(r3[1], r4, ":");

				cout << "r2:" << Trim(r4[1]) << endl;

				vector<SDRmFileRight> nxl_file_rights = Tick_Rights1(Trim(r4[1]));
				vector<SDRmFileRight> csv_file_rights = Tick_Rights1(csv_rights);


				EXPECT_EQ(nxl_file_rights.size(), csv_file_rights.size());
				int nxl_file_rights_value = 0;
				for (int i = 0; i < nxl_file_rights.size(); i++) {
					nxl_file_rights_value += nxl_file_rights[i];
				}
				int csv_file_rights_value = 0;
				for (int i = 0; i < csv_file_rights.size(); i++) {
					csv_file_rights_value += csv_file_rights[i];
				}

				EXPECT_EQ(nxl_file_rights_value, csv_file_rights_value);
			}

		}
		else
		{
			cout << "-----file protect fail-----" << endl;
			EXPECT_EQ(code, expect_rsult);
		}

		std::cout << Actual_Result << std::endl;

		
	}

	CoUninitialize();

	//getchar();

	cout << "----------- case end ----------- " << endl;
}
INSTANTIATE_TEST_CASE_P(nxrcomProtectFileTesting, nxrcomProtectFileTest, testing::ValuesIn(csv_nxrcom_Protect));


const vector<vector<string>> csv_nxrcom_view = readCSV("C:\\SDK test data\\nxrcomView.csv");
class nxrcomViewFileTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(nxrcomViewFileTest, nxrcomViewFile) {
	//init
	CoInitialize(NULL);

	HRESULT hResult;

	nxrmcomLib::ICnxrmcomPtr nxrmcomPtr(__uuidof(nxrmcomLib::Cnxrmcom));

	vector<string> csv = GetParam();
	cout << " run case no: -----" << csv[0] <<"-----"<< endl;


	string csv_tags = csv[4];
	string csv_rights = csv[5];
	string view_file_path = csv[6];
	wstring view_file_pathw = StringToWString(view_file_path);
	int option = atoi(csv[7].c_str());
	int expect_value = atoi(csv[8].c_str());
	int is_run = atoi(csv[1].c_str());
	if (is_run == 1) {
		wstring use_tags = StringToWString(patchTagString1(csv_tags));
		wcout << "use_tags: " << use_tags << endl;
		
		std::wstring nxlFilePath = view_file_pathw;

		std::wstring riths;

		BSTR _nxlFilePath = SysAllocStringLen(nxlFilePath.data(), nxlFilePath.size());

		BSTR _riths = SysAllocStringLen(riths.data(), riths.size());

		hResult = nxrmcomPtr->GetRights(_nxlFilePath, &_riths);

		std::wstring str_riths(_riths);
		std::wcout << "GetRights API return json: " << str_riths << std::endl;

		long pdwWin32 = HRESULT_CODE(hResult);
		int Actual_Result = (int)pdwWin32;

		if (Actual_Result == 0) {
			std::wstring wstrRights(_riths);	
			string strRights = WStringToString(wstrRights);
			transform(strRights.begin(), strRights.end(), strRights.begin(), ::tolower);

			vector<string> r;
			splitString1(strRights, r, ",");

			vector<string> r2;
			splitString1(r[1], r2, ":");

			cout << "r2:" << Trim(r2[1]) << endl;

			vector<SDRmFileRight> nxl_file_rights = Tick_Rights1(Trim(r2[1]));
			vector<SDRmFileRight> csv_file_rights = Tick_Rights1(csv_rights);


			EXPECT_EQ(nxl_file_rights.size(), csv_file_rights.size());
			int nxl_file_rights_value = 0;
			for (int i = 0; i < nxl_file_rights.size(); i++) {
				nxl_file_rights_value += nxl_file_rights[i];
			}
			int csv_file_rights_value = 0;
			for (int i = 0; i < csv_file_rights.size(); i++) {
				csv_file_rights_value += csv_file_rights[i];
			}

			EXPECT_EQ(nxl_file_rights_value, csv_file_rights_value);
		}
		std::wstring OutputJson;
		BSTR _OutputJson = SysAllocStringLen(OutputJson.data(), OutputJson.size());

		hResult = nxrmcomPtr->ViewFile(_nxlFilePath, option, &_OutputJson);

		long pdwWin321 = HRESULT_CODE(hResult);
		int Actual_Result2 = (int)pdwWin321;
		EXPECT_EQ(expect_value, Actual_Result2);
		std::wstring strOutputJson(_OutputJson);
		std::wcout << "view API return json: " << strOutputJson << std::endl;
		std::cout << "hResult: " << hResult << std::endl;
		cout << "protect return code: " << pdwWin321<< endl;
		
	}

	CoUninitialize();

	//getchar();

	cout << "----------- case end ----------- " << endl;
}
INSTANTIATE_TEST_CASE_P(nxrcomViewFileTesting, nxrcomViewFileTest, testing::ValuesIn(csv_nxrcom_view));


TEST(singleGetRightsTest, singleGetRights) {
	//init
	CoInitialize(NULL);

	HRESULT hResult;

	nxrmcomLib::ICnxrmcomPtr nxrmcomPtr(__uuidof(nxrmcomLib::Cnxrmcom));

	//std::wstring nxlFilePath = L"C:\\nxrcomTest\\normal file but add.txt.nxl";
	std::wstring nxlFilePath = L"C:\\nxrcomTest\\2txt.txt";
	//std::wstring nxlFilePath = L"C:\\nxrcomTest\\Course Introduction -2019-08-21-03-33-46.pptx.nxl";

	std::wstring riths;
	
	int *single;

	BSTR _nxlFilePath = SysAllocStringLen(nxlFilePath.data(), nxlFilePath.size());

	BSTR _riths = SysAllocStringLen(riths.data(), riths.size());

	hResult = nxrmcomPtr->GetRights(_nxlFilePath, &_riths);

	long pdwWin32 = HRESULT_CODE(hResult);

	std::wstring strRignts(_riths);

	std::wcout << "strRights: "<< strRignts << std::endl;

	std::cout << "hResult: " << hResult << std::endl;
	
	CoUninitialize();

	getchar();


}

TEST(singleViewFileTest, singleViewFile) {
	//init
	CoInitialize(NULL);

	HRESULT hResult;

	nxrmcomLib::ICnxrmcomPtr nxrmcomPtr(__uuidof(nxrmcomLib::Cnxrmcom));


	//std::wstring nxlFilePath = L"C:\\Users\\sfeng\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate\\my-2019-08-23-02-48-21.docx";
	//std::wstring nxlFilePath = L"C:\\\\hz-ts03\\transfer\\Starry\\nxl\\aeskey-2019-08-22-06-59-05.c.nxl";
	std::wstring nxlFilePath = L"C:\\viewPath\\pot-2019-08-22-11-08-31.pot.nxl";

	std::wstring riths;

	std::wstring OutputJson;

	BSTR _nxlFilePath = SysAllocStringLen(nxlFilePath.data(), nxlFilePath.size());

	BSTR _riths = SysAllocStringLen(riths.data(), riths.size());

	BSTR _OutputJson = SysAllocStringLen(OutputJson.data(), OutputJson.size());
	
	int option = 1;

	hResult = nxrmcomPtr->ViewFile(_nxlFilePath, option,&_OutputJson);

	long pdwWin32 = HRESULT_CODE(hResult);

	std::wstring strRignts(_riths);

	std::wcout <<"strRights: "<< strRignts << std::endl;

	std::cout << "hResult: " << hResult << std::endl;
	std::wstring strOutputJson(_OutputJson);
	std::wcout << "outputJson: " << strOutputJson << std::endl;

	CoUninitialize();

	getchar();


}

TEST(singleProtectFileTest, singleProtectFile) {
	//init
	CoInitialize(NULL);

	HRESULT hResult;

	nxrmcomLib::ICnxrmcomPtr nxrmcomPtr(__uuidof(nxrmcomLib::Cnxrmcom));

	std::wstring DestinationFolder = L"E:\\B";

	//ProtectFile
	std::wstring  filePath = L"E:\\A\\API test-2019-09-25-06-19-16.txt.nxl";
	//std::wstring  filePath = L"C:\\888.txt";

	std::wstring  tags = L"{\"Rights\":[\t\"Rights_All\"],\"itar\":[\"itar_no\"]}";

	std::wstring newNxlFilePath;

	BSTR _filePath = SysAllocStringLen(filePath.data(), filePath.size());

	BSTR _tags = SysAllocStringLen(tags.data(), tags.size());

	BSTR _newNxlFilePath = SysAllocStringLen(newNxlFilePath.data(), newNxlFilePath.size());

	BSTR _DestinationFolder = SysAllocStringLen(DestinationFolder.data(), DestinationFolder.size());

	hResult = nxrmcomPtr->ProtectFile(_filePath, _tags, _DestinationFolder, &_newNxlFilePath);

	long pdwWin32 = HRESULT_CODE(hResult);

	std::wstring StrNewNxlFilePath(_newNxlFilePath);

	std::wcout <<"Protect API return : "<< StrNewNxlFilePath << std::endl;

	std::cout <<"hResult: "<< hResult << std::endl;

	CoUninitialize();

	getchar();


}

TEST(singLockFileSyncTest, singLockFileSync) {
	CoInitialize(NULL);

	HRESULT hResult;

	nxrmcomLib::ICnxrmcomPtr nxrmcomPtr(__uuidof(nxrmcomLib::Cnxrmcom));
	wstring filePath = L"C:\\Users\\sfeng\\Desktop\\test7.txt.nxl";
	//wstring filePath = L"C:\\Users\\sfeng\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate\\LocalApp-2019-09-26-07-49-12.log.nxl";
	wstring outputJson;
	BSTR _filePath = SysAllocStringLen(filePath.data(), filePath.size());
	BSTR _outPutJson = SysAllocStringLen(outputJson.data(), outputJson.size());

	hResult = nxrmcomPtr->LockFileSync(_filePath,&_outPutJson);

	std::wstring StrOutPutJson(_outPutJson);

	std::wcout << "LockFileSync API return : " << StrOutPutJson << std::endl;
	std::wcout << "hResult: " << hResult << std::endl;
	CoUninitialize();

	//getchar();

	cout << "----------- case end ----------- " << endl;
}

TEST(singResumeFileSyncTest, singResumeFileSync) {
	CoInitialize(NULL);

	HRESULT hResult;

	nxrmcomLib::ICnxrmcomPtr nxrmcomPtr(__uuidof(nxrmcomLib::Cnxrmcom));
	wstring filePath = L"C:\\Users\\sfeng\\Desktop\\test7.txt.nxl";
	//wstring filePath = L"C:\\Users\\sfeng\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate\\LocalApp-2019-09-26-07-49-12.log.nxl";
	wstring outputJson;
	BSTR _filePath = SysAllocStringLen(filePath.data(), filePath.size());
	BSTR _outPutJson = SysAllocStringLen(outputJson.data(), outputJson.size());

	hResult = nxrmcomPtr->ResumeFileSync(_filePath,&_outPutJson);

	std::wstring StrOutPutJson(_outPutJson);

	std::wcout << "ResumeFileSync API return : " << StrOutPutJson << std::endl;
	std::wcout << "hResult: " << hResult << std::endl;
	CoUninitialize();

	//getchar();

	cout << "----------- case end ----------- " << endl;
}

TEST(LockFileSync, test)
{
	bool flag = true;
	while (flag) {
		//init
		HRESULT hResult;
		CoInitialize(NULL);
		nxrmcomLib::ICnxrmcomPtr nxrmcomPtr(__uuidof(nxrmcomLib::Cnxrmcom));

		string f;
		int option;
		cout << endl << "Input test file: ";
		cin >> f;
		cout << "Input option(0:LockFileSync, 1: ResumeFileSync): ";
		cin >> option;

		std::wstring testfile = StringToWString(f);
		BSTR _filePath = SysAllocStringLen(testfile.data(), testfile.size());
		std::wstring OutputJson;
		BSTR _OutputJson = SysAllocStringLen(OutputJson.data(), OutputJson.size());

		if (option == 0) {
			cout << "LockFileSync" << endl;
			std::wstring filePath(_filePath);
			wcout << "filePath: " << filePath << endl;
			hResult = nxrmcomPtr->LockFileSync(_filePath, &_OutputJson);
			std::wstring strOutputJson(_OutputJson);
			std::wcout << "outputJson: " << strOutputJson << std::endl;
		}
		else {
			cout << "ResumeFileSync" << endl;
			std::wstring filePath(_filePath);
			wcout << "filePath: " << filePath << endl;
			hResult = nxrmcomPtr->ResumeFileSync(_filePath, &_OutputJson);
			std::wstring strOutputJson(_OutputJson);
			std::wcout << "outputJson: " << strOutputJson << std::endl;
		}
		long lres = HRESULT_CODE(hResult);
		std::cout << "hResult: " << lres << std::endl;
		flag = false;
		CoUninitialize();

	}
}
