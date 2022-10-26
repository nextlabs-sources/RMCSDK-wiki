#include "stdafx.h"
#include "LogTest.h"
#include "NxlTest.h"
#include "UserTest.h"
#include "HttpReqest.h"
#include <assert.h>
#include "rmccore/restful/rmnxlfile.h"

using namespace RMCCORE;

void TestNxlLog(void)
{
	RMNXLFile nfile = GetDefaultNXLFile();
	RMUser	  user = GetDefaultUser();

	RMNXLFileLogs logs;

	HTTPRequest request = user.GetNXLFileActivitylogQuery(nfile, 0, 20, RLSFEmpty, "", RMLogOrderBy::RMLOAccessTime, false);
	printHttpRequest(request);

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile("RMNXLLog.txt");
	assert(doc.LoadJsonString(jsonstr, &err_code, &err_pos));

	RetValue retv = logs.ImportFromRMSResponse(jsonstr);

	size_t lognum = logs.size();

	for (size_t i = 0; i < lognum; i++) {

		cout << "Log " << i + 1 << " :" << logs.GetAt(i)->GetEmail() <<":" << logs.GetAt(i)->GetOperation() << endl;
	}

	cout << endl;

	cout << "log data:" << logs.ExportToString() << endl;

	RMLogPool logpool = GetDefaultLogPool();

	HTTPRequest logrequest = user.GetUploadActivitylogQuery(&logpool);
	printHttpRequest(logrequest);


}