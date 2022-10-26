#include "stdafx.h"
#include "rmccore\restful\rmrecipients.h"
#include "RecipTest.h"
#include "common.h"

#include <assert.h>

using namespace std;
using namespace RMCCORE;

void TestRecipients()
{
	RMRecipients recipients;

	RMRecipientList list{ "test@test.com", "test1@test.com", "test2@test.com", "test2@nextlabs.com" };
	RMRecipientList addlist{ "test2@test.com", "test1@nextlabs.com", "test3@nextlabs.com" };
	RMRecipientList removelist{ "test3@test.com", "test2@nextlabs.com", "test@test.com" };

	recipients.AddRecipients(list);
	assert(recipients.NeedUpdateRMS());

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;
	const std::string jsonstr = ReadFromFile("RMSRecipients.txt");
	if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
		assert(true);
	}
	JsonValue *root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		assert(true);
	}
	RetValue retv = recipients.ImportFromRMSResponse(root->AsObject());
	assert(retv);
	assert(!recipients.NeedUpdateRMS());

	uint8_t ret = recipients.AddRecipients(addlist);
	assert(ret == 2);
	ret = recipients.RemoveRecipients(removelist);
	assert(ret == 2);
	ret = recipients.AddRecipients({ "test@test.com" });
	assert(ret == 1);
	ret = recipients.RemoveRecipients({ "test3@nextlabs.com" });
	assert(ret == 1);

	assert(recipients.NeedUpdateRMS());
	cout << endl;
	cout << endl;
	cout << "Recipients: " << recipients.ExportToString() << endl;

	const std::string jsonstr1 = ReadFromFile("RMSRecipients1.txt");
	if (!doc.LoadJsonString(jsonstr1, &err_code, &err_pos)) {
		assert(true);
	}
	root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		assert(true);
	}
	retv = recipients.ImportFromRMSResponse(root->AsObject());
	assert(!recipients.NeedUpdateRMS());

	cout << endl;
	cout << endl;
	cout << "Recipients: " << recipients.ExportToString() << endl;

}