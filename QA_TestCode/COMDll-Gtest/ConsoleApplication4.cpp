#include "pch.h"
#include <iostream>
#include <string>

#import "nxrmcom.tlb"  named_guids raw_interfaces_only  

int main()
{
	//init
	CoInitialize(NULL);

	HRESULT hResult;

	nxrmcomLib::ICnxrmcomPtr nxrmcomPtr(__uuidof(nxrmcomLib::Cnxrmcom));

	//GetRights
	std::wstring nxlFilePath = L"C:\\Users\\jrzhou\\Desktop\\Export\\2Dnormal-2019-08-22-17-02-15.pdf.nxl";

	std::wstring riths;

	BSTR _nxlFilePath = SysAllocStringLen(nxlFilePath.data(), nxlFilePath.size());

	BSTR _riths = SysAllocStringLen(riths.data(), riths.size());

	hResult = nxrmcomPtr->GetRights(_nxlFilePath , &_riths);
	
	std::wstring strRignts(_riths);

	std::wcout << strRignts << std::endl;

	std::cout << hResult << std::endl;

	//ProtectFile
	std::wstring  filePath = L"C:\\Users\\jrzhou\\Desktop\\TestFiles\\doc\\E4 transcript.docx";
   
	std::wstring  tags = L"{\"Public\": [\"Public\"],\"Restricted\" : [\"Restricted\"],\"Confidential\" : [\"Confidential\"],\"Internal\" : [\"Internal\"],\"Sensitivity\" : [\"Internal or Internal Use\"]}";
	
	std::wstring newNxlFilePath;

	BSTR _filePath = SysAllocStringLen(filePath.data(), filePath.size());

	BSTR _tags = SysAllocStringLen(tags.data(), tags.size());

	BSTR _newNxlFilePath = SysAllocStringLen(newNxlFilePath.data(), newNxlFilePath.size());

	hResult = nxrmcomPtr->ProtectFile(_filePath, _tags, &_newNxlFilePath);

	std::wstring StrNewNxlFilePath(_newNxlFilePath);

	std::wcout << StrNewNxlFilePath << std::endl;

	std::cout << hResult << std::endl;

	//ViewFile
	int option = 0;

	hResult = nxrmcomPtr->ViewFile(_nxlFilePath, option );

	std::cout << hResult << std::endl;

	CoUninitialize();

	getchar();
	return 0;
}

