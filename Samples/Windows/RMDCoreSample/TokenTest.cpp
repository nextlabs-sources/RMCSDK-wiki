#include "stdafx.h"
#include "TokenTest.h"

#include "rmccore\rmccore.h"
#include "common.h"
#include "rmccore\restful\rmtoken.h"
#include "HttpReqest.h"
#include "TenantTest.h"
#include "UserTest.h"


using namespace std;
using namespace RMCCORE;

void printToken(RMToken & token)
{
	cout << "Token " << token.GetDUID() << ":" << token.GetKey() << endl;
}


#define DEFAULT_TOKEN_POOL_FILENAME		"DefaultTokens.txt"

RMTokenPool & GetDefaultTokens()
{
	static RMTokenPool tokens;
	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile(DEFAULT_TOKEN_POOL_FILENAME);
	if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
		throw;
	}

	JsonValue *root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		throw;
	}

	tokens.ImportFromJson(root->AsObject());

	return tokens;
}

int TestToken()
{
	std::string clientid = GenerateClientID();

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	//	create a user class first
	RMUser user = GetDefaultUser();
	cout << "Import basic user login" << endl;
	RMTenant tenant = CreateDefaultTenant();
	HTTPRequest member_request = user.GetMembershipTokenQuery(user.GetDefaultMembership(), 100);
	printHttpRequest(member_request);

	RMTokenPool tpool;

	const std::string tokenstr = ReadFromFile("RMSTokenTest.txt");
	if (!doc.LoadJsonString(tokenstr, &err_code, &err_pos)) {
		return 1;
	}

	JsonValue * root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		return 2;
	}

	tpool.ImportFromRMSResponse(root->AsObject());
	cout << "Imported " << tpool.size() << " tokens" << endl;

	for (int i = 0; i < 10; i++) {
		RMToken token = tpool.pop();
		cout << "Remove Token :" << token.GetKey() << endl;
	}
	cout << endl;

	const std::string tokenstr1 = ReadFromFile("RMSTokenTest1.txt");
	if (!doc.LoadJsonString(tokenstr1, &err_code, &err_pos)) {
		return 1;
	}

	root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		return 2;
	}

	tpool.ImportFromRMSResponse(root->AsObject());
	cout << "Import another set of tokens" << endl;
	cout << "Total " << tpool.size() << " tokens" << endl;
	cout << endl;

	WriteToFile(DEFAULT_TOKEN_POOL_FILENAME, tpool.ExportToString());

	return 0;

}