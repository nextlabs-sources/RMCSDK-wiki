#include "stdafx.h"
#include "SDLInc.h"
#include <shlwapi.h>
#include <pthread.h>
static string clientID = "";
TEST(RPM, OpenfileWorkflow)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	ISDRmHttpRequest *prequest = NULL;
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	wstring app = L"C:\\auto\\Clone-rmcsdk-wiki2\\QA_TestCode\\x64\\Debug\\RMD-WindowsAPI-Gtest.exe";
	wstring nxlPath = L"F:\\includingRPM\\Safe1\\7306\\AD-2019-03-01-08-56-33.txt.nxl";
	wstring noNXLPath = L"F:\\includingRPM\\Safe1\\7306\\AD-2019-03-01-08-56-33.txt";
	int charNum = 35;
	bool retval;

	//cout << "Before GetCurrentLoggedInUser: " << endl;
	//bool retval0 = PathFileExists(nxlPath.c_str());
	//if (!retval0)
	//{
	//	cout << "with .NXL, file cannot find." << endl;
	//}
	//retval0 = PathFileExists(noNXLPath.c_str());
	//if (!retval0)
	//{
	//	cout << "NO .NXL, file cannot find." << endl;
	//}
	//cout << "================= " << endl;

	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res) {
		wcout << L"puser info: " << puser->GetEmail() << endl;
	}

	cout << "RPMRegisterApp_ " << ws2s(app)<<": ";
	res = pInstance->RPMRegisterApp(app, secr);
	checkResult(res, 0);

	cout << "RPMNotifyRMXStatus: " << endl;
	cout << "[1]:" << getTime() << endl;
	res = pInstance->RPMNotifyRMXStatus(true, secr);
	cout << "[2]:" << getTime() << endl;
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	Sleep(1000);

	cout << "with .NXL content:" << endl;
	retval = PathFileExists(nxlPath.c_str());
	if (retval)
	{
		readFileBeginningChar(ws2s(nxlPath), charNum);
	}
	else {
		cout << "with .NXL, file cannot find." << endl;
	}

	cout << endl << "NO   .NXL content:" << endl;
	retval = PathFileExists(noNXLPath.c_str());
	if (retval)
	{
		cout << "[3]:" << getTime() << endl;
		string first35char=readFileBeginningChar(ws2s(noNXLPath), charNum);
		string NXLhead = "NXLFMT@";
		string::size_type idx;
		idx = first35char.find(NXLhead);
		if (!(idx == string::npos))
		{
			wcout << L"Press Enter key to end..." << endl;
			getchar();
		}
	}
	else {
		cout << "NO .NXL, file cannot find." << endl;
	}

	
	SDWLibDeleteRmcInstance(pInstance);
}


TEST(User, loopHeartbeat)
{
	string testFolder = TestDirectory + "7302John";
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
	wstring f = s2ws(testFolder);

	cout << "CreateInstance: ";
	res = SDWLibCreateSDRmcInstance(DEFUALT_SDKLib_FOLDER, f.c_str(), &pInstance);
	checkSDWLResult(res);

	string workingfoler = ws2s(f) + "\\working";
	createFolder(workingfoler);
	cout << "Initialize: ";
	res = pInstance->Initialize(s2ws(workingfoler), DEFUALT_ROUTER, DEFUALT_TENANT);
	checkSDWLResult(res);
	string clientId = "";
	waitInstanceInitFinish(pInstance);
	string John_return = "C:\\SDK test data\\7302john.tyler.json";

	cout << "GetLoginRequest: ";
	ISDRmHttpRequest *prequest = NULL;
	res = pInstance->GetLoginRequest(&prequest);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	cout << "GetPath():" << ws2s(prequest->GetPath()) << endl;
	if (res.GetCode() == 0)
	{
		cout << "-----LoginRequest-----" << endl;
		for each (HttpCookie cookie in prequest->GetCookies())
		{
			cout << "Cookie name:[" << ws2s(cookie.first) << "]";
			cout << "  Vaule:[" << ws2s(cookie.second) << "]" << endl;
			if (ws2s(cookie.first) == "clientId")
			{
				clientId = ws2s(cookie.second);
			}
		}
	}

	cout << "Set SDK LoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(John_return), &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res) {
		string loginInfo;
		res= pInstance->GetLoginData(ws2s(puser->GetEmail()), puser->GetPasscode(), loginInfo);
		string exportfilename = plan_path + "JohnExport.txt";
		ofstream SaveFile(exportfilename, ios::ate);
		SaveFile << loginInfo;// << endl;
		SaveFile.close();
		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(loginInfo, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		int i = 0;
		while (i<10)
		{
			i++;
			cout << "[" << i << "] " << "GetHeartBeatInfo: ";
			SDWLResult res = puser->GetHeartBeatInfo();
			checkSDWLResult(res);

			cout << "SyncUserAttributes: ";
			res = pInstance->SyncUserAttributes();
			checkSDWLResult(res);

			Sleep(3000);

			res = pInstance->Save();
			EXPECT_EQ(0, res.GetCode());
		}
		cout << "RPMLogout";
		res = pInstance->RPMLogout();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << endl << "SDK user logout:";
		res = puser->LogoutUser();
		checkSDWLResult(res);

		string email = "julie.fang@nextlabs.com";
		string password = "123blue!";
		md5(password);
		string user_return2 = testFolder + "\\User_" + email + "_RMSreturn.json";
		GetUserLoginResponseData(email, password, clientId, user_return2);

		cout << "Set SDK LoginResult: ";
		res = pInstance->SetLoginResult(ReadFromFile(user_return2), &puser, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (res) {
			string loginInfo2;
			res = pInstance->GetLoginData(ws2s(puser->GetEmail()), puser->GetPasscode(), loginInfo2);
			string exportfilename = plan_path + "julieExport.txt";
			ofstream SaveFile(exportfilename, ios::ate);
			SaveFile << loginInfo2;// << endl;
			SaveFile.close();
			cout << "SetRPMLoginResult: ";
			res = pInstance->SetRPMLoginResult(loginInfo2, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			int i = 0;
			while (i<20)
			{
				i++;
				cout << "[" << i << "] " << "GetHeartBeatInfo: ";
				SDWLResult res = puser->GetHeartBeatInfo();
				checkSDWLResult(res);

				cout << "SyncUserAttributes: ";
				res = pInstance->SyncUserAttributes();
				checkSDWLResult(res);

				Sleep(3000);

				res = pInstance->Save();
				EXPECT_EQ(0, res.GetCode());
			}
		}

	}
	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}

TEST(RPMTest, loginNewInstance)
{
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	string testFolder = TestDirectory + "RPMtest2";
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
	string clientID = "";

	string email = "xiaozhuzhu0712@163.com";
	string password = "2018@next";

	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	md5(password);
	string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
	GetUserLoginResponseData(email, password, clientID, user_return);

	cout << "----------SDK------------" << endl;
	cout << "Set SDK LoginResult: ";
	string userJson = ReadFromFile(user_return);
	res = pInstance->SetLoginResult(userJson, &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	res = pInstance->Save();
	EXPECT_EQ(0, res.GetCode());

	cout << endl << "SDK user logout:";
	res = puser->LogoutUser();
	checkSDWLResult(res);
	//SDWLibDeleteRmcInstance(pInstance);

	ISDRmcInstance * pInstance1;
	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance1, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	GetUserLoginResponseData(email, password, clientID, user_return);
	ISDRmUser *puser2 = NULL;

	cout << "----------SDK------------" << endl;
	cout << "Set SDK LoginResult: ";
	userJson = ReadFromFile(user_return);
	res = pInstance1->SetLoginResult(userJson, &puser2, secr);
	//res = pInstance->SetLoginResult(userJson, &puser2);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	SDWLibDeleteRmcInstance(pInstance);
	SDWLibDeleteRmcInstance(pInstance1);
	cout << "-------------This case finished--------" << endl << endl;
}
TEST(RPMTest, loginNewInstance_2)
{
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	string testFolder = TestDirectory + "RPMtest2";
	createFolder(testFolder);
	string testFolder2 = TestDirectory + "RPMtest3";
	createFolder(testFolder2);
	ISDRmUser *puser = NULL;
	string passcode = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	ISDRmTenant * pTenant;

	string email = "3057318205@qq.com";
	string password = "2018@next";

	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	md5(password);
	string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
	GetUserLoginResponseData(email, password, clientID, user_return);

	cout << "----------SDK------------" << endl;
	cout << "Set SDK LoginResult: ";
	string userJson = ReadFromFile(user_return);
	res = pInstance->SetLoginResult(userJson, &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

#pragma region [Prepare RPM environment]
	string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

	cout << "SetRPMLoginResult: ";
	res = pInstance->SetRPMLoginResult(user_logindata, secr);
	EXPECT_EQ(0, res.GetCode());
	res = pInstance->Save();
	EXPECT_EQ(0, res.GetCode());

	ISDRmcInstance * pInstance1;
	ISDRmUser *puser11 = NULL;
	res = RPMGetCurrentLoggedInUser(passcode, pInstance1, pTenant, puser11);
#pragma endregion

	SDWLibDeleteRmcInstance(pInstance);

	ISDRmcInstance * pInstance_new;
	string clientID_new = "";
	string user_return_new = testFolder2 + "\\User_" + email + "_RMSreturn.json";
	CreateSDRmcInstance_CreateClientID(s2ws(testFolder2).c_str(), pInstance_new, clientID_new, DEFUALT_ROUTER, DEFUALT_TENANT);
	GetUserLoginResponseData(email_zhu, password, clientID_new, user_return_new);
	ISDRmUser *puser2 = NULL;

	cout << "----------SDK------------" << endl;
	cout << "Set SDK LoginResult: ";
	userJson = ReadFromFile(user_return_new);
	res = pInstance_new->SetLoginResult(userJson, &puser2, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
#pragma region [Prepare RPM environment]
	string user_logindata_new;
	res = pInstance->GetLoginData(ws2s(puser2->GetEmail()), puser2->GetPasscode(), user_logindata_new);

	cout << "SetRPMLoginResult: ";
	res = pInstance_new->SetRPMLoginResult(user_logindata_new, secr);
	EXPECT_EQ(0, res.GetCode());

	res = pInstance->Save();
	EXPECT_EQ(0, res.GetCode());

	ISDRmcInstance * pInstance2;
	res = RPMGetCurrentLoggedInUser(passcode, pInstance_new, pTenant, puser2);
#pragma endregion
	SDWLibDeleteRmcInstance(pInstance1);

	ISDRmcInstance * pInstance3;
	res = RPMGetCurrentLoggedInUser(passcode, pInstance3, pTenant, puser);
	wstring email_str = puser->GetEmail();
	EXPECT_STREQ(ws2s(email_str).c_str(), email_zhu.c_str());
	EXPECT_EQ(0, res.GetCode());
	cout << "-------------This case finished--------" << endl << endl;
}

void Protect_hardcode(ISDRmUser *puser)
{
	SDWLResult res;
	SDR_Expiration expire;
	SDR_WATERMARK_INFO watermarkinfo;
	expire.type = IExpiryType::NEVEREXPIRE;
	std::vector<SDRmFileRight> rights;
	wstring newcreatedfilePath = L"C:\\SDK test\\AddRPMtest\\A0\\B1first\\7303";
	string rights_str = "view";
	rights = Tick_Rights(rights_str);
	cout << "ProtectFile_Protect_hardcode checking: ";
	res = puser->ProtectFile(L"C:\\SDK test data\\Normal_TXT.txt", newcreatedfilePath, rights, watermarkinfo, expire, tags);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
}
void Protect_midInput(ISDRmUser *puser, string mid)
{
	SDWLResult res;
	SDR_Expiration expire;
	SDR_WATERMARK_INFO watermarkinfo;
	expire.type = IExpiryType::NEVEREXPIRE;
	std::vector<SDRmFileRight> rights;
	wstring newcreatedfilePath = L"";
	//string rights_str = "view";
	//rights = Tick_Rights(rights_str);
	string tagfile = "C:\\SDK test data\\tag_String.txt";
	string tags = ReadFromFile(tagfile);

	cout << "ProtectFile_with mid:[ "<<mid<<"]";
	res = puser->ProtectFile(L"C:\\SDK test data\\Normal_TXT.txt", newcreatedfilePath, rights, watermarkinfo, expire, tags, mid);
	//res = puser->ProtectFile_tc(L"C:\\SDK test data\\Normal_TXT.txt", newcreatedfilePath, rights, watermarkinfo, expire, tags, mid);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
}

string common_clientID = "";
string common_testFolder = TestDirectory + "10.3test";
string common_email = "Lifi.yuan@nextlabs.com";
//string common_email = "lifi.yuan@nextlabs.com";
//string common_email = "472049994@qq.com";
//string common_email = "472049994@qq.com";
//string common_email = "jewelry.zhu@nextlabs.com";
string common_passcode = "";
string common_user_return = common_testFolder + "\\User_" + common_email + "_login.json";
SDWLResult common_loginres;
TEST(NOTRUN_V103, SDKLogin)
{
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant;
	res = CreateSDRmcInstance_CreateClientID(s2ws(common_testFolder).c_str(), pInstance, common_clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);
	string password = "123blue!";
	//string password = "Scott0707!";
	//string password = "2018@next";
	md5(password);
	GetUserLoginResponseData(common_email, password, common_clientID, common_user_return);


	cout << "SetLoginResult: ";
	common_loginres = pInstance->SetLoginResult(ReadFromFile(common_user_return), &puser, secr);
	checkSDWLResult(common_loginres);
	EXPECT_EQ(0, common_loginres.GetCode());

	res = pInstance->GetCurrentTenant(&pTenant);
	cout << "GetCurrentTenant(&pTenant):/after user login ";
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res.GetCode() == 0)
	{
		wcout << L"Tenant:" << pTenant->GetTenant() << endl;
		wcout << L"Router URL:" << pTenant->GetRouterURL() << endl;
	}

	if (common_loginres)
	{
		cout << "get passcode. " << endl;
		common_passcode = puser->GetPasscode();
	}

	//cout << "SetRPMClientId: ";
	//res = pInstance->SetRPMClientId(common_clientID);
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());

	cout << "SetRPMLoginResult: ";
	DWORD start = ::GetTickCount();
	res = pInstance->SetRPMLoginResult(ReadFromFile(common_user_return), secr);
	DWORD Etime = ::GetTickCount() - start;
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	cout << "  Evaluation time: " << Etime << " (ms)" << endl;

	cout << "GetHeartBeatInfo: ";
	res = puser->GetHeartBeatInfo();
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	cout << "Save: ";
	res = pInstance->Save();
	checkSDWLResult(res);

	//Protect_hardcode(puser);

	SDWLibDeleteRmcInstance(pInstance);
}

TEST(NOTRUN_V103, 100times)
{
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant;
	res = CreateSDRmcInstance_CreateClientID(s2ws(common_testFolder).c_str(), pInstance, common_clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);
	cout << "GetLoginUser(): ";
	res = pInstance->GetLoginUser(common_email, common_passcode, &puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	if (res) {
		wcout << L"email: " << puser->GetEmail() << endl;
		wcout << L"name: " << puser->GetName() << endl;

		cout << "GetHeartBeatInfo:";
		res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "GetListProjects:";
		res = puser->GetListProjects(1, 20, "lastActionTime", RF_All);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		bool adhoc; int heartbeat; int sysprojectid; string sysprojecttenantid;
		res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid);
		string mid_s = puser->GetMembershipID(sysprojecttenantid);
		int i = 0;
		while (i < 1)
		{
			Protect_midInput(puser, mid_s);
			Sleep(5000);
			i++;
		}
	}

	SDWLibDeleteRmcInstance(pInstance);
	cout << "=======Test END======" << endl;

}

TEST(NOTRUN_V103, 100times_open)
{
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant;
	res = CreateSDRmcInstance_CreateClientID(s2ws(common_testFolder).c_str(), pInstance, common_clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);
	cout << "GetLoginUser(): ";
	res = pInstance->GetLoginUser(common_email, common_passcode, &puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	if (res) {
		wcout << L"email: " << puser->GetEmail() << endl;
		wcout << L"name: " << puser->GetName() << endl;

		cout << "GetHeartBeatInfo:";
		res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		int i = 1;
		for each(vector<string> csv_line in csv_filelist)
		{
			cout << "test[" << i << "], " << csv_line[0] << endl;
			string file = "C:\\SDK test\\10.3test\\testFiles\\" + csv_line[0];
			ISDRmNXLFile* nxlfile;
			res = puser->OpenFile(s2ws(file), &nxlfile);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (!res)
			{
				cout << "test is stop since openFile failed." << endl;
				break;
			}
			i++;

			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
			rightsAndWatermarks.clear();
			res = puser->GetRights(s2ws(file), rightsAndWatermarks, false);
			checkSDWLResult(res);
			printRightsOnly(res, rightsAndWatermarks);

			string tags_get = nxlfile->GetTags();
			cout << "tags:" << endl << tags_get << endl;
		}
	}

	SDWLibDeleteRmcInstance(pInstance);
	cout << "=======Test END======" << endl;
}

//V103: SDK version 10.3 new
TEST(NOTRUN_V103, ChangeRightsOfFile)
{
#pragma region[init]
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant;
	res = CreateSDRmcInstance_CreateClientID(s2ws(common_testFolder).c_str(), pInstance, common_clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);

//#define __setlogin__
#if defined(__setlogin__)
	string password = "123blue!";
	md5(password);
	GetUserLoginResponseData(common_email, password, common_clientID, common_user_return);
	cout << "SetLoginResult: ";
	common_loginres = pInstance->SetLoginResult(ReadFromFile(common_user_return), &puser);
	checkSDWLResult(common_loginres);
	EXPECT_EQ(0, common_loginres.GetCode());
#else
	cout << "GetLoginUser(): ";
	res = pInstance->GetLoginUser(common_email, common_passcode, &puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
#endif
	if (res) {
		wcout << L"email: " << puser->GetEmail() << endl;
		wcout << L"name: " << puser->GetName() << endl;

		cout << "GetHeartBeatInfo:";
		res= puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "GetListProjects:";
		res= puser->GetListProjects(1, 20, "lastActionTime", RF_All);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
	/*	cout << "Save():";
		res = pInstance->Save();
		checkSDWLResult(res);*/
#pragma endregion
#pragma region[TestData]
		struct testData {
			std::wstring filepath;
			std::vector<SDRmFileRight> rights;
			SDR_WATERMARK_INFO watermarkinfo;
			SDR_Expiration expire;
			std::string tags;
		};
		testData data;
		string rights_str;
		string wm_text;
		int Validity_option;
		string Validity_startDate;
		string Validity_endDate;
		string tags_csv;

#pragma endregion

		string UserFolder = common_testFolder + "\\" + common_email;
		createFolder(UserFolder);
		//string testFile;

		for each(vector<string> csv_line in csv_UserChangeRights)
		{
			data.rights.clear();
			data.watermarkinfo = {};
			data.tags.clear();
			if (csv_line[0] != "0")
			{
#pragma region[read data from csv]
				data.filepath = s2ws(csv_line[2]);
				rights_str = csv_line[3];
				if (rights_str != "null")
				{
					data.rights = Tick_Rights(rights_str);
				}
				wm_text = csv_line[4];
				if (wm_text != "null")
				{
					data.watermarkinfo = { wm_text,"font","color",9,0,ANTICLOCKWISE,true };
				}

				Validity_option = string2Int(csv_line[5]);
				Validity_startDate = csv_line[6];
				Validity_endDate = csv_line[7];
				IExpiryType t;
				switch (Validity_option) {
				case 1: {
					t = RELATIVEEXPIRE;
					data.expire.end = stringToUINT64(Validity_endDate);
					break;
				}
				case 2: {
					t = ABSOLUTEEXPIRE;
					data.expire.end = stringToUINT64(Validity_endDate);
					break;
				}
				case 3: {
					t = RANGEEXPIRE;
					data.expire.start = stringToUINT64(Validity_startDate);
					data.expire.end = stringToUINT64(Validity_endDate);
					break;
				}
				default:
					t = NEVEREXPIRE;
				}

				data.expire.type = t;

				tags_csv = csv_line[8];
				if (tags_csv == "null") tags_csv = "";
				data.tags = patchTagString(tags_csv);
#pragma endregion
				vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
				bool IsAdmin;
				//do ChangeRightsOfFile with input test file
				if (csv_line[0] == "1") {

					cout << "---------File: " << csv_line[1] << endl;
					cout << "puser->GetRights:";
					rightsAndWatermarks.clear();
					res = puser->GetRights(data.filepath, rightsAndWatermarks, false);
					checkSDWLResult(res);
					printRightsOnly(res, rightsAndWatermarks);


					cout << "HasAdminRights: ";
					IsAdmin = puser->HasAdminRights(data.filepath);
					cout << IsAdmin << endl;

					cout << "ChangeRightsOfFile: ";
					res = puser->ChangeRightsOfFile(data.filepath, data.rights, data.watermarkinfo, data.expire, data.tags);
					checkSDWLResult(res);
					EXPECT_EQ(0, res.GetCode());

					cout << "puser->GetRights:";
					rightsAndWatermarks.clear();
					res = puser->GetRights(data.filepath, rightsAndWatermarks, false);
					checkSDWLResult(res);
					printRightsOnly(res, rightsAndWatermarks);
				}

				//Protect file and do ChangeRightsOfFile
				else if (csv_line[0] == "2" || csv_line[0] == "4")
				{
					wstring destNXL;

					string mid_p = puser->GetMembershipID(68);
					bool adhoc; int heartbeat; int sysprojectid; string sysprojecttenantid;
					res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid);
					string mid_s = puser->GetMembershipID(sysprojecttenantid);
					vector<string> mids = { "null", mid_p, mid_s };
					//vector<string> mids = { "null" };
					//vector<string> mids = { mid_s };
					std::vector<SDRmFileRight> r;
					r.clear();
					if (rights_str != "null")
					{
						rights_str = "view|edit";
						r = Tick_Rights(rights_str);
					}
					
					string t = "{\"Rights\":[\"Rights_view\"]}";
					//string t = "{}";
					//string t = "{\"Default\":[\"YES\"]}";
					for each (string mid in mids)
					{
						destNXL.clear();
						destNXL = L"C:\\SDK test\\AddRPMtest\\A0\\B1first\\7303";
						if (mid == "null")
						{
							//Protect_hardcode(puser);
							mid = "";
						}
						cout << endl << "ProtectFile with mid:[" << mid << "] ";
						res = puser->ProtectFile(data.filepath, destNXL, r, data.watermarkinfo, data.expire, t, mid);
						//res = puser->ProtectFile_tc(data.filepath, destNXL, r, data.watermarkinfo, data.expire, t, mid);
						checkSDWLResult(res);

						EXPECT_EQ(0, res.GetCode());
						if (res)
						{
							if (mid == "")
							{
								//cout << "GetFile: ";
								//ISDRFiles* pFile = NULL;
								//res = puser->GetLocalFileManager(&pFile);
								//size_t fileNum = pFile->GetListNumber();
								//cout << fileNum << endl;
								//if (pFile)
								//{
								//	ISDRmNXLFile* nxlfile = pFile->GetFile(destNXL);
								//	wcout << nxlfile->GetFileName() << endl;
								//	cout << "UploadFile: ";
								//	res = puser->UploadFile(nxlfile);
								//	checkSDWLResult(res);
								//	EXPECT_EQ(0, res.GetCode());
								//}
						/*		ISDRmNXLFile* nxlfile;
								res = puser->OpenFile(destNXL, &nxlfile);*/
								cout << "UploadFile: ";
								res = puser->UploadFile(destNXL, destNXL);
								checkSDWLResult(res);
								EXPECT_EQ(0, res.GetCode());
							}
							//copy file outside from working folder
							/*testFile.clear();
							testFile = UserFolder + "\\" + getFileNameFromPath(ws2s(destNXL));
							CopyviaCMD(ws2s(destNXL), testFile);*/

							//destNXL = s2ws(testFile);
							cout << "-----------------testFile---------------" << endl;
							wcout << L"[" << destNXL << L"]" << endl;
							cout << "puser->GetRights:";
							rightsAndWatermarks.clear();
							res = puser->GetRights(destNXL, rightsAndWatermarks, false);
							checkSDWLResult(res);
							printRightsOnly(res, rightsAndWatermarks);
							if (csv_line[0] == "2") {
								cout << "HasAdminRights: ";
								IsAdmin = puser->HasAdminRights(destNXL);
								cout << IsAdmin << endl;
								cout << "ChangeRightsOfFile: ";
								res = puser->ChangeRightsOfFile(destNXL, data.rights, data.watermarkinfo, data.expire, data.tags);
								checkSDWLResult(res);
								EXPECT_EQ(0, res.GetCode());
								if (res) {
									cout << "GetRights after change rights:";
									rightsAndWatermarks.clear();
									res = puser->GetRights(destNXL, rightsAndWatermarks, false);
									checkSDWLResult(res);
									printRightsOnly(res, rightsAndWatermarks);
								}
							}
							cout << "-----------------   end  ---------------" << endl;

						}
					}
				}

				else if (csv_line[0] == "3")
				{
					cout << "UpdateNXLMetaData for file: [" << csv_line[2] << "] ";
					res = puser->UpdateNXLMetaData(data.filepath);
					checkSDWLResult(res);
					EXPECT_EQ(0, res.GetCode());
				}
			}
		}
	}
	SDWLibDeleteRmcInstance(pInstance);
	cout << "=======Test END======" << endl;
}


TEST(V103, protect_new)
{
#pragma region[init]
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant;

	//#define __setlogin__
#if defined(__setlogin__)
	res = CreateSDRmcInstance_CreateClientID(s2ws(common_testFolder).c_str(), pInstance, common_clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);

	string password = "123blue!";
	md5(password);
	GetUserLoginResponseData(common_email, password, common_clientID, common_user_return);
	cout << "SetLoginResult: ";
	common_loginres = pInstance->SetLoginResult(ReadFromFile(common_user_return), &puser);
	checkSDWLResult(common_loginres);
	EXPECT_EQ(0, common_loginres.GetCode());
#else
	//string email = "Lifi.yuan@nextlabs.com";
	//string passcode;
	//cout << "Input User Email: ";
	//cin >> email;
	//cout << "Input passcode: ";
	//cin >> passcode;
	//cout << "GetLoginUser(): ";
	//res = pInstance->GetLoginUser(email, passcode, &puser);
	//res = pInstance->GetLoginUser(common_email, common_passcode, &puser);
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
#endif
	if (res) {
		wcout << L"email: " << puser->GetEmail() << endl;
		wcout << L"name: " << puser->GetName() << endl;

		cout << "GetHeartBeatInfo:";
		res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "GetListProjects:";
		res = puser->GetListProjects(1, 20, "lastActionTime", RF_All);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		/*	cout << "Save():";
		res = pInstance->Save();
		checkSDWLResult(res);*/
#pragma endregion
#pragma region[TestData]
		struct testData {
			std::wstring filepath;
			std::vector<SDRmFileRight> rights;
			SDR_WATERMARK_INFO watermarkinfo;
			SDR_Expiration expire;
			std::string tags;
			wstring destNXL_folder;
		};
		testData data;
		string rights_str;
		string wm_text;
		int Validity_option;
		string Validity_startDate;
		string Validity_endDate;
		string tags_csv;

#pragma endregion

		string UserFolder = common_testFolder + "\\" + common_email;
		createFolder(UserFolder);
		//string testFile;

		for each(vector<string> csv_line in csv_UserChangeRights)
		{
			data.rights.clear();
			data.watermarkinfo = {};
			data.tags.clear();
			if (csv_line[0] == "10")
			{
#pragma region[read data from csv]
				replace(csv_line[2].begin(), csv_line[2].end(), '<', ',');
				data.filepath = s2ws(csv_line[2]);
				rights_str = csv_line[3];
				if (rights_str != "null")
				{
					data.rights = Tick_Rights(rights_str);
				}
				wm_text = csv_line[4];
				if (wm_text != "null")
				{
					data.watermarkinfo = { wm_text,"font","color",9,0,ANTICLOCKWISE,true };
				}

				Validity_option = string2Int(csv_line[5]);
				Validity_startDate = csv_line[6];
				Validity_endDate = csv_line[7];
				IExpiryType t;
				switch (Validity_option) {
				case 1: {
					t = RELATIVEEXPIRE;
					data.expire.end = stringToUINT64(Validity_endDate);
					break;
				}
				case 2: {
					t = ABSOLUTEEXPIRE;
					data.expire.end = stringToUINT64(Validity_endDate);
					break;
				}
				case 3: {
					t = RANGEEXPIRE;
					data.expire.start = stringToUINT64(Validity_startDate);
					data.expire.end = stringToUINT64(Validity_endDate);
					break;
				}
				default:
					t = NEVEREXPIRE;
				}

				data.expire.type = t;

				tags_csv = csv_line[8];
				if (tags_csv == "null") tags_csv = "";
				data.tags = patchTagString(tags_csv);

				replace(csv_line[9].begin(), csv_line[9].end(), '<', ',');
				data.destNXL_folder = s2ws(csv_line[9]);
#pragma endregion
				vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

				string mid_p = puser->GetMembershipID(9);
				bool adhoc; int heartbeat; int sysprojectid; string sysprojecttenantid;
				res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid);
				string mid_s = puser->GetMembershipID(sysprojecttenantid);
				//vector<string> mids = { "null", mid_p, mid_s };
				//vector<string> mids = { "null" };
				vector<string> mids = { mid_p };
				//vector<string> mids = { mid_s };
				int i = 0;
				while (i < 1) {
					for each (string mid in mids)
					{
						wstring destNXL = data.destNXL_folder;
						if (mid == "null")
						{
							mid = "";
						}
						cout << "----------------------------" << endl;
						wcout << L"inputPath:" << data.filepath << endl;
						wcout << L"outputPath:" << destNXL << endl;
						//std::string tags = "{\"Country\":[\"\xE4\xB8\xAD\xE5\x9C\x8B\"],\"City\":[\"\xE6\x9D\xAD\xE5\xB7\x9E\"]}";
						std::string tags = u8"{\"Country\":[\"\u4E2D\u570B\"],\"City\":[\"\u676D\u5DDE\"]}";
						data.tags = tags;
						cout << endl << "ProtectFile with mid:[" << mid << "] ";
						res = puser->ProtectFile(data.filepath, destNXL, data.rights, data.watermarkinfo, data.expire, data.tags, mid);
						//res = puser->ProtectFile_tc(data.filepath, destNXL, r, data.watermarkinfo, data.expire, t, mid);
						checkSDWLResult(res);
						//Sleep(1500);


						EXPECT_EQ(0, res.GetCode());
						if (res)
						{
							Sleep(1000);
							if (mid == "")
							{
								cout << "UploadFile: ";
								res = puser->UploadFile(destNXL, destNXL);
								checkSDWLResult(res);
								EXPECT_EQ(0, res.GetCode());
		/*						ISDRmNXLFile* nxlfile;
								res = puser->OpenFile(destNXL, &nxlfile);
								cout << "GetNXLFileActivitylog:";
								res = puser->GetNXLFileActivityLog(nxlfile, 0, 20, RL_SFEmpty, "", RL_OBAccessTime, false);
								checkSDWLResult(res);
								EXPECT_EQ(0, res.GetCode());
								cout << "GetActivityInfo:";
								std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
								res = puser->GetActivityInfo(destNXL, infoVec);
								checkSDWLResult(res);
								EXPECT_EQ(0, res.GetCode());
								cout << "CloseFile: ";
								res = puser->CloseFile(nxlfile);
								checkSDWLResult(res);
								EXPECT_EQ(0, res.GetCode());*/

							}
							else if (mid == mid_p) {
								ISDRmNXLFile* nxlfile;
								res = puser->OpenFile(destNXL, &nxlfile);
								checkSDWLResult(res);
								string nxltag = nxlfile->GetTags();
								cout << "input tags           : " <<tags<<endl;
								cout << "Gettags from NXL file: " << nxltag << endl;
								EXPECT_STRCASEEQ(tags.c_str(), nxltag.c_str());
								cout << "UploadFile: ";
								//res = puser->UploadProjectFile(9, L"/002/", nxlfile, true);
								res = puser->UploadProjectFile(9, L"/002/", nxlfile);
								checkSDWLResult(res);
								EXPECT_EQ(0, res.GetCode());
								cout << "CloseFile: ";
								res = puser->CloseFile(nxlfile);
								checkSDWLResult(res);
								EXPECT_EQ(0, res.GetCode());
							}

							cout << "--testFile--" << endl;
							wcout << L"[" << destNXL << L"]" << endl;
							cout << "puser->GetRights:";
							rightsAndWatermarks.clear();
							res = puser->GetRights(destNXL, rightsAndWatermarks, false);
							checkSDWLResult(res);
							printRightsOnly(res, rightsAndWatermarks);

							cout << "-----------------   end  ---------------" << endl;
						}
						else
						{
							checkSDWLResult(res);
						}
					}
					i++;
				}
			}
		}
	}
	SDWLibDeleteRmcInstance(pInstance);
	cout << "=======Test END======" << endl;
}

TEST(V104, uploadFile)
{
#pragma region[init]
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant = NULL;
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res) {
		wcout << L"puser info: " << puser->GetEmail() << endl;
	
#pragma endregion
		vector<string> files;
		string path = "F:\\RPM\\Protect\\Offline\\";
		getAllFiles(path+"*", files);
		int i = 1;
		for each (string file in files) 
		{
			cout << "["<<i<<"]UploadFile: " << file << endl;
			wstring filePath = s2ws(path + file);
			res = puser->UploadFile(filePath, filePath);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			Sleep(1000);
			i++;
		}
	}
	SDWLibDeleteRmcInstance(pInstance);
	cout << "=======Test END======" << endl;
}

//V10.3 API for policy preview feature (CentralPolicy)
TEST(NOTRUN_V103, GetFileRightsFromCentralPolicies)
{
#pragma region[init]
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant;
	res = CreateSDRmcInstance_CreateClientID(s2ws(common_testFolder).c_str(), pInstance, common_clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);

	cout << "GetLoginUser(): ";
	res = pInstance->GetLoginUser(common_email, common_passcode, &puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res) {
		wcout << L"email: " << puser->GetEmail() << endl;

		cout << "GetHeartBeatInfo:";
		res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "GetListProjects:";
		res = puser->GetListProjects(1, 20, "lastActionTime", RF_All);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
#pragma endregion
#pragma region[TestData]
		struct testData {
			string caseName;
			std::string tenantname;
			uint32_t projectid;
			std::string tags;
			 
			SDR_WATERMARK_INFO watermarkinfo;
			SDR_Expiration expire;
		};
		testData d;

		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
#pragma endregion

		for each(vector<string> csv_line in csv_PolicyPreview)
		{
			rightsAndWatermarks.clear();

			if (csv_line[0] != "0")
			{
#pragma region[read data from csv]
				d.caseName = csv_line[1];
				d.tenantname = csv_line[2]; 
				d.projectid = string2Int(csv_line[3]);
				string tags_csv = csv_line[4];
				if (tags_csv == "null") tags_csv = "";
				d.tags = patchTagString(tags_csv);
#pragma endregion
				cout << endl<< "---------Case: " << d.caseName << "------------" << endl;
				cout << "Tags: " << d.tags << endl;
				if (d.tenantname != "null")
				{
					res = puser->GetFileRightsFromCentralPolicies(d.tenantname, d.tags, rightsAndWatermarks);
				}
				else
				{
					res = puser->GetFileRightsFromCentralPolicies(d.projectid, d.tags, rightsAndWatermarks);
				}
				checkSDWLResult(res);
				//printRightsOnly(res, rightsAndWatermarks);
				printEvaluationResultRights(res, rightsAndWatermarks);
			}
		}
	}

	SDWLibDeleteRmcInstance(pInstance);
	cout << "=======Test END======" << endl;
}

TEST(NOTRUN_V103, SDKLogout)
{
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant;
	res = CreateSDRmcInstance_CreateClientID(s2ws(common_testFolder).c_str(), pInstance, common_clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);

	cout << "GetLoginUser(): ";
	res = pInstance->GetLoginUser(common_email, common_passcode, &puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	cout << "LogoutUser: ";
	res = puser->LogoutUser();
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	cout << "RPMLogout: ";
	res = pInstance->RPMLogout();
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	SDWLibDeleteRmcInstance(pInstance);
	cout << "=======SDKLogout END======" << endl;
}

#define NUM_THREADS     200
struct Param {
	ISDRmUser *p_user;
	string testFile;
};
void *RPM_APIs_RMX(void *threadid)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	ISDRmHttpRequest *prequest = NULL;

	wstring app1 = L"C:\\auto\\Clone-rmcsdk-wiki2\\QA_TestCode\\x64\\Debug\\App1.exe";
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	unsigned int dirstatus;

	wstring RPMdir = L"F:\\includingRPM\\Safe1";

	Sleep(100);
	cout << "RPMGetCurrentLoggedInUser: ";
	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res) {
		wcout << L"puser info: " << puser->GetEmail() << endl;

//#define __RPM_more__
#if defined(__RPM_more__)
		cout << "SyncUserAttributes: ";
		res = pInstance->SyncUserAttributes();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << "SetRPMDeleteCacheToken: ";
		res = pInstance->SetRPMDeleteCacheToken();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << "RPMDeleteFileToken: ";
		res = pInstance->RPMDeleteFileToken(L"F:\\Tag-2019-04-04-13-22-26.txt.nxl");
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		bool ret = pInstance->IsRPMDriverExist();

		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(RPMdir);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << "IsRPMFolder: ";
		res = pInstance->IsRPMFolder(RPMdir, &dirstatus);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		
		wstring destpath = RPMdir;
		cout << "RPMEditCopyFile: ";
		res = pInstance->RPMEditCopyFile(L"F:\\Tag-2019-04-04-13-22-26.txt.nxl", destpath);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << "RPMEditSaveFile: ";
		res = pInstance->RPMEditSaveFile(destpath);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		for each(vector<string> csv_line in csv_filePath)
		{
			std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

			string filename;
			bool filestatus;
			if (csv_line[0] == "1") {
				filename = csv_line[1];
				cout << "RPMGetFileStatus for file: [" << filename << "]";
				res = pInstance->RPMGetFileStatus(s2ws(filename), &dirstatus, &filestatus);
				checkSDWLResult(res);

				cout << "dirstatus: " << dirstatus << " " << "filestatus" << filestatus << endl;


				cout << "RPMGetRights: ";
				res = pInstance->RPMGetRights(s2ws(filename), rightsAndWatermarks);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());


			}
		}

		////
		cout << "RemoveRPMDir: ";
		res = pInstance->RemoveRPMDir(RPMdir);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());


		cout << "RPMRegisterApp_ " << ws2s(app1);
		res = pInstance->RPMRegisterApp(app1, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << "RPMIsAppRegistered: ";
		bool reg;
		res = pInstance->RPMIsAppRegistered(app1, &reg);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());


		cout << "RPMUnregisterApp: ";
		res = pInstance->RPMUnregisterApp(app1, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());


		wstring SecretDir;
		cout << "RPMGetSecretDir: ";
		res = pInstance->RPMGetSecretDir(SecretDir);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
#endif
	}
	//SDWLibDeleteRmcInstance(pInstance);
	pthread_exit(NULL);

	return NULL;
}
void *RPM_APIs_upload(void *param)
{
	SDWLResult res;
	Param *data = (Param *)param;

	//string *f = (string *)file;
	ISDRmNXLFile* nxlfile;
	res = data->p_user->OpenFile(s2ws(data->testFile), &nxlfile);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (!res)
	{
		cout << "test is stop since openFile failed." << endl;
	}
	else {
		cout << "UploadFile: ";
		res = data->p_user->UploadProjectFile(9, L"/002/", nxlfile, true);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (!res)
		{
			string exportfilename = plan_path + "uploadfailed.txt";
			ofstream SaveFile(exportfilename, ios::app);
			SaveFile << data->testFile << endl;
			SaveFile.close();
		}
		else {
			cout << "Upload file " << data->testFile << " success!" << endl;
		}

		data->p_user->CloseFile(nxlfile);
	}


	pthread_exit(NULL);

	return NULL;
}

//multi-threads
TEST(Test_RPM, threads)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	ISDRmHttpRequest *prequest = NULL;

	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";


	Sleep(100);
	cout << "RPMGetCurrentLoggedInUser: ";
	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res) {
		wcout << L"puser info: " << puser->GetEmail() << endl;

		cout << "GetHeartBeatInfo:";
		res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "GetListProjects:";
		res = puser->GetListProjects(1, 20, "lastActionTime", RF_All);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		Sleep(1000);

		pthread_t threads[NUM_THREADS];
		int indexes[NUM_THREADS];
		int rc;
		int i;

		vector<string> files;
		string path = "E:\\ad613\\";
		getAllFiles(path + "*", files);

		for (i = 0; i < NUM_THREADS; i++) {
			cout << "main() : create thread, " << i << endl;
			indexes[i] = i;
			Sleep(100);

			//string* pTestFile = new string;
			//*pTestFile = path + files[i];
			//Param *param = new Param();
			//param->p_user = puser;
			//param->testFile = path + files[i];

			rc = pthread_create(&threads[i], NULL,
				RPM_APIs_RMX, (void *)&(indexes[i]));
			//rc = pthread_create(&threads[i], NULL,
			//	RPM_APIs_RMX, param);
			if (rc) {
				cout << "Error:cannot create thread," << rc << endl;
				exit(-1);
			}
		}

		//Sleep(10000);
		wcout << L"Press Enter key to do end..." << endl;
		getchar();
	}
	SDWLibDeleteRmcInstance(pInstance);
}

TEST(Test_RPM, all_100times)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	ISDRmHttpRequest *prequest = NULL;

	wstring app1 = L"C:\\auto\\Clone-rmcsdk-wiki2\\QA_TestCode\\x64\\Debug\\App1.exe";
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	unsigned int dirstatus;

	wstring RPMdir = L"F:\\includingRPM\\Safe1";

	for (int i = 0; i < 100; i++)
	{
		cout << endl << "-----------------------[" << i + 1 << "]" << endl;
		Sleep(100);
		cout << "RPMGetCurrentLoggedInUser: ";
		res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (res) {
			wcout << L"puser info: " << puser->GetEmail() << endl;

			//cout << "SyncUserAttributes: ";
			//res = pInstance->SyncUserAttributes();
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());

			////cout << "SetRPMCachedToken: ";
			////res = pInstance->SetRPMCachedToken();
			////checkSDWLResult(res);
			////EXPECT_EQ(0, res.GetCode());

			//cout << "SetRPMDeleteCacheToken: ";
			//res = pInstance->SetRPMDeleteCacheToken();
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());

			////cout << "CacheRPMFileToken: ";
			////res = pInstance->CacheRPMFileToken(L"F:\\Tag-2019-04-04-13-22-26.txt.nxl");
			////checkSDWLResult(res);
			////EXPECT_EQ(0, res.GetCode());

			//cout << "RPMDeleteFileToken: ";
			//res = pInstance->RPMDeleteFileToken(L"F:\\Tag-2019-04-04-13-22-26.txt.nxl");
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());

			//bool ret = pInstance->IsRPMDriverExist();

			//cout << "AddRPMDir: ";
			//res = pInstance->AddRPMDir(RPMdir);
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());

			//cout << "IsRPMFolder: ";
			//res = pInstance->IsRPMFolder(RPMdir, &dirstatus);
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());

			//wstring destpath = RPMdir;
			//cout << "RPMEditCopyFile: ";
			//res = pInstance->RPMEditCopyFile(L"F:\\Tag-2019-04-04-13-22-26.txt.nxl", destpath);
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());

			//cout << "RPMEditSaveFile: ";
			//res = pInstance->RPMEditSaveFile(destpath);
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());

			//for each(vector<string> csv_line in csv_filePath)
			//{
			//	std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

			//	string filename;
			//	bool filestatus;
			//	if (csv_line[0] == "1") {
			//		filename = csv_line[1];
			//		cout << "RPMGetFileStatus for file: [" << filename << "]";
			//		res = pInstance->RPMGetFileStatus(s2ws(filename), &dirstatus, &filestatus);
			//		checkSDWLResult(res);

			//		cout << "dirstatus: " << dirstatus << " " << "filestatus" << filestatus << endl;


			//		cout << "RPMGetRights: ";
			//		res = pInstance->RPMGetRights(s2ws(filename), rightsAndWatermarks);
			//		checkSDWLResult(res);
			//		EXPECT_EQ(0, res.GetCode());


			//	}
			//}



			//////
			//cout << "RemoveRPMDir: ";
			//res = pInstance->RemoveRPMDir(RPMdir);
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());


			//cout << "RPMRegisterApp_ " << ws2s(app1);
			//res = pInstance->RPMRegisterApp(app1, secr);
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());

			//cout << "RPMIsAppRegistered: ";
			//bool reg;
			//res = pInstance->RPMIsAppRegistered(app1, &reg);
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());


			//cout << "RPMUnregisterApp: ";
			//res = pInstance->RPMUnregisterApp(app1, secr);
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());


			//wstring SecretDir;
			//cout << "RPMGetSecretDir: ";
			//res = pInstance->RPMGetSecretDir(SecretDir);
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());

		}
		SDWLibDeleteRmcInstance(pInstance);
	}
}


TEST(TC, WindowsAPIvsDriver)
{
	try {
#pragma region[Init: getUser, trust exe and open NXL file]
		ISDRmcInstance * pInstance;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		ISDRmTenant *pTenant = NULL;
		ISDRmHttpRequest *prequest = NULL;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		wstring app1 = L"C:\\auto\\Clone-rmcsdk-wiki2\\QA_TestCode\\x64\\Debug\\RMD-WindowsAPI-Gtest.exe";

		res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (res) {
			wcout << L"puser info: " << puser->GetEmail() << endl;
		}

		res = pInstance->RPMRegisterApp(app1, secr);

		cout << "RPMNotifyRMXStatus: ";
		res = pInstance->RPMNotifyRMXStatus(true, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		string file = "F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt";
		/*cout << "Input test file path: ";
		cin >> file;*/
		int charNum = 35;
		cout << "Read file>>>";
		ifstream infile;
		infile.open(file.data());
		string content = "";

		if (infile.is_open()) {

			infile >> noskipws;
			int i = 0;
			char c;

			while (!infile.eof() && i < charNum)
			{
				i++;
				infile >> c;
				content.push_back(c);
			}

			infile.close();
			cout << content << endl;
		}
		else
		{
			cout << "[ERROR]: the test file cannot open, please check." << endl;
		}
#pragma endregion
		writecode4CreateFile();

//#define __test__debug__winAPI
#if defined(__test__debug__winAPI)
		string file="F:\\RPM\\mark\\4\\7513\\2\\demo.prt";
		/*cout << "Input test file path: ";
		cin >> file;*/
		int charNum = 35;
		cout << "Read file>>>";
		ifstream infile;
		infile.open(file.data());
		string content = "";

		if (infile.is_open()) {

			infile >> noskipws;
			int i = 0;
			char c;

			while (!infile.eof() && i < charNum)
			{
				i++;
				infile >> c;
				content.push_back(c);
			}

			infile.close();
			cout << content << endl;
		}
		else
		{
			cout << "[ERROR]: the test file cannot open, please check." << endl;
		}
#pragma endregion

		wstring testfile = s2ws(file);
		wstring destfile = L"F:\\RPM\\mark\\4\\7513\\2\\tcad161041A91pn5";
		//CreateFile
		HANDLE hFile = CreateFile(testfile.c_str(),                // name of the write
			GENERIC_WRITE,
			FILE_SHARE_WRITE | FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			cout << "CreateFile have error." << endl;
		}
		else cout << "CreateFile -ok." << endl;
		CloseHandle(hFile);

		//OpenFile
		//HFILE hOpen = OpenFile();

		//MoveFile
		BOOL bMoveEx = _wrename(testfile.c_str(), destfile.c_str());
		//BOOL bMoveEx = MoveFileEx(testfile.c_str(), destfile.c_str(), MOVEFILE_REPLACE_EXISTING);
		if (bMoveEx) cout << "MoveFileEx -ok."<< endl;
		else cout << "MoveFileEx have error"  << endl;
		//Sleep(3000);
		wstring file2 = L"F:\\RPM\\mark\\4\\7513\\2\\tcad161041A91pn4.prt";
		bMoveEx = _wrename(file2.c_str(), testfile.c_str());
		//bMoveEx = MoveFileEx(file2.c_str(), testfile.c_str(), MOVEFILE_REPLACE_EXISTING);
		if (bMoveEx) cout << "MoveFileEx -ok." << endl;
		else cout << "MoveFileEx have error" << endl;
		//BOOL bMove= MoveFile(testfile.c_str(), destfile.c_str() );
		//if (bMove) cout << "MoveFile have error" << endl;
		//else cout << "MoveFile -ok." << endl;

		_wremove(destfile.c_str());

		//CreateFile
		hFile = CreateFile(testfile.c_str(),                // name of the write
			GENERIC_WRITE,
			FILE_SHARE_WRITE | FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			cout << "CreateFile have error." << endl;
		}
		else cout << "CreateFile -ok." << endl;
		CloseHandle(hFile);
#endif

		//DeleteFile
		//BOOL bDelete = DeleteFile(testfile.c_str());
#define __test__debug__winAPI_copyCode
#if defined(__test__debug__winAPI_copyCode)
		HANDLE hFile;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt",                // name of the write
			GENERIC_WRITE,
			FILE_SHARE_WRITE | FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			cout << "CreateFile have error." << endl;
		}
		else cout << "CreateFile -ok." << endl;
		CloseHandle(hFile);
		cout << endl << "--------CreateFile - w01 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_READ, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w02 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w03 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_EXECUTE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w04 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_ALL, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w05 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w06 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w07 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w08 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w09 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w10 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w11 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w12 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w13 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w14 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_READONLY, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w15 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w16 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_SYSTEM, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w17 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_DIRECTORY, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w18 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w19 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_DEVICE, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w20 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w21 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w22 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_SPARSE_FILE, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w23 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_REPARSE_POINT, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w24 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_COMPRESSED, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w25 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_OFFLINE, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w26 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NOT_CONTENT_INDEXED, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w27 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ENCRYPTED, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w28 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_INTEGRITY_STREAM, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w29 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_VIRTUAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w30 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NO_SCRUB_DATA, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w31 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_EA, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w32 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_PINNED, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w33 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_UNPINNED, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w34 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_RECALL_ON_OPEN, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w35 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w36 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w37 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w38 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_NO_BUFFERING, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w39 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_RANDOM_ACCESS, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w40 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w41 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w42 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_BACKUP_SEMANTICS, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w43 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_POSIX_SEMANTICS, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w44 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_SESSION_AWARE, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w45 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_OPEN_REPARSE_POINT, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w46 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_OPEN_NO_RECALL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w47 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_FIRST_PIPE_INSTANCE, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w48 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, SECURITY_ANONYMOUS, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w49 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, SECURITY_IDENTIFICATION, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w50 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, SECURITY_IMPERSONATION, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w51 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, SECURITY_DELEGATION, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w52 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, SECURITY_CONTEXT_TRACKING, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w53 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, SECURITY_EFFECTIVE_ONLY, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w54 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, SECURITY_SQOS_PRESENT, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile); cout << endl << "--------CreateFile - w55 ----------" << endl;
		hFile = CreateFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-04.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, SECURITY_VALID_SQOS_FLAGS, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "CreateFile have error." << endl; }
		else { cout << "CreateFile -ok." << endl; }
		CloseHandle(hFile);
#endif
		//////////////////////
		//RenameFile

		//SetFileAttributes

		SDWLibDeleteRmcInstance(pInstance);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(RPM, logoutIssue)
{
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	string testFolder = TestDirectory + "logoutIssue";
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
	string email = "Lifi.yuan@nextlabs.com";
	string password = "123blue!";

	md5(password);
	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);
	string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
	GetUserLoginResponseData(email, password, clientID, user_return);

	cout << "Set SDK LoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	if (res) {
		wstring app1 = L"C:\\auto\\Clone-rmcsdk-wiki2\\QA_TestCode\\x64\\Debug\\RMD-WindowsAPI-Gtest.exe";
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

		LOG(INFO) << "RPMRegisterApp_ " << ws2s(app1);
		res = pInstance->RPMRegisterApp(app1, secr);
		checkResult(res, 0);

		cout << "SetRPMLoginResult: ";
		DWORD start = ::GetTickCount();
		res = pInstance->SetRPMLoginResult(ReadFromFile(user_return), secr);
		DWORD Etime = ::GetTickCount() - start;
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "  Evaluation time: " << Etime << " (ms)" << endl;

		cout << "RPMNotifyRMXStatus: ";
		res = pInstance->RPMNotifyRMXStatus(true, secr);
		checkSDWLResult(res);

		//unsigned long pid;
		//cout << "input pid for Add:";
		//cin >> pid;
		//res = pInstance->RPMAddTrustedProcess(pid, secr);
		//cout << "Add " << pid << " result: ";
		//checkSDWLResult(res);
		string file;
		file = "C:\\Users\\Administrator\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate\\code2-2019-07-29-07-04-10.txt";
		/*cout << "Input test file path: ";
		cin >> file;*/
		int charNum = 35;
		cout << "Read file>>>";
		ifstream infile;
		infile.open(file.data());
		string content = "";

		if (infile.is_open()) {

			infile >> noskipws;
			int i = 0;
			char c;

			while (!infile.eof() && i < charNum)
			{
				i++;
				infile >> c;
				content.push_back(c);
			}

			infile.close();
			cout << content << endl;
		}
		else
		{
			cout << "[ERROR]: the test file cannot open, please check." << endl;
		}

		cout << endl << "SDK user logout:";
		res = puser->LogoutUser();
		checkSDWLResult(res);

		cout << "RPMLogout: ";
		DWORD start_logout = ::GetTickCount();
		res = pInstance->RPMLogout();
		Etime = ::GetTickCount() - start_logout;
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "  Evaluation time: " << Etime << " (ms)" << endl;

		res = pInstance->Save();
		EXPECT_EQ(0, res.GetCode());
		SDWLibDeleteRmcInstance(pInstance);
	}
}

string passcode = "";
string email_secr = "Lifi.yuan@nextlabs.com";
TEST(Security, login)
{
	string testFolder = "C:\\SDK test\\TrustApp_Central";

	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
	//string email = "testB@nextlabs.com";
	//string email = "Lifi.yuan@nextlabs.com";
	string password = "123blue!";
	//string passcode;
	md5(password);
	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);
	string user_return = testFolder + "\\User_" + email_secr + "_RMSreturn.json";
	GetUserLoginResponseData(email_secr, password, clientID, user_return);

	cout << "Set SDK LoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	if (res) {
		passcode = puser->GetPasscode();
		cout << "User's passcode:[" << passcode << "]" << endl;
		//cout << "GetHeartBeatInfo: ";
		//SDWLResult res = puser->GetHeartBeatInfo();
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(ReadFromFile(user_return), secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		string userdata;
		res = pInstance->GetLoginData(email_secr, passcode, userdata);
	}

	res = pInstance->Save();

	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}
TEST(Security, withoutlogin)
{
#pragma region[init]
	string testFolder = "C:\\SDK test\\TrustApp_Central";

	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant;
	ISDRmHttpRequest *prequest;
	string clientID = "";
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

	res = CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	//res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	waitInstanceInitFinish(pInstance);
#pragma endregion

	//wstring dir = L"F:\\RPM\\mark\\1";
	//res = pInstance->AddRPMDir(dir);
	//res = pInstance->RemoveRPMDir(dir);

	string nv, nurl, nsum;
	res = pInstance->CheckSoftwareUpdate(nv, nurl, nsum);

	res = pInstance->GetCurrentTenant(&pTenant);

	string userdata;
	//res = pInstance->GetLoginUser(email_secr, passcode, &puser);

	/*string pcode = puser->GetPasscode();
	string em = ws2s(puser->GetEmail());*/
	//res= pInstance->GetLoginData(ws2s(puser->GetEmail()), puser->GetPasscode(), userdata);
	res = pInstance->GetLoginData("472049994@qq.com", passcode, userdata);
	res = pInstance->GetLoginData(email_secr, "12345667785678", userdata);
	res = pInstance->GetLoginData(email_secr, passcode, userdata);

	res = pInstance->GetLoginRequest(&prequest);

	res = pInstance->Save();



	SDWLibDeleteRmcInstance(pInstance);
	cout << "=======Test END======" << endl;
}

