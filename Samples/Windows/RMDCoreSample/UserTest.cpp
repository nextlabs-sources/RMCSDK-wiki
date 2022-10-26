#include "stdafx.h"
#include "TenantTest.h"
#include "UserTest.h"
#include "HttpReqest.h"

#include "rmccore\rmccore.h"
#include "common.h"
#include "rmccore\restful\rmuser.h"
#include "rmccore/network/httpClient.h"
#include "rmccore/restful/rmsyspara.h"
#include <assert.h>

using namespace std;
using namespace RMCCORE;


void printMembership(RMMembership & mem)
{
	cout << "Member ID:" << mem.GetID() << endl;
	cout << "Member tenant ID: " << mem.GetTenantID() << endl;
	cout << "Member Project ID:" << mem.GetProjectID() << endl;
	cout << "Member IDP Type: " << mem.GetIdpType() << endl;
}

void printIDPType(int type)
{
	// 	typedef enum _USERIDPTYPE {
	// 		SKYDRM = 0,
	// 		SAML,
	// 		GOOGLE,
	// 		FACEBOOK,
	// 		YAHOO
	// 	} USER_IDPTYPE;

	switch (type)
	{
	case 0:
		cout << " SkyDRM";
		break;
	case 1:
		cout << " SAML";
		break;
	case 2:
		cout << " Google";
		break;
	case 3:
		cout << " FaceBook";
		break;
	case 4:
		cout << " Yahoo";
		break;
	default:
		cout << " Unknown";
		break;
	}
}

#define DEFAULT_USER_PROFILE		"DUserData1.txt"

const RMUser& GetDefaultUser(void) 
{
	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile(DEFAULT_USER_PROFILE);
	if(!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
		throw;
	}

	JsonValue *root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		throw;
	}

	static RMUser user(root->AsObject());

	return user;
}

int TestUser()
{
	const std::string usersetting = ReadFromFile(DEFAULT_USER_PROFILE);
	
	if (usersetting.length() > 0) {//skip the user test when there is saved user setting.
		RMUser  user = GetDefaultUser();
		uint64_t utime = user.GetExpiredTime();
		cout << "User Session TTL (GetExpiredTime):" << utime << endl;

		bool bret = false;
		bret = user.IsLoginExpired();
		if (bret)
		{
			cout << "login expired" << endl;
			cout << "Press any key to continue...";
			_getch();
		}
	}

	std::string clientid = GenerateClientID();

	RMUser user(GenerateSystemParameter(), CreateDefaultTenant());

	cout << "Login User HTTP Request" << endl;
	HTTPRequest request = user.GetUserLoginURL();
	printHttpRequest(request);
	cout << endl;

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile("RMSBasicLogin.txt");
	if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
		return 1;
	}

	JsonValue *root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		return 2;
	}
	RetValue retv = user.ImportFromRMSResponse(root->AsObject());

	cout << "User Name:" << user.GetName() << endl;
	cout << "User Email:" << user.GetEmail() << endl;
	cout << "User ID:" << user.GetUserID() << endl;
	cout << "Login type:";
	printIDPType(user.GetIdpType());
	cout << endl << endl;

	
	cout << "Default membership information: " << endl;
	printMembership(user.GetDefaultMembership());

	std::string tenantid = "21b06c79-baab-419d-8197-bad3ce3f4476";
	RMMembership * mem = user.FindMembership(tenantid);
	if (NULL == mem) {
		cout << "Tenant " << tenantid << " not found." << endl;
	}
	else {
		cout << "Found Tenant " << tenantid << " Information" << endl;
		printMembership(*mem);
	}

	cout << endl;

	tenantid = "8360d053-9f7d-4bcc-a265-dab4a31a92fd";
	mem = user.FindMembership(tenantid);
	if (NULL == mem) {
		cout << "Tenant " << tenantid << " not found." << endl;
	}
	else {
		cout << "Found Tenant " << tenantid << " Information" << endl;
		printMembership(*mem);
	}
	cout << endl;

	tenantid = "8360d053-9f7d-4bcc-a265-bad3ce3f4476";
	mem = user.FindMembership(tenantid);
	if (NULL == mem) {
		cout << "Tenant " << tenantid << " not found." << endl;
	}
	else {
		cout << "Found Tenant " << tenantid << " Information" << endl;
		printMembership(*mem);
	}
	cout << endl;

	cout << "User data:" << user.ExportToString() << endl;

	cout << "Query membership info:" << endl;
	request = user.GetMembershipQuery(user.GetDefaultMembership());
	printHttpRequest(request);

	uint32_t option = 2;
	uint64_t start = (uint64_t)std::time(nullptr) * 1000;
	uint64_t end = start + 5 * 60 * 1000;
	request = user.GetUpdateUserPreferenceQuery(option, start, end);
	printHttpRequest(request);

	option = 3;
	request = user.GetUpdateUserPreferenceQuery(option, start, end);
	printHttpRequest(request);

	const std::string certstr = ReadFromFile("RMSCertsTest.txt");
	if (!doc.LoadJsonString(certstr, &err_code, &err_pos)) {
		return 1;
	}

	root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		return 2;
	}
	user.GetDefaultMembership().m_certificate.ImportFromRMSResponse(root->AsObject());


	cout << "User data:" << user.ExportToString() << endl;
	cout << endl;
	cout << endl;

	WriteToFile(DEFAULT_USER_PROFILE, user.ExportToString());

	cout << endl;
	cout << endl;

	request = user.GetBasicProfileQuery();
	printHttpRequest(request);

	const std::string basicprofile = ReadFromFile("RMSUserBasic.txt");
	user.ImportFromRMSResponse(basicprofile);
	cout << "User Name:" << user.GetName() << endl;
	cout << "User Email:" << user.GetEmail() << endl;
	cout << endl << endl;

	// BasicProfile.txt
	const std::string basicprofile1 = ReadFromFile("BasicProfile.txt");
	user.ImportFromRMSResponse(basicprofile1);
	cout << "User Name:" << user.GetName() << endl;
	const std::vector<std::string>vec = user.GetEmails();
	for (std::string email : vec)
		cout << "User Email:" << email << endl;
	cout << endl << endl;

	request = user.GetProfileQuery();
	printHttpRequest(request);

	const std::string profile = ReadFromFile("RMSUserProfile.txt");
	user.ImportFromRMSResponse(profile);
	cout << "User Name:" << user.GetName() << endl;
	cout << "User Email:" << user.GetEmail() << endl;
	cout << "Default membership information: " << endl;
	printMembership(user.GetDefaultMembership());
	cout << endl << endl;


	return 0;

}