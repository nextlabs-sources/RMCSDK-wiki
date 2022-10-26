#include "stdafx.h"
#include "Header_Tenant.h"
#include "Header_SystemPara.h"


RMUser GetDefaultUser() {
	string user_response = default_file_path + "LoginNormal.txt";
	RMTenant tenant = CreateDefaultTenant();
	RMSystemPara sys_para = CreateDefaultSysPara();

	string upgrade = ReadFromFile(default_file_path + "upgrade.txt");
	string cer_str = ReadFromFile(default_file_path + "RMCCoreAccount_Cer.txt");
	RMProduct product = sys_para.GetProduct();
	RetValue ret = product.ImportFromRMSResponse(upgrade);
	sys_para.SetProduct(product);

	RMUser user(sys_para, tenant);
	string user_str = ReadFromFile(user_response);
	RetValue ret_usr = user.ImportFromRMSResponse(user_str);
	user.GetDefaultMembership().m_certificate.ImportFromRMSResponse(cer_str);
	return user;
}


//void printUserInfo(RMUser user)
//{
//	cout << "Print user info:" << endl;
//	cout << "User Name:" << user.GetName() << endl;
//	cout << "User Email:" << user.GetEmail() << endl;
//	cout << "User ID:" << user.GetUserID() << endl;
//	cout << "Login type:" << user.GetIdpType() << endl;
//	cout << "---" << endl;
//}
//
//void checkErrorCode(RetValue ret, string jsonFile)
//{
//	//ERROR CODE : 0xF000 = 61940
//	if (ret.GetCode() >= 0xF000)
//	{
//		// using 3rd party Jsoncpp to read and get json value
//		Json::Value jroot;
//		jroot = ParseJsonFile(jsonFile);
//		string status, message;
//
//		//Read root node: status and message
//		status = jroot["statusCode"].asString();
//		message = jroot["message"].asString();
//
//		// using atoi function to transfer string to const char* 
//		EXPECT_EQ(atoi(status.c_str()), ret.GetCode() - 0xF000);
//		EXPECT_STREQ(message.c_str(), ret.GetMessage());
//		cout << "RMS code: " << ret.GetCode() - 0xF000 << " - " << message.c_str() << endl;
//
//	}
//	else
//		EXPECT_EQ(0, ret.GetCode());
//}
//
////importer user version 1.0
//RMUser importUser(string jsonFile, int importType)
//{
//	RMUser u, user;
//	RMCCORE::JsonDocument doc;
//	int err_code = 0;
//	size_t err_pos = 0;
//
//	string jsonstr = ReadFromFile(jsonFile);
//	cout << "jsonstr length is: " << jsonstr.length() << endl;
//	doc.LoadJsonString(jsonstr, &err_code, &err_pos);
//	RMCCORE::JsonValue* root;
//
//	root = doc.GetRoot();
//	cout << "debug importUser- before import" << endl;
//	RetValue ret = u.ImportFromRMSResponse(root->AsObject());
//	printUserInfo(u);
//
//	// ERROR CODE: 0xF000 = 61940 
//	checkErrorCode(ret, jsonFile);
//
//	switch (importType)
//	{
//	case 1:
//	{
//		JsonValue* ex_json = u.ExportToJson();
//		//cout << "-ExportToJson0- [" << ex_json <<"]" << endl;
//		ret = user.ImportFromJson(ex_json->AsObject());
//		//cout << "-ExportToJson1- [" << user.ExportToJson() << "]" << endl;
//		EXPECT_EQ(0, ret.GetCode());
//		break;
//	}
//	case 2:
//	{
//		string ex_string = u.ExportToString();
//		//cout << "-ExportToString0-" << endl << ex_string<< endl;
//		ret = user.ImportFromString(ex_string);
//		cout << "-ExportToString-" << endl << user.ExportToString() << endl << "---" << endl;
//		EXPECT_EQ(0, ret.GetCode());
//		break;
//	}
//	default:
//		user = u;
//		break;
//	}
//
//	return user;
//}
//
////importer user version 2.0, for RMC core build 58 and later, Lifi.yuan 3/2018
//const RMUser& importUser58(string jsonFile, int RMS_ID)
//{
//	RMSystemPara clientInfo = GenerateSystemParameter("Testing app");
//	static RMUser user(clientInfo, CreateTenant_perRMS(RMS_ID));
//
//	RMCCORE::JsonDocument doc;
//	int err_code = 0;
//	size_t err_pos = 0;
//
//	string jsonstr = ReadFromFile(jsonFile);
//	doc.LoadJsonString(jsonstr, &err_code, &err_pos);
//	RMCCORE::JsonValue* root;
//	root = doc.GetRoot();
//	cout << "Import user:" << endl;
//	RetValue ret = user.ImportFromRMSResponse(root->AsObject());
//	//printUserInfo(user);
//	// ERROR CODE: 0xF000 = 61940 
//	checkErrorCode(ret, jsonFile);
//
//	return user;
//
//}
//
//const RMUser& importUser58_2(string jsonFile, int RMS_ID, RMSystemPara& clientInfo)
//{
//	clientInfo = GenerateSystemParameter("Testing app");
//	static RMUser user(clientInfo, CreateTenant_perRMS(RMS_ID));
//
//	RMCCORE::JsonDocument doc;
//	int err_code = 0;
//	size_t err_pos = 0;
//
//	string jsonstr = ReadFromFile(jsonFile);
//	doc.LoadJsonString(jsonstr, &err_code, &err_pos);
//	RMCCORE::JsonValue* root;
//	root = doc.GetRoot();
//	cout << "Import user:" << endl;
//	RetValue ret = user.ImportFromRMSResponse(root->AsObject());
//	//printUserInfo(user);
//	// ERROR CODE: 0xF000 = 61940 
//	checkErrorCode(ret, jsonFile);
//
//	return user;
//
//}
//
//
//int ifExist_Membership(RMUser user, string tenantId_m)
//{
//	RMMembership* mem = user.FindMembership(tenantId_m);
//	if (NULL == mem) {
//		cout << "Tenant " << tenantId_m << " not found." << endl;
//		return 0;
//	}
//	else {
//		cout << "Found Tenant " << tenantId_m << " Information" << endl;
//		return 1;
//	}
//}
//
//RMMembership FindMembership(RMUser user, string tenantId = "Default")
//{
//	if (tenantId == "Default")
//	{
//		return user.GetDefaultMembership();
//	}
//	else
//	{
//		RMMembership *mem = user.FindMembership(tenantId);
//		if (mem != NULL) {
//			return *mem;
//		}
//		else {
//			cout << "FindMembership()_return NULL" << endl;
//			return *mem;
//		}
//	}
//}


