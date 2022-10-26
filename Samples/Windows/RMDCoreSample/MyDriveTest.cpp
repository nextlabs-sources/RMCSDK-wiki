#include "stdafx.h"
#include "MyDriveTest.h"
#include "UserTest.h"
#include "HttpReqest.h"
#include <assert.h>
#include "rmccore/restful/rmmydrive.h"

using namespace RMCCORE;
using namespace std;

void TestMyDrive(void)
{
	RMUser	  user = GetDefaultUser();

	RMMyDrive * mydrive = user.GetMyDrive();
	assert(NULL != mydrive);
	HTTPRequest	request = mydrive->GetStorageQuery();
	printHttpRequest(request);

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile("RMMyDrive.txt");
	assert(doc.LoadJsonString(jsonstr, &err_code, &err_pos));

	RetValue retv = mydrive->ImportFromRMSResponse(jsonstr);

	cout << endl;
	cout << "My drive usage:" << mydrive->GetUsage() << endl;
	cout << "Total Quota:" << mydrive->GetQuota() << endl;
	cout << "My vault usage:" << mydrive->GetVaultUsage() << endl;
	cout << "Vault Quota:" << mydrive->GetVaultQuota() << endl;

}