#include "stdafx.h"
#include "UpdateTest.h"
#include "TenantTest.h"
#include "UserTest.h"
#include "HttpReqest.h"

#include "rmccore/restful/rmproduct.h"

#include "common.h"
#include <assert.h>


using namespace RMCCORE;
using namespace std;

void TestAutoUpdate(void)
{
	RMTenant tenant = CreateDefaultTenant();
	RMSystemPara param = GenerateSystemParameter();

	RMProduct product("RMD Core Sample", 1, 0, 100);
	product.SetAppInfo("c:\test", "Nextlabs");
	param.SetProduct(product);
	param.SetProcessor(RPAWindowsX64);

	HTTPRequest request = tenant.GetProductUpdateQuery(param);

	printHttpRequest(request);
	cout << endl;

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile("RMAutoUpdate.txt");
	assert(doc.LoadJsonString(jsonstr, &err_code, &err_pos));

	RetValue retv = product.ImportFromRMSResponse(jsonstr);

	cout << "new version: " << product.GetNewVersionString() << endl;
	cout << "download URL: " << product.GetDownloadURL() << endl;
	cout << "package checksum: " << product.GetDownloadChecksum() << endl;
}
