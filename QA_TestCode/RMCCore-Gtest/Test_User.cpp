//
//  UserTest.cpp

//
//  Created by Jinli Xu on 1/10/18.
//  Copyright © 2018 NextLabs, Inc. All rights reserved.
//

#include "stdafx.h"
#include "rmccore\restful\rmuser.h"
#include "rmccore\network\httpClient.h"

#include "Header_Tenant.h"

using namespace RMCCORE;
using namespace std;

vector<vector<string>> csv_valid = readCSV("User_Login_Valid.csv");
vector<vector<string>> csv_noUser = readCSV("User_Login_NoUser.csv");

//Test Expire Time
TEST(ExpireTimeTest, ExpireTest) {
	try {
		JsonDocument doc;
		RMUser user = GetDefaultUser();
		int err_code = 0;
		size_t err_pos = 0;
		string user_str = user.ExportToString();
		if (!doc.LoadJsonString(user_str, &err_code, &err_pos)) {
			assert(true);
		}
		JsonValue *root = doc.GetRoot();
		
		uint64_t time = user.GetExpiredTime();
		bool expire = user.IsLoginExpired();

		cout << "Test Finished!" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(LogInFailTest, LoginFail) {
	try {
		JsonDocument doc;
		RMUser user;
		int err_code = 0;
		size_t err_pos = 0;
		HTTPRequest request =  user.GetUserLoginURL();
		string user_imt = ReadFromFile("Case7_login with not active account.json");
		if (!doc.LoadJsonString(user_imt, &err_code, &err_pos)) {
			assert(true);
		}
		JsonValue *root = doc.GetRoot();
		RetValue ret = user.ImportFromRMSResponse(root->AsObject());
		string user_ext = user.ExportToString();
		RetValue ret_import = user.ImportFromString(user_ext);
		cout << "123123" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(LogoutRequestTest,LogoutTest) {
	try {
		//RMUser user = GetDefaultUser();
		//string user_str = user.ExportToString();
		//RMUser user_import;
		//RetValue ret = user_import.ImportFromString(user_str);
		//string user_e = user_import.ExportToString();
		RMUser user;
		string user_str = ReadFromFile("MissingUser.json");
		JsonDocument doc;
		int err_code = 0;
		size_t err_pos = 0;
		if (!doc.LoadJsonString(user_str, &err_code, &err_pos)) {
			assert(true);
		}
		JsonValue *root = doc.GetRoot();
		RetValue ret =  user.ImportFromRMSResponse(root->AsObject());
		HTTPRequest request = user.GetUserLogoutQuery();
		EXPECT_STREQ(request.GetAcceptTypes()[0].c_str(), "application/json");
		//Check http header
		bool header_result = Check_HttpHeader(request, "text/csv", user);
		EXPECT_TRUE(header_result);
		//Check URL and Method
		bool urlmethod_result = Check_HttpURL_Method(request, "GET", "/rs/usr/logout", user);
		EXPECT_TRUE(urlmethod_result);

		cout << "Test Finished!" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}


// Test Pattern to test valid user with different input. -  Created by Jinli Xu on 1/10/18 - Update by Lifi.Yuan 3/2018
class ValidUserDifferentImport :public::testing::TestWithParam<vector<string>> {};
TEST_P(ValidUserDifferentImport, ValidUserInfo)
{
	try {
		//get parameter of file name, type is string
		vector<string> csv = GetParam();
		string caseID = csv[0];
		string jsonFile = csv[1];
		int importType = string2Int(csv[2]);
		cout << "---CaseID:[" << caseID << "], Input file: [" << jsonFile << "] ---" << endl;

		RMUser user;
		RMMembership* mem;

		int userId = 0;
		string userName = "";
		string email = "";
		int idpT = 0;
		string tenantId;
		string message;
		string id_m, defaultm_id;
		int type_m, defaultm_type;
		string tenantId_m, defaultm_tId;
		int projectId_m, defaultm_pId;

		// using rmccore lib to read and load json file, then call GetName(), GetUserID(), GetEmail(), GetIdpType() to return userName, userId, email, idpType.

		//user = importUser(jsonFile, importType);
		user = importUser58(jsonFile, importType);


		string clientid = GenerateClientID();
		RMTenant tenant = CreateDefaultTenant();
		//HTTPRequest request = user.GetUserLoginURL(clientid, tenant);
		HTTPRequest request = user.GetUserLoginURL();
		cout << "---HttpRequest: GetUserLoginURL---" << endl;
		printHttpRequest(request);
		cout << "-----------------" << endl;

		// using 3rd party Jsoncpp to read and get json value
		Json::Value jroot;
		jroot = ParseJsonFile(jsonFile);
		//Read sub node: userId, userName, email and idpType
		userId = jroot["extra"]["userId"].asInt();
		userName = jroot["extra"]["name"].asString();
		email = jroot["extra"]["email"].asString();
		idpT = jroot["extra"]["idpType"].asInt();
		tenantId = jroot["extra"]["tenantId"].asString();
		uint64_t serverTime = jroot["serverTime"].asInt64();
		uint64_t ttl = jroot["extra"]["ttl"].asInt64();
		uint64_t time = (ttl - serverTime)/1000;

		// Read array node
		std::cout << "Size of memberships: " << jroot["extra"]["memberships"].size() << endl;
		for (int i = 0; i < jroot["extra"]["memberships"].size(); i++)
		{
			id_m = jroot["extra"]["memberships"][i]["id"].asString();
			type_m = jroot["extra"]["memberships"][i]["type"].asInt();
			tenantId_m = jroot["extra"]["memberships"][i]["tenantId"].asString();
			projectId_m = jroot["extra"]["memberships"][i]["projectId"].asInt();

			// get default membership
			if (tenantId_m == tenantId)
			{
				defaultm_tId = tenantId_m;
				defaultm_id = id_m;
				defaultm_type = type_m;
				defaultm_pId = projectId_m;

				// Compare default membership 
				EXPECT_STREQ(defaultm_id.c_str(), user.GetDefaultMembership().GetID().c_str());
				EXPECT_STREQ(defaultm_tId.c_str(), user.GetDefaultMembership().GetTenantID().c_str());
				EXPECT_EQ(defaultm_type, user.GetDefaultMembership().GetIdpType());
				EXPECT_EQ(defaultm_pId, user.GetDefaultMembership().GetProjectID());
			}

			// call FindMembership () to get all other specific tenantId, return all membership infomation
			//cout << "get mem by FindMembership()_begin" << endl;
			mem = user.FindMembership(tenantId_m);
			//cout << "get mem by FindMembership()_end" << endl;
			// compare id,type, tenantId and projectId 
			if (mem != NULL) {
				EXPECT_STREQ(id_m.c_str(), mem->GetID().c_str());
				EXPECT_EQ(type_m, mem->GetIdpType());
				EXPECT_STREQ(tenantId_m.c_str(), mem->GetTenantID().c_str());
				EXPECT_EQ(projectId_m, mem->GetProjectID());
			}
			else
				cout << "FindMembership()_return NULL" << endl;
			//FindMenbership with wildcard
			if (i == 0) {
				int len = tenantId_m.length() - 1;
				string t0 = tenantId_m;
				string t = tenantId_m.erase(len);
				string tenantId_m_changed[] = { t, t + "*", t + "?", t0 + t0, "", "NULL", "**", "*", t0 + "||" + t };

				for (string id : tenantId_m_changed)
				{
					EXPECT_EQ(0, ifExist_Membership(user, id));
				}
			}
		}

		cout << "Debug: GetMembershipQuery()_begin" << endl;
		//request = user.GetMembershipQuery(clientid, "this computer", tenant, user.GetDefaultMembership());
		request = user.GetMembershipQuery(user.GetDefaultMembership());
		cout << "---HttpRequest: GetMembershipQuery---" << endl;
		printHttpRequest(request);
		cout << "-----------------" << endl;

		// Check if userName, Email, userId, IdpType is same
		EXPECT_EQ(userId, user.GetUserID());
		EXPECT_STREQ(userName.c_str(), user.GetName().c_str());
		EXPECT_STREQ(email.c_str(), user.GetEmail().c_str());
		EXPECT_EQ(idpT, user.GetIdpType());
		EXPECT_EQ(time, user.GetExpiredTime());


		cout << "Test finished." << endl;
	}
	catch (exception e)
	{
		std::cout << "Exception: " << e.what() << endl;
	}

}
INSTANTIATE_TEST_CASE_P(RMUserTesting, ValidUserDifferentImport, testing::ValuesIn(csv_valid));

// Test Pattern to test invalid user with different input -  Created by Jinli Xu on 1/10/18 - Update by Lifi.Yuan 3/2018
class InvalidUserParamTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(InvalidUserParamTest, NoUser) {
	//get parameter of file name, type is string
	vector<string> csv = GetParam();
	string caseID = csv[0];
	string jsonFile = csv[1];
	int importType = string2Int(csv[2]);
	cout << "---CaseID:[" << caseID << "], Input file: [" << jsonFile << "] ---" << endl;

	RMUser user;

	user = importUser(jsonFile, importType);

	EXPECT_EQ(0, user.GetUserID());
	EXPECT_STREQ("", user.GetName().c_str());
	EXPECT_STREQ("", user.GetEmail().c_str());
	EXPECT_EQ(0, user.GetIdpType());

}
INSTANTIATE_TEST_CASE_P(RMUserTesting, InvalidUserParamTest, testing::ValuesIn(csv_noUser));