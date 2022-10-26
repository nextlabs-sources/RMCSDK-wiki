#include "stdafx.h"
#include "rmccore\restful\rmuser.h"

using namespace RMCCORE;
using namespace std;

 //Test from user interface FindMembership,GetID,GetTenantID,GetProjectID,GetIdpType
TEST(MembershipTest, FindMembership) {
	JsonDocument doc;
	JsonValue* root;
	int err_code = 0;
	size_t err_pos = 0;
	int project_id, idp_type;
	string id, tenant_id;
	try {
		RMUser user = GetDefaultUser();
		RMMembership *membership = user.FindMembership("492dbdbb-427d-4f48-a6f4-400d49793af4");
		Json::Value jroot;
		jroot = ParseJsonFile(default_file_path + "DUserData.txt");
		project_id = jroot["Memberships"][1]["ProjectId"].asInt();
		id = jroot["Memberships"][1]["Id"].asString();
		tenant_id = jroot["Memberships"][1]["TenantId"].asString();
		idp_type = jroot["Memberships"][1]["IDPType"].asInt();

		cout <<"Get ID: "<< membership->GetID() << endl;
		cout << "Get IDPID: " << membership->GetIdpType() << endl;
		cout << "Get Project ID: " << membership->GetProjectID() << endl;
		cout << "Get Tenant ID: " << membership->GetTenantID() << endl;
		cout << "Export To String: " << membership->ExportToString() << endl;

		//TEST GetID,GetTenantID,GetProjectID,GetIdpType
		std::cout << "Membership Export: " << membership->ExportToString() << endl;
		string ext_string = membership->ExportToString();
		EXPECT_EQ(176,ext_string.size());
		EXPECT_STREQ(id.c_str(), membership->GetID().c_str());
		EXPECT_STREQ(tenant_id.c_str(), membership->GetTenantID().c_str());
		EXPECT_EQ(project_id, membership->GetProjectID());
		EXPECT_EQ(idp_type, membership->GetIdpType());

		//TEST ImportFromString
		RMMembership membership_import;
		membership_import.ImportFromString(ext_string);
		EXPECT_EQ(176, membership_import.ExportToString().size());
		std::cout << "Test finished." << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Test from user interface GetDefaultMembership
TEST(MembershipTest, GetDefaultMembership) {
	JsonDocument doc;
	JsonValue* root;
	int err_code = 0;
	size_t err_pos = 0;
	int project_id, idp_type;
	string id, tenant_id;
	try {
		RMUser user = GetDefaultUser();
		RMMembership membership = user.GetDefaultMembership();
		Json::Value jroot;
		jroot = ParseJsonFile(default_file_path + "DUserData.txt");
		project_id = jroot["Memberships"][0]["ProjectId"].asInt();
		id = jroot["Memberships"][0]["Id"].asString();
		tenant_id = jroot["Memberships"][0]["TenantId"].asString();
		idp_type = jroot["Memberships"][0]["IDPType"].asInt();

		cout << "Get ID: " << membership.GetID() << endl;
		cout << "Get IDPID: " << membership.GetIdpType() << endl;
		cout << "Get Project ID: " << membership.GetProjectID() << endl;
		cout << "Get Tenant ID: " << membership.GetTenantID() << endl;
		cout << "Export To String: " << membership.ExportToString() << endl;

		//TEST GetID,GetTenantID,GetProjectID,GetIdpType
		std::cout << "Membership Export: " << membership.ExportToString() << endl;
		string ext_string = membership.ExportToString();
		EXPECT_EQ(1173, ext_string.size());
		EXPECT_STREQ(id.c_str(), membership.GetID().c_str());
		EXPECT_STREQ(tenant_id.c_str(), membership.GetTenantID().c_str());
		EXPECT_EQ(project_id, membership.GetProjectID());
		EXPECT_EQ(idp_type, membership.GetIdpType());

		//TEST ImportFromString
		RMMembership membership_import;
		membership_import.ImportFromString(ext_string);
		string import_str = membership_import.ExportToString();
		EXPECT_EQ(1173, membership_import.ExportToString().size());

		//TEST ImportFromJson
		RMMembership membership_importjson;
		JsonValue *ext_json = membership.ExportToJson();
		membership_importjson.ImportFromJson(ext_json ->AsObject());
		EXPECT_EQ(1173, membership_importjson.ExportToString().size());
		std::cout << "Test finished." << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Test membership invalid parameter
class InvalidParaMembershipTest :public::testing::TestWithParam<string> {};
TEST_P(InvalidParaMembershipTest, InvalidPara) {
	try {
		//get parameter of file name, type is string
		string para = GetParam();
		//int base_code = 61440;
		//int code;
		//string message;
		//RMMembership membership;
		//RMCCORE::JsonDocument doc;
		//RMCCORE::JsonValue* root;
		//int err_code = 0;
		//size_t err_pos = 0;
		//Json::Value jroot;
		//jroot = ParseJsonFile(para);

		////Read root node: status and message
		//code = jroot["statusCode"].asInt();
		//message = jroot["message"].asString();
		////std::cout << "responde code is: " << code << endl;
		////std::cout << "message is: " << message << endl;

		//string membershipstr = ReadFromFile(para);
		//std::cout << "tokenstr length is: " << membershipstr.length() << endl;
		//doc.LoadJsonString(membershipstr, &err_code, &err_pos);
		//root = doc.GetRoot();
		//RetValue ret = membership.ImportFromRMSResponse(root->AsObject());

		//Json::Value jroot_result;
		//jroot_result = ParseJsonFile(para);

		////Read root node: status and message
		//code = jroot_result["statusCode"].asInt();
		//message = jroot_result["message"].asString();
		//std::cout << "return code is: " << ret.GetCode() << endl;
		//std::cout << "return message is: " << ret.GetMessage() << endl;
		//EXPECT_EQ((base_code + code), ret.GetCode());
		//EXPECT_STREQ(message.c_str(), ret.GetMessage());
		
		//std::cout << "membership export is: " << membership.ExportToString() << endl;

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
INSTANTIATE_TEST_CASE_P(InvalidParaMembershipReturn, InvalidParaMembershipTest, testing::Values(default_file_path+"Case2_Membership_errorDuid.json", default_file_path+"Case3_Membership_errorMembership.json", default_file_path+"Case5_Membership_errorPublickey.json", default_file_path+"Case6_Membership_emptyPublickey.json"));


//// Membership
//TEST(MembershipTest, NormalParameter)
//{
//	RMUser user;
//	JsonDocument doc;
//	JsonValue* root;
//	int err_code = 0;
//	size_t err_pos = 0;
//
//	try {
//		//Test default parameter
//		RMMembership mem;
//
//		string jsonstr = ReadFromFile("RMSBasicLogin.txt");
//		std::cout << "jsonstr length is: " << jsonstr.length() << endl;
//
//		doc.LoadJsonString(jsonstr, &err_code, &err_pos);
//		root = doc.GetRoot();
//		RetValue ret = user.ImportFromRMSResponse(root->AsObject());
//		//membership.ImportCertficateFromRMSResponse(root->AsObject());
//
//
//		std::string tenantid = "21b06c79-baab-419d-8197-bad3ce3f4476";
//		mem = user.GetDefaultMembership();
//
//		string exported_user = user.ExportToString();
//		std::cout << "USER :" << exported_user << endl;
//
//		std::cout << "Membership ID:" << mem.GetID() << endl;
//		std::cout << "Tenant ID:" << mem.GetTenantID() << endl;
//		std::cout << "Idp Type:" << mem.GetIdpType() << endl;
//		std::cout << "Project ID:" << mem.GetProjectID() << endl;
//		std::cout << "Exported string:" << mem.ExportToString() << endl;
//
//		RMMembership * membership = user.FindMembership(tenantid);
//		ExportToFile("membership.txt", mem.ExportToString());
//
//		string jsonstr2 = ReadFromFile("ex_membership.txt");
//		doc.LoadJsonString(jsonstr2, &err_code, &err_pos);
//		root = doc.GetRoot();
//		mem.ImportFromRMSResponse(root->AsObject());
//
//		std::cout << "Membership ID:" << membership -> GetID() << endl;
//		std::cout << "Tenant ID:" << membership -> GetTenantID() << endl;
//		std::cout << "Idp Type:" << membership ->GetIdpType() << endl;
//		std::cout << "Project ID:" << membership ->GetProjectID() << endl;
//		std::cout << "Exported string:" << membership ->ExportToString() << endl;
//		
//		std::cout << "Test finished." << endl;
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//	}
//}