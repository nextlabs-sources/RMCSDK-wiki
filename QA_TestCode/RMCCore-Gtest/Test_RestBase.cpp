//
//  RestBaseTest.cpp

//
//  Created by Jinli Xu on 1/4/18.
//  Copyright © 2018 NextLabs, Inc. All rights reserved.
//

#include "stdafx.h"
#include "RMCCORE\restful\rmuser.h"

using namespace RMCCORE;
using namespace std;


/*class JsonParse {
	void JsonRead(string path) {
		Json::Reader reader;
		Json::Value root;
		ifstream is;
		is.open(path, ios::binary);
		if (reader.parse(is, root))
		{
			//Read root node
			string status = root["statusCode"].asString();
			string message = root["message"].asString();
			long long serverTime = root["serverTime"].asLargestInt();
			cout << "statusCode: " << status << endl;
			cout << "mesage: " << message << endl;
			cout << "serverTime: " << serverTime << endl;

			//Read sub node
			int userId = root["extra"]["userId"].asInt();
			string ticket = root["extra"]["ticket"].asString();
			string tenantId = root["extra"]["tenantId"].asString();
			long long ttl = root["extra"]["ttl"].asLargestInt();
			string name = root["extra"]["name"].asString();
			string email = root["extra"]["email"].asString();
			int idpT = root["extra"]["idpType"].asInt();
			cout << "userId: " << userId << endl;
			cout << "ticket: " << ticket << endl;
			cout << "tenantId: " << tenantId << endl;
			cout << "ttl: " << ttl << endl;
			cout << "name: " << name << endl;
			cout << "email: " << email << endl;
			cout << "idpT: " << idpT << endl;

			// Read array node
			cout << "Size of memberships: " << root["extra"]["memberships"].size() << endl;
			for (int i = 0; i < root["extra"]["memberships"].size(); i++)
			{
				string id_m = root["extra"]["memberships"][i]["id"].asCString();
				int type_m = root["extra"]["memberships"][i]["type"].asInt();
				string tenantId_m = root["extra"]["memberships"][i]["tenantId"].asString();
				int projectId_m = root["extra"]["memberships"][i]["projectId"].asInt();
				cout << "id_m: " << id_m << endl;
				cout << "type_m: " << type_m << endl;
				cout << "tenantId_m: " << tenantId_m << endl;
				cout << "projectId_m: " << projectId_m << endl;
			}
			cout << endl;
			cout << "Reading complete! " << endl;
		}
		is.close();
		getchar();
	}
};*/

/*class RMCCoreEnvironment : public testing::Environment
{
public:
	virtual void SetUp()
	{
		tenant = new RMCCORE::RMTenant;
		user = new RMCCORE::RMUser;
	}
	virtual void TearDown()
	{
		delete tenant;
		delete user;
	}
};*/
//Test Fixture setup and teardown
/*class UserTest : public testing::Test {

		static void SetUpTestCase() {
			tenant = new RMCCORE::RMTenant;
			user = new RMCCORE::RMUser;

		}
		static void TearDownTestCase() {
			delete tenant;
			delete user;
		}

};*/


//TEST(InitiateTest, NUllValue) {
//	JsonDocument doc;
//	int err_code = 0;
//	size_t err_pos = 0;
//	string jsonstr = ReadFromFile("fail2.json");
//	doc.LoadJsonString(jsonstr, &err_code, &err_pos);
//	JsonValue * root = doc.GetRoot();
//	std::cout << "Root size is: " <<root<< endl;
//	EXPECT_FALSE(root);
//}
//
//TEST(InitiateTest, NotObject) {
//	JsonDocument doc;
//	int err_code = 0;
//	size_t err_pos = 0;
//	string jsonstr = ReadFromFile("fail_test_array_01.json");
//	doc.LoadJsonString(jsonstr, &err_code, &err_pos);
//	JsonValue * root = doc.GetRoot();
//	cout << "IsObject return value: " << root->IsObject() << endl;
//	EXPECT_FALSE(root->IsObject());
//}

// Test return error code for RMCCORE_ERROR_INVALID_DATA, RMCCORE_INVALID_JSAON_FORMAT
class ImportStringParamTest :public::testing::TestWithParam<string> {};
//TEST_P(ImportStringParamTest, InvalidJsonString)
//{
//	string para = GetParam();
//	RMUser user;
//	string jsonstr = ReadFromFile("successlogin.json");
//	try {
//		RetValue ret = user.ImportFromString(jsonstr);
//		EXPECT_TRUE(ret.GetCode());
//		EXPECT_EQ(131, ret.GetCode());
//	}
//	catch (...)
//	{
//		std::cout << "Exception: " << endl;
//	}
//
//}
//INSTANTIATE_TEST_CASE_P(InvalidJsonStringReturn, ImportStringParamTest, testing::Values("NormalString.json","MissingPart.json","test_array_01.json",
//	"fail_test_array_01.json","fail33.json","pass1.json"));

//TEST(ImportStringTest, ValidJsonResponse)
//{
//	RMUser user;
//	string jsonstr = ReadFromFile("successlogin.json");
//	try {
//		RetValue ret = user.ImportFromString(jsonstr);
//		EXPECT_TRUE(ret.GetCode());
//	}
//	catch (...)
//	{
//		std::cout << "Exception: " << endl;
//	}
//
//}

//// Test return error code for RMCCORE_ERROR_INVALID_DATA
//class ImportJsonParamTest :public::testing::TestWithParam<string> {};
//TEST_P(ImportJsonParamTest, MissingLoginElement) {
//	string para = GetParam();
//	RMUser user;
//	JsonDocument doc;
//	JsonValue* root;
//	int err_code = 0;
//	size_t err_pos = 0;
//	try {
//		string jsonstr = ReadFromFile(para);
//		doc.LoadJsonString(jsonstr, &err_code, &err_pos);
//		root = doc.GetRoot();
//		RetValue ret = user.ImportFromJson(root->AsObject());
//		EXPECT_EQ(131, ret.GetCode());
//	}
//	catch (ios_base::failure e)
//	{
//		cout << "IO error!" << endl;
//	}
//	catch (...)
//	{
//		cout << "Other exceptions!" << endl;
//	}
//	
//}
//INSTANTIATE_TEST_CASE_P(MissingLoginElementReturn, ImportJsonParamTest, testing::Values("MissingUserValue.json", "MissingUser.json","MissingTenantIdValue.json",
//	"MissingTicketValue.json","MissingMembership.json","MissingExtra.json","MissingDefaultMem.json","ChangeDefaultTen.json","ChangeDefaultTenInMem.json"));

//TEST(ImportJsonResponseTest, ValidData) 
//{
//	RMUser user;
//	JsonDocument doc;
//	JsonValue* root;
//	int err_code = 0;
//	size_t err_pos = 0;
//	try {
//		string jsonstr = ReadFromFile("successlogin.json");
//		doc.LoadJsonString(jsonstr, &err_code, &err_pos);
//		root = doc.GetRoot();
//		RetValue ret = user.ImportFromJson(root->AsObject());
//		EXPECT_EQ(131, ret.GetCode());
//	}
//	catch(...) 
//	{
//		cout << "Other Exceptions!" << endl;
//	}
//}






/*int _tmain(int argc, _TCHAR* argv[])
{
	//testing::AddGlobalTestEnvironment(new RMCCoreEnvironment);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}*/

