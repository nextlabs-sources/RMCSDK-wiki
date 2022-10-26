//
//  TennatTest.cpp

//
//  Created by Jinli Xu on 1/10/18.
//  Copyright @2018 NextLabs, Inc. All rights reserved.
//

#include "stdafx.h"
#include "RMCCORE\restful\rmtenant.h"

using namespace RMCCORE;
using namespace std;

//#define Tenant_Reg ("https://(.*)\.");

#define Skydrm_Tenant_Return	"{\"statusCode\":200,\"message\":\"OK\",\"serverTime\":1512505538781,\"results\":{\"server\":\"https://skydrm.com/rms\"}}"
#define RMTST_Tenant_Return   "{\"statusCode\":200,\"message\":\"OK\",\"serverTime\":1512505670638,\"results\":{\"server\":\"https://rmtest.nextlabs.solutions/rms\"}}"
#define DEFAULT_TENANT "skydrm.com"
#define DEFAULT_ROUTER "https://r.skydrm.com"

const RMTenant CreateDefaultTenant(void)
{
	RMTenant tenant;
	tenant.SetRouter("https://rmtest.nextlabs.solutions");
	tenant.SetTenant("skydrm.com");

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;
	assert(doc.LoadJsonString(RMTST_Tenant_Return, &err_code, &err_pos));
	JsonValue* root = doc.GetRoot();
	assert(NULL != root);
	assert(root->IsObject());
	tenant.ImportFromRMSResponse(root->AsObject());
	return tenant;
}

const RMTenant CreateTenant_perRMS(int RMS_ID)
{
	RMTenant tenant;

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	switch (RMS_ID)
	{
	case 0:
		assert(doc.LoadJsonString(Skydrm_Tenant_Return, &err_code, &err_pos));
		break;
	case 1:
		assert(doc.LoadJsonString(RMTST_Tenant_Return, &err_code, &err_pos));
		break;
	default:
		assert(doc.LoadJsonString(Skydrm_Tenant_Return, &err_code, &err_pos));
	}

	JsonValue* root = doc.GetRoot();
	assert(NULL != root);
	assert(root->IsObject());

	tenant.ImportFromRMSResponse(root->AsObject());

	return tenant;
}

// Default parameter and set parameter
class TenantSetParamTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(TenantSetParamTest, SetParam)
{
	string expect_tenant;
	string tenant_set;
	string expect_router;
	string router_set;
	string initial_tenant;
	string initial_router;
	string default_rmsURL = "https://www.skydrm.com/rms";
	try {
		vector<string> csv = GetParam();
		//Test default parameter
		initial_tenant = csv[2];
		initial_router = csv[3];
		tenant_set = csv[4];
		router_set = csv[5];
		expect_tenant = csv[6];
		expect_router = csv[7];
		RMTenant tenant(initial_router, initial_tenant);
		HTTPRequest req = tenant.GetTenantQuery();
		std::cout << "Tenant ID:" << tenant.GetTenant() << endl;
		std::cout << "RMS URL:" << tenant.GetRMSURL() << endl;
		std::cout << "Rourter URL:" << tenant.GetRouterURL() << endl;
		if (initial_tenant == "" && initial_router == "")
		{
			initial_tenant = DEFAULT_TENANT;
			initial_router = DEFAULT_ROUTER;
		}

		EXPECT_STREQ(initial_tenant.c_str(), tenant.GetTenant().c_str());
		EXPECT_STREQ(initial_router.c_str(), tenant.GetRouterURL().c_str());

		tenant.SetTenant(tenant_set);
		tenant.SetRouter(router_set);
		
		std::cout << "Tenant ID:" << tenant.GetTenant() << endl;
		std::cout << "RMS URL:" << tenant.GetRMSURL() << endl;
		std::cout << "Rourter URL:" << tenant.GetRouterURL() << endl;

		EXPECT_STREQ(expect_tenant.c_str(), tenant.GetTenant().c_str());
		EXPECT_STREQ(expect_router.c_str(), tenant.GetRouterURL().c_str());

		JsonValue* jv;
		jv = tenant.ExportToJson();
		if (!(NULL != jv && jv->IsObject())) {
			throw;
		}
		jv->AsObject();
		RetValue  ret = tenant.ImportFromJson(jv->AsObject());
		EXPECT_EQ(ret.GetCode(),0);

		std::cout << "Test finished." << tenant.ExportToString()<< endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> vf_setp = readCSV("TenantSetParamTest.csv");
INSTANTIATE_TEST_CASE_P(TenantSetParamTestReturn, TenantSetParamTest, testing::ValuesIn(vf_setp));

//Test invalid para
class InvalidTenantParaTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(InvalidTenantParaTest, InvalidPara) {
	string tenant_name = DEFAULT_TENANT;
	string router = DEFAULT_ROUTER;
	string file_name;
	string server_URL;
	int expect_code;
	string expect_message;
	JsonDocument doc;
	JsonValue* root;
	int code;
	int base_code = 61440;
	string message;
	int err_code = 0;
	size_t err_pos = 0;
	try {
		vector<string> csv = GetParam();
		file_name = csv[2];
		server_URL = csv[3];
		string rmsURL = "https://www.skydrm.com/rms";;
		expect_message = csv[4];
		expect_code = const_str_int(csv[5]);
		RMTenant tenant;
		Json::Value jroot;
		jroot = ParseJsonFile(file_name);
		if (server_URL == "yes" && expect_code == 0) {
			rmsURL = jroot["results"]["server"].asString();
		}
		string jsonstr = ReadFromFile(file_name);
		std::cout << "jsonstr length is: " << jsonstr.length() << endl;
		doc.LoadJsonString(jsonstr, &err_code, &err_pos);
		root = doc.GetRoot();
		RetValue ret = tenant.ImportFromRMSResponse(root->AsObject());
		//tenant.ImportFromString(Skydrm_Tenant_Return);
		if (ret.GetCode() == 0) {
			std::cout << "Tenant ID:" << tenant.GetTenant() << endl;
			std::cout << "RMS URL:" << tenant.GetRMSURL() << endl;
			std::cout << "Rourter URL:" << tenant.GetRouterURL() << endl;
			EXPECT_STREQ(tenant_name.c_str(), tenant.GetTenant().c_str());
			EXPECT_STREQ(rmsURL.c_str(), tenant.GetRMSURL().c_str());
			EXPECT_STREQ(router.c_str(), tenant.GetRouterURL().c_str());
		}
		else {
			//Read root node: status and message
			Json::Value jroot_result;
			jroot_result = ParseJsonFile(file_name);
			code = jroot_result["statusCode"].asInt();
			message = jroot_result["message"].asString();
			std::cout << "return code is: " << ret.GetCode() << endl;
			std::cout << "return message is: " << ret.GetMessage() << endl;
			EXPECT_EQ(expect_code, ret.GetCode());
			EXPECT_STREQ(expect_message.c_str(), ret.GetMessage());
		}

		std::cout << "Test finished." << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
	}
}
vector<vector<string>> vf_invalid = readCSV("InvalidTenantParaTest.csv");
INSTANTIATE_TEST_CASE_P(InvalidParaTenantReturn, InvalidTenantParaTest, testing::ValuesIn(vf_invalid));

// Test different tenant with different router
class DisplayTenantLoginParaTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(DisplayTenantLoginParaTest, TenantName)
{
	//get parameter of file name, type is string
	vector<string> csv = GetParam();
	string public_rmtest_tenant = "rmtest.nextlabs.solutions";
	string public_skyDRM_tenant = "skydrm.com";
	size_t tenantName_pos;
	string tenantName, router;
	string file_name;

	JsonDocument doc;
	JsonValue* root;

	int err_code = 0;
	size_t err_pos = 0;

	string status, rmsURL;

	try {
		// using 3rd party Jsoncpp to read and get json value
		file_name = csv[2];
		Json::Value jroot;
		jroot = ParseJsonFile(file_name);

		//Read root node: status and message
		status = jroot["statusCode"].asString();
		rmsURL = jroot["results"]["server"].asString();
		tenantName_pos = rmsURL.find_first_of(":");
		string::size_type idx_rmtest = rmsURL.find("rmtest");
		string::size_type idx_public = file_name.find("public");
		if (idx_public != string::npos)
		{
			cout << "It is public server!" << endl;
			// Parse rmsURL, get tenant name and router name.
			tenantName = public_skyDRM_tenant;

			if (idx_rmtest != string::npos) {
				cout << "It is public rmtest server!" << endl;
				router = "https://rmtest.nextlabs.solutions/router";
			}
			else
			{
				cout << "It is public skydrm server!" << endl;
				router = "https://r." + public_skyDRM_tenant + "/router";
			}
		}
		else
		{
			cout << "It is specific server!" << endl;

			// Parse rmsURL, get tenant name and router name.
			if (idx_rmtest != string::npos) {
				cout << "It is specific rmtest server!" << endl;
				tenantName = rmsURL.substr(tenantName_pos + 3, rmsURL.length() - public_rmtest_tenant.length() - 1 - tenantName_pos - 7);
				router = "https://rmtest.nextlabs.solutions/router";
			}
			else
			{
				cout << "It is specific skydrm server!" << endl;
				tenantName = rmsURL.substr(tenantName_pos + 3, rmsURL.length() - public_skyDRM_tenant.length() - 1 - tenantName_pos - 7);
				router = "https://r." + public_skyDRM_tenant + "/router";
			}
		}

		RMTenant tenant(router, tenantName);

		string jsonstr = ReadFromFile(file_name);
		std::cout << "jsonstr length is: " << jsonstr.length() << endl;
		doc.LoadJsonString(jsonstr, &err_code, &err_pos);
		root = doc.GetRoot();
		//RetValue ret = tenant.ImportFromJson(root->AsObject());
		RetValue ret = tenant.ImportFromRMSResponse(root->AsObject());
		//tenant.ImportFromRMSResponse(root->AsObject());
		//ImportJsonFile("successlogin.json", 0);
		// in GetTenant(), it use setTenant() directly to set skydrm.com as tenant name?
		std::cout << "Tenant ID:" << tenant.GetTenant() << endl;
		std::cout << "RMS URL:" << tenant.GetRMSURL() << endl;
		std::cout << "Rourter URL:" << tenant.GetRouterURL() << endl;

		EXPECT_STREQ(tenantName.c_str(), tenant.GetTenant().c_str());
		EXPECT_STREQ(rmsURL.c_str(), tenant.GetRMSURL().c_str());
		EXPECT_STREQ(router.c_str(), tenant.GetRouterURL().c_str());

		ExportToFile(file_name, tenant.ExportToString());
		std::cout << "Test finished." << endl;
	}
	catch (ios_base::failure e)
	{
		cout << "IO error!" << endl;
		throw;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> vf_valid = readCSV("ValidTenantLoginTest.csv");
INSTANTIATE_TEST_CASE_P(DisplayTenantLoginParaReturn, DisplayTenantLoginParaTest, testing::ValuesIn(vf_valid));

class TenantSpecailURLTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(TenantSpecailURLTest, URLTest) {
	string file_name;
	string expect_result;
	try {
		JsonDocument doc;
		JsonValue* root;

		int err_code = 0;
		size_t err_pos = 0;
		vector<string> csv = GetParam();
	    file_name = csv[2];
		expect_result = csv[3];
		RMTenant tenant("","");

		string jsonstr = ReadFromFile(file_name);
		std::cout << "jsonstr length is: " << jsonstr.length() << endl;
		doc.LoadJsonString(jsonstr, &err_code, &err_pos);
		root = doc.GetRoot();
		RetValue ret = tenant.ImportFromRMSResponse(root->AsObject());
		cout << "Tanent ServerURL: " << tenant.GetRMSURL().c_str() << endl;
		cout << "Tanent Exported: " << tenant.ExportToString().c_str() << endl;
		EXPECT_STREQ(expect_result.c_str(), tenant.GetRMSURL().c_str());

		cout << "Test finish!" << endl;
	}
	catch (...) {
		cout  << "Exception!" << endl;
		throw;
	}
}
vector<vector<string>> vf_url = readCSV("TenantSpecailURLTest.csv");
INSTANTIATE_TEST_CASE_P(TenantSpecailURLTestReturn, TenantSpecailURLTest,testing::ValuesIn(vf_url));

/* 1.Client need configure following information:Center router url: default should be: https://r.skydrm.com/router
	2.Client tenant, default tenant should be "skydrm.com".
	3.Client need query where is your rms server: router_url + tenant_name, for example: https://r.skydrm.com/router/rs/q/tenant/skydrm  */
TEST(TenantTest, HttpRequest) {
	string httprul_str,http_defualt, http_set;
	string method_str = "GET";
	JsonDocument doc;
	JsonValue* root;
	int err_code = 0;
	size_t err_pos = 0;
	RMTenant defualt_tenant;
	http_defualt = defualt_tenant.GetRouterURL() + "/router/rs/q/tenant/" + defualt_tenant.GetTenant();

	//Defualt Tenant
	HTTPRequest req_defualt = defualt_tenant.GetTenantQuery();
	EXPECT_STREQ(req_defualt.GetAcceptTypes()[0].c_str(), "application/json");
	EXPECT_STREQ(method_str.c_str(), req_defualt.GetMethod().c_str());
	EXPECT_STREQ(http_defualt.c_str(), req_defualt.GetURL().c_str());

	//Set Tenant
	defualt_tenant.SetRouter("https://rmtest.nextlabs.solutions");
	defualt_tenant.SetTenant("jt2go");
	cout << "SDWEFWD" << defualt_tenant.GetRMSURL() << endl;
	http_set = defualt_tenant.GetRouterURL() + "/router/rs/q/tenant/" + defualt_tenant.GetTenant();
	HTTPRequest req_set = defualt_tenant.GetTenantQuery();
	EXPECT_STREQ(req_set.GetAcceptTypes()[0].c_str(), "application/json");
	EXPECT_STREQ(method_str.c_str(), req_set.GetMethod().c_str());
	EXPECT_STREQ(http_set.c_str(), req_set.GetURL().c_str());

	//ExportToString
	RMTenant tenant = CreateDefaultTenant();
	cout << "Tenant ID:" << tenant.GetTenant() << endl;
	cout << "Tenant Router:" << tenant.GetRouterURL() << endl;
	cout << "Tenant RMS URL:" << tenant.GetRMSURL() << endl;
	cout << "export to string:" << tenant.ExportToString() << endl;
	httprul_str = tenant.GetRouterURL() + "/router/rs/q/tenant/" + tenant.GetTenant();
	HTTPRequest req = tenant.GetTenantQuery();
	EXPECT_STREQ(req.GetAcceptTypes()[0].c_str(), "application/json");
	EXPECT_STREQ(method_str.c_str(), req.GetMethod().c_str());
	EXPECT_STREQ(httprul_str.c_str(), req.GetURL().c_str());

	string tn = tenant.ExportToString();
	RMTenant ext_tn;
	RetValue RET =  ext_tn.ImportFromString(tn);

	//Import From RMS response
	string tenant_str = ReadFromFile("Tenant.txt");
	RMTenant tenant_import, tenant_importJSON;
	RetValue ret_import = tenant_import.ImportFromRMSResponse(tenant_str);
	EXPECT_EQ(ret_import.GetCode(),0);
	doc.LoadJsonString(tenant_str, &err_code, &err_pos);
	root = doc.GetRoot();
	RetValue ret_imporjson = tenant_importJSON.ImportFromRMSResponse(root->AsObject());
	EXPECT_EQ(ret_imporjson.GetCode(), 0);
	cout << endl;
}