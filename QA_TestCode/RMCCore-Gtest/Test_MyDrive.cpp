#include "stdafx.h"
#include "RMCCORE/restful/rmmydrive.h"
#include "Header_Token.h"

using namespace RMCCORE;
using namespace std;


//Test MyDrive 
TEST(MyDriveTest, MyDriveTest) {
	try {
		int user_id;
		int usage;
		int quota;
		string file_name = default_file_path + "RMMyDrive.txt";
		RMUser	  user = GetDefaultUser();
		RMMyDrive * mydrive = user.GetMyDrive();
		assert(NULL != mydrive);
		HTTPRequest	request = mydrive->GetStorageQuery();
		bool header_result = Check_HttpHeader(request, "application/json", user);
		EXPECT_TRUE(header_result);
		bool urlmethod_result = Check_HttpURL_Method(request, "POST", "/rs/myDrive/getUsage", user);
		EXPECT_TRUE(urlmethod_result);
		user_id = user.GetUserID();
		std::stringstream ss;
		std::string str;
		ss << user_id;
		ss >> str;
		string url = "{\"parameters\":{\"userId\":" + str + ",\"ticket\":\"1514993ABFEC88772DE337AB3C722E33\"}}";
		bool body_result = Check_Http_Body(request, url);
		EXPECT_TRUE(body_result);
		printHttpRequest(request);

		JsonDocument doc;
		int err_code = 0;
		size_t err_pos = 0;
		Json::Value jroot;

		const std::string jsonstr = ReadFromFile(file_name);
		assert(doc.LoadJsonString(jsonstr, &err_code, &err_pos));

		RetValue retv = mydrive->ImportFromRMSResponse(jsonstr);
		if (retv.GetCode() == 0) {
			jroot = ParseJsonFile(file_name);
			usage = jroot["results"]["usage"].asInt();
			quota = jroot["results"]["quota"].asInt();
			EXPECT_EQ(usage, mydrive->GetUsage());
			EXPECT_EQ(quota, mydrive->GetQuota());
			cout << "My drive usage:" << mydrive->GetUsage() << endl;
			cout << "Total Quota:" << mydrive->GetQuota() << endl;
		}
		else {
			cout << "Import failed!" << endl;
			EXPECT_TRUE(0);
		}

		cout << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Test Http code
class MyDriveHttpCodeTest : public::testing::TestWithParam<vector<string>> {};
TEST_P(MyDriveHttpCodeTest, HttpCodeTest) {
	try {
		Json::Value jroot;
		int base_code = 61440;
		vector<string> para = GetParam();
		string file_name = default_file_path + para[2];
		jroot = ParseJsonFile(file_name);
		int http_code = jroot["statusCode"].asInt();
		string message = jroot["message"].asString();
		RMUser	  user = GetDefaultUser();
		RMMyDrive * mydrive = user.GetMyDrive();
		string file_str = ReadFromFile(file_name);
		RetValue ret = mydrive->ImportFromRMSResponse(file_str);
		EXPECT_EQ(ret.GetCode(), base_code + http_code);
		EXPECT_STREQ(ret.GetMessage(), message.c_str());

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> csv = readCSV(default_file_path + "MyDriveHttpCodeTest.csv");
INSTANTIATE_TEST_CASE_P(MyDriveHttpCodeTestReturn, MyDriveHttpCodeTest, testing::ValuesIn(csv));