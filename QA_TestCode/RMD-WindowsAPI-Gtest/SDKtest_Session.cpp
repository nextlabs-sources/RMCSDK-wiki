#include "stdafx.h"
#include "SDLInc.h"

class SessionInterface :public::testing::TestWithParam<vector<string>> {};
TEST_P(SessionInterface, ValidInput)
{
	vector<string> csv = GetParam();
	string IsRun = csv[0];
	if (IsRun == "run" || IsRun == "1")
	{
		string caseID = csv[1];
		string router_str = csv[2];
		string tenant_str = csv[3];
		string RmcAPIWorkingPath = csv[4];
		string dataFolder = csv[5];
		string JsonFile = csv[6];
		string clientID = csv[7];

		wstring router_ws = s2ws(router_str);
		if (router_ws == L"null") { router_ws = L""; }
		wstring tenant_ws = s2ws(tenant_str);
		if (tenant_ws == L"null") { tenant_ws = L""; }
		wstring folder = s2ws(RmcAPIWorkingPath);
		wstring defaultWorkingFolder = L"c:\\temp";
		cout << "caseID: " << caseID << endl;
		string user_return = dataFolder + "\\" + JsonFile;

		string email = "Lifi.yuan@nextlabs.com";
		string password = "123blue!";
		md5(password);

		ISDRmcInstance * pInstance;
		ISDRmTenant *pTenant = NULL;
		ISDRmHttpRequest *prequest = NULL;
		ISDRmUser *puser = NULL;
		SDWLResult res;

		//res = SDWLibCreateSDRmcInstance((WCHAR*)folder.c_str(), &pInstance);
		cout << "SDWLibCreateSDRmcInstance: ";
		if (clientID == "default")
		{
			cout << "(Default ClientID) ";
			res = SDWLibCreateSDRmcInstance("", 1, 0, 0, defaultWorkingFolder.c_str(), defaultWorkingFolder.c_str(), &pInstance);

		}
		else
		{
			cout << "(Input ClientID: "<<clientID<<") ";
			res = SDWLibCreateSDRmcInstance("", 1, 0, 0, defaultWorkingFolder.c_str(), defaultWorkingFolder.c_str(), &pInstance, (char*)clientID.c_str());
		}

		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//test: Initialize
		if (folder == L"null")
		{
			folder = L"";
			//test: SDWLResult Initialize(std::wstring router, std::wstring tenant);
			cout << "Initialize(router, tenant): ";
			res = pInstance->Initialize(router_ws, tenant_ws);
		}
		else
		{
			//test: SDWLResult Initialize(std::wstring workingfolder, std::wstring router, std::wstring tenant);
			if (folder == L"NULL") { folder = L""; }
			cout << "Initialize(folder, router_ws, tenant_ws): ";
			res = pInstance->Initialize(folder, router_ws, tenant_ws);
		}
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		/*Error: 21: SDWL_CERT_NOT_READY (? how to install certificate )
		**Error: SDWL_RMS_ERRORCODE_BASE +
		*/

		//test: GetCurrentTenant
		res = pInstance->GetCurrentTenant(&pTenant);
		cout << "GetCurrentTenant(&pTenant): ";
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (res.GetCode() == 0)
		{
			cout << "----CurrentTenant----" << endl;
			wcout << L"Tenant:" << pTenant->GetTenant() << endl;
			wcout << L"Router URL:" << pTenant->GetRouterURL() << endl;
			wcout << L"RMS URL:" << pTenant->GetRMSURL() << endl;
			cout << "---------------------" << endl;
		}

		//test: Save
		wcout << L"Save(folder): [" << folder << L"]";
		res = pInstance->Save();
		//res = pInstance->Save(folder);
		//pInstance->Save(L"C:\\work\\123"); //hard code path for check if the file can be create
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//check if the Instance.history, Tenant.history created in the folder
		if (folder == L"") {
			folder = defaultWorkingFolder;
		}
		bool fExist = checkFileExist(ws2s(folder), "Instance.history");
		EXPECT_TRUE(fExist);
		if (fExist)
			deleteFile(ws2s(folder), "Instance.history");

		fExist = checkFileExist(ws2s(folder), "Tenant.history");
		EXPECT_TRUE(fExist);
		if (fExist)
			deleteFile(ws2s(folder), "Tenant.history");


		//test: GetLoginRequest(ISDRmHttpRequest **prequest)
		cout << "GetLoginRequest: ";
		res = pInstance->GetLoginRequest(&prequest);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (res.GetCode() == 0)
		{
			cout << "-----LoginRequest-----" << endl;
			wcout << L"DomainUrl: " << prequest->GetDomainUrl() << endl;
			wcout << L"GetPath: " << prequest->GetPath() << endl;
			wcout << L"HTTP Method: " << prequest->GetMethod() << endl;
			for each (HttpCookie cookie in prequest->GetCookies())
			{
				wcout << "Cookie name:[" << cookie.first << "]";
				wcout << "  Vaule:[" << cookie.second << "]" << endl;
				if (ws2s(cookie.first) == "clientId")
				{
					clientID = ws2s(cookie.second);
				}
			}
			for each(wstring htype in prequest->GetAcceptTypes()) {
				wcout << "Accept Type: " << htype << endl;
			}
			cout << "---------------------" << endl;
		}

		if (dataFolder != "null")
		{
			GetUserLoginResponseData(email, password, clientID, user_return);

			//test: SetLoginResult
			cout << "SetLoginResult: ";
			res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (res.GetCode() == 0)
			{
				cout << "-----SetLoginResult-----" << endl;
				wcout << "Username: " << puser->GetName() << endl;
				wcout << "Email address: " << puser->GetEmail() << endl;
				wcout << "IDP type: " << puser->GetIdpType() << endl;
				cout << "passcode : " << puser->GetPasscode() << endl;
				cout << "---------------------" << endl;
				

				//test: GetCurrentTenant
				res = pInstance->GetCurrentTenant(&pTenant);
				cout << "GetCurrentTenant(&pTenant):/after user login ";
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				if (res.GetCode() == 0)
				{
					cout << "----CurrentTenant----" << endl;
					wcout << L"Tenant:" << pTenant->GetTenant() << endl;
					wcout << L"Router URL:" << pTenant->GetRouterURL() << endl;
					wcout << L"RMS URL:" << pTenant->GetRMSURL() << endl;
					cout << "---------------------" << endl;
				}
				//Check Initialize without tenant, can do protect 
				std::string tags = "";
				SDR_Expiration expire;
				SDR_WATERMARK_INFO watermarkinfo;
				expire.type = IExpiryType::NEVEREXPIRE;
				std::vector<SDRmFileRight> rights = { RIGHT_VIEW,RIGHT_EDIT };
				wstring localfile_pathw = L"C:\\SDK test data\\PLAN FILE\\1.3.pdf";
				cout << "ProtectFile: ";
				wstring newcreatedfilePath = L"";
				res = puser->ProtectFile(localfile_pathw, newcreatedfilePath, rights, watermarkinfo, expire, tags);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
			}
		}


		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl;
	}
}
INSTANTIATE_TEST_CASE_P(SessionTesting, SessionInterface, testing::ValuesIn(csv_Session));

class Session_GetLoginUser :public::testing::TestWithParam<vector<string>> {};
TEST_P(Session_GetLoginUser, ValidInput)
{
	vector<string> csv = GetParam();
	string IsRun = csv[0];
	if (IsRun == "run" || IsRun == "1")
	{
		string caseID = csv[1];
		string dataFolder = csv[2];
		string JsonFile = csv[3];
		string test_email = csv[4];
		string test_passcode = csv[5];
		wstring router_new = s2ws(csv[6]);
		wstring tenant_new = s2ws(csv[7]);

		wstring defaultWorkingFolder = L"c:\\temp";
		wstring defaultRouter = L"https://rmtest.nextlabs.solutions";
		wstring defaultTenant = L"skydrm.com";
		string user_return = dataFolder + "\\" + JsonFile;
		if (router_new == L"default") { router_new = defaultRouter; }
		if (tenant_new == L"default") { tenant_new = defaultTenant; }

		ISDRmcInstance * pInstance;
		ISDRmUser *puser1 = NULL;
		ISDRmUser *puser2 = NULL;
		SDWLResult res;

		//SDWLibCreateSDRmcInstance(defaultWorkingFolder.c_str(), &pInstance1);
		string clientID = "";
		//string email = "Lifi.yuan@nextlabs.com";
		string email = "472049994@qq.com";
		string password = "123blue!";
		md5(password);

		res = CreateSDRmcInstance_CreateClientID(defaultWorkingFolder.c_str(), pInstance, clientID, defaultRouter, defaultTenant);

		pInstance->Save();
		GetUserLoginResponseData(email, password, clientID, user_return);

		cout << "SetLoginResult: ";
		res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser1, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		if (test_email == "default") { wstring email_ws = puser1->GetEmail();			test_email = ws2s(email_ws); }
		else if (test_email == "null") { test_email = ""; }
		if (test_passcode == "default") { test_passcode = puser1->GetPasscode(); }
		else if (test_passcode == "null") { test_passcode = ""; }

		////////
		cout << "GetLoginUser() with email: " << test_email << ", password: " << test_passcode << endl;
		res = pInstance->GetLoginUser(test_email, test_passcode, &puser2);
		checkSDWLResult(res);
		if (s2ws(test_email) == puser1->GetEmail() && test_passcode == puser1->GetPasscode())
		{
			cout << "Debug info- case: Expected valid "<<endl;
			EXPECT_EQ(0, res.GetCode());
			if (res.GetCode() == 0)
			{
				EXPECT_STREQ(puser1->GetName().c_str(), puser2->GetName().c_str());
				EXPECT_EQ(puser1->GetIdpType(), puser2->GetIdpType());
			}

			//case: SDWL_LOGIN_REQUIRED, check the error message
			//EXPECT_EQ(12, res.GetCode())

		}
		else
		{
			cout << "Debug info- case: Expected Invalid " << endl;
			EXPECT_EQ(0, res.GetCode()); //Delete this line after implement the following functions

			//case: SDWL_INVALID_DATA
			//EXPECT_EQ(13, res.GetCode())
			//invalid email, check the error message
			//invalid passcode, check the error message
		}

		SDWLibDeleteRmcInstance(pInstance);

		cout << "-------------This case finished--------" << endl << endl;
	}
}
INSTANTIATE_TEST_CASE_P(SessionTesting, Session_GetLoginUser, testing::ValuesIn(csv_Session_GetLoginUser));


//Create for Protect workflow test 6/11/2018
TEST(protectWorkflow, user)
{
	try
	{
		ISDRmcInstance * pInstance;
		ISDRmTenant *pTenant = NULL;
		ISDRmHttpRequest *prequest = NULL;
		ISDRmUser *puser = NULL;
		SDWLResult res;
		const wstring tempfolder = L"C:\\auto\\output\\SDK\\Protect";
		wstring localfile_pathw = L"C:\\SDK test data\\PLAN FILE\\Normal_TXT.txt";
		string rights_str ="view|edit";
		std::vector<SDRmFileRight> rights;
		rights = Tick_Rights(rights_str);

		string clientID="";
		string email = "Lifi.yuan@nextlabs.com";
		//string password = "c27b83a22f39dceb435ed6d08ec42748";
		string password = "123blue!";
		md5(password);
		string user_return = "C:\\auto\\output\\SDK\\Protect\\User1.txt";
		res = CreateSDRmcInstance_CreateClientID(tempfolder.c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);

		pInstance->Save();
		GetUserLoginResponseData(email, password, clientID, user_return);

		cout << "SetLoginResult: ";
		res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());


		//std::vector<SDRmFileRight> rights;
		//rights.push_back(RIGHT_VIEW);
		//rights.push_back(RIGHT_PRINT);
		////rights.push_back(RIGHT_CLIPBOARD);
		//rights.push_back(RIGHT_SHARE);
		//rights.push_back(RIGHT_DOWNLOAD);
		SDR_WATERMARK_INFO watermarkinfo;
		watermarkinfo.text = "$(User)$(Date)$(Time)";
		watermarkinfo.fontName = "";
		watermarkinfo.fontColor = "";
		watermarkinfo.fontSize = 20;
		watermarkinfo.transparency = 2;
		watermarkinfo.rotation = WATERMARK_ROTATION::NOROTATION;
		watermarkinfo.repeat = 1;
		std::string tags = "";
		SDR_Expiration expire;
		expire.type = IExpiryType::RELATIVEEXPIRE;
		expire.end = 1550051236000;
		wstring newcreatedfilePath = L"";
		res = puser->ProtectFile(localfile_pathw, newcreatedfilePath, rights, watermarkinfo, expire, tags);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		wstring npath = L"C:\\auto\\output\\SDK\\Protect\\skydrm.com\\52\\Normal_TXT-2018-06-12-05-17-50.txt.nxl";
		ISDRmNXLFile *nfile=NULL;
		res= puser->OpenFile(npath, &nfile);


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


string clientID = "";
string passcode_session_common = "";
string tempfolder = "c:\\SDK test\\work\\001";
wstring username = L"";
int GetIdpType;
string email_session = "472049994@qq.com";
TEST(SessionGetloginUser, caseSet)
{
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;

	string password = "123blue!";
	md5(password);
	
	string user_return = tempfolder + "\\user_" + email_session + ".json";
	res = CreateSDRmcInstance_CreateClientID(s2ws(tempfolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);

	//waitInstanceInitFinish(pInstance);

	GetUserLoginResponseData(email_session, password, clientID, user_return);

	cout << "SetLoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res.GetCode() == 0)
	{
		username = puser->GetName();
		GetIdpType = puser->GetIdpType();
	}


	passcode_session_common = puser->GetPasscode();
	cout << "passcode: " << passcode_session_common << endl;

	pInstance->Save();
	SDWLibDeleteRmcInstance(pInstance);

	cout << "----1----" << endl;
}
TEST(SessionGetloginUser, caseGet)
{
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;

	res = CreateSDRmcInstance_CreateClientID(s2ws(tempfolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	cout << "GetLoginUser() with email: " << email_session << ", passcode: " << passcode_session_common << endl;
	res = pInstance->GetLoginUser(email_session, passcode_session_common, &puser);
	checkSDWLResult(res);
	if (res.GetCode() == 0)
	{
		EXPECT_STREQ(username.c_str(), puser->GetName().c_str());
		EXPECT_EQ(GetIdpType, puser->GetIdpType());
	}

	SDWLibDeleteRmcInstance(pInstance);

	cout << "----2----" << endl;
}

//Test RPM cache token
TEST(CacheTokenToRPM, CacheToken) {
	try {
		string clientID = "";
		SDWLResult res;
		string testFolder = TestDirectory + "RPMtest";
		createFolder(testFolder);
		bool ret = false;
		wstring localfile_pathw = plan_file_path + L"Normal_TXT.txt";

		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_DOWNLOAD);
		wstring nxlfile_path;
		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		string tag_text = "{}";
		wstring w_name;

		ISDRmcInstance * pInstance;
		ISDRmUser *puser = NULL;
		ISDRmTenant *pTenant = NULL;
		//string email = "";
		//string password = "";

		//SDK LOGIN
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email_zhu, password);
		//bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, "472049994@qq.com", "c27b83a22f39dceb435ed6d08ec42748");
		string exportpasscode = ReadFromFile(plan_path + "passcode.txt");
		ISDRmNXLFile* nxlfile = NULL;
		/*res = puser->OpenFile(L"C:\\auto\\output\\SDK\\Project\\", &nxlfile);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());*/
		//bool login_result = PreviousUser_Login(pInstance, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, exportpasscode);
		//res = pInstance->GetLoginUser(email_zhu, exportpasscode, &puser);
		/*for (int i = 0; i < 10;i++) {
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
		res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
		Sleep(1500);
		}*/
		//std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
		//res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);

		string  puser_return = plan_path + "User_Return.txt";
		if (res) {
			cout << "GetHeartBeatInfo: ";
			SDWLResult res = puser->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			//res = puser->ProjectDownloadFile(5, "/john_return-2018-10-22-09-46-02.txt.nxl", L"C:\\auto\\output\\SDK\\Project\\", false);
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());

			std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
			wstring nxlFilePath = L"C:\\SDK test\\JE\\7303\\MYVAULT\\5-2018-09-12-10-55-54.jpg.nxl";
			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_b;
			ISDRmNXLFile* nxlfile_a = NULL;
			//7303
			//for (int i = 0; i < 10000; i++) {
			//	res = puser->OpenFile(L"C:\\SDK test\\JE\\7303\\MYVAULT\\ProjectFileInfo-2018-10-12-11-18-37.txt.nxl", &nxlfile_a);
			//	Sleep(500);
			//	res = puser->GetRights(L"C:\\SDK test\\JE\\7303\\MYVAULT\\ProjectFileInfo-2018-10-12-11-18-37.txt.nxl", rightsAndWatermarks);

			//	res = puser->GetFileRightsFromCentralPolicies(nxlFilePath, rightsAndWatermarks_b);
			//}
			//res = puser->OpenFile(L"C:\\Users\\jzhu\\Downloads\\revokeUserA-2018-11-06-07-06-03.txt.nxl", &nxlfile_a);
			//pInstance->Save();
			//res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\revokeUserA-2018-11-06-07-06-03.txt.nxl", rightsAndWatermarks);
			//
			//string net;
			//cout << "Change Network: ";
			//cin >> net;
			//res = puser->OpenFile(L"C:\\Users\\jzhu\\Downloads\\share20181106-2018-11-06-09-35-05.txt.nxl", &nxlfile_a);
			//bool expired = nxlfile_a->CheckExpired();
			//bool rights_check = nxlfile_a->CheckRights(RIGHT_CLASSIFY);

			//res = puser->OpenFile(L"C:\\Users\\jzhu\\Downloads\\Local_protect.txt-2018-11-06-11-12-22.nxl.nxl", &nxlfile_a);
			//res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\Local_protect.txt-2018-11-06-11-12-22.nxl.nxl", rightsAndWatermarks);

			//res = puser->AddActivityLog(nxlfile_a, RL_OReshare, RL_RDenied);
			//res = puser->AddActivityLog(nxlfile_a, RL_OCaptureScreen, RL_RDenied);
			//res = puser->AddActivityLog(nxlfile_a, RL_ORemoveUser, RL_RDenied);
			//res = puser->AddActivityLog(nxlfile_a, RL_ODecrypt, RL_RAllowed);
			//res = puser->AddActivityLog(nxlfile_a, RL_OView, RL_RDenied);
			//res = puser->AddActivityLog(nxlfile_a, RL_OEdit, RL_RDenied);
			//res = puser->AddActivityLog(nxlfile_a, RL_OPrint, RL_RAllowed);
			//res = puser->UploadActivityLogs();
			res = puser->OpenFile(L"C:\\Users\\jzhu\\Downloads\\setting-2018-11-08-10-12-58.docx.nxl", &nxlfile_a);
			bool expired = nxlfile_a->CheckExpired();
			res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\setting-2018-11-08-10-12-58.docx.nxl", rightsAndWatermarks_b);
			//res = puser->LogoutUser();
			//checkSDWLResult(res);
			//SDWLibDeleteRmcInstance(pInstance);
			//cout << "-------------This case finished--------" << endl << endl;

			//RMTEST
			//res = puser->OpenFile(L"C:\\SDK test\\JE\\RMTEST\\MYVAULT\\Protect_normal-2018-09-25-03-42-37.txt.nxl", &nxlfile_a);
			//res = puser->GetRights(L"C:\\SDK test\\JE\\RMTEST\\MYVAULT\\Protect_normal-2018-09-25-03-42-37.txt.nxl", rightsAndWatermarks);
			//printPolicy(pInstance, puser);

			string tenantName_s = "";

			cout << "Input tenant name: ";
			cin >> tenantName_s;
			wstring tenantName = s2ws(tenantName_s);

			wcout << L"test GetPolicyBundle() with input: [" << tenantName << L"]" << endl;
			printPolicyBaseOnTenant(tenantName, puser);
		}
		cout << "----------RPM------------" << endl;

		ret = pInstance->IsRPMDriverExist();
		cout << "IsRPMDriverExist: " << ret << endl;
		if (!ret)
		{
			cout << "Please double check if the RPM build is installed!" << endl;
		}

		cout << "GetloginData: " << endl;
		passcode_session_common = puser->GetPasscode();
		string user_return; res = pInstance->GetLoginData(email, passcode_session_common, user_return);

		cout << "SetRPMLoginResult: ";
		DWORD start = ::GetTickCount();
		res = pInstance->SetRPMLoginResult(user_return, secr);
		checkSDWLResult(res);

		/*cout << "SetRPMCachedToken: ";
		res = pInstance->SetRPMCachedToken();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());*/
		cout << "AddRPMDir: ";
		wstring rpm_folder = L"E:\\spin1\\ZHUZHU1";
		res = pInstance->AddRPMDir(rpm_folder);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		pInstance->SetRPMServiceStop(true, secr);
		//cout << "cached tokens(myvault) : " << endl;

		//string net;
		//cout << "Remove   1: ";
		//cin >> net;

		//res = pInstance->RemoveRPMDir(rpm_folder);
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		//string net1;
		//cout << "ADD  1: ";
		//cin >> net1;

		//res = pInstance->AddRPMDir(rpm_folder);
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		//string remove;
		//cout << "Remove folder: ";
		//cin >> remove;
		//if (remove == "y") {
		//	res = pInstance->RemoveRPMDir(rpm_folder);
		//	checkSDWLResult(res);
		//	EXPECT_EQ(0, res.GetCode());
		//}


		/*res = pInstance->CacheRPMFileToken(L"C:\\SDK test\\JE\\7303\\MYVAULT\\size-2018-10-30-09-51-16.png.nxl");
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());*/
		//cout << "cached tokens2(central policy file) : " << endl;
		//res = pInstance->CacheRPMFileToken(L"C:\\auto\\output\\SDK\\Project\\DUMPALL-2018-10-24-04-13-51.txt.nxl");
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_cp;
		ISDRmNXLFile* nxlfile_b = NULL;
		//7303
		/*res = pInstance->SetRPMDeleteCacheToken();
		res = puser->OpenFile(L"C:\\SDK test\\JE\\7303\\MYVAULT\\bbbbb-2018-10-11-02-45-06.txt.nxl", &nxlfile_b);
		res = puser->GetRights(L"C:\\SDK test\\JE\\7303\\MYVAULT\\bbbbb-2018-10-11-02-45-06.txt.nxl", rightsAndWatermarks) ;*/

		//RMTEST
		//res = puser->OpenFile(L"C:\\SDK test\\JE\\RMTEST\\MYVAULT\\t-2018-11-01-09-47-38.png.nxl", &nxlfile_b);
		//res = puser->GetRights(L"C:\\SDK test\\JE\\RMTEST\\MYVAULT\\t-2018-11-01-09-47-38.png.nxl", rightsAndWatermarks);
		//res  = pInstance->SetRPMDeleteCacheToken();
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		string iflogout = "";
		cout << "Input Y do logout: ";
		cin >> iflogout;
		if (iflogout == "Y" || iflogout == "y" || iflogout == "yes") {
			cout << "RPMLogout: ";
			DWORD start_logout = ::GetTickCount();
			res = pInstance->RPMLogout();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (!res.GetCode()) {
				cout << "RPM Logout!" << endl;
			}

			cout << endl << "SDK user logout:";
			res = puser->LogoutUser();
			checkSDWLResult(res);
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