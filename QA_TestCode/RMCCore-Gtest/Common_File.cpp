#include "stdafx.h""
#include "rmccore\restful\rmnxlfile.h"
#include "rmccore\restful\rmuser.h"
#include "rmccore\restful\rmactivitylog.h"
#include <Windows.h>

using namespace NXLFMT;

vector<std::string> ReturnRecipient(string recipient) {
	vector<std::string> r;
	splitString(recipient, r, "|");
	return r;
}

uint64_t getRightsValue(string rights_csv)
{
	uint64_t rights_value = 0x0000000000000000ULL;;
	vector<string> r;
	splitString(rights_csv, r, "|");
	for (vector<string>::size_type i = 0; i != r.size(); ++i)
	{
		/*
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
		*/
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

void printRights(NXLFMT::Rights rights)
{
	cout << "NXL file rights: ";

	std::vector<std::string> rightstr1;
	rights.ToStrings(rightstr1);

	for (size_t i = 0; i < rightstr1.size(); i++)
		cout << rightstr1[i] << ";";
	cout << endl;
}


// GetFileSpaceSize
#define CLOSE_HANDLE(handle) \
     do \
     { \
         CloseHandle(handle); \
         handle = NULL; \
     } while (FALSE)

std::wstring s2ws(const std::string& s) {

	int len;

	int slength = (int)s.length() + 1;

	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);

	wchar_t* buf = new wchar_t[len];

	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);

	std::wstring r(buf);

	delete[] buf;

	return r.c_str();

}

int GetFileSpaceSize(string path)
{
	std::wstring stemp = s2ws(path);
	// 文件路径
	LPCWSTR  szFileName = stemp.c_str();


	// 打开文件句柄
	HANDLE hFile = ::CreateFileW(szFileName, GENERIC_READ | FILE_SHARE_READ, 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf_s(_T("Failed to create file handle: %s ! error code:%d\n"), szFileName, GetLastError());
		return -1;
	}

	// 获取文件大小
	UINT64 uFileSize = 0;
	::GetFileSizeEx(hFile, reinterpret_cast<PLARGE_INTEGER>(&uFileSize));

	FILE_STANDARD_INFO fsi = { 0 };
	if (!::GetFileInformationByHandleEx(hFile, FileStandardInfo, &fsi, sizeof(FILE_STANDARD_INFO)))
	{
		_tprintf_s(_T("Failed to get file info! error code:%d\n"), GetLastError());
		CLOSE_HANDLE(hFile);
		return -2;
	}

	_tprintf_s(_T("FileName : %s\n"), szFileName);
	_tprintf_s(_T("FileSize : %I64u Byte\n"), uFileSize);
	_tprintf_s(_T("FileSpacesSize : %I64u Byte\n"), fsi.AllocationSize);
	CLOSE_HANDLE(hFile);

	return 0;
}

void EXPECT_EQ_CompareFileSpaceSize(string file_1, string file_2)
{
	std::wstring f1 = s2ws(file_1);
	std::wstring f2 = s2ws(file_2);

	LPCWSTR  szFileName_1 = f1.c_str();
	LPCWSTR  szFileName_2 = f2.c_str();


	HANDLE hFile_1 = ::CreateFileW(szFileName_1, GENERIC_READ | FILE_SHARE_READ, 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFile_2 = ::CreateFileW(szFileName_2, GENERIC_READ | FILE_SHARE_READ, 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ((hFile_1 == INVALID_HANDLE_VALUE) || (hFile_2 == INVALID_HANDLE_VALUE))
	{
		_tprintf_s(_T("Failed to create file handle! error code:%d\n"), GetLastError());
		EXPECT_EQ(0, 1);
	}

	else
	{
		//UINT64 uFileSize = 0;
		//::GetFileSizeEx(hFile_1, reinterpret_cast<PLARGE_INTEGER>(&uFileSize));

		FILE_STANDARD_INFO fsi_1 = { 0 };
		FILE_STANDARD_INFO fsi_2 = { 0 };
		if (!::GetFileInformationByHandleEx(hFile_1, FileStandardInfo, &fsi_1, sizeof(FILE_STANDARD_INFO)))
		{
			_tprintf_s(_T("Failed to get file info! error code:%d\n"), GetLastError());
			CLOSE_HANDLE(hFile_1);
			EXPECT_EQ(0, 1);
		}
		else if (!::GetFileInformationByHandleEx(hFile_2, FileStandardInfo, &fsi_2, sizeof(FILE_STANDARD_INFO)))
		{
			_tprintf_s(_T("Failed to get file info! error code:%d\n"), GetLastError());
			CLOSE_HANDLE(hFile_2);
			EXPECT_EQ(0, 1);
		}
		else
		{
			_tprintf_s(_T("FileName : %s\n"), szFileName_1);
			////_tprintf_s(_T("FileSize : %I64u Byte\n"), uFileSize);
			_tprintf_s(_T("FileSpacesSize : %I64u Byte\n"), fsi_1.AllocationSize);
			_tprintf_s(_T("FileName : %s\n"), szFileName_2);
			////_tprintf_s(_T("FileSize : %I64u Byte\n"), uFileSize);
			_tprintf_s(_T("FileSpacesSize : %I64u Byte\n"), fsi_2.AllocationSize);


			//EXPECT_STREQ(fsi_1.AllocationSize, fsi_2.AllocationSize);

			CLOSE_HANDLE(hFile_1);
			CLOSE_HANDLE(hFile_2);
		}

	}

}
