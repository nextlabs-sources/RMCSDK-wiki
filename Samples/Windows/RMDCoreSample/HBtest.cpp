#include "stdafx.h"
#include "HBtest.h"
#include "UserTest.h"
#include "common.h"
#include "HttpReqest.h"
#include "rmccore/restful/rmheartbeat.h"
#include "rmccore/restful/rmuser.h"

using namespace RMCCORE;
using namespace std;

void TestHeartbeat()
{
	RMUser user = GetDefaultUser();

	cout << "Initial Heartbeat query:" << endl;
	RMHeartbeat heartbeat;
	HTTPRequest initialquery = user.GetHeartBeatQuery(heartbeat);
	printHttpRequest(initialquery);
	cout << endl;

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile("RMHeartbeatData.txt");

	RetValue retv = heartbeat.ImportFromRMSResponse(jsonstr);

	Watermark watermark = heartbeat.GetDefaultWatermarkSetting();
	

	cout << "Heartbeat data :" << endl;
	cout << "\tfrequency:" << heartbeat.GetFrequency() << endl;
	cout << "\tdefault watermark:" << watermark.ExportToString() << endl;
	cout << "\tnumber of policy:" << heartbeat.GetPolicyConfigCount() << endl;
	cout << endl;

	policymap policyconfigs = heartbeat.GetAllPolicyConfigs();
	RMPolicyConfig policyConfig;
	std::string policybundle;
	std::string tenantid = heartbeat.GetPolicyConfigTenantID(0);
	bool bval = heartbeat.GetPolicyConfig(tenantid, policyConfig);
	policybundle = policyConfig.GetPolicyBundle();
	cout << "\t tenantid:" << tenantid << endl;
	cout << "\t " << policybundle << endl;
	cout << endl;

	std::vector<CLASSIFICATION_CAT> clasi_cat;
	CLASSIFICATION_CAT classi;
	size_t count = heartbeat.GetPolicyConfigCount();

	for (int i = 0; i < count; i++)
	{
		tenantid = heartbeat.GetPolicyConfigTenantID(i);
		bval = heartbeat.GetPolicyConfig(tenantid, policyConfig);
		clasi_cat = policyConfig.GetClassificationCategories();
		if (clasi_cat.size() > 0)
		{
			classi = clasi_cat[0];
		}
	}
	
}