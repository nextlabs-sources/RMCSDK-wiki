#include "stdafx.h"
#include "TenantTest.h"

#include "rmccore\rmccore.h"
#include "common.h"
#include "rmccore\restful\rmtenant.h"
#include "HttpReqest.h"

#include "assert.h"

#define Tenant_Return	"{\"statusCode\":200,\"message\":\"OK\",\"serverTime\":1512505538781,\"results\":{\"server\":\"https://skydrm.com/rms\"}}"

using namespace RMCCORE;
using namespace std;

const RMTenant CreateDefaultTenant(void)
{
	RMTenant tenant;

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	bool bret = doc.LoadJsonString(Tenant_Return, &err_code, &err_pos);

	assert(bret);
	JsonValue* root = doc.GetRoot();
	assert(NULL != root);
	assert(root->IsObject());

	tenant.ImportFromRMSResponse(root->AsObject());

	return tenant;
}

int TestTenant(void)
{
	RMTenant tenant = CreateDefaultTenant();



	cout << "Tenant ID:" << tenant.GetTenant() << endl;
	cout << "Tenant Router:" << tenant.GetRouterURL() << endl;
	cout << "Tenant RMS URL:" << tenant.GetRMSURL() << endl;
	cout << "export to string:" << tenant.ExportToString() << endl;
	HTTPRequest req = tenant.GetTenantQuery();
	printHttpRequest(req);
	cout << endl;

	tenant.SetRouter("https://rmtest.nextlabs.solutions");
	tenant.SetTenant("skydrm.com");

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile("RMSTenantTest.txt");
	if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
		return 1;
	}


	JsonValue* root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		return 2;
	}
	tenant.ImportFromRMSResponse(root->AsObject());

	cout << "Tenant ID:" << tenant.GetTenant() << endl;
	cout << "Tenant Router:" << tenant.GetRouterURL() << endl;
	cout << "Tenant RMS URL:" << tenant.GetRMSURL() << endl;
	cout << "export to string:" << tenant.ExportToString() << endl;

	printHttpRequest(tenant.GetTenantQuery());

	cout << endl;
	cout << endl;

	return 0;
}