#include "stdafx.h"
#include <assert.h>
#include "MyValutTest.h"
#include "NxlTest.h"
#include "UserTest.h"
#include "HttpReqest.h"
#include "rmccore/restful/rmnxlfile.h"

void TestMyVault()
{
	RMNXLFile nfile = GetDefaultNXLFile();
	RMUser user = GetDefaultUser();

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	cout << "Query NXLFile Metadata:" << endl;

	HTTPRequest request = user.GetNXLFileMetadataQuery(nfile);
	printHttpRequest(request);
	cout << endl;

	const std::string jsonstr = ReadFromFile("RMNXLFileMData.txt");

	RetValue retv = nfile.ImportMetadataQueryFromRMSResponse(jsonstr);

	RMRecipients rp(nfile.GetFileRecipients());

	cout << "NXLFile Metadata :" << endl;
	cout << "\tfile name:" << nfile.GetName() << endl;
	cout << "\tfile link:"<<nfile.GetFileLink() << endl;
	cout << "\tLast modify on:" << nfile.GetLastModify() << endl;
	cout << "\tShared flag:" << (nfile.IsShared() ? "true" : "false") << endl;
	cout << "\tDelete flag:" << (nfile.IsDeleted() ? "true" : "false") << endl;
	cout << "\tRevoke flag:" << (nfile.IsRevoked() ? "true" : "false") << endl;
	cout << endl;



}