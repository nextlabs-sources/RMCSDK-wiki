#include "stdafx.h"
#include "rmccore\restful\rmnxlfile.h"
#include "rmccore\restful\rmuser.h"
#include "rmccore\restful\rmheartbeat.h"
#include "Header_HttpRequest.h"
#include "rmccore\format\nxlfile.h"
#include "Header_Tenant.h"
#include "Header_File.h"
#include "time.h"
#include <ctime>

using namespace RMCCORE;
using namespace std;
using  namespace OBLIGATION;


//Test HeartBeat
TEST(HeartbeatRequestTest, HeartbeatRequestTest) {
	try {
		RMUser user = GetDefaultUser();
		bool result = user.IsLoginExpired();
		string file_name1 = default_file_path + "HeartBeatTestTotal2.txt";
		string file_name2 = default_file_path + "HeartBeatTest2.txt";
		RMHeartbeat hb = RMHeartbeat();
		RMPolicyConfig policy_conf;
		RMPolicyConfig policy_conf1;
		JsonDocument doc;
		JsonValue* root;
		int err_code = 0;
		size_t err_pos = 0;

		//Check Http Request
		HTTPRequest request = user.GetHeartBeatQuery(hb);
		bool header_result = Check_HttpHeader(request, "application/json", user);
		EXPECT_TRUE(header_result);
		bool urlmethod_result = Check_HttpURL_Method(request, "POST", "/rs/v2/heartbeat", user);
		EXPECT_TRUE(urlmethod_result);
		bool body_result = Check_Http_Body(request, "{\"parameters\":{\"platformId\":4,\"clientData\":[]}}");
		EXPECT_TRUE(body_result);

		//Check GetDefaultWatermarkSetting,GetFrequency,GetPolicyConfig,GetPolicyConfigCount
		std::string str = ReadFromFile(file_name1);
		Json::Value jroot_result;
		jroot_result = ParseJsonFile(file_name1);
		int  frequency = jroot_result["results"]["heartbeatFrequency"].asInt();

		assert(doc.LoadJsonString(str, &err_code, &err_pos));
		root = doc.GetRoot();
		RetValue ret1 = hb.ImportFromRMSResponse(root->AsObject());
		RetValue ret = hb.ImportFromRMSResponse(str);
		if ((ret1.GetCode()) == 0 && (ret.GetCode()) == 0) {
			Watermark watermark = hb.GetDefaultWatermarkSetting();
			string font_color = watermark.GetFontColor();
			EXPECT_STREQ(font_color.c_str(), "#008000");
			string font_name = watermark.getFontName();
			EXPECT_STREQ(font_name.c_str(), "Arial Unicode MS");
			int font_size = watermark.getFontSize();
			EXPECT_EQ(font_size, 26);
			string name = watermark.getName();
			EXPECT_STREQ(name.c_str(), "WATERMARK");
			bool repeaet = watermark.getRepeat();
			EXPECT_TRUE(repeaet);
			Watermark::Rotation ration = watermark.getRotation();
			string rotation_str = watermark.getRotationString();
			int rotation_int;
			stringstream stream(rotation_str);
			stream >> rotation_int;
			switch (rotation_int)
			{
			case 0: rotation_str = "NOROTATION";  break;
			case 1: rotation_str = "CLOCKWISE";  break;
			case 2: rotation_str = "ANTICLOCKWISE";  break;
			default: rotation_str = "NOROTATION";  break;
			}
			EXPECT_STREQ(rotation_str.c_str(), "ANTICLOCKWISE");
			string text = watermark.getText();
			EXPECT_STREQ(text.c_str(), "$(User)\n$(Date) $(Time)");
			int transpa = watermark.getTransparency();
			EXPECT_EQ(transpa, 70);
			EXPECT_EQ(frequency, hb.GetFrequency());
			string tenant_str;
			bool pc;
			for (int i = 0; i < jroot_result["results"]["policyConfigData"].size(); i++) {
				tenant_str = jroot_result["results"]["policyConfigData"][i]["tenantName"].asString();
				pc = hb.GetPolicyConfig(tenant_str, policy_conf);
				EXPECT_TRUE(pc);
			}

			size_t size = hb.GetPolicyConfigCount();
			int size_config = jroot_result["results"]["policyConfigData"].size();
			if (size_config == size) {
				for (int i = 0; i < jroot_result["results"]["policyConfigData"].size(); i++) {
					string policy_tenant = jroot_result["results"]["policyConfigData"][i]["tenantName"].asString();
					EXPECT_STREQ(policy_tenant.c_str(), hb.GetPolicyConfigTenantID(i).c_str());
				}
			}
			else {
				EXPECT_EQ(size_config, size);
			}

			EXPECT_STREQ("", hb.GetPolicyConfigTenantID(10).c_str());
		}
		else {
			cout << "Import failed!" << endl;
			EXPECT_TRUE(0);
		}

		//Request server the second time
		HTTPRequest request1 = user.GetHeartBeatQuery(hb);
		string body_str = ReadFromFile(default_file_path + "HeartBeat3.txt");
		bool body_result1 = Check_Http_Body(request1, body_str);
		EXPECT_TRUE(body_result1);
		string str2 = ReadFromFile(file_name1);
		RetValue ret2 = hb.ImportFromRMSResponse(str2);
		int config_count = hb.GetPolicyConfigCount();
		string tenant_str1;

		if ((ret2.GetCode()) == 0) {
			//Check policy config
			Json::Value jroot_result1;
			jroot_result1 = ParseJsonFile(file_name1);
			string bundle;
			bool watermark_result;
			uint64_t modified;
			bool pc_result;
			for (int i = 0; i < jroot_result1["results"]["policyConfigData"].size(); i++) {
				tenant_str1 = jroot_result1["results"]["policyConfigData"][i]["tenantName"].asString();
				pc_result = hb.GetPolicyConfig(tenant_str1, policy_conf1);
				EXPECT_TRUE(pc_result);
				bundle = jroot_result1["results"]["policyConfigData"][i]["policyBundle"].asString();
				EXPECT_STREQ(bundle.c_str(), policy_conf1.GetPolicyBundle().c_str());
				watermark_result = policy_conf1.HasWatermarkPolicy();
				EXPECT_FALSE(watermark_result);
				Watermark water = policy_conf1.GetWatermarkConfig();
				uint64_t modified = policy_conf1.GetPolicyLastModify();
				EXPECT_EQ(modified, 1522831449421);
				uint64_t stamp = policy_conf1.GetPolicyBundleTimeStamp();
				EXPECT_EQ(stamp, 1522831307024);
			}
		}
		else {
			cout << "Import failed!" << endl;
			EXPECT_TRUE(0);
		}

		cout << "Test Finished!" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
	}
}

//Test HeartBeat without policyConfigData
TEST(HeartbeatWithoutPolicyTest, WithoutPolicyTest) {
	try {
		RMUser user = GetDefaultUser();
		string file_name = default_file_path + "HeartBeatWithoutPolicy.txt";
		RMHeartbeat hb = RMHeartbeat();
		RMPolicyConfig policy_conf;
		JsonDocument doc;
		JsonValue* root;
		int err_code = 0;
		size_t err_pos = 0;

		//Check Http Request
		HTTPRequest request = user.GetHeartBeatQuery(hb);
		bool header_result = Check_HttpHeader(request, "application/json", user);
		EXPECT_TRUE(header_result);
		bool urlmethod_result = Check_HttpURL_Method(request, "POST", "/rs/v2/heartbeat", user);
		EXPECT_TRUE(urlmethod_result);
		bool body_result = Check_Http_Body(request, "{\"parameters\":{\"platformId\":4,\"clientData\":[]}}");
		EXPECT_TRUE(body_result);

		//Check GetDefaultWatermarkSetting,GetFrequency,GetPolicyConfig,GetPolicyConfigCount
		std::string str = ReadFromFile(file_name);
		Json::Value jroot_result;
		jroot_result = ParseJsonFile(file_name);
		int  frequency = jroot_result["results"]["heartbeatFrequency"].asInt();

		RetValue ret = hb.ImportFromRMSResponse(str);

		EXPECT_FALSE(ret.GetCode());

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
	}
}

//Test 100 policyConfig
TEST(HeartBeatMaxPolicyTest, MaxPolicyTest) {
	try {
		RMUser user = GetDefaultUser();
		user.GetExpiredTime();
		bool result = user.IsLoginExpired();
		string file_name = default_file_path + "HeartBeat110.txt";
		Json::Value jroot;
		RMHeartbeat hb = RMHeartbeat();
		RMPolicyConfig policy_conf;
		std::string str = ReadFromFile(file_name);
		jroot = ParseJsonFile(file_name);
		int size_jroot = jroot["results"]["policyConfigData"].size();
		RetValue ret = hb.ImportFromRMSResponse(str);
		int size = hb.GetPolicyConfigCount();
		EXPECT_EQ(size, size_jroot);
		cout << "Test Finished!" << endl;

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
	}
}

//Test HttpCode
class HeartBeatHttpCodeTest : public::testing::TestWithParam<vector<string>> {};
TEST_P(HeartBeatHttpCodeTest, HttpCodeTest) {
	try {
		RMHeartbeat hb = RMHeartbeat();
		Json::Value jroot;
		int base_code = 61440;
		vector<string> para = GetParam();
		string file_name = para[2];
		jroot = ParseJsonFile(default_file_path + file_name);
		int http_code = jroot["statusCode"].asInt();
		string message = jroot["message"].asString();
		string str = ReadFromFile(default_file_path + file_name);
		RetValue ret = hb.ImportFromRMSResponse(str);
		EXPECT_EQ(ret.GetCode(), base_code + http_code);
		cout << "Test Finished!" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
	}
}
vector<vector<string>> csv1 = readCSV(default_file_path +"HeartBeat_HttpCode.csv");
INSTANTIATE_TEST_CASE_P(HeartBeatHttpCodeTestReturn, HeartBeatHttpCodeTest, testing::ValuesIn(csv1));