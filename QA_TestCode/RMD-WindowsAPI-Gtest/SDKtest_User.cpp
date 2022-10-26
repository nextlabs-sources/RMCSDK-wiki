#include "stdafx.h"

using namespace std;
static string clientID = "";
bool Default_Login(ISDRmcInstance *&pInstance, ISDRmTenant *&pTenant, ISDRmUser *&puser, const wstring &router, const wstring &tenant, const string email, const string password) {
	try {
		SDWLResult res;

		string pclientID = "";
		string  puser_return = plan_path + "User_Return.txt";

		res = CreateSDRmcInstance_CreateClientID(L"C:\\work", pInstance, pclientID, router, tenant, L"C:\\Program Files\\NextLabs");
		//res = CreateSDRmcInstance_CreateClientID(L"C:\\SDK test\\John", pInstance, pclientID, router, tenant, L"C:\\Program Files\\NextLabs");

		Sleep(1000);
		bool te_result = GetTenantResponseData(router);
		Sleep(1000);
		bool finished = false;
		int i = 0;
		do {
			i++;
			Sleep(1000);
			res = pInstance->IsInitFinished(finished);
			cout << i << ": Wait RMCInstance Class finish initialization...";
			cout << " >res: ";
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (i == 20)
			{
				cout << i << "Cannot connect to PDPMan" << endl;
				break;
			}
		} while (!finished);

		if (!res.GetCode()) {
			// pInstance->Save();
			GetUserLoginResponseData(email, password, pclientID, puser_return);
			string user_return = ReadFromFile(puser_return);
			Json::Value jroot;
			jroot = ParseJsonFile(puser_return);

			res = pInstance->SetLoginResult(user_return, &puser, secr);
			pInstance->GetCurrentTenant(&pTenant);
			wcout << L"Current Tenant:  " << pTenant->GetTenant() << endl;
			if (!res.GetCode()) {
				string ps_code = puser->GetPasscode();
				cout << "User passcode is :" + ps_code << endl;
				string exportfilename = plan_path + passcode_txt;
				ofstream SaveFile(exportfilename);
				SaveFile << ps_code;
				SaveFile.close();
				pInstance->Save();
				cout << "Login Successful!" << endl;
				return true;
			}
			else {
				cout << "Login failed!" << endl;
				return false;
			}
		}
		else {
			cout << "Create Instance failed!" << endl;
			return false;
		}
	}

	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

bool SDKRPM_Login(ISDRmcInstance *&pInstance, ISDRmTenant *&pTenant, ISDRmUser *&puser, const wstring &router, const wstring &tenant, const string email, const string password) {
	try {
		SDWLResult res;

		string pclientID = "";
		string  puser_return = plan_path + "User_Return.txt";

		res = CreateSDRmcInstance_CreateClientID(L"C:\\work", pInstance, pclientID, router, tenant, L"C:\\Program Files\\NextLabs");
		//res = CreateSDRmcInstance_CreateClientID(L"C:\\SDK test\\John", pInstance, pclientID, router, tenant, L"C:\\Program Files\\NextLabs");

		Sleep(1000);
		bool te_result = GetTenantResponseData(router);
		Sleep(1000);
		bool finished = false;
		int i = 0;
		do {
			i++;
			Sleep(1000);
			res = pInstance->IsInitFinished(finished);
			cout << i << ": Wait RMCInstance Class finish initialization...";
			cout << " >res: ";
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (i == 20)
			{
				cout << i << "Cannot connect to PDPMan" << endl;
				break;
			}
		} while (!finished);

		if (!res.GetCode()) {
			// pInstance->Save();
			GetUserLoginResponseData(email, password, pclientID, puser_return);
			string user_return = ReadFromFile(puser_return);
			Json::Value jroot;
			jroot = ParseJsonFile(puser_return);

			res = pInstance->SetLoginResult(user_return, &puser, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			pInstance->GetCurrentTenant(&pTenant);
			wcout << L"Current Tenant:  " << pTenant->GetTenant() << endl;
			if (!res.GetCode()) {
				string ps_code = puser->GetPasscode();
				cout << "User passcode is :" + ps_code << endl;
				string exportfilename = plan_path + passcode_txt;
				ofstream SaveFile(exportfilename);
				SaveFile << ps_code;
				SaveFile.close();
				pInstance->Save();
				cout << "SDK Login Successful!" << endl;
				cout << "GetHeartBeatInfo: ";
				SDWLResult res = puser->GetHeartBeatInfo();
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());

				cout << "----------RPM------------" << endl;

				bool ret = pInstance->IsRPMDriverExist();
				cout << "IsRPMDriverExist: " << ret << endl;
				if (!ret)
				{
					cout << "Please double check if the RPM build is installed!" << endl;
				}
				string passcode = puser->GetPasscode();
				string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);
				res = pInstance->SetRPMServiceStop(true, secr);
				cout << "SetRPMLoginResult: ";
				res = pInstance->SetRPMLoginResult(user_logindata, secr);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				if (res.GetCode())
				{
					cout << "RPM Login failed!" << endl;
					return false;
				}
				return true;
			}
			else {
				cout << "Login failed!" << endl;
				return false;
			}
		}
		else {
			cout << "Create Instance failed!" << endl;
			return false;
		}
	}

	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

bool Admin_Login(ISDRmcInstance *&pInstance, ISDRmTenant *&pTenant, ISDRmUser *&puser, const wstring &router, const wstring &tenant, const string email, const string password, string id) {
	try {
		SDWLResult res;
		ISDRmHttpRequest *prequest = NULL;

		string pclientID = "";

		res = CreateSDRmcInstance_CreateClientID(L"C:\\work", pInstance, pclientID, router, tenant, L"C:\\Program Files\\NextLabs");

		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		Sleep(1000);
		bool te_result = GetTenantResponseData(router);
		Sleep(1000);
		bool finished = false;
		int i = 0;
		do {
			i++;
			Sleep(1000);
			res = pInstance->IsInitFinished(finished);
			cout << i << ": Wait RMCInstance Class finish initialization...";
			cout << " >res: ";
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (i == 20)
			{
				cout << i << "Cannot connect to PDPMan" << endl;
				break;
			}
		} while (!finished);

		if (!res.GetCode()) {
			bool respond_result = AdminLoginResponseData("john.tyler", "john.tyler", "2", pclientID);
			string user_return = ReadFromFile(user_respond);
			Json::Value jroot;
			jroot = ParseJsonFile(user_respond);

			res = pInstance->SetLoginResult(user_return, &puser, secr);
			pInstance->GetCurrentTenant(&pTenant);

			cout << "GetHeartBeatInfo: ";
			SDWLResult res = puser->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			string passcode = puser->GetPasscode();
			string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);
			res = pInstance->SetRPMServiceStop(true, secr);
			cout << "SetRPMLoginResult: ";
			res = pInstance->SetRPMLoginResult(user_logindata, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			wcout << L"Current Tenant:  " << pTenant->GetTenant() << endl;
			if (!res.GetCode()) {
				string ps_code = puser->GetPasscode();
				cout << "User passcode is :" + ps_code << endl;
				string exportfilename = plan_path + passcode_txt;
				ofstream SaveFile(exportfilename);
				SaveFile << ps_code;
				SaveFile.close();
				pInstance->Save();
				cout << "Login Successful!" << endl;
				return true;
			}
			else {
				cout << "Login failed!" << endl;
				return false;
			}
		}
		else {
			cout << "Create Instance failed!" << endl;
			return false;
		}
		return res;

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//bool Switch_User(ISDRmcInstance *&pInstance, ISDRmUser *&puser, const string email, const string password) {
//	try {
//		SDWLResult res;
//		ISDRmHttpRequest *prequest = NULL;
//		string clientId = "";
//		string  puser_return = "C:\\SDK test data\\User_Return.txt";
//
//		res = pInstance->GetLoginRequest(&prequest);
//		checkSDWLResult(res);
//		EXPECT_EQ(0, res.GetCode());
//		if (res.GetCode() == 0) {
//			cout << "-----LoginRequest-----" << endl;
//			for each (HttpCookie cookie in prequest->GetCookies())
//			{
//				wcout << "Cookie name:[" << cookie.first << "]";
//				wcout << "  Vaule:[" << cookie.second << "]" << endl;
//				if (ws2s(cookie.first) == "clientId")
//				{
//					clientId = ws2s(cookie.second);
//					string exportfilename = plan_path + "client_id.txt";
//					ofstream SaveFile(exportfilename);
//					SaveFile << clientId;
//					SaveFile.close();
//				}
//			}
//
//			GetUserLoginResponseData(email, password, clientId, puser_return);
//			string user_return = ReadFromFile(puser_return);
//			if (!res.GetCode()) {
//				res = pInstance->SetLoginResult(user_return, &puser, secr);
//				string ps_code = puser->GetPasscode();
//				cout << "User passcode is : " + ps_code << endl;
//				string exportfilename = plan_path + "passcode.txt";
//				ofstream SaveFile(exportfilename);
//				SaveFile << ps_code;
//				SaveFile.close();
//				cout << "Login Successful!" << endl;
//				return true;
//			}
//			else {
//				cout << "Login failed!" << endl;
//				return false;
//			}
//		}
//		else {
//			cout << "GetLogin Request failed!" << endl;
//			return false;
//		}
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

bool PreviousUser_Login(ISDRmcInstance *&pInstance, ISDRmUser *&puser, const wstring &router, const wstring &tenant, const string email, string passcode) {
	SDWLResult res;
	ISDRmTenant *pTenant = NULL;
	string clientID = "";
	res = CreateSDRmcInstance_CreateClientID(L"C:\\work", pInstance, clientID, router, tenant);
	//res = SDWLibCreateSDRmcInstance("Product name", 1, 1, 1621, L"C:\\Program Files\\NextLabs", L"C:\\work", &pInstance);
	//res = pInstance->Initialize(L"C:\\work", router, tenant);
	if (!res.GetCode()) {
		res = pInstance->GetLoginUser(email, passcode, &puser);
		//res = pInstance->GetCurrentTenant(&pTenant);
		//wcout << L"Tenant from previous user:  " << pTenant->GetTenant() << endl;
		if (!res.GetCode()) {
			cout << "Previous user login!" << endl;

			string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);
			res = pInstance->SetRPMServiceStop(true, secr);
			cout << "SetRPMLoginResult: ";
			res = pInstance->SetRPMLoginResult(user_logindata, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (res.GetCode())
			{
				cout << "RPM Login failed!" << endl;
				return false;
			}
			return true;
		}
		else {
			return false;
			cout << res.GetMsg() << endl;
			cout << "Get previous user failed!" << endl;
		}
	}
	else {
		cout << "Create instance failed!" << endl;
		return false;
	}
}

bool python_login(ISDRmcInstance *&pInstance, ISDRmTenant *&pTenant, ISDRmUser *&puser, const wstring &router, const wstring &tenant, const string email, const string password) {
	//std::string r1 = GetLoginResult(user, pass, "fullid", "d:\\test\\rt.json");
	return true;
}

const std::vector<SDRmFileRight> Tick_Rights(string rights_str) {
	std::vector<SDRmFileRight> rights;
	vector<string> r;
	splitString(rights_str, r, "|");
	for (vector<string>::size_type i = 0; i != r.size(); ++i)
	{
		if (r[i] == "view")
		{
			rights.push_back(RIGHT_VIEW);
		}
		else if (r[i] == "edit")
		{
			rights.push_back(RIGHT_EDIT);
		}
		else if (r[i] == "print")
		{
			rights.push_back(RIGHT_PRINT);
		}
		else if (r[i] == "copycontent")
		{
			rights.push_back(RIGHT_CLIPBOARD);
		}
		else if (r[i] == "saveas")
		{
			rights.push_back(RIGHT_SAVEAS);
		}
		else if (r[i] == "decrypt")
		{
			rights.push_back(RIGHT_DECRYPT);
		}
		else if (r[i] == "sc")
		{
			rights.push_back(RIGHT_SCREENCAPTURE);
		}
		else if (r[i] == "send")
		{
			rights.push_back(RIGHT_SEND);
		}
		else if (r[i] == "classify")
		{
			rights.push_back(RIGHT_CLASSIFY);
		}
		else if (r[i] == "share")
		{
			rights.push_back(RIGHT_SHARE);
		}
		else if (r[i] == "download")
		{
			rights.push_back(RIGHT_DOWNLOAD);
		}
		else if (r[i] == "watermark")
		{
			rights.push_back(RIGHT_WATERMARK);
		}
	}
	return rights;
}

const std::vector<std::string> Get_Recipients(string recipients_str) {
	vector<string> r;
	splitString(recipients_str, r, "|");
	return r;
}


class ValidUser :public::testing::TestWithParam<vector<string>> {};
TEST_P(ValidUser, ValidInput)
{
	vector<string> csv = GetParam();
	string caseID = csv[0];
	string jsonFile = csv[1];

	cout << "caseID: " << caseID << endl;
	cout << "jsonFile: " << jsonFile << endl;

	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmTenant *pTenant = NULL;
	ISDRmUser *puser = NULL;

	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	string user_return = ReadFromFile("C:\\SDK test data\\User_Return.txt");
	cout << "passcode : " << puser->GetPasscode() << endl;
	if (login_result) {
		// using 3rd party Jsoncpp to read and get json value
		Json::Value jroot;
		jroot = ParseJsonFile("C:\\SDK test data\\User_Return.txt");

		string userName = "";
		string email = "";
		int idpT = 0;

		userName = jroot["extra"]["name"].asString();
		email = jroot["extra"]["email"].asString();
		idpT = jroot["extra"]["idpType"].asInt();

		//wcout << "Username: " << puser->GetName() << endl;
		wstring wstr = puser->GetName();
		string str = ws2s(wstr);
		EXPECT_STREQ(userName.c_str(), str.c_str());

		wcout << "Email address: " << puser->GetEmail() << endl;
		wstr = puser->GetEmail();
		str = ws2s(wstr);
		EXPECT_STREQ(email.c_str(), str.c_str());

		wcout << "IDP type: " << puser->GetIdpType() << endl;
		EXPECT_EQ(idpT, puser->GetIdpType());

		uint64_t usage = 0;
		uint64_t totalquota = 0;
		uint64_t vaultusage = 0;
		uint64_t vaultquota = 0;

		res = puser->GetMyDriveInfo(usage, totalquota, vaultusage, vaultquota);
		EXPECT_FALSE(res.GetCode());
		res = puser->UpdateUserInfo();
		EXPECT_FALSE(res.GetCode());
		res = puser->GetMyDriveInfo(usage, totalquota, vaultusage, vaultquota);
		EXPECT_FALSE(res.GetCode());
	}

	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl;
}
INSTANTIATE_TEST_CASE_P(UserTesting, ValidUser, testing::ValuesIn(csv_valid));

//Test Share local file
class UserShareTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(UserShareTest, ShareTest) {
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

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
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
						//Get nxl file activity log from server
						//res = puser->GetNXLFileActivitylog(nxlfile, 0, 20, 0, "", 0, false);
						//EXPECT_FALSE(res.GetCode());
						//Get log
						res = puser->GetActivityInfo(filename, infoVec);
						EXPECT_FALSE(res.GetCode());
					}
					else {
						//Update recipeints
						res = puser->UpdateRecipients(nxlfile, { "11111@qq.com" });
						EXPECT_FALSE(res.GetCode());
						//Get nxl file activity log from server
						/*res = puser->GetNXLFileActivitylog(nxlfile, 0, 20, 0, "", 0, false);
						EXPECT_EQ(res.GetCode(), 61844);
						EXPECT_STREQ(res.GetMsg().c_str(), "File not found");*/
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
		login_result = SDKRPM_Login(pInstance_b, pTenant_b, puser_b, DEFUALT_ROUTER, L"", email_zhu, password);
		if (login_result) {
			res = puser_b->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
			if (res.GetCode() == 0) {
				ISDRFiles* pFile = NULL;
				std::wstring filename;
				bool delete_result;
				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					filename = nxlfile->GetFileName();
					//Upload file
					res = puser_b->UploadFile(nxlfile_path, nxlfile_path, recipients, comments_w);
					if (!res.GetCode()) {
						//Get rights
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
						//Rights verified
						bool rights_result = rights_verified(rights, nxlfile);
						EXPECT_TRUE(rights_result);

						//Update recipeints
						res = puser_b->UpdateRecipients(nxlfile, { "11111@qq.com" });
						EXPECT_FALSE(res.GetCode());
						//Get nxl file activity log from server
						/*res = puser_b->GetNXLFileActivitylog(nxlfile, 0, 20, 0, "", 0, false);
						EXPECT_FALSE(res.GetCode());*/
						//Get log from local
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
INSTANTIATE_TEST_CASE_P(UserShareTesting, UserShareTest, testing::ValuesIn(csv_share));

TEST(UserFailGetLog, Log) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		SDR_WATERMARK_INFO watermark_info = { "watermark_text","font","color",9,0,ANTICLOCKWISE,true };
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		wstring nxlfile_path;

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			res = puser->ProtectFile(plan_file_path + pptxnormal_testfile, nxlfile_path, rights, watermark_info, expire, "");
			if (!res.GetCode())
			{
				std::wstring filename;
				bool delete_result;
				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				//if (nxlfile)
				//{
				//	res = puser->GetNXLFileActivitylog(nxlfile, 0, 20, 0, "", 0, false);
				//	EXPECT_TRUE(res.GetCode());
				//	if (res.GetCode()) {
				//		EXPECT_FALSE(1);
				//		cout << "Here is a message issue!" << endl;
				//		cout << "Get activity log message: " << res.GetMsg() << endl;
				//	}
				//}
			}
			else {
				cout << "Share file failed!" << endl;
			}
		}
		else {
			EXPECT_TRUE(login_result);
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

TEST(UserShareInvalidTest, CommentsTooLong) {
	try {
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;

		wstring localfile_pathw = plan_file_path + txtnormal_testfile;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;

		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);

		string tag_text = "{\"EAR\":[\"EAR - 01\"]}";
		wstring comments_w = L"test test test test test test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test testtest test test test";
		wstring recipients = L"123123@qq.com";
		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			wstring nxlfile_path;
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
			if (res.GetCode() == 0) {
				//ISDRFiles* pFile = NULL;
				std::wstring filename;
				bool delete_result;
				//res = puser->GetLocalFileManager(&pFile);
				//size_t no = pFile->GetListNumber();
				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					filename = nxlfile->GetFileName();

					//Upload file
					res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, comments_w);
					EXPECT_EQ(res.GetCode(), 65447);
					EXPECT_STREQ(res.GetMsg().c_str(), "Comment too long");
					//Upload activity log
					//res = puser->UploadActivityLogs();
					//EXPECT_FALSE(res.GetCode());
					//Update recipeints
					res = puser->UpdateRecipients(nxlfile, { "11111@qq.com" });
					EXPECT_TRUE(res.GetCode());

					//Remove file
					//delete_result = pFile->RemoveFile(nxlfile);
					//EXPECT_TRUE(delete_result);
				}
				else {
					cout << "Upload file failed!" << endl;
					EXPECT_FALSE(res.GetCode());
				}

				res = puser->CloseFile(nxlfile);
			}
			else {
				cout << "Share file failed!" << endl;
				EXPECT_TRUE(false);
			}
			pInstance->Save();
			puser->LogoutUser();
		}
		else {
			EXPECT_FALSE(res.GetCode());
			cout << "User login failed!" << endl;
		}
		SDWLibDeleteRmcInstance(pInstance);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(UserShareInvalidTest, InvalidEmail) {
	try {
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;

		wstring localfile_pathw = plan_file_path + txtnormal_testfile;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;

		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);

		string tag_text = "{\"EAR\":[\"EAR - 01\"]}";
		wstring comments_w = L"test ";
		wstring recipients = L"123123";
		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			wstring nxlfile_path;
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
			if (res.GetCode() == 0) {
				ISDRFiles* pFile = NULL;
				std::wstring filename;
				bool delete_result;
				//res = puser->GetLocalFileManager(&pFile);
				//size_t no = pFile->GetListNumber();
				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
				if (nxlfile)
				{
					filename = nxlfile->GetFileName();

					//Upload file
					res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, comments_w);
					EXPECT_EQ(res.GetCode(), 61840);
					EXPECT_STREQ(res.GetMsg().c_str(), "One or more emails have an invalid format.");
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
			pInstance->Save();
			puser->LogoutUser();
		}
		else {
			EXPECT_FALSE(res.GetCode());
			cout << "User login failed!" << endl;
		}
		SDWLibDeleteRmcInstance(pInstance);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Test protect local file
class UserProtectTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(UserProtectTest, ProtectTest) {
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
					             
					//Get nxl file activity log
					//res = puser->GetNXLFileActivityLog(nxlfile, 0, 20, RL_SFEmpty, "", RL_OBAccessTime, false);
					//res = puser->GetNXLFileActivityLog(nxlfile, 0, 20, 0, "", 0, false);
					EXPECT_FALSE(res.GetCode());
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
		login_result = SDKRPM_Login(pInstance_b, pTenant_b, puser_b, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		if (login_result) {
			wstring nxlfile_path;
			res = puser_b->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
			if (res.GetCode() == 0) {
				//ISDRFiles* pFile = NULL;
				std::wstring filename;
				bool delete_result;
				//res = puser_b->GetLocalFileManager(&pFile);
				//size_t no = pFile->GetListNumber();
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
					//Decrypt file
					//SRMActivityLogOperation log = RLOView;
					//res = puser_b->DecryptNXLFile(nxlfile, decrypt_file_path + w_name, log);
					//EXPECT_FALSE(res.GetCode());

					//Upload file
					res = puser_b->UploadFile(nxlfile_path, nxlfile_path);
					EXPECT_FALSE(res.GetCode());
					//Upload activity log
					//res = puser_b->UploadActivityLogs();
					//EXPECT_FALSE(res.GetCode());
					//Get nxl file activity log
					/*res = puser_b->GetNXLFileActivitylog(nxlfile, 0, 20, 0, "", 0, false);
					EXPECT_FALSE(res.GetCode());*/
					res = puser_b->GetActivityInfo(filename, infoVec);

					EXPECT_FALSE(res.GetCode());
					EXPECT_EQ(infoVec.size(), 1);
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
INSTANTIATE_TEST_CASE_P(UserProtectTesting, UserProtectTest, testing::ValuesIn(csv_protect));

//Protect and share file without rights
TEST(UserProtectWithoutRights, ProtectWithoutRights) {
	try {
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;

		string tag_text = "{}";
		wstring w_name;
		wstring localfile_pathw = plan_file_path + bmpnormal_testfile;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		std::vector<SDRmFileRight> rights;
		//rights.push_back(RIGHT_VIEW);

		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };

		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			//vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

			//res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\t-2019-05-29-06-31-56.png.nxl", rightsAndWatermarks, false);
			wstring nxlfile_path;

			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
			if (res.GetCode() == 0) {
				std::wstring filename;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					bool is_open = nxlfile->IsOpen();
					EXPECT_TRUE(is_open);
					//Get file name
					filename = nxlfile->GetFileName();

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

					//Get nxl file activity log
					/*res = puser->GetNXLFileActivitylog(nxlfile, 0, 20, 0, "", 0, false);
					EXPECT_FALSE(res.GetCode());*/
					res = puser->GetActivityInfo(filename, infoVec);
					EXPECT_FALSE(res.GetCode());
				}
			}
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
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

					//Get tag
					string tag = nxlfile->GetTags();
					EXPECT_STREQ(tag.c_str(), tag_text.c_str());
					//Get rights
					vector<SDRmFileRight> rights_v = nxlfile->GetRights();
					EXPECT_EQ(rights_v.size(), rights.size());
					bool rights_result = rights_verified(rights, nxlfile);
					EXPECT_TRUE(rights_result);

					//Upload file
					nxlfile_path = L"";
					res = puser->UploadFile(nxlfile_path, nxlfile_path, L"3361106242@qq.com", L"Comments Test");
					EXPECT_FALSE(res.GetCode());

					//Get nxl file activity log
					/*res = puser->GetNXLFileActivitylog(nxlfile, 0, 20, 0, "", 0, false);
					EXPECT_FALSE(res.GetCode());*/
					res = puser->GetActivityInfo(filename, infoVec);
					EXPECT_FALSE(res.GetCode());
				}
			}
			puser->LogoutUser();
			pInstance->Save();
		}
		else {
			cout << "login failed!" << endl;
		}
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

TEST(UserProtectEmptyFile, ProtectEmptyFile) {
	try {
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;

		string tag_text = "{}";
		wstring w_name;
		wstring localfile_pathw = plan_file_path + txtempty_testfile;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		std::vector<SDRmFileRight> rights;

		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };

		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			wstring nxlfile_path;
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
			//res = puser->ShareFile(localfile_pathw, rights, { "3361106242@qq.com" }, L"Comments Test", watermark_info, expire, tag_text);
			if (res.GetCode() == 0) {
				std::wstring filename;
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

					//Upload file
					res = puser->UploadFile(nxlfile_path, nxlfile_path);
					EXPECT_EQ(res.GetCode(), 66445);
					EXPECT_STREQ(res.GetMsg().c_str(), "Empty files are not allowed to be uploaded.");
				}
			}
			else {
				EXPECT_FALSE(res.GetCode());
				cout << "User protect local file failed!" << endl;
			}
			puser->LogoutUser();
			pInstance->Save();
		}
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

//class UserProtectTest2 :public::testing::TestWithParam<vector<string>> {};
//TEST_P(UserProtectTest2, UserProtectTest2) {
//	try {
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmUser *puser = NULL;
//		vector<string> csv = GetParam();
//		SDWLResult res;
//		ISDRmTenant *pTenant = NULL;
//		ISDRmcInstance *pInstance_b = NULL;
//		ISDRmUser *puser_b = NULL;
//		SDR_Expiration expire;
//		expire.type = IExpiryType::NEVEREXPIRE;
//
//		string rights_str = csv[4];
//		string file_name = csv[2];
//		string file_path = csv[3];
//		string localfile_path = file_path + file_name;
//		wstring localfile_pathw = s2ws(localfile_path);
//		string message = csv[5];
//		std::vector<SDRmFileRight> rights;
//		rights = Tick_Rights(rights_str);
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		//res = puser->LogoutUser();
//		//SDWLibDeleteRmcInstance(pInstance);
//		string passcode_global = ReadFromFile(plan_path + passcode_txt);
//		login_result = PreviousUser_Login(pInstance_b, puser_b, DEFUALT_ROUTER, DEFUALT_TENANT, email, passcode_global);
//		if (login_result) {
//			wstring nxlfile_path;
//			//for (int i = 0; i < 55; i++) {
//			//	res = puser->ProtectFile(L"C:\\SDK test data\\plan file\\Protect_normal.txt", nxlfile_path, rights, watermarkinfo, expire, tags);
//			//	Sleep(2000);
//			//}
//			res = puser_b->ProtectFile(localfile_pathw, nxlfile_path, rights, watermarkinfo, expire, tags);
//			cout << res.GetCode() << endl;
//			cout << res.GetMsg() << endl;
//			EXPECT_STREQ(res.GetMsg().c_str(), message.c_str());
//			Sleep(1000);
//			res = puser_b->ShareFile(localfile_pathw, nxlfile_path, rights, { "test@123.com" }, L"comments", watermarkinfo, expire, tags);
//			cout << res.GetCode() << endl;
//			cout << res.GetMsg() << endl;
//			EXPECT_STREQ(res.GetMsg().c_str(), message.c_str());
//
//			ISDRFiles* pFile = NULL;
//			bool delete_result;
//			res = puser_b->GetLocalFileManager(&pFile);
//			size_t no = pFile->GetListNumber();
//			std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//			SDR_FILE_ACTIVITY_INFO ac_info;
//			if (no)
//			{
//				ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
//				ISDRmNXLFile* nxlfile2 = pFile->GetFile(no - 2);
//				//Remove file
//				delete_result = pFile->RemoveFile(nxlfile);
//				EXPECT_TRUE(delete_result);
//				delete_result = pFile->RemoveFile(nxlfile2);
//				EXPECT_TRUE(delete_result);
//			}
//			cout << "Test finished!" << endl;
//		}
//		else {
//			cout << "Get previouse user failed!" << endl;
//			EXPECT_TRUE(login_result);
//		}
//		SDWLibDeleteRmcInstance(pInstance_b);
//
//		//login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		//if (login_result) {
//		//	wstring nxlfile_path;
//		//	for (int i = 0; i < 55; i++) {
//		//		res = puser->ProtectFile(L"C:\\SDK test data\\plan file\\Protect_normal.txt", nxlfile_path, rights, watermarkinfo, expire, tags);
//		//		Sleep(2000);
//		//	}
//		//}
//		//SDWLibDeleteRmcInstance(pInstance);
//	}
//	catch (...) {
//		printf("excption!");
//	}
//}
//INSTANTIATE_TEST_CASE_P(UserProtectTesting2, UserProtectTest2, testing::ValuesIn(csv_User_InvalidProtect));

TEST(UserProtectWithLongTag, WithLongTag) {
	try {
		SDR_Expiration expire;
		expire.type = IExpiryType::RELATIVEEXPIRE;
		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;
		expire.start = (uint64_t)std::time(nullptr) * 1000;

		string tag_text = ReadFromFile(plan_path + tag_txt);
		//string tag_text = "{}";
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		SDR_WATERMARK_INFO watermark_info = { "Jewelry","font","color",9,0,ANTICLOCKWISE,true };
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			wstring nxlfile_path;
			res = puser->ProtectFile(plan_file_path + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire, tag_text);
			EXPECT_EQ(res.GetCode(), 13);
			EXPECT_STREQ(res.GetMsg().c_str(), "The max length of tag string is 4096");
			cout << "Test finished!" << endl;
		}
		SDWLibDeleteRmcInstance(pInstance);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Test diff expiration type local and upload it from get login user
class UserDiffExpirationLocalTest :public::testing::TestWithParam<IExpiryType> {};
TEST_P(UserDiffExpirationLocalTest, DiffExpirationTest) {
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
		SDWLibDeleteRmcInstance(pInstance_pre);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
INSTANTIATE_TEST_CASE_P(UserDiffExpirationLocalTesting, UserDiffExpirationLocalTest, testing::ValuesIn(csv_expiration));

////Test diff expiration type  server file
//class UserDiffExpirationServerTest :public::testing::TestWithParam<IExpiryType> {};
//TEST_P(UserDiffExpirationServerTest, DiffExpirationTest) {
//	try {
//		wstring localfile_pathw;
//		string file_name;
//		IExpiryType para = GetParam();
//		if (para == NEVEREXPIRE) {
//			localfile_pathw = server_file_path + servernever_testfile;
//			file_name = ws2s(servernever_testfile);
//		}
//		else if (para == RELATIVEEXPIRE) {
//			localfile_pathw = server_file_path + serverrelative_testfile;
//			file_name = ws2s(serverrelative_testfile);
//		}
//		else if (para == ABSOLUTEEXPIRE) {
//			localfile_pathw = server_file_path + serverabsolute_testfile;
//			file_name = ws2s(serverabsolute_testfile);
//		}
//		else if (para == RANGEEXPIRE) {
//			localfile_pathw = server_file_path + serverrange_testfile;
//			file_name = ws2s(serverrange_testfile);
//		}
//
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		uint64_t start_ex;
//		uint64_t end_ex;
//		int date_size;
//		string mem_id = "";
//		bool expired;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, L"https://rmtest.nextlabs.solutions", L"skydrm.com", email, password);
//		if (login_result) {
//			bool result = GetFileInfoResponseData(file_name);
//			if (result) {
//				Json::Value jroot;
//				jroot = ParseJsonFile(fileinfo_return);
//				int code = jroot["statusCode"].asInt();
//				if (code == 200) {
//					date_size = jroot["results"]["detail"]["validity"].size();
//					if (date_size == 1) {
//						end_ex = jroot["results"]["detail"]["validity"]["endDate"].asInt64();
//					}
//					else if (date_size == 2) {
//						end_ex = jroot["results"]["detail"]["validity"]["endDate"].asInt64();
//						start_ex = jroot["results"]["detail"]["validity"]["startDate"].asInt64();
//					}
//				}
//			}
//			ISDRmNXLFile* nxlfile = NULL;
//			puser->OpenFile(localfile_pathw, &nxlfile);
//			bool is_open = nxlfile->IsOpen();
//			if (is_open) {
//				wstring file_name = nxlfile->GetFileName();
//
//				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//				//Get nxl file activity log from server
//				res = puser->GetNXLFileActivitylog(nxlfile, 0, 20, 0, "", 0, false);
//				EXPECT_FALSE(res.GetCode());
//				//Get log from local
//				res = puser->GetActivityInfo(file_name, infoVec);
//				EXPECT_FALSE(res.GetCode());
//
//				SDR_Expiration ex = nxlfile->GetExpiration();
//				uint64_t start = ex.start;
//				uint64_t end = ex.end;
//				if (ex.type == RELATIVEEXPIRE || ex.type == ABSOLUTEEXPIRE) {
//					expired = nxlfile->CheckExpired("m4994@skydrm.com");
//					EXPECT_EQ(end, end_ex);
//				}
//				else if (ex.type == RANGEEXPIRE) {
//					expired = nxlfile->CheckExpired("m4994@skydrm.com");
//					EXPECT_EQ(end, end_ex);
//					EXPECT_EQ(start, start_ex);
//				}
//				cout << "Test !" << endl;
//			}
//			else {
//				cout << "Open file failed!" << endl;
//				EXPECT_TRUE(0);
//			}
//		}
//		else {
//			cout << "Login failed!" << endl;
//			EXPECT_TRUE(login_result);
//		}
//		SDWLibDeleteRmcInstance(pInstance);
//
//		//Get previous user
//		ISDRmcInstance *pInstance_pre = NULL;
//		ISDRmUser *puser_pre = NULL;
//		string passcode_global = ReadFromFile(plan_path + passcode_txt);
//		login_result = PreviousUser_Login(pInstance_pre, puser_pre, L"https://rmtest.nextlabs.solutions", L"skydrm.com", email, passcode_global);
//		if (login_result) {
//			ISDRmNXLFile* nxlfile = NULL;
//			res = puser_pre->OpenFile(localfile_pathw, &nxlfile);
//			bool is_open = nxlfile->IsOpen();
//			if (is_open) {
//				wstring file_name = nxlfile->GetFileName();
//				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//				//Get nxl file activity log from server
//				res = puser_pre->GetNXLFileActivitylog(nxlfile, 0, 20, 0, "", 0, false);
//				EXPECT_FALSE(res.GetCode());
//				//Get log from local
//				res = puser_pre->GetActivityInfo(file_name, infoVec);
//				EXPECT_FALSE(res.GetCode());
//
//				SDR_Expiration ex = nxlfile->GetExpiration();
//				uint64_t start = ex.start;
//				uint64_t end = ex.end;
//				if (ex.type == RELATIVEEXPIRE || ex.type == ABSOLUTEEXPIRE) {
//					EXPECT_EQ(end, end_ex);
//				}
//				else if (ex.type == RANGEEXPIRE) {
//					EXPECT_EQ(end, end_ex);
//					EXPECT_EQ(start, start_ex);
//				}
//			}
//			else {
//				cout << "Open file failed!" << endl;
//				EXPECT_TRUE(0);
//			}
//		}
//		else {
//			cout << "Login failed!" << endl;
//			EXPECT_TRUE(login_result);
//		}
//		SDWLibDeleteRmcInstance(pInstance_pre);
//		cout << "-------------This case finished--------" << endl;
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}
//INSTANTIATE_TEST_CASE_P(UserDiffExpirationServerTesting, UserDiffExpirationServerTest, testing::ValuesIn(csv_expiration));

////Test diff expiration type  server file
//class UserDiffExpirationServerTest :public::testing::TestWithParam<IExpiryType> {};
//TEST_P(UserDiffExpirationServerTest, DiffExpirationTest) {
//	try {
//		wstring localfile_pathw;
//		string file_name;
//		IExpiryType para = GetParam();
//		bool expired;
//		if (para == NEVEREXPIRE) {
//			localfile_pathw = server_file_path + L"never-2018-07-06-06-19-59.txt.nxl";
//			file_name = "never-2018-07-06-06-19-59.txt.nxl";
//		}
//		else if (para == RELATIVEEXPIRE) {
//			localfile_pathw = server_file_path + L"relative-2018-07-06-05-55-59.txt.nxl";
//			file_name = "relative-2018-07-06-05-55-59.txt.nxl";
//		}
//		else if (para == ABSOLUTEEXPIRE) {
//			localfile_pathw = server_file_path + L"absolute (2)-2018-07-06-05-56-29.txt.nxl";
//			file_name = "absolute%20(2)-2018-07-06-05-56-29.txt.nxl";
//		}
//		else if (para == RANGEEXPIRE) {
//			localfile_pathw = server_file_path + L"range-2018-07-06-05-57-15.txt.nxl";
//			file_name = "range-2018-07-06-05-57-15.txt.nxl";
//		}
//
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		uint64_t start_ex;
//		uint64_t end_ex;
//		int date_size;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, L"https://rmtest.nextlabs.solutions", L"skydrm.com", email, password);
//		if (login_result) {
//			bool result = GetFileInfoResponseData(file_name);
//			if (result) {
//				Json::Value jroot;
//				jroot = ParseJsonFile(fileinfo_return);
//				int code = jroot["statusCode"].asInt();
//				if (code == 200) {
//					date_size = jroot["results"]["detail"]["validity"].size();
//					if (date_size == 1) {
//						end_ex = jroot["results"]["detail"]["validity"]["endDate"].asInt64();
//					}
//					else if (date_size == 2) {
//						end_ex = jroot["results"]["detail"]["validity"]["endDate"].asInt64();
//						start_ex = jroot["results"]["detail"]["validity"]["startDate"].asInt64();
//					}
//				}
//			}
//			ISDRmNXLFile* nxlfile = NULL;
//			puser->OpenFile(localfile_pathw, &nxlfile);
//			bool is_open = nxlfile->IsOpen();
//			if (is_open) {
//				wstring file_name = nxlfile->GetFileName();
//
//				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//				//Get nxl file activity log from server
//				res = puser->GetNXLFileActivitylog(nxlfile, 0, 20, 0, "", 0, false);
//				EXPECT_FALSE(res.GetCode());
//				//Get log from local
//				res = puser->GetActivityInfo(file_name, infoVec);
//				EXPECT_FALSE(res.GetCode());
//
//				SDR_Expiration ex = nxlfile->GetExpiration();
//				uint64_t start = ex.start;
//				uint64_t end = ex.end;
//				if (ex.type == RELATIVEEXPIRE || ex.type == ABSOLUTEEXPIRE) {
//					expired = nxlfile->CheckExpired("m4994@skydrm.com");
//					EXPECT_EQ(end, end_ex);
//				}
//				else if (ex.type == RANGEEXPIRE) {
//					expired = nxlfile->CheckExpired("m4994@skydrm.com");
//					EXPECT_EQ(end, end_ex);
//					EXPECT_EQ(start, start_ex);
//				}
//				cout << "Test !" << endl;
//			}
//			else {
//				cout << "Open file failed!" << endl;
//				EXPECT_TRUE(0);
//			}
//		}
//		else {
//			cout << "Login failed!" << endl;
//			EXPECT_TRUE(login_result);
//		}
//		SDWLibDeleteRmcInstance(pInstance);
//
//		//Get previous user
//		ISDRmcInstance *pInstance_pre = NULL;
//		ISDRmUser *puser_pre = NULL;
//		string passcode_global = ReadFromFile(plan_path + "passcode.txt");
//		login_result = PreviousUser_Login(pInstance_pre, puser_pre, L"https://rmtest.nextlabs.solutions", L"skydrm.com", email, passcode_global);
//		if (login_result) {
//			ISDRmNXLFile* nxlfile = NULL;
//			res = puser_pre->OpenFile(localfile_pathw, &nxlfile);
//			bool is_open = nxlfile->IsOpen();
//			if (is_open) {
//				wstring file_name = nxlfile->GetFileName();
//				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//				//Get nxl file activity log from server
//				res = puser_pre->GetNXLFileActivitylog(nxlfile, 0, 20, 0, "", 0, false);
//				EXPECT_FALSE(res.GetCode());
//				//Get log from local
//				res = puser_pre->GetActivityInfo(file_name, infoVec);
//				EXPECT_FALSE(res.GetCode());
//
//				SDR_Expiration ex = nxlfile->GetExpiration();
//				uint64_t start = ex.start;
//				uint64_t end = ex.end;
//				if (ex.type == RELATIVEEXPIRE || ex.type == ABSOLUTEEXPIRE) {
//					EXPECT_EQ(end, end_ex);
//				}
//				else if (ex.type == RANGEEXPIRE) {
//					EXPECT_EQ(end, end_ex);
//					EXPECT_EQ(start, start_ex);
//				}
//			}
//			else {
//				cout << "Open file failed!" << endl;
//				EXPECT_TRUE(0);
//			}
//		}
//		else {
//			cout << "Login failed!" << endl;
//			EXPECT_TRUE(login_result);
//		}
//		SDWLibDeleteRmcInstance(pInstance_pre);
//		cout << "-------------This case finished--------" << endl;
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}
//INSTANTIATE_TEST_CASE_P(UserDiffExpirationServerTesting, UserDiffExpirationServerTest, testing::ValuesIn(csv_expiration));

////Update recipients from protect local file
//TEST(UserUpdateRecipeintsProtectTest, RecipientsLocalTest) {
//	try {
//		SDR_Expiration expire;
//		expire.type = IExpiryType::NEVEREXPIRE;
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		wstring localfile_pathw = plan_file_path + pngnormal_testfile;
//		std::vector<SDRmFileRight> rights;
//		rights.push_back(RIGHT_VIEW);
//		rights.push_back(RIGHT_EDIT);
//		rights.push_back(RIGHT_SHARE);
//		wstring file_name;
//		wstring nxlfile_path;
//
//		std::vector<std::string> recipientsemail = { "11111@nextlabs.com","222222@nextlab.com" };
//		std::vector<std::string> addrecipientsemail = { "addrecipients@nextlabs.com","jewelry.zhu@nextlabs.com" };
//		std::vector<std::string> removerecipientsemail = { "jewelry.zhu@nextlabs.com" };
//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//
//		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			ISDRFiles* pFile = NULL;
//			
//			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermarkinfo, expire, tags);
//			if (!res.GetCode()) {
//				ISDRmNXLFile* nxlfile;
//				res = puser->OpenFile(nxlfile_path, &nxlfile);
//				bool recipients_sync;
//				//ISDRmNXLFile* nxlfile = pFile->GetFile(0);
//				file_name = nxlfile->GetFileName();
//				std::vector<std::string> currentrecipients;
//				std::vector<std::string> addrecipient;
//				std::vector<std::string> removerecipients;
//				//Update recipients online
//				res = puser->UpdateRecipients(nxlfile, addrecipientsemail, {});
//				//Add "addrecipients@nextlabs.com","jewelry.zhu@nextlabs.com"
//				EXPECT_FALSE(res.GetCode());
//				recipients_sync = nxlfile->IsRecipientsSynced();
//				EXPECT_FALSE(recipients_sync);
//				//Get Recipients
//				res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//				res = puser->UpdateRecipients(nxlfile, {}, removerecipientsemail);
//				//Remove "jewelry.zhu@nextlabs.com" - >  "addrecipients@nextlabs.com"
//				recipients_sync = nxlfile->IsRecipientsSynced();
//				EXPECT_FALSE(recipients_sync);
//				EXPECT_FALSE(res.GetCode());
//				//Get Recipients
//				res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//				wstring filename = nxlfile->GetFileName();
//
//				//Is Upload
//				bool is_upload = nxlfile->IsUploadToRMS();
//				EXPECT_FALSE(is_upload);
//
//				//Get Recipients
//				res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//				recipients_sync = nxlfile->IsRecipientsSynced();
//				EXPECT_FALSE(recipients_sync);
//				//Upload file
//				res = puser->UploadFile(nxlfile_path, nxlfile_path);
//				EXPECT_FALSE(res.GetCode());
//				is_upload = nxlfile->IsUploadToRMS();
//				EXPECT_TRUE(is_upload);
//
//				recipients_sync = nxlfile->IsRecipientsSynced();
//				EXPECT_TRUE(recipients_sync);
//
//				//Upload activity log
//				//res = puser->UploadActivityLogs();
//				//EXPECT_FALSE(res.GetCode());
//
//				res = puser->UpdateRecipients(nxlfile, addrecipientsemail, {});
//				//Add "addrecipients@nextlabs.com","jewelry.zhu@nextlabs.com" -> Add "addrecipients@nextlabs.com","jewelry.zhu@nextlabs.com"
//				recipients_sync = nxlfile->IsRecipientsSynced();
//				EXPECT_TRUE(recipients_sync);
//				//Get Recipients
//				res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//
//				res = puser->UpdateRecipients(nxlfile, {}, {});
//				recipients_sync = nxlfile->IsRecipientsSynced();
//				EXPECT_TRUE(recipients_sync);
//				//Get Recipients
//				res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//				bool fileinfo = GetFileInfoResponseData(ws2s(file_name));
//				if (fileinfo) {
//					Json::Value jroot;
//					jroot = ParseJsonFile(fileinfo_return);
//					int size = jroot["results"]["detail"]["recipients"].size();
//					EXPECT_EQ(currentrecipients.size(), size);
//					for (int i = 0; i < size; i++) {
//						EXPECT_STREQ(jroot["results"]["detail"]["recipients"][i].asString().c_str(), currentrecipients[i].c_str());
//					}
//
//				}
//				//Is valid NXL file
//				bool valid_result = nxlfile->IsValidNXL();
//				EXPECT_TRUE(valid_result);
//				//Remove file
//				//delete_result = pFile->RemoveFile(nxlfile);
//				//EXPECT_TRUE(delete_result);
//
//				if (!res.GetCode()) {
//					cout << "Update recipeints finished!" << endl;
//				}
//				else {
//					cout << "Update recipients failed!";
//					EXPECT_FALSE(res.GetCode());
//				}
//
//			}
//			else {
//				cout << "Protect file failed!" << endl;
//				EXPECT_FALSE(1);
//			}
//		}
//		else {
//			cout << "User login failed!" << endl;
//			EXPECT_TRUE(login_result);
//		}
//		SDWLibDeleteRmcInstance(pInstance);
//		cout << "-------------This case finished--------" << endl;
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//
//}
//
////Update recipients from share local file
//TEST(UserUpdateRecipeintsTest, RecipientsShareTest) {
//	try {
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		wstring localfile_pathw = plan_file_path + bmpnormal_testfile;
//		std::vector<SDRmFileRight> rights;
//		rights.push_back(RIGHT_VIEW);
//		rights.push_back(RIGHT_EDIT);
//		rights.push_back(RIGHT_SHARE);
//
//		SDR_Expiration expire;
//		expire.type = IExpiryType::NEVEREXPIRE;
//
//		std::vector<std::string> recipientsemail = { "aaaaa@nextlabs.com","bbbbb@nextlab.com" };
//		std::vector<std::string> addrecipientsemail = { "ccccc@nextlabs.com","ddddd@nextlabs.com" };
//		std::vector<std::string> addrecipientsemail2 = { "eeeee@nextlabs.com","fffff@nextlabs.com" };
//		std::vector<std::string> addrecipientsemail3 = { "ggggg@nextlabs.com","hhhhh@nextlabs.com" };
//		std::vector<std::string> removerecipientsemail = { "ddddd@nextlabs.com" };
//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			ISDRFiles* pFile = NULL;
//			wstring nxlfile_path;
//			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermarkinfo, expire, tags);
//			//Share:  -> "11111@nextlabs.com","222222@nextlab.com"
//			//res = puser->ProtectFile(localfile_pathw, rights, watermarkinfo, tags);
//
//			std::vector<std::string> currentrecipients;
//			std::vector<std::string> addrecipient;
//			std::vector<std::string> removerecipients;
//
//			ISDRmNXLFile* nxlfile;
//			res = puser->OpenFile(nxlfile_path, &nxlfile);
//
//			wstring filename = nxlfile->GetFileName();
//			bool recipients_sync;
//			//Get Recipients
//			res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//
//			res = puser->UpdateRecipients(nxlfile, addrecipientsemail, {});
//			//Add recipients:  ->  "11111@nextlabs.com","222222@nextlab.com", "3333@nextlabs.com","4444@nextlabs.com"
//			EXPECT_FALSE(res.GetCode());
//			recipients_sync = nxlfile->IsRecipientsSynced();
//			EXPECT_FALSE(recipients_sync);
//			//Get Recipients
//			res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//			EXPECT_EQ(currentrecipients.size(), 4);
//			res = puser->UpdateRecipients(nxlfile, addrecipientsemail2, removerecipientsemail);
//			//Add and remove: "11111@nextlabs.com","222222@nextlab.com", "3333@nextlabs.com",  "55@nextlabs.com","66@nextlabs.com"
//			EXPECT_FALSE(res.GetCode());
//			recipients_sync = nxlfile->IsRecipientsSynced();
//			EXPECT_FALSE(recipients_sync);
//			//Get Recipients
//			res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//			EXPECT_EQ(currentrecipients.size(), 5);
//			res = puser->UploadFile(nxlfile_path, nxlfile_path);
//			EXPECT_FALSE(res.GetCode());
//			recipients_sync = nxlfile->IsRecipientsSynced();
//			EXPECT_TRUE(recipients_sync);
//
//			//Upload activity log
//			//res = puser->UploadActivityLogs();
//
//			res = puser->UpdateRecipients(nxlfile, addrecipientsemail3, removerecipientsemail);
//			//Add and remove:  -> "11111@nextlabs.com","222222@nextlab.com", "3333@nextlabs.com",  "55@nextlabs.com","66@nextlabs.com", "77@nextlabs.com","88@nextlabs.com"
//			EXPECT_FALSE(res.GetCode());
//			recipients_sync = nxlfile->IsRecipientsSynced();
//			EXPECT_TRUE(recipients_sync);
//			//Get Recipients
//			res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//			EXPECT_EQ(currentrecipients.size(), 7);
//			res = puser->UpdateRecipients(nxlfile, {}, {});
//			recipients_sync = nxlfile->IsRecipientsSynced();
//			EXPECT_TRUE(recipients_sync);
//
//			//Get Recipients
//			res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//			bool fileinfo = GetFileInfoResponseData(ws2s(filename));
//			if (fileinfo) {
//				Json::Value jroot;
//				jroot = ParseJsonFile(fileinfo_return);
//				int size = jroot["results"]["detail"]["recipients"].size();
//				EXPECT_EQ(currentrecipients.size(), size);
//			}
//			//Remove file
//			//delete_result = pFile->RemoveFile(nxlfile);
//			//EXPECT_TRUE(delete_result);
//		}
//		else {
//			cout << "User login failed!" << endl;
//			EXPECT_TRUE(login_result);
//		}
//		SDWLibDeleteRmcInstance(pInstance);
//		cout << "-------------This case finished--------" << endl;
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

////update recipients from server file
//TEST(UserUpdateRecipientServerExpiredTest, RecipeintServerExpireTest) {
//	try {
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		ISDRmNXLFile* nxlfile = NULL;
//		wstring file_name = serveshared_testfile;
//		std::vector<std::string> recipientsemail = { "11111@nextlabs.com","222222@nextlab.com" };
//		std::vector<std::string> addrecipientsemail = { "3333@nextlabs.com","4444@nextlabs.com" };
//		std::vector<std::string> addrecipientsemail2 = { "55@nextlabs.com","66@nextlabs.com" };
//		std::vector<std::string> removerecipientsemail = { "11111@123.com" };
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, L"https://rmtest.nextlabs.solutions", L"skydrm.com", email, password);
//		if (login_result) {
//			std::vector<std::string> currentrecipients;
//			std::vector<std::string> addrecipient;
//			std::vector<std::string> removerecipients;
//
//			res = puser->OpenFile(server_file_path + file_name, &nxlfile);
//			SRMActivityLogOperation log = RLOView;
//			bool result = GetUpdateRecipientsResponseData(ws2s(serveshared_testfile), { "111@qq.com" }, { "222@ww.com" });
//			Json::Value jroot;
//			jroot = ParseJsonFile(recipients_return);
//			uint64_t code = jroot["statusCode"].asInt64();
//			string message;
//			if (code != 200) {
//				message = jroot["message"].asString();
//			}
//			res = puser->UpdateRecipients(nxlfile, addrecipientsemail, {});
//			EXPECT_EQ(res.GetCode(), code + 61440);
//			EXPECT_STREQ(res.GetMsg().c_str(), message.c_str());
//			//Get Recipients
//			res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//			res = puser->UpdateRecipients(nxlfile, {}, removerecipientsemail);
//			//Get Recipients
//			res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//			EXPECT_FALSE(res.GetCode());
//			SDWLibDeleteRmcInstance(pInstance);
//			cout << "-------------This case finished--------" << endl;
//		}
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}
//
////update recipients from server file
//TEST(UserUpdateRecipientServerTest, RecipeintServerTest) {
//	try {
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		ISDRmNXLFile* nxlfile = NULL;
//		std::vector<std::string> recipientsemail = { "11111@nextlabs.com","222222@nextlab.com" };
//		std::vector<std::string> addrecipientsemail = { "3333@nextlabs.com","4444@nextlabs.com" };
//		std::vector<std::string> addrecipientsemail2 = { "55@nextlabs.com","66@nextlabs.com" };
//		std::vector<std::string> removerecipientsemail = { "11111@123.com" };
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, L"https://rmtest.nextlabs.solutions", L"skydrm.com", email, password);
//		if (login_result) {
//			std::vector<std::string> currentrecipients;
//			std::vector<std::string> addrecipient;
//			std::vector<std::string> removerecipients;
//			bool recipients_sync;
//			res = puser->OpenFile(server_file_path + serveshared2_testfile, &nxlfile);
//			res = puser->UpdateRecipients(nxlfile, addrecipientsemail, {});
//			EXPECT_FALSE(res.GetCode());
//			recipients_sync = nxlfile->IsRecipientsSynced();
//			EXPECT_TRUE(recipients_sync);
//			//Get Recipients
//			res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//			if (currentrecipients.size() == addrecipientsemail.size()) {
//				for (int i = 0; i < currentrecipients.size(); i++)
//				{
//					EXPECT_STREQ(currentrecipients[i].c_str(), addrecipientsemail[i].c_str());
//				}
//			}
//			else {
//				EXPECT_EQ(currentrecipients.size(), addrecipientsemail.size());
//			}
//
//			res = puser->UpdateRecipients(nxlfile, {}, addrecipientsemail);
//			//Get Recipients
//			res = puser->GetRecipients(nxlfile, currentrecipients, addrecipient, removerecipients);
//			EXPECT_FALSE(res.GetCode());
//			EXPECT_FALSE(currentrecipients.size());
//			SDWLibDeleteRmcInstance(pInstance);
//			cout << "-------------This case finished--------" << endl;
//		}
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

////Sync recipients
//TEST(UserSyncRecipients, SyncRecipients) {
//	try {
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		wstring localfile_pathw = plan_file_path + txtnormal_testfile;
//		std::vector<SDRmFileRight> rights;
//		rights.push_back(RIGHT_VIEW);
//		rights.push_back(RIGHT_EDIT);
//		rights.push_back(RIGHT_SHARE);
//
//		SDR_Expiration expire;
//		expire.type = IExpiryType::NEVEREXPIRE;
//
//		std::vector<std::string> recipientsemail = { "11111@nextlabs.com","222222@nextlab.com" };
//		std::vector<std::string> addrecipientsemail = { "3333@nextlabs.com","4444@nextlabs.com" };
//		std::vector<std::string> addrecipientsemail2 = { "55@nextlabs.com","66@nextlabs.com" };
//		std::vector<std::string> removerecipientsemail = { "4444@nextlabs.com" };
//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			ISDRFiles* pFile = NULL;
//			wstring nxlfile_path;
//			for (int i = 0; i < 10; i++) {
//				res = puser->ShareFile(localfile_pathw, nxlfile_path, rights, recipientsemail, L"Comments Test", watermarkinfo, expire, tags);
//				Sleep(2000);
//			}
//			bool delete_result;
//			res = puser->GetLocalFileManager(&pFile);
//			size_t no = pFile->GetListNumber();
//			for (int j = no - 1; j >= 0; j--) {
//				ISDRmNXLFile* nxlfile = pFile->GetFile(j);
//				//puser->UploadFile(nxlfile);
//				res = puser->UpdateRecipients(nxlfile, addrecipientsemail, {});
//				EXPECT_FALSE(res.GetCode());
//				//Remove file
//				delete_result = pFile->RemoveFile(nxlfile);
//				EXPECT_TRUE(delete_result);
//			}
//		}
//		else {
//			cout << "User login failed!" << endl;
//			EXPECT_TRUE(login_result);
//		}
//		SDWLibDeleteRmcInstance(pInstance);
//		cout << "-------------This case finished--------" << endl;
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

//Get activity log from server(file download from server)
//TEST(UserGetActivityLogFromServerTest, GetActivityLogFromServer) {
//	try {
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		uint64_t startPos = 10;
//		uint64_t count = 100;
//		uint8_t searchField = 0;
//		string tag_s = "{\"itar\":[\"taa-01\"]}";
//
//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, L"https://rmtest.nextlabs.solutions", L"skydrm.com", email, password);
//		if (login_result) {
//			std::wstring nxlfilepath = server_file_path + servegetlog_testfile;
//
//			ISDRmNXLFile * file = NULL;
//			res = puser->OpenFile(nxlfilepath, &file);
//
//			//File from server download User A
//
//			//Get Rights
//			std::vector<SDRmFileRight> rights = file->GetRights();
//			string tags_user = file->GetTags();
//			EXPECT_STREQ(tags_user.c_str(), tag_s.c_str());
//			//Get watermark
//			//SDR_WATERMARK_INFO wm_a = puser->GetWaterMark(file);
//			//EXPECT_STREQ(wm_a.text.c_str(), watermarkinfo.text.c_str());
//			wstring filename = file->GetFileName();
//			bool result = GetActivityLogResponseData(ws2s(filename), "0", "100", "accessTime", "true");
//			Json::Value jroot;
//			jroot = ParseJsonFile(activity_return);
//			int code = jroot["statusCode"].asInt();
//			int activity_count;
//			if (code == 200) {
//				activity_count = jroot["results"]["data"]["logRecords"].size();
//			}
//			//Get Activity log
//			res = puser->GetNXLFileActivitylog(file, startPos, count, searchField, "", searchField, false);
//			res = puser->GetActivityInfo(filename, infoVec);
//			SDR_FILE_ACTIVITY_INFO log = infoVec[0];
//			EXPECT_FALSE(res.GetCode());
//			//EXPECT_EQ(infoVec.size(), 5);
//			EXPECT_GT(infoVec.size(), activity_count);
//			//Get tags
//			string tags_a = file->GetTags();
//			EXPECT_STREQ(tags_a.c_str(), tag_s.c_str());
//			//Get expiration time
//			SDR_Expiration ex = file->GetExpiration();
//			EXPECT_EQ(ex.type, 1);
//			cout << "Test finished!" << endl;
//
//			//File server User B
//			//Get Activity log
//
//			nxlfilepath = server_file_path + log_userb_testfile;
//			ISDRmNXLFile * file_local_b = NULL;
//			res = puser->OpenFile(nxlfilepath, &file_local_b);
//			EXPECT_FALSE(res.GetCode());
//			res = puser->GetNXLFileActivitylog(file_local_b, startPos, count, searchField, "", searchField, false);
//			res = puser->GetActivityInfo(log_userb_testfile, infoVec);
//			std::vector<SDRmFileRight> rights_b = file_local_b->GetRights();
//			EXPECT_EQ(rights_b.size(), 5);
//			//SDR_WATERMARK_INFO wm_local_b = puser->GetWaterMark(file_local_b);
//			//EXPECT_STREQ("$(User)$(Break)$(Date) $(Time)", wm_local_b.text.c_str());
//			//Get expiration time
//			SDR_Expiration ex_b = file_local_b->GetExpiration();
//			EXPECT_EQ(ex_b.type, 0);
//			cout << "Test finished!" << endl;
//
//			//File server User C
//			nxlfilepath = server_file_path + log_userc_testfile;
//			ISDRmNXLFile * file_local_c = NULL;
//			res = puser->OpenFile(nxlfilepath, &file_local_c);
//			EXPECT_STREQ("Unauthorized", res.GetMsg().c_str());
//			cout << "Test finished!" << endl;
//		}
//		SDWLibDeleteRmcInstance(pInstance);
//		cout << "-------------This case finished--------" << endl;
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

//TEST(UserLargeFileTest, LargeFileTest) {
//	try {
//		SDR_Expiration expire;
//		expire.type = IExpiryType::NEVEREXPIRE;
//
//		string tag_text = "{}";
//		//string membership = "m5122@t-5e0de1b1f6984d239896f6a6e432f4d0";
//		wstring w_name;
//		wstring localfile_pathw = plan_file_path + L"t.bmp";
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		std::vector<SDRmFileRight> rights;
//		rights.push_back(RIGHT_VIEW);
//
//		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };
//
//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		//bool login_result = Default_Login(pInstance, pTenant, puser, L"https://jenkins-centos7302.qapf1.qalab01.nextlabs.com:8443", L"785901fa-92c0-4ce7-b280-c44b56efe17e", "john.tyler", "john.tyler");
//		//if (login_result) {
//
//		//	//res = puser->ProtectFile(L"C:\\SDK test data\\plan file\\5M00.doc", rights, watermark_info, expire, tag_text);
//		//	//EXPECT_FALSE(res.GetCode());
//		//	//res = puser->ProtectFile(L"C:\\work\\5M04.doc", rights, watermark_info, expire, tag_text);
//		//	//EXPECT_FALSE(res.GetCode());
//		//	//res = puser->ProtectFile(L"C:\\SDK test data\\plan file\\5M00.doc", rights, watermark_info, expire, tag_text);
//		//	//EXPECT_FALSE(res.GetCode());
//		//	res = puser->ProtectFile(L"C:\\SDK test data\\plan file\\5M04.doc", rights, watermark_info, expire, tag_text);
//		//	EXPECT_FALSE(res.GetCode());
//		//	res = puser->ProtectFile(L"C:\\SDK test data\\plan file\\5M04.doc", rights, watermark_info, expire, tag_text);
//		//	EXPECT_FALSE(res.GetCode());
//		//	res = puser->ProtectFile(L"C:\\SDK test data\\plan file\\5M04.doc", rights, watermark_info, expire, tag_text);
//		//	EXPECT_FALSE(res.GetCode());
//		//	res = puser->ProtectFile(L"C:\\SDK test data\\plan file\\5M04.doc", rights, watermark_info, expire, tag_text);
//		//	EXPECT_FALSE(res.GetCode());
//		//	res = puser->ProtectFile(L"C:\\SDK test data\\plan file\\5M04.doc", rights, watermark_info, expire, tag_text);
//		//	//EXPECT_FALSE(res.GetCode());
//		//	//res = puser->ProtectFile(L"C:\\SDK test data\\plan file\\5M00.doc", rights, watermark_info, expire, tag_text);
//		//	//EXPECT_FALSE(res.GetCode());
//		//	//res = puser->ProtectFile(L"C:\\SDK test data\\plan file\\5M04.doc", rights, watermark_info, expire, tag_text);
//		//	//EXPECT_FALSE(res.GetCode());
//
//		//	if (res.GetCode() == 0) {
//		//		ISDRFiles* pFile = NULL;
//		//		std::wstring filename;
//		//		bool delete_result;
//		//		res = puser->GetLocalFileManager(&pFile);
//		//		size_t no = pFile->GetListNumber();
//		//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//		//		SDR_FILE_ACTIVITY_INFO ac_info;
//		//		for (no; no > 0; no--)
//		//		{
//		//			ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
//		//			if (nxlfile)
//		//			{
//		//				time_t timeBegin, timeEnd;
//		//				timeBegin = time(NULL);
//
//		//				timeEnd = time(NULL);
//		//				printf("%d\n", timeEnd - timeBegin);
//		//				//Upload file
//		//				//res = puser->UploadFile(nxlfile);
//		//				//EXPECT_FALSE(res.GetCode());
//		//				timeEnd = time(NULL);
//		//				printf("%d\n", timeEnd - timeBegin);
//
//		//				//Isupload
//		//				bool is_upload = nxlfile->IsUploadToRMS();
//		//				EXPECT_TRUE(is_upload);
//
//		//				/*res = puser->UploadFile(nxlfile);
//		//				is_upload = nxlfile->IsUploadToRMS();
//		//				cout << "Message: " << res.GetMsg() << endl;*/
//		//				//Upload activity log
//		//				res = puser->UploadActivityLogs();
//		//				EXPECT_FALSE(res.GetCode());
//		//			}
//		//		}
//		//		SDWLibDeleteRmcInstance(pInstance);
//		//		cout << "-------------This case finished--------" << endl;
//
//		//	}
//
//		//}
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//
//}

//TEST(UserLargeFile, LargeFileTest) {
//	try {
//		SDR_Expiration expire;
//		expire.type = IExpiryType::NEVEREXPIRE;
//
//		string tag_text = "{}";
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		std::vector<SDRmFileRight> rights;
//		rights.push_back(RIGHT_VIEW);
//
//		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			wstring nxlfile_path;
//			res = puser->ProtectFile(plan_file_path + mp3normal_testfile, nxlfile_path, rights, watermark_info, expire, tag_text);
//			EXPECT_FALSE(res.GetCode());
//
//			if (res.GetCode() == 0) {
//				ISDRFiles* pFile = NULL;
//				std::wstring filename;
//				bool delete_result;
//				res = puser->GetLocalFileManager(&pFile);
//				size_t no = pFile->GetListNumber();
//				ISDRmNXLFile* nxlfile = pFile->GetFile(0);
//				if (nxlfile)
//				{
//					time_t timeBegin, timeEnd;
//					timeBegin = time(NULL);
//					timeEnd = time(NULL);
//					printf("%d\n", timeEnd - timeBegin);
//					//Upload file the first time
//					res = puser->UploadFile(nxlfile);
//					EXPECT_FALSE(res.GetCode());
//					timeEnd = time(NULL);
//					printf("%d\n", timeEnd - timeBegin);
//					if ((timeEnd - timeBegin) > 20) {
//						EXPECT_FALSE(true);
//						cout << "It takes a long time to upload file!" << endl;
//					}
//					//Isupload
//					bool is_upload = nxlfile->IsUploadToRMS();
//					EXPECT_TRUE(is_upload);
//
//					//Upload the file the second time
//					res = puser->UploadFile(nxlfile);
//					cout << "Message: " << res.GetMsg() << endl;
//					is_upload = nxlfile->IsUploadToRMS();
//					EXPECT_TRUE(is_upload);
//
//				}
//
//				cout << "Upload finished!" << endl;
//
//			}
//
//		}
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//
//}

TEST(UserTimeOut, TimeOut) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		string clientId;
		string pclientID = "";
		string  puser_return = plan_path + "User_Return.txt";
		ISDRmHttpRequest *prequest = NULL;

		//string email = "Scott.Xie@nextlabs.com";
		//string password = "Scott0707!";
		//md5(password);

		//bool login_result =Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password, "2");
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);

		createFolder("C:\\work");
		res = SDWLibCreateSDRmcInstance(L"C:\\Program Files\\NextLabs", L"C:\\work", &pInstance);
		res = pInstance->Initialize(DEFULAT_WORKING_FOLDER, DEFUALT_ROUTER, DEFUALT_TENANT);
		cout << "GetLoginRequest: ";
		res = pInstance->GetLoginRequest(&prequest);

		if (res.GetCode() == 0)
		{
			cout << "-----LoginRequest-----" << endl;
			for each (HttpCookie cookie in prequest->GetCookies())
			{
				wcout << "Cookie name:[" << cookie.first << "]";
				wcout << "  Vaule:[" << cookie.second << "]" << endl;

				if (ws2s(cookie.first) == "clientId")
				{
					clientId = ws2s(cookie.second);
				}
			}
			cout << "---------------------" << endl;
		}
		if (!res.GetCode()) {
			pInstance->Save();
			GetUserLoginResponseData(email, password, clientId, puser_return);
			string user_return = ReadFromFile(puser_return);
			res = pInstance->SetLoginResult(user_return, &puser, secr);
			if (!res.GetCode()) {
				string ps_code = puser->GetPasscode();
				cout << "User passcode is :" + ps_code << endl;
				string exportfilename = plan_path + passcode_txt;
				ofstream SaveFile(exportfilename);
				SaveFile << ps_code;
				SaveFile.close();
				pInstance->Save();
				cout << "Save passcode Successful!" << endl;
			}
		}
		SDWLibDeleteRmcInstance(pInstance);

		ISDRmcInstance *pInstance_pre = NULL;
		ISDRmUser *puser_pre = NULL;
		string passcode_global = ReadFromFile(plan_path + passcode_txt);

		res = SDWLibCreateSDRmcInstance(L"C:\\Program Files\\NextLabs", L"C:\\work\\working", &pInstance);
		res = pInstance->Initialize(L"C:\\work\\working", DEFUALT_ROUTER, DEFUALT_TENANT);
		if (!res.GetCode()) {
			res = pInstance->GetLoginUser(email, passcode_global, &puser);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			wcout << L"Test Finished! " << endl;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Protect with specify membership
TEST(UserProtectWithMem, ProtectWithMem) {
	try {
		//SDR_Expiration expire;
		//expire.type = IExpiryType::RELATIVEEXPIRE;
		//expire.end = 1536422399999;
		//expire.start = 1536227926000;

		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		string tenant = ws2s(DEFUALT_TENANT);
		//string tag_text = "{}";			//"{\"Security Clearance\":[\"test\"],\"Classification\":[\"test\"]}";
		string tag_text = "{\"Security Clearance\":[\"test\"],\"Classification\":[\"test\"]}";			//"{\"Security Clearance\":[\"test\"],\"Classification\":[\"test\"]}";
		string membership = "";
		wstring w_name;
		wstring localfile_pathw = plan_file_path + bmpnormal_testfile;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		std::vector<SDRmFileRight> rights;
		//rights.push_back(RIGHT_VIEW);
		//rights.push_back(RIGHT_DOWNLOAD);

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
		string tanen_name;
		wstring nxlfile_path;

		//bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		if (login_result) {
			Json::Value jroot;
			jroot = ParseJsonFile(user_respond);
			string server = jroot["results"]["server"].asString();

			if (jroot["extra"]["memberships"].size() == 1) {
				cout << "No projects from this user!" << endl;
				return;
			}
			else {
				for (int i = 0; i < jroot["extra"]["memberships"].size(); i++) {
					if ((jroot["extra"]["memberships"][i]["type"].asInt()) == 1) {
						membership = jroot["extra"]["memberships"][i]["id"].asString();
						p_id = jroot["extra"]["memberships"][i]["projectId"].asInt();
						tanen_str = jroot["extra"]["memberships"][i]["tenantId"].asString();
						break;
					}
				}
				bool list_result = GetProjectListResponseData("1", "100", "", "");
				jroot = ParseJsonFile(projectlist_return);
				int total_projects = jroot["results"]["totalProjects"].asInt();
				for (int i = 0; i < total_projects; i++) {
					int id = jroot["results"]["detail"][i]["id"].asInt();
					if (id == 2) {
						tanen_name = jroot["results"]["detail"][i]["tokenGroupName"].asString();
						break;
					}
				}
			}

			//res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
			//std::vector<SDR_PROJECT_INFO> projects = puser->GetProjectsInfo();
			//for (int i = 0; i < projects.size(); i++) {
			//	if (projects[i].projid == p_id) {
			//		promem_id = projects[i]
			//	}
			//}
			string mem_id = puser->GetMembershipID(2);
			EXPECT_STREQ(mem_id.c_str(), membership.c_str());
			string mem_id2 = puser->GetMembershipID(tanen_name);
			EXPECT_STREQ(mem_id2.c_str(), membership.c_str());
			string mem_id3 = puser->GetMembershipIDByTenantName(tanen_name);
			EXPECT_STREQ(mem_id3.c_str(), membership.c_str());

			vector<SDR_CLASSIFICATION_CAT> categories;
			bool original = false;
			bool newly = false;
			res = puser->GetClassification(tanen_name, categories);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			res = puser->ProtectFile(plan_file_path + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire, tag_text, mem_id);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (!res.GetCode()) {
				//ISDRFiles* pFile = NULL;
				std::wstring filename;
				bool delete_result;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);

				res = puser->UploadProjectFile(2, L"/", nxlfile);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				file_name = nxlfile->GetFileName();
				wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
				std::transform(file_name.begin(), file_name.end(), file_name.begin(), ::tolower);
				res = puser->ProjectDownloadFile(2, "/" + ws2s(file_name), projectdownloaded_path, RD_Normal);
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
			}
			res = puser->LogoutUser();
			res = pInstance->RPMLogout();
		}
		else {
			cout << "Login Failed!" << endl;
			EXPECT_TRUE(login_result);
		}
		SDWLibDeleteRmcInstance(pInstance);


		ISDRmcInstance *pInstance_pre1 = NULL;
		ISDRmUser *puser_pre1 = NULL;
		string passcode_global = ReadFromFile(plan_path + passcode_txt);
		bool login_result1 = PreviousUser_Login(pInstance_pre1, puser_pre1, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, passcode_global);
		EXPECT_TRUE(login_result1);
		wstring email = puser->GetEmail();
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

TEST(UserProtectWithMem1, ProtectWithMem1) {
	try {
		SDWLResult res;
		ISDRmcInstance *pInstance_pre1 = NULL;
		ISDRmUser *puser_pre1 = NULL;
		string passcode_global = ReadFromFile(plan_path + passcode_txt);
		bool login_result1 = PreviousUser_Login(pInstance_pre1, puser_pre1, DEFUALT_ROUTER, DEFUALT_TENANT, email, passcode_global);
		res = puser_pre1->LogoutUser();
		res = pInstance_pre1->RPMLogout();
		EXPECT_TRUE(login_result1);



	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}



////Upload lots of activitylogs
//TEST(UserUploadLotsActivityLogTest, UploadLotsActivityLog) {
//	try {
//		SDR_Expiration expire;
//		expire.type = IExpiryType::NEVEREXPIRE;
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		uint64_t startPos = 0;
//		uint64_t count = 10;
//		uint8_t searchField = 0;
//		std::vector<SDRmFileRight> rights;
//		rights.push_back(RIGHT_VIEW);
//		rights.push_back(RIGHT_EDIT);
//		rights.push_back(RIGHT_SHARE);
//		wstring localfile_pathw = plan_file_path + L"How to update build on RMS.docx";
//
//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			for (int i = 0; i < 10; i++) {
//				puser->ProtectFile(localfile_pathw, rights, watermarkinfo, expire, tags);
//				ISDRFiles* pFile = NULL;
//				bool delete_result;
//				res = puser->GetLocalFileManager(&pFile);
//				ISDRmNXLFile* nxlfile = pFile->GetFile(i);
//				wstring filename = nxlfile->GetFileName();
//				//Decrypt file
//				SRMActivityLogOperation log = RLOView;
//				res = puser->DecryptNXLFile(nxlfile, decrypt_file_path + L"DecryptRights.docx", log);
//				res = puser->UpdateRecipients(nxlfile, { "test@123.com" }, {});
//				EXPECT_EQ(res.GetCode(), 0);
//				//Decrypt file
//				res = puser->DecryptNXLFile(nxlfile, decrypt_file_path + L"DecryptRights.docx", log);
//				Sleep(2000);
//			}
//			puser->LogoutUser();
//			SDWLibDeleteRmcInstance(pInstance);
//		}
//
//		ISDRmcInstance *pInstance_1 = NULL;
//		ISDRmTenant *pTenant_1 = NULL;
//		ISDRmUser *puser_1 = NULL;
//		login_result = Default_Login(pInstance_1, pTenant_1, puser_1, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			for (int i = 10; i > 0; i--) {
//				ISDRFiles* pFile = NULL;
//				bool delete_result;
//				res = puser_1->GetLocalFileManager(&pFile);
//				ISDRmNXLFile* nxlfile = pFile->GetFile(i - 1);
//				wstring filename = nxlfile->GetFileName();
//				res = puser_1->UpdateRecipients(nxlfile, { "hhhh@123.com" }, {});
//				EXPECT_EQ(res.GetCode(), 0);
//				//Decrypt file
//				SRMActivityLogOperation log = RLOView;
//				res = puser->DecryptNXLFile(nxlfile, decrypt_file_path + L"DecryptRights.docx", log);
//				EXPECT_EQ(res.GetCode(), 0);
//				res = puser_1->UploadFile(nxlfile);
//				EXPECT_EQ(res.GetCode(), 0);
//				res = puser_1->UploadActivityLogs();
//				EXPECT_EQ(res.GetCode(), 0);
//				//Remove file
//				delete_result = pFile->RemoveFile(nxlfile);
//				EXPECT_TRUE(delete_result);
//			}
//			SDWLibDeleteRmcInstance(pInstance_1);
//		}
//
//		ISDRmcInstance *pInstance_pre = NULL;
//		ISDRmUser *puser_pre = NULL;
//		string passcode_global = ReadFromFile(plan_path + "passcode.txt");
//		login_result = PreviousUser_Login(pInstance_pre, puser_pre, DEFUALT_ROUTER, DEFUALT_TENANT, email, passcode_global);
//		if (login_result) {
//			res = puser_pre->UploadActivityLogs();
//			EXPECT_EQ(res.GetCode(), 0);
//		}
//
//		SDWLibDeleteRmcInstance(pInstance_pre);
//		cout << "-------------This case finished--------" << endl;
//
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

//class PerformanceTest :public::testing::TestWithParam<vector<string>> {};
//TEST_P(PerformanceTest, TestPerformance) {
//	try {
//		vector<string> csv = GetParam();
//		string file_name = csv[3];
//		string file_path = csv[2];
//		wstring file_pathw = s2ws(file_path);
//		wstring file_namew = s2ws(file_name);
//		SDR_Expiration expire;
//		expire.type = IExpiryType::NEVEREXPIRE;
//		string tag_text = "{\"EAR\":[\"EAR - 01\"]}";
//
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		std::vector<SDRmFileRight> rights;
//		rights.push_back(RIGHT_VIEW);
//		rights.push_back(RIGHT_DOWNLOAD);
//
//		SDR_WATERMARK_INFO watermark_info = { "$(User)$(Break)$(Date) $(Time)","font","color",9,0,ANTICLOCKWISE,true };
//		string exportfilename = plan_path + "Performance_result.csv";
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		long total = 0;
//		long total_dec = 0;
//		if (login_result) {
//			cout << "File name:  " + file_name << endl;
//			ofstream SaveFile(exportfilename, ios::app);
//			SaveFile << file_name << ",";
//			wstring nxlfile_path;
//
//			for (int i = 0; i < 10; i++) {
//				cout << "Protect takes: " << endl;
//				DWORD start = ::GetTickCount();
//				res = puser->ProtectFile(file_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
//				if (!res.GetCode()) {
//					ISDRFiles* pFile = NULL;
//					long mill_s = ::GetTickCount() - start;
//					cout << mill_s << endl;
//					total = total + mill_s;
//					SaveFile << mill_s << ",";
//				}
//				else {
//					cout << res.GetMsg() << endl;
//					SaveFile << res.GetMsg() << ",";
//				}
//				Sleep(1000);
//			}
//
//			long avg = total / 10;
//			cout << "===============AVG==================" << endl;
//			cout << avg << endl;
//			SaveFile << avg << endl;
//
//			cout << "Decrypt file takes:  " << endl;
//			ISDRFiles* pFile = NULL;
//			res = puser->GetLocalFileManager(&pFile);
//			ISDRmNXLFile* nxlfile = pFile->GetFile(0);
//			EXPECT_FALSE(res.GetCode());
//			//Decrypt file
//			SRMActivityLogOperation log = RLOView;
//			SaveFile << "Decrypt:  " << ",";
//			for (int i = 0; i < 10; i++) {
//				DWORD start1 = ::GetTickCount();
//				res = puser->DecryptNXLFile(nxlfile, decrypt_file_path + file_namew, log);
//				if (!res.GetCode()) {
//					long mill_s = ::GetTickCount() - start1;
//
//					cout << mill_s << endl;
//					SaveFile << mill_s << ",";
//					total_dec = total_dec + mill_s;
//				}
//				else {
//					cout << res.GetMsg() << endl;
//					SaveFile << res.GetMsg() << ",";
//				}
//			}
//
//
//			long avg_dec = total_dec / 10;
//			cout << "===============AVG=DEC=================" << endl;
//			cout << avg_dec << endl;
//			SaveFile << avg_dec << endl;
//			SaveFile.close();
//
//			for (int i = 0; i < 10; i++) {
//				ISDRmNXLFile* nxlfile = pFile->GetFile(0);
//				bool delete_result = pFile->RemoveFile(nxlfile);
//				cout << "Delete file !" << endl;
//			}
//
//
//			cout << "Test finish!" << endl;
//		}
//		puser->LogoutUser();
//		SDWLibDeleteRmcInstance(pInstance);
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}
//INSTANTIATE_TEST_CASE_P(PerformanceResultTesting, PerformanceTest, testing::ValuesIn(csv_performence));


////Upload lots of activitylogs
//TEST(UserUploadLotsActivityLogTest, UploadLotsActivityLog) {
//	try {
//		SDR_Expiration expire;
//		expire.type = IExpiryType::NEVEREXPIRE;
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		uint64_t startPos = 0;
//		uint64_t count = 10;
//		uint8_t searchField = 0;
//		std::vector<SDRmFileRight> rights;
//		rights.push_back(RIGHT_VIEW);
//		rights.push_back(RIGHT_EDIT);
//		rights.push_back(RIGHT_SHARE);
//		wstring localfile_pathw = plan_file_path + L"How to update build on RMS.docx";
//
//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			wstring nxlfile_path;
//			puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermarkinfo, expire, tags);
//			ISDRFiles* pFile = NULL;
//			bool delete_result;
//			res = puser->GetLocalFileManager(&pFile);
//			ISDRmNXLFile* nxlfile = pFile->GetFile(0);
//			wstring filename = nxlfile->GetFileName();
//			res = puser->UpdateRecipients(nxlfile, { "test@123.com" }, {});
//			EXPECT_FALSE(res.GetCode());
//
//			puser->LogoutUser();
//			SDWLibDeleteRmcInstance(pInstance);
//		}
//
//		ISDRmcInstance *pInstance_1 = NULL;
//		ISDRmTenant *pTenant_1 = NULL;
//		ISDRmUser *puser_1 = NULL;
//		login_result = Default_Login(pInstance_1, pTenant_1, puser_1, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			ISDRFiles* pFile = NULL;
//			bool delete_result;
//			res = puser_1->GetLocalFileManager(&pFile);
//			ISDRmNXLFile* nxlfile = pFile->GetFile(0);
//			wstring filename = nxlfile->GetFileName();
//			bool result = nxlfile->IsUploadToRMS();
//			res = puser_1->UpdateRecipients(nxlfile, { "hhhh@123.com" }, {});
//			EXPECT_FALSE(res.GetCode());
//
//			res = puser_1->UploadFile(nxlfile);
//			EXPECT_EQ(res.GetCode(), 0);
//			res = puser_1->UploadActivityLogs();
//			EXPECT_EQ(res.GetCode(), 0);
//			//Remove file
//			delete_result = pFile->RemoveFile(nxlfile);
//			EXPECT_TRUE(delete_result);
//			SDWLibDeleteRmcInstance(pInstance_1);
//		}
//
//		cout << "-------------This case finished--------" << endl;
//
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

////Upload log from login again
//TEST(UserUploadLogFromNextLogin, UploadLogFromLogin) {
//	try {
//		SDR_Expiration expire;
//		expire.type = IExpiryType::NEVEREXPIRE;
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		uint64_t startPos = 0;
//		uint64_t count = 10;
//		uint8_t searchField = 0;
//		std::vector<SDRmFileRight> rights;
//		rights.push_back(RIGHT_VIEW);
//		rights.push_back(RIGHT_EDIT);
//		rights.push_back(RIGHT_SHARE);
//		wstring localfile_pathw = plan_file_path + docnormal_testfile;
//
//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			wstring nxlfile_path;
//			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermarkinfo, expire, tags);
//			pInstance->Save();
//			if (!res.GetCode()) {
//				ISDRFiles* pFile = NULL;
//				res = puser->GetLocalFileManager(&pFile);
//				ISDRmNXLFile* nxlfile = pFile->GetFile(0);
//				cout << "Protected file!" << endl;
//			}
//			else {
//				cout << "Protect file failed!" << endl;
//				EXPECT_EQ(res.GetCode(), 0);
//				return;
//			}
//			pInstance->Save();
//			res = puser->LogoutUser();
//			SDWLibDeleteRmcInstance(pInstance);
//		}
//
//		ISDRmcInstance *pInstance_1 = NULL;
//		ISDRmTenant *pTenant_1 = NULL;
//		ISDRmUser *puser_1 = NULL;
//		login_result = Default_Login(pInstance_1, pTenant_1, puser_1, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//
//			ISDRFiles* pFile = NULL;
//			bool delete_result;
//			res = puser_1->GetLocalFileManager(&pFile);
//			ISDRmNXLFile* nxlfile = pFile->GetFile(0);
//			wstring filename = nxlfile->GetFileName();
//			res = puser_1->UploadFile(nxlfile);
//
//			if (!res.GetCode()) {
//				res = puser_1->UploadActivityLogs();
//				if (!res.GetCode()) {
//					bool log_result = GetActivityLogResponseData(ws2s(filename), "0", "100", "accessTime", "true");
//					Json::Value jroot;
//					jroot = ParseJsonFile(activity_return);
//					int log_count = jroot["results"]["data"]["logRecords"].size();
//					std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//					SDR_FILE_ACTIVITY_INFO ac_info;
//					//Get nxl file activity log
//					res = puser_1->GetNXLFileActivitylog(nxlfile, 0, 20, 0, "", 0, false);
//					if (!res.GetCode()) {
//						res = puser_1->GetActivityInfo(filename, infoVec);
//						if (res.GetCode()) {
//							cout << res.GetCode() << endl;
//							cout << res.GetMsg() << endl;
//							EXPECT_FALSE(res.GetCode());
//						}
//						else {
//							EXPECT_EQ(infoVec.size(), log_count);
//							EXPECT_EQ(infoVec.size(), 1);
//						}
//					}
//					else {
//						cout << "GetNXLFileActivitylog failed" << endl;
//						EXPECT_EQ(res.GetCode(), 0);
//					}
//				}
//				else {
//					cout << "Upload Activity logs failed!" << endl;
//					EXPECT_EQ(res.GetCode(), 0);
//				}
//			}
//			else {
//				cout << "Upload file failed!" << endl;
//				EXPECT_EQ(res.GetCode(), 0);
//			}
//
//			//Remove file
//			delete_result = pFile->RemoveFile(nxlfile);
//			EXPECT_TRUE(delete_result);
//
//			SDWLibDeleteRmcInstance(pInstance_1);
//			cout << "Test finished!" << endl;
//		}
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

////Open file
//TEST(UserOpenFileTest, OpenFile) {
//	try {
//		SDR_Expiration expire;
//		expire.type = IExpiryType::RANGEEXPIRE;
//		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000 + 604800000;
//		expire.start = (uint64_t)std::time(nullptr) * 1000 + 604800000;
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//
//		RM_ProjectFilter filter = RF_All;
//		int base_code = 61440;
//		wstring localfile_pathw = plan_file_path + opennormal_testfile;
//
//		std::vector<SDRmFileRight> rights;
//		rights.push_back(RIGHT_VIEW);
//		rights.push_back(RIGHT_EDIT);
//		rights.push_back(RIGHT_SHARE);
//		rights.push_back(RIGHT_SEND);
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, L"https://rmtest.nextlabs.solutions", L"skydrm.com", email, password);
//		if (login_result) {
//			wstring nxlfile_path;
//			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermarkinfo, expire, tags);
//			ISDRFiles* pFile = NULL;
//			bool delete_result;
//			res = puser->GetLocalFileManager(&pFile);
//			std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
//			ISDRmNXLFile * nxlfile;
//			ISDRmNXLFile* nxlfile_a = NULL;
//			ISDRmNXLFile* nxlfile_b = NULL;
//			ISDRmNXLFile* nxlfile_c = NULL;
//			ISDRmNXLFile* nxlfile_local = NULL;
//			ISDRmNXLFile* nxlfile_invalid = NULL;
//			ISDRmNXLFile* nxlfile_invalid_notexist = NULL;
//			ISDRmNXLFile* nxlfile_invalid_invalidpath = NULL;
//			ISDRmNXLFile* nxlfile_invalid_invalidnxl = NULL;
//
//			//Open a local protect file
//			ISDRmNXLFile* nxlfile_local_protect = pFile->GetFile(0);
//			wstring filename = nxlfile_local_protect->GetFileName();
//
//			filename = nxlfile_local_protect->GetFileName();
//			uint64_t size = nxlfile_local_protect->GetFileSize();
//			vector<SDRmFileRight> rights_v = nxlfile_local_protect->GetRights();
//			bool is_upload = nxlfile_local_protect->IsUploadToRMS();
//			bool is_valid = nxlfile_local_protect->IsValidNXL();
//			//Remove file
//			delete_result = pFile->RemoveFile(nxlfile_local_protect);
//			EXPECT_TRUE(delete_result);
//
//			if (nxlfile_local_protect == NULL) {
//				cout << "Open local file failed!" << endl;
//				EXPECT_TRUE(0);
//			}
//			wstring filea_name = open_usera_testfile;
//			string filea_duid;
//			int rights_size;
//			//As User A to Open a server file
//			//bool filea_inforesult = GetFileInfoResponseData(ws2s(filea_name));
//			bool usera_file = GetMyVaultFileListResponseData("1", "10", ws2s(filea_name), "-creationTime", "allFiles");
//			if (usera_file) {
//				Json::Value jroot;
//				jroot = ParseJsonFile(myvaultfilelist_return);
//				string uuu = ReadFromFile(myvaultfilelist_return);
//				rights_size = jroot["results"]["detail"]["files"][0]["rights"].size();
//				filea_duid = jroot["results"]["detail"]["files"][0]["duid"].asString();
//
//			}
//			res = puser->OpenFile(server_file_path + filea_name, &nxlfile_a);
//			EXPECT_FALSE(res.GetCode());
//			wstring filename1 = nxlfile_a->GetFileName();
//			uint64_t size1 = nxlfile_a->GetFileSize();
//			if (!res.GetCode()) {
//				vector<SDRmFileRight> rights_v1 = nxlfile_a->GetRights();
//				EXPECT_EQ(rights_v1.size(), rights_size);
//				bool is_upload1 = nxlfile_a->IsUploadToRMS();
//				bool is_valid1 = nxlfile_a->IsValidNXL();
//				SDR_Expiration exp = nxlfile_a->GetExpiration();
//				uint64_t end = exp.end;
//				uint64_t start = exp.start;
//				if (nxlfile_a == NULL) {
//					cout << "Open SERVER file failed!" << endl;
//					EXPECT_TRUE(0);
//				}
//				else {
//					res = puser->CloseFile(nxlfile_a);
//					EXPECT_FALSE(res.GetCode());
//				}
//			}
//
//			//File not exist
//			res = puser->OpenFile(server_file_path + L"1RMC8.3 Spin Case-2018-06-25-06-58-07.xlsx.nxl", &nxlfile_invalid_notexist);
//			EXPECT_TRUE(res.GetCode());
//			EXPECT_STREQ("File not found", res.GetMsg().c_str());
//
//			//As User C to open server file
//			res = puser->OpenFile(server_file_path + open_userc_testfile, &nxlfile_c);
//			EXPECT_EQ(res.GetCode(), 61843);
//			EXPECT_STREQ("Unauthorized", res.GetMsg().c_str());
//
//			//Invalid file path
//			res = puser->OpenFile(L"C:\\notExistPath\\New Text Document-2018-06-13-07-41-33.txt.nxl", &nxlfile_invalid_invalidpath);
//			int nerror = GetLastError();
//			EXPECT_TRUE(res.GetCode());
//			EXPECT_STREQ("File not found", res.GetMsg().c_str());
//			//Invalid nxl file
//			res = puser->OpenFile(plan_pathw + L"Invalid NXL File.txt.nxl", &nxlfile_invalid_invalidnxl);
//			EXPECT_TRUE(res.GetCode());
//			EXPECT_STREQ("Invalid nxl file", res.GetMsg().c_str());
//		}
//		else {
//			EXPECT_FALSE(res.GetCode());
//			cout << "User login failed!" << endl;
//		}
//		SDWLibDeleteRmcInstance(pInstance);
//		cout << "Test Open file finished!" << endl;
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

////File owner open expired file
//TEST(UserOpenExpiredFileTest, OpenExpiredFile) {
//	try {
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		ISDRmUser *puser_b = NULL;
//		wstring file_path = server_file_path + expire_testfile;
//		wstring file_path_nxlfile_revoke = server_file_path + expirerevoke_testfile;
//		wstring file_path_delete = server_file_path + expiredelete_testfile;
//
//		//User A
//		bool login_result = Default_Login(pInstance, pTenant, puser, L"https://autorms-centos7301.qapf1.qalab01.nextlabs.com:8443", L"bf1a2cc2-83cc-4163-b70d-43bcec44acd8", email, password);
//		if (login_result) {
//			//Open expired file
//			ISDRmNXLFile * nxlfile;
//			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
//			res = puser->OpenFile(file_path, &nxlfile);
//			res = puser->GetRights(file_path, rightsAndWatermarks);
//			EXPECT_EQ(rightsAndWatermarks.size(), 10);
//
//			//Open revoked file
//			bool revoke;
//			ISDRmNXLFile * nxlfile_revoke;
//			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_file_path_nxlfile_revoke;
//			res = puser->OpenFile(file_path_nxlfile_revoke, &nxlfile_revoke);
//			res = puser->GetRights(file_path_nxlfile_revoke, rightsAndWatermarks_file_path_nxlfile_revoke);
//			res = puser->IsFileRevoked(file_path_nxlfile_revoke, revoke);
//			EXPECT_EQ(rightsAndWatermarks_file_path_nxlfile_revoke.size(), 10);
//
//			//Open deleted file
//			ISDRmNXLFile * nxlfile_delete;
//			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_file_path_delete;
//			res = puser->OpenFile(file_path_delete, &nxlfile_delete);
//			res = puser->GetRights(file_path_delete, rightsAndWatermarks_file_path_delete);
//			EXPECT_EQ(rightsAndWatermarks_file_path_delete.size(), 10);
//
//			cout << "Test finished!" << endl;
//		}
//		else
//		{
//			EXPECT_FALSE(res.GetCode());
//			cout << "User login failed!" << endl;
//		}
//
//		//User B
//		login_result = Default_Login(pInstance, pTenant, puser_b, L"https://autorms-centos7301.qapf1.qalab01.nextlabs.com:8443", L"bf1a2cc2-83cc-4163-b70d-43bcec44acd8", email_zhu, password);
//		if (login_result)
//		{
//			//Open expired file
//			ISDRmNXLFile * nxlfile;
//			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
//			res = puser_b->OpenFile(file_path, &nxlfile);
//			res = puser_b->GetRights(file_path, rightsAndWatermarks);
//			EXPECT_EQ(rightsAndWatermarks.size(), 0);
//
//			//Open revoked file
//			bool revoke;
//			ISDRmNXLFile * nxlfile_revoke;
//			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_file_path_nxlfile_revoke;
//			res = puser_b->OpenFile(file_path_nxlfile_revoke, &nxlfile_revoke);
//			res = puser_b->GetRights(file_path_nxlfile_revoke, rightsAndWatermarks_file_path_nxlfile_revoke);
//			res = puser_b->IsFileRevoked(file_path_nxlfile_revoke, revoke);
//			EXPECT_EQ(rightsAndWatermarks_file_path_nxlfile_revoke.size(), 0);
//
//			//Open deleted file
//			ISDRmNXLFile * nxlfile_delete;
//			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_file_path_delete;
//			res = puser_b->OpenFile(file_path_delete, &nxlfile_delete);
//			res = puser_b->GetRights(file_path_delete, rightsAndWatermarks_file_path_delete);
//			EXPECT_EQ(rightsAndWatermarks_file_path_delete.size(), 0);
//
//			cout << "Test finished!" << endl;
//		}
//		else
//		{
//			EXPECT_FALSE(res.GetCode());
//			cout << "User login failed!" << endl;
//		}
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

////User B Open file
//class UserBOpenFileTest :public::testing::TestWithParam<vector<string>> {};
//TEST_P(UserBOpenFileTest, OpenFileTest) {
//	try {
//		vector<string> csv = GetParam();
//		string rights_str = csv[2];
//		string file_path = csv[4];
//		string file_name = csv[3];
//		string local_path = file_path + file_name;
//		string email_csv = csv[5];
//		string password_csv = csv[6];
//		wstring local_path_w = s2ws(local_path);
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//
//		std::vector<SDRmFileRight> rights;
//		rights = Tick_Rights(rights_str);
//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//		SDR_FILE_ACTIVITY_INFO ac_info;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, L"https://rmtest.nextlabs.solutions", L"skydrm.com", email_csv, password_csv);
//		EXPECT_TRUE(login_result);
//		if (login_result) {
//			ISDRmNXLFile* nxlfile = NULL;
//			res = puser->OpenFile(local_path_w, &nxlfile);
//			//res = puser->OpenFile(L"C:\\SDK test data\\Server File\\UserB\\small-2018-10-15-02-43-18.txt.nxl", &nxlfile);
//			bool open_result = nxlfile->IsOpen();
//			vector<std::string> addrecipientsemail = { "123123@qq.com" };
//			vector<std::string> removerecipientsemail;
//			//res = puser->UpdateRecipients(nxlfile, addrecipientsemail, removerecipientsemail);
//			EXPECT_TRUE(open_result);
//			if (!res.GetCode()) {
//				wstring name = nxlfile->GetFileName();
//				uint64_t size = nxlfile->GetFileSize();
//				std::vector<SDRmFileRight> rights_nxl = nxlfile->GetRights();
//				bool right_result = rights_verified(rights, nxlfile);
//				EXPECT_TRUE(right_result);
//				bool is_upload = nxlfile->IsUploadToRMS();
//				EXPECT_TRUE(is_upload);
//				bool is_valid = nxlfile->IsValidNXL();
//				EXPECT_TRUE(is_valid);
//
//				//Get watermark
//				SDR_WATERMARK_INFO wm = nxlfile->GetWaterMark();
//				if (wm.text != "") {
//					EXPECT_STREQ(wm.text.c_str(), watermarkinfo.text.c_str());
//				}
//			}
//			else {
//				cout << "Open file failed!" << endl;
//				EXPECT_TRUE(0);
//			}
//		}
//		else {
//			cout << "User login failed!" << endl;
//			EXPECT_FALSE(1);
//		}
//		SDWLibDeleteRmcInstance(pInstance);
//
//		//Get previous user
//		ISDRmcInstance *pInstance_pre = NULL;
//		ISDRmUser *puser_pre = NULL;
//		string passcode_global = ReadFromFile(plan_path + passcode_txt);
//		login_result = PreviousUser_Login(pInstance_pre, puser_pre, L"https://rmtest.nextlabs.solutions", L"skydrm.com", email_csv, passcode_global);
//		EXPECT_TRUE(login_result);
//		if (login_result) {
//			ISDRmNXLFile* nxlfile = NULL;
//			res = puser_pre->OpenFile(local_path_w, &nxlfile);
//			bool open_result = nxlfile->IsOpen();
//			EXPECT_TRUE(open_result);
//			if (!res.GetCode()) {
//				wstring name = nxlfile->GetFileName();
//				uint64_t size = nxlfile->GetFileSize();
//				bool right_result = rights_verified(rights, nxlfile);
//				EXPECT_TRUE(right_result);
//				bool is_upload = nxlfile->IsUploadToRMS();
//				EXPECT_TRUE(is_upload);
//				bool is_valid = nxlfile->IsValidNXL();
//				EXPECT_TRUE(is_valid);
//
//				//Get watermark
//				SDR_WATERMARK_INFO wm = nxlfile->GetWaterMark();
//				if (wm.text != "") {
//					EXPECT_STREQ(wm.text.c_str(), watermarkinfo.text.c_str());
//				}
//
//				res = puser_pre->LogoutUser();
//			}
//			else {
//				cout << "Open file failed!" << endl;
//				EXPECT_TRUE(0);
//			}
//		}
//		else {
//			cout << "Get previous user failed" << endl;
//			EXPECT_TRUE(0);
//		}
//		EXPECT_FALSE(res.GetCode());
//		SDWLibDeleteRmcInstance(pInstance_pre);
//
//		//User login again
//		ISDRmcInstance *pInstance1 = NULL;
//		ISDRmUser *puser1 = NULL;
//		login_result = Default_Login(pInstance1, pTenant, puser1, L"https://rmtest.nextlabs.solutions", L"skydrm.com", email_csv, password_csv);
//		EXPECT_TRUE(login_result);
//		SDWLibDeleteRmcInstance(pInstance1);
//
//
//		//Get previous user
//		ISDRmcInstance *pInstance_pre1 = NULL;
//		ISDRmUser *puser_pre1 = NULL;
//		passcode_global = ReadFromFile(plan_path + passcode_txt);
//		login_result = PreviousUser_Login(pInstance_pre1, puser_pre1, L"https://rmtest.nextlabs.solutions", L"skydrm.com", email_csv, passcode_global);
//		EXPECT_TRUE(login_result);
//		res = puser_pre1->LogoutUser();
//		EXPECT_FALSE(res.GetCode());
//		SDWLibDeleteRmcInstance(pInstance_pre1);
//		cout << "Test Open file finished!" << endl;
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}
//INSTANTIATE_TEST_CASE_P(UserBOpenFileTesting, UserBOpenFileTest, testing::ValuesIn(csv_UserB_OpenFile));

//UpdateUserPreferenceQuery and GetUserPreference
TEST(UserUpdateAndGetUserPreferenceTest, UpdateAndGetUserPreference) {
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
			//L"$(User)\n$(Date) $(Time)";

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
			//Watermark
			res = puser->GetUserPreference(option, get_start, get_end, watermark);

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

//Get Login User
TEST(UserGetPreviousUserTest, GetPrevousUserTest) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		string email = "3057318205@qq.com";
		string password = "a1409d7e150f67df4e1fbccafe2ec5aa";
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		EXPECT_TRUE(login_result);
		SDWLibDeleteRmcInstance(pInstance);

		//Get previous user
		ISDRmcInstance *pInstance_pre = NULL;
		ISDRmUser *puser_pre = NULL;
		string passcode_global = ReadFromFile(plan_path + passcode_txt);
		login_result = PreviousUser_Login(pInstance_pre, puser_pre, DEFUALT_ROUTER, DEFUALT_TENANT, email, passcode_global);
		EXPECT_TRUE(login_result);
		res = puser_pre->LogoutUser();
		EXPECT_FALSE(res.GetCode());
		SDWLibDeleteRmcInstance(pInstance_pre);

		//Login again 
		ISDRmcInstance *pInstance1 = NULL;
		ISDRmUser *puser1 = NULL;
		login_result = Default_Login(pInstance1, pTenant, puser1, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		EXPECT_TRUE(login_result);
		SDWLibDeleteRmcInstance(pInstance1);

		//Get previous user
		ISDRmcInstance *pInstance_pre1 = NULL;
		ISDRmUser *puser_pre1 = NULL;
		passcode_global = ReadFromFile(plan_path + passcode_txt);
		bool login_result1 = PreviousUser_Login(pInstance_pre1, puser_pre1, DEFUALT_ROUTER, DEFUALT_TENANT, email, passcode_global);
		EXPECT_TRUE(login_result1);
		res = puser_pre1->LogoutUser();
		EXPECT_FALSE(res.GetCode());
		SDWLibDeleteRmcInstance(pInstance_pre1);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

////DecryptNXLFile
//TEST(UserDecryptNXLFileTest, DecryptNXLFileTest) {
//	try {
//		SDR_Expiration expire;
//		expire.type = IExpiryType::RELATIVEEXPIRE;
//		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;
//		expire.start = (uint64_t)std::time(nullptr) * 1000;
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		std::vector<std::string> recipientsemail = { "jewelry.zhu@nextlabs.com", "lifi.yuan@nextlabs.com" };
//		std::wstring  comments = L"Comments Test";
//
//		std::vector<SDRmFileRight> rights;
//		rights = Tick_Rights("view|edit");
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			ISDRFiles* pFile = NULL;
//			ISDRmNXLFile* nxlfile1 = NULL;
//			std::wstring filename;
//			bool delete_result;
//
//			//Decrypt local shared file
//			wstring nxlfile_path;
//			res = puser->ShareFile(plan_file_path + L"Protect_normal.txt", nxlfile_path, rights, recipientsemail, comments, watermarkinfo, expire, tags);
//			if (!res.GetCode()) {
//				res = puser->GetLocalFileManager(&pFile);
//				size_t no = pFile->GetListNumber();
//				ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
//				//Decrypt local protected file
//				SRMActivityLogOperation log = RLOView;
//				res = puser->DecryptNXLFile(nxlfile, decrypt_file_path + L"DecryptLocalSharedFile.txt", log);
//				EXPECT_FALSE(res.GetCode());
//				//Remove file
//				delete_result = pFile->RemoveFile(nxlfile);
//				EXPECT_TRUE(delete_result);
//			}
//			else {
//				cout << "Share local file failed!" << endl;
//				EXPECT_FALSE(res.GetCode());
//			}
//
//			//Decrypt local protected file
//			res = puser->ProtectFile(plan_file_path + L"Protect_normal.txt", nxlfile_path, rights, watermarkinfo, expire, tags);
//			if (!res.GetCode()) {
//				res = puser->GetLocalFileManager(&pFile);
//				size_t no = pFile->GetListNumber();
//				ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
//				//Decrypt local protected file
//				SRMActivityLogOperation log = RLOView;
//				res = puser->DecryptNXLFile(nxlfile, decrypt_file_path + L"DecryptLocalProtectedFile.txt", log);
//				EXPECT_FALSE(res.GetCode());
//				//Remove file
//				delete_result = pFile->RemoveFile(nxlfile);
//				EXPECT_TRUE(delete_result);
//			}
//			else {
//				cout << "Share local file failed!" << endl;
//				EXPECT_FALSE(res.GetCode());
//			}
//
//			//Decrypt server file, User A
//			ISDRmNXLFile* nxlfile_a = NULL;
//			res = puser->OpenFile(L"C:\\SDK test data\\Decrypt_Owner.txt.nxl", &nxlfile_a);
//			SRMActivityLogOperation log = RLOView;
//			res = puser->DecryptNXLFile(nxlfile_a, decrypt_file_path + L"Decrypt_Owner.txt", log);
//			EXPECT_FALSE(res.GetCode());
//
//			//Decrypt server file, User B
//			ISDRmNXLFile* nxlfile_b = NULL;
//			res = puser->OpenFile(L"C:\\SDK test data\\Decrypt_UserB.txt.nxl", &nxlfile_b);
//			res = puser->DecryptNXLFile(nxlfile_b, decrypt_file_path + L"Decrypt_UserB.txt", log);
//			EXPECT_FALSE(res.GetCode());
//
//			//Decrypt server file, User C
//			ISDRmNXLFile* nxlfile_c = NULL;
//			res = puser->OpenFile(L"C:\\SDK test data\\Decrypt_UserC.txt.nxl", &nxlfile_c);
//			EXPECT_STREQ(res.GetMsg().c_str(), "Unauthorized");
//
//		}
//		else {
//			EXPECT_FALSE(res.GetCode());
//			cout << "User login failed!" << endl;
//		}
//
//		SDWLibDeleteRmcInstance(pInstance);
//		cout << "-------------This case finished--------" << endl;
//
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

//TEST(UserDecryptExpiredFile, DecryptExpireFile) {
//	try {
//		SDR_Expiration expire;
//		expire.type = IExpiryType::RELATIVEEXPIRE;
//		expire.end = (uint64_t)std::time(nullptr) * 1000 - 604800000;
//		expire.start = (uint64_t)std::time(nullptr) * 1000 - 1209600000;
//
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		std::vector<std::string> recipientsemail = { "jewelry.zhu@nextlabs.com", "3079175375@qq.com" };
//		std::wstring  comments = L"Comments Test";
//
//		std::vector<SDRmFileRight> rights;
//		rights = Tick_Rights("view|edit");
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			//Decrypt local expired file, owner
//			ISDRFiles* pFile = NULL;
//			ISDRmNXLFile* nxlfile1 = NULL;
//			std::wstring filename;
//			bool delete_result;
//
//			//Decrypt local shared file
//			wstring nxlfile_path;
//			res = puser->ShareFile(plan_file_path + L"Protect_normal.txt", nxlfile_path, rights, recipientsemail, comments, watermarkinfo, expire, tags);
//			if (!res.GetCode()) {
//				res = puser->GetLocalFileManager(&pFile);
//				size_t no = pFile->GetListNumber();
//				ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
//				//Decrypt local protected file
//				SRMActivityLogOperation log = RLOView;
//				res = puser->DecryptNXLFile(nxlfile, decrypt_file_path + L"DecryptLocalExpireFile.txt", log);
//				EXPECT_FALSE(res.GetCode());
//
//				//Remove file
//				delete_result = pFile->RemoveFile(nxlfile);
//				EXPECT_TRUE(delete_result);
//			}
//			else {
//				cout << "Share local file failed!" << endl;
//				EXPECT_FALSE(res.GetCode());
//			}
//			SDWLibDeleteRmcInstance(pInstance);
//			cout << "-------------This case finished--------" << endl;
//		}
//
//		//Decrypt expired file, User B
//		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, "xiaozhuzhu0712@163.com", "a1409d7e150f67df4e1fbccafe2ec5aa");
//		if (login_result) {
//			ISDRmNXLFile* nxlfile = NULL;
//			res = puser->OpenFile(server_file_path + L"Expire\\ExpireTest-2018-09-12-03-25-23.txt.nxl", &nxlfile);
//			EXPECT_FALSE(res.GetCode());
//			SRMActivityLogOperation log = RLOView;
//			res = puser->DecryptNXLFile(nxlfile, decrypt_file_path + L"UserBDecrypt.txt", log);
//			EXPECT_EQ(res.GetCode(), 5);
//			EXPECT_STREQ(res.GetMsg().c_str(), "Unauthorized to decrypt file");
//			res = puser->LogoutUser();
//			EXPECT_FALSE(res.GetCode());
//		}
//		else {
//			cout << "Share local file failed!" << endl;
//			EXPECT_FALSE(res.GetCode());
//		}
//		SDWLibDeleteRmcInstance(pInstance);
//
//		//Decrypt expired range file, User B
//		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, "xiaozhuzhu0712@163.com", "a1409d7e150f67df4e1fbccafe2ec5aa");
//		if (login_result) {
//			ISDRmNXLFile* nxlfile = NULL;
//			res = puser->OpenFile(server_file_path + L"Expire\\setting-2018-09-14-08-35-11.docx.nxl", &nxlfile);
//			EXPECT_FALSE(res.GetCode());
//			SRMActivityLogOperation log = RLOView;
//			res = puser->DecryptNXLFile(nxlfile, decrypt_file_path + L"UserBDecrypt.txt", log);
//			EXPECT_EQ(res.GetCode(), 5);
//			EXPECT_STREQ(res.GetMsg().c_str(), "Unauthorized to decrypt file");
//			res = puser->LogoutUser();
//			EXPECT_FALSE(res.GetCode());
//		}
//		else {
//			cout << "Share local file failed!" << endl;
//			EXPECT_FALSE(res.GetCode());
//		}
//		SDWLibDeleteRmcInstance(pInstance);
//		cout << "-------------This case finished--------" << endl;
//
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

//Test Switch user
TEST(UserLogoutTest, LogoutTest) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result)
		{
			res = puser->LogoutUser();
			if (res.GetCode()) {
				cout << "User Logout failed!" << endl;
				EXPECT_FALSE(res.GetCode());
			}
			SDWLibDeleteRmcInstance(pInstance);
		}

		ISDRmcInstance *pInstance_b = NULL;
		ISDRmTenant *pTenant_b = NULL;
		ISDRmUser *puser_b = NULL;
		login_result = Default_Login(pInstance_b, pTenant_b, puser_b, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		if (!login_result) {
			cout << "User B login failed!" << endl;
			EXPECT_TRUE(login_result);
		}
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

//My Drive Info
TEST(UserMyDriveInfoTest, DriveInfoTest) {
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
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User Login failed!" << endl;
		}
		cout << "-------------This case finished--------" << endl;

		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
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

TEST(UserGetNXLFileInfoTest, GetNXLFileInfoTest) {
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

		string create_by = nxlfile1->Get_CreatedBy();
		uint64_t create_date = nxlfile1->Get_DateCreated();
		uint64_t modified_date = nxlfile1->Get_DateModified();
		string modified_by = nxlfile1->Get_ModifiedBy();
		string watermark_str = nxlfile1->GetAdhocWaterMarkString();
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Update User Info
TEST(UserUpdateInfoTest, UpdateInfoTest) {
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

//HeartBeat Test without policyBundle
TEST(UserHeartBeatTest, HeartBeatTest) {
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

TEST(UserAddActivityLogTest, AddActivityLogTest) {
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

		//searchField. can be empty. Supported values are email, operation, deviceId 
		//res = puser->GetNXLFileActivitylog(file1, 0, 100, 0, "", 0, true);
		//When file not found
		Sleep(3000000);
		res = puser->GetActivityInfo(filename, infoVec);
		res = puser->GetActivityInfo(L"zhuzhu.txt", infoVec);
		EXPECT_EQ(res.GetCode(), 1168);
		EXPECT_STREQ(res.GetMsg().c_str(), "File not found");
		res = puser->GetActivityInfo(filename, infoVec);
		EXPECT_EQ(infoVec.size(), 31);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

////TEST(UserLocalActivityLogTest, LocalActivityLogTest) {
//	try {
//		SDR_Expiration expire;
//		expire.type = IExpiryType::RELATIVEEXPIRE;
//		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;
//		expire.start = (uint64_t)std::time(nullptr) * 1000;
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		std::vector<std::string> recipientsemail = { "jewelry.zhu@nextlabs.com", "lifi.yuan@nextlabs.com" };
//		std::wstring  comments = L"Comments Test";
//
//		std::vector<SDRmFileRight> rights;
//		rights = Tick_Rights("view|edit");
//
//		// login
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			ISDRFiles* pFile = NULL;
//			ISDRmNXLFile* nxlfile1 = NULL;
//			std::wstring filename;
//			wstring nxlfile_path;
//
//			//res = puser->ProtectFile(plan_file_path + L"Protect_normal.txt", rights, watermarkinfo, expire, tags);
//			res = puser->ShareFile(plan_file_path + L"Protect_normal.txt", nxlfile_path, rights, recipientsemail, comments, watermarkinfo, expire, tags);
//			EXPECT_FALSE(res.GetCode());
//
//			// Record passCode, in order to next use.
//			const std::string recordPassCode = puser->GetPasscode();
//
//			// Delete instance
//			SDWLibDeleteRmcInstance(pInstance);
//
//			// Should not login again --- because of not logout, only DeleteRmcInstance(equals Skill the process!)
//			//bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//
//			// Create RmsInstance again(pInstance2).
//			ISDRmcInstance *pInstance2 = NULL;
//			cout << "SDWLibCreateSDRmcInstance: ";
//			res = SDWLibCreateSDRmcInstance("Product name", 1, 1, 1, L"C:\\work", L"C:\\work", &pInstance2); // Now the working folder has existed.
//			checkSDWLResult(res);
//			EXPECT_EQ(0, res.GetCode());
//
//			// Initialize Instance.
//			cout << "Initialize(router, tenant): ";
//			res = pInstance2->Initialize(L"C:\\work", DEFUALT_ROUTER, DEFUALT_TENANT);
//			checkSDWLResult(res);
//			EXPECT_EQ(0, res.GetCode());
//
//			// get login User
//			ISDRmUser* pUser;
//			// GetLoginUser内部已经判断User是否登陆，以及登陆是否过期！
//			res = pInstance2->GetLoginUser(email, recordPassCode, &pUser);
//			checkSDWLResult(res);
//			EXPECT_EQ(0, res.GetCode());
//
//			// Do uploading activity log
//			res = pUser->GetLocalFileManager(&pFile);
//			//std::vector<std::wstring> list = pFile->GetList();
//			size_t no = pFile->GetListNumber();
//			if (no) {
//				ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
//				wstring filename = nxlfile->GetFileName();
//				string tags_1 = nxlfile->GetTags();
//				SDR_WATERMARK_INFO wm1 = nxlfile->GetWaterMark();
//				vector<SDRmFileRight> rr = nxlfile->GetRights();
//				SDR_Expiration ex = nxlfile->GetExpiration();
//				res = pUser->UploadFile(nxlfile);
//				res = pUser->UploadActivityLogs();
//				SRMActivityLogOperation log = RLOView;
//				res = pUser->DecryptNXLFile(nxlfile, decrypt_file_path + L"DecryptLocalProtectedFile.txt", log);
//				EXPECT_FALSE(res.GetCode());
//
//				//Remove file
//				bool delete_result = pFile->RemoveFile(nxlfile);
//				EXPECT_TRUE(delete_result);
//			}
//
//			// release PInstace2
//			SDWLibDeleteRmcInstance(pInstance2);
//
//		}
//		else {
//			EXPECT_FALSE(res.GetCode());
//			cout << "User login failed!" << endl;
//		}
//
//		cout << "-------------This case finished--------" << endl;
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

////Offline case
//TEST(UserProtectFilewithAllTokenTest, AllToekn) {
//	try {
//		SDR_Expiration expire;
//		expire.type = IExpiryType::RELATIVEEXPIRE;
//		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;
//		expire.start = (uint64_t)std::time(nullptr) * 1000;
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		std::vector<std::string> recipientsemail = { "jewelry.zhu@nextlabs.com", "lifi.yuan@nextlabs.com" };
//		std::wstring  comments = L"Comments Test";
//
//		std::vector<SDRmFileRight> rights;
//		rights = Tick_Rights("view|edit");
//
//		// login
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			wstring nxlfile_path;
//			for (int i = 0; i < 50; i++)
//			{
//				cout << "The number is:" << i << endl;
//				res = puser->ShareFile(plan_file_path + L"Protect_normal.txt", nxlfile_path, rights, recipientsemail, comments, watermarkinfo, expire, tags);
//				EXPECT_FALSE(res.GetCode());
//				Sleep(1000);
//			}
//			for (int i = 0; i < 20; i++)
//			{
//				cout << "The number is:" << i << endl;
//				res = puser->ShareFile(plan_file_path + L"Protect_normal.txt", nxlfile_path, rights, recipientsemail, comments, watermarkinfo, expire, tags);
//				EXPECT_FALSE(res.GetCode());
//				Sleep(1000);
//			}
//			ISDRFiles* pFile = NULL;
//			ISDRmNXLFile* nxlfile1 = NULL;
//
//			res = puser->GetLocalFileManager(&pFile);
//			size_t no = pFile->GetListNumber();
//			while (no) {
//				ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
//				wstring filename = nxlfile->GetFileName();
//				res = puser->UploadFile(nxlfile);
//
//				//Remove file
//				bool delete_result = pFile->RemoveFile(nxlfile);
//				EXPECT_TRUE(delete_result);
//				res = puser->GetLocalFileManager(&pFile);
//				no = pFile->GetListNumber();
//			}
//			res = puser->UploadActivityLogs();
//			EXPECT_FALSE(res.GetCode());
//
//			// release PInstace2
//			SDWLibDeleteRmcInstance(pInstance);
//		}
//		else {
//			EXPECT_FALSE(res.GetCode());
//			cout << "User login failed!" << endl;
//		}
//
//		cout << "-------------This case finished--------" << endl;
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

//TEST(Python, Pythonlogin) {
//	//std::wstring clientID = L"fsdsdfsdf";
//	//std::wstring wcmd = L"python d:\\test\\rms-login.py -u osmond.ye@nextlabs.com -p 123blue! -o d:\\test\\loginresult.json -c ";
//	//wcmd += clientID;
//	//system(std::string(wcmd.begin(), wcmd.end()).c_str());
//
//	// jewelry.zhu@nextlabs.com     2017@next
//	std::string user = "jewelry.zhu@nextlabs.com";
//	std::string pass = "2017@next";
//
//
//	std::string r1 = GetLoginResult(user, pass, "fullid", "d:\\test\\rt.json");
//	std::string r2 = GetLoginResult(user, pass, "fullid");
//	std::string r3 = GetLoginResult(user, pass);
//
//	EXPECT_STREQ(r1.c_str(), r2.c_str());
//	EXPECT_STREQ(r3.c_str(), r2.c_str());
//}

TEST(FileStatusTest, FileStatus)
{
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	string testFolder = TestDirectory + "TrustApp_Central";
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
	string email = "3057318205@qq.com";
	string password = "2018@next";
	md5(password);
	unsigned int dirstatus;
	bool filestatus;
	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);

	res = pInstance->RPMGetFileStatus(L"C:\\SDK test\\RPM\\QQ\\65119239441DFDF6663F50F922ADE35E\\DebugDump-2018-12-20-08-42-11.txt", &dirstatus, &filestatus);
	res = pInstance->RPMGetFileStatus(L"C:\\SDK test\\RPM\\QQ\\65119239441DFDF6663F50F922ADE35E\\DebugDump-2018-12-20-08-42-11.txt", &dirstatus, &filestatus);


	//waitInstanceInitFinish(pInstance);
	bool finished = false;
	int i = 0;
	do {
		i++;
		Sleep(1000);
		res = pInstance->IsInitFinished(finished);
		cout << i << ": Wait RMCInstance Class finish initialization...";
		cout << " >res: ";
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (i == 20)
		{
			cout << i << "Cannot connect to PDPMan" << endl;
			break;
		}
	} while (!finished);


	cout << "111111" << endl;
	string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
	GetUserLoginResponseData(email, password, clientID, user_return);

	cout << "Will set login!" << endl;
	cout << "Set SDK LoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	if (res) {
		cout << "GetHeartBeatInfo: ";
		SDWLResult res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << "----------RPM------------" << endl;

		ret = pInstance->IsRPMDriverExist();
		cout << "IsRPMDriverExist: " << ret << endl;
		if (!ret)
		{
			cout << "Please double check if the RPM build is installed!" << endl;
		}
		wstring app0 = L"C:\\zhuzhu\\trustapp\\app0\\app0.exe";
		wstring app1 = L"C:\\zhuzhu\\trustapp\\app1\\app1.exe";
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";


		cout << "Will trust app!" << endl;
		res = pInstance->RPMRegisterApp(app0, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RPMRegisterApp(app1, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << endl << "---------RPM login-----------" << endl;

		//cout << "SetRPMClientId: ";
		//res = pInstance->SetRPMClientId(clientID);
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		cout << "SetRPMLoginResult: ";
		DWORD start = ::GetTickCount();
		res = pInstance->SetRPMLoginResult(ReadFromFile(user_return), secr);
		DWORD Etime = ::GetTickCount() - start;
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "  Evaluation time: " << Etime << " (ms)" << endl;


		//cout << "SetRPMPolicyBundle: ";
		//res = pInstance->SetRPMPolicyBundle();
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		cout << "SyncUserAttributes: ";
		res = pInstance->SyncUserAttributes();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

	}
	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}

TEST(UserGetTenantPreferenceTest, GetTenantPreferenceTest) {
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
			//res = puser->OpenFile(L"C:\\Users\\jzhu\\Downloads\\Protect_normal-2019-01-04-09-15-24.txt.nxl", &file);
			//res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\Protect_normal-2019-01-04-09-15-24.txt.nxl", rightsAndWatermarks, true);

			tenan_str = jroot["extra"]["tenantId"].asString();
			res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid);
			EXPECT_EQ(sysprojectid, 1);
			cout << heartbeat << endl;
			res = pInstance->Save();
			bool is_adhoc = puser->IsTenantAdhocEnabled();
			EXPECT_TRUE(is_adhoc);
			res = pInstance->Save();
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

TEST(TempUserSystemProjectTest, SystemProjectTest) {
	try {
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		wstring nxlfile_path;
		wstring cpnxlfile_path;

		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;

		//expire.type = RANGEEXPIRE;
		//expire.start = (uint64_t)std::time(nullptr) * 1000 + 604800000;
		//expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000 + 604800000;

		string tenant = ws2s(DEFUALT_TENANT);
		string tag_text = "{\"Security Clearance\":[\"test\"],\"Classification\":[\"test\"]}";
		string change_tag = "{\"allRights\":[\"allRights\"]}";
		wstring w_name;
		wstring localfile_pathw = plan_file_path + txtnormal_testfile;
		std::vector<SDRmFileRight> rights;
		//rights.push_back(RIGHT_VIEW);
		//rights.push_back(RIGHT_DOWNLOAD);

		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };

		bool ret = false;
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir7";
		string passcode = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

		//bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");
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
			string  mem_id2 = puser->GetMembershipIDByTenantName("hacentos7503skydrm");
			string tenant_id = puser->GetSystemProjectTenantId();
			string default_tid = puser->GetDefaultTenantId();

			vector<SDR_CLASSIFICATION_CAT> categories;
			res = puser->GetClassification("hacentos7503skydrm", categories);
			checkSDWLResult(res);

			res = puser->ProtectFile(plan_file_path + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire, tag_text, mem_id);
			//ISDRmNXLFile *nxlfile;
			//res = puser->OpenFile(nxlfile_path, &nxlfile);

			//File owner, adhoc file

			//	res = puser->GetRights(L"C:\\work\\working\\skydrm0015\\9\\Protect_normal-2019-05-31-09-25-11.txt.nxl", rightsAndWatermarks, false);
			//	cout << res.GetCode() << endl;
			//	cout << res.GetMsg() << endl;
			//	cout << rightsAndWatermarks.size() << endl;
			//	puser->LogoutUser();
			//}

			//login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
			//if (login_result) {
			//	bool adhoc;
			//	int heartbeat;
			//	int sysprojectid;
			//	string tenan_str;
			//	std::vector<SDRmFileRight> rights_cp;

			//	string sysprojecttenantid;

			//	vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_b;
			//	vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_cp;
			//	//Not file owner, adhoc file
			//	res = puser->GetRights(nxlfile_path, rightsAndWatermarks_b, false);
			//	res = puser->GetHeartBeatInfo();
			//	res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid, tenan_str);
			//	string mem_id = puser->GetMembershipID(sysprojectid);
			//	//Protect a cp file
			//	res = puser->ProtectFile(paln_file_ws + txtnormal_testfile, cpnxlfile_path, rights_cp, watermark_info, expire, tag_text, mem_id);

			//	//File owner, cp file
			//	vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
			//	res = puser->GetFileRightsFromCentralPolicies(cpnxlfile_path, rightsAndWatermarks);
			//	res = puser->GetRights(cpnxlfile_path, rightsAndWatermarks_cp, true);
			//	puser->LogoutUser();
			//}
			//login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
			//if (login_result) {
			//	res = puser->GetHeartBeatInfo();
			//	vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_cpb;
			//	res = puser->GetRights(cpnxlfile_path, rightsAndWatermarks_cpb, true);
			//	puser->LogoutUser();
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(UserDecryptPartialTest, DecryptPartialTest) {
	try {
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);

		//wstring  projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project";
		//res = puser->ProjectDownloadFile(2, "/cp/project_centralized-2019-06-17-06-14-49.txt.nxl", projectdownloaded_path, RD_ForOffline);
		//Sleep(3000);
		//projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project1";
		//res = puser->ProjectDownloadFile(2, "/cp/project_centralized-2019-06-17-06-14-49.txt.nxl", projectdownloaded_path, RD_ForViewer);


		//TXT
		//const std::string nxlFile("C:\\Users\\jzhu\\Downloads\\RPMSetTokenTest-2019-06-06-02-07-18.txt.nxl");
		//const std::string txtFile("C:\\Users\\jzhu\\Downloads\\PDdecode.txt");

		//DOCX
		const std::string nxlFile("C:\\Users\\jzhu\\Downloads\\foroffline-2019-06-17-06-14-49.txt.nxl");
		const std::string txtFile("C:\\Users\\jzhu\\Downloads\\123.txt");

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

		//long header_len = 16384;
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

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(UserDecryptPartialTest1, DecryptPartialTest1) {
	try {
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		ISDRmUser *puser = NULL;
		SDWLResult res;
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);

		wstring d_path = L"C:\\auto\\output\\SDK\\Project";

		//res = puser->ProjectDownloadFile(6, "/rpmsettokentest.txt.nxl", d_path, RD_ForOffline);

		const std::string nxlFile("C:\\Users\\jzhu\\Downloads\\UserResetSourcePathTest-2019-06-17-03-17-41.txt.nxl");
		//const std::string nxlFile(ws2s(d_path));
		const std::string txtFile("C:\\Users\\jzhu\\Downloads\\123.txt");

		//ISDRmNXLFile * file;
		//res = puser->OpenFile(L"C:\\Users\\jzhu\\Downloads\\IOS Automation env setup.docx.nxl", &file);

		//const std::string nxlFile("C:\\Users\\jzhu\\Downloads\\Jewelry-2019-06-06-07-36-37.docx.nxl");
		//const std::string txtFile("C:\\Users\\jzhu\\Downloads\\321.docx");
		std::fstream fp;
		unsigned char header[12288];
		unsigned char in[2050];
		long in_len = 2048;
		long offset = 0;
		unsigned char out[2048];
		long out_len = 2048;
		long header_len = 12288;
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

		long bytesToRead = 1024; // in_len;

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

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//TEST(UserUpdateNXLMetaDataTest, UpdateNXLMetaDataTest) {
//	try {
//		ISDRmcInstance * pInstance;
//		ISDRmTenant * pTenant;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		wstring file_path = L"C:\\Users\\jzhu\\Downloads\\header-2019-03-26-08-42-21.txt.nxl";
//		bool login_result = Default_Login(pInstance, pTenant, puser, L"https://autorms-rhel74.qapf1.qalab01.nextlabs.com:8443", L"", email, password);
//
//		if (login_result) {
//			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
//			res = puser->GetHeartBeatInfo();
//
//			//Test adhoc file
//			vector<SDRmFileRight> rights;
//			rights.push_back(RIGHT_VIEW);
//			SDR_WATERMARK_INFO watermarkinfo;
//			watermarkinfo.text = "zhuzhu";
//			SDR_Expiration expire;
//			expire.type = RANGEEXPIRE;
//			expire.start = (uint64_t)std::time(nullptr) * 1000 + 604800000;
//			expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000 + 604800000;
//			wstring output_path = L"C:\\auto\\output\\SDK\\Project";
//			//res = puser->ProjectDownloadFile(15, "/cp/ortest(305)tag2-2019-03-26-09-04-58.txt.nxl", output_path, RD_ForOffline);
//			res = puser->ProjectDownloadFile(15, "/adhoc-2019-03-27-01-21-20.txt.nxl", output_path, RD_ForOffline);
//			res = puser->GetRights(output_path, rightsAndWatermarks, false);
//			//const std::string tags = "\"All rights\":[\"All rights\"]";
//			bool has_admin = puser->HasAdminRights(output_path);
//			res = puser->ChangeRightsOfFile(output_path, rights, watermarkinfo, expire, "");
//			res = puser->UpdateNXLMetaData(output_path);
//			ISDRmNXLFile  *nxlfile;
//			res = puser->OpenFile(output_path, &nxlfile);
//			res = puser->UploadProjectFile(15, L"/", nxlfile);
//		}
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

TEST(UserResetSourcePathTest, ResetSourcePathTest) {
	try {
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		wstring nxlfile_path = L"";
		wstring nxlfile_sharepath = L"";
		SDR_Expiration expire;
		expire.type = IExpiryType::RELATIVEEXPIRE;
		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;
		expire.start = (uint64_t)std::time(nullptr) * 1000;
		wstring localfile_pathw = plan_file_path + bmpnormal_testfile;
		SDR_WATERMARK_INFO watermark_info = { "ZHUZHU","font","color",9,0,ANTICLOCKWISE,true };

		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_EDIT);

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		if (login_result) {
			//res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, "");
			////Open server file
			//ISDRmNXLFile* nxlfile = NULL;

			//res = puser->ResetSourcePath(nxlfile_path, L"Project: TimeTest\\ZZHUHZH.xlsx.nxl");
			//res = puser->OpenFile(nxlfile_path, &nxlfile);
			////Upload file
			//res = puser->UploadFile(nxlfile_path, nxlfile_path);
			//EXPECT_FALSE(res.GetCode());

			ISDRmNXLFile* nxlfile_share = NULL;
			res = puser->ProtectFile(localfile_pathw, nxlfile_sharepath, rights, watermark_info, expire, "");
			//res = puser->ResetSourcePath(nxlfile_sharepath, L"Project: TimTest\\ZZHUHZH.xlsx.nxl");
			wstring path_s;
			res = puser->OpenFile(nxlfile_sharepath, &nxlfile_share);
			wstring file_name = nxlfile_share->GetFileName();
			//res = puser->GetFilePath(file_name, path_s);

			//Upload file
			//res = puser->UploadFile(nxlfile_sharepath, nxlfile_sharepath, L"jewelry.zhu@nextlabs.com", L"Test comment");
			//EXPECT_FALSE(res.GetCode());
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//TEST(UserGetRightsTest, GetRightsTest) {
//	try {
//		ISDRmcInstance * pInstance;
//		ISDRmTenant * pTenant;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		wstring nxlfilepath = L"";
//		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
//		std::vector<SDRmFileRight> rights;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
//		if (login_result) {
//			puser->GetHeartBeatInfo();
//			/*	wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
//				res = puser->ProjectDownloadFile(8, "/expired/log-2019-04-02-07-41-53.txt.nxl", projectdownloaded_path, RD_ForOffline);
//				ISDRmNXLFile * nxlfile;
//				res = puser->OpenFile(projectdownloaded_path, &nxlfile);
//				vector<SDRmFileRight> file_rights = nxlfile->GetRights();*/
//
//				//User	
//				//my vault ad hoc
//			wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
//			res = puser->ProjectDownloadFile(8, "/cp/win10-2019-04-01-09-26-34.png.nxl", projectdownloaded_path, RD_ForOffline);
//			//ISDRmNXLFile * file;
//			res = puser->GetRights(projectdownloaded_path, rightsAndWatermarks, false);
//			//User B share with me
//			//res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\UserC-adhoc-2019-03-27-02-05-16.txt.nxl", rightsAndWatermarks, true);
//			//User C
//			//res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\ios-2019-03-28-06-05-56.docx.nxl", rightsAndWatermarks, true);
//			//Project adhoc
//			//res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\Test plan-2019-03-05-2019-03-28-06-11-45.docx.nxl", rightsAndWatermarks, true);
//			//Project adhoc file, User c
//			//res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\testFile_Txt-2019-03-28-06-13-59.txt.nxl", rightsAndWatermarks, true);
//			//Project cp file, file owner
//			//res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\UserC-adhoc-2019-03-28-07-04-21.txt.nxl", rightsAndWatermarks, true);
//			//Project cp file, not file owner
//			//res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\header-2019-03-28-06-20-34.txt.nxl", rightsAndWatermarks, true);
//
//			//res = puser->GetFileRightsFromCentralPolicies(L"C:\\Users\\jzhu\\Downloads\\UserC-adhoc-2019-03-28-07-04-21.txt.nxl", rightsAndWatermarks);
//			//std::vector<SDRmFileRight> rights;
//			//res = puser->GetMyVaultFileRights("91CBE0CE093F4CABB1A2C65C5BA26169", "/nxl_myvault_nxl/withoutsharerights-2019-03-27-03-31-21.txt.nxl", rights);
//
//			////Instance
//			//Not File owner, central policy
//			res = puser->GetProjectFileRights("49", "/cp/log-2019-04-03-01-48-40.txt.nxl", rights);
//			//res = puser->OpenFile();
//			//res = pInstance->RPMGetRights(const std::wstring& filepath, std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> &rightsAndWatermarks);
//			//res = pInstance -> RPMGetFileRights(const std::wstring &filepath, std::vector<SDRmFileRight>& filerights);
//
//			////NXL 
//			//ISDRmNXLFile*  file;
//			//res = puser->OpenFile(L"C:\\Users\\jzhu\\Downloads\\UserC-adhoc-2019-03-28-07-04-21.txt.nxl", &file);
//			//std::vector<SDRmFileRight> rights = file->GetRights();
//
//			//pass
//			//res = puser->GetProjectFileRights("8", "/how to create mac os vm in vmware player-2019-03-26-07-01-37.docx.nxl", rights);
//		}
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

//ChangeRightsOfProjectFile
TEST(UserChangeAdminRightsTest, ChangeAdminRightsTest1) {
	try {
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		string default_tenantid;
		string default_tenantname;
		string tenantAdmin_email = email;
		string projectAdmin_email = email_zhu;
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
			addProjectAdmin_result = AddRemoveProjectAdminResponseData(default_tenantid, true, email_zhu);
			//Make sure email_zhu not in tenant admin list
			addTenantAdmin_result = AddRemoveTenantAminResponseData(default_tenantname, false, email_zhu);
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User login failed!" << endl;
		}

		//Tenant admin not project owner
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
						bool is_owner = is_projectowner(jroot["extra"]["memberships"][i]["projectId"].asInt(), email_zhu);
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
			vector<SDR_CLASSIFICATION_CAT> categories;
			bool original = false;
			bool newly = false;
			res = puser->GetClassification(tokenGroupName, categories);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			//Make sure tag is valid
			for (int i = 0; i < categories.size(); i++) {
				if (original&&newly)
					continue;
				for (int j = 0; j < categories[i].labels.size(); j++) {
					rights_eval = PolicyEvaluationResponseData(membership, categories[i].name, categories[i].labels[j].name);
					if (rights_eval != 0) {
						if (!original) {
							tag_original = "{\"" + categories[i].name + "\":[\"" + categories[i].labels[j].name + "\"]}";
							original = true;
							continue;
						}
						else if (original&&newly) {
							break;
						}
						else {
							if (!newly) {
								tag_new = "{\"" + categories[i].name + "\":[\"" + categories[i].labels[j].name + "\"]}";
								newly = true;
								break;
							}

						}
					}
					else {
						continue;
					}
				}
			}

			if ((tag_original == "") || (tag_new == "")) {
				cout << "There is no enough tag to do change rights" << endl;
				return;
			}

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

				//Tenant admin try to change file rights
				res = puser->ChangeRightsOfProjectFile(projectdownloaded_path, p_id, ws2s(file_name), "/", rights, watermark_info, expire, tag_new);
				EXPECT_STREQ(res.GetMsg().c_str(), "no permission to change file rights");
				EXPECT_EQ(res.GetCode(), 5);

				//Make sure email_zhu is invited member
				is_member = is_projectmember(p_id, email_zhu);
				if (!is_member)
				{
					Project_InviteMemberResponseData(to_string(p_id), email_zhu);
				}
			}
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User login failed!" << endl;
		}

		//Project owner invited by others
		login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email_zhu, password);
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
		login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email_zhu, password);
		if (login_result) {
			Json::Value jroot;
			jroot = ParseJsonFile(user_respond);

			if (jroot["extra"]["memberships"].size() == 1) {
				cout << "No projects from this user!" << endl;
				return;
			}
			else {
				for (int i = 0; i < jroot["extra"]["memberships"].size(); i++) {
					bool is_owner = is_projectowner(jroot["extra"]["memberships"][i]["projectId"].asInt(), email_zhu);
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
			vector<SDR_CLASSIFICATION_CAT> categories;
			bool original = false;
			bool newly = false;
			res = puser->GetClassification(tokenGroupName, categories);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			//Make sure tag is valid
			for (int i = 0; i < categories.size(); i++) {
				if (original&&newly)
					continue;
				for (int j = 0; j < categories[i].labels.size(); j++) {
					rights_eval = PolicyEvaluationResponseData(membership, categories[i].name, categories[i].labels[j].name);
					if (rights_eval != 0) {
						if (!original) {
							tag_original = "{\"" + categories[i].name + "\":[\"" + categories[i].labels[j].name + "\"]}";
							original = true;
							continue;
						}
						else if (original&&newly) {
							break;
						}
						else {
							if (!newly) {
								tag_new = "{\"" + categories[i].name + "\":[\"" + categories[i].labels[j].name + "\"]}";
								newly = true;
								break;
							}
						}
					}
					else {
						continue;
					}
				}
			}

			if ((tag_original == "") || (tag_new == "")) {
				cout << "There is no enough tag to do change rights" << endl;
				return;
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
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User login failed!" << endl;
		}

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
TEST(UserChangeAdminRightsTest, ChangeAdminRightsTest2) {
	ISDRmcInstance * pInstance;
	ISDRmTenant * pTenant;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	string default_tenantid;
	string default_tenantname;
	string tenantAdmin_email = email;
	string projectAdmin_email = email_zhu;
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
			vector<SDR_CLASSIFICATION_CAT> categories;
			res = puser->GetClassification(default_tokengroup, categories);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			//Make sure tag is valid
			for (int i = 0; i < categories.size(); i++) {
				if (original&&newly)
					break;
				for (int j = 0; j < categories[i].labels.size(); j++) {
					rights_eval = PolicyEvaluationResponseData(mem_id, categories[i].name, categories[i].labels[j].name);
					if (rights_eval != 0) {
						if (!original) {
							tag_original = "{\"" + categories[i].name + "\":[\"" + categories[i].labels[j].name + "\"]}";
							original = true;
							continue;
						}
						else if (original&&newly) {
							break;
						}
						else {
							if (!newly) {
								tag_new = "{\"" + categories[i].name + "\":[\"" + categories[i].labels[j].name + "\"]}";
								newly = true;
								break;
							}
						}
					}
					else {
						continue;
					}
				}
			}

			res = puser->ProtectFile(plan_file_path + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire, tag_original, mem_id);
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
		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		if (login_result) {
			res = puser->ChangeRightsOfFile(copy_file, rights, watermarkinfo, expire, tag_original);
			EXPECT_EQ(5, res.GetCode());
			EXPECT_STREQ("no permission to change file rights", res.GetMsg().c_str());
		}
		else {
			EXPECT_TRUE(login_result);
			cout << "User login failed!" << endl;
		}

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

TEST(UserShortNameTest, ShortNameTest) {
	try {
		wstring safe_folder = L"C:\\SDK test\\RPM\\DirShortNameTest7";
		string pathid;
		wstring downloaded_p = L"C:\\auto\\output\\SDK\\Project\\";
		createFolder(ws2s(safe_folder));
		createFolder(ws2s(downloaded_p));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		bool running = true;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

		ISDRmcInstance * pInstance1;
		ISDRmTenant * pTenant1;
		ISDRmUser *puser1 = NULL;
		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		string apppath_shortname = GetFileShortName(apppath);
		string safe_folder_short = GetFileShortName(safe_folder);
		//RPMRegisterApp
		res = pInstance->RPMRegisterApp(s2ws(apppath_shortname), secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RPMNotifyRMXStatus(running, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		//RPMUnregisterApp
		res = pInstance->RPMUnregisterApp(s2ws(apppath_shortname), secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RPMIsAppRegistered
		bool is_register;
		res = pInstance->RPMIsAppRegistered(s2ws(apppath_shortname), &is_register);

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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

		//AddRPMDir
		res = pInstance->AddRPMDir(s2ws(safe_folder_short));
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//IsRPMFolder
		unsigned int dirstatus;
		wstring safe_path = s2ws(safe_folder_short);
		res = pInstance->IsRPMFolder(safe_path, &dirstatus);
		EXPECT_EQ(dirstatus, 0);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RemoveRPMDir
		res = pInstance->RemoveRPMDir(s2ws(safe_folder_short));
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->AddRPMDir(s2ws(safe_folder_short));
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->SetRPMServiceStop(true, secr);
#pragma endregion

#pragma region[Prepare a MYVAULT nxl file]
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;
		//wstring localfile_pathw = s2ws(GetFileShortName(plan_file_path + txtnormal_testfileb));
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
		//res = puser->GetLocalFileManager(&pFile);
		//size_t no = pFile->GetListNumber();
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();
			puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Test comments");
			//Remove file
			delete_result = pFile->RemoveFile(nxlfile);
			EXPECT_TRUE(delete_result);
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

		string downloaded_short = GetFileShortName(downloaded_p);
		res = pInstance->RPMEditCopyFile(s2ws(downloaded_short), safe_folder);
		EXPECT_FALSE(res.GetCode());

		string safe_folder_short1 = GetFileShortName(safe_folder);
		vector<SDRmFileRight> filerights;
		//RPMGetFileRights

		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks1;
		//RPMGetRights
		res = pInstance->RPMGetRights(s2ws(safe_folder_short1), rightsAndWatermarks1);

		unsigned int dirstatus1;
		bool filestatus1;
		//RPMGetFileStatus
		res = pInstance->RPMGetFileStatus(s2ws(safe_folder_short1), &dirstatus1, &filestatus1);
		EXPECT_EQ(2, dirstatus1);
		EXPECT_TRUE(filestatus1);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RPMEditSaveFile
		res = pInstance->RPMEditSaveFile(s2ws(safe_folder_short1), L"", false, 3);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RPMDeleteFile
		res = pInstance->RPMDeleteFile(s2ws(safe_folder_short1));
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->RemoveRPMDir(s2ws(safe_folder_short), 1);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(UserShortNormalNameTest, ShortNormalNameTest) {
	try {
		wstring safe_folder = L"C:\\SDK test\\RPM\\DirShortNameTest2";
		string notmal_name = "\\NormalNameTest";
		string pathid;
		wstring downloaded_p = L"C:\\auto\\output\\SDK\\ProjectNormalNameTest";
		wstring download_normal = L"\\NormalNameTest";
		createFolder(ws2s(safe_folder + s2ws(notmal_name)));
		createFolder(ws2s(downloaded_p));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		bool running = true;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

		ISDRmcInstance * pInstance1;
		ISDRmTenant * pTenant1;
		ISDRmUser *puser1 = NULL;
		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		wstring app_path_folder = apppath.substr(0, (apppath.length() - 25));
		string app_path_normal = "\\RMD-WindowsAPI-Gtest.exe";
		string app_path_short_s = GetFileShortName(app_path_folder);
		string apppath_shortname = app_path_short_s + app_path_normal;

		string safe_folder_short_t = GetFileShortName(safe_folder);
		string safe_folder_short = safe_folder_short_t + notmal_name;
		//RPMRegisterApp
		res = pInstance->RPMRegisterApp(s2ws(apppath_shortname), secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RPMNotifyRMXStatus(running, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		//RPMUnregisterApp
		res = pInstance->RPMUnregisterApp(s2ws(apppath_shortname), secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RPMIsAppRegistered
		bool is_register;
		res = pInstance->RPMIsAppRegistered(s2ws(apppath_shortname), &is_register);

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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

		//AddRPMDir
		res = pInstance->AddRPMDir(s2ws(safe_folder_short));
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//IsRPMFolder
		unsigned int dirstatus;
		wstring safe_path = s2ws(safe_folder_short);
		res = pInstance->IsRPMFolder(safe_path, &dirstatus);
		EXPECT_EQ(dirstatus, 0);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RemoveRPMDir
		res = pInstance->RemoveRPMDir(s2ws(safe_folder_short));
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->AddRPMDir(s2ws(safe_folder_short));
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->SetRPMServiceStop(true, secr);
#pragma endregion

#pragma region[Prepare a MYVAULT nxl file]
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		string plan_file_short = GetFileShortName(plan_file_path);
		wstring localfile_pathw = s2ws(plan_file_short) + txtnormal_testfileb;
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
		//res = puser->GetLocalFileManager(&pFile);
		//size_t no = pFile->GetListNumber();
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();
			puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Test comments");
			//Remove file
			delete_result = pFile->RemoveFile(nxlfile);
			EXPECT_TRUE(delete_result);
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
		string downloaded_short_p = GetFileShortName(downloaded_p);
		res = puser->MyVaultDownloadFile(pathid, downloaded_p, 2);

#pragma endregion

		safe_folder = safe_folder + s2ws(notmal_name);
		string downloaded_short = GetFileShortName(s2ws(downloaded_short_p)) + "\\" + ws2s(filename);
		res = pInstance->RPMEditCopyFile(s2ws(downloaded_short), safe_folder);
		EXPECT_FALSE(res.GetCode());

		string safe_folder_short_s = GetFileShortName(safe_folder.substr(0, 95)) + "\\" + ws2s(safe_folder.substr(96, 134));
		vector<SDRmFileRight> filerights;
		//RPMGetFileRights
		res = pInstance->RPMGetFileRights(s2ws(safe_folder_short_s), filerights);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks1;
		//RPMGetRights
		res = pInstance->RPMGetRights(s2ws(safe_folder_short_s), rightsAndWatermarks1);

		unsigned int dirstatus1;
		bool filestatus1;
		//RPMGetFileStatus
		res = pInstance->RPMGetFileStatus(s2ws(safe_folder_short_s), &dirstatus1, &filestatus1);
		EXPECT_EQ(2, dirstatus1);
		EXPECT_TRUE(filestatus1);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RPMEditSaveFile
		res = pInstance->RPMEditSaveFile(s2ws(safe_folder_short_s), L"", false, 3);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RPMDeleteFile
		res = pInstance->RPMDeleteFile(s2ws(safe_folder_short_s));
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->RemoveRPMDir(s2ws(safe_folder_short), 1);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(UserNormalNameTest, NormalNameTest) {
	try {
		wstring safe_folder = L"C:\\SDK test\\RPM\\DirShortNameTest10";
		string pathid;
		wstring downloaded_p = L"C:\\auto\\output\\SDK\\Project\\";
		createFolder(ws2s(safe_folder));
		createFolder(ws2s(downloaded_p));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		bool running = true;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

		ISDRmcInstance * pInstance1;
		ISDRmTenant * pTenant1;
		ISDRmUser *puser1 = NULL;
		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

		//bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, "john.tyler", "john.tyler", "2");
		//RPMRegisterApp
		res = pInstance->RPMRegisterApp(apppath, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RPMNotifyRMXStatus(running, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		////RPMUnregisterApp
		//res = pInstance->RPMUnregisterApp(apppath, secr);
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		//RPMIsAppRegistered
		bool is_register;
		res = pInstance->RPMIsAppRegistered(apppath, &is_register);

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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

		//AddRPMDir
		res = pInstance->AddRPMDir(safe_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//IsRPMFolder
		unsigned int dirstatus;
		res = pInstance->IsRPMFolder(safe_folder, &dirstatus);
		EXPECT_EQ(dirstatus, 0);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RemoveRPMDir
		res = pInstance->RemoveRPMDir(safe_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->AddRPMDir(safe_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->SetRPMServiceStop(true, secr);
#pragma endregion

		vector<SDRmFileRight> filerights1;
		//RPMGetFileRights

		//std::wstring tempFile = L"C:\\SDK test\\RPM\\DirShortNameTest10\\6-2019-04-28-08-00-17.txt.nxl";
		//LPCTSTR lpFileName = tempFile.c_str();
		//WIN32_FIND_DATA pNextInfo;
		//HANDLE h = FindFirstFile(lpFileName, &pNextInfo);
		//if (h != INVALID_HANDLE_VALUE)
		//{
		//	FindClose(h);
		//}


		res = pInstance->RPMGetFileRights(L"C:\\SDK test\\RPM\\DirShortNameTest10\\withoutsharerights-2019-04-28-09-02-17.txt", filerights1);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		//		checkSDWLResult(res);
		//		EXPECT_EQ(0, res.GetCode());
		//		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks2;
		//		//RPMGetRights
		//		//res = pInstance->RPMGetRights(L"C:\\SDK test\\RPM\\DirShortNameTest9\\Document-2019-04-25-03-33-24.txt", rightsAndWatermarks2);
		//
		//		int processid;
		//		cout << "Process id: ";
		//		cin >> processid;
		//		res = pInstance->RPMAddTrustedProcess(processid, secr);
		//
		cout << "RPMLogout: ";
		res = pInstance->RPMLogout();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << endl << "SDK user logout:";
		res = puser->LogoutUser();
		checkSDWLResult(res);

		SDWLibDeleteRmcInstance(pInstance);
		//
		//
		//
		//
		//
		//
		//		login_result = Default_Login(pInstance, pTenant, puser, L"https://rms-rhel74.qapf1.qalab01.nextlabs.com:8443", L"", "jewelry.zhu@nextlabs.com", password);
		//		//RPMRegisterApp
		//		res = pInstance->RPMRegisterApp(apppath, secr);
		//		checkSDWLResult(res);
		//		EXPECT_EQ(0, res.GetCode());
		//		res = pInstance->RPMNotifyRMXStatus(running, secr);
		//		checkSDWLResult(res);
		//		EXPECT_EQ(0, res.GetCode());
		//		//RPMUnregisterApp
		//		res = pInstance->RPMUnregisterApp(apppath, secr);
		//		checkSDWLResult(res);
		//		EXPECT_EQ(0, res.GetCode());
		//
		//		//RPMIsAppRegistered
		//		res = pInstance->RPMIsAppRegistered(apppath, &is_register);
		//
		//#pragma region [Prepare RPM environment]
		//		if (login_result) {
		//			cout << "GetHeartBeatInfo: ";
		//			SDWLResult res = puser->GetHeartBeatInfo();
		//			checkSDWLResult(res);
		//			EXPECT_EQ(0, res.GetCode());
		//		}
		//		cout << "----------RPM------------" << endl;
		//
		//		ret = pInstance->IsRPMDriverExist();
		//		cout << "IsRPMDriverExist: " << ret << endl;
		//		if (!ret)
		//		{
		//			cout << "Please double check if the RPM build is installed!" << endl;
		//		}
		//
		//		user_logindata = pInstance->GetLoginData();
		//
		//		cout << "SetRPMLoginResult: ";
		//		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		//		EXPECT_EQ(0, res.GetCode());
		//
		//		//AddRPMDir
		//		res = pInstance->AddRPMDir(safe_folder);
		//		checkSDWLResult(res);
		//		EXPECT_EQ(0, res.GetCode());
		//
		//		//IsRPMFolder
		//		
		//		res = pInstance->IsRPMFolder(safe_folder, &dirstatus);
		//		EXPECT_EQ(dirstatus, 0);
		//		checkSDWLResult(res);
		//		EXPECT_EQ(0, res.GetCode());
		//
		//		//RemoveRPMDir
		//		res = pInstance->RemoveRPMDir(safe_folder);
		//		checkSDWLResult(res);
		//		EXPECT_EQ(0, res.GetCode());
		//
		//		res = pInstance->AddRPMDir(safe_folder);
		//		checkSDWLResult(res);
		//		EXPECT_EQ(0, res.GetCode());
		//
		//		res = pInstance->SetRPMServiceStop(true, secr);
		//#pragma endregion
		//
		//		int processid2;
		//		cout << "Process id: ";
		//		cin >> processid2;
		//		res = pInstance->RPMAddTrustedProcess(processid2, secr);
		//
		//		//cout << "RPMLogout: ";
		//		//res = pInstance->RPMLogout();
		//		//checkSDWLResult(res);
		//		//EXPECT_EQ(0, res.GetCode());
		//
		//		//cout << endl << "SDK user logout:";
		//		/*res = puser->LogoutUser();
		//		checkSDWLResult(res);
		//
		//		SDWLibDeleteRmcInstance(pInstance);*/
		//
		//
		//
		//#pragma region[Prepare a MYVAULT nxl file]
		//		SDR_Expiration expire;
		//		expire.type = IExpiryType::NEVEREXPIRE;
		//		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;
		//
		//		std::vector<SDRmFileRight> rights;
		//		rights.push_back(RIGHT_VIEW);
		//		rights.push_back(RIGHT_EDIT);
		//
		//		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
		//		std::wstring filename;
		//		std::vector<std::string> recipients = { email_zhu };
		//		wstring nxlfile_path;
		//		res = puser->ShareFile(localfile_pathw, nxlfile_path, rights, recipients, L"Test comments", watermark_info, expire);
		//
		//		ISDRFiles* pFile = NULL;
		//		bool delete_result;
		//		res = puser->GetLocalFileManager(&pFile);
		//		size_t no = pFile->GetListNumber();
		//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
		//		if (no) {
		//			ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
		//			if (nxlfile)
		//			{
		//				filename = nxlfile->GetFileName();
		//				puser->UploadFile(nxlfile);
		//				//Remove file
		//				delete_result = pFile->RemoveFile(nxlfile);
		//				EXPECT_TRUE(delete_result);
		//			}
		//			else {
		//				cout << "No file generated!" << endl;
		//				EXPECT_FALSE(res.GetCode());
		//			}
		//		}
		//
		//		bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
		//		if (file_result) {
		//			Json::Value jroot;
		//			jroot = ParseJsonFile(myvaultfilelist_return);
		//			pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
		//		}
		//
		//		res = puser->MyVaultDownloadFile(pathid, downloaded_p, 2);
		//
		//#pragma endregion
		//
		//		res = pInstance->RPMEditCopyFile(downloaded_p, safe_folder);
		//		EXPECT_FALSE(res.GetCode());
		//
		//		vector<SDRmFileRight> filerights;
		//		//RPMGetFileRights
		//		res = pInstance->RPMGetFileRights(safe_folder, filerights);
		//		checkSDWLResult(res);
		//		EXPECT_EQ(0, res.GetCode());
		//		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks1;
		//		//RPMGetRights
		//		res = pInstance->RPMGetRights(safe_folder, rightsAndWatermarks1);
		//
		//		unsigned int dirstatus1;
		//		bool filestatus1;
		//		//RPMGetFileStatus
		//		string shot_name= GetFileShortName(safe_folder);
		//		res = pInstance->RPMGetFileStatus(L"C:\\SDK test\\RPM\\DirShortNameTest10\\UserC-adhoc-2019-04-23-07-05-25.txt", &dirstatus1, &filestatus1);
		//		EXPECT_EQ(2, dirstatus1);
		//		EXPECT_TRUE(filestatus1);
		//		checkSDWLResult(res);
		//		EXPECT_EQ(0, res.GetCode());
		//
		//		//RPMEditSaveFile
		//		res = pInstance->RPMEditSaveFile(safe_folder, L"", false, 3);
		//		checkSDWLResult(res);
		//		EXPECT_EQ(0, res.GetCode());
		//
		//		//RPMDeleteFile
		//		res = pInstance->RPMDeleteFile(safe_folder);
		//		checkSDWLResult(res);
		//		EXPECT_EQ(0, res.GetCode());

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(UserLongNameTest, LongNameTest) {
	try {
		//wstring safe_folder = L"C:/SDK test/RPM/Dir3/Sub1/zhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuz/";
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir3\\Sub1\\zhu\\zhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuh\\";
		string pathid;
		wstring downloaded_p = L"C:\\auto\\output\\SDK\\Project";
		createFolder(ws2s(safe_folder));
		createFolder(ws2s(downloaded_p));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		bool running = true;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

		ISDRmcInstance * pInstance1;
		ISDRmTenant * pTenant1;
		ISDRmUser *puser1 = NULL;
		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		//bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, "john.tyler", "john.tyler", "2");
		//RPMRegisterApp
		res = pInstance->RPMRegisterApp(apppath, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RPMNotifyRMXStatus(running, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		bool is_register;
		res = pInstance->RPMIsAppRegistered(apppath, &is_register);
		EXPECT_TRUE(is_register);

		////RPMUnregisterApp
		//res = pInstance->RPMUnregisterApp(apppath, secr);
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		////RPMIsAppRegistered
		//res = pInstance->RPMIsAppRegistered(apppath, &is_register);
		//EXPECT_FALSE(is_register);

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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

		//AddRPMDir
		res = pInstance->AddRPMDir(safe_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RemoveRPMDir
		res = pInstance->RemoveRPMDir(safe_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->AddRPMDir(safe_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->SetRPMServiceStop(true, secr);
#pragma endregion

#pragma region[Prepare a MYVAULT nxl file]
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;

		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_EDIT);

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
		//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
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

		res = puser->MyVaultDownloadFile(pathid, downloaded_p, 2);

#pragma endregion
		res = pInstance->RPMEditCopyFile(downloaded_p, safe_folder);
		EXPECT_FALSE(res.GetCode());

		vector<SDRmFileRight> filerights;
		//RPMGetFileRights
		res = pInstance->RPMGetFileRights(safe_folder, filerights);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks1;
		//RPMGetRights
		res = pInstance->RPMGetRights(safe_folder, rightsAndWatermarks1);

		unsigned int dirstatus1;
		bool filestatus1;
		//RPMGetFileStatus
		res = pInstance->RPMGetFileStatus(safe_folder, &dirstatus1, &filestatus1);
		EXPECT_EQ(2, dirstatus1);
		EXPECT_TRUE(filestatus1);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RPMEditSaveFile
		res = pInstance->RPMEditSaveFile(safe_folder, L"", false, 3);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RPMDeleteFile
		res = pInstance->RPMDeleteFile(safe_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		safe_folder = L"C:\\SDK test\\RPM\\Dir3\\Sub1\\zhu\\zhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuh\\";

		res = pInstance->RemoveRPMDir(safe_folder, 1);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(UserBackSlashNameTest, BackSlashNameTest) {
	try {
		wstring safe_folder = L"C:/SDK test/RPM/Dir3/Sub1/";
		string pathid;
		wstring downloaded_p = L"C:/auto/output/SDK/Project";
		createFolder(ws2s(safe_folder));
		createFolder(ws2s(downloaded_p));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		bool running = true;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

		ISDRmcInstance * pInstance1;
		ISDRmTenant * pTenant1;
		ISDRmUser *puser1 = NULL;
		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		//RPMRegisterApp
		res = pInstance->RPMRegisterApp(apppath_slash, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RPMNotifyRMXStatus(running, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		bool is_register;
		res = pInstance->RPMIsAppRegistered(apppath_slash, &is_register);
		EXPECT_TRUE(is_register);

		res = pInstance->RPMUnregisterApp(apppath_slash, secr);

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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

		//AddRPMDir
		res = pInstance->AddRPMDir(safe_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//IsRPMFolder
		unsigned int dirstatus;
		res = pInstance->IsRPMFolder(safe_folder, &dirstatus);
		EXPECT_EQ(dirstatus, 0);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RemoveRPMDir
		res = pInstance->RemoveRPMDir(safe_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->AddRPMDir(safe_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		res = pInstance->SetRPMServiceStop(true, secr);
#pragma endregion

#pragma region[Prepare a MYVAULT nxl file]
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;
		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_EDIT);

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
		//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
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

		res = puser->MyVaultDownloadFile(pathid, downloaded_p, 2);

#pragma endregion
		string d_p = ws2s(downloaded_p);
		string_replace(d_p, "\\", "/");
		res = pInstance->RPMEditCopyFile(downloaded_p, safe_folder);
		EXPECT_FALSE(res.GetCode());
		string safe_folder_s = ws2s(safe_folder);

		string safe_p = ws2s(safe_folder);
		string_replace(safe_p, "\\", "/");
		res = pInstance->RPMDeleteFileToken(s2ws(safe_p));

		string_replace(safe_folder_s, "\\", "/");

		vector<SDRmFileRight> filerights;
		//RPMGetFileRights
		res = pInstance->RPMGetFileRights(s2ws(safe_folder_s), filerights);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks1;
		//RPMGetRights
		res = pInstance->RPMGetRights(s2ws(safe_folder_s), rightsAndWatermarks1);

		unsigned int dirstatus1;
		bool filestatus1;
		//RPMGetFileStatus
		res = pInstance->RPMGetFileStatus(s2ws(safe_folder_s), &dirstatus1, &filestatus1);
		EXPECT_EQ(2, dirstatus1);
		EXPECT_TRUE(filestatus1);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RPMEditSaveFile
		res = pInstance->RPMEditSaveFile(s2ws(safe_folder_s), L"", false, 3);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RPMDeleteFile
		res = pInstance->RPMDeleteFile(s2ws(safe_folder_s));
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//RemoveRPMDir
		safe_folder = L"C:/SDK test/RPM/Dir3/Sub1/";
		res = pInstance->RemoveRPMDir(safe_folder, 1);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}