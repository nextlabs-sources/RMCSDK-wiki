#include "stdafx.h"
#include "SDLInc.h"

void waitInstanceInitFinish(ISDRmcInstance *&pInstance)
{
	SDWLResult res;
	bool finished = false;
	int i = 0;
	do {
		i++;
		Sleep(1000);
		res = pInstance->IsInitFinished(finished);
		LOG(INFO) << i << ": Wait RMCInstance Class finish initialization...";
		LOG(INFO) <<" >res: ";
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (i == 20)
		{
			LOG(INFO) << i << "Cannot connect to PDPMan" << endl;
			break;
		}
	} while (!finished);

}

void printPolicy(ISDRmcInstance *pInstance, ISDRmUser *&puser)
{
	SDWLResult res;
	ISDRmTenant *pTenant;

	cout << "GetCurrentTenant: ";
	res = pInstance->GetCurrentTenant(&pTenant);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res) {
		cout << "---print Default tenant policy---" << endl;

		std::string policybundle;
		bool bval = false;
		bval = puser->GetPolicyBundle(pTenant->GetTenant(), policybundle);
		//if (bval)
		//	cout << policybundle << endl;
		//else
		//	cout << "ERROR: GetPolicyBundle failed." << endl;
	}
}

void printPolicyBaseOnTenant(wstring & tenantName, ISDRmUser *&puser)
{
	cout << "---printPolicyBaseOnTenant---" << endl;
	std::string policybundle;
	bool bval = false;
	bval = puser->GetPolicyBundle(tenantName, policybundle);
	if (bval) {
		wcout << "tenantName: " << tenantName;
		cout << " policybundle: " << endl << policybundle << endl;
	}
	else
		cout << "ERROR: GetPolicyBundle failed." << endl;
}

void printRightsOnly(const SDWLResult res, const vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks)
{
	if (res) {
		cout << "--Evaluation Result--" << endl;
		for (const auto& rightAndWatermarks : rightsAndWatermarks) {
			wcout << L"\t0x" << hex << setfill(L'0') << setw(8) << rightAndWatermarks.first << endl;
			/*if (!rightAndWatermarks.second.empty()) {
				for (const auto& watermark : rightAndWatermarks.second) {
					cout << "\t\tWatermark:" << endl;
					cout << "\t\t\ttext: " << watermark.text << endl;
					cout << "\t\t\tfontName: " << watermark.fontName << endl;
					cout << "\t\t\tfontColor: " << watermark.fontColor << endl;
					cout << "\t\t\tfontSize: " << watermark.fontSize << endl;
					cout << "\t\t\ttransparency: " << watermark.transparency << endl;
					cout << "\t\t\trotation: " << watermark.rotation << endl;
					cout << "\t\t\trepeat: " << watermark.repeat << endl;
				}
			}*/
		}
		cout << "--End Evaluation Result--" << endl;
	}
}

void printEvaluationResultRights(const SDWLResult res, const vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks)
{
	if (res) {
		cout << "--Evaluation Result--" << endl;
		for (const auto& rightAndWatermarks : rightsAndWatermarks) {
			wcout << L"\t0x" << hex << setfill(L'0') << setw(8) << rightAndWatermarks.first << endl;
			if (!rightAndWatermarks.second.empty()) {
				for (const auto& watermark : rightAndWatermarks.second) {
					cout << "\t\tWatermark:" << endl;
					cout << "\t\t\ttext: " << watermark.text << endl;
					cout << "\t\t\tfontName: " << watermark.fontName << endl;
					cout << "\t\t\tfontColor: " << watermark.fontColor << endl;
					cout << "\t\t\tfontSize: " << watermark.fontSize << endl;
					cout << "\t\t\ttransparency: " << watermark.transparency << endl;
					cout << "\t\t\trotation: " << watermark.rotation << endl;
					cout << "\t\t\trepeat: " << watermark.repeat << endl;
				}
			}
		}
		cout << "--End Evaluation Result--" << endl;	
	}
}

void printEvaluationResult_rightsAndObligations(const SDWLResult res, const std::vector<std::pair<SDRmFileRight, std::vector<SDR_OBLIGATION_INFO>>>rightsAndObligations)
{
	if (res) {
		cout << "--Evaluation Result--" << endl;
		wcout << L"Rights:" << endl;
		for (const auto& rightAndObligations : rightsAndObligations) {
			wcout << L"\t0x" << hex << setfill(L'0') << setw(8) << rightAndObligations.first << endl;
			if (!rightAndObligations.second.empty()) {
				for (const auto& obligation : rightAndObligations.second) {
					wcout << L"\t\tObligation: " << obligation.name << endl;
					for (const auto& op : obligation.options) {
						wcout << L"\t\t\t" << op.first << L": " << op.second << endl;
					}
				}
			}
		}
		cout << "--End Evaluation Result--" << endl;
	}
}

class policyenv :public::testing::TestWithParam<vector<string>> {};
TEST_P(policyenv, ValidInput)
{
	vector<string> csv = GetParam();
	string IsRun = csv[0];
	if (IsRun == "run" || IsRun == "1")
	{
		string caseID = csv[1];
		string email = csv[2];
		string password = csv[3];
		string plain_file = csv[4];
		string tags_csv = csv[5];
		string PCpath = csv[6];
		string email2 = csv[7];
		string password2 = csv[8];
		string memberid = csv[9];
		if (memberid == "default"){	memberid = "";}

		string tags = patchTagString(tags_csv);

		string clientID = "";
		string testFolder = TestDirectory + caseID;
		createFolder(testFolder);
		string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
		string user_return2 = testFolder + "\\User_" + email2 + "_RMSreturn.json";
		wstring localfile_pathw = plan_file_path + s2ws(plain_file);
		md5(password);
		md5(password2);

		ISDRmcInstance * pInstance;
		ISDRmUser *puser = NULL;
		ISDRmUser *puser2 = NULL;
		SDWLResult res;
		ISDRmTenant *pTenant;
		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;


		res = CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT,s2ws(PCpath).c_str());
		
		waitInstanceInitFinish(pInstance);

		GetUserLoginResponseData(email, password, clientID, user_return);

		cout << "SetLoginResult: ";
		res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		if (res) {
			
			SDR_Expiration expire;
			SDR_WATERMARK_INFO watermarkinfo;
			expire.type = IExpiryType::NEVEREXPIRE;
			std::vector<SDRmFileRight> rights;
			wstring newcreatedfilePath = L"";

			cout << "ProtectFile: ";
			res = puser->ProtectFile(localfile_pathw, newcreatedfilePath, rights, watermarkinfo, expire, tags, memberid);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			////for get lastCreatedNXLFile
			//string uid= getUserIDFromJson(user_return);			
			//string path = testFolder + "\\working\\" + ws2s(DEFUALT_TENANT) + "\\" + uid;
			//string lastCreatedNXLFile = "";
			//getLastCreatedNXL(path, lastCreatedNXLFile);
#pragma region[Upload NXL file]
			//ISDRmNXLFile* nxlfile;
			//res = puser->OpenFile(newcreatedfilePath, &nxlfile);
			cout << "UploadFile: ";
			res = puser->UploadFile(newcreatedfilePath, localfile_pathw);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
#pragma endregion

			//Test start//
			wcout << L"---------------------Test--- User: " << puser->GetEmail() << endl;
			cout << "GetHeartBeatInfo: ";
			res = puser->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			printPolicy(pInstance, puser);

			cout << "GetFileRightsFromCentralPolicies: ";
			res = puser->GetFileRightsFromCentralPolicies(newcreatedfilePath, rightsAndWatermarks);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			printEvaluationResultRights(res, rightsAndWatermarks);

			pInstance->Save();
			cout << "LogoutUser: ";
			res = puser->LogoutUser();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			//Login with user2
			GetUserLoginResponseData(email2, password2, clientID, user_return2);
			cout << "SetLoginResult(user2): ";
			res = pInstance->SetLoginResult(ReadFromFile(user_return2), &puser2, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			wcout << L"---------------------Test--- User: " << puser2->GetEmail() << endl;
			cout << "GetHeartBeatInfo(user2): ";
			res = puser2->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			cout << "GetFileRightsFromCentralPolicies(user2): ";
			rightsAndWatermarks.clear();
			res = puser2->GetFileRightsFromCentralPolicies(newcreatedfilePath, rightsAndWatermarks);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			printEvaluationResultRights(res, rightsAndWatermarks);

			//Delete test file
			pInstance->Save();
			cout << "LogoutUser: ";
			res = puser2->LogoutUser();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
#pragma region[Delete test file]
			//GetUserLoginResponseData(email, password, clientID, user_return);
			//res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());
			//bool deleteFile = pFile->RemoveFile(nxlfile);
			//EXPECT_TRUE(deleteFile);

			//string err = GetLastErrorAsString();
#pragma endregion
		}

		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl;
		/*wcout << L"Press any key to quit...";
		_getch();*/
	}
}
INSTANTIATE_TEST_CASE_P(JPCtest, policyenv, testing::ValuesIn(csv_JPC));

class policyenv2 :public::testing::TestWithParam<vector<string>> {};
TEST_P(policyenv2, specialInput)
{
	vector<string> csv = GetParam();
	string IsRun = csv[0];
	if (IsRun == "run" || IsRun == "1")
	{
#pragma region[test data env]
		ISDRmcInstance * pInstance;
		ISDRmcInstance * pInstance_A2;
		ISDRmcInstance * pInstance_B;
		ISDRmUser *puser_A1 = NULL;
		ISDRmUser *puser_A2 = NULL;
		ISDRmUser *puser_A2_new = NULL;
		ISDRmUser *puser_B = NULL;
		SDWLResult res;
		ISDRmTenant *pTenant;

		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

		std::string tags = "";
		SDR_Expiration expire;
		SDR_WATERMARK_INFO watermarkinfo;
		expire.type = IExpiryType::NEVEREXPIRE;
		std::vector<SDRmFileRight> rights;

		string caseID = csv[1];
		string email_A1 = csv[2];
		string password_A1 = csv[3];
		string email_A2 = csv[4];
		string password_A2 = csv[5];
		string plain_file = csv[6];
		int protectType = string2Int(csv[7]);
		string email_B = csv[8];
		string password_B = csv[9];
		string PCpath = csv[10];

		switch (protectType) {
		case 1://Tag
		{
			tags = "{\"itar\":[\"TAA-01\"],\"QA\":[\"Lifi\",\"Hello\"]}";
			break;
		}
		case 2://AD-hoc
		{
			string rights_str = "view|edit";
			rights = Tick_Rights(rights_str);
			break;
		}
		default://AD-hoc + TAg
		{
			tags = "{\"itar\":[\"TAA-01\"],\"QA\":[\"Lifi\",\"Hello\"]}";
		/*	tags = "{\"itar\":[\"TAA-01\"]}";*/
			string rights_str = "view|edit";
			rights = Tick_Rights(rights_str);
			break;
		}
		}

		string clientID = "";
		string testFolder = TestDirectory + caseID;
		createFolder(testFolder);
		string user_return_A1 = testFolder + "\\User_" + email_A1 + "_RMSreturn.json";
		string user_return_A2 = testFolder + "\\User_" + email_A2 + "_RMSreturn.json";
		string user_return_B = testFolder + "\\User_" + email_B + "_RMSreturn.json";
		md5(password_A1);
		md5(password_A2);
		md5(password_B);
		wstring localfile_pathw = plan_file_path + s2ws(plain_file);
#pragma endregion

		const wstring ROUTER_A = DEFUALT_ROUTER;
		const wstring TENANT_A = DEFUALT_TENANT;
		string server_A = "rms-centos7303";
		const wstring ROUTER_B = L"https://rmtest.nextlabs.solutions";
		const wstring TENANT_B = L"skydrm.com";
		string server_B = "rmtest";
		wstring newcreatedfilePath = L"";
		res = CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, ROUTER_A, TENANT_A, s2ws(PCpath).c_str());
		waitInstanceInitFinish(pInstance);

#pragma region[prepare test file]
		GetUserLoginResponseData_InputServer(email_A1, password_A1, clientID, user_return_A1, server_A);
		cout << "SetLoginResult: ";
		res = pInstance->SetLoginResult(ReadFromFile(user_return_A1), &puser_A1, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (res) {
			cout << "ProtectFile: ";
			res = puser_A1->ProtectFile(localfile_pathw, newcreatedfilePath, rights, watermarkinfo, expire, tags);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			//for get lastCreatedNXLFile
			string uid = getUserIDFromJson(user_return_A1);
			string path = testFolder + "\\working\\" + ws2s(TENANT_A) + "\\" + uid;
			string lastCreatedNXLFile = "";
			getLastCreatedNXL(path, lastCreatedNXLFile);
#pragma region[Upload NXL file]
			ISDRmNXLFile* nxlfile;
			res = puser_A1->OpenFile(newcreatedfilePath, &nxlfile);
			cout << "UploadFile: ";
			res = puser_A1->UploadFile(newcreatedfilePath, newcreatedfilePath);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			cout << "AD-hoc- GetRights():";
			std::vector<SDRmFileRight> rights_nxl = nxlfile->GetRights();
			for each(SDRmFileRight r in rights_nxl)
			{
				cout << " " << r;
			}
			cout << endl;

#pragma endregion
#pragma endregion
			//Test start//
#pragma region[User: File Owner]
			wcout << L"---------------------Test--- User: " << puser_A1->GetEmail() << endl;
			cout << "GetHeartBeatInfo: ";
			res = puser_A1->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			printPolicy(pInstance, puser_A1);

			cout << "GetFileRightsFromCentralPolicies(Owner): ";
			res = puser_A1->GetFileRightsFromCentralPolicies(s2ws(lastCreatedNXLFile), rightsAndWatermarks);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			printEvaluationResultRights(res, rightsAndWatermarks);
#pragma endregion
			pInstance->Save();
			cout << "LogoutUser: ";
			res = puser_A1->LogoutUser();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

#pragma region[User: other user with same Instance]
			GetUserLoginResponseData_InputServer(email_A2, password_A2, clientID, user_return_A2, server_A);
			cout << "SetLoginResult- other user with same Instance: ";
			res = pInstance->SetLoginResult(ReadFromFile(user_return_A2), &puser_A2, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			wcout << L"---------------------Test--- User(with same Instance): " << puser_A2->GetEmail() << endl;
			cout << "GetHeartBeatInfo: ";
			res = puser_A2->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			cout << "GetFileRightsFromCentralPolicies: ";
			rightsAndWatermarks.clear();
			res = puser_A2->GetFileRightsFromCentralPolicies(s2ws(lastCreatedNXLFile), rightsAndWatermarks);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			printEvaluationResultRights(res, rightsAndWatermarks);
#pragma endregion

			SDWLibDeleteRmcInstance(pInstance);

#pragma region[User: Same tenant with new pInstance]
			res = CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance_A2, clientID, ROUTER_A, TENANT_A, s2ws(PCpath).c_str());
			waitInstanceInitFinish(pInstance_A2);

			cout << "SetLoginResult: ";
			res = pInstance_A2->SetLoginResult(ReadFromFile(user_return_A2), &puser_A2_new, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			wcout << L"---------------------Test--- User(new instance): " << puser_A2_new->GetEmail() << endl;
			cout << "GetHeartBeatInfo: ";
			res = puser_A2_new->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			cout << "GetFileRightsFromCentralPolicies: ";
			rightsAndWatermarks.clear();
			res = puser_A2_new->GetFileRightsFromCentralPolicies(s2ws(lastCreatedNXLFile), rightsAndWatermarks);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			printEvaluationResultRights(res, rightsAndWatermarks);
#pragma endregion

			SDWLibDeleteRmcInstance(pInstance_A2);

#pragma region[User: Other tenant]
			cout << "Login the other tenant user: " << endl;
			testFolder = testFolder + "\\otherTenant";
			createFolder(testFolder);
			res = CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance_B, clientID, ROUTER_B, TENANT_B, s2ws(PCpath).c_str());
			waitInstanceInitFinish(pInstance_B);

			GetUserLoginResponseData_InputServer(email_B, password_B, clientID, user_return_B, server_B);
			cout << "SetLoginResult: ";
			res = pInstance_B->SetLoginResult(ReadFromFile(user_return_B), &puser_B, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (res) {

				wcout << L"---------------------Test--- User: " << puser_B->GetEmail() << endl;
				cout << "GetHeartBeatInfo: ";
				res = puser_B->GetHeartBeatInfo();
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());

				cout << "GetFileRightsFromCentralPolicies: ";
				rightsAndWatermarks.clear();
				res = puser_B->GetFileRightsFromCentralPolicies(s2ws(lastCreatedNXLFile), rightsAndWatermarks);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());
				printEvaluationResultRights(res, rightsAndWatermarks);
			}
			cout << "--------------" << endl;
			SDWLibDeleteRmcInstance(pInstance_B);
#pragma endregion
			////Delete test file
			//bool deleteFile = pFile->RemoveFile(nxlfile);
			//EXPECT_TRUE(deleteFile);
		}
		else
		{
			SDWLibDeleteRmcInstance(pInstance);
		}
		cout << "-------------This case finished--------" << endl;
		/*wcout << L"Press any key to quit...";
		_getch();*/
	}
}
INSTANTIATE_TEST_CASE_P(JPCtest, policyenv2, testing::ValuesIn(csv_JPC_specialInput));

#define test_JPCinit
//#define test_ViaShare
#define test_InputPath

#if defined(test_JPCinit)
string jpc_clientID = "";
//string jpc_PCpath = "C:\\SDK test";
string jpc_PCpath = "C:\\Program Files\\NextLabs";
//string jpc_testFolder = TestDirectory + "JPCinit";
string jpc_testFolder = TestDirectory + "RPMtest";
//string jpc_email = "lifi.yuan@nextlabs.com";
string jpc_email = "472049994@qq.com";
string jpc_passcode = "";
string jpc_user_return = jpc_testFolder + "\\User_" + jpc_email + "_RMSreturn.json";
SDWLResult jpc_loginres;
TEST(JPCtest, init)
{
	string password = "123blue!";
	md5(password);

	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	
	createFolder(jpc_testFolder);

	jpc_loginres = CreateSDRmcInstance_CreateClientID(s2ws(jpc_testFolder).c_str(), pInstance, jpc_clientID, DEFUALT_ROUTER, DEFUALT_TENANT, s2ws(jpc_PCpath).c_str());
	GetUserLoginResponseData(jpc_email, password, jpc_clientID, jpc_user_return);

	cout << "SetLoginResult: ";
	jpc_loginres = pInstance->SetLoginResult(ReadFromFile(jpc_user_return), &puser, secr);
	checkSDWLResult(jpc_loginres);
	EXPECT_EQ(0, jpc_loginres.GetCode());
	if (jpc_loginres)
	{
		cout << "get passcode. "<<endl;
		jpc_passcode = puser->GetPasscode();
	}
	cout << "GetHeartBeatInfo: ";
	SDWLResult res = puser->GetHeartBeatInfo();
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	SDWLibDeleteRmcInstance(pInstance);
}
#endif

#if defined(test_InputPath)
class pathinput :public::testing::TestWithParam<vector<string>> {};
TEST_P(pathinput, pathFromCSV)
{
	vector<string> csv = GetParam();
	string IsRun = csv[0];
	if ((IsRun == "run" || IsRun == "1")&&(jpc_loginres))
	{
		string filePath = csv[1];
		int resultcode = string2Int(csv[2]);
		
		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		SDWLResult res;
		ISDRmcInstance * pInstance;
		ISDRmUser *puser = NULL;

		//res = CreateSDRmcInstance_CreateClientID(s2ws(jpc_testFolder).c_str(), pInstance, jpc_clientID, DEFUALT_ROUTER, DEFUALT_TENANT, s2ws(jpc_PCpath).c_str());
		res = CreateSDRmcInstance_CreateClientID(s2ws(jpc_testFolder).c_str(), pInstance, jpc_clientID, DEFUALT_ROUTER, DEFUALT_TENANT);

		waitInstanceInitFinish(pInstance);

		cout << "GetLoginUser(): ";
		res = pInstance->GetLoginUser(jpc_email,jpc_passcode, &puser);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		if (res) {
			//cout << "GetHeartBeatInfo: ";
			//res = puser->GetHeartBeatInfo();
			//checkSDWLResult(res);
			//EXPECT_EQ(0, res.GetCode());
			printPolicy(pInstance, puser);

#pragma region[test GetPolicyBundle() with input tenantName]
			wstring tenantName = L"t-ad61b8f4496f4d1ca271a9f3f7761948";
			wcout << L"test GetPolicyBundle() with input: [" << tenantName << L"]" << endl;
			printPolicyBaseOnTenant(tenantName, puser);
#pragma endregion

			cout << "---test start---" << endl;
			ISDRmNXLFile * nfile = NULL;
			cout << "OpenFile: ";
			res = puser->OpenFile(s2ws(filePath), &nfile);
			checkSDWLResult(res);
			bool nfileOpened = false;
			if (res) {
				nfileOpened = true;

				cout << "file's tenantname: " << nfile->GetTenantName() << endl;
				ISDRmTenant *pTenant;
				res = pInstance->GetCurrentTenant(&pTenant);
				wcout << L"Default tenantName:" << pTenant->GetTenant() << endl;
				cout << "fileTags:" << endl << nfile->GetTags() << endl;
			}

			cout << "Input filePath: [" << filePath << "]" << endl << "-GetFileRightsFromCentralPolicies: ";
			rightsAndWatermarks.clear();
			//time_t timeBegin, timeEnd;
			//timeBegin = time(NULL);
			DWORD start = ::GetTickCount();
			res = puser->GetFileRightsFromCentralPolicies(s2ws(filePath), rightsAndWatermarks);
			DWORD Etime = ::GetTickCount() - start;
			//timeEnd = time(NULL);
			checkSDWLResult(res);
			printEvaluationResultRights(res, rightsAndWatermarks);
			
			cout << "Evaluation time: "<< Etime<< " (ms)" << endl;
			//printf("=== Evaluation time: %d ===\n", timeEnd - timeBegin);
			string exportfilename = plan_path + "JPC_EvaluationTime.txt";
			ofstream SaveFile(exportfilename, ios::app);
			//SaveFile << timeEnd - timeBegin;
			SaveFile << Etime << endl;
			SaveFile.close();

			EXPECT_EQ(resultcode, res.GetCode());

			cout << "============1=========" << endl;
			cout << "puser->GetRights():";
			rightsAndWatermarks.clear();
			res = puser->GetRights(s2ws(filePath), rightsAndWatermarks);
			checkSDWLResult(res);
			printEvaluationResultRights(res, rightsAndWatermarks);

			if (nfileOpened) {
				cout << "============2=========" << endl;
				cout << "nfile->GetRights():" << endl;
				vector<SDRmFileRight> ADfilerights = nfile->GetRights();
				for each(SDRmFileRight adrights in ADfilerights) {
					cout << "AD_rights: " << adrights << endl;
				}
			}
		}
		wcout << L"Press Enter key to do next..." << endl;
		getchar();

		SDWLibDeleteRmcInstance(pInstance);
		cout << "=======Test END======" << endl;

	}
}
INSTANTIATE_TEST_CASE_P(JPCtest, pathinput, testing::ValuesIn(csv_JPC_invalidinput));
#endif

class initEnd :public::testing::TestWithParam<int> {};
TEST_P(initEnd, initLogout)
{
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;

	SDWLResult res = CreateSDRmcInstance_CreateClientID(s2ws(jpc_testFolder).c_str(), pInstance, jpc_clientID, DEFUALT_ROUTER, DEFUALT_TENANT, s2ws(jpc_PCpath).c_str());

	cout << "GetLoginUser(): ";
	res = pInstance->GetLoginUser(jpc_email, jpc_passcode, &puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	cout << "LogoutUser: ";
	res = puser->LogoutUser();
	checkSDWLResult(res);

	SDWLibDeleteRmcInstance(pInstance);
	cout << "=======Test END======" << endl;
}
INSTANTIATE_TEST_CASE_P(JPCtest, initEnd, testing::Values(1));

#if defined(test_ViaShare)
TEST(JPCtest, viaShare)
{
	std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
	SDWLResult res;
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	ISDRmUser *puser2 = NULL;

	jpc_clientID = "";
	res = CreateSDRmcInstance_CreateClientID(s2ws(jpc_testFolder).c_str(), pInstance, jpc_clientID, DEFUALT_ROUTER, DEFUALT_TENANT, s2ws(jpc_PCpath).c_str());
	
	//waitInstanceInitFinish(pInstance);

	cout << "GetLoginUser(): ";
	res = pInstance->GetLoginUser(jpc_email, jpc_passcode, &puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	//share a file
	if (res) {
		string tags = "{\"itar\":[\"TAA-01\"],\"QA\":[\"Lifi\",\"Hello\"]}";
		SDR_Expiration expire;
		SDR_WATERMARK_INFO watermarkinfo;
		expire.type = IExpiryType::NEVEREXPIRE;
		std::vector<SDRmFileRight> rights;
		std::vector<std::string> recipients = { "472049994@qq.com", "lifi.yuan@nextlabs.com" };
		wstring comments_w = L"Comments to recipients";
		wstring localfile_pathw = L"C:\\SDK test data\\PLAN FILE\\1.3.pdf";
		wstring newcreatedfilePath = L"";
		cout << "ShareFile: ";
		res = puser->ShareFile(localfile_pathw, newcreatedfilePath, rights, recipients, comments_w, watermarkinfo, expire, tags);
		//cout << "ProtectFile: ";
		//res = puser->ProtectFile(localfile_pathw, newcreatedfilePathrights, watermarkinfo, expire, tags);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(newcreatedfilePath, &nxlfile);
		cout << "UploadFile: ";
		res = puser->UploadFile(nxlfile);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		string uid = getUserIDFromJson(jpc_user_return);
		string path = jpc_testFolder + "\\working\\" + ws2s(DEFUALT_TENANT) + "\\" + uid;
		string lastCreatedNXLFile = "";
		getLastCreatedNXL(path, lastCreatedNXLFile);

		waitInstanceInitFinish(pInstance);

		cout << "GetHeartBeatInfo: ";
		res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << "---test start---" << endl;
		cout << "GetFileRightsFromCentralPolicies: ";
		rightsAndWatermarks.clear();
		res = puser->GetFileRightsFromCentralPolicies(s2ws(lastCreatedNXLFile), rightsAndWatermarks);
		checkSDWLResult(res);
		printEvaluationResultRights(res, rightsAndWatermarks);

		pInstance->Save();
		cout << "LogoutUser: ";
		res = puser->LogoutUser();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

#pragma region[User: other user with same Instance]
		string password = "123blue!";
		md5(password);
		GetUserLoginResponseData("472049994@qq.com", password, jpc_clientID, jpc_user_return);
		cout << "SetLoginResult- other user with same Instance: ";
		res = pInstance->SetLoginResult(ReadFromFile(jpc_user_return), &puser2);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << "GetHeartBeatInfo: ";
		res = puser2->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << "GetFileRightsFromCentralPolicies: ";
		rightsAndWatermarks.clear();
		res = puser2->GetFileRightsFromCentralPolicies(s2ws(lastCreatedNXLFile), rightsAndWatermarks);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		printEvaluationResultRights(res, rightsAndWatermarks);
#pragma endregion
	}

	SDWLibDeleteRmcInstance(pInstance);

}
#endif

TEST(JPCtest, GetResourceRightsFromCentralPolicies)
{
#pragma region[TestData]
	struct resourceDetail {
		string caseName;
		wstring resourceName;
		wstring resourceType;
		vector<std::pair<std::wstring, std::wstring>> attrs;
	};
	resourceDetail rd;
	vector<resourceDetail> vec;

	vector<string> tags;
	vector<string> tNameValue;

	for each(vector<string> csv_line in csv_JPC_GetResourceRights)
	{
		if (csv_line[0] == "1") {
			rd.caseName = csv_line[1];
			rd.resourceName = s2ws(csv_line[2]);
			rd.resourceType = s2ws(csv_line[3]);
			tags.clear();
			splitString(csv_line[4],tags,";");
			rd.attrs.clear();
			for each(string tag in tags){
				tNameValue.clear();
				splitString(tag, tNameValue, "=");
				rd.attrs.push_back({ s2ws(tNameValue[0]), s2ws(tNameValue[1]) });
			}
			vec.push_back(rd);
		}
	}
#pragma endregion

	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant;
	std::vector<std::pair<SDRmFileRight, std::vector<SDR_OBLIGATION_INFO>>> rightsAndObligations;
#pragma region[Instance init and user login]
	string passcode = "";
	wstring tenantName = L"";
	//string email = "lifi.yuan@nextlabs.com";
	string email = "472049994@qq.com";
	string password = "123blue!";
	md5(password);
	string clientId = "";
	string testFolder = TestDirectory + "JPC_GetResourceRights";
	res = userFirstTimeLogin(email, password, testFolder, tenantName, passcode);
	if (res) {
		res = CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientId, DEFUALT_ROUTER, DEFUALT_TENANT, DEFUALT_SDKLib_FOLDER);
		waitInstanceInitFinish(pInstance);
		cout << "GetLoginUser(): ";
		res = pInstance->GetLoginUser(email, passcode, &puser);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "GetHeartBeatInfo: ";
		res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		printPolicy(pInstance, puser);
#pragma endregion

		// Test getting resource rights and obligations from Central Policies.
		for each(resourceDetail r in vec)
		{
			rightsAndObligations.clear();
			cout << "CaseName: " << r.caseName << ":" << endl;
			wcout << L">> [" << r.resourceType << "] Resource: " << r.resourceName << endl;
			res = puser->GetResourceRightsFromCentralPolicies(r.resourceName, r.resourceType, r.attrs, rightsAndObligations);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			printEvaluationResult_rightsAndObligations(res, rightsAndObligations);
			cout << endl;
		}

		SDWLibDeleteRmcInstance(pInstance);
		cout << "-----Test Finished------" << endl;
	}
}

TEST(JPCtest, tokenCases)
{
	SDWLResult res;

	string passcode = "";
	wstring tenantName = L"";
	//string email= "lifi.yuan@nextlabs.com";
	string email = "472049994@qq.com";
	string password = "123blue!";
	md5(password);
	string testFolder = TestDirectory + "JPCtokenCases";
	res = userFirstTimeLogin(email, password, testFolder, tenantName, passcode);

	if (res) {
		ISDRmcInstance * pInstance;
		ISDRmUser *puser = NULL;
		ISDRmTenant *pTenant;
		string clientId = "";
		res = CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientId, DEFUALT_ROUTER, tenantName, DEFUALT_SDKLib_FOLDER);
		waitInstanceInitFinish(pInstance);

		cout << "GetLoginUser(): ";
		res = pInstance->GetLoginUser(email, passcode, &puser);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		if (res)
		{
			std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

			string tags = "{\"itar\":[\"TAA-03\"],\"QA\":[\"Lifi3\",\"Hello\"]}";
			SDR_Expiration expire;
			SDR_WATERMARK_INFO watermarkinfo;
			expire.type = IExpiryType::NEVEREXPIRE;
			std::vector<SDRmFileRight> rights;
			wstring localfile_pathw = L"C:\\SDK test data\\PLAN FILE\\Normal_TXT.txt";
			wstring newcreatedfilePath = L"";
			cout << "ProtectFile: ";
			res = puser->ProtectFile(localfile_pathw, newcreatedfilePath, rights, watermarkinfo, expire, tags);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			//ISDRmNXLFile* nxlfile;
			//res = puser->OpenFile(newcreatedfilePath, &nxlfile);
			cout << "UploadFile: ";
			res = puser->UploadFile(newcreatedfilePath, newcreatedfilePath);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
			string uid = getUserIDFromJson(user_return);
			string path = testFolder + "\\working\\" + ws2s(tenantName) + "\\" + uid;
			string lastCreatedNXLFile = "";
			getLastCreatedNXL(path, lastCreatedNXLFile);

			cout << "GetHeartBeatInfo: ";
			res = puser->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			printPolicy(pInstance, puser);

			cout << "GetFileRightsFromCentralPolicies: ";
			rightsAndWatermarks.clear();
			res = puser->GetFileRightsFromCentralPolicies(s2ws(lastCreatedNXLFile), rightsAndWatermarks);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			printEvaluationResultRights(res, rightsAndWatermarks);

			/*cout << "Try decrypt this file: ";
			wstring destFile = s2ws(testFolder) + L"\\Decrypted.txt";
			res = puser->DecryptNXLFile(nxlfile, destFile, RLODecrypt);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());*/
		}
		SDWLibDeleteRmcInstance(pInstance);
	}
}

TEST(JPCtest, GetHeartBeatInfo1)
{
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant;
	res = CreateSDRmcInstance_CreateClientID(s2ws(jpc_testFolder).c_str(), pInstance, jpc_clientID, DEFUALT_ROUTER, DEFUALT_TENANT, s2ws(jpc_PCpath).c_str());
	waitInstanceInitFinish(pInstance);
	string password = "123blue!";
	md5(password);
	GetUserLoginResponseData(jpc_email, password, jpc_clientID, jpc_user_return);


	cout << "SetLoginResult: ";
	jpc_loginres = pInstance->SetLoginResult(ReadFromFile(jpc_user_return), &puser, secr);
	checkSDWLResult(jpc_loginres);
	EXPECT_EQ(0, jpc_loginres.GetCode());

	res = pInstance->GetCurrentTenant(&pTenant);
	cout << "GetCurrentTenant(&pTenant):/after user login ";
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res.GetCode() == 0)
	{
		wcout << L"Tenant:" << pTenant->GetTenant() << endl;
		wcout << L"Router URL:" << pTenant->GetRouterURL() << endl;
	}

	cout << "GetHeartBeatInfo: ";
	res = puser->GetHeartBeatInfo();
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	
	cout << "Save: ";
	res = pInstance->Save();
	checkSDWLResult(res);

	if (jpc_loginres)
	{
		cout << "get passcode. " << endl;
		jpc_passcode = puser->GetPasscode();
	}

	SDWLibDeleteRmcInstance(pInstance);
}
TEST(JPCtest, GetHeartBeatInfo2)
{
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant;
	res = CreateSDRmcInstance_CreateClientID(s2ws(jpc_testFolder).c_str(), pInstance, jpc_clientID, DEFUALT_ROUTER, DEFUALT_TENANT, s2ws(jpc_PCpath).c_str());
	waitInstanceInitFinish(pInstance);

	cout << "GetLoginUser(): ";
	res = pInstance->GetLoginUser(jpc_email, jpc_passcode, &puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	wcout<< L"email: " <<puser->GetEmail()<<endl;
	wcout<<L"name: "<<puser->GetName() << endl;

	//cout << "Save: ";
	//res = pInstance->Save();
	//checkSDWLResult(res);

	res = pInstance->GetCurrentTenant(&pTenant);
	cout << "GetCurrentTenant(&pTenant):/after user login ";
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res.GetCode() == 0)
	{
		wcout << L"Tenant:" << pTenant->GetTenant() << endl;
		wcout << L"Router URL:" << pTenant->GetRouterURL() << endl;
	}

	//cout << "GetHeartBeatInfo: ";
	//res = puser->GetHeartBeatInfo();
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());

	cout << "GetResourceRightsFromCentralPolicies:" << endl;
	std::vector<std::pair<SDRmFileRight, std::vector<SDR_OBLIGATION_INFO>>> rightsAndObligations;
	vector<std::pair<std::wstring, std::wstring>> attrs;
	res = puser->GetResourceRightsFromCentralPolicies(L"C:\\SDK test\\JPC11\\working\\ee4eddd8-81fc-4488-a1a7-2a71505c44b3\\3\\Normal_TXT-2018-07-26-07-51-37.txt.nxl", L"fso", attrs, rightsAndObligations);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	printEvaluationResult_rightsAndObligations(res, rightsAndObligations);


	SDWLibDeleteRmcInstance(pInstance);
}


TEST(processTest, bugverify)
{
	SDWLResult res;
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	string testFolder = TestDirectory + "Processtest1";
	string email = "472049994@qq.com";
	string password = "123blue!";
	md5(password);
	string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
	createFolder(testFolder);
	string clientID = "";

	//string filePath = "C:\\SDK test\\RPM\\Dir2\\7308\\4720PTag\\DxDiag-2018-09-17-07-15-08.txt.nxl";
	string filePath = "C:\\SDK test\\RPM\\Dir2\\7308\\4720PTag\\save as issue.-2018-09-19-10-33-52.png.nxl";
	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);


	GetUserLoginResponseData(email, password, clientID, user_return);

	cout << "SetLoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
	checkSDWLResult(res);

	cout << "GetHeartBeatInfo: ";
	res = puser->GetHeartBeatInfo();
	checkSDWLResult(res);


	ISDRmNXLFile * nfile = NULL;
	cout << "OpenFile: ";
	res = puser->OpenFile(s2ws(filePath), &nfile);
	checkSDWLResult(res);

	wcout << L"Press Enter key to do DeleteRmcInstance..." << endl;
	getchar();

	cout << "LogoutUser: ";
	res = puser->LogoutUser();
	checkSDWLResult(res);
	SDWLibDeleteRmcInstance(pInstance);
}