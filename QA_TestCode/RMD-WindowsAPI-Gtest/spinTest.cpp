#include "Header_User.h"
#include <curl/curl.h>  
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <tlhelp32.h>
#include "SDLAPI.h"


#pragma region Import the type libraries

#import "libid:2DF8D04C-5BFA-101B-BDE5-00AA0044DE52" \
	rename("RGB", "MSORGB") \
	rename("DocumentProperties", "MSODocumentProperties")

using namespace Office;

#import "libid:0002E157-0000-0000-C000-000000000046"

using namespace VBIDE;

#import "libid:00020905-0000-0000-C000-000000000046" \
	rename("ExitWindows", "WordExitWindows") \
	rename("FindText", "WordFindText")

using namespace std;


vector<string> get_tags(ISDRmUser *&puser, string tokenGroupName, string membership) {
	vector<SDR_CLASSIFICATION_CAT> categories;
	bool original = false;
	bool newly = false;
	string tag_original = "";
	string tag_new = "";
	vector<string> tags;
	SDWLResult res = puser->GetClassification(tokenGroupName, categories);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	//Make sure tag is valid
	for (int i = 0; i < categories.size(); i++) {
		if (original&&newly)
			continue;
		for (int j = 0; j < categories[i].labels.size(); j++) {
			int rights_eval = PolicyEvaluationResponseData(membership, categories[i].name, categories[i].labels[j].name);
			if (rights_eval != 0) {
				if (!original) {
					tag_original = "{\"" + categories[i].name + "\":[\"" + categories[i].labels[j].name + "\"]}";
					original = true;
					tags.push_back(tag_original);
					continue;
				}
				else if (original&&newly) {
					break;
				}
				else {
					if (!newly) {
						tag_new = "{\"" + categories[i].name + "\":[\"" + categories[i].labels[j].name + "\"]}";
						newly = true;
						tags.push_back(tag_new);
						break;
					}

				}
			}
			else {
				continue;
			}
		}
	}
	return tags;
}

bool tag_is_valid(vector<string> tags) {
	if (tags.size() != 2 || tags[0] == "" || tags[1] == "") {
		return false;
	}
	else {
		return true;
	}
}

vector<string> get_project_msg(int projectID_type, bool is_project) {
	Json::Value jroot;
	jroot = ParseJsonFile(user_respond);
	string server = jroot["results"]["server"].asString();
	string membership = "";
	string tokenGroupName = "";
	string project_name = "";
	string expire = "";
	string watermark = "";
	string totalMembers = "";
	string p_id = "";
	string type = "";
	string parentTenantId = "";
	string parentTenantName = "";
	string creationTime = "";
	string configurationModified = "";
	string totalFiles = "";
	vector<string> project_msg;
	if (jroot["extra"]["memberships"].size() <= 2) {
		cout << "No projects from this user!" << endl;
		return project_msg;
	}
	else {
		if (is_project) {
			for (int i = 0; i < jroot["extra"]["memberships"].size(); i++) {
				if (((jroot["extra"]["memberships"][i]["type"].asInt()) == 1) && (jroot["extra"]["memberships"][i]["projectId"].asInt() == projectID_type)) {
					membership = jroot["extra"]["memberships"][i]["id"].asString();
					type = jroot["extra"]["memberships"][i]["type"].asString();
					p_id = jroot["extra"]["memberships"][i]["projectId"].asString();
					break;
				}
			}
		}
		else {
			for (int i = 0; i < jroot["extra"]["memberships"].size(); i++) {
				if ((jroot["extra"]["memberships"][i]["type"].asInt()) == projectID_type) {
					membership = jroot["extra"]["memberships"][i]["id"].asString();
					type = jroot["extra"]["memberships"][i]["type"].asString();
					p_id = jroot["extra"]["memberships"][i]["projectId"].asString();
					break;
				}
			}
		}
		bool list_result = GetProjectListResponseData("1", "100", "", "");
		jroot = ParseJsonFile(projectlist_return);
		int total_projects = jroot["results"]["totalProjects"].asInt();
		for (int i = 0; i < total_projects; i++) {
			int id = jroot["results"]["detail"][i]["id"].asInt();
			if (id == atoi(p_id.c_str())) {
				tokenGroupName = jroot["results"]["detail"][i]["tokenGroupName"].asString();
				p_id = jroot["results"]["detail"][i]["id"].asString();
				project_name = jroot["results"]["detail"][i]["name"].asString();
				totalMembers = jroot["results"]["detail"][i]["totalMembers"].asString();
				totalFiles = jroot["results"]["detail"][i]["totalFiles"].asString();
				expire = jroot["results"]["detail"][i]["expiry"].asString();
				watermark = jroot["results"]["detail"][i]["watermark"].asString();
				parentTenantId = jroot["results"]["detail"][i]["parentTenantId"].asString();
				parentTenantName = jroot["results"]["detail"][i]["parentTenantName"].asString();
				creationTime = jroot["results"]["detail"][i]["creationTime"].asString();
				configurationModified = jroot["results"]["detail"][i]["configurationModified"].asString();
				break;
			}
		}
	}
	project_msg.push_back(p_id);
	project_msg.push_back(project_name);
	project_msg.push_back(type);
	project_msg.push_back(membership);
	project_msg.push_back(tokenGroupName);
	project_msg.push_back(totalMembers);
	project_msg.push_back(totalFiles);
	project_msg.push_back(expire);
	project_msg.push_back(watermark);
	project_msg.push_back(parentTenantId);
	project_msg.push_back(parentTenantName);
	project_msg.push_back(creationTime);
	project_msg.push_back(configurationModified);
	return project_msg;
}

class spinUserShareTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(spinUserShareTest, spinShareTest) {
	try {
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		//expire.type = IExpiryType::RANGEEXPIRE;
		//expire.start = (uint64_t)std::time(nullptr) * 1000;
		//expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;

		vector<string> csv = GetParam();

		string rights_str = csv[2];
		string localfile_path = csv[5];
		string recipients_str = csv[3];
		string_replace(recipients_str, "|", ",");
		string comments = csv[4];
		string code = csv[9];
		string message = csv[10];

		string name = csv[6];
		string wm_text = csv[7];
		string tag_text = "{}";
		wstring w_name;
		w_name = s2ws(name);
		wstring comments_w;
		wstring recipients = s2ws(recipients_str);
		SDR_WATERMARK_INFO watermark_info = { wm_text,"font","color",9,0,ANTICLOCKWISE,true };

		if (comments != "") {
			comments_w = s2ws(comments);
		}

		wstring localfile_pathw = s2ws(localfile_path);
		wstring nxlfile_path;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;

		std::vector<SDRmFileRight> rights;
		rights = Tick_Rights(rights_str);

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		//bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");
		if (login_result) {
			//Operation here
			res = puser->UpdateUserInfo();
			wstring tenantName;
			printPolicyBaseOnTenant(tenantName, puser);

			ISDRmNXLFile* nxlfile_a;
			ISDRmNXLFile* nxlfile_b;
			EXPECT_EQ(res.GetCode(), 0);
			std::string newVersionStr;
			std::string downloadURL;
			std::string checksum;
			//res = pInstance->CheckSoftwareUpdate(newVersionStr, downloadURL, checksum);
			//EXPECT_FALSE(res.GetCode());
			//Share file
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
			if (res.GetCode() == 0) {
				std::wstring filename;
				bool delete_result;
				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					filename = nxlfile->GetFileName();
					//Rights verified
					std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
					res = puser->GetRights(nxlfile_path, rightsAndWatermarks, true);
					EXPECT_EQ(rightsAndWatermarks.size(), 11);
					std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks1;
					res = puser->GetRights(nxlfile_path, rightsAndWatermarks1, false);
					EXPECT_EQ(rightsAndWatermarks1.size(), rights.size());
					int rights_value = rights_changeToValue(rightsAndWatermarks1);
					int rights_value_original = rights_changeToValue2(rights);
					EXPECT_EQ(rights_value, rights_value_original);
					vector<SDRmFileRight> rights_v = nxlfile->GetRights();
					EXPECT_EQ(rights_v.size(), rights.size());
					bool rights_result = rights_verified(rights, nxlfile);
					EXPECT_TRUE(rights_result);

					bool has_watermark = nxlfile->CheckRights(RIGHT_WATERMARK);

					//Get TAG
					string tag = nxlfile->GetTags();
					EXPECT_STREQ(tag.c_str(), tag_text.c_str());

					//Get watermark 
					SDR_WATERMARK_INFO wm = nxlfile->GetWaterMark();
					EXPECT_STREQ(wm.text.c_str(), watermark_info.text.c_str());

					//Get expiration 
					SDR_Expiration exp = nxlfile->GetExpiration();
					uint64_t end = exp.end;
					uint64_t start = exp.start;
					EXPECT_EQ(start, expire.start);
					EXPECT_EQ(end, expire.end);

					//Upload file
					res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, comments_w);
					if (code == "0") {
						cout << "Upload file result!" << endl;
						EXPECT_FALSE(res.GetCode());
					}
					else {
						cout << "Upload file failed!" << endl;
						EXPECT_STREQ(message.c_str(), res.GetMsg().c_str());
					}

					if (!res.GetCode()) {
						//Update recipeints
						res = puser->UpdateRecipients(nxlfile, { "3333111@qq.com" });
						EXPECT_FALSE(res.GetCode());

						res = puser->GetActivityInfo(filename, infoVec);
						EXPECT_FALSE(res.GetCode());
					}
					else {
						//Update recipeints
						res = puser->UpdateRecipients(nxlfile, { "11111@qq.com" });
						EXPECT_FALSE(res.GetCode());
						EXPECT_STREQ(res.GetMsg().c_str(), "File not found");
						//Get log
						res = puser->GetActivityInfo(filename, infoVec);
						EXPECT_EQ(res.GetCode(), 1168);
						EXPECT_STREQ("File not found", res.GetMsg().c_str());
					}

					res = puser->CloseFile(nxlfile);
					checkSDWLResult(res);
					EXPECT_EQ(0, res.GetCode());
				}
				else {
					cout << "No file generated!" << endl;
					EXPECT_FALSE(res.GetCode());
				}
			}
			else {
				cout << "Share file failed!" << endl;
				if (code == "0") {
					EXPECT_TRUE(false);
				}
				else {
					EXPECT_STREQ(message.c_str(), res.GetMsg().c_str());
				}
			}
			res = pInstance->Save();
			res = puser->LogoutUser();
		}
		else {
			EXPECT_FALSE(res.GetCode());
			cout << "User login failed!" << endl;
		}
		SDWLibDeleteRmcInstance(pInstance);

		//Switch user to protect file
		ISDRmTenant *pTenant_b = NULL;
		ISDRmUser *puser_b = NULL;
		ISDRmcInstance *pInstance_b = NULL;
		nxlfile_path = L"";
		login_result = SDKRPM_Login(pInstance_b, pTenant_b, puser_b, DEFUALT_ROUTER, L"", email, password);
		if (login_result) {
			res = puser_b->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
			if (res.GetCode() == 0) {
				ISDRFiles* pFile = NULL;
				std::wstring filename;
				bool delete_result;
				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
				ISDRmNXLFile* nxlfile;
				res = puser_b->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					filename = nxlfile->GetFileName();
					//Upload file
					res = puser_b->UploadFile(nxlfile_path, nxlfile_path, recipients, comments_w);
					if (!res.GetCode()) {
						//Get rights
						std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
						res = puser_b->GetRights(nxlfile_path, rightsAndWatermarks, true);
						EXPECT_EQ(rightsAndWatermarks.size(), 11);
						std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks1;
						res = puser_b->GetRights(nxlfile_path, rightsAndWatermarks1, false);
						EXPECT_EQ(rightsAndWatermarks1.size(), rights.size());
						int rights_value = rights_changeToValue(rightsAndWatermarks1);
						int rights_value_original = rights_changeToValue2(rights);
						EXPECT_EQ(rights_value, rights_value_original);
						vector<SDRmFileRight> rights_v = nxlfile->GetRights();
						EXPECT_EQ(rights_v.size(), rights.size());
						//Rights verified
						bool rights_result = rights_verified(rights, nxlfile);
						EXPECT_TRUE(rights_result);

						//Update recipeints
						res = puser_b->UpdateRecipients(nxlfile, { "11111@qq.com" });
						EXPECT_FALSE(res.GetCode());

						res = puser_b->GetActivityInfo(filename, infoVec);
						EXPECT_FALSE(res.GetCode());
						//Get TAG
						string tag = nxlfile->GetTags();
						EXPECT_STREQ(tag.c_str(), tag_text.c_str());
						//Get watermark 
						SDR_WATERMARK_INFO wm = nxlfile->GetWaterMark();
						EXPECT_STREQ(wm.text.c_str(), watermark_info.text.c_str());
					}
					else {
						cout << "Upload file failed!" << endl;
					}
				}
			}
			else {
				cout << "Share file failed!" << endl;
				EXPECT_TRUE(false);
			}

			puser_b->LogoutUser();
		}
		else {
			EXPECT_TRUE(res.GetCode());
			cout << "User login failed!" << endl;
		}
		res = pInstance_b->Save();
		SDWLibDeleteRmcInstance(pInstance_b);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
INSTANTIATE_TEST_CASE_P(spinUserShareTesting, spinUserShareTest, testing::ValuesIn(csv_share_spin));

class spinUserProtectTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(spinUserProtectTest, spinProtectTest) {
	try {
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		//expire.type = IExpiryType::RELATIVEEXPIRE;
		//expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;
		//expire.start = (uint64_t)std::time(nullptr) * 1000;

		vector<string> csv = GetParam();
		string rights_str = csv[2];
		string localfile_path = csv[3];
		string name = csv[4];
		string wm_text = csv[5];
		string tag_text = "{}";
		wstring w_name;
		w_name = s2ws(name);
		wstring localfile_pathw = s2ws(localfile_path);
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		std::vector<SDRmFileRight> rights;
		rights = Tick_Rights(rights_str);
		if (wm_text == "null")
		{
			wm_text = "";
		}
		SDR_WATERMARK_INFO watermark_info = { wm_text,"font","color",9,0,ANTICLOCKWISE,true };

		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			wstring nxlfile_path = L"C:\\zhuzhu";
			createFolder(ws2s(nxlfile_path));
			res = puser->UpdateUserInfo();
			EXPECT_EQ(res.GetCode(), 0);
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
			pInstance->Save();
			if (res.GetCode() == 0) {
				std::wstring filename;
				bool delete_result;
				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
				SDR_FILE_ACTIVITY_INFO ac_info;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					bool is_open = nxlfile->IsOpen();
					EXPECT_TRUE(is_open);
					//Get file name
					filename = nxlfile->GetFileName();

					//Get expiration 
					SDR_Expiration exp = nxlfile->GetExpiration();
					uint64_t end = exp.end;
					uint64_t start = exp.start;
					EXPECT_EQ(end, expire.end);

					//Get watermark
					SDR_WATERMARK_INFO wm = nxlfile->GetWaterMark();
					EXPECT_STREQ(wm.text.c_str(), watermark_info.text.c_str());
					//Get tag
					string tag = nxlfile->GetTags();
					EXPECT_STREQ(tag.c_str(), tag_text.c_str());
					//Get rights
					vector<SDRmFileRight> rights_v = nxlfile->GetRights();
					EXPECT_EQ(rights_v.size(), rights.size());
					bool rights_result = rights_verified(rights, nxlfile);
					EXPECT_TRUE(rights_result);

					//Upload file
					res = puser->UploadFile(nxlfile_path, nxlfile_path);
					EXPECT_FALSE(res.GetCode());

					res = puser->UpdateRecipients(nxlfile, { "xiaozhuzhu@11.com" });


					res = puser->GetActivityInfo(filename, infoVec);
					EXPECT_FALSE(res.GetCode());
				}
				cout << "Generated NXL file successful!" << endl;
			}
			else {
				EXPECT_FALSE(res.GetCode());
				cout << "User protect local file failed!" << endl;
			}
			puser->LogoutUser();
			pInstance->Save();
			SDWLibDeleteRmcInstance(pInstance);
		}
		else {
			cout << "login failed!" << endl;
			EXPECT_TRUE(false);
		}

		//Switch user to protect file
		ISDRmUser *puser_b = NULL;
		ISDRmcInstance *pInstance_b = NULL;
		ISDRmTenant *pTenant_b = NULL;

		//login_result = Switch_User(pInstance_b, puser_b, email_b, password);
		login_result = SDKRPM_Login(pInstance_b, pTenant_b, puser_b, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
		if (login_result) {
			wstring nxlfile_path;
			res = puser_b->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
			if (res.GetCode() == 0) {
				std::wstring filename;
				bool delete_result;
				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
				SDR_FILE_ACTIVITY_INFO ac_info;
				ISDRmNXLFile* nxlfile;
				res = puser_b->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					bool is_open = nxlfile->IsOpen();
					EXPECT_TRUE(is_open);
					//Get file name
					filename = nxlfile->GetFileName();
					//Get expiration 
					SDR_Expiration exp = nxlfile->GetExpiration();
					uint64_t end = exp.end;
					uint64_t start = exp.start;
					EXPECT_EQ(end, expire.end);

					//Get watermark
					SDR_WATERMARK_INFO wm = nxlfile->GetWaterMark();
					EXPECT_STREQ(wm.text.c_str(), watermark_info.text.c_str());
					//Get tag
					string tag = nxlfile->GetTags();
					EXPECT_STREQ(tag.c_str(), tag_text.c_str());
					//Get rights
					vector<SDRmFileRight> rights_v = nxlfile->GetRights();
					EXPECT_EQ(rights_v.size(), rights.size());

					//Upload file
					res = puser_b->UploadFile(nxlfile_path, nxlfile_path);
					EXPECT_FALSE(res.GetCode());

				}
			}
			else {
				EXPECT_FALSE(res.GetCode());
				cout << "User protect local file failed!" << endl;
			}
		}
		else {
			cout << "login failed!" << endl;
			EXPECT_TRUE(false);
		}
		pInstance_b->Save();
		SDWLibDeleteRmcInstance(pInstance_b);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
INSTANTIATE_TEST_CASE_P(spinUserProtectTesting, spinUserProtectTest, testing::ValuesIn(csv_protect_spin));

vector<IExpiryType> csv_expiration_spin = { RANGEEXPIRE, RELATIVEEXPIRE, ABSOLUTEEXPIRE };
class spinUserDiffExpirationLocalTest :public::testing::TestWithParam<IExpiryType> {};
TEST_P(spinUserDiffExpirationLocalTest, DiffExpirationTest) {
	try {
		wstring localfile_pathw = plan_file_path + txtnormal_testfile;
		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		wstring nxlfile_path;

		IExpiryType para = GetParam();
		SDR_Expiration expire;
		expire.type = para;
		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;
		expire.start = (uint64_t)std::time(nullptr) * 1000;

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email, password);
		if (login_result) {
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermarkinfo, expire, tags);
			if (!res.GetCode()) {
				bool delete_result;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					wstring name = nxlfile->GetFileName();
					//Check expiration time
					SDR_Expiration ex = nxlfile->GetExpiration();
					string mem_id = puser->GetMembershipID(1);
					cout << "mem_id: " << mem_id << endl;
					bool expired_result = nxlfile->CheckExpired(mem_id);
					EXPECT_FALSE(expired_result);
					if (expire.type == 1)
					{
						expire.type = ABSOLUTEEXPIRE;
					}
					EXPECT_EQ(ex.type, expire.type);
					if (expire.type) {
						uint64_t start = ex.start;
						if (expire.type == 3) {
							uint64_t start = ex.start;
							EXPECT_EQ(ex.start, expire.start);
						}
						uint64_t end = ex.end;
						EXPECT_EQ(ex.end, expire.end);
					}
				}
			}
			else {
				cout << "Protect file failed!";
				EXPECT_FALSE(res.GetCode());
			}
		}
		else {
			cout << "Login failed!" << endl;
			EXPECT_TRUE(login_result);
		}
		res = pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);

		//Get previous user
		ISDRmcInstance *pInstance_pre = NULL;
		ISDRmUser *puser_pre = NULL;
		string passcode_global = ReadFromFile(plan_path + passcode_txt);
		login_result = PreviousUser_Login(pInstance_pre, puser_pre, DEFUALT_ROUTER, L"", email, passcode_global);
		if (login_result) {
			ISDRmNXLFile* nxlfile1;
			res = puser->OpenFile(nxlfile_path, &nxlfile1);
			if (nxlfile1)
			{
				//Check expiration time
				SDR_Expiration ex = nxlfile1->GetExpiration();
				wstring name = nxlfile1->GetFileName();
				EXPECT_EQ(ex.type, expire.type);
				if (expire.type) {
					uint64_t start = ex.start;
					uint64_t end = ex.end;
					EXPECT_EQ(ex.end, expire.end);
				}
				//Upload file
				res = puser_pre->UploadFile(nxlfile_path, nxlfile_path);
				EXPECT_FALSE(res.GetCode());

				SDR_Expiration ex_uploaded = nxlfile1->GetExpiration();
				EXPECT_EQ(ex_uploaded.type, expire.type);
				if (expire.type) {
					uint64_t start = ex_uploaded.start;
					uint64_t end = ex_uploaded.end;
					EXPECT_EQ(ex_uploaded.end, expire.end);
				}
			}
		}
		else {
			cout << "Get Previous user failed!" << endl;
			EXPECT_TRUE(0);
		}
		pInstance_pre->Save();
		SDWLibDeleteRmcInstance(pInstance_pre);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
INSTANTIATE_TEST_CASE_P(spinUserDiffExpirationLocalTesting, spinUserDiffExpirationLocalTest, testing::ValuesIn(csv_expiration_spin));


TEST(spinUserChangeAdminRightsTest, ChangeAdminRightsTest1) {
	try {
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		string default_tenantid;
		string default_tenantname;
		string tenantAdmin_email = email;
		string projectAdmin_email = email_feng;
		int p_id;
		string membership;
		wstring file_name;
		wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
		string profile_pathid;
		string tokenGroupName;
		int rights_eval;
		bool is_member = false;

		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;

		string tag_original;
		string tag_new;
		wstring localfile_pathw = plan_file_path + xlsnormal_testfile;
		std::vector<SDRmFileRight> rights;

		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };
		wstring nxlfile_path;
		createFolder(ws2s(projectdownloaded_path));

		bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email_admin, password_admin, "2");
		if (login_result) {
			//Get Default tenantid and tenant name
			bool tenant_listresult = TenantListResponseData();
			Json::Value jroot;
			jroot = ParseJsonFile(tenantList_return);
			int total_tenant = jroot["results"]["details"]["totalTenants"].asInt();
			for (int i = 0; i < total_tenant; i++) {
				bool is_default = jroot["results"]["details"]["tenantList"][i]["isDefault"].asBool();
				if (is_default) {
					default_tenantid = jroot["results"]["details"]["tenantList"][i]["id"].asString();
					default_tenantname = jroot["results"]["details"]["tenantList"][i]["name"].asString();
				}
			}
			//Make sure email not in project admin list
			bool addProjectAdmin_result = AddRemoveProjectAdminResponseData(default_tenantid, false, email);
			//Add email to tenant admin list
			bool addTenantAdmin_result = AddRemoveTenantAminResponseData(default_tenantname, true, email);

			//Add email_zhu to project admin list
			addProjectAdmin_result = AddRemoveProjectAdminResponseData(default_tenantid, true, email_feng);
			//Make sure email_zhu not in tenant admin list
			addTenantAdmin_result = AddRemoveTenantAminResponseData(default_tenantname, false, email_feng);
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User login failed!" << endl;
		}

		//Tenant admin and project owner
		login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email, password);
		if (login_result) {
			res = puser->GetHeartBeatInfo();
			EXPECT_EQ(res.GetCode(), 0);
			checkSDWLResult(res);
			Json::Value jroot;
			jroot = ParseJsonFile(user_respond);

			if (jroot["extra"]["memberships"].size() == 1) {
				cout << "No projects from this user!" << endl;
				return;
			}
			else {
				for (int i = 0; i < jroot["extra"]["memberships"].size(); i++) {
					int type = jroot["extra"]["memberships"][i]["type"].asInt();
					if (type == 1) {
						bool is_owner = is_projectowner(jroot["extra"]["memberships"][i]["projectId"].asInt(), email_feng);
						if (!is_owner) {
							membership = jroot["extra"]["memberships"][i]["id"].asString();
							p_id = jroot["extra"]["memberships"][i]["projectId"].asInt();
							tokenGroupName = jroot["extra"]["memberships"][i]["tokenGroupName"].asString();
							break;
						}
					}
				}
			}

			if (p_id == 0)
			{
				cout << "No enough project can be use to test this case" << endl;
				return;
			}

			string mem_id = puser->GetMembershipID(p_id);

			vector<string> tags = get_tags(puser, tokenGroupName, mem_id);
			bool tag_valid = tag_is_valid(tags);
			if (!tag_valid) {
				cout << "There is no enough tag to do change rights" << endl;
				EXPECT_TRUE(false);
				return;
			}
			else {
				tag_original = tags[0];
				tag_new = tags[1];
			}

			res = puser->ProtectFile(plan_file_path + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire, tag_original, mem_id);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (!res.GetCode()) {
				ISDRFiles* pFile = NULL;
				std::wstring filename;
				bool delete_result;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				file_name = nxlfile->GetFileName();

				res = puser->UploadProjectFile(p_id, L"/", nxlfile);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());

				//Download file from server
				res = puser->GetListProjects(1, 100, "-lastActionTime", RF_All);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				std::vector<SDR_PROJECT_INFO> project_info_list = puser->GetProjectsInfo();
				std::vector<SDR_PROJECT_FILE_INFO> listfiles;
				res = puser->GetProjectListFiles(p_id, 1, 20, "-creationTime", "/", ws2s(file_name), listfiles);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				profile_pathid = listfiles[0].m_pathid;
				res = puser->ProjectDownloadFile(p_id, profile_pathid, projectdownloaded_path, RD_ForOffline);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());

				//Tenant admin try to change file rights
				res = puser->ChangeRightsOfProjectFile(projectdownloaded_path, p_id, ws2s(file_name), "/", rights, watermark_info, expire, tag_new);
				EXPECT_STREQ(res.GetMsg().c_str(), "no permission to change file rights");
				EXPECT_EQ(res.GetCode(), 5);

				//Make sure email_zhu is invited member
				is_member = is_projectmember(p_id, email_feng);
				if (!is_member)
				{
					Project_InviteMemberResponseData(to_string(p_id), email_feng);
				}
			}
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User login failed!" << endl;
		}

		//Project owner invited by others
		login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email_feng, password);
		if (login_result) {
			res = puser->GetHeartBeatInfo();
			EXPECT_EQ(res.GetCode(), 0);
			checkSDWLResult(res);
			std::vector<SDR_PROJECT_INFO> projectsInfo;
			if (!is_member)
			{
				bool accept_result = Project_AcceptInvitation1(p_id);
			}
			cout << "GetListProjects: ";
			res = puser->GetListProjects(1, 100, "name", RF_All);
			projectsInfo = puser->GetProjectsInfo();
			res = puser->ChangeRightsOfProjectFile(projectdownloaded_path, p_id, ws2s(file_name), "/", rights, watermark_info, expire, tag_new);
			EXPECT_EQ(res.GetCode(), 0);
			checkSDWLResult(res);

			//Download protected file to verify new tag and expiration
			string t = "del \"" + ws2s(projectdownloaded_path) + "\"";
			system(t.c_str());
			projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
			res = puser->ProjectDownloadFile(p_id, profile_pathid, projectdownloaded_path, RD_ForOffline);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			ISDRmNXLFile *nxlfile;
			res = puser->OpenFile(projectdownloaded_path, &nxlfile);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			string tag_changed = nxlfile->GetTags();
			EXPECT_STREQ(tag_changed.c_str(), tag_new.c_str());
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User login failed!" << endl;
		}

		p_id = 0;
		tag_original = "";
		tag_new = "";
		projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
		//Project owner
		login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email_feng, password);
		if (login_result) {
			Json::Value jroot;
			jroot = ParseJsonFile(user_respond);

			if (jroot["extra"]["memberships"].size() == 1) {
				cout << "No projects from this user!" << endl;
				return;
			}
			else {
				for (int i = 0; i < jroot["extra"]["memberships"].size(); i++) {
					bool is_owner = is_projectowner(jroot["extra"]["memberships"][i]["projectId"].asInt(), email_feng);
					if ((jroot["extra"]["memberships"][i]["projectId"].asInt() != 1) && (is_owner)) {
						membership = jroot["extra"]["memberships"][i]["id"].asString();
						p_id = jroot["extra"]["memberships"][i]["projectId"].asInt();
						tokenGroupName = jroot["extra"]["memberships"][i]["tokenGroupName"].asString();
						break;
					}
				}
				if (p_id == 0)
				{
					cout << "No enough project can be use to test this case" << endl;
					return;
				}
			}

			string mem_id = puser->GetMembershipID(p_id);

			vector<string> tags = get_tags(puser, tokenGroupName, mem_id);
			bool tag_valid = tag_is_valid(tags);
			if (!tag_valid) {
				cout << "There is no enough tag to do change rights" << endl;
				EXPECT_TRUE(false);
				return;
			}
			else {
				tag_original = tags[0];
				tag_new = tags[1];
			}

			nxlfile_path = L"";
			res = puser->ProtectFile(plan_file_path + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire, tag_original, mem_id);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (!res.GetCode()) {
				ISDRFiles* pFile = NULL;
				std::wstring filename;
				bool delete_result;
				//res = puser->GetLocalFileManager(&pFile);

				//ISDRmNXLFile* nxlfile = pFile->GetFile(0);

				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				file_name = nxlfile->GetFileName();

				res = puser->UploadProjectFile(p_id, L"/", nxlfile);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());

				//Download file from server
				res = puser->GetListProjects(1, 100, "-lastActionTime", RF_All);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				std::vector<SDR_PROJECT_INFO> project_info_list = puser->GetProjectsInfo();
				std::vector<SDR_PROJECT_FILE_INFO> listfiles;
				res = puser->GetProjectListFiles(p_id, 1, 20, "-creationTime", "/", ws2s(file_name), listfiles);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				profile_pathid = listfiles[0].m_pathid;
				res = puser->ProjectDownloadFile(p_id, profile_pathid, projectdownloaded_path, RD_ForOffline);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());

				//Project owner try to change file rights
				res = puser->ChangeRightsOfProjectFile(projectdownloaded_path, p_id, ws2s(file_name), "/", rights, watermark_info, expire, tag_new);
				checkSDWLResult(res);
				EXPECT_EQ(res.GetCode(), 0);

				ISDRmNXLFile *nxlfile1;
				res = puser->OpenFile(projectdownloaded_path, &nxlfile1);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				string tag_changed = nxlfile1->GetTags();
				EXPECT_STREQ(tag_changed.c_str(), tag_new.c_str());
			}
			else {
				cout << "protect file fail!" << endl;
				EXPECT_TRUE(false);
			}
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User login failed!" << endl;
		}
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//ChangeRightsOfFileTest
TEST(spinUserChangeAdminRightsTest, ChangeAdminRightsTest2) {
	ISDRmcInstance * pInstance;
	ISDRmTenant * pTenant;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	string default_tenantid;
	string default_tenantname;
	string tenantAdmin_email = email;
	string projectAdmin_email = email_feng;
	wstring localfile_pathw = plan_file_path + bmpnormal_testfile;
	std::vector<SDRmFileRight> rights;
	wstring nxlfile_path;
	wstring cpnxlfile_path;
	SDR_Expiration expire;
	expire.type = IExpiryType::NEVEREXPIRE;
	bool adhoc;
	bool original = false;
	bool newly = false;
	string tag_original = "";
	string tag_new = "";
	string tenan_str;
	int heartbeat;
	wstring copy_file;

	int p_id;
	string membership;
	wstring file_name;
	wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
	string tokenGroupName;
	int rights_eval;

	SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };

	int sysprojectid;
	string sysprojecttenantid;

	try {
		//Tenant admin change rights for system bucket file
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			res = puser->GetHeartBeatInfo();

			int heartbeat;
			string modified;
			string tenan_str;
			ISDRmNXLFile * file;
			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
			int sysprojectid;
			string sysprojecttenantid;

			res = puser->GetListProjects(1, 10, "-lastActionTime", RF_All);
			res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid, tenan_str);
			string default_tokengroup = puser->GetDefaultTokenGroupName();
			string mem_id = puser->GetMembershipID(sysprojecttenantid);
			//Json::Value jroot;
			//jroot = ParseJsonFile(user_respond);
			//int mem_count = jroot["extra"]["memberships"].size();
			//for (int i = 0; i < mem_count; i++) {
			//	int type = jroot["extra"]["memberships"][i]["projectId"].asInt();
			//	if (p_id == 1)
			//	{
			//		tokenGroupName = jroot["extra"]["memberships"][i]["tokenGroupName"].asString();
			//		break;
			//	}
			//}

			vector<string> tags = get_tags(puser, default_tokengroup, mem_id);
			bool tag_valid = tag_is_valid(tags);
			if (!tag_valid) {
				cout << "There is no enough tag to do change rights" << endl;
				EXPECT_TRUE(false);
				return;
			}
			else {
				tag_original = tags[0];
				tag_new = tags[1];
			}

			res = puser->ProtectFile(paln_file_ws + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire, tag_original, mem_id);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			ISDRmNXLFile *nxlfile;
			res = puser->OpenFile(nxlfile_path, &nxlfile);
			file_name = nxlfile->GetFileName();

			wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
			copy_file = downloaded_path + file_name;

			pInstance->Save();
			SDWLibDeleteRmcInstance(pInstance);

		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User login failed!" << endl;
		}

		string cmd_str = "copy /Y \"" + ws2s(nxlfile_path) + "\"  \"" + ws2s(copy_file) + "\"";
		system(cmd_str.c_str());

		string del_cmd = "del \"" + ws2s(nxlfile_path) + "\"";
		system(del_cmd.c_str());
		string passcode_filepath = plan_path + passcode_txt;
		string passcode = ReadFromFile(passcode_filepath);

		//Tenant admin(file owner) to change rights
		bool result = PreviousUser_Login(pInstance, puser, DEFUALT_ROUTER, L"", email, passcode);
		if (result) {
			res = puser->ChangeRightsOfFile(copy_file, rights, watermarkinfo, expire, tag_new);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			ISDRmNXLFile *nxlfile1;
			res = puser->OpenFile(copy_file, &nxlfile1);
			string tag_chaned = nxlfile1->GetTags();
			EXPECT_STREQ(tag_chaned.c_str(), tag_new.c_str());
		}
		else {
			EXPECT_TRUE(result);
			cout << "User login failed!" << endl;
		}

		//Remove tenant admin for email user
		login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");
		if (login_result) {
			//Make sure email not in tenant admin list
			//Get Default tenantid and tenant name
			bool tenant_listresult = TenantListResponseData();
			Json::Value jroot;
			jroot = ParseJsonFile(tenantList_return);
			int total_tenant = jroot["results"]["details"]["totalTenants"].asInt();
			for (int i = 0; i < total_tenant; i++) {
				bool is_default = jroot["results"]["details"]["tenantList"][i]["isDefault"].asBool();
				if (is_default) {
					default_tenantid = jroot["results"]["details"]["tenantList"][i]["id"].asString();
					default_tenantname = jroot["results"]["details"]["tenantList"][i]["name"].asString();
				}
			}
			bool addTenantAdmin_result = AddRemoveTenantAminResponseData(default_tenantname, false, email);
		}
		else {
			EXPECT_TRUE(result);
			cout << "User login failed!" << endl;
		}

		//Fileowner(not tenant admin) to change file rights
		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			res = puser->ChangeRightsOfFile(copy_file, rights, watermarkinfo, expire, tag_original);
			EXPECT_EQ(61843, res.GetCode());
			EXPECT_STREQ("Unauthorized to classify", res.GetMsg().c_str());
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User login failed!" << endl;
		}

		//Project owner(not tenant owner) to change file rights
		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
		if (login_result) {
			res = puser->ChangeRightsOfFile(copy_file, rights, watermarkinfo, expire, tag_original);
			EXPECT_EQ(5, res.GetCode());
			EXPECT_STREQ("no permission to change file rights", res.GetMsg().c_str());
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User login failed!" << endl;
		}
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

int invited_p_id = 66;
int create_p_id = 25;
int ABAC_p_id = 31;
TEST(spinUserProtectWithMem, ProtectWithMem) {
	try {
		//SDR_Expiration expire;
		//expire.type = IExpiryType::RELATIVEEXPIRE;
		//expire.end = 1536422399999;
		//expire.start = 1536227926000;

		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		string tenant = ws2s(DEFUALT_TENANT);
		string tag_text = "{}";
		string membership = "";
		wstring w_name;
		wstring localfile_pathw = plan_file_path + bmpnormal_testfile;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_DOWNLOAD);
		std::vector<SDRmFileRight> rights1;

		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };

		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
		wstring file_name;
		uint32_t pageId = 1;
		uint32_t pageSize = 10;
		std::string orderBy = "name";
		RM_ProjectFilter filter = RF_All;
		uint32_t p_id;
		string tanen_str;
		string promem_id;
		string tanent_name;
		string tenant_id;
		wstring nxlfile_path=L"C:\\zhuzhu";
		string tag_text2 = " ";
		//bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {

			vector<string> project_msg = get_project_msg(invited_p_id, true);
			if (project_msg.size() == 0 || project_msg[0] == "") {
				cout << "No projects from this user!" << endl;
				return;
			}
			membership = project_msg[3];
			tanent_name = project_msg[4];
			//res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
			//std::vector<SDR_PROJECT_INFO> projects = puser->GetProjectsInfo();
			//for (int i = 0; i < projects.size(); i++) {
			//	if (projects[i].projid == p_id) {
			//		promem_id = projects[i]
			//	}
			//}
			string mem_id = puser->GetMembershipID(invited_p_id);
			EXPECT_STREQ(mem_id.c_str(), membership.c_str());
			string mem_id2 = puser->GetMembershipID(tanent_name);
			EXPECT_STREQ(mem_id2.c_str(), membership.c_str());
			string mem_id3 = puser->GetMembershipIDByTenantName(tanent_name);
			EXPECT_STREQ(mem_id3.c_str(), membership.c_str());

			/*vector<SDR_CLASSIFICATION_CAT> categories;
			bool original = false;
			bool newly = false;
			res = puser->GetClassification(tanent_name, categories);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());*/

			vector<string> tags = get_tags(puser, tanent_name, mem_id);
			bool tag_valid = tag_is_valid(tags);
			if (!tag_valid) {
				cout << "------There is no enough tag------" << endl;
				EXPECT_TRUE(false);
				return;
			}
			else {
				tag_text2 = tags[0];
			}

			//protect file to invited by me
			//string tag_text2 = "{\"Rights\":[\"Rights_All\"]}";
			res = puser->ProtectFile(plan_file_path + txtnormal_testfile, nxlfile_path, rights1, watermark_info, expire, tag_text2, mem_id);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (!res.GetCode()) {
				//ISDRFiles* pFile = NULL;
				std::wstring filename;
				bool delete_result;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);

				res = puser->UploadProjectFile(invited_p_id, L"/", nxlfile);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				file_name = nxlfile->GetFileName();
				wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
				std::transform(file_name.begin(), file_name.end(), file_name.begin(), ::tolower);
				res = puser->ProjectDownloadFile(invited_p_id, "/" + ws2s(file_name), projectdownloaded_path, RD_ForOffline);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				wstring projectdownloaded_path2 = L"C:\\auto\\output\\SDK\\Project\\";
				std::transform(file_name.begin(), file_name.end(), file_name.begin(), ::tolower);
				string filename_lower = ws2s(file_name);
				res = puser->ProjectDownloadPartialFile(invited_p_id, "/" + filename_lower, projectdownloaded_path2, RD_ForOffline);
				checkSDWLResult(res);
				EXPECT_EQ(res.GetCode(), 0);
				//Get watermark
				SDR_WATERMARK_INFO wm = nxlfile->GetWaterMark();
				EXPECT_STREQ(wm.text.c_str(), watermark_info.text.c_str());
				//Get tag
				string tag = nxlfile->GetTags();
				EXPECT_STREQ(tag.c_str(), tag_text2.c_str());
				//Get rights
				vector<SDRmFileRight> rights_v = nxlfile->GetRights();
				EXPECT_EQ(rights_v.size(), rights1.size());
				bool rights_result = rights_verified(rights1, nxlfile);
				EXPECT_TRUE(rights_result);
				res = puser->CloseFile(nxlfile);
			}
			//protect file to create by owner
			vector<string> project_msg_owner = get_project_msg(create_p_id, true);
			if (project_msg_owner.size() == 0 || project_msg_owner[0] == "") {
				cout << "No projects from this user!" << endl;
				return;
			}
			string mem_id_owner = project_msg_owner[3];
			string tanent_name_owner = project_msg_owner[4];
			vector<string> tags_owner = get_tags(puser, tanent_name_owner, mem_id_owner);
			bool tag_valid_owner = tag_is_valid(tags_owner);
			if (!tag_valid_owner) {
				cout << "------There is no enough tag------" << endl;
				EXPECT_TRUE(false);
				return;
			}
			else {
				tag_text2 = tags[0];
			}
			//string mem_id_owner = puser->GetMembershipID(create_p_id);
			nxlfile_path = L"";
			res = puser->ProtectFile(plan_file_path + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire, tag_text2, mem_id_owner);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (!res.GetCode()) {
				//ISDRFiles* pFile = NULL;
				std::wstring filename;
				bool delete_result;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);

				res = puser->UploadProjectFile(create_p_id, L"/", nxlfile);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				file_name = nxlfile->GetFileName();
				wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
				std::transform(file_name.begin(), file_name.end(), file_name.begin(), ::tolower);
				res = puser->ProjectDownloadFile(create_p_id, "/" + ws2s(file_name), projectdownloaded_path, RD_Normal);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				//Get watermark
				SDR_WATERMARK_INFO wm = nxlfile->GetWaterMark();
				EXPECT_STREQ(wm.text.c_str(), watermark_info.text.c_str());
				//Get tag
				string tag = nxlfile->GetTags();
				EXPECT_STREQ(tag.c_str(), tag_text2.c_str());
				//Get rights
				vector<SDRmFileRight> rights_v = nxlfile->GetRights();
				EXPECT_EQ(rights_v.size(), rights.size());
				bool rights_result = rights_verified(rights, nxlfile);
				EXPECT_TRUE(rights_result);
				res = puser->CloseFile(nxlfile);
			}

			//protect file to ABAC
			string mem_id_ABAC = puser->GetMembershipID(ABAC_p_id);
			nxlfile_path = L"";
			res = puser->ProtectFile(plan_file_path + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire, tag_text, mem_id_ABAC);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (!res.GetCode()) {
				//ISDRFiles* pFile = NULL;
				std::wstring filename;
				bool delete_result;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);

				res = puser->UploadProjectFile(ABAC_p_id, L"/", nxlfile);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				file_name = nxlfile->GetFileName();
				wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
				std::transform(file_name.begin(), file_name.end(), file_name.begin(), ::tolower);
				res = puser->ProjectDownloadFile(ABAC_p_id, "/" + ws2s(file_name), projectdownloaded_path, RD_Normal);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				//Get watermark
				SDR_WATERMARK_INFO wm = nxlfile->GetWaterMark();
				EXPECT_STREQ(wm.text.c_str(), watermark_info.text.c_str());
				//Get tag
				string tag = nxlfile->GetTags();
				EXPECT_STREQ(tag.c_str(), tag_text.c_str());
				//Get rights
				vector<SDRmFileRight> rights_v = nxlfile->GetRights();
				EXPECT_EQ(rights_v.size(), rights.size());
				bool rights_result = rights_verified(rights, nxlfile);
				EXPECT_TRUE(rights_result);
				res = puser->CloseFile(nxlfile);
			}

			res = puser->LogoutUser();
			res = pInstance->RPMLogout();
		}
		else {
			cout << "Login Failed!" << endl;
			EXPECT_TRUE(login_result);
		}
		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl;

		//ISDRmcInstance *pInstance_pre1 = NULL;
		//ISDRmUser *puser_pre1 = NULL;
		//string passcode_global = ReadFromFile(plan_path + passcode_txt);
		//bool login_result1 = PreviousUser_Login(pInstance_pre1, puser_pre1, DEFUALT_ROUTER, DEFUALT_TENANT, email, passcode_global);
		//EXPECT_TRUE(login_result1);
		//wstring email = puser->GetEmail();
		//ISDRmcInstance *pInstance_b = NULL;
		//ISDRmTenant *pTenant_b = NULL;
		//ISDRmUser *puser_b = NULL;
		//login_result = Default_Login(pInstance_b, pTenant_b, puser_b, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		//if (login_result) {
		//	ISDRmNXLFile* nxlfile;
		//	res = puser_b->OpenFile(nxlfile_path, &nxlfile);
		//	checkSDWLResult(res);
		//	EXPECT_EQ(0, res.GetCode());
		//	wstring filename = nxlfile->GetFileName();
		//	//Get watermark
		//	SDR_WATERMARK_INFO wm = nxlfile->GetWaterMark();
		//	EXPECT_STREQ(wm.text.c_str(), watermark_info.text.c_str());
		//	//Get tag
		//	string tag = nxlfile->GetTags();
		//	EXPECT_STREQ(tag.c_str(), tag_text.c_str());
		//	//Get rights
		//	vector<SDRmFileRight> rights_v = nxlfile->GetRights();
		//	EXPECT_EQ(rights_v.size(), rights.size());
		//	bool rights_result = rights_verified(rights, nxlfile);
		//	EXPECT_TRUE(rights_result);
		//	res = puser_b->LogoutUser();
		//}
		//else {
		//	cout << "Login Failed!" << endl;
		//	EXPECT_TRUE(login_result);
		//}
		//SDWLibDeleteRmcInstance(pInstance_b);

		//ISDRmcInstance *pInstance_c = NULL;
		//ISDRmTenant *pTenant_c = NULL;
		//ISDRmUser *puser_c = NULL;
		//login_result = Default_Login(pInstance_c, pTenant_c, puser_c, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		//if (login_result) {
		//	ISDRmNXLFile* nxlfile;
		//	res = puser_c->OpenFile(nxlfile_path, &nxlfile);
		//	EXPECT_STREQ(res.GetMsg().c_str(), "Access denied: Invalid Membership");
		//	EXPECT_EQ(res.GetCode(), 61843);

		//	res = puser_c->LogoutUser();
		//}
		//else {
		//	cout << "Login Failed!" << endl;
		//	EXPECT_TRUE(login_result);
		//}
		//SDWLibDeleteRmcInstance(pInstance_c);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinUserGetNXLFileInfoTest, GetNXLFileInfoTest) {
	try {
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		string pathid;
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";

#pragma region[Prepare a MYVAULT nxl file]
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;

		std::vector<SDRmFileRight> rights;
		rights = Tick_Rights("view|share|download|print");

		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
		std::wstring filename;
		wstring recipients = s2ws(email_zhu);
		wstring nxlfile_path;
		res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire);

		//ISDRFiles* pFile = NULL;
		bool delete_result;
		//res = puser->GetLocalFileManager(&pFile);
		//size_t no = pFile->GetListNumber();
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();
			puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Test comments");
			//Remove file
			//delete_result = pFile->RemoveFile(nxlfile);
			//EXPECT_TRUE(delete_result);
		}
		else {
			cout << "No file generated!" << endl;
			EXPECT_FALSE(res.GetCode());
		}


		bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
		if (file_result) {
			Json::Value jroot;
			jroot = ParseJsonFile(myvaultfilelist_return);
			pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
		}
		res = puser->MyVaultDownloadFile(pathid, downloaded_path, 2);

#pragma endregion	

		ISDRmNXLFile* nxlfile1;
		res = puser->OpenFile(downloaded_path, &nxlfile1);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		string tenantName = nxlfile1->GetTenantName();
		string membership = puser->GetMembershipIDByTenantName(tenantName);
		bool owner = nxlfile1->IsOwner(membership);
		EXPECT_TRUE(owner);
		int size = nxlfile1->GetFileSize();
		cout << "file size: " << size << endl;
		if (size < 16 * 1024) {
			EXPECT_TRUE(false);
		}
		string create_by = nxlfile1->Get_CreatedBy();
		EXPECT_STREQ(membership.c_str(), create_by.c_str());
		uint64_t create_date = nxlfile1->Get_DateCreated();
		uint64_t modified_date = nxlfile1->Get_DateModified();
		string modified_by = nxlfile1->Get_ModifiedBy();
		string watermark_str = nxlfile1->GetAdhocWaterMarkString();
		EXPECT_STREQ(watermark_info.text.c_str(), watermark_str.c_str());
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinUserMyDriveInfoTest, DriveInfoTest) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		uint64_t result_code;
		uint64_t usage;
		uint64_t totalquota;
		uint64_t vaultusage;
		uint64_t vaultquota;
		uint64_t usage_api;
		uint64_t totalquota_api;
		uint64_t vaultusage_api;
		uint64_t vaultquota_api;
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email, password);

		if (login_result) {
			res = puser->UpdateMyDriveInfo();
			res = puser->GetMyDriveInfo(usage, totalquota, vaultusage, vaultquota);
			wcout << L"\t Usage:"; wcout << usage << endl;
			wcout << L"\t Totat Quota:"; wcout << totalquota << endl;
			wcout << L"\t Vault Usage:"; wcout << vaultusage << endl;
			wcout << L"\t Vault Quota:"; wcout << vaultquota << endl;
			EXPECT_FALSE(res.GetCode());
			bool  i = GetMyDriveInfoResponseData(result_code, usage_api, totalquota_api, vaultusage_api, vaultquota_api);
			if (result_code == 200)
			{
				EXPECT_EQ(usage_api, usage);
				EXPECT_EQ(totalquota_api, totalquota);
				EXPECT_EQ(vaultusage_api, vaultusage);
				EXPECT_EQ(vaultquota_api, vaultquota);
			}

			cout << "Got MyDrive Info!" << endl;
			res = puser->LogoutUser();
			pInstance->Save();
			SDWLibDeleteRmcInstance(pInstance);
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User Login failed!" << endl;
		}
		cout << "-------------This case finished--------" << endl;

		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
		if (login_result) {
			res = puser->UpdateMyDriveInfo();
			res = puser->GetMyDriveInfo(usage, totalquota, vaultusage, vaultquota);
			wcout << L"\t Usage:"; wcout << usage << endl;
			wcout << L"\t Totat Quota:"; wcout << totalquota << endl;
			wcout << L"\t Vault Usage:"; wcout << vaultusage << endl;
			wcout << L"\t Vault Quota:"; wcout << vaultquota << endl;
			EXPECT_FALSE(res.GetCode());
			bool  i = GetMyDriveInfoResponseData(result_code, usage_api, totalquota_api, vaultusage_api, vaultquota_api);
			if (result_code == 200)
			{
				EXPECT_EQ(usage_api, usage);
				EXPECT_EQ(totalquota_api, totalquota);
				EXPECT_EQ(vaultusage_api, vaultusage);
				EXPECT_EQ(vaultquota_api, vaultquota);
			}

			EXPECT_FALSE(res.GetCode());
			cout << "Got MyDrive Info!" << endl;
			res = puser->LogoutUser();
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User Login failed!" << endl;
		}
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinUserAddActivityLogTest, AddActivityLogTest) {
	try {
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		string pathid;
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
		createFolder(ws2s(downloaded_path));
		string rights_str = "view|download|edit|saveas|share";

#pragma region[Prepare a MYVAULT nxl file]
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;

		std::vector<SDRmFileRight> rights;
		rights = Tick_Rights(rights_str);

		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
		std::wstring filename;
		wstring recipients = s2ws(email_zhu);
		wstring nxlfile_path;
		res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire);

		bool delete_result;
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();
			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Test comments");
		}
		else {
			cout << "No file generated!" << endl;
			EXPECT_FALSE(res.GetCode());
		}

		bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
		if (file_result) {
			Json::Value jroot;
			jroot = ParseJsonFile(myvaultfilelist_return);
			pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
		}
		res = puser->MyVaultDownloadFile(pathid, downloaded_path, 2);

#pragma endregion	
		//When file not found
		res = puser->AddActivityLog(L"zhuzhu", RL_OPrint, RL_RDenied);
		EXPECT_EQ(res.GetCode(), 3);
		EXPECT_STREQ(res.GetMsg().c_str(), "File not found");
		res = puser->AddActivityLog(plan_pathw + invalid_testfile, RL_OPrint, RL_RDenied);
		EXPECT_EQ(res.GetCode(), 13);
		EXPECT_STREQ(res.GetMsg().c_str(), "Invalid nxl file");
		for (RM_ActivityLogOperation enu = RL_OProtect; enu <= RL_ODelete; enu = (RM_ActivityLogOperation)(enu + 1)) {
			res = puser->AddActivityLog(downloaded_path, enu, RL_RDenied);
			EXPECT_FALSE(res.GetCode());
		}

		ISDRmNXLFile * file1;
		res = puser->OpenFile(downloaded_path, &file1);
		filename = file1->GetFileName();
		EXPECT_FALSE(res.GetCode());
		for (RM_ActivityLogOperation enu = RL_OProtect; enu <= RL_ODelete; enu = (RM_ActivityLogOperation)(enu + 1)) {
			res = puser->AddActivityLog(file1, enu, RL_RDenied);
			EXPECT_FALSE(res.GetCode());
		}

		Sleep(330000);
		//searchField. can be empty. Supported values are email, operation, deviceId 
		//res = puser->GetNXLFileActivitylog(file1, 0, 100, 0, "", 0, true);
		res = puser->GetNXLFileActivityLog(file1, 0, 100, RL_SFEmpty, "", RL_OBAccessTime, true);
		//When file not found
		res = puser->GetActivityInfo(L"zhuzhu333.txt", infoVec);
		EXPECT_EQ(res.GetCode(), 1168);
		EXPECT_STREQ(res.GetMsg().c_str(), "File not found");
		res = puser->GetActivityInfo(filename, infoVec);
		EXPECT_EQ(infoVec.size(), 31);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinSharedWithMeDownloadPartialFileTest, SharedWithMeDownloadPartialFileTest) {
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmUser *puser_zhu = NULL;
	ISDRmTenant *pTenant = NULL;
	bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	SDR_Expiration expire;
	expire.type = IExpiryType::NEVEREXPIRE;
	wstring localfile_pathw = plan_file_path + txtnormal_testfile;
	wstring nxlfile_path = L"";
	wstring transactioncode;
	wstring transactionid;
	wstring duid;
	string pathid;
	std::wstring filename;
	std::vector<SDRmFileRight> rights;
	rights.push_back(RIGHT_VIEW);
	rights.push_back(RIGHT_DOWNLOAD);

	string tag_text = "{}";
	wstring comments_w = L"test";
	wstring recipients = s2ws(email_feng);
	SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };

	wstring targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
	wstring targert_folder2 = L"C:\\auto\\output\\SDK\\Project\\";
	//User A share file to User B
	res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
	if (res.GetCode() == 0) {
		ISDRFiles* pFile = NULL;

		bool delete_result;
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		bool revoked_owner;
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();

			//Upload file
			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comments Test!");
			EXPECT_EQ(res.GetCode(), 0);
		}
		else {
			cout << "Upload file failed!" << endl;
			EXPECT_FALSE(res.GetCode());
		}

		bool myvault_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "creationTime", "allFiles");
		Json::Value jroot;
		jroot = ParseJsonFile(myvaultfilelist_return);
		pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
	}
	else {
		cout << "Share file failed!" << endl;
		EXPECT_TRUE(false);
	}

	//User B login and download partial file
	login_result = SDKRPM_Login(pInstance, pTenant, puser_zhu, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
	if (!res.GetCode()) {
		vector<SDR_SHAREDWITHME_FILE_INFO> listfiles;
		res = puser_zhu->GetSharedWithMeFiles(1, 10, "sharedDate", ws2s(filename), listfiles);
		transactioncode = s2ws(listfiles[0].m_transactioncode);
		transactionid = s2ws(listfiles[0].m_transactionid);
		duid = s2ws(listfiles[0].m_duid);
		res = puser_zhu->SharedWithMeDownloadPartialFile(transactioncode, transactionid, targert_folder, true);
		EXPECT_EQ(res.GetCode(), 0);
		bool isfile_exist = file_exist(ws2s(targert_folder));
		EXPECT_TRUE(isfile_exist);
		bool revoked1;

		//When transactioncode is invalid
		res = puser_zhu->SharedWithMeDownloadPartialFile(L"123", transactionid, targert_folder, true);
		EXPECT_EQ(res.GetCode(), 400 + 61440);
		puser_zhu->LogoutUser();
	}
	//When file is invoked
	login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	bool revoke_result = InvokeSharedFileResponseData(ws2s(duid));
	puser->LogoutUser();

	login_result = SDKRPM_Login(pInstance, pTenant, puser_zhu, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
	res = puser_zhu->SharedWithMeDownloadPartialFile(transactioncode, transactionid, targert_folder2, true);
	EXPECT_EQ(res.GetCode(), 403 + 61440);
	EXPECT_STREQ(res.GetMsg().c_str(), "download failed");

	//when file is deleted
	login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	bool delete_result = DeleteMyvaultFile(ws2s(duid), pathid);
	login_result = Default_Login(pInstance, pTenant, puser_zhu, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
	res = puser_zhu->SharedWithMeDownloadPartialFile(transactioncode, transactionid, targert_folder2, true);
	EXPECT_EQ(res.GetCode(), 403 + 61440);
	EXPECT_STREQ(res.GetMsg().c_str(), "download failed");
	cout << "-------------This case finished--------" << endl;
}

TEST(spinUserUpdateAndGetUserPreferenceTest, UpdateAndGetUserPreference) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000 + 604800000;
		expire.start = (uint64_t)std::time(nullptr) * 1000 + 604800000;

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			//Option 1
			uint32_t option;
			uint64_t get_start;
			uint64_t get_end;
			wstring watermark;
			uint64_t start;
			uint64_t end;
			wstring set_watermark = L"zhuwenling";

			//Option 0
			res = puser->UpdateUserPreference(0, 0, 0, set_watermark);
			res = puser->GetUserPreference(option, get_start, get_end, watermark);
			EXPECT_EQ(0, option);
			EXPECT_STREQ(ws2s(set_watermark).c_str(), ws2s(watermark).c_str());
			//Option 1
			uint32_t year = 99, month = 1;
			start = ((uint64_t)year) << 32 | month;
			uint32_t week = 1, day = 27;
			end = ((uint64_t)week) << 32 | day;

			res = puser->UpdateUserPreference(1, start, end, L"$(User)\n$(Date) $(Time)zhuzhu");
			res = puser->GetUserPreference(option, get_start, get_end, watermark);
			EXPECT_EQ(1, option);
			EXPECT_EQ(start, get_start);
			EXPECT_EQ(end, get_end);
			EXPECT_STREQ(ws2s(L"$(User)\n$(Date) $(Time)zhuzhu").c_str(), ws2s(watermark).c_str());
			//Option 2
			res = puser->UpdateUserPreference(2, expire.start, expire.end, set_watermark);
			res = puser->GetUserPreference(option, get_start, get_end, watermark);
			EXPECT_EQ(2, option);
			EXPECT_STREQ(ws2s(set_watermark).c_str(), ws2s(watermark).c_str());
			EXPECT_EQ(expire.end, get_end);

			//Option 3
			res = puser->UpdateUserPreference(3, expire.start, expire.end, set_watermark);
			res = puser->GetUserPreference(option, get_start, get_end, watermark);
			EXPECT_EQ(3, option);
			EXPECT_STREQ(ws2s(set_watermark).c_str(), ws2s(watermark).c_str());
			EXPECT_EQ(expire.start, get_start);
			EXPECT_EQ(expire.end, get_end);
			cout << "-------------This case finished--------" << endl;
		}
		else
		{
			EXPECT_FALSE(res.GetCode());
			cout << "User login failed!" << endl;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinUserGetPreviousUserTest, GetPrevousUserTest) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		string email = "3057318205@qq.com";
		string password = "a1409d7e150f67df4e1fbccafe2ec5aa";
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		EXPECT_TRUE(login_result);
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);

		//Get previous user
		ISDRmcInstance *pInstance_pre = NULL;
		ISDRmUser *puser_pre = NULL;
		string passcode_global = ReadFromFile(plan_path + passcode_txt);
		login_result = PreviousUser_Login(pInstance_pre, puser_pre, DEFUALT_ROUTER, DEFUALT_TENANT, email, passcode_global);
		EXPECT_TRUE(login_result);
		res = puser_pre->LogoutUser();
		EXPECT_FALSE(res.GetCode());
		pInstance_pre->Save();
		SDWLibDeleteRmcInstance(pInstance_pre);

		//Login again 
		ISDRmcInstance *pInstance1 = NULL;
		ISDRmUser *puser1 = NULL;
		login_result = Default_Login(pInstance1, pTenant, puser1, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		EXPECT_TRUE(login_result);
		pInstance1->Save();
		SDWLibDeleteRmcInstance(pInstance1);

		//Get previous user
		ISDRmcInstance *pInstance_pre1 = NULL;
		ISDRmUser *puser_pre1 = NULL;
		passcode_global = ReadFromFile(plan_path + passcode_txt);
		bool login_result1 = PreviousUser_Login(pInstance_pre1, puser_pre1, DEFUALT_ROUTER, DEFUALT_TENANT, email, passcode_global);
		EXPECT_TRUE(login_result1);
		res = puser_pre1->LogoutUser();
		EXPECT_FALSE(res.GetCode());
		pInstance_pre1->Save();
		SDWLibDeleteRmcInstance(pInstance_pre1);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinUserUpdateInfoTest, UpdateInfoTest) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		uint64_t result_code;
		string display_name;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			bool result = GetUpdateUserInfo(result_code, display_name);
			if (result) {
				res = puser->UpdateUserInfo();
				if (res.GetCode() == 0) {
					wstring name = puser->GetName();
					wstring email_val = puser->GetEmail();
					EXPECT_STREQ(ws2s(email_val).c_str(), email.c_str());
					string name_s = ws2s(name);
					EXPECT_STREQ(name_s.c_str(), display_name.c_str());
					cout << "Update user info, the diaplay name is :  " + display_name << endl;
				}
				else {
					cout << "Update user info failed!" << endl;
					EXPECT_FALSE(res.GetCode());
				}
			}
		}
		else {
			cout << "User Login failed!" << endl;
		}
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinUserHeartBeatTest, HeartBeatTest) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		string w_text;
		string font_name;
		string fontColor;
		int fontSize;
		string rotation;
		bool repeat;
		int transpa;

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email, password);
		if (login_result) {
			bool heartbea_result = GetHeartBeatResponseData();
			if (heartbea_result) {
				Json::Value jroot;
				jroot = ParseJsonFile(hearbeat_return);
				string content = jroot["results"]["watermarkConfig"]["content"].asString();
				Json::Reader Reader;
				Json::Value devJson;
				Reader.parse(content, devJson);
				w_text = devJson["text"].asString();
				font_name = devJson["fontName"].asString();
				fontColor = devJson["fontColor"].asString();
				rotation = devJson["rotation"].asString();
				repeat = devJson["repeat"].asBool();
				transpa = devJson["transparentRatio"].asInt();
				fontSize = devJson["fontSize"].asInt();
			}
			std::string policybundle;
			bool bval = false;
			res = pInstance->GetCurrentTenant(&pTenant);
			res = puser->GetHeartBeatInfo();
			pInstance->Save();
			wstring tenantName;
			bval = puser->GetPolicyBundle(tenantName, policybundle);
			if (bval)
				cout << "policybundle: " << policybundle << endl;
			SDR_WATERMARK_INFO winfo = puser->GetWaterMarkInfo();
			EXPECT_STREQ(winfo.text.c_str(), w_text.c_str());
			EXPECT_STREQ(font_name.c_str(), winfo.fontName.c_str());
			EXPECT_STREQ(fontColor.c_str(), winfo.fontColor.c_str());
			EXPECT_EQ(fontSize, winfo.fontSize);
			//EXPECT_EQ(rotation, winfo.rotation);
			EXPECT_EQ(repeat, winfo.repeat);
			EXPECT_EQ(transpa, winfo.transparency);
			cout << endl;
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User Login failed!" << endl;
		}
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinUserGetTenantPreferenceTest, GetTenantPreferenceTest) {
	try {
		ISDRmcInstance * pInstance;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		ISDRmTenant *pTenant = NULL;
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email, password);

		if (login_result) {
			bool adhoc;
			int heartbeat;
			Json::Value jroot;
			jroot = ParseJsonFile(user_respond);
			string modified;
			string tenan_str;
			ISDRmNXLFile * file;
			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
			int sysprojectid;
			string sysprojecttenantid;
			res = puser->GetHeartBeatInfo();

			tenan_str = jroot["extra"]["tenantId"].asString();
			res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid);
			EXPECT_EQ(sysprojectid, 0);
			cout << heartbeat << endl;
			res = pInstance->Save();
			bool is_adhoc = puser->IsTenantAdhocEnabled();
			EXPECT_TRUE(is_adhoc);
			res = pInstance->Save();
			cout << "-------------This case finished--------" << endl;
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User login failed!" << endl;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}



TEST(spinProjectAndFileListTest, ProjectListAndFileTest) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		uint32_t pageId = 1;
		uint32_t pageSize = 5;
		std::string orderBy = "-name";
		vector<string> order = { "name","-name", "lastActionTime", "-lastActionTime" };
		RM_ProjectFilter filter = RF_All;
		string owedbyme;
		string page = to_string(pageId);
		string size = to_string(pageSize);
		int projectcount;
		int files_count;
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		//bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, "john.tyler", "john.tyler", "2");
		if (login_result) {
			for (RM_ProjectFilter f = RF_All; f <= RF_OwnedByOther; f = (RM_ProjectFilter)(f + 1)) {
				for (int i = 0; i < order.size(); i++) {
					orderBy = order[i];
					if (filter == RF_OwnedByMe)
					{
						owedbyme = "true";
					}
					else if (filter == RF_OwnedByOther) {
						owedbyme = "false";
					}
					else {
						owedbyme = "all";
					}
					//Get project list: "-name" "all"
					bool list_result = GetProjectListResponseData(page, size, orderBy, owedbyme);
					if (list_result) {
						Json::Value jroot;
						jroot = ParseJsonFile(projectlist_return);
						projectcount = jroot["results"]["detail"].size();
						if (!projectcount) {
							cout << "There is no projects from this user!" << endl;
							EXPECT_TRUE(projectcount);
							return;
						}
						res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
						std::vector<SDR_PROJECT_INFO> project_info_list = puser->GetProjectsInfo();
						EXPECT_EQ(project_info_list.size(), projectcount);
						for (int y = 0; y < projectcount; y++) {
							EXPECT_STREQ(project_info_list[y].displayname.c_str(), jroot["results"]["detail"][y]["displayName"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].description.c_str(), jroot["results"]["detail"][y]["description"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].name.c_str(), jroot["results"]["detail"][y]["name"].asString().c_str());
							EXPECT_EQ(project_info_list[y].bowner, jroot["results"]["detail"][y]["ownedByMe"].asBool());
							EXPECT_STREQ(project_info_list[y].tokengroupname.c_str(), jroot["results"]["detail"][y]["tokenGroupName"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].parenttenantid.c_str(), jroot["results"]["detail"][y]["parentTenantId"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].parenttenantname.c_str(), jroot["results"]["detail"][y]["parentTenantName"].asString().c_str());
							EXPECT_EQ(project_info_list[y].projid, jroot["results"]["detail"][y]["id"].asInt());
							uint64_t   projid = project_info_list[y].projid;
							//Project file info test
							vector<string> orderby_files = { "creationTime","-creationTime", "name", "-name" };

							for (int z = 0; z < 4; z++) {
								string filelist_orderby = orderby_files[z];
								bool filelist_result = GetProjectFilelistResponseData(to_string(projid), page, size, filelist_orderby, "/", "");
								if (filelist_result) {
									Json::Value jroot_filelist;
									jroot_filelist = ParseJsonFile(projectfilelist_return);
									files_count = jroot_filelist["results"]["detail"]["totalFiles"].asInt();
									std::vector<SDR_PROJECT_FILE_INFO> listfiles;
									res = puser->GetProjectListFiles(projid, pageId, pageSize, filelist_orderby, "/", "", listfiles);
									if (files_count) {
										//special operation, so add this if, because pagesize is 5
										if (files_count > listfiles.size()) {
											files_count = listfiles.size();
										}

										EXPECT_EQ(listfiles.size(), files_count);
										for (int j = 0; j < files_count; j++) {
											bool is_folder;
											Json::Value subjroot;
											subjroot = ParseJsonFile(projectfilelist_return);
											if (is_folder = subjroot["results"]["detail"]["files"][j]["folder"].asBool()) {
												cout << "Is folder: true!" << endl;
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["folder"].asBool(), listfiles[j].m_bfolder);
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["creationTime"].asInt64(), listfiles[j].m_created);
												EXPECT_STREQ("", listfiles[j].m_duid.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["id"].asString().c_str(), listfiles[j].m_fileid.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["lastModified"].asInt64(), listfiles[j].m_lastmodified);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["name"].asString().c_str(), listfiles[j].m_nxlname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["displayName"].asString().c_str(), listfiles[j].m_ownerdisplayname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["email"].asString().c_str(), listfiles[j].m_owneremail.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["owner"]["userId"].asInt64(), listfiles[j].m_ownerid);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathDisplay"].asString().c_str(), listfiles[j].m_pathdisplay.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathId"].asString().c_str(), listfiles[j].m_pathid.c_str());
												string path_id = listfiles[j].m_pathid;
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["size"].asInt64(), listfiles[j].m_size);
											}
											else {
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["folder"].asBool(), listfiles[j].m_bfolder);
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["creationTime"].asInt64(), listfiles[j].m_created);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["duid"].asString().c_str(), listfiles[j].m_duid.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["id"].asString().c_str(), listfiles[j].m_fileid.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["lastModified"].asInt64(), listfiles[j].m_lastmodified);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["name"].asString().c_str(), listfiles[j].m_nxlname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["displayName"].asString().c_str(), listfiles[j].m_ownerdisplayname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["email"].asString().c_str(), listfiles[j].m_owneremail.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["owner"]["userId"].asInt64(), listfiles[j].m_ownerid);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathDisplay"].asString().c_str(), listfiles[j].m_pathdisplay.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathId"].asString().c_str(), listfiles[j].m_pathid.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["size"].asInt64(), listfiles[j].m_size);
											}
										}
									}
									else {
										cout << "No files in this project" << endl;
									}
								}
							}
						}
					}
				}
			}
			res = puser->LogoutUser();
			cout << "-------------This case finished--------" << endl;
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);

		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
		if (login_result) {
			for (RM_ProjectFilter f = RF_All; f <= RF_OwnedByOther; f = (RM_ProjectFilter)(f + 1)) {
				for (int i = 0; i < order.size(); i++) {
					orderBy = order[i];
					if (filter == RF_OwnedByMe)
					{
						owedbyme = "true";
					}
					else if (filter == RF_OwnedByOther) {
						owedbyme = "false";
					}
					else {
						owedbyme = "all";
					}
					//Get project list: "-name" "all"
					bool list_result = GetProjectListResponseData(page, size, orderBy, owedbyme);
					if (list_result) {
						Json::Value jroot;
						jroot = ParseJsonFile(projectlist_return);
						projectcount = jroot["results"]["detail"].size();
						if (!projectcount) {
							cout << "There is no projects from this user!" << endl;
							EXPECT_TRUE(projectcount);
							return;
						}
						res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
						std::vector<SDR_PROJECT_INFO> project_info_list = puser->GetProjectsInfo();
						EXPECT_EQ(project_info_list.size(), projectcount);
						for (int y = 0; y < projectcount; y++) {
							EXPECT_STREQ(project_info_list[y].displayname.c_str(), jroot["results"]["detail"][y]["displayName"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].description.c_str(), jroot["results"]["detail"][y]["description"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].name.c_str(), jroot["results"]["detail"][y]["name"].asString().c_str());
							EXPECT_EQ(project_info_list[y].bowner, jroot["results"]["detail"][y]["ownedByMe"].asBool());
							EXPECT_STREQ(project_info_list[y].tokengroupname.c_str(), jroot["results"]["detail"][y]["tokenGroupName"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].parenttenantid.c_str(), jroot["results"]["detail"][y]["parentTenantId"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].parenttenantname.c_str(), jroot["results"]["detail"][y]["parentTenantName"].asString().c_str());
							EXPECT_EQ(project_info_list[y].projid, jroot["results"]["detail"][y]["id"].asInt());
							uint64_t   projid = project_info_list[y].projid;
							//Project file info test
							vector<string> orderby_files = { "creationTime","-creationTime", "name", "-name" };

							for (int z = 0; z < 4; z++) {
								string filelist_orderby = orderby_files[z];
								bool filelist_result = GetProjectFilelistResponseData(to_string(projid), page, size, filelist_orderby, "/", "");
								if (filelist_result) {
									Json::Value jroot_filelist;
									jroot_filelist = ParseJsonFile(projectfilelist_return);
									files_count = jroot_filelist["results"]["detail"]["totalFiles"].asInt();
									std::vector<SDR_PROJECT_FILE_INFO> listfiles;
									res = puser->GetProjectListFiles(projid, pageId, pageSize, filelist_orderby, "/", "", listfiles);
									if (files_count) {
										if (files_count > listfiles.size()) {
											files_count = listfiles.size();
										}
										EXPECT_EQ(listfiles.size(), files_count);
										for (int j = 0; j < files_count; j++) {
											bool is_folder;
											Json::Value subjroot;
											subjroot = ParseJsonFile(projectfilelist_return);
											if (is_folder = subjroot["results"]["detail"]["files"][j]["folder"].asBool()) {
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["folder"].asBool(), listfiles[j].m_bfolder);
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["creationTime"].asInt64(), listfiles[j].m_created);
												EXPECT_STREQ("", listfiles[j].m_duid.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["id"].asString().c_str(), listfiles[j].m_fileid.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["lastModified"].asInt64(), listfiles[j].m_lastmodified);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["name"].asString().c_str(), listfiles[j].m_nxlname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["displayName"].asString().c_str(), listfiles[j].m_ownerdisplayname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["email"].asString().c_str(), listfiles[j].m_owneremail.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["owner"]["userId"].asInt64(), listfiles[j].m_ownerid);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathDisplay"].asString().c_str(), listfiles[j].m_pathdisplay.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathId"].asString().c_str(), listfiles[j].m_pathid.c_str());
												string path_id = listfiles[j].m_pathid;
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["size"].asInt64(), listfiles[j].m_size);
											}
											else {
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["folder"].asBool(), listfiles[j].m_bfolder);
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["creationTime"].asInt64(), listfiles[j].m_created);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["duid"].asString().c_str(), listfiles[j].m_duid.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["id"].asString().c_str(), listfiles[j].m_fileid.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["lastModified"].asInt64(), listfiles[j].m_lastmodified);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["name"].asString().c_str(), listfiles[j].m_nxlname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["displayName"].asString().c_str(), listfiles[j].m_ownerdisplayname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["email"].asString().c_str(), listfiles[j].m_owneremail.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["owner"]["userId"].asInt64(), listfiles[j].m_ownerid);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathDisplay"].asString().c_str(), listfiles[j].m_pathdisplay.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathId"].asString().c_str(), listfiles[j].m_pathid.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["size"].asInt64(), listfiles[j].m_size);
											}
										}
									}
									else {
										cout << "No files in this project" << endl;
									}
								}
							}
						}
					}
				}
			}

			res = puser->LogoutUser();
			cout << "-------------This case finished--------" << endl;
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

// Test Upload and download project files, and get project file metadata
TEST(spinProjectUploadFileTest, UploadFileTest) {
	try {
		wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
		string profile_pathid;
		int p_id;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		ISDRmUser *puser_b = NULL;
		uint32_t pageId = 1;
		uint32_t pageSize = 10;
		std::string orderBy = "name";
		RM_ProjectFilter filter = RF_All;
		SDR_Expiration expire;
		expire.type = IExpiryType::RANGEEXPIRE;
		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000 + 604800000;
		expire.start = (uint64_t)std::time(nullptr) * 1000 + 604800000;
		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };
		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_DOWNLOAD);

		std::vector<SDRmFileRight> userb_rights;
		userb_rights.push_back(RIGHT_VIEW);
		userb_rights.push_back(RIGHT_PRINT);
		userb_rights.push_back(RIGHT_DOWNLOAD);

		string tag_text = "{}";
		wstring nxlfile_path;
		wstring file_name;
		string mem_id;


		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email_feng, password);
		if (login_result) {
			res = puser->GetListProjects(1, 100, orderBy, filter);
			if (!res.GetCode()) {
				std::vector<SDR_PROJECT_INFO> project_info_list = puser->GetProjectsInfo();
				if (project_info_list.size()) {
					p_id = project_info_list[1].projid;
					mem_id = puser->GetMembershipID(p_id);
				}
				else {
					cout << "No projects from this user" << endl;
					EXPECT_TRUE(false);
					return;

				}
			}
			else {
				cout << "List project failed!!" << endl;
			}
			//Protect file with ad-hoc rights
			res = puser->ProtectFile(paln_file_ws + L"Small.txt", nxlfile_path, rights, watermark_info, expire, "", mem_id);
			bool list_result = GetProjectListResponseData("1", "100", "name", "all");
			if (!res.GetCode()) {
				ISDRFiles* pFile = NULL;
				bool delete_result;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				file_name = nxlfile->GetFileName();
				//Rights verified
				vector<SDRmFileRight> rights_v = nxlfile->GetRights();
				EXPECT_EQ(rights_v.size(), rights.size());
				bool rights_result = rights_verified(rights, nxlfile);
				EXPECT_TRUE(rights_result);

				bool has_watermark = nxlfile->CheckRights(RIGHT_WATERMARK);

				res = puser->UploadProjectFile(p_id, L"/", nxlfile);
				Sleep(3000);
				if (!res.GetCode()) {
					std::vector<SDR_PROJECT_FILE_INFO> listfiles;
					res = puser->GetProjectListFiles(p_id, 1, 20, "-creationTime", "/", ws2s(file_name), listfiles);
					if (!res.GetCode()) {
						profile_pathid = listfiles[0].m_pathid;
						//Download from root

						res = puser->ProjectDownloadFile(p_id, profile_pathid, projectdownloaded_path, RD_ForOffline);
						std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
						res = puser->GetRights(projectdownloaded_path, rightsAndWatermarks);
						EXPECT_EQ(11, rightsAndWatermarks.size());

						SDR_FILE_META_DATA metadata;
						res = puser->GetProjectFileMetadata(projectdownloaded_path, to_string(p_id), profile_pathid, metadata);
						std::vector<SDRmFileRight> filerights;
						res = puser->GetProjectFileRights(to_string(p_id), profile_pathid, filerights);
						EXPECT_EQ(rights.size(), filerights.size());

						bool metadata_result = GetProjectFileMetadataResponseData(to_string(p_id), profile_pathid);
						if (metadata_result) {
							Json::Value jroot;
							jroot = ParseJsonFile(projectfilemetadatalist_return);
							//File owner get file rights
							int rights_count = jroot["results"]["fileInfo"]["rights"].size();
							EXPECT_EQ(11, rightsAndWatermarks.size());
							string f_name = jroot["results"]["fileInfo"]["name"].asString();
							string f_pathdisplay = jroot["results"]["fileInfo"]["pathDisplay"].asString();
							string f_pathid = jroot["results"]["fileInfo"]["pathId"].asString();
							string f_filetype = jroot["results"]["fileInfo"]["fileType"].asString();
							uint64_t f_lastModified = jroot["results"]["fileInfo"]["lastModified"].asInt64();
							int f_size = jroot["results"]["fileInfo"]["size"].asInt();
							bool f_owner = jroot["results"]["fileInfo"]["owner"].asBool();
							bool f_nxl = jroot["results"]["fileInfo"]["nxl"].asBool();
							int f_protectiontype = jroot["results"]["fileInfo"]["protectionType"].asInt();
							uint64_t f_expireend = jroot["results"]["fileInfo"]["expiry"]["endDate"].asInt64();
							uint64_t f_expirestart = jroot["results"]["fileInfo"]["expiry"]["startDate"].asInt64();

							uint64_t m_start = metadata.expiration.start;
							uint64_t m_end = metadata.expiration.end;
							uint64_t m_lastmodified = metadata.lastmodify;
							string m_name = metadata.name;
							bool m_nxl = metadata.nxl;
							bool m_owner = metadata.owner;
							string m_pathdisplay = metadata.pathDisplay;
							string m_pathid = metadata.pathid;
							int m_protectionType = metadata.protectionType;

							EXPECT_EQ(m_start, expire.start);
							EXPECT_EQ(m_end, expire.end);
							EXPECT_STREQ(f_name.c_str(), m_name.c_str());
							EXPECT_EQ(m_lastmodified, f_lastModified);
							EXPECT_EQ(m_nxl, f_nxl);
							EXPECT_EQ(m_owner, f_owner);
							EXPECT_STREQ(m_pathdisplay.c_str(), f_pathdisplay.c_str());
							EXPECT_STREQ(m_pathid.c_str(), f_pathid.c_str());
							EXPECT_EQ(m_protectionType, f_protectiontype);
						}
						else {
							cout << "Get Project file metadata failed!!" << endl;
							EXPECT_TRUE(false);
							return;
						}
					}
					else {
						cout << "Upload file to project failed!!" << endl;
						EXPECT_TRUE(false);
						return;
					}
				}
				else {
					cout << "Upload file to project failed!!" << endl;
					EXPECT_TRUE(false);
					return;
				}
				res = puser->LogoutUser();
				cout << "-------------This case finished--------" << endl;
			}
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);

		login_result = Default_Login(pInstance, pTenant, puser_b, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
		if (login_result) {
			res = puser_b->GetListProjects(1, 100, orderBy, filter);
			if (!res.GetCode()) {
				std::vector<SDR_PROJECT_INFO> project_info_list = puser_b->GetProjectsInfo();
				if (project_info_list.size()) {
					mem_id = puser_b->GetMembershipID(p_id);
				}
				else {
					cout << "No projects from this user" << endl;
					EXPECT_TRUE(false);
					return;
				}
			}
			else {
				cout << "List project failed!!" << endl;
			}
			//Protect file with ad-hoc rights
			res = puser_b->ProtectFile(paln_file_ws + L"Small.txt", nxlfile_path, userb_rights, watermark_info, expire, "", mem_id);
			bool list_result = GetProjectListResponseData("1", "100", "name", "all");
			if (!res.GetCode()) {
				ISDRFiles* pFile = NULL;
				bool delete_result;

				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				file_name = nxlfile->GetFileName();
				//Rights verified
				vector<SDRmFileRight> rights_v = nxlfile->GetRights();
				EXPECT_EQ(rights_v.size(), userb_rights.size());
				bool rights_result = rights_verified(userb_rights, nxlfile);
				EXPECT_TRUE(rights_result);

				bool has_watermark = nxlfile->CheckRights(RIGHT_WATERMARK);

				res = puser_b->UploadProjectFile(p_id, L"/", nxlfile);
				Sleep(3000);
				//Remove file
				delete_result = pFile->RemoveFile(nxlfile);
				EXPECT_TRUE(delete_result);
				if (!res.GetCode()) {
					std::vector<SDR_PROJECT_FILE_INFO> listfiles;
					res = puser_b->GetProjectListFiles(p_id, 1, 20, "-creationTime", "/", ws2s(file_name), listfiles);
					if (!res.GetCode()) {
						string userfile_pathid = listfiles[0].m_pathid;
						//Download from root
						wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
						res = puser_b->ProjectDownloadFile(p_id, userfile_pathid, downloaded_path, RD_ForOffline);
						wstring download_nxlfilepath = downloaded_path;
						std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
						puser_b->GetRights(downloaded_path, rightsAndWatermarks);
						EXPECT_EQ(11, rightsAndWatermarks.size());

					}
					else {
						cout << "Upload file to project failed!!" << endl;
						EXPECT_TRUE(false);
						return;
					}
				}
				else {
					cout << "Upload file to project failed!!" << endl;
					checkSDWLResult(res);
					EXPECT_TRUE(false);
					return;
				}
				res = puser_b->LogoutUser();
				cout << "-------------This case finished--------" << endl;
			}
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinMyVaultGetMyvaultFiles, GetMyvaultFiles) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		ISDRmUser *puser_b = NULL;
		uint32_t pageId = 1;
		uint32_t pageSize = 1000;
		int file_testcount = 5;
		//orderBy Should be fileName, creationTime, size
		string orderBy;

		std::vector<SDR_MYVAULT_FILE_INFO> listfiles2;
		std::vector<SDR_MYVAULT_FILE_INFO> listfiles3;
		std::vector<SDR_MYVAULT_FILE_INFO> listfiles4;
		std::vector<SDR_MYVAULT_FILE_INFO> listfiles5;

		string filter_s;
		vector<string> order_by = { "fileName","-fileName","creationTime","-creationTime", "size", "-size" };
		vector<string> filter = { "allShared","revoked", "activeTransaction","allFiles", "protected", "deleted" };
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			for (int i = 0; i < order_by.size(); i++) {
				for (int j = 0; j < filter.size(); j++) {
					int file_count;
					std::vector<SDR_MYVAULT_FILE_INFO> listfiles;
					orderBy = order_by[i];
					filter_s = filter[j];
					bool list_result = GetMyVaultFileListResponseData(to_string(pageId), to_string(pageSize), "", orderBy, filter_s);
					res = puser->GetMyVaultFiles(pageId, pageSize, orderBy, filter_s, listfiles);
					if (list_result) {
						Json::Value jroot;
						jroot = ParseJsonFile(myvaultfilelist_return);
						file_count = jroot["results"]["detail"]["totalFiles"].asInt();
						if (file_count == listfiles.size()) {
							if (file_count <= 5) {
								file_testcount = file_count;
							}
							for (int z = 0; z < file_testcount; z++) {
								bool is_delete = jroot["results"]["detail"]["files"][z]["deleted"].asBool();
								bool is_revoke = jroot["results"]["detail"]["files"][z]["revoked"].asBool();
								bool is_share = jroot["results"]["detail"]["files"][z]["shared"].asBool();
								uint64_t create_time = jroot["results"]["detail"]["files"][z]["sharedOn"].asInt64();
								string duid = jroot["results"]["detail"]["files"][z]["duid"].asString();
								uint64_t last_modified;
								string nxl_name = jroot["results"]["detail"]["files"][z]["name"].asString();
								string path_dis = jroot["results"]["detail"]["files"][z]["pathDisplay"].asString();
								string path_id = jroot["results"]["detail"]["files"][z]["pathId"].asString();
								string pro_id = jroot["results"]["detail"]["files"][z]["repoId"].asString();
								uint64_t share_time = jroot["results"]["detail"]["files"][z]["sharedOn"].asInt64();
								string recipients = listfiles[z].m_sharedwith;
								//string share_with = jroot["results"]["detail"]["files"][z]["sharedWith"].asString();
								uint64_t size = jroot["results"]["detail"]["files"][z]["size"].asInt64();
								string source_pathdis = jroot["results"]["detail"]["files"][z]["customMetadata"]["SourceFilePathDisplay"].asString();
								string source_pathid;
								string source_repoid;
								string source_reponame = jroot["results"]["detail"]["files"][z]["customMetadata"]["SourceRepoName"].asString();
								string sorce_retype = jroot["results"]["detail"]["files"][z]["customMetadata"]["SourceRepoType"].asString();
								int rights_size = jroot["results"]["detail"]["files"][z]["rights"].size();
								if (jroot["results"]["detail"]["files"][z]["rights"][rights_size - 1].asString() == "WATERMARK")
									rights_size = rights_size - 1;
								EXPECT_EQ(is_delete, listfiles[z].m_bdeleted);
								EXPECT_EQ(is_revoke, listfiles[z].m_brevoked);
								EXPECT_EQ(is_share, listfiles[z].m_bshared);
								//EXPECT_EQ(create_time, listfiles[z].m_creationtime);
								EXPECT_STREQ(duid.c_str(), listfiles[z].m_duid.c_str());
								EXPECT_STREQ(nxl_name.c_str(), listfiles[z].m_nxlname.c_str());
								EXPECT_STREQ(path_dis.c_str(), listfiles[z].m_pathdisplay.c_str());
								EXPECT_STREQ(path_id.c_str(), listfiles[z].m_pathid.c_str());
								EXPECT_STREQ(pro_id.c_str(), listfiles[z].m_repoid.c_str());
								EXPECT_EQ(share_time, listfiles[z].m_sharedon);
								//EXPECT_STREQ(share_with.c_str(), listfiles[z].m_sharedwith.c_str());
								EXPECT_EQ(size, listfiles[z].m_size);
								EXPECT_STREQ(source_pathdis.c_str(), listfiles[z].m_sourcefilepathdisplay.c_str());
								EXPECT_STREQ(source_reponame.c_str(), listfiles[z].m_sourcereponame.c_str());
								EXPECT_STREQ(sorce_retype.c_str(), listfiles[z].m_sourcerepotype.c_str());

								vector<SDRmFileRight> get_rights;
								res = puser->GetMyVaultFileRights(listfiles[z].m_duid.c_str(), listfiles[z].m_pathid.c_str(), get_rights);
								EXPECT_FALSE(res.GetCode());
								EXPECT_EQ(get_rights.size(), rights_size);
							}
						}
						else {
							cout << "File count not equals!" << endl;
							EXPECT_TRUE(false);
							return;
						}
					}
					else {
						cout << "Can not get myvault file list!" << endl;
						EXPECT_TRUE(false);
						return;
					}
				}

			}

			cout << "Test Finished!" << endl;
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinShareWithMeTest, ShareWithMe) {
	try {
		SDWLResult res;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;

		ISDRmUser *puser = NULL;
		ISDRmUser *puser_b = NULL;
		uint32_t pageId = 1;
		uint32_t pageSize = 100;
		//orderBy Should be fileName, creationTime, size
		string orderBy;

		vector<string> order = { "name","-name","size","-size","sharedBy", "-sharedBy", "sharedDate", "-sharedDate" };
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			for (int i = 0; i < order.size(); i++) {
				orderBy = order[i];
				bool shrewithme_result = GetShareWithMeListResponseData(to_string(pageId), to_string(pageSize), orderBy);
				Json::Value jroot;
				jroot = ParseJsonFile(shrewithmefilelist_return);
				int file_count = jroot["results"]["detail"]["totalFiles"].asInt();
				vector<SDR_SHAREDWITHME_FILE_INFO> listfiles;
				res = puser->GetSharedWithMeFiles(pageId, pageSize, orderBy, "", listfiles);
				if (file_count) {
					if (file_count == listfiles.size()) {
						for (int j = 0; j < file_count; j++) {
							string  duid = jroot["results"]["detail"]["files"][j]["duid"].asString();
							string  nxlname = jroot["results"]["detail"]["files"][j]["name"].asString();
							string  filetype = jroot["results"]["detail"]["files"][j]["fileType"].asString();
							uint64_t	 size = jroot["results"]["detail"]["files"][j]["size"].asInt64();
							uint64_t     shareddate = jroot["results"]["detail"]["files"][j]["sharedDate"].asInt64();
							string  sharedby = jroot["results"]["detail"]["files"][j]["sharedBy"].asString();
							string  transactionid = jroot["results"]["detail"]["files"][j]["transactionId"].asString();
							string  transactioncode = jroot["results"]["detail"]["files"][j]["transactionCode"].asString();
							string  sharedlink = jroot["results"]["detail"]["files"][j]["sharedLink"].asString();
							int  rights_size = jroot["results"]["detail"]["files"][j]["rights"].size();
							string  comments;
							int count = jroot["results"]["detail"]["files"][j].size();
							if (count == 13) {
								comments = jroot["results"]["detail"]["files"][j]["sharedLink"].asString();
							}
							bool         isowner = jroot["results"]["detail"]["files"][j]["isOwner"].asBool();
							EXPECT_STREQ(duid.c_str(), listfiles[j].m_duid.c_str());
							EXPECT_STREQ(nxlname.c_str(), listfiles[j].m_nxlname.c_str());
							EXPECT_STREQ(filetype.c_str(), listfiles[j].m_filetype.c_str());
							EXPECT_EQ(size, listfiles[j].m_size);
							EXPECT_EQ(shareddate, listfiles[j].m_shareddate);
							EXPECT_STREQ(sharedby.c_str(), listfiles[j].m_sharedby.c_str());
							EXPECT_STREQ(transactionid.c_str(), listfiles[j].m_transactionid.c_str());
							EXPECT_STREQ(transactioncode.c_str(), listfiles[j].m_transactioncode.c_str());
							EXPECT_STREQ(sharedlink.c_str(), listfiles[j].m_sharedlink.c_str());
							string ri = listfiles[j].m_rights;
						}
					}
					else {
						cout << "The file count is diff!  " << endl;
						EXPECT_TRUE(false);
						return;
					}
				}
				else {
					cout << "There is no file in this user share with me file list!" << endl;
					EXPECT_TRUE(false);
					return;
				}
			}
			puser->LogoutUser();
			pInstance->Save();
			SDWLibDeleteRmcInstance(pInstance);
			cout << "Test Finished!" << endl;
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinShareWithMeDownloadFileTest, ShareWithMeDownloadFileTest) {
	try {
		SDWLResult res;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		ISDRmUser *puser = NULL;
		string transactionid;
		string transactioncode;
		string duid;
		wstring emaillist = s2ws(email_feng);
		wstring localfile_pathw = paln_file_ws + txtnormal_testfile;
		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_SHARE);
		rights.push_back(RIGHT_DOWNLOAD);
		std::vector<std::string> recipients = { email };
		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		std::wstring filename;
		wstring download_path = L"C:\\auto\\output\\SDK\\Project\\";
		createFolder(ws2s(download_path));

		wstring nxlfile_path;
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, "{}");
			if (res.GetCode() == 0) {
				ISDRFiles* pFile = NULL;
				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					filename = nxlfile->GetFileName();
					res = puser->UploadFile(nxlfile_path, nxlfile_path, emaillist, L"Share file to you!");
					if (!res.GetCode()) {
						cout << "Upload file result!" << endl;
						EXPECT_FALSE(res.GetCode());
					}
					else {
						cout << "Upload file failed!" << endl;
						EXPECT_FALSE(res.GetCode());
					}
				}
				else {
					cout << "No file generated!" << endl;
					EXPECT_FALSE(res.GetCode());
				}
			}
			res = puser->LogoutUser();
		}
		else {
			cout << "Login failed!" << endl;
			EXPECT_TRUE(login_result);
		}
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);

		uint32_t pageId = 1;
		uint32_t pageSize = 1000;
		//orderBy Should be fileName, creationTime, size
		string orderBy;

		std::vector<SDR_SHAREDWITHME_FILE_INFO> listfiles;

		login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
		if (login_result) {
			res = puser->GetSharedWithMeFiles(1, 10, "sharedDate", ws2s(filename), listfiles);
			string transactioncode = listfiles[0].m_transactioncode;
			string transactionid = listfiles[0].m_transactionid;

			res = puser->SharedWithMeDownloadFile(s2ws(transactioncode), s2ws(transactionid), download_path, false);
			bool exist_result = file_exist(ws2s(download_path));
			EXPECT_TRUE(exist_result);
			cout << "----------Test Finish----------" << endl;
		}
		else {
			cout << "Login failed!" << endl;
			EXPECT_TRUE(login_result);
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinMyVaultFileMetadataTest, FileMetadata) {
	try {
		SDR_Expiration expire;
		expire.type = IExpiryType::RANGEEXPIRE;
		expire.start = (uint64_t)std::time(nullptr) * 1000;
		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;

		SDWLResult res;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		wstring localfile_pathw = plan_file_path + txtnormal_testfile;

		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);

		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };

		ISDRmUser *puser = NULL;
		ISDRmUser *puser_b = NULL;
		wstring nxlfilepath;
		string pathid;

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			std::wstring filename;
			wstring recipients = L"123123@qq.com";
			wstring nxlfile_path;
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire);
			//res = puser->ProtectFile(paln_file_ws + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire);
			if (res.GetCode() == 0) {
				ISDRFiles* pFile = NULL;
				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					filename = nxlfile->GetFileName();
					//Upload file
					res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comments Test!");
					EXPECT_FALSE(res.GetCode());

					bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
					if (file_result) {
						Json::Value jroot;
						jroot = ParseJsonFile(myvaultfilelist_return);
						pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
					}
					wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
					res = puser->MyVaultDownloadFile(pathid, downloaded_path, 2);
					string  name;
					string  fileLink;
					uint64_t     lastmodify;
					vector<SDRmFileRight>  rights;
					bool         shared;
					bool         deleted;
					bool         revoked;
					uint32_t     protectionType;
					uint64_t     protectedon;
					uint64_t     sharedon;
					std::vector<std::string> recipients;
					int recipients_count;
					SDR_Expiration expiration;
					if (!res.GetCode()) {
						SDR_FILE_META_DATA metadata;
						nxlfilepath = downloaded_path;
						res = puser->GetNXLFileMetadata(nxlfilepath, pathid, metadata);

						protectedon = metadata.protectedon;
						sharedon = metadata.sharedon;
						deleted = metadata.deleted;
						expiration = metadata.expiration;
						fileLink = metadata.fileLink;
						lastmodify = metadata.lastmodify;
						name = metadata.name;
						protectionType = metadata.protectionType;
						recipients = metadata.recipients;
						revoked = metadata.revoked;
						//rights = metadata.rights;
						shared = metadata.shared;
						recipients_count = recipients.size();

						bool metadata_result = GetMyVaultFileMetadataResponseData(ws2s(filename));
						string  name_json;
						string  fileLink_json;
						uint64_t     lastmodify_json;
						vector<string>  rights_json;
						bool         shared_json;
						bool         deleted_json;
						bool         revoked_json;
						uint32_t     protectionType_json;
						int recipients_json;
						uint64_t     start_json;
						uint64_t     end_json;
						uint64_t protecton_json;
						uint64_t shareon_json;
						if (metadata_result) {
							Json::Value jroot;
							jroot = ParseJsonFile(filemetadatalist_return);
							name_json = jroot["results"]["detail"]["name"].asString();
							fileLink_json = jroot["results"]["detail"]["fileLink"].asString();
							lastmodify_json = jroot["results"]["detail"]["protectedOn"].asInt64();
							protectionType_json = jroot["results"]["detail"]["protectionType"].asInt();
							start_json = jroot["results"]["detail"]["validity"]["startDate"].asInt64();
							end_json = jroot["results"]["detail"]["validity"]["endDate"].asInt64();
							shared_json = jroot["results"]["detail"]["shared"].asBool();
							deleted_json = jroot["results"]["detail"]["deleted"].asBool();
							revoked_json = jroot["results"]["detail"]["revoked"].asBool();
							recipients_json = jroot["results"]["detail"]["recipients"].size();
							protecton_json = jroot["results"]["detail"]["protectedOn"].asInt64();
							shareon_json = jroot["results"]["detail"]["sharedOn"].asInt64();
							EXPECT_STREQ(name_json.c_str(), name.c_str());
							EXPECT_STREQ(fileLink_json.c_str(), fileLink.c_str());
							EXPECT_EQ(lastmodify_json, lastmodify);
							EXPECT_EQ(protectionType_json, protectionType);
							EXPECT_EQ(revoked, revoked_json);
							EXPECT_EQ(deleted, deleted_json);
							EXPECT_EQ(shared, shared_json);
							EXPECT_EQ(recipients_count, recipients_json);
							EXPECT_EQ(start_json, expiration.start);
							EXPECT_EQ(end_json, expiration.end);
							EXPECT_EQ(protecton_json, protectedon);
							EXPECT_EQ(shareon_json, sharedon);
						}
					}
					else {
						cout << "Downloaded file failed!" << endl;
						EXPECT_TRUE(false);
					}
				}
				else {
					cout << "Upload file failed!" << endl;
					EXPECT_FALSE(res.GetCode());
				}
			}
			else {
				cout << "Share file failed!" << endl;
				EXPECT_TRUE(false);
			}
			res = pInstance->Save();
			puser->LogoutUser();
			SDWLibDeleteRmcInstance(pInstance);
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinMyVaultDownloadPartialFileTest, MyVaultDownloadPartialFile) {
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmUser *puser_zhu = NULL;
	ISDRmTenant *pTenant = NULL;
	bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	SDR_Expiration expire;
	expire.type = IExpiryType::NEVEREXPIRE;
	wstring localfile_pathw = plan_file_path + txtnormal_testfile;
	wstring nxlfile_path = L"";
	wstring transactioncode;
	wstring transactionid;
	string duid;
	string pathid;
	std::wstring filename;
	std::vector<SDRmFileRight> rights;
	rights.push_back(RIGHT_VIEW);
	rights.push_back(RIGHT_DOWNLOAD);

	string tag_text = "{}";
	wstring comments_w = L"test";
	wstring recipients = s2ws(email_zhu);
	SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };

	wstring targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
	wstring targert_folder2 = L"C:\\auto\\output\\SDK\\Project\\";
	//User A protect file
	res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
	if (res.GetCode() == 0) {
		ISDRFiles* pFile = NULL;

		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();

			//Upload file
			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comments Test");
			EXPECT_EQ(res.GetCode(), 0);


			bool myvault_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "creationTime", "allFiles");
			Json::Value jroot;
			jroot = ParseJsonFile(myvaultfilelist_return);
			pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
			duid = jroot["results"]["detail"]["files"][0]["duid"].asString();

			//Download protected file
			res = puser->MyVaultDownloadPartialFile(pathid, targert_folder, 2);
			EXPECT_EQ(res.GetCode(), 0);
			bool isfile_exist = file_exist(ws2s(targert_folder));
			EXPECT_TRUE(isfile_exist);

			ISDRmNXLFile* nxlfile;
			res = puser->OpenFile(targert_folder, &nxlfile);
			vector<SDRmFileRight> rights_nxl;
			rights_nxl = nxlfile->GetRights();
			EXPECT_EQ(rights_nxl.size(), rights.size());

			//When file is invoked
			targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
			bool revoke_result = InvokeSharedFileResponseData(duid);
			res = puser->MyVaultDownloadPartialFile(pathid, targert_folder, 2);
			EXPECT_EQ(res.GetCode(), 0);

			//When target folder is invalid
			wstring target_invalidfolder = L"C:\\invalidfolder\\";
			res = puser->MyVaultDownloadPartialFile(pathid, target_invalidfolder, 2);
			EXPECT_EQ(res.GetCode(), 3);

			//When path id  is invalid
			targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
			res = puser->MyVaultDownloadPartialFile("\123", targert_folder, 2);
			EXPECT_EQ(res.GetCode(), 400 + 61440);

			//when file is deleted
			targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
			bool delete_result = DeleteMyvaultFile(duid, pathid);
			res = puser->MyVaultDownloadPartialFile(pathid, targert_folder, 2);
			EXPECT_EQ(res.GetCode(), 500 + 61440);
			cout << "----------Test Finish----------" << endl;
		}
		else {
			cout << "Upload file failed!" << endl;
			EXPECT_FALSE(res.GetCode());
		}
	}
	else {
		cout << "Protect file failed!" << endl;
		ASSERT_TRUE(false);
	}
}

string spinfileName;
TEST(spinMyVaultAndWithMeShareFileTest, ShareFileTest1) {
	try {
		vector<std::string> recipients = { "3333@qwe.com", "2222@qq.com" };
		vector<std::string> recipient_shared = { email_feng };
		vector<std::string> recipient_end = { email_feng, "3333@qwe.com", "2222@qq.com" };
		string repositoryId;
		string filePathId;
		string filePath;
		string pathid;
		string comment = "Share file with you!";
		wstring downloaded_p = L"C:\\auto\\output\\SDK\\Project\\";
		SDWLResult res;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		ISDRmUser *puser = NULL;
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {

#pragma region[Prepare a MYVAULT nxl file]
			SDR_Expiration expire;
			expire.type = IExpiryType::NEVEREXPIRE;
			wstring localfile_pathw = plan_file_path + txtnormal_testfileb;

			std::vector<SDRmFileRight> rights;
			rights.push_back(RIGHT_VIEW);
			rights.push_back(RIGHT_SHARE);
			rights.push_back(RIGHT_EDIT);

			SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
			std::wstring filename;
			wstring nxlfile_path;
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			ISDRFiles* pFile = NULL;
			std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
			ISDRmNXLFile* nxlfile;
			res = puser->OpenFile(nxlfile_path, &nxlfile);
			if (nxlfile)
			{
				filename = nxlfile->GetFileName();
				res = puser->UploadFile(nxlfile_path, nxlfile_path);
			}
			else {
				cout << "No file generated!" << endl;
				EXPECT_FALSE(res.GetCode());
			}


			bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
			if (file_result) {
				Json::Value jroot;
				jroot = ParseJsonFile(myvaultfilelist_return);
				pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
			}

			res = puser->MyVaultDownloadFile(pathid, downloaded_p, 2);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
#pragma endregion	

			std::vector<SDR_MYVAULT_FILE_INFO> listfiles;
			res = puser->GetMyVaultFiles(1, 100, "-creationTime", "all", listfiles);
			repositoryId = listfiles[0].m_repoid;
			filePathId = listfiles[0].m_pathid;
			spinfileName = listfiles[0].m_nxlname;
			filePath = listfiles[0].m_pathdisplay;
			//Share protected file
			res = puser->ShareFileFromMyVault(downloaded_p, recipients, repositoryId, spinfileName, filePathId, filePath, comment);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			bool metadate_result = GetMyVaultFileMetadataResponseData(ws2s(filename));
			Json::Value jroot;
			jroot = ParseJsonFile(filemetadatalist_return);
			int recipients_count = jroot["results"]["detail"]["recipients"].size();
			EXPECT_EQ(recipients_count, recipients.size());
			for (int i = 0; i < recipients.size(); i++) {
				string email_name = jroot["results"]["detail"]["recipients"][i].asString();
				EXPECT_STREQ(recipients[i].c_str(), email_name.c_str());
			}

			//Share shared file
			Sleep(1000);
			res = puser->ShareFileFromMyVault(downloaded_p, recipient_shared, repositoryId, spinfileName, filePathId, filePath, comment);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			metadate_result = GetMyVaultFileMetadataResponseData(ws2s(filename));
			jroot = ParseJsonFile(filemetadatalist_return);
			recipients_count = jroot["results"]["detail"]["recipients"].size();
			EXPECT_EQ(recipients_count, recipient_end.size());
			for (int i = 0; i < recipient_end.size(); i++) {
				string email_name = jroot["results"]["detail"]["recipients"][i].asString();
				EXPECT_STREQ(recipient_end[i].c_str(), email_name.c_str());
			}
		}

		cout << "----------Test Finish----------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinMyVaultAndWithMeShareFileTest, ShareFileTest2) {
	try {
		SDWLResult res;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		ISDRmUser *puser = NULL;
		string transactionid;
		string transactioncode;
		string duid;
		string emaillist = "4444@qq.com";
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
		if (login_result)
		{
			vector<SDR_SHAREDWITHME_FILE_INFO> listfiles;
			res = puser->GetSharedWithMeFiles(1, 10, "name", spinfileName, listfiles);
			int count = listfiles.size();
			for (int i = 0; i < count; i++) {
				if (listfiles[i].m_nxlname == spinfileName)
				{
					transactioncode = listfiles[i].m_transactioncode;
					transactionid = listfiles[i].m_transactionid;
					duid = listfiles[i].m_duid;
					break;
				}
			}
			//when user with share rights
			res = puser->SharedWithMeReShareFile(transactionid, transactioncode, emaillist);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
		}
		login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			bool metadate_result = GetMyVaultFileMetadataResponseData(spinfileName);
			Json::Value jroot;
			jroot = ParseJsonFile(filemetadatalist_return);
			int cout = jroot["results"]["detail"]["recipients"].size();
			EXPECT_EQ(cout, 4);

			bool revoke_result = InvokeSharedFileResponseData(duid);
		}
		login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
		if (login_result) {
			//when user without share rights
			res = puser->SharedWithMeReShareFile(transactionid, transactioncode, emaillist);
			EXPECT_EQ(res.GetCode(), 65441);
			EXPECT_STREQ(res.GetMsg().c_str(), "File has been revoked");
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinRPMEditSaveFileTest, EditSaveFileTest) {
	try {
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir1";
		wstring safe_folder_new = L"C:\\SDK test\\RPM\\Dir2";
		createFolder(ws2s(safe_folder));
		createFolder(ws2s(safe_folder_new));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		string testFolder = TestDirectory + "RPMtest";
		createFolder(testFolder);
		ISDRmUser *puser = NULL;
		string pathid;
		wstring downloaded_p = L"C:\\auto\\output\\SDK\\Project\\";
		//string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		int rights_value;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		cout << ws2s(apppath) << endl;
		res = pInstance->RPMRegisterApp(apppath, security);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RPMNotifyRMXStatus(running, security);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
#pragma region[Prepare RPM environment]
		if (login_result) {
			cout << "GetHeartBeatInfo: ";
			SDWLResult res = puser->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
		}
		cout << "----------RPM------------" << endl;

		ret = pInstance->IsRPMDriverExist();
		cout << "IsRPMDriverExist: " << ret << endl;
		if (!ret)
		{
			cout << "Please double check if the RPM build is installed!" << endl;
		}
		string passcode_global = ReadFromFile(plan_path + passcode_txt);
		string user_logindata; res = pInstance->GetLoginData(email, passcode_global, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, security);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->RemoveRPMDir(safe_folder);
		res = pInstance->RemoveRPMDir(safe_folder_new);
		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_folder);
		res = pInstance->AddRPMDir(safe_folder_new);

		res = pInstance->SetRPMServiceStop(true, security);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
#pragma endregion

#pragma region[Prepare a MYVAULT nxl file]
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;
		string original_content = read_filecontent(ws2s(localfile_pathw));

		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_EDIT);

		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
		std::wstring filename;
		wstring recipients = s2ws(email_zhu);
		wstring nxlfile_path;
		res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire);

		ISDRFiles* pFile = NULL;
		bool delete_result;
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();
			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comment Test!");
			rights_value = rights_changeToValue2(nxlfile->GetRights());
		}
		else {
			cout << "No file generated!" << endl;
			EXPECT_FALSE(res.GetCode());
		}


		bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
		if (file_result) {
			Json::Value jroot;
			jroot = ParseJsonFile(myvaultfilelist_return);
			pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
		}

		res = puser->MyVaultDownloadFile(pathid, downloaded_p, 2);

#pragma endregion	
		//Copy file to RPM folder then edit
		res = pInstance->RPMEditCopyFile(downloaded_p, safe_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		std::vector<SDRmFileRight> filerights;
		res = pInstance->RPMGetRights(safe_folder, rightsAndWatermarks);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		// file owner return 11
		EXPECT_EQ(11, rightsAndWatermarks.size());

		res = pInstance->RPMGetFileRights(safe_folder, filerights);
		int rights_v2 = rights_changeToValue2(filerights);
		EXPECT_EQ(rights_v2, rights_value);

		//std::wstring tempFile = safe_folder + L".nxl";
		//LPCTSTR lpFileName = tempFile.c_str();
		//WIN32_FIND_DATA pNextInfo;
		//HANDLE h = FindFirstFile(lpFileName, &pNextInfo);
		//if (h != INVALID_HANDLE_VALUE)
		//{
		//	FindClose(h);
		//}

		string conten_str = read_filecontent(ws2s(safe_folder));
		EXPECT_STREQ(conten_str.c_str(), original_content.c_str());
		string new_content = "Edit !";
		edit_file(ws2s(safe_folder), new_content);
		res = pInstance->RPMEditSaveFile(safe_folder, L"", false, 3);
		EXPECT_FALSE(res.GetCode());
		string conten_saved = read_filecontent(ws2s(safe_folder));
		EXPECT_STREQ(conten_saved.c_str(), new_content.c_str());

		//Download file to RPM folder then edit
		res = puser->MyVaultDownloadFile(pathid, safe_folder_new, 2);
		string safe_folder_new_s = ws2s(safe_folder_new);
		int str_len = safe_folder_new_s.length() - 4;

		std::wstring tempFile1 = safe_folder_new;
		LPCTSTR lpFileName1 = tempFile1.c_str();
		WIN32_FIND_DATA pNextInfo1;
		HANDLE h1 = FindFirstFile(lpFileName1, &pNextInfo1);
		if (h1 != INVALID_HANDLE_VALUE)
		{
			FindClose(h1);
		}

		const string safe_folder_new_ss = safe_folder_new_s.substr(0, str_len);
		string conten_str1 = read_filecontent(safe_folder_new_ss);
		EXPECT_STREQ(conten_str1.c_str(), original_content.c_str());
		string new_content1 = "Edit !";
		edit_file(safe_folder_new_ss, new_content);
		res = pInstance->RPMEditSaveFile(s2ws(safe_folder_new_ss), L"", false, 3);
		EXPECT_FALSE(res.GetCode());
		string conten_saved1 = read_filecontent(safe_folder_new_ss);
		EXPECT_STREQ(conten_saved1.c_str(), new_content1.c_str());
		safe_folder = L"C:\\SDK test\\RPM\\Dir1";
		res = pInstance->RemoveRPMDir(safe_folder, true);
		checkSDWLResult(res);
		EXPECT_EQ(res.GetCode(), 0);

		res = pInstance->RemoveRPMDir(safe_folder_new, true);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//RPMGetFileRights failed
vector<string> rights_spinvc = { "view|edit|print|copycontent|saveas|sc|send|classify|share|download", "view|edit", "view|share|print", "view|edit|print|download", "view|edit|print|copycontent|saveas|sc|send|classify|share|download", "view" };

wstring safe_normalfolder11 = L"C:\\SDK test\\RPM\\Dir2\\Starry";

class spinRPMUserBEditCopyFileTest :public::testing::TestWithParam<string> {};
TEST_P(spinRPMUserBEditCopyFileTest, NormalTest2)
{
	try {
		wstring safe_spinnormal2folder = L"C:\\SDK test\\RPM\\Dir1";
		wstring remove_safe_spinnormal2folder = safe_spinnormal2folder;
		createFolder(ws2s(safe_normalfolder11));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		string testFolder = TestDirectory + "RPMtest";
		createFolder(testFolder);
		ISDRmUser *puser = NULL;
		string pathid;
		//string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		res = pInstance->RPMRegisterApp(apppath, security);
		res = pInstance->RPMNotifyRMXStatus(running, security);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		string rights_str = GetParam();
		wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";

#pragma region[Prepare a MYVAULT nxl file]
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;

		std::vector<SDRmFileRight> rights;
		rights = Tick_Rights(rights_str);

		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
		std::wstring filename;
		wstring recipients = s2ws(email_zhu);
		wstring nxlfile_path;
		res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire);

		ISDRFiles* pFile = NULL;
		bool delete_result;
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();
			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comments!");

		}
		else {
			cout << "No file generated!" << endl;
			EXPECT_FALSE(res.GetCode());
		}

		res = puser->CloseFile(nxlfile);

		checkSDWLResult(res);
		bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
		if (file_result) {
			Json::Value jroot;
			jroot = ParseJsonFile(myvaultfilelist_return);
			pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
		}
		res = puser->MyVaultDownloadFile(pathid, downloaded_path, 2);

#pragma endregion	

		//#pragma region [User Logout]
		//		cout << endl << "SDK user logout:";
		//		res = puser->LogoutUser();
		//		checkSDWLResult(res);
		//#pragma endregion
		//
		//		string clientID = ReadFromFile(plan_path + "client_id.txt");
		//		string  puser_return = plan_path + "User_Return.txt";
		//		GetUserLoginResponseData(email_zhu, password, clientID, puser_return);
		//		string userJson = ReadFromFile(puser_return);
		//		res = pInstance->SetLoginResult(userJson, &puser);
#pragma region[Prepare RPM environment]
		if (login_result) {
			cout << "GetHeartBeatInfo: ";
			SDWLResult res = puser->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
		}
		cout << "----------RPM------------" << endl;

		ret = pInstance->IsRPMDriverExist();
		cout << "IsRPMDriverExist: " << ret << endl;
		if (!ret)
		{
			cout << "Please double check if the RPM build is installed!" << endl;
		}

		string passcode_global = ReadFromFile(plan_path + passcode_txt);
		string email_temp = ws2s(puser->GetEmail());
		string user_logindata; res = pInstance->GetLoginData(email_temp, passcode_global, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}");
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->RemoveRPMDir(safe_normalfolder11);
		res = pInstance->RemoveRPMDir(safe_spinnormal2folder);
		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_normalfolder11);
		res = pInstance->AddRPMDir(safe_spinnormal2folder);

#pragma endregion

		//Copy file to RPM folder
		res = pInstance->RPMEditCopyFile(downloaded_path, safe_spinnormal2folder);
		EXPECT_FALSE(res.GetCode());
		std::wstring tempFile = safe_spinnormal2folder + L".nxl";
		LPCTSTR lpFileName = tempFile.c_str();
		WIN32_FIND_DATA pNextInfo;
		HANDLE h = FindFirstFile(lpFileName, &pNextInfo);
		if (h != INVALID_HANDLE_VALUE)
		{
			FindClose(h);
		}
		Sleep(1000);
		EXPECT_TRUE(file_exist(ws2s(safe_spinnormal2folder)));

		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		//RPMGetRights
		res = pInstance->RPMGetRights(safe_spinnormal2folder, rightsAndWatermarks);
		EXPECT_EQ(rightsAndWatermarks.size(), 11);
		//RPMGetFileRights
		vector<SDRmFileRight> filerights;
		res = pInstance->RPMGetFileRights(safe_spinnormal2folder, filerights);
		EXPECT_EQ(filerights.size(), rights.size());

#pragma region [User Logout]
		cout << "RPMLogout: ";
		res = pInstance->RPMLogout();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << endl << "SDK user logout:";
		res = puser->LogoutUser();
		checkSDWLResult(res);
#pragma endregion
		safe_normalfolder11 = L"C:\\SDK test\\RPM\\Dir2\\Starry";
		res = pInstance->RemoveRPMDir(remove_safe_spinnormal2folder, true);
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl << endl;
	}

	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
INSTANTIATE_TEST_CASE_P(spinRPMUserBEditCopyFileTesting, spinRPMUserBEditCopyFileTest, testing::ValuesIn(rights_spinvc));

//RPMGetFileStatus failed
TEST(spinRPMGetFileStatusTest, RPMGetFileStatusTest) {
	try {
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir3";
		wstring remove_safe_folder = safe_folder;
		wstring safe_copyfolder = L"C:\\SDK test\\RPM\\Dir3";
		wstring safe_subfolder = L"C:\\SDK test\\RPM\\Dir3\\Sub1";
		wstring safe_parfolder = L"C:\\SDK test\\RPM";
		wstring unsafe_folder = L"C:\\SDK test\\NonRPM";
		createFolder(ws2s(safe_folder));
		createFolder(ws2s(safe_subfolder));
		createFolder(ws2s(unsafe_folder));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		string testFolder = TestDirectory + "RPMtest";
		createFolder(testFolder);
		ISDRmUser *puser = NULL;
		string pathid;
		//string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		wstring downloaded_p = L"C:\\auto\\output\\SDK\\Project\\";

		bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");
		res = pInstance->RPMRegisterApp(apppath, security);
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RPMNotifyRMXStatus(running, security);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

#pragma region[Prepare RPM environment]
		if (login_result) {
			cout << "GetHeartBeatInfo: ";
			SDWLResult res = puser->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
		}
		cout << "----------RPM------------" << endl;

		ret = pInstance->IsRPMDriverExist();
		cout << "IsRPMDriverExist: " << ret << endl;
		if (!ret)
		{
			cout << "Please double check if the RPM build is installed!" << endl;
		}

		string passcode_global = ReadFromFile(plan_path + passcode_txt);
		string email_temp = ws2s(puser->GetEmail());
		string user_logindata; res = pInstance->GetLoginData(email_temp, passcode_global, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}");
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RemoveRPMDir(safe_folder);
		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_folder);

#pragma endregion


#pragma region[Prepare a MYVAULT nxl file]
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;
		string original_content = read_filecontent(ws2s(localfile_pathw));

		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_EDIT);

		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
		std::wstring filename;
		wstring recipients = s2ws(email_zhu);
		wstring nxlfile_path;
		res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire);

		ISDRFiles* pFile = NULL;
		bool delete_result;
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();
			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comments Test");
		}
		else {
			cout << "No file generated!" << endl;
			EXPECT_FALSE(res.GetCode());
		}

		bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
		if (file_result) {
			Json::Value jroot;
			jroot = ParseJsonFile(myvaultfilelist_return);
			pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
		}

		res = puser->MyVaultDownloadFile(pathid, downloaded_p, 2);

#pragma endregion	

		//int processid;
		//cout << "Process id: ";
		//cin >> processid;
		//res = pInstance->RPMAddTrustedProcess(processid, secr);
		unsigned int dirstatus;
		bool filestatus;
		wstring des_path = safe_folder + L"\\" + filename;
		wstring des_subpath = safe_subfolder + L"\\" + filename;
		wstring des_prapath = safe_parfolder + L"\\" + filename;
		wstring des_nonpath = unsafe_folder + L"\\" + filename;

		//1. From SDK copy file
		res = pInstance->RPMEditCopyFile(downloaded_p, safe_copyfolder);

		std::wstring tempFile = safe_copyfolder+L".nxl";
		LPCTSTR lpFileName = tempFile.c_str();
		WIN32_FIND_DATA pNextInfo;
		HANDLE h = FindFirstFile(lpFileName, &pNextInfo);
		if (h != INVALID_HANDLE_VALUE)
		{
			FindClose(h);
		}

		EXPECT_FALSE(res.GetCode());
		res = pInstance->RPMGetFileStatus(safe_copyfolder, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 4);
		EXPECT_TRUE(filestatus);

		//2. Get file status from RPM folder
		CopyFile(downloaded_p.c_str(), des_path.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(des_path, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 1);
		EXPECT_TRUE(filestatus);

		//3. Get file status from RPM sub folder
		CopyFile(downloaded_p.c_str(), des_subpath.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(des_subpath, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 4);
		EXPECT_TRUE(filestatus);

		//4. Get file status from RPM parent folder
		CopyFile(downloaded_p.c_str(), des_prapath.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(des_prapath, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 2);
		EXPECT_TRUE(filestatus);

		//5. Get file status from NonRPM folder
		CopyFile(downloaded_p.c_str(), des_nonpath.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(des_nonpath, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 0);
		EXPECT_TRUE(filestatus);

		wstring nonnxl_sourcepath = plan_file_path + txtnormal_testfile;
		wstring invalidnxl_sourcepath = plan_file_path + invalidnxl_testfile;
		wstring notnxl_path = safe_folder + L"\\" + txtnormal_testfile;
		wstring notnxl_subpath = safe_subfolder + L"\\" + txtnormal_testfile;
		wstring notnxl_parpath = safe_parfolder + L"\\" + txtnormal_testfile;
		wstring invalidnxl_path = safe_folder + L"\\" + invalidnxl_testfile;
		//6. Not nxl file(without extension ".nxl")
		CopyFile(nonnxl_sourcepath.c_str(), notnxl_path.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(notnxl_path, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 1);
		EXPECT_FALSE(filestatus);

		//7. Not nxl file(without extension ".nxl"), sub safe folder
		CopyFile(nonnxl_sourcepath.c_str(), notnxl_subpath.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(notnxl_subpath, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 4);
		EXPECT_FALSE(filestatus);

		//8. Not nxl file(without extension ".nxl"), parent safe folder
		CopyFile(nonnxl_sourcepath.c_str(), notnxl_parpath.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(notnxl_parpath, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 2);
		EXPECT_FALSE(filestatus);

		//9. Invalid nxl file(with extension ".nxl")
		CopyFile(invalidnxl_sourcepath.c_str(), invalidnxl_path.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(invalidnxl_path, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 1);
		EXPECT_FALSE(filestatus);

		//Delete file
		res = pInstance->RPMDeleteFile(safe_copyfolder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RemoveRPMDir(remove_safe_folder, true);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//run again
TEST(tttEditOfficeFileTest, OfficeFileTest) {
	wstring safe_folder = L"C:\\SDK test\\RPM\\Dir3\\Sub1";
	wstring remove_safe_folder = safe_folder;
	wstring file_path = safe_folder + L"\\setting-2019-04-10-02-02-36.docx";
	createFolder(ws2s(safe_folder));

	ISDRmcInstance * pInstance;
	ISDRmTenant * pTenant;
	bool ret = false;
	SDWLResult res;
	string testFolder = TestDirectory + "RPMtest";
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
	string pathid;
	//string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	bool running = true;

	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	res = pInstance->RPMRegisterApp(apppath, security);
	res = pInstance->RPMNotifyRMXStatus(running, security);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

#pragma region[Prepare RPM environment]
	if (login_result) {
		cout << "GetHeartBeatInfo: ";
		SDWLResult res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
	}
	cout << "----------RPM------------" << endl;

	ret = pInstance->IsRPMDriverExist();
	cout << "IsRPMDriverExist: " << ret << endl;
	if (!ret)
	{
		cout << "Please double check if the RPM build is installed!" << endl;
	}

	string passcode_global = ReadFromFile(plan_path + passcode_txt);
	string email_temp = ws2s(puser->GetEmail());
	string user_logindata; res = pInstance->GetLoginData(email_temp, passcode_global, user_logindata);

	cout << "SetRPMLoginResult: ";
	res = pInstance->SetRPMLoginResult(user_logindata, "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}");
	EXPECT_EQ(0, res.GetCode());
	res = pInstance->RemoveRPMDir(safe_folder);
	cout << "AddRPMDir: ";
	res = pInstance->AddRPMDir(safe_folder);

#pragma endregion

#pragma region[Prepare a MYVAULT nxl file]
	SDR_Expiration expire;
	expire.type = IExpiryType::NEVEREXPIRE;
	wstring localfile_pathw = plan_file_path + docnormal_testfile;
	string original_content = read_filecontent(ws2s(localfile_pathw));

	std::vector<SDRmFileRight> rights;
	rights.push_back(RIGHT_VIEW);
	rights.push_back(RIGHT_EDIT);

	SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
	std::wstring filename;
	wstring recipients = s2ws(email_zhu);
	wstring nxlfile_path;
	res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire);

	ISDRFiles* pFile = NULL;
	bool delete_result;
	std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

	ISDRmNXLFile* nxlfile;
	res = puser->OpenFile(nxlfile_path, &nxlfile);
	if (nxlfile)
	{
		filename = nxlfile->GetFileName();
		res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comment Test!");
	}
	else {
		cout << "No file generated!" << endl;
		EXPECT_FALSE(res.GetCode());
	}


	bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
	if (file_result) {
		Json::Value jroot;
		jroot = ParseJsonFile(myvaultfilelist_return);
		pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
	}

	res = puser->MyVaultDownloadFile(pathid, safe_folder, 2);

#pragma endregion	
	for (int i = 0; i < 1; i++) {
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		LPCSTR pro_name = "WINWORD.EXE";
		DWORD dwPid = 0;
		Word::_ApplicationPtr spWordApp;
		HRESULT hr = spWordApp.CreateInstance(__uuidof(Word::Application));
		if (FAILED(hr))
		{
			wprintf(L"CreateInstance failed w/err 0x%08lx\n", hr);
			return;
		}

		_putws(L"Word.Application is started");

		spWordApp->Visible = VARIANT_TRUE;

		Word::DocumentsPtr spDocs = spWordApp->Documents;

		bool fid_processid = FindProcessPid(pro_name, dwPid);
		if (fid_processid) {
			cout << "WINWORD.EXE: " << endl;
			cout << dwPid << endl;
		}
		res = pInstance->RPMAddTrustedProcess(dwPid, security);

		safe_folder = safe_folder.substr(0, safe_folder.length() - 4);
		const wchar_t *destFileName = safe_folder.c_str();
		variant_t vtdestFileName(destFileName);
		Word::_DocumentPtr doc = spDocs->Open(&vtdestFileName);

		_putws(L"Insert a paragraph");

		Word::ParagraphsPtr spParas = doc->Paragraphs;
		Word::ParagraphPtr spPara = spParas->Add();
		Word::RangePtr spParaRng = spPara->Range;
		spParaRng->Text = _bstr_t(L"Heading 1");
		Word::_FontPtr spFont = spParaRng->Font;
		spFont->Bold = 1;
		spParaRng->InsertParagraphAfter();

		_putws(L"Save and close the document");

		doc->Save();
		doc->Close();

		_putws(L"Quit the Word application");
		spWordApp->Quit();
		CoUninitialize();

		std::wstring tempFile = safe_folder;
		LPCTSTR lpFileName = tempFile.c_str();
		WIN32_FIND_DATA pNextInfo;
		HANDLE h = FindFirstFile(lpFileName, &pNextInfo);
		if (h != INVALID_HANDLE_VALUE)
		{
			FindClose(h);
		}

		res = pInstance->RPMEditSaveFile(safe_folder, L"", false, 3);
		EXPECT_FALSE(res.GetCode());
		string original_content = read_filecontent(ws2s(localfile_pathw));
		res = pInstance->RemoveRPMDir(remove_safe_folder, true);
		cout << "-------------This case finished--------" << endl;
	}
};

TEST(spinRPMViewEditCentralizedProjectFileTest, ViewEditCentralizedProjectFileTest) {
	try {
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir1";
		wstring remove_safe_folder = safe_folder;
		wstring safe_folder_new = L"C:\\SDK test\\RPM\\Dir2";
		wstring remove_safe_folder_new = safe_folder_new;
		createFolder(ws2s(safe_folder));
		createFolder(ws2s(safe_folder_new));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		string testFolder = TestDirectory + "RPMtest";
		createFolder(testFolder);
		ISDRmUser *puser = NULL;
		string pathid;
		wstring downloaded_p = L"C:\\auto\\output\\SDK\\Project\\";
		//string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		int rights_eval = 0;
		wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
		string filename_lower;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
		res = pInstance->RPMRegisterApp(apppath, security);
		res = pInstance->RPMNotifyRMXStatus(running, security);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
#pragma region[Prepare RPM environment]
		if (login_result) {
			cout << "GetHeartBeatInfo: ";
			SDWLResult res = puser->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
		}
		cout << "----------RPM------------" << endl;

		ret = pInstance->IsRPMDriverExist();
		cout << "IsRPMDriverExist: " << ret << endl;
		if (!ret)
		{
			cout << "Please double check if the RPM build is installed!" << endl;
		}

		string passcode_global = ReadFromFile(plan_path + passcode_txt);
		string email_temp = ws2s(puser->GetEmail());
		string user_logindata; res = pInstance->GetLoginData(email_temp, passcode_global, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}");
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RemoveRPMDir(safe_folder);
		res = pInstance->RemoveRPMDir(safe_folder_new);

		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_folder);
		res = pInstance->AddRPMDir(safe_folder_new);


		res = pInstance->SetRPMServiceStop(true,security);
		wstring secret_path;
		res = pInstance->RPMGetSecretDir(secret_path);
#pragma endregion

#pragma region[Prepare a centralize nxl file]
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;
		string original_content = read_filecontent(ws2s(localfile_pathw));
		string membership = "";
		string tag_text = "{}";

		std::vector<SDRmFileRight> rights;
		//rights.push_back(RIGHT_VIEW);
		//rights.push_back(RIGHT_EDIT);
		//rights.push_back(RIGHT_PRINT);
		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
		std::wstring filename;
		wstring nxlfile_path;

		wstring file_name;
		uint32_t pageId = 1;
		uint32_t pageSize = 10;
		std::string orderBy = "name";
		RM_ProjectFilter filter = RF_All;
		uint32_t p_id;
		string tanen_str;
		string promem_id;
		string tanen_name;

		vector<string> project_msg = get_project_msg(1, false);
		if (project_msg.size() == 0 || project_msg[0] == "") {
			cout << "No projects from this user!" << endl;
			return;
		}
		p_id = atoi(project_msg[0].c_str());
		membership = project_msg[3];
		tanen_name = project_msg[4];

		string mem_id = puser->GetMembershipID(p_id);
		EXPECT_STREQ(mem_id.c_str(), membership.c_str());

		vector<SDR_CLASSIFICATION_CAT> categories;
		bool original = false;
		bool newly = false;
		res = puser->GetClassification(tanen_name, categories);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//Make sure tag is valid
		for (int i = 0; i < categories.size(); i++) {
			if (tag_text != "{}") {
				break;
			}
			for (int j = 0; j < categories[i].labels.size(); j++) {
				rights_eval = PolicyEvaluationResponseData(membership, categories[i].name, categories[i].labels[j].name);
				if ((rights_eval != 0) && (checkRights(rights_eval, "edit"))) {
					tag_text = "{\"" + categories[i].name + "\":[\"" + categories[i].labels[j].name + "\"]}";
					break;
				}
				else {
					continue;
				}
			}
		}

		res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text, mem_id);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		ISDRFiles* pFile = NULL;
		bool delete_result;
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		file_name = nxlfile->GetFileName();

		res = puser->UploadProjectFile(p_id, L"/", nxlfile);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		std::transform(file_name.begin(), file_name.end(), file_name.begin(), ::tolower);
		filename_lower = ws2s(file_name);
		res = puser->ProjectDownloadFile(p_id, "/" + filename_lower, projectdownloaded_path, RD_Normal);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		wstring projectdownloaded_path1 = L"C:\\auto\\output\\SDK\\Project\\";
		res = puser->ProjectDownloadFile(p_id, "/" + filename_lower, projectdownloaded_path1, RD_ForOffline);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		wstring projectdownloaded_path2 = L"C:\\auto\\output\\SDK\\Project\\";
		res = puser->ProjectDownloadPartialFile(p_id, "/" + filename_lower, projectdownloaded_path2, RD_ForOffline);
		EXPECT_EQ(res.GetCode(), 0);
		bool isfile_exist = file_exist(ws2s(projectdownloaded_path2));
		EXPECT_TRUE(isfile_exist);

#pragma endregion	
		//Copy file to RPM folder then edit
		res = pInstance->RPMEditCopyFile(projectdownloaded_path, safe_folder);
		EXPECT_FALSE(res.GetCode());

		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		std::vector<SDRmFileRight> filerights;
		res = pInstance->RPMGetRights(safe_folder, rightsAndWatermarks);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		int rights_v = rights_changeToValue(rightsAndWatermarks);
		EXPECT_EQ(rights_v, rights_eval);

		res = pInstance->RPMGetFileRights(safe_folder, filerights);
		int rights_v2 = rights_changeToValue2(filerights);
		EXPECT_EQ(rights_v2, rights_eval);

		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_cp;
		res = puser->GetFileRightsFromCentralPolicies(projectdownloaded_path, rightsAndWatermarks_cp);
		int rights_v3 = rights_changeToValue(rightsAndWatermarks_cp);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		EXPECT_EQ(rights_v3, rights_eval);

		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_cp2;
		res = puser->GetFileRightsFromCentralPolicies(tanen_name, tag_text, rightsAndWatermarks_cp2);
		int rights_v4 = rights_changeToValue(rightsAndWatermarks_cp2);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		EXPECT_EQ(rights_v4, rights_eval);

		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_tag;
		res = puser->GetFileRightsFromCentralPolicies(p_id, tag_text, rightsAndWatermarks_tag);
		int rights_v5 = rights_changeToValue(rightsAndWatermarks_tag);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		EXPECT_EQ(rights_v5, rights_eval);

		std::wstring tempFile = safe_folder;
		LPCTSTR lpFileName = tempFile.c_str();
		WIN32_FIND_DATA pNextInfo;
		HANDLE h = FindFirstFile(lpFileName, &pNextInfo);
		if (h != INVALID_HANDLE_VALUE)
		{
			FindClose(h);
		}

		string conten_str = read_filecontent(ws2s(safe_folder));
		EXPECT_STREQ(conten_str.c_str(), original_content.c_str());
		string new_content = "Edit !";
		edit_file(ws2s(safe_folder), new_content);
		res = pInstance->RPMEditSaveFile(safe_folder, L"", false, 3);
		EXPECT_FALSE(res.GetCode());
		string conten_saved = read_filecontent(ws2s(safe_folder));
		EXPECT_STREQ(conten_saved.c_str(), new_content.c_str());
		res = pInstance->RPMDeleteFile(safe_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		//Download file to RPM folder then edit
		res = puser->ProjectDownloadFile(p_id, "/" + filename_lower, safe_folder_new, RD_ForOffline);
		string safe_folder_new_s = ws2s(safe_folder_new);
		int str_len = safe_folder_new_s.length() - 4;

		std::wstring tempFile1 = safe_folder_new;
		LPCTSTR lpFileName1 = tempFile1.c_str();
		WIN32_FIND_DATA pNextInfo1;
		HANDLE h1 = FindFirstFile(lpFileName1, &pNextInfo1);
		if (h1 != INVALID_HANDLE_VALUE)
		{
			FindClose(h1);
		}
		const string safe_folder_new_ss = safe_folder_new_s.substr(0, str_len);
		string conten_str1 = read_filecontent(safe_folder_new_ss);
		EXPECT_STREQ(conten_str1.c_str(), original_content.c_str());
		string new_content1 = "Edit !";
		edit_file(safe_folder_new_ss, new_content);
		res = pInstance->RPMEditSaveFile(s2ws(safe_folder_new_ss), L"", false, 3);
		EXPECT_FALSE(res.GetCode());
		string conten_saved1 = read_filecontent(safe_folder_new_ss);
		EXPECT_STREQ(conten_saved1.c_str(), new_content1.c_str());

		res = pInstance->RPMDeleteFile(s2ws(safe_folder_new_ss));
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		std::wstring tempFile2 = s2ws(safe_folder_new_ss);
		LPCTSTR lpFileName2 = tempFile2.c_str();
		WIN32_FIND_DATA pNextInfo2;
		HANDLE h2 = FindFirstFile(lpFileName2, &pNextInfo2);
		if (h2 != INVALID_HANDLE_VALUE)
		{
			FindClose(h2);
		}

		bool exist_result = file_exist(safe_folder_new_ss);
		EXPECT_FALSE(exist_result);

		safe_folder = L"C:\\SDK test\\RPM\\Dir1";
		safe_folder_new = L"C:\\SDK test\\RPM\\Dir2";
		res = pInstance->RemoveRPMDir(remove_safe_folder, true);
		res = pInstance->RemoveRPMDir(remove_safe_folder_new, true);
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinRPMViewEditSystemBucketCPFileTest, ViewEditSytemBucketCPFileTest)
{
	try {
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir1";
		wstring remove_safe_folder = safe_folder;
		wstring safe_folder_new = L"C:\\SDK test\\RPM\\Dir2\\Starry\\";
		wstring remove_safe_folder_new = safe_folder_new;
		createFolder(ws2s(safe_folder));
		createFolder(ws2s(safe_folder_new));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		string testFolder = TestDirectory + "RPMtest";
		createFolder(testFolder);
		ISDRmUser *puser = NULL;
		string pathid;
		wstring downloaded_p = L"C:\\auto\\output\\SDK\\Project\\";
		//string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		int rights_eval = 0;
		wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
		string filename_lower;
		wstring file_name;

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		res = pInstance->RPMRegisterApp(apppath, security);
		res = pInstance->RPMNotifyRMXStatus(running, security);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

#pragma region[Prepare a ststembucket nxl file]
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;
		string original_content = read_filecontent(ws2s(localfile_pathw));
		string membership = "";
		string tag_text = "{}";

		std::vector<SDRmFileRight> rights;

		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
		std::wstring filename;
		wstring nxlfile_path;

		bool adhoc;
		int heartbeat;
		string modified;
		string tenan_str;
		ISDRmNXLFile * file;
		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		int sysprojectid;
		string sysprojecttenantid;

		res = puser->GetListProjects(1, 10, "-lastActionTime", RF_All);
		res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid, tenan_str);
		string mem_id = puser->GetMembershipID(sysprojectid);
		string default_tokengroup = puser->GetDefaultTokenGroupName();

		vector<SDR_CLASSIFICATION_CAT> categories;
		bool original = false;
		bool newly = false;
		res = puser->GetClassification(default_tokengroup, categories);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//Make sure tag is valid
		for (int i = 0; i < categories.size(); i++) {
			if (tag_text != "{}") {
				break;
			}
			for (int j = 0; j < categories[i].labels.size(); j++) {
				rights_eval = PolicyEvaluationResponseData(mem_id, categories[i].name, categories[i].labels[j].name);
				if ((rights_eval != 0) && (checkRights(rights_eval, "edit"))) {
					tag_text = "{\"" + categories[i].name + "\":[\"" + categories[i].labels[j].name + "\"]}";
					break;
				}
				else {
					continue;
				}
			}
		}

		res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text, mem_id);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		ISDRFiles* pFile = NULL;
		bool delete_result;
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		file_name = nxlfile->GetFileName();

		pInstance->Save();
		res = puser->LogoutUser();
		SDWLibDeleteRmcInstance(pInstance);
#pragma endregion	

		string cmd_str = "copy /Y \"" + ws2s(nxlfile_path) + "\"  \"" + ws2s(projectdownloaded_path) + "\"";
		system(cmd_str.c_str());

		string cmd_str_new = "copy /Y \"" + ws2s(nxlfile_path) + "\"  \"" + ws2s(safe_folder_new) + "\"";
		system(cmd_str_new.c_str());
		safe_folder_new = safe_folder_new + file_name;

		string del_cmd = "del \"" + ws2s(nxlfile_path) + "\"";

		system(del_cmd.c_str());
		projectdownloaded_path = projectdownloaded_path + file_name;

		//Copy file to RPM folder then edit
		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
#pragma region[Prepare RPM environment]
			if (login_result) {
				cout << "GetHeartBeatInfo: ";
				SDWLResult res = puser->GetHeartBeatInfo();
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
			}
			cout << "----------RPM------------" << endl;

			ret = pInstance->IsRPMDriverExist();
			cout << "IsRPMDriverExist: " << ret << endl;
			if (!ret)
			{
				cout << "Please double check if the RPM build is installed!" << endl;
			}

			string passcode_global = ReadFromFile(plan_path + passcode_txt);
			string email_temp = ws2s(puser->GetEmail());
			string user_logindata; res = pInstance->GetLoginData(email_temp, passcode_global, user_logindata);

			cout << "SetRPMLoginResult: ";
			res = pInstance->SetRPMLoginResult(user_logindata, "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}");
			EXPECT_EQ(0, res.GetCode());
			res = pInstance->RemoveRPMDir(safe_folder);
			res = pInstance->RemoveRPMDir(remove_safe_folder_new);

			cout << "AddRPMDir: ";
			res = pInstance->AddRPMDir(safe_folder);
			res = pInstance->AddRPMDir(remove_safe_folder_new);
			res = pInstance->SetRPMServiceStop(true,security);
#pragma endregion

			res = pInstance->RPMEditCopyFile(projectdownloaded_path, safe_folder);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
			std::vector<SDRmFileRight> filerights;
			res = pInstance->RPMGetRights(safe_folder, rightsAndWatermarks);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			int rights_v = rights_changeToValue(rightsAndWatermarks);
			EXPECT_EQ(rights_v, rights_eval);

			res = pInstance->RPMGetFileRights(safe_folder, filerights);
			int rights_v2 = rights_changeToValue2(filerights);
			EXPECT_EQ(rights_v2, rights_eval);

			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_cp;
			res = puser->GetFileRightsFromCentralPolicies(projectdownloaded_path, rightsAndWatermarks_cp);
			int rights_v3 = rights_changeToValue(rightsAndWatermarks_cp);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(rights_v3, rights_eval);

			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_cp2;
			res = puser->GetFileRightsFromCentralPolicies(default_tokengroup, tag_text, rightsAndWatermarks_cp2);
			int rights_v4 = rights_changeToValue(rightsAndWatermarks_cp2);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(rights_v4, rights_eval);

			std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_cp3;
			res = puser->GetFileRightsFromCentralPolicies(sysprojectid, tag_text, rightsAndWatermarks_cp3);

			std::wstring tempFile = safe_folder;
			LPCTSTR lpFileName = tempFile.c_str();
			WIN32_FIND_DATA pNextInfo;
			HANDLE h = FindFirstFile(lpFileName, &pNextInfo);
			if (h != INVALID_HANDLE_VALUE)
			{
				FindClose(h);
			}


			string conten_str = read_filecontent(ws2s(safe_folder));
			EXPECT_STREQ(conten_str.c_str(), original_content.c_str());
			string new_content = "Edit !";
			edit_file(ws2s(safe_folder), new_content);
			res = pInstance->RPMEditSaveFile(safe_folder, L"", false, 3);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			string conten_saved = read_filecontent(ws2s(safe_folder));
			EXPECT_STREQ(conten_saved.c_str(), new_content.c_str());

			string safe_folder_new_s = ws2s(safe_folder_new);
			int str_len = safe_folder_new_s.length() - 4;
			const string safe_folder_new_ss = safe_folder_new_s.substr(0, str_len);

			std::wstring tempFile1 = safe_folder_new;
			LPCTSTR lpFileName1 = tempFile1.c_str();
			WIN32_FIND_DATA pNextInfo1;
			HANDLE h1 = FindFirstFile(lpFileName1, &pNextInfo1);
			if (h1 != INVALID_HANDLE_VALUE)
			{
				FindClose(h1);
			}

			string conten_str1 = read_filecontent(safe_folder_new_ss);
			EXPECT_STREQ(conten_str1.c_str(), original_content.c_str());
			string new_content1 = "Edit !";
			edit_file(safe_folder_new_ss, new_content);
			res = pInstance->RPMEditSaveFile(s2ws(safe_folder_new_ss), L"", false, 3);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			string conten_saved1 = read_filecontent(safe_folder_new_ss);
			EXPECT_STREQ(conten_saved1.c_str(), new_content1.c_str());

			res = pInstance->RemoveRPMDir(remove_safe_folder, true);
			res = pInstance->RemoveRPMDir(remove_safe_folder_new, true);
			pInstance->Save();
			SDWLibDeleteRmcInstance(pInstance);

			cout << "-------------This case finished--------" << endl;
		}
		else {
			cout << "Login Failed!" << endl;
			EXPECT_TRUE(login_result);
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(spinRPMGetCurrentUserTest, GetCurrentUserTest) {
	try {
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		ISDRmcInstance * pInstance1;
		ISDRmTenant * pTenant1;
		ISDRmUser *puser1 = NULL;
		ISDRmcInstance * pInstance2;
		ISDRmTenant * pTenant2;
		ISDRmUser *puser2 = NULL;
		ISDRmcInstance * pInstance3;
		ISDRmTenant * pTenant3;
		ISDRmUser *puser3 = NULL;
		ISDRmcInstance * pInstance4;
		ISDRmTenant * pTenant4;
		ISDRmUser *puser4 = NULL;
		bool ret = false;
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir7";
		wstring email_user;
		string passcode = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
#pragma region [Prepare RPM environment]
			if (login_result) {
				cout << "GetHeartBeatInfo: ";
				SDWLResult res = puser->GetHeartBeatInfo();
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
			}
			cout << "----------RPM------------" << endl;

			ret = pInstance->IsRPMDriverExist();
			cout << "IsRPMDriverExist: " << ret << endl;
			if (!ret)
			{
				cout << "Please double check if the RPM build is installed!" << endl;
			}

			string passcode_global = ReadFromFile(plan_path + passcode_txt);
			string email_temp = ws2s(puser->GetEmail());
			string user_logindata; res = pInstance->GetLoginData(email_temp, passcode_global, user_logindata);

			cout << "SetRPMLoginResult: ";
			res = pInstance->SetRPMLoginResult(user_logindata, "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}");
			EXPECT_EQ(0, res.GetCode());

			res = pInstance->SetRPMServiceStop(true,security);
			ISDRmcInstance * pInstance11;
			res = RPMGetCurrentLoggedInUser(passcode, pInstance11, pTenant, puser);
			email_user = puser->GetEmail();
			EXPECT_STREQ(ws2s(email_user).c_str(), email.c_str());
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
#pragma endregion
			pInstance->Save();
			SDWLibDeleteRmcInstance(pInstance);
		}
		else {
			cout << "Login failed!" << endl;
			EXPECT_TRUE(false);
		}

		//Incorrect passcode
		string invalid_passcode = "TESTPASSCODE";
		res = RPMGetCurrentLoggedInUser(invalid_passcode, pInstance1, pTenant1, puser1);
		EXPECT_STREQ(res.GetMsg().c_str(), "Invalid passcode to auto login");
		EXPECT_EQ(res.GetCode(), 12);

		string pclientID = "";
		string  puser_return = plan_path + "User_Return.txt";

		res = RPMGetCurrentLoggedInUser(passcode, pInstance2, pTenant2, puser2);
		email_user = puser2->GetEmail();
		EXPECT_STREQ(ws2s(email_user).c_str(), email.c_str());
		EXPECT_EQ(0, res.GetCode());

		res = puser2->LogoutUser();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//cout << "RPMLogout: ";
		//res = pInstance2->RPMLogout();
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());
		//SDWLibDeleteRmcInstance(pInstance2);
		pInstance2->Save();

		res = RPMGetCurrentLoggedInUser(passcode, pInstance3, pTenant3, puser3);
		//wstring email = puser3->GetEmail();
		EXPECT_STREQ(res.GetMsg().c_str(), "User not logged in.");
		EXPECT_EQ(res.GetCode(), 12);

		cout << "RPMLogout: ";
		res = pInstance3->RPMLogout();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		pInstance->Save();
		SDWLibDeleteRmcInstance(pInstance);

		if (!res.GetCode()) {
			res = RPMGetCurrentLoggedInUser(passcode, pInstance4, pTenant4, puser4);
			wstring email4 = puser->GetEmail();
			EXPECT_STREQ(res.GetMsg().c_str(), "User not logged in.");
			EXPECT_EQ(res.GetCode(), 12);
		}
		else {
			cout << "User logout failed!" << endl;
			EXPECT_TRUE(false);
		}
		cout << "-------------This case finished--------" << endl << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(manulUserDecryptPartialTest, DecryptPartialTest) {
	try {
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);

		//txt
		const std::string nxlFile("C:\\auto\\output\\SDK\\Project\\protect_normalB-2019-08-07-03-16-46.txt.nxl");
		const std::string txtFile("C:\\zhuzhu\\12345.txt");

		//EXCEL
		//const std::string nxlFile("C:\\Users\\jzhu\\Downloads\\LocalAPP_2_TestPoint_YestCase-2019-06-06-03-10-57.xlsx.nxl");
		//const std::string txtFile("C:\\Users\\jzhu\\Downloads\\123.xlsx");

		//PPTX
		//const std::string nxlFile("C:\\Users\\jzhu\\Downloads\\PPT-2019-06-06-06-25-51.pptx.nxl");
		//const std::string txtFile("C:\\Users\\jzhu\\Downloads\\PPT.pptx");
		std::fstream fp;
		unsigned char header[16384];
		unsigned char in[2050];
		long in_len = 1000;
		long offset = 0;
		unsigned char out[2048];
		long out_len = 2048;
		long header_len = 16384;
		//adhoc
		//long header_len = 12288;
		unsigned int contentOffset = 0; // 16384; 
		int64_t contentLength = 0;

		fp.open(nxlFile, ios_base::in | ios_base::binary);
		if (!fp.is_open())
		{
			std::cout << "Error opening file";
			return;
		}
		std::ofstream ifs;
		//ifs.open(txtFile, ios_base::trunc | ios_base::out);
		ifs.open(txtFile, ios_base::trunc | ios_base::out | ios_base::binary);
		if (!ifs.is_open())
			std::cout << "Error opening output txt file";

		memset(header, 0, sizeof(header));

		fp.seekg(0, std::ios_base::end);
		size_t fsize = (size_t)fp.tellg();
		fp.seekg(0, std::ios_base::beg);
		fp.read((char*)&header, header_len);

		res = puser->PDSetupHeader(header, header_len, &contentLength, &contentOffset);
		fp.seekg(contentOffset, std::ios_base::beg);

		long bytesToRead = 512; // in_len;

		while (contentLength > 0)
		{
			memset(in, 0, sizeof(in));
			memset(out, 0, sizeof(out));
			fp.read((char*)in, bytesToRead);
			if (!fp.good())
			{
				std::cout << "Error reading file";
			}
			long count = (long)fp.gcount();
			if (count < bytesToRead)
				bytesToRead = count;
			out_len = 2048;

			res = puser->PDDecryptPartial(in, bytesToRead, offset, out, &out_len, header, header_len);

			ifs.write((char *)out, out_len);
			cout << out;
			offset += out_len;
			contentLength -= out_len;
		}

		fp.close();
		ifs.close();
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//myvault adhoc , project adhoc cp, systembucket
TEST(manulRPMViewFileTestBug, RPMViewFileTest)
{
	try {
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir1";
		createFolder(ws2s(safe_folder));

		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		string testFolder = TestDirectory + "RPMtest";
		createFolder(testFolder);
		ISDRmUser *puser = NULL;
		string pathid;
		wstring downloaded_p = L"C:\\auto\\output\\SDK\\Project\\";
		//string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email, password);
		//bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");
		res = pInstance->RPMRegisterApp(apppath, security);
		res = pInstance->RPMNotifyRMXStatus(running, security);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
#pragma region[Prepare RPM environment]
		if (login_result) {
			cout << "GetHeartBeatInfo: ";
			SDWLResult res = puser->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
		}
		cout << "----------RPM------------" << endl;

		ret = pInstance->IsRPMDriverExist();
		cout << "IsRPMDriverExist: " << ret << endl;
		if (!ret)
		{
			cout << "Please double check if the RPM build is installed!" << endl;
		}

		string passcode_global = ReadFromFile(plan_path + passcode_txt);
		string email_temp = ws2s(puser->GetEmail());
		string user_logindata; res = pInstance->GetLoginData(email_temp, passcode_global, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}");
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RemoveRPMDir(safe_folder);
		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_folder);
		res = pInstance->SetRPMServiceStop(true,security);
#pragma endregion
		for (int i = 0; i < 10; i++) {
			int processid11;
			cout << "Process id: ";
			cin >> processid11;
			res = pInstance->RPMAddTrustedProcess(processid11, security);
		}
		

		res = pInstance->RemoveRPMDir(safe_folder, true);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(manulGetClassification, GetClassification) {
	ISDRmcInstance *pInstance = NULL;
	ISDRmTenant *pTenant = NULL;
	ISDRmUser *puser = NULL;
	string email1 = "472049994@qq.com";
	string password1 = "c27b83a22f39dceb435ed6d08ec42748";
	vector<SDR_CLASSIFICATION_CAT> categories1;
	vector<SDR_CLASSIFICATION_CAT> categories2;
	SDWLResult res;
	uint32_t pageId = 1;
	uint32_t pageSize = 10;
	std::string orderBy = "name";
	RM_ProjectFilter filter = RF_All;


	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_feng, password);
	if (login_result) {
		res = puser->GetHeartBeatInfo();
		res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
		//res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
		//res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
		std::vector<SDR_PROJECT_INFO> project_info_list = puser->GetProjectsInfo();
		project_info_list = puser->GetProjectsInfo();
		//std::vector<SDR_PROJECT_INFO> project_info_list1 = puser->GetProjectsInfo();

		string name = project_info_list[0].name;
		bool owner = project_info_list[0].bowner;
		uint32_t project_id = project_info_list[0].projid;
		string   displayname = project_info_list[0].displayname;
		string   description = project_info_list[0].description;
		uint64_t	  totalfiles = project_info_list[0].totalfiles;
		//string   tenantid = project_info_list[0].tenantid;
		std::vector<SDR_PROJECT_FILE_INFO> listfiles;
		res = puser->GetProjectListFiles(project_info_list[0].projid, 1, 20, orderBy, "/", "", listfiles);
		std::vector<SDR_PROJECT_FILE_INFO> listfiles1;
		res = puser->GetProjectListFiles(project_info_list[0].projid, 1, 20, orderBy, "/123123/", "", listfiles1);

		string name1 = project_info_list[1].name;
		bool owner1 = project_info_list[1].bowner;
		uint32_t project_id1 = project_info_list[1].projid;
		string   displayname1 = project_info_list[1].displayname;
		string   description1 = project_info_list[1].description;
		uint64_t	  totalfiles1 = project_info_list[1].totalfiles;
		string   tokengroupname = project_info_list[1].tokengroupname;
		std::vector<SDR_PROJECT_FILE_INFO> listfiles11;
		res = puser->GetProjectListFiles(project_info_list[0].projid, 1, 20, orderBy, "/aaa/mmm/uuu/", "", listfiles11);

		string name2 = project_info_list[2].name;
		bool owner2 = project_info_list[2].bowner;
		uint32_t project_id2 = project_info_list[2].projid;
		string   displayname2 = project_info_list[2].displayname;
		string   description2 = project_info_list[2].description;
		uint64_t	  totalfiles2 = project_info_list[2].totalfiles;
		string   tokengroupname2 = project_info_list[2].tokengroupname;

		cout << "project name: " << name2 << endl;
		res = puser->GetClassification(tokengroupname2, categories1);
		if (res) {
			for each(SDR_CLASSIFICATION_CAT cat in categories1)
			{	
				cout << "{" << endl;
				cout << "	\"name\": " << cat.name << endl;
				cout << "	\"multiSelect\": " << cat.multiSelect << endl;
				cout << "	\"mandatory\": " << cat.mandatory << endl;
				cout << "	\"labels\": [" << endl;
				for each (SDR_CLASSIFICATION_LABELS lab in cat.labels)
				{
					cout <<	"		{" << endl;
					cout <<	"			\"name\": " << lab.name << endl;
					cout <<	"			\"default\": " << lab.allow << endl;
					cout <<	"		}" << endl;
				}
				cout <<	"	]" << endl;
				cout << "}" << endl;
			}
		}
		res = puser->GetClassification(tokengroupname2, categories2);

		res = puser->LogoutUser();
	}
	pInstance->Save();
	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl;

	login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);

	if (login_result) {
		res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
		std::vector<SDR_PROJECT_INFO> project_info_list = puser->GetProjectsInfo();
		res = puser->GetClassification(project_info_list[0].tokengroupname, categories1);
		cout << "project name: " << project_info_list[0].name << endl;
		if (res) {
			for each(SDR_CLASSIFICATION_CAT cat in categories1)
			{
				cout << "{" << endl;
				cout << "	\"name\": " << cat.name << endl;
				cout << "	\"multiSelect\": " << cat.multiSelect << endl;
				cout << "	\"mandatory\": " << cat.mandatory << endl;
				cout << "	\"labels\": [" << endl;
				for each (SDR_CLASSIFICATION_LABELS lab in cat.labels)
				{
					cout << "		{" << endl;
					cout << "			\"name\": " << lab.name << endl;
					cout << "			\"default\": " << lab.allow << endl;
					cout << "		}" << endl;
				}
				cout << "	]" << endl;
				cout << "}" << endl;
			}
		}
		res = puser->GetClassification(project_info_list[0].tokengroupname, categories2);

		res = puser->LogoutUser();
	}
	pInstance->Save();
	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl;
}

TEST(manulTempUserSystemProjectTest, SystemProjectTest) {
	try {
		ISDRmcInstance *pInstance;
		ISDRmTenant *pTenant;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		wstring nxlfile_path;
		wstring cpnxlfile_path;

		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;

		string tenant = ws2s(DEFUALT_TENANT);
		string tag_text = "{}";
		//string change_tag = "{\"allRights\":[\"allRights\"]}";
		wstring w_name;
		wstring localfile_pathw = plan_file_path + txtnormal_testfile;
		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_DOWNLOAD);

		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };

		bool ret = false;
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir7";
		string passcode = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		//bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");
		if (login_result) {
			res = puser->GetHeartBeatInfo();

			bool adhoc;
			int heartbeat;
			string modified;
			string tenan_str;
			ISDRmNXLFile * file;
			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
			int sysprojectid;
			string sysprojecttenantid;

			res = puser->GetListProjects(1, 10, "-lastActionTime", RF_All);
			res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid, tenan_str);
			string mem_id = puser->GetMembershipID(sysprojectid);
			string tenant_id = puser->GetSystemProjectTenantId();
			string default_tid = puser->GetDefaultTenantId();
			string defaultTokengroupname = puser->GetDefaultTokenGroupName();
			cout << "adhoc: " << adhoc << " heartbeat: " << heartbeat << " sysprojectid: " << sysprojectid << " sysprojecttenantid: " << sysprojecttenantid << "tenan_str" << tenan_str << endl;

			vector<SDR_CLASSIFICATION_CAT> categories;
			vector<SDR_CLASSIFICATION_CAT> categories1;
			res = puser->GetClassification(default_tid, categories);
			res = puser->GetClassification(defaultTokengroupname, categories1);
			checkSDWLResult(res);
			cout << "mem_id: " << mem_id << " tenant_id: " << tenant_id << " default_tid: " << default_tid << endl;
			res = puser->ProtectFile(paln_file_ws + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire, tag_text, mem_id);
			cout << "-------------This case finished--------" << endl;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}


TEST(checkRights, checkRightsTest) {
	ISDRmcInstance *pInstance;
	ISDRmTenant *pTenant;
	SDWLResult res;
	ISDRmUser *puser = NULL;

	bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");
	//bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email_feng, password);
	if (login_result) {
		wstring download_path = L"C:\\zhuzhu";
		res = puser->ProjectDownloadFile(13, "/aa/ssss/0psWOjpIbm8-2019-06-25-17-42-42.jpg.nxl", download_path, RD_ForOffline);
		vector<SDRmFileRight> rights;
		res = puser->GetProjectFileRights("13", "/aa/ssss/0psWOjpIbm8-2019-06-25-17-42-42.jpg.nxl", rights);
		
		vector<SDRmFileRight> rights2;
		res = pInstance->RPMGetFileRights(download_path, rights2);
		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		wstring file_path = L"C:\\zhuzhu\\0psWOjpIbm8-2019-06-25-17-42-42.jpg.nxl";
		res = pInstance->RPMGetRights(download_path, rightsAndWatermarks);
		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks2;
		wstring file_path1 = L"C:\\zhuzhu\\0psWOjpIbm8-2019-06-25-17-42-42.jpg.nxl";
		res = pInstance->RPMGetRights(download_path, rightsAndWatermarks);


		ISDRmNXLFile *nxlfile;
		res = puser->OpenFile(download_path, &nxlfile);
		checkSDWLResult(res);
		bool open = nxlfile->IsOpen();
		if (open) {
			

		}
	}

}

TEST(changeFileTagTest, changeFileTag) {

	ISDRmcInstance *pInstance;
	ISDRmTenant *pTenant;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	wstring nxlfile_path;
	wstring cpnxlfile_path;

	SDR_Expiration expire;
	expire.type = IExpiryType::NEVEREXPIRE;

	string tenant = ws2s(DEFUALT_TENANT);
	string tag_text = "{}";
	//string change_tag = "{\"allRights\":[\"allRights\"]}";
	wstring w_name;
	wstring localfile_pathw = plan_file_path + txtnormal_testfile;
	std::vector<SDRmFileRight> rights;
	//rights.push_back(RIGHT_VIEW);
	//rights.push_back(RIGHT_DOWNLOAD);
	string tag="{\"Rights\":[\"Rights_Edit\"]}";
	SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };

	bool adhoc;
	int heartbeat;
	string tenan_str;
	int sysprojectid;
	string sysprojecttenantid;

	bool ret = false;
	wstring safe_folder = L"C:\\SDK test\\RPM\\Dir7";
	string passcode = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");

	res = puser->GetListProjects(1, 10, "-lastActionTime", RF_All);
	res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid, tenan_str);
	string mem_id = puser->GetMembershipID(sysprojectid);
	string tenant_id = puser->GetSystemProjectTenantId();
	string default_tid = puser->GetDefaultTenantId();
	string defaultTokengroupname = puser->GetDefaultTokenGroupName();
	if (login_result) {

		wstring nxlFilePath = L"C:\\Users\\sfeng\\Desktop\\c++-2019-09-26-08-50-33.txt.nxl";
		string fileName;
		string parethPathID;
		res = puser->ChangeRightsOfProjectFile(nxlfile_path, sysprojectid, "c++-2019-09-26-08-50-33.txt.nxl", "/", rights, watermark_info, expire, tag);

		cout << "--------test end---------" << endl;
	}
}

