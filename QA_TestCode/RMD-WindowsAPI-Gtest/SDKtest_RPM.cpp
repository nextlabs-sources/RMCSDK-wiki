#include "stdafx.h"
#include "SDLInc.h"
#include <experimental/filesystem> 

static string clientID = "";

void CopyviaCMD(string source, string dest)
{
	LOG(INFO) << "Copy: " << source;
	LOG(INFO) << " Into: " << dest;
	string c = "cmd /c copy \"" + source + "\" \"" + dest + "\" /y";
	LPCSTR cmd = c.c_str();
	WinExec(cmd, SW_HIDE);
	Sleep(2000);
	cout << "Copy end" << endl;
}

wstring CopyNXL2RPMdir(wstring sourceNXL, string RPMdir)
{
	string sourceNXLpath = ws2s(sourceNXL);
	string destNXLpath = RPMdir + "\\" + getFileNameFromPath(sourceNXLpath);
	LOG(INFO) << "Copy: " << sourceNXLpath;
	LOG(INFO) << " Into: " << destNXLpath;

	string c = "cmd /c copy \"" + sourceNXLpath + "\" \"" + destNXLpath + "\" /y";
	LPCSTR cmd = c.c_str();
	WinExec(cmd, SW_HIDE);
	Sleep(2000);
	cout << "Copy end" << endl;

	return s2ws(destNXLpath);
}

string renameviaCMD(string filePath, string renameTo)
{
	//string c = "cmd /c ren \"" + filePath + "\" \"" + renameTo + "\" /y";
	string c = "cmd /c ren \"" + filePath + "\" \"" + renameTo + "\"";
	LPCSTR cmd = c.c_str();
	WinExec(cmd, SW_HIDE);
	Sleep(1000);

	return getPathFromFullPath(filePath)+ renameTo;
}

void DeleteViaSystem(string filePath)
{
	LOG(INFO) << "Delete test file: " << filePath;
	string c = "del \"" + filePath + "\"";
	system(c.c_str());

	Sleep(2000);
	cout << "Delete end" << endl;
}

bool ReadNXLfileInRPMdir(wstring sourceNXL, string RPMdir, bool allowOpen)
{
	bool result = false;
	//Copy the test file into RPMdir
	string sourceNXLpath = ws2s(sourceNXL);
	string destNXLpath = RPMdir + "\\" + getFileNameFromPath(sourceNXLpath);
	LOG(INFO) << "---ReadNXLfile: [" << getFileNameFromPath(sourceNXLpath) << "] in RPMdir---" << endl;

	CopyviaCMD(sourceNXLpath, destNXLpath);

	//Open test file and check if it including NXL heard
	string destHiddenNXL = destNXLpath.substr(0, destNXLpath.length() - 4);
	//Open another process to read the test file
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;
	DWORD returnCode;
	wstring commandstr = L"ReadFileBeginningChar.exe \"" + s2ws(destHiddenNXL) + L"\" 35";
	wchar_t commandLine[256];
	wcscpy(commandLine, commandstr.c_str());

	BOOL bRet = CreateProcess(
		NULL,
		commandLine,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi);

	if (!bRet) { LOG(INFO) << "Create Read Process Error!"; }
	else
	{
		//std::LOG(INFO) << "process is running..." << std::endl;
		WaitForSingleObject(pi.hProcess, -1);
		LOG(INFO) << "Read finished. ";
		GetExitCodeProcess(pi.hProcess, &returnCode);
		LOG(INFO) << "Read process return code:" << returnCode;

		if (returnCode)
		{
			string first35char = ReadFromFile(destHiddenNXL + "_readedContent.txt");
			//string NXLhead = "Protected by SkyDRM.com";
			string NXLhead = "NXLFMT@";
			string::size_type idx;
			idx = first35char.find(NXLhead);
			if (idx == string::npos)
			{
				LOG(INFO) << ">>> File is opened!" << endl;
				result = allowOpen;
			}
			else
			{
				LOG(INFO) << ">>> File is opened with ciphertext(with nxlhead content)" << endl;
				result = !allowOpen;
			}
		}
		else
		{
			LOG(INFO) << "ERROR: Test file cannot open, please check." << endl;
		}
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	DeleteViaSystem(destNXLpath);

	LOG(INFO) << "------------ReadNXLfileInRPMdir end-----------------";
	LOG(INFO) << endl;
	return result;
}


const string charnum = "35";
const string temp_readfile = DataDirectory + "temp_filePath.txt";
bool ReadNXLfile(wstring pathWithNXL, bool allowOpen)
{
	//Open test file and check if it including NXL heard
	bool result = false;
	wstring HiddenNXL = pathWithNXL.substr(0, pathWithNXL.length() - 4);
	
	ofstream SaveFile(temp_readfile, ios::ate);
	SaveFile << ws2s(HiddenNXL);
	SaveFile.close();

	LOG(INFO) << "---ReadNXLfile: [" << getFileNameFromPath(ws2s(pathWithNXL)) << "] ---" << endl;
	
	//Open another process to read the test file
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;
	DWORD returnCode;
	wstring commandstr = L"ReadFileBeginningChar.exe \"" + HiddenNXL + L"\" " + s2ws(charnum);
	wchar_t commandLine[256];
	wcscpy(commandLine, commandstr.c_str());

	BOOL bRet = CreateProcess(
		NULL,
		commandLine,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi);

	if (!bRet) { LOG(INFO) << "Create Read Process Error!"; }
	else
	{
		//std::LOG(INFO) << "process is running..." << std::endl;
		WaitForSingleObject(pi.hProcess, -1);
		LOG(INFO) << "Read finished. ";
		GetExitCodeProcess(pi.hProcess, &returnCode);
		LOG(INFO) << "Read process return code:" << returnCode;

		if (returnCode)
		{
			string first35char = ReadFromFile(ws2s(HiddenNXL) + "_readedContent.txt");
			//string NXLhead = "Protected by SkyDRM.com";
			string NXLhead = "NXLFMT@";
			string::size_type idx;
			idx = first35char.find(NXLhead);
			if (idx == string::npos)
			{
				LOG(INFO) << ">>> File is opened!" << endl;
				result = allowOpen;
			}
			else
			{
				LOG(INFO) << ">>> File is opened with ciphertext(with nxlhead content)" << endl;
				result = !allowOpen;
			}
		}
		else
		{
			LOG(INFO) << "ERROR: Test file cannot open, please check." << endl;
		}
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	LOG(INFO) << "------------ReadNXL end-----------------";
	LOG(INFO) << endl;
	return result;
}

TEST(ReadFileBeginningChar, trustedapp)
{
	try {
		ISDRmcInstance * pInstance;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		ISDRmTenant *pTenant = NULL;
		ISDRmHttpRequest *prequest = NULL;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		//RPMNotifyRMXStatus
		res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (res) {
			wcout << L"puser info: " << puser->GetEmail() << endl;
		}

		bool running = true;
		LOG(INFO) << "RPMNotifyRMXStatus: ";
		res = pInstance->RPMNotifyRMXStatus(running, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		string file = ReadFromFile(temp_readfile);
		int charNum = string2Int(charnum);
		cout << "Test file name: " << file << endl;
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
			cout << "ReadFileBeginningChar: the test file cannot open, please check." << endl;
			throw "Test file cannot open.";
		}

		string exportfilename = file + "_readedContent.txt";
		ofstream SaveFile(exportfilename, ios::ate);
		SaveFile << content;
		SaveFile.close();

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

void prepareTestNXLFiles(ISDRmUser* puser, string sourceFile, std::vector<SDRmFileRight> rights, wstring recipients, wstring comments, SDR_WATERMARK_INFO watermarkinfo, SDR_Expiration expire, string tags, string membershipId, wstring &NXL_myValut, wstring &NXL_project, wstring &NXL_Sys)
{
	SDWLResult res;
	string orgname = getFileNameFromPath(sourceFile);
	string namestart = "";
	string namefull = "";
	string tempsource = "";
	wstring sourceFile_w;
	if (rights.size() == 0) { namestart = "Tag_"; }
	else { namestart = "ADhoc_"; }
	
	namefull = namestart + "My_" + getFileNameFromPath(sourceFile);
	tempsource = renameviaCMD(sourceFile, namefull);
	sourceFile_w = s2ws(tempsource);
	LOG(INFO) << "ShareFile- MyVault: ";
	res = puser->ProtectFile(sourceFile_w, NXL_myValut, rights, watermarkinfo, expire, tags);
	checkResult(res, 0);
	renameviaCMD(tempsource, orgname);
	Sleep(1000);

	LOG(INFO) << "UploadFile: ";
	//ISDRmNXLFile* nxlfile;
	//res = puser->OpenFile(NXL_myValut, &nxlfile);
	//LOG(INFO) << ws2s(nxlfile->GetFileName());
	res = puser->UploadFile(NXL_myValut, sourceFile_w, recipients, comments);
	checkResult(res, 0);
	Sleep(500);

	//protect a Project file
	namefull = namestart + "Pj_" + getFileNameFromPath(sourceFile);
	tempsource = renameviaCMD(sourceFile, namefull);
	sourceFile_w = s2ws(tempsource);
	LOG(INFO) << "ProtectFile- project: ";
	res = puser->ProtectFile(sourceFile_w, NXL_project, rights, watermarkinfo, expire, tags, membershipId);
	checkResult(res, 0);
	renameviaCMD(tempsource, orgname);
	Sleep(1000);

	//Protect Systembucket file
	bool adhoc; int heartbeat; int sysprojectid; string sysprojecttenantid;
	res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid);
	string sysMembershipId = puser->GetMembershipID(sysprojectid);

	namefull = namestart + "Sys_" + getFileNameFromPath(sourceFile);
	tempsource = renameviaCMD(sourceFile, namefull);
	sourceFile_w = s2ws(tempsource);
	LOG(INFO) << "ProtectFile- System Bucket: ";
	res = puser->ProtectFile(sourceFile_w, NXL_Sys, rights, watermarkinfo, expire, tags, sysMembershipId);
	checkResult(res, 0);
	renameviaCMD(tempsource, orgname);
	Sleep(1000);

}

void UserTestingInit(ISDRmUser* puser, ISDRmcInstance* pInstance, string clientID)
{
	LOG(INFO) << "GetHeartBeatInfo: ";
	SDWLResult res = puser->GetHeartBeatInfo();
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	//LOG(INFO) << "SetRPMClientId: ";
	//res = pInstance->SetRPMCcontrolientId(clientID);
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());

	LOG(INFO) << "SetRPMLoginResult: ";
	string passcode = puser->GetPasscode();
	string email = ws2s(puser->GetEmail());
	string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);
	res = pInstance->SetRPMLoginResult(user_logindata, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	//LOG(INFO) << "SetRPMPolicyBundle: ";
	//res = pInstance->SetRPMPolicyBundle();
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());

	LOG(INFO) << "SyncUserAttributes: ";
	res = pInstance->SyncUserAttributes();
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
}

void check_rights(std::vector<SDRmFileRight> ExpectedRights, std::vector<SDRmFileRight> CurrentRights)
{
	int esize = ExpectedRights.size();
	int csize = CurrentRights.size();
	if (esize != csize)
	{
		LOG(INFO) << "Rights mismatch.";
		EXPECT_TRUE(false);
	}
	else
	{
		for each(SDRmFileRight r in ExpectedRights)
		{

		}
	}
}

void check_RPMGetRights(std::vector<SDRmFileRight> ExpectedRights, wstring filePath, ISDRmcInstance* pInstance)
{
	LOG(INFO) << "check_RPMGetRights() for file: " + ws2s(filePath);
	std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
	SDWLResult res = pInstance->RPMGetRights(filePath, rightsAndWatermarks);
	std::vector<SDRmFileRight> Rights;
	for (const auto& r : rightsAndWatermarks) {
		Rights.push_back(r.first);
	}
	check_rights(ExpectedRights, Rights);
}

class Spin :public::testing::TestWithParam<vector<string>> {};
TEST_P(Spin, OpenInDifferentDir)
{
#pragma region[read csv data]
	vector<string> csv = GetParam();
	string casename = csv[0];
	LOG(INFO) << endl;
	LOG(INFO) << "====== Case: [" << casename << "] ======";
	string UserA = csv[1];
	string passwordA = csv[2];
	md5(passwordA);
	string UserB = csv[3];
	string passwordB = csv[4];
	md5(passwordB);
	string RPMdir = csv[5];
	createFolder(RPMdir);
	string testFile = csv[6];
	string rights_str = csv[7];

	int Validity_option = string2Int(csv[8]);
	string Validity_startDate = csv[9];
	string Validity_endDate = csv[10];

	string tags_csv = csv[11];
	string tags = patchTagString(tags_csv);

	string allow = csv[12];
	transform(allow.begin(), allow.end(), allow.begin(), ::tolower);
	bool PolicyExpected_A = (allow == "allow") ? true : false;
	allow = csv[13];
	transform(allow.begin(), allow.end(), allow.begin(), ::tolower);
	bool PolicyExpected_B = (allow == "allow") ? true : false;
#pragma endregion
#pragma region[Variable declaration and init]
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmUser *puserB = NULL;
	ISDRmTenant *pTenant = NULL;
	Json::Value jroot;

	SDR_WATERMARK_INFO watermarkinfo;
	std::vector<SDRmFileRight> rights;
	std::vector<std::string> recipients = { UserB };
	wstring recipients_w = s2ws(UserB);
	wstring comments_w = L"Comments to recipients";

	SDR_Expiration expire;
	IExpiryType t;
	switch (Validity_option) {
	case 1: {
		t = RELATIVEEXPIRE;
		expire.end = stringToUINT64(Validity_endDate);
		break;
	}
	case 2: {
		t = ABSOLUTEEXPIRE;
		expire.end = stringToUINT64(Validity_endDate);
		break;
	}
	case 3: {
		t = RANGEEXPIRE;
		expire.start = stringToUINT64(Validity_startDate);
		expire.end = stringToUINT64(Validity_endDate);
		break;
	}
	default:
		t = NEVEREXPIRE;
	}
	expire.type = t;

	wstring NXL_ADhoc_myValut = L"";
	wstring NXL_Tag_myValut = L"";
	wstring NXL_ADhoc_project = L"";
	wstring NXL_Tag_project = L"";
	wstring NXL_ADhoc_Sys = L"";
	wstring NXL_Tag_Sys = L"";

	string testFolder = TestDirectory + "RPM_Spin";
	createFolder(testFolder);
	//Copy test data from data server into testfolder
	string sourceFile = testFolder + "\\" + casename + "_" + getFileNameFromPath(testFile);
	CopyviaCMD(testFile, sourceFile);

	string UserA_json = testFolder + "\\User_" + UserA + "_RMSreturn.json";
	string UserB_json = testFolder + "\\User_" + UserB + "_RMSreturn.json";

#pragma endregion

	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);

	res = pInstance->GetCurrentTenant(&pTenant);
	wstring rmsurl = pTenant->GetRMSURL();

#pragma region[User A prepare test file]
	LOG(INFO) << "----------User A------------" << endl;
	GetUserLoginResponseData(UserA, passwordA, clientID, UserA_json);
	//create a project
	string projectJson = testFolder + "\\User_" + UserA.substr(0, 4) + "_NewProject.json";
	Project_Create(UserA_json, clientID, ws2s(rmsurl), "RPMSpin_" + casename, recipients, projectJson);
	jroot = ParseJsonFile(projectJson);
	string projectId = jroot["results"]["projectId"].asString(); //get the project ID
	string membershipId = jroot["results"]["membership"]["id"].asString(); //get the membership ID
																		   //update project info to puser
	GetUserLoginResponseData(UserA, passwordA, clientID, UserA_json);
	LOG(INFO) << "Set SDK LoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(UserA_json), &puser, secr);
	checkResult(res, 0);
	if (res) {
		LOG(INFO) << "Central policy file: " << endl;
		prepareTestNXLFiles(puser, sourceFile, rights, recipients_w, comments_w, watermarkinfo, expire, tags, membershipId, NXL_Tag_myValut, NXL_Tag_project, NXL_Tag_Sys);

		LOG(INFO) << "AD-hoc file: " << endl;
		rights = Tick_Rights(rights_str);
		prepareTestNXLFiles(puser, sourceFile, rights, recipients_w, comments_w, watermarkinfo, expire, tags, membershipId, NXL_ADhoc_myValut, NXL_ADhoc_project, NXL_ADhoc_Sys);
#pragma endregion

#pragma region[RPM init]
		LOG(INFO) << "----------RPM------------" << endl;

		ret = pInstance->IsRPMDriverExist();
		LOG(INFO) << "IsRPMDriverExist: " << ret << endl;
		if (!ret) { LOG(INFO) << "Please double check if the RPM build is installed!" << endl; 	exit(1); }

		//LOG(INFO) << "SetRPMRDPDir: ";
		//res = pInstance->SetRPMPDPDir(L"C:\\Program Files\\NextLabs");
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		wstring Dir = s2ws(RPMdir);
		LOG(INFO) << "AddRPMDir: [" << RPMdir << "]:  ";
		res = pInstance->AddRPMDir(Dir);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		LOG(INFO) << "AddRPMDir: [" << RPMdir << "] again:  ";
		res = pInstance->AddRPMDir(Dir);
		checkSDWLResult(res);
		EXPECT_EQ(183, res.GetCode());
#pragma endregion

#pragma region[UseA try open those file in RPMdir]
		UserTestingInit(puser, pInstance, clientID);
		LOG(INFO) << endl;
		LOG(INFO) << "NXL_ADhoc_myValut - UserA" << endl;	
		EXPECT_TRUE(ReadNXLfileInRPMdir(NXL_ADhoc_myValut, RPMdir, true));
		LOG(INFO) << "NXL_ADhoc_project - UserA" << endl;
		EXPECT_TRUE(ReadNXLfileInRPMdir(NXL_ADhoc_project, RPMdir, true));
		LOG(INFO) << "NXL_Tag_myValut - UserA" << endl;
		EXPECT_TRUE(ReadNXLfileInRPMdir(NXL_Tag_myValut, RPMdir, PolicyExpected_A));
		LOG(INFO) << "NXL_Tag_project - UserA" << endl;
		EXPECT_TRUE(ReadNXLfileInRPMdir(NXL_Tag_project, RPMdir, PolicyExpected_A));
#pragma endregion

		LOG(INFO) << "RPM- UserA Logout: ";
		res = pInstance->RPMLogout();
		checkResult(res, 0);

		LOG(INFO) << "SDK- userA logout:";
		res = puser->LogoutUser();
		checkResult(res, 0);
#pragma endregion

#pragma region[UseB try open those file in RPMdir]
		GetUserLoginResponseData(UserB, passwordB, clientID, UserB_json);
		res = pInstance->SetLoginResult(ReadFromFile(UserB_json), &puserB, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (res) {
			RPMdir += "\\UserB";
			createFolder(RPMdir);

			UserTestingInit(puserB, pInstance, clientID);
			LOG(INFO) << endl;
			LOG(INFO) << "NXL_ADhoc_myValut - UserB" << endl;
			EXPECT_TRUE(ReadNXLfileInRPMdir(NXL_ADhoc_myValut, RPMdir, true));
			LOG(INFO) << "NXL_Tag_myValut - UserB" << endl;
			EXPECT_TRUE(ReadNXLfileInRPMdir(NXL_Tag_myValut, RPMdir, PolicyExpected_B));

			LOG(INFO) << "Access project file before join project:" << endl;
			LOG(INFO) << "NXL_ADhoc_project - UserB" << endl;
			EXPECT_TRUE(ReadNXLfileInRPMdir(NXL_ADhoc_project, RPMdir, false));
			LOG(INFO) << "NXL_Tag_project - UserB" << endl;
			EXPECT_TRUE(ReadNXLfileInRPMdir(NXL_Tag_project, RPMdir, false));


			string PendingInvitations_json = testFolder + "\\User_" + UserB.substr(0, 4) + "_PendingInvitations.json";
			Project_UserPendingInvitations(UserB_json, clientID, ws2s(rmsurl), PendingInvitations_json);
			Project_AcceptInvitation(UserB_json, clientID, ws2s(rmsurl), PendingInvitations_json, string2Int(projectId));
			LOG(INFO) << "Access project file after join project:" << endl;
			RPMdir += "\\after Join project";
			createFolder(RPMdir);
			LOG(INFO) << "NXL_ADhoc_project - UserB" << endl;
			EXPECT_TRUE(ReadNXLfileInRPMdir(NXL_ADhoc_project, RPMdir, true));
			LOG(INFO) << "NXL_Tag_project - UserB" << endl;
			EXPECT_TRUE(ReadNXLfileInRPMdir(NXL_Tag_project, RPMdir, PolicyExpected_B));
		}
#pragma endregion

		Sleep(2000);
		LOG(INFO) << "RemoveRPMDir: [" << RPMdir << "]:  ";
		res = pInstance->RemoveRPMDir(Dir);
		checkResult(res, 0);

		LOG(INFO) << "RPM- UserB Logout: ";
		res = pInstance->RPMLogout();
		checkResult(res, 0);

		LOG(INFO) << "SDK- userB logout:";
		res = puserB->LogoutUser();
		checkResult(res, 0);

	}

	SDWLibDeleteRmcInstance(pInstance);
	LOG(INFO) << "-------------This case finished--------" << endl << endl;
}
INSTANTIATE_TEST_CASE_P(RPMTest, Spin, testing::ValuesIn(csv_RPMSpin));

class Spin2 :public::testing::TestWithParam<vector<string>> {};
TEST_P(Spin2, Spin_OpenSameFileFor2Users)
{
#pragma region[read csv data]
	vector<string> csv = GetParam();
	string IsRun = csv[14];
	if (IsRun == "run" || IsRun == "1")
	{
		string casename = csv[0];
		LOG(INFO) << endl;
		LOG(INFO) << "====== Case: [" << casename << "] ======";
		string UserA = csv[1];
		string passwordA = csv[2];
		md5(passwordA);
		string UserB = csv[3];
		string passwordB = csv[4];
		md5(passwordB);
		string RPMdir = csv[5];
		createFolder(RPMdir);
		string testFile = csv[6];
		string rights_str = csv[7];

		int Validity_option = string2Int(csv[8]);
		string Validity_startDate = csv[9];
		string Validity_endDate = csv[10];

		string tags_csv = csv[11];
		string tags = patchTagString(tags_csv);

		string allow = csv[12];
		transform(allow.begin(), allow.end(), allow.begin(), ::tolower);
		bool PolicyExpected_A = (allow == "allow") ? true : false;
		allow = csv[13];
		transform(allow.begin(), allow.end(), allow.begin(), ::tolower);
		bool PolicyExpected_B = (allow == "allow") ? true : false;
#pragma endregion
#pragma region[Variable declaration and init]
		ISDRmcInstance * pInstance;
		bool ret = false;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		ISDRmUser *puserB = NULL;
		ISDRmTenant *pTenant = NULL;
		Json::Value jroot;

		SDR_WATERMARK_INFO watermarkinfo;
		std::vector<SDRmFileRight> rights;
		std::vector<std::string> recipients = { UserB };
		wstring recipients_w = s2ws(UserB);
		wstring comments_w = L"Comments to recipients";

		SDR_Expiration expire;
		IExpiryType t;
		switch (Validity_option) {
		case 1: {
			t = RELATIVEEXPIRE;
			expire.end = stringToUINT64(Validity_endDate);
			break;
		}
		case 2: {
			t = ABSOLUTEEXPIRE;
			expire.end = stringToUINT64(Validity_endDate);
			break;
		}
		case 3: {
			t = RANGEEXPIRE;
			expire.start = stringToUINT64(Validity_startDate);
			expire.end = stringToUINT64(Validity_endDate);
			break;
		}
		default:
			t = NEVEREXPIRE;
		}
		expire.type = t;

		wstring NXL_ADhoc_myValut = L"";
		wstring NXL_Tag_myValut = L"";
		wstring NXL_ADhoc_project = L"";
		wstring NXL_Tag_project = L"";
		wstring NXL_ADhoc_Sys = L"";
		wstring NXL_Tag_Sys = L"";

		string testFolder = TestDirectory + "RPM_Spin";
		createFolder(testFolder);
		//Copy test data from data server into testfolder
		string sourceFile = testFolder + "\\" + casename + "_" + getFileNameFromPath(testFile);
		CopyviaCMD(testFile, sourceFile);

		string UserA_json = testFolder + "\\User_" + UserA + "_RMSreturn.json";
		string UserB_json = testFolder + "\\User_" + UserB + "_RMSreturn.json";

		string PolicyModels_return = testFolder + "\\PolicyModels_RMSreturn.json";
		string PolicyDeploy_return = testFolder + "\\PolicyDeploy_return.json";
		string projectJson_return = testFolder + "\\NewProject_return.json";
		string SetClassification_return = testFolder + "\\SetClassification_return.json";
		int statusCode;

		string tenantId;
		string projectId;
		string membershipId;
		vector<SDRmFileRight> filerights;
#pragma endregion

		CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
		waitInstanceInitFinish(pInstance);

		res = pInstance->GetCurrentTenant(&pTenant);
		wstring rmsurl = pTenant->GetRMSURL();

#pragma region[User A prepare test file]
		LOG(INFO) << "----------User A------------" << endl;
		GetUserLoginResponseData(UserA, passwordA, clientID, UserA_json);
		//create a project
		//string projectJson = testFolder + "\\User_" + UserA.substr(0, 4) + "_NewProject.json";
		//Project_Create(UserA_json, clientID, ws2s(rmsurl), "RPMSpin_" + casename, recipients, projectJson);
		//jroot = ParseJsonFile(projectJson);
		//string projectId = jroot["results"]["projectId"].asString(); 
		//string membershipId = jroot["results"]["membership"]["id"].asString();
		//
		LOG(INFO) << "Create a project.";
		uint64_t tstamp = (uint64_t)std::time(nullptr);
		Project_Create(UserA_json, clientID, ws2s(rmsurl), casename + "_" + to_string(tstamp), recipients, projectJson_return);
		jroot = ParseJsonFile(projectJson_return);
		statusCode = jroot["statusCode"].asInt();
		if (statusCode != 200) { LOG(INFO) << "Fail in RESTAPI - Project_Create"; }
		else
		{
			tenantId = jroot["results"]["membership"]["tenantId"].asString();
			projectId = jroot["results"]["projectId"].asString();
			membershipId = jroot["results"]["membership"]["id"].asString();

			LOG(INFO) << "SetClassification for project.";
			//SetClassification
			SetClassification(UserA_json, clientID, ws2s(rmsurl), tenantId, SetClassification_return);
			jroot = ParseJsonFile(SetClassification_return);
			statusCode = jroot["statusCode"].asInt();
			if (statusCode != 200 && statusCode != 201) { LOG(INFO) << "Fail in RESTAPI - Set Classification"; }
			else
			{
				LOG(INFO) << "GetPolicyModels for project.";
				//GetPolicyModels
				GetPolicyModels(UserA_json, clientID, ws2s(rmsurl), tenantId, PolicyModels_return);
				jroot = ParseJsonFile(PolicyModels_return);
				statusCode = jroot["statusCode"].asInt();
				if (statusCode != 200) { LOG(INFO) << "Fail in RESTAPI - Set GetPolicyModels"; }
				else {
					string PolicyModelsID = jroot["results"]["resource"]["id"].asString();
					string PolicyModelsName = jroot["results"]["resource"]["name"].asString();

					//PolicyDeploy
					LOG(INFO) << "PolicyDeploy for project.";
					PolicyDeploy(UserA_json, clientID, ws2s(rmsurl), PolicyModelsID, PolicyModelsName, tenantId, PolicyDeploy_return);
					jroot = ParseJsonFile(PolicyModels_return);
					statusCode = jroot["statusCode"].asInt();
					if (statusCode != 200) { LOG(INFO) << "Fail in RESTAPI - Set PolicyDeploy"; }
					else { LOG(INFO) << "Policy deploy success."; }
				}
			}
		}
		///////////////////////
		//update project info to puser
		GetUserLoginResponseData(UserA, passwordA, clientID, UserA_json);
		LOG(INFO) << "Set SDK LoginResult: ";
		res = pInstance->SetLoginResult(ReadFromFile(UserA_json), &puser, secr);
		checkResult(res, 0);
		LOG(INFO) << "GetHeartBeatInfo: ";
		res = puser->GetHeartBeatInfo();
		checkResult(res, 0);

		if (res) {
			LOG(INFO) << "Central policy file: " << endl;
			prepareTestNXLFiles(puser, sourceFile, rights, recipients_w, comments_w, watermarkinfo, expire, tags, membershipId, NXL_Tag_myValut, NXL_Tag_project, NXL_Tag_Sys);

			LOG(INFO) << "AD-hoc file: " << endl;
			rights = Tick_Rights(rights_str);
			prepareTestNXLFiles(puser, sourceFile, rights, recipients_w, comments_w, watermarkinfo, expire, tags, membershipId, NXL_ADhoc_myValut, NXL_ADhoc_project, NXL_ADhoc_Sys);
#pragma endregion
			//Copy test files into test foler (Before folder make as PRMdir)
			NXL_ADhoc_myValut = CopyNXL2RPMdir(NXL_ADhoc_myValut, RPMdir);
			NXL_Tag_project = CopyNXL2RPMdir(NXL_Tag_project, RPMdir);

#pragma region[RPM init]
			LOG(INFO) << "----------RPM------------" << endl;

			ret = pInstance->IsRPMDriverExist();
			LOG(INFO) << "IsRPMDriverExist: " << ret << endl;
			if (!ret) { LOG(INFO) << "Please double check if the RPM build is installed!" << endl; 	exit(1); }

			//Register App 
			wstring app_read = L"C:\\auto\\Clone-rmcsdk-wiki2\\QA_TestCode\\x64\\Debug\\ReadFileBeginningChar.exe";
			string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

			LOG(INFO) << "RPMRegisterApp_ " << ws2s(app_read);
			res = pInstance->RPMRegisterApp(app_read, secr);
			checkResult(res, 0);

			//Add RPM dir
			wstring Dir = s2ws(RPMdir);
			LOG(INFO) << "AddRPMDir: [" << RPMdir << "]:  ";
			res = pInstance->AddRPMDir(Dir);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			LOG(INFO) << "AddRPMDir: [" << RPMdir << "] again:  ";
			res = pInstance->AddRPMDir(Dir);
			checkSDWLResult(res);
			EXPECT_EQ(183, res.GetCode());
#pragma endregion

			//Copy test files into RPMdir (After folder make as PRMdir)
			NXL_ADhoc_project = CopyNXL2RPMdir(NXL_ADhoc_project, RPMdir);
			NXL_Tag_myValut = CopyNXL2RPMdir(NXL_Tag_myValut, RPMdir);

#pragma region[UseA try open those file in RPMdir]
			UserTestingInit(puser, pInstance, clientID);
			LOG(INFO) << endl;
			LOG(INFO) << "Read by: [UserA]" << endl;
			EXPECT_TRUE(ReadNXLfile(NXL_ADhoc_myValut, true));
			EXPECT_TRUE(ReadNXLfile(NXL_ADhoc_project, true));
			EXPECT_TRUE(ReadNXLfile(NXL_ADhoc_Sys, true));
			EXPECT_TRUE(ReadNXLfile(NXL_Tag_myValut, PolicyExpected_A));
			EXPECT_TRUE(ReadNXLfile(NXL_Tag_project, PolicyExpected_A));
			EXPECT_TRUE(ReadNXLfile(NXL_Tag_Sys, PolicyExpected_A));
			

#pragma endregion

			LOG(INFO) << "RPM- UserA Logout: ";
			res = pInstance->RPMLogout();
			checkResult(res, 0);

			LOG(INFO) << "SDK- userA logout:";
			res = puser->LogoutUser();
			checkResult(res, 0);
#pragma endregion

#pragma region[UseB try open those file in RPMdir]
			GetUserLoginResponseData(UserB, passwordB, clientID, UserB_json);
			res = pInstance->SetLoginResult(ReadFromFile(UserB_json), &puserB, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (res) {
				RPMdir += "\\UserB";
				createFolder(RPMdir);

				UserTestingInit(puserB, pInstance, clientID);
				LOG(INFO) << endl;
				LOG(INFO) << "Read by: [UserB]" << endl;
				EXPECT_TRUE(ReadNXLfile(NXL_ADhoc_myValut, true));
				EXPECT_TRUE(ReadNXLfile(NXL_Tag_myValut, PolicyExpected_B));
				EXPECT_TRUE(ReadNXLfile(NXL_ADhoc_Sys, false));
				EXPECT_TRUE(ReadNXLfile(NXL_Tag_Sys, PolicyExpected_B));

				LOG(INFO) << "Access project file before join project:" << endl;
				EXPECT_TRUE(ReadNXLfile(NXL_ADhoc_project, false));
				EXPECT_TRUE(ReadNXLfile(NXL_Tag_project, false));

				string PendingInvitations_json = testFolder + "\\User_" + UserB.substr(0, 4) + "_PendingInvitations.json";
				Project_UserPendingInvitations(UserB_json, clientID, ws2s(rmsurl), PendingInvitations_json);
				Project_AcceptInvitation(UserB_json, clientID, ws2s(rmsurl), PendingInvitations_json, string2Int(projectId));
				cout << endl;
				//
				LOG(INFO) << "GetHeartBeatInfo: ";
				SDWLResult res = puserB->GetHeartBeatInfo();
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());

				LOG(INFO) << "SyncUserAttributes: ";
				res = pInstance->SyncUserAttributes();
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());

				LOG(INFO) << "Access project file after join project:" << endl;
				RPMdir += "\\after Join project";
				createFolder(RPMdir);
				EXPECT_TRUE(ReadNXLfile(NXL_ADhoc_project, true));
				EXPECT_TRUE(ReadNXLfile(NXL_Tag_project, PolicyExpected_B));
			}
#pragma endregion
			LOG(INFO) << "RPM- UserB Logout: ";
			res = pInstance->RPMLogout();
			checkResult(res, 0);

			LOG(INFO) << "SDK- userB logout:";
			res = puserB->LogoutUser();
			checkResult(res, 0);

			LOG(INFO) << "RemoveRPMDir: [" << RPMdir << "]:  ";
			res = pInstance->RemoveRPMDir(Dir);
			checkResult(res, 0);

			//delete test files
			DeleteViaSystem(ws2s(NXL_ADhoc_myValut));
			DeleteViaSystem(ws2s(NXL_Tag_myValut));
			DeleteViaSystem(ws2s(NXL_Tag_project));
			DeleteViaSystem(ws2s(NXL_ADhoc_project));
			Sleep(2000);
		}

		SDWLibDeleteRmcInstance(pInstance);
		LOG(INFO) << "-------------This case finished--------" << endl << endl;
	}
}
INSTANTIATE_TEST_CASE_P(RPMTest, Spin2, testing::ValuesIn(csv_RPMSpin));

TEST(RPMTest, runcases)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	bool ret = false;
	string testFolder = TestDirectory + "RPMtest";
	createFolder(testFolder);
#pragma region[TestData]
	struct testData {
		string caseName;
		wstring addDir;
		int expret_add;
		wstring removeDir;
		int expret_remove;
	};
	testData data;

	string clientID = "";
	//string password = md5_s(password_plain);
	//string user_return = testFolder + "\\User_" + email_global + "_RMSreturn.json";
#pragma endregion

	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	//GetUserLoginResponseData(email_global, password, clientID, user_return);

	ret = pInstance->IsRPMDriverExist();
	cout << "IsRPMDriverExist: " << ret << endl;

	//AddRPMDir, RemoveRPM cases
	for each(vector<string> csv_line in csv_RPMTest)
	{
		if (csv_line[0] != "0")
		{
			data.caseName = csv_line[1];
			replace(csv_line[2].begin(), csv_line[2].end(), '<', ',');
			data.addDir = s2ws(csv_line[2]);
			data.expret_add = string2Int(csv_line[3]);
			replace(csv_line[4].begin(), csv_line[4].end(), '<', ',');
			data.removeDir = s2ws(csv_line[4]);
			data.expret_remove = string2Int(csv_line[5]);
		}
		if (csv_line[0] == "1") {
			cout << "Case: " << data.caseName << endl;

			wcout << L"AddRPMDir: [" << data.addDir << L"]:  ";
			res = pInstance->AddRPMDir(data.addDir);
			checkSDWLResult(res);
			/*	EXPECT_EQ(data.expret_add, res.GetCode());*/

			wcout << L"AddRPMDir again: [" << data.addDir << L"]:  ";
			res = pInstance->AddRPMDir(data.addDir);
			checkSDWLResult(res);
			//	//EXPECT_EQ(data.expret_add, res.GetCode());

			//	wcout << L"Press Enter key to do RemoveRPMDir..." << endl;
			//	getchar();
		}
		else if (csv_line[0] == "11") {
			cout << "Case: " << data.caseName << endl;

			wcout << L"AddRPMDir: [" << data.addDir << L"]:  ";
			res = pInstance->AddRPMDir(data.addDir, 1);
			checkSDWLResult(res);

			wcout << L"AddRPMDir again: [" << data.addDir << L"]:  ";
			res = pInstance->AddRPMDir(data.addDir, 1);
			checkSDWLResult(res);
		}
		else if (csv_line[0] == "2")
		{
			cout << "Case: " << data.caseName << endl;

			wcout << L"RemoveRPMDir: [" << data.removeDir << L"]:  ";
			res = pInstance->RemoveRPMDir(data.removeDir);
			checkSDWLResult(res);

			cout << endl;
		}
		else if (csv_line[0] == "22")
		{
			cout << "Case: " << data.caseName << endl;

			wcout << L"RemoveRPMDir: [" << data.removeDir << L"]:  ";
			res = pInstance->RemoveRPMDir(data.removeDir, true);
			checkSDWLResult(res);

			cout << endl;
		}
		else if (csv_line[0] == "3") {
			cout << "Case: " << data.caseName << endl;
			unsigned int dirstatus;

			cout << "Is [" << csv_line[2] << "] a RPMdir?";
			res = pInstance->IsRPMFolder(data.addDir, &dirstatus);
			cout << "-It is [" << dirstatus << "]" << endl;
			checkSDWLResult(res);
		}
		else if (csv_line[0] == "4")
		{
			data.caseName = csv_line[1];
			data.addDir = s2ws(csv_line[2]);

			cout << "RPMResetNXLinFolder: [" << csv_line[2] << "]" << endl;
			//pInstance->RPMResetNXLinFolder(data.addDir);
		}
	}
	SDWLibDeleteRmcInstance(pInstance);

	cout << "-------------This case finished--------" << endl << endl;
}

TEST(RPMTest, runlogin)
{
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	string testFolder = TestDirectory + "RPMtest2";
	createFolder(testFolder);
	ISDRmUser *puser = NULL;

	//string router = ReadFromFile("TEST_ROUTER.txt");
	//string tenant = ReadFromFile("TEST_TENANT.txt");
	//string clientID = "";
	string email = "";
	string password = "";
	//string email = "472049994@qq.com";
	//string email = "lifi.yuan@nextlabs.com";
	//string password = "123blue!";

	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);
	//CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT, L"C:\\ERROR");
	//CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, s2ws(router), s2ws(tenant));
	cout << "Input User Email: ";
	cin >> email;
	cout << "Input password: ";
	cin >> password;
	if (password == "123")
		password = "123blue!";
	md5(password);
	string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
	GetUserLoginResponseData(email, password, clientID, user_return);

	cout << "----------SDK------------" << endl;
	cout << "Set SDK LoginResult: ";
	string userJson = ReadFromFile(user_return);
	res = pInstance->SetLoginResult(userJson, &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	if (res) {
//#define __Test_ProtectFile__
#if defined(__Test_ProtectFile__)
		string tags = "{\"itar\":[\"TAA-01\"],\"QA\":[\"Lifi\",\"Hello\"]}";
		SDR_Expiration expire;
		SDR_WATERMARK_INFO watermarkinfo;
		expire.type = IExpiryType::NEVEREXPIRE;
		std::vector<SDRmFileRight> rights;
		//string rights_str = "view|edit|download";
		//rights = Tick_Rights(rights_str);
		std::vector<std::string> recipients = { "472049994@qq.com", "lifi.yuan@nextlabs.com" };
		wstring comments_w = L"Comments to recipients";

		//cout << "ProtectFile: ";
		//res = puser->ShareFile(L"C:\\SDK test data\\Normal_TXT.txt", rights, recipients, comments_w, watermarkinfo, expire, tags);
		////res = puser->ProtectFile(L"C:\\SDK test data\\Normal_TXT.txt", rights, watermarkinfo, expire, tags);
		////res = puser->ProtectFile(L"C:\\SDK test\\03-04.pdf", rights, watermarkinfo, expire, tags);
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());
		////Sleep(3000);

		//cout << "UploadFile: ";
		//ISDRFiles* pFile = NULL;
		//res = puser->GetLocalFileManager(&pFile);
		//size_t fileNum = pFile->GetListNumber();
		//ISDRmNXLFile* nxlfile = pFile->GetFile(fileNum - 1);
		//wcout << nxlfile->GetFileName() << endl;
		//res = puser->UploadFile(nxlfile);
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());
		wstring newcreatedfilePath = L"";
		cout << "ProtectProject file: ";
		res = puser->ProtectFile(L"C:\\SDK test data\\PLAN FILE\\4720.xlsx", newcreatedfilePath, rights, watermarkinfo, expire, tags, "m13@t-ad61b8f4496f4d1ca271a9f3f7761948");
		//res = puser->ProtectFile(L"C:\\SDK test data\\Project_lifi.txt", rights, watermarkinfo, expire, tags, "m14@t-ad61b8f4496f4d1ca271a9f3f7761948");
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

#endif

		cout << "GetHeartBeatInfo: ";
		SDWLResult res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		printPolicy(pInstance, puser);

		//wstring tenantName = L"t-ad61b8f4496f4d1ca271a9f3f7761948";
		//wcout << L"test GetPolicyBundle() with input: [" << tenantName << L"]" << endl;
		//printPolicyBaseOnTenant(tenantName, puser);
		string getrightsfile;
		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

		while (0) {
			getrightsfile.clear();
			cout << "Input file full path for GetRights: (input 1 to exit) ";
			cin >> getrightsfile;
			if (getrightsfile == "1") break;


			cout << "SDK GetRights(): ";
			rightsAndWatermarks.clear();
			puser->GetRights(s2ws(getrightsfile), rightsAndWatermarks);
			checkResult(res, 0);
			printEvaluationResultRights(res, rightsAndWatermarks);
		}

	}
	cout << "----------RPM------------" << endl;

	ret = pInstance->IsRPMDriverExist();
	cout << "IsRPMDriverExist: " << ret << endl;
	if (!ret)
	{
		cout << "Please double check if the RPM build is installed!" << endl;
	}

	cout << "SetRPMLoginResult: ";
	DWORD start = ::GetTickCount();
	res = pInstance->SetRPMLoginResult(ReadFromFile(user_return), secr);
	DWORD Etime = ::GetTickCount() - start;
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	cout << "  Evaluation time: " << Etime << " (ms)" << endl;

	//cout << "SetRPMRDPDir: ";
	//res = pInstance->SetRPMPDPDir(L"C:\\Program Files\\NextLabs");
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());

	//cout << "SetRPMPolicyBundle: ";
	//res = pInstance->SetRPMPolicyBundle();
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());

	cout << "SyncUserAttributes: ";
	res = pInstance->SyncUserAttributes();
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	res = pInstance->Save();
	EXPECT_EQ(0, res.GetCode());

	//cout << "AddRPMDir: ";
	////res = pInstance->AddRPMDir(L"C:\\SDK test\\RPM\\Dir1\\7303\\003");
	//res = pInstance->AddRPMDir(L"C:\\SDK test\\RPM\\7303\\R");
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());
	//cout << "AddRPMDir: ";
	//res = pInstance->AddRPMDir(L"C:\\SDK test\\RPM\\Dirnew");
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());
	//cout << "AddRPMDir: ";
	//res = pInstance->AddRPMDir(L"C:\\SDK test\\RPM\\Dirnew");
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());

	//wstring addDir = L"E:\\BugVerify\\1257\\007";
	//wcout << L"AddRPMDir: [" << addDir << L"]:  ";
	//res = pInstance->AddRPMDir(addDir);
	//checkSDWLResult(res);

	//wstring removeDir = L"E:\\BugVerify\\1257\\008";
	//wcout << L"RemoveRPMDir: [" << removeDir << L"]:  ";
	//res = pInstance->RemoveRPMDir(removeDir);
	//checkSDWLResult(res);

	/*	cout << "SetRPMServiceStop(true, secr): ";
	res = pInstance->SetRPMServiceStop(true, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode())*/;
	/*	string disableService = "";
	cout << "Input Y do SetRPMServiceStop(false): ";
	cin >> disableService;
	cout << "-- ";
	res = pInstance->SetRPMServiceStop(false);
	checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode())*/;

	string ifRPMdeleteFileToken = "";
	cout << "Input Y do RPMdeleteFileToken: ";
	cin >> ifRPMdeleteFileToken;
	if (ifRPMdeleteFileToken == "Y" || ifRPMdeleteFileToken == "y" || ifRPMdeleteFileToken == "yes") {
		struct testData {
			string caseName;
			wstring filePath;
		};
		testData deleteTokendata;

		for each(vector<string> csv_line in csv_RPM_deleteFileToken)
		{
			deleteTokendata.caseName = csv_line[0];
			deleteTokendata.filePath = s2ws(csv_line[1]);
			cout << "Case: " << deleteTokendata.caseName << endl;

			wcout << L"deleteFile [" << deleteTokendata.filePath << L"]'s token:  ";
			ret = pInstance->RPMDeleteFileToken(deleteTokendata.filePath);
			checkSDWLResult(res);
		}
	}

	string iflogout = "";
	cout << "Input Y do logout: ";
	cin >> iflogout;
	if (iflogout == "Y" || iflogout == "y" || iflogout == "yes") {
		cout << "RPMLogout: ";
		DWORD start_logout = ::GetTickCount();
		res = pInstance->RPMLogout();
		Etime = ::GetTickCount() - start_logout;
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "  Evaluation time: " << Etime << " (ms)" << endl;

		cout << endl << "SDK user logout:";
		res = puser->LogoutUser();
		checkSDWLResult(res);
	}

	//GetUserLoginResponseData(email, password, clientID, user_return);
	//ISDRmUser *puser2 = NULL;
	//cout << "----------SDK------------" << endl;
	//cout << "Set SDK LoginResult: ";
	//userJson = ReadFromFile(user_return);
	//res = pInstance->SetLoginResult(userJson, &puser2);
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());

	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}

TEST(RPMTest, cases)
{
	ISDRmcInstance * pInstance;
	ISDRmTenant *pTenant = NULL;
	ISDRmHttpRequest *prequest = NULL;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	bool ret = false;
	string testFolder = TestDirectory + "RPMtest";
	createFolder(testFolder);
	string PCpath = "C:\\Program Files\\NextLabs";
#pragma region[TestData]
	struct testData {
		string caseName;
		wstring addDir;
		int expret_add;
		wstring removeDir;
		int expret_remove;
	};
	testData data;


	string clientID = "";
	string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";

	cout << "SDWLibCreateSDRmcInstance: ";
	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT, s2ws(PCpath).c_str());
	GetUserLoginResponseData("michelle.xu@nextlabs.com", "085dc19766409f69e4b1e9d44021a8f1", clientID, user_return);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	ret = pInstance->IsRPMDriverExist();
	cout << "IsRPMDriverExist: " << ret << endl;

	if (1)
	{
		string response_user = ReadFromFile(user_return);
		ret = pInstance->SetRPMLoginResult(response_user, secr);

		cout << "SetRPMLoginResult: " << ret << endl;
		EXPECT_TRUE(ret);
	}

	////AddRPMDir, RemoveRPM cases
	//for each(vector<string> csv_line in csv_RPMTest)
	//{
	//	if (csv_line[0] == "1") {
	//		data.caseName = csv_line[1];
	//		data.addDir = s2ws(csv_line[2]);
	//		data.expret_add = string2Int(csv_line[3]);
	//		data.removeDir = s2ws(csv_line[4]);
	//		data.expret_remove = string2Int(csv_line[5]);
	//		cout << "Case: " << data.caseName << endl;

	//		ret = pInstance->AddRPMDir(data.addDir);
	//		wcout << L"AddRPMDir: [" << data.addDir << L"]:  " << ret << endl;
	//		if (data.expret_add)
	//		{
	//			EXPECT_TRUE(ret);
	//		}
	//		else
	//		{
	//			EXPECT_FALSE(ret);
	//		}

	//		wcout << L"Press Enter key to do RemoveRPMDir..." << endl;
	//		getchar();

	//		ret = pInstance->RemoveRPMDir(data.removeDir);
	//		wcout << L"RemoveRPMDir: [" << data.removeDir << L"]:  " << ret << endl;
	//		if (data.expret_remove)
	//		{
	//			EXPECT_TRUE(ret);
	//		}
	//		else
	//		{
	//			EXPECT_FALSE(ret);
	//		}
	//		cout << endl;
	//	}
	//}

	//wcout << L"Press any key to quit and Delete Rmc Instance..." << endl;
	//_getch();
#pragma endregion
	SDWLibDeleteRmcInstance(pInstance);

	cout << "-------------This case finished--------" << endl << endl;
}

TEST(RPMTest, protect)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	bool ret = false;
	string testFolder = TestDirectory + "RPMtest_1";
	createFolder(testFolder);

	string clientID = "";
	string password = md5_s(password_plain);
	string user_return = testFolder + "\\User_" + email_global + "_RMSreturn.json";

	//std::string tags = "";
	string tags = "{\"EAR\":[\"EAR-01\"]}";
	//string tags = "{\"itar\":[\"TAA-04\"],\"QA\":[\"Lifi\"]}";
	//string tags = "{\"itar\":[\"TAA-04\"],\"QA\":[\"Lifi\",\"Hello\"]}";
	SDR_Expiration expire;
	SDR_WATERMARK_INFO watermarkinfo;
	expire.type = IExpiryType::NEVEREXPIRE;
	std::vector<SDRmFileRight> rights;
	wstring newcreatedfilePath = L"";
	//string rights_str = "view|edit|download";
	string rights_str = "view";
	rights = Tick_Rights(rights_str);

	std::vector<std::string> recipients = { "472049994@qq.com", "lifi.yuan@nextlabs.com" };
	wstring comments_w = L"Comments to recipients";

	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	GetUserLoginResponseData(email_global, password, clientID, user_return);

	cout << "SetLoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res) {
		//cout << "ProtectFile: ";
		//res = puser->ProtectFile(L"C:\\SDK test data\\Normal_TXT.txt", newcreatedfilePath, rights, watermarkinfo, expire, tags, "m9@t-59d7bb09f07a467a8c738d543c3c0dcb");
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());
		//cout << "ShareFile: ";
		//res = puser->ShareFile(L"C:\\SDK test data\\Normal_TXT.txt", rights, recipients, comments_w, watermarkinfo, expire, tags);
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());
		res = pInstance->Save();
		//Sleep(3000);
		cout << "ProtectFile: ";
		res = puser->ProtectFile(L"C:\\SDK test data\\Normal_TXT.txt", newcreatedfilePath, rights, watermarkinfo, expire, tags);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		Sleep(3000);

		//ISDRmNXLFile* nxlfile;
		//res = puser->OpenFile(newcreatedfilePath, &nxlfile);
		cout << "UploadFile: ";
		res = puser->UploadFile(newcreatedfilePath, newcreatedfilePath);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//cout << "UploadFile: ";
		//ISDRFiles* pFile = NULL;
		//res = puser->GetLocalFileManager(&pFile);
		//size_t fileNum = pFile->GetListNumber();
		//ISDRmNXLFile* nxlfile = pFile->GetFile(fileNum - 1);
		//wcout << nxlfile->GetFileName() << endl;
		//res = puser->UploadFile(nxlfile);
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());


		//string uid = getUserIDFromJson(user_return);
		//string path = testFolder + "\\working\\" + ws2s(DEFUALT_TENANT) + "\\" + uid;
		//string lastCreatedNXLFile = "";
		//getLastCreatedNXL(path, lastCreatedNXLFile);
	}

	SDWLibDeleteRmcInstance(pInstance);

	cout << "-------------This case finished--------" << endl << endl;
}

TEST(RPMTest, bugverify)
{
	SDWLResult res;
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	string testFolder = TestDirectory + "RPMtest";
	createFolder(testFolder);
	string user_return = "C:\\SDK test\\RPMtest\\User_Lifi.yuan@nextlabs.com_RMSreturn.json";
	string filePath = "C:\\SDK test\\RPM\\AD_test_cert-2018-09-11-02-34-34.txt.nxl";
	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);

	cout << "SetLoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
	checkSDWLResult(res);

	ISDRmNXLFile * nfile = NULL;
	cout << "OpenFile: ";
	res = puser->OpenFile(s2ws(filePath), &nfile);
	checkSDWLResult(res);
}

TEST(RPMTest, bug378)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	bool ret = false;
	string testFolder = TestDirectory + "RPMtest";
	string clientID = "";
	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	//AddRPMDir, RemoveRPM cases
	wstring subdir = L"E:\\SDK test\\test\\04";
	wstring dir = L"E:\\SDK test\\test";
	//#define __remove__
#if defined(__remove__)
	wcout << L"RemoveRPMDir: [" << subdir << L"]:  ";
	res = pInstance->RemoveRPMDir(subdir);
	checkSDWLResult(res);
	wcout << L"RemoveRPMDir: [" << dir << L"]:  ";
	res = pInstance->RemoveRPMDir(dir);
	checkSDWLResult(res);
#else
	wcout << L"AddRPMDir: [" << subdir << L"]:  ";
	res = pInstance->AddRPMDir(subdir);
	checkSDWLResult(res);

	wcout << L"AddRPMDir: [" << dir << L"]:  ";
	res = pInstance->AddRPMDir(dir);
	checkSDWLResult(res);

	wcout << L"RemoveRPMDir: [" << dir << L"]:  ";
	res = pInstance->RemoveRPMDir(dir);
	checkSDWLResult(res);

	wcout << L"RemoveRPMDir: [" << subdir << L"]:  ";
	res = pInstance->RemoveRPMDir(subdir);
	checkSDWLResult(res);

	wcout << L"AddRPMDir: [" << subdir << L"]:  ";
	res = pInstance->AddRPMDir(subdir);
	checkSDWLResult(res);

	wcout << L"AddRPMDir: [" << dir << L"]:  ";
	res = pInstance->AddRPMDir(dir);
	checkSDWLResult(res);

	wcout << L"RemoveRPMDir: [" << dir << L"]:  ";
	res = pInstance->RemoveRPMDir(dir);
	checkSDWLResult(res);

	wcout << L"RemoveRPMDir: [" << subdir << L"]:  ";
	res = pInstance->RemoveRPMDir(subdir);
	checkSDWLResult(res);

	//wcout << L"AddRPMDir: [" << subdir << L"]:  ";
	//res = pInstance->AddRPMDir(subdir);
	//checkSDWLResult(res);

	/*	wcout << L"AddRPMDir: [" << dir << L"]:  ";
	res = pInstance->AddRPMDir(dir);
	checkSDWLResult(res)*/;
#endif

	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}

TEST(RPMTest, serverStopEnable)
{
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	string testFolder = TestDirectory + "RPMtest";
	createFolder(testFolder);

	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);

	cout << "SDWLibCreateSDRmcInstance: ";
	res = SDWLibCreateSDRmcInstance("", 1, 0, 0, DEFUALT_SDKLib_FOLDER, s2ws(testFolder).c_str(), &pInstance);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	cout << "SetRPMServiceStop(true, secr): ";
	res = pInstance->SetRPMServiceStop(true, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	SDWLibDeleteRmcInstance(pInstance);

}

TEST(RPMTest, OfflineCase)
{
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	string testFolder = TestDirectory + "RPMtest3";
	createFolder(testFolder);
	ISDRmUser *puser = NULL;

	string email = "";
	string password = "";
	//string email = "472049994@qq.com";
	//string email = "lifi.yuan@nextlabs.com";
	//string password = "123blue!";

	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);

	cout << "Input User Email: ";
	cin >> email;
	cout << "Input password: ";
	cin >> password;
	if (password == "123")
		password = "123blue!";
	md5(password);
	string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
	GetUserLoginResponseData(email, password, clientID, user_return);

	cout << "----------SDK------------" << endl;
	cout << "Set SDK LoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	if (res) {
		cout << "GetHeartBeatInfo: ";
		SDWLResult res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		printPolicy(pInstance, puser);
	}
	cout << "----------RPM------------" << endl;

	ret = pInstance->IsRPMDriverExist();
	cout << "IsRPMDriverExist: " << ret << endl;
	if (!ret)
	{
		cout << "Please double check if the RPM build is installed!" << endl;
	}

	cout << "SetRPMLoginResult: ";
	DWORD start = ::GetTickCount();
	res = pInstance->SetRPMLoginResult(ReadFromFile(user_return), secr);
	DWORD Etime = ::GetTickCount() - start;
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	cout << "  Evaluation time: " << Etime << " (ms)" << endl;

	//cout << "SetRPMRDPDir: ";
	//res = pInstance->SetRPMPDPDir(L"C:\\Program Files\\NextLabs");
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());

	//cout << "SetRPMPolicyBundle: ";
	//res = pInstance->SetRPMPolicyBundle();
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());

	cout << "SyncUserAttributes: ";
	res = pInstance->SyncUserAttributes();
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());


	//cout << "CacheRPMFileToken(In non-RPMdir): ";
	//res = pInstance->CacheRPMFileToken(L"E:\\SDK test\\001n\\denylogout-2018-10-31-07-59-51.txt.nxl");
	//checkSDWLResult(res);

	//cout << "CacheRPMFileToken(RPMdir: no .nxl in path): ";
	//res = pInstance->CacheRPMFileToken(L"E:\\SDK test\\001\\4720-2018-10-11-05-41-16.xlsx");
	//checkSDWLResult(res);

	//cout << "CacheRPMFileToken(RPMdir: with nxl in path): ";
	//res = pInstance->CacheRPMFileToken(L"E:\\SDK test\\001\\4720-2018-10-12-08-01-41.xlsx.nxl");
	//checkSDWLResult(res);


	cout << "Wait for logout: ";
	cin >> email;

	cout << "RPMLogout: ";
	DWORD start_logout = ::GetTickCount();
	res = pInstance->RPMLogout();
	Etime = ::GetTickCount() - start_logout;
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	cout << "  Evaluation time: " << Etime << " (ms)" << endl;

	cout << endl << "SDK user logout:";
	res = puser->LogoutUser();
	checkSDWLResult(res);


	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}

TEST(RPMTest, base)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	string testFolder = TestDirectory + "RPMbase";
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
//#define _full_
//#define _login_
#define _debug_
	string email = "472049994@qq.com";
	string password = "123blue!";
	unsigned int dirstatus;

	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);
	md5(password);
	string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
	GetUserLoginResponseData(email, password, clientID, user_return);

#if defined(_login_)
	cout << "----------SDK------------" << endl;
	cout << "Set SDK LoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	if (res) {
#if defined(_full_)
		std::wstring hiddir;
		res = pInstance->RPMGetSecretDir(hiddir);
		wcout << L"RPMGetSecretDir[Before Login RPM]: " << hiddir << endl;
		cout << "res.GetCode(): " << res.GetCode() << endl;
		cout << "res.GetMsg(): " << res.GetMsg() << endl;
		checkSDWLResult(res);
#endif
		cout << "GetHeartBeatInfo: ";
		SDWLResult res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << "SetRPMClientId: ";
		res = pInstance->SetRPMClientId(clientID);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << "SetRPMLoginResult: ";
		DWORD start = ::GetTickCount();
		res = pInstance->SetRPMLoginResult(ReadFromFile(user_return), secr);
		DWORD Etime = ::GetTickCount() - start;
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "  Evaluation time: " << Etime << " (ms)" << endl;

		cout << "SetRPMRDPDir: ";
		res = pInstance->SetRPMPDPDir(L"C:\\Program Files\\NextLabs");
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
#endif
#if defined(_full_)
		hiddir.clear();
		res = pInstance->RPMGetSecretDir(hiddir);
		wcout << L"RPMGetSecretDir: " << hiddir << endl;
		cout << "res.GetCode(): " << res.GetCode() << endl;
		cout << "res.GetMsg(): " << res.GetMsg() << endl;
		checkResult(res, 0);
#endif
#if defined(_debug_)
#pragma region[TestData]
		struct testData {
			string caseName;
			wstring addDir;
			int expret_add;
			wstring removeDir;
			int expret_remove;
		};
		testData data;
#pragma endregion
		//test API: RPMIsFolder()
		for each(vector<string> csv_line in csv_RPMTest)
		{
			if (csv_line[0] == "1") {
				data.caseName = csv_line[1];
				data.addDir = s2ws(csv_line[2]);
				data.expret_add = string2Int(csv_line[3]);
				data.removeDir = s2ws(csv_line[4]);
				data.expret_remove = string2Int(csv_line[5]);
				cout << "Case: " << data.caseName << endl;


				cout<< "Is [" << csv_line[2] << "] a RPMdir?";
				res = pInstance->IsRPMFolder(data.addDir, &dirstatus);
				cout << "-It is [" << dirstatus << "]" << endl;
				cout << "res.GetCode(): " << res.GetCode() << endl;
				cout << "res.GetMsg(): " << res.GetMsg() << endl;
				checkSDWLResult(res);
				
			}
		}
#endif
#if defined(_full_)
		string ps_code = puser->GetPasscode();
		cout << "User passcode is :" + ps_code << endl;
		//test API: RPMDeleteFile()
		string deletefile;
		while (1) {
			deletefile.clear();
			cout << "Input deletefile full path: (input 1 to exit) ";
			cin >> deletefile;
			if (deletefile == "1") break;
			LOG(INFO) << "Delete NXL file in RPM dir:";
			res = pInstance->RPMDeleteFile(s2ws(deletefile));
			checkResult(res, 0);
		}
#endif
		//test RPMGetCurrentUserInfo()
		//std::wstring router; std::wstring tenant; std::wstring workingfolder; std::wstring tempfolder; std::wstring sdklibfolder;
		//res = pInstance->RPMGetCurrentUserInfo(router, tenant, workingfolder, tempfolder, sdklibfolder);
		//wcout << "router: " << router << endl << "tenant: " << tenant << endl;
		//wcout << "workingfolder: " << workingfolder << endl;
		//wcout << "sdklibfolder: " << sdklibfolder << endl;
#if defined(_login_)
		cout << "RPMLogout: ";
		DWORD start_logout = ::GetTickCount();
		res = pInstance->RPMLogout();
		Etime = ::GetTickCount() - start_logout;
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "  Evaluation time: " << Etime << " (ms)" << endl;

		cout << endl << "SDK user logout:";
		res = puser->LogoutUser();
		checkSDWLResult(res);
	}
#endif
}

TEST(RPM, withoutLogin)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	bool ret = false;
	string testFolder = TestDirectory + "RPMtest";
	createFolder(testFolder);
	string clientID = "";
#pragma endregion

	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	wstring d = L"";
	res = pInstance->RPMDeleteFile(L"F:\\RPM\\mark\\4\\7513\\1\\1-2019-06-18-02-15-05.txt");
	checkSDWLResult(res);

}

TEST(RPMTest, GetUserInfo)
{

	ISDRmcInstance * pInstance;
	SDWLResult res;
	string testFolder = TestDirectory + "RPMbase111";
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	ISDRmHttpRequest *prequest = NULL;

	//CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);

	
	//test RPMGetCurrentUserInfo()
	//std::wstring router; std::wstring tenant; std::wstring workingfolder; std::wstring tempfolder; std::wstring sdklibfolder;
	//
	//res = pInstance->RPMGetCurrentUserInfo(router, tenant, workingfolder, tempfolder, sdklibfolder);
	//wcout << "router: " << router << endl << "tenant: " << tenant << endl;
	//wcout << "workingfolder: " << workingfolder << endl << "tempfolder: " << tempfolder << endl;
	//wcout << "sdklibfolder: " << sdklibfolder << endl;

	string passcode;
	cout << "debug debug: ";
	cin >> passcode;
	res = RPMGetCurrentLoggedInUser(passcode, pInstance, pTenant, puser);
	LOG(INFO) << "GetLoginRequest: ";
	res = pInstance->GetLoginRequest(&prequest);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	LOG(INFO) << "\tGetPath():" << ws2s(prequest->GetPath()) << endl;
	if (res.GetCode() == 0)
	{
		LOG(INFO) << "-----LoginRequest-----" << endl;
		for each (HttpCookie cookie in prequest->GetCookies())
		{
			LOG(INFO) << "Cookie name:[" << ws2s(cookie.first) << "]";
			LOG(INFO) << "  Vaule:[" << ws2s(cookie.second) << "]" << endl;

			if (ws2s(cookie.first) == "clientId")
			{
				string clientId = ws2s(cookie.second);
				string exportfilename = plan_path + "client_id.txt";
				ofstream SaveFile(exportfilename, ios::ate);
				SaveFile << clientId;// << endl;
				SaveFile.close();
			}
		}
		LOG(INFO) << "---------------------" << endl;
	}

}

TEST(RPMTest, forExplorerAddon)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	string filename;
	unsigned int dirstatus;
	bool filestatus;

	LOG(INFO) << "SDWLibCreateSDRmcInstance: ";

	res = SDWLibCreateSDRmcInstance(DEFUALT_SDKLib_FOLDER, DEFUALT_TEMP_FOLDER.c_str(), &pInstance);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

#pragma region[TestData]
	struct testData {
		string caseName;
		wstring addDir;
		int expret_add;
		wstring removeDir;
		int expret_remove;
	};
	testData data;
#pragma endregion
	//test API: RPMIsFolder()
	for each(vector<string> csv_line in csv_RPMTest)
	{
		if (csv_line[0] == "1") {
			data.caseName = csv_line[1];
			data.addDir = s2ws(csv_line[2]);
			data.expret_add = string2Int(csv_line[3]);
			data.removeDir = s2ws(csv_line[4]);
			data.expret_remove = string2Int(csv_line[5]);
			cout << "Case: " << data.caseName << endl;


			LOG(INFO) << "Is [" << csv_line[2] << "] a RPMdir?";
			res = pInstance->IsRPMFolder(data.addDir, &dirstatus);
			cout << "res.GetCode(): " << res.GetCode() << endl;
			cout << "res.GetMsg(): " << res.GetMsg() << endl;
			checkSDWLResult(res);

		}
	}

	for each(vector<string> csv_line in csv_filePath)
	{
		if (csv_line[0] == "1") {
			filename = csv_line[1];		
			cout << "RPMGetFileStatus for file: [" << filename << "]";
			res = pInstance->RPMGetFileStatus(s2ws(filename), &dirstatus, &filestatus);
			checkSDWLResult(res);
	
			cout << "dirstatus: " << dirstatus << " " << "filestatus" << filestatus << endl;

		}
	}


	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}
TEST(RPMTest, forExplorerAddon2)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	string filename;
	unsigned int dirstatus;
	bool filestatus;

	LOG(INFO) << "SDWLibCreateSDRmcInstance: ";
	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

#pragma region[TestData]
	struct testData {
		string caseName;
		wstring addDir;
		int expret_add;
		wstring removeDir;
		int expret_remove;
	};
	testData data;
#pragma endregion
	//test API: RPMIsFolder()
	for each(vector<string> csv_line in csv_RPMTest)
	{
		if (csv_line[0] == "1") {
			data.caseName = csv_line[1];
			data.addDir = s2ws(csv_line[2]);
			data.expret_add = string2Int(csv_line[3]);
			data.removeDir = s2ws(csv_line[4]);
			data.expret_remove = string2Int(csv_line[5]);
			cout << "Case: " << data.caseName << endl;


			LOG(INFO) << "Is [" << csv_line[2] << "] a RPMdir?";
			res = pInstance->IsRPMFolder(data.addDir, &dirstatus);
			LOG(INFO) << "it is [" << dirstatus << "].";
			checkSDWLResult(res);

		}
	}

	//test API: RPMGetFileStatus()
	for each(vector<string> csv_line in csv_filePath)
	{
		if (csv_line[0] == "1") {
			filename = csv_line[1];
			cout << "RPMGetFileStatus for file: [" << filename << "]";
			res = pInstance->RPMGetFileStatus(s2ws(filename), &dirstatus, &filestatus);
			checkSDWLResult(res);
			cout << "                            ";
			cout << "dirstatus: " << dirstatus << ", " << "filestatus: " << filestatus << endl;

		}
	}


	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}


TEST(RPMTest, RPMdeleteFileToken)
{
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	string testFolder = TestDirectory + "RPMbase";
	createFolder(testFolder);

#pragma region[TestData]
	struct testData {
		string caseName;
		wstring filePath;
	};
	testData deleteTokendata;
#pragma endregion

	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);

	for each(vector<string> csv_line in csv_RPM_deleteFileToken)
	{
		deleteTokendata.caseName = csv_line[0];
		deleteTokendata.filePath = s2ws(csv_line[1]);
		cout << "Case: " << deleteTokendata.caseName << endl;

		wcout << L"deleteFile [" << deleteTokendata.filePath << L"]'s token:  ";
		ret = pInstance->RPMDeleteFileToken(deleteTokendata.filePath);
		checkSDWLResult(res);
	}
}

TEST(Policy, create)
{

	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	string testFolder = TestDirectory + "Policy";
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	string email = "testC@test.com";
	string password = "123blue!";
	std::vector<std::string> recipients = { "testB@test.com","472049994@qq.com" };
	md5(password);
	string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
	string PolicyModels_return = testFolder + "\\PolicyModels_RMSreturn.json";
	string PolicyDeploy_return = testFolder + "\\PolicyDeploy_return.json";
	string projectJson_return = testFolder + "\\NewProject_return.json";
	string SetClassification_return = testFolder + "\\SetClassification_return.json";
	int statusCode;

	Json::Value jroot;
	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	res = pInstance->GetCurrentTenant(&pTenant);
	wstring rmsurl = pTenant->GetRMSURL();

	GetUserLoginResponseData(email, password, clientID, user_return);

	//create a project
	uint64_t t = (uint64_t)std::time(nullptr);
	Project_Create(user_return, clientID, ws2s(rmsurl), "RPMSpin_" + to_string(t), recipients, projectJson_return);
	jroot = ParseJsonFile(projectJson_return);
	statusCode = jroot["statusCode"].asInt();
	if (statusCode != 200) { LOG(INFO) << "Fail in RESTAPI - Project_Create"; }
	else
	{
		string tenantId = jroot["results"]["membership"]["tenantId"].asString();

		//SetClassification
		SetClassification(user_return, clientID, ws2s(rmsurl), tenantId, SetClassification_return);
		jroot = ParseJsonFile(SetClassification_return);
		statusCode = jroot["statusCode"].asInt();
		if (statusCode != 200 && statusCode != 201) { LOG(INFO) << "Fail in RESTAPI - Set Classification"; }
		else
		{
			//GetPolicyModels
			GetPolicyModels(user_return, clientID, ws2s(rmsurl), tenantId, PolicyModels_return);
			jroot = ParseJsonFile(PolicyModels_return);
			statusCode = jroot["statusCode"].asInt();
			if (statusCode != 200) { LOG(INFO) << "Fail in RESTAPI - Set GetPolicyModels"; }
			else {
				string PolicyModelsID = jroot["results"]["resource"]["id"].asString();
				string PolicyModelsName = jroot["results"]["resource"]["name"].asString();

				//PolicyDeploy
				PolicyDeploy(user_return, clientID, ws2s(rmsurl), PolicyModelsID, PolicyModelsName, tenantId, PolicyDeploy_return);
				jroot = ParseJsonFile(PolicyModels_return);
				statusCode = jroot["statusCode"].asInt();
				if (statusCode != 200) { LOG(INFO) << "Fail in RESTAPI - Set PolicyDeploy"; }
				else { LOG(INFO) << "Policy deploy success."; }
			}
		}
	}
	
}

//this part not ready
TEST(Policy, cleanDataInCC)
{
	string ccrul = "https://rms-cc81.qapf1.qalab01.nextlabs.com";
	string JSESSIONID = "E8B8DEA652E1557113B577C69934FD1C";
	string policyId = "298";
	string output_retur = "C:\\SDK test\\Policy\\policydelete_return.txt";
	DeletePolicyInCC(ccrul, JSESSIONID, policyId, output_retur);
}


string testFolder = TestDirectory + "TrustApp_Central";
TEST(TrustApp, App0)
{
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
	//string email = "testB@nextlabs.com";
	string email = "Lifi.yuan@nextlabs.com";
	string password = "123blue!";
	string passcode;
	/*cout << "Input User Email: ";
	cin >> email;
	cout << "Input password: ";
	cin >> password;
	if (password == "123")
		password = "123blue!";*/
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
		passcode = puser->GetPasscode();
		cout << "User's passcode:[" << passcode << "]" << endl;
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
		//wstring app1 = L"C:\\auto\\Clone-rmcsdk-wiki2\\QA_TestCode\\x64\\Debug\\New folder\\BApplong1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678.exe";
		wstring app1 = L"C:\\auto\\Clone-rmcsdk-wiki2\\QA_TestCode\\x64\\Debug\\New folder\\```````````````````````````````````````````````.exe";
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

		LOG(INFO) << "RPMRegisterApp_ " << ws2s(app1);
		res = pInstance->RPMRegisterApp(app1, secr);
		checkResult(res, 0);
		bool r;
		res = pInstance->RPMIsAppRegistered(app1, &r);
		checkResult(res, 0);

		cout << endl << "---------RPM login-----------" << endl;

		cout << "SetRPMLoginResult: ";
		DWORD start = ::GetTickCount();
		res = pInstance->SetRPMLoginResult(ReadFromFile(user_return), secr);
		DWORD Etime = ::GetTickCount() - start;
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "  Evaluation time: " << Etime << " (ms)" << endl;

		//cout << "SetRPMRDPDir: ";
		//res = pInstance->SetRPMPDPDir(L"C:\\Program Files\\NextLabs");
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		//cout << "SetRPMPolicyBundle: ";
		//res = pInstance->SetRPMPolicyBundle();
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		cout << "SyncUserAttributes: ";
		res = pInstance->SyncUserAttributes();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		wstring default_RPM;
		cout << "RPMGetSecretDir: [";
		res = pInstance->RPMGetSecretDir(default_RPM);
		wcout << default_RPM << "]" << endl; ;
		checkSDWLResult(res);

		//#define __Test__addRPMdir
#if defined(__Test__addRPMdir)
		DWORD t = GetTickCount();
		std::stringstream sstr;
		sstr << t;
		string s;
		sstr >> s;
		sstr.clear();
		string dir = "C:\\SDK test\\AddRPMtest\\autoadd\\" + s;
		createFolder(dir);
		cout << "AddRPMDir: [" << dir << L"]:  ";
		res = pInstance->AddRPMDir(s2ws(dir), 1);
		checkSDWLResult(res);
#endif
		res = pInstance->Save();
		EXPECT_EQ(0, res.GetCode());

		string iflogout = "";
		//string iflogout = "Y";
		cout << "Input Y do logout: ";
		cin >> iflogout;
		if (iflogout == "Y" || iflogout == "y" || iflogout == "yes") {
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
		}
#define __Test__GetLoginUserAfterLogout
#if defined(__Test__GetLoginUserAfterLogout)
		SDWLibDeleteRmcInstance(pInstance);

		ISDRmcInstance *pInstance_pre1 = NULL;
		ISDRmUser *puser_pre1 = NULL;
		ISDRmUser *puser_pre2 = NULL;
		CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance_pre1, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);

		cout << "GetLoginUser: ";
		res = pInstance_pre1->GetLoginUser(email, passcode, &puser_pre1);
		checkSDWLResult(res);
		if (!res.GetCode()) {
			cout << "Previous user login!" << endl;
			wcout << puser_pre1->GetEmail();
		}
		cout << "RPMGetLoginUser: ";
		res = pInstance_pre1->RPMGetLoginUser(secr, &puser_pre2);
		checkSDWLResult(res);
		if (!res.GetCode()) {
			cout << "Previous user login!" << endl;
			wcout << puser_pre2->GetEmail();
		}
		SDWLibDeleteRmcInstance(pInstance_pre1);
#endif
	}
#if defined(__Test__GetLoginUserAfterLogout)
#else
	SDWLibDeleteRmcInstance(pInstance);
#endif
	cout << "-------------This case finished--------" << endl << endl;
}

TEST(TrustApp, app1)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	ISDRmHttpRequest *prequest = NULL;
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

	ISDRmcInstance *pInstance_pre1 = NULL;
	ISDRmUser *puser_pre1 = NULL;
	ISDRmUser *puser_pre2 = NULL;
	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance_pre1, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	string email = "Lifi.yuan@nextlabs.com";
	string passcode;
	cout << "Input  plugin passcode: ";
	cin >> passcode;
	cout << "GetLoginUser: ";
	res = pInstance_pre1->GetLoginUser(email, passcode, &puser_pre1);
	checkSDWLResult(res);
	if (!res.GetCode()) {
		cout << "Previous user login!" << endl;
		wcout << puser_pre1->GetEmail();
	}
	cout << "RPMGetLoginUser: ";
	res = pInstance_pre1->RPMGetLoginUser(secr, &puser_pre2);
	checkSDWLResult(res);
	if (!res.GetCode()) {
		cout << "Previous user login!" << endl;
		wcout << puser_pre2->GetEmail();
	}
	SDWLibDeleteRmcInstance(pInstance_pre1);
	//LOG(INFO) << "SDWLibCreateSDRmcInstance: ";
	//res = SDWLibCreateSDRmcInstance(DEFUALT_SDKLib_FOLDER, DEFUALT_TEMP_FOLDER.c_str(), &pInstance);
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());


	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res) {
		wcout << L"puser info: " << puser->GetEmail() << endl;
	}

	cout << "GetLoginUser: ";
	res = pInstance->GetLoginUser(email, passcode, &puser);
	checkSDWLResult(res);
	if (!res.GetCode()) {
		cout << "Previous user login!" << endl;
		wcout << puser->GetEmail();
	}
	cout << "RPMGetLoginUser: ";
	res = pInstance->RPMGetLoginUser(secr, &puser);
	checkSDWLResult(res);
	if (!res.GetCode()) {
		cout << "Previous user login!" << endl;
		wcout << puser->GetEmail();
	}

	string r;
	cout << "Is RMX running(y/n):";
	cin >> r;
	bool running = true;
	if (r == "N" || r == "n")
		running = false;
	cout << "RPMNotifyRMXStatus: ";
	res= pInstance->RPMNotifyRMXStatus(running, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	////
	unsigned long pid;
	cout << "input pid for Add:";
	cin >> pid;
	res = pInstance->RPMAddTrustedProcess(pid, secr);
	cout << "Add " << pid << " result: ";
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	//
	string filep;
	cout << "input test file:";
	cin >> filep;

	//cout << "RPMReadFileTags:";
	//wstring returntag;
	//res = pInstance->RPMReadFileTags(s2ws(filep), returntag);
	//checkSDWLResult(res);
	//if(res)
	//{
	//	wcout << L"tags: " << returntag << endl;
	//}
	//EXPECT_EQ(0, res.GetCode());

	cout << "RPMEditSaveFile:";
	res = pInstance->RPMEditSaveFile(s2ws(filep), L"", false, 3);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	cout << "input pid for Remove:";
	cin >> pid;
	res = pInstance->RPMRemoveTrustedProcess(pid, secr);
	cout << "Remove " << pid << " result: ";
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	////
	string getrightsfile;
	std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
	std::vector<SDRmFileRight> filerights;
	while (1) {
		getrightsfile.clear();
		filerights.clear();
		rightsAndWatermarks.clear();

		cout << "Input file full path for GetRights: (input 1 to exit) ";
		cin >> getrightsfile;
		if (getrightsfile == "1") break;

		cout << "RPMGetFileRights(): ";
		res = pInstance->RPMGetFileRights(s2ws(getrightsfile), filerights);
		checkResult(res, 0);
		if (res)
		{
			for each(SDRmFileRight r in filerights)
				cout << "filerights: " << r << endl;
		}
		
		cout << "RPMGetRights(): ";
		res = pInstance->RPMGetRights(s2ws(getrightsfile), rightsAndWatermarks);
		checkResult(res, 0);
		printEvaluationResultRights(res, rightsAndWatermarks);


		cout << "PuserGetRights(): ";
		res = puser->GetRights(s2ws(getrightsfile), rightsAndWatermarks);
		checkResult(res, 0);
		printEvaluationResultRights(res, rightsAndWatermarks);

		//cout << "CacheRPMFileToken(): ";
		//res = pInstance->CacheRPMFileToken(s2ws(getrightsfile));
		//checkResult(res, 0);

	}

	////
	string deletefile;
	while (1) {
		deletefile.clear();
		cout << "Input deletefile full path: (input 1 to exit) ";
		cin >> deletefile;
		if (deletefile == "1") break;
		LOG(INFO) << "Delete NXL file in RPM dir:";
		res = pInstance->RPMDeleteFile(s2ws(deletefile));
		checkResult(res, 0);
	}

	string file;
	cout << "Input test file path: ";
	cin >> file;
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
	SDWLibDeleteRmcInstance(pInstance);
}

//
TEST(TrustApp, Start)
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
	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
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
	bool tenant_result = GetTenantResponseData(DEFUALT_ROUTER);
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
		wstring app1 = L"D:\\test\\Clone_rmcsdk-wiki3\\QA_TestCode\\Debug\\RMD-WindowsAPI-Gtest.exe";
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";


		cout << "Will trust app!" << endl;
		res = pInstance->RPMRegisterApp(app0, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		res = pInstance->RPMRegisterApp(app1, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << endl << "---------RPM login-----------" << endl;

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

TEST(TrustApp, app1_2)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	ISDRmHttpRequest *prequest = NULL;
	unsigned int dirstatus;
	bool filestatus;
	bool ret = false;
	string folder_name;
	cout << "Please input folder name:";
	cin >> folder_name;
	wstring safe_folder = s2ws(TestDirectory) + L"RPM\\" + s2ws(folder_name);
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	createFolder(ws2s(safe_folder));

	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	res = pInstance->SetRPMServiceStop(true, secr);
	//bool login_result = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
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

	res = pInstance->AddRPMDir(safe_folder);
	res = pInstance->AddRPMDir(L"E:\\SDK\\RPM");
	res = pInstance->SetRPMServiceStop(true, secr);
#pragma endregion

	if (res) {
		wcout << L"puser info: " << puser->GetEmail() << endl;
	}

	string r;
	cout << "Is RMX running(y/n):";
	cin >> r;
	bool running = true;
	if (r == "N" || r == "n")
		running = false;
	cout << "RPMNotifyRMXStatus: ";
	//wstring apppath = L"C:\\zhuzhu\\trustapp\\app1\\app1.exe";
	wstring apppath = L"D:\\test\\Clone_rmcsdk-wiki3\\QA_TestCode\\x64\\Debug\\RMD-WindowsAPI-Gtest.exe";
	res = pInstance->RPMRegisterApp(apppath, secr);
	res = pInstance->RPMNotifyRMXStatus(running, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	res = pInstance->AddRPMDir(L"C:\\SDK test\\RPM\\QQ");
	res = pInstance->AddRPMDir(safe_folder);
	res = pInstance->SetRPMServiceStop(true, secr);

	unsigned long pid1;
	cout << "input pid:";
	cin >> pid1;
	res = pInstance->RPMAddTrustedProcess(pid1, secr);

	string file1;
	cout << "Stop to edit file: ";
	cin >> file1;

	string file_name;
	cout << "input file name:";
	cin >> file_name;
	wstring file_namew = s2ws(file_name);
	//res = pInstance->RPMEditCopyFile(L"C:\\SDK test\\RPM\\" + file_namew, safe_folder);
	//checkSDWLResult(res);
	//EXPECT_EQ(0, res.GetCode());

	//unsigned long pid;
	//cout << "input pid:";
	//cin >> pid;
	//res = pInstance->RPMAddTrustedProcess(pid, secr);

	//res = pInstance->RPMEditSaveFile(L"C:\\SDK test\\RPM\\QQ\\wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww305adhocFile-2018-12-29-07-44-13.txt", L"", true, 3);

	res = pInstance->RPMEditSaveFile(L"C:\\SDK test\\RPM\\QQ\\" + file_namew, L"", false, 3);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	cout << "input pid:";
	cin >> pid1;
	res = pInstance->RPMAddTrustedProcess(pid1, secr);

	cout << "Stop to edit file: ";
	cin >> file1;

	cout << "input pid:";
	cin >> pid1;
	res = pInstance->RPMAddTrustedProcess(pid1, secr);

	cout << "Stop to edit file: ";
	cin >> file1;

	//string file;
	//cout << "Input test file path: ";
	//cin >> file;
	//int charNum = 35;
	//cout << "Read file>>>";
	//ifstream infile;
	//infile.open(file.data());
	//string content = "";

	//if (infile.is_open()) {

	//	infile >> noskipws;
	//	int i = 0;
	//	char c;

	//	while (!infile.eof() && i < charNum)
	//	{
	//		i++;
	//		infile >> c;
	//		content.push_back(c);
	//	}
	//	infile.close();
	//	cout << content << endl;
	//}
	//else
	//{
	//	cout << "[ERROR]: the test file cannot open, please check." << endl;
	//}
	SDWLibDeleteRmcInstance(pInstance);

}
//
TEST(TrustApp, App_getloginuser)
{
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
	string email = "Lifi.yuan@nextlabs.com";
	string passcode;
	cout << "Input User Email: ";
	cin >> email;
	cout << "Input passcode: ";
	cin >> passcode;

	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	waitInstanceInitFinish(pInstance);
	

	cout << "SDK GetLoginUser: ";
	res = pInstance->GetLoginUser(email, passcode, &puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	if (res) {
		
		cout << "----------RPM------------" << endl;	
		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);
		cout << "SetRPMLoginResult: ";
		DWORD start = ::GetTickCount();
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		DWORD Etime = ::GetTickCount() - start;
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "  Evaluation time: " << Etime << " (ms)" << endl;

		//cout << "SetRPMRDPDir: ";
		//res = pInstance->SetRPMPDPDir(L"C:\\Program Files\\NextLabs");
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		//cout << "SetRPMPolicyBundle: ";
		//res = pInstance->SetRPMPolicyBundle();
		//checkSDWLResult(res);
		//EXPECT_EQ(0, res.GetCode());

		cout << "SyncUserAttributes: ";
		res = pInstance->SyncUserAttributes();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());


		res = pInstance->Save();
		EXPECT_EQ(0, res.GetCode());

		string iflogout = "";
		cout << "Input Y do logout: ";
		cin >> iflogout;
		if (iflogout == "Y" || iflogout == "y" || iflogout == "yes") {
			cout << "RPMLogout: ";
			DWORD start_logout = ::GetTickCount();
			res = pInstance->RPMLogout();
			Etime = ::GetTickCount() - start_logout;
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			cout << "  Evaluation time: " << Etime << " (ms)" << endl;

			cout << endl << "SDK user logout:";
			res = puser->LogoutUser();
			checkSDWLResult(res);
		}
	}
	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}


TEST(TrustApp, AppRegister)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	ISDRmHttpRequest *prequest = NULL;
	string secr;
	string app;

#define _CreateInstance_
#if defined(_CreateInstance_)
	LOG(INFO) << "SDWLibCreateSDRmcInstance: ";
	res = SDWLibCreateSDRmcInstance(DEFUALT_SDKLib_FOLDER, DEFUALT_TEMP_FOLDER.c_str(), &pInstance);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
#else
	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res) {
		wcout << L"puser info: " << puser->GetEmail() << endl;
	}
#endif
	cout << "-------------" << endl;
	for each(vector<string> csv_line in csv_appPath)
	{
		if (csv_line[0] == "1") {
			app = csv_line[1];
			secr = csv_line[2];
			cout << "RPMRegisterApp: [" << app << "]" << endl;
			cout << "With secr string: " << secr << endl;
			res = pInstance->RPMRegisterApp(s2ws(app), secr);
			checkSDWLResult(res);
			cout << endl;
		}
		else if (csv_line[0] == "2") {
			app = csv_line[1];
			secr = csv_line[2];
			cout << "Un-registerApp: [" << app << "]" << endl;
			cout << "With secr string: " << secr << endl;
			res = pInstance->RPMUnregisterApp(s2ws(app), secr);
			checkSDWLResult(res);
			cout << endl;
		}
		else if (csv_line[0] == "3")
		{
			app = csv_line[1];
			bool reg=true;
			cout <<"RPMIsAppRegistered: " << endl;
			res = pInstance->RPMIsAppRegistered(s2ws(app), &reg); 
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			cout << "Is [" << app << "]" << "AppRegistered: " <<reg<< endl;
			cout << endl;
		}
		else if (csv_line[0] == "4")
		{
			app = csv_line[1];
			secr = csv_line[2];
			bool reg = true;
			res = pInstance->RPMRegisterApp(apppath, "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}");
			cout << "RPMNotifyRMXStatus: ";
			res = pInstance->RPMNotifyRMXStatus(true, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			cout << "RPMAddTrustedProcess: ";
			res = pInstance->RPMAddTrustedProcess(123, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());


			cout << "RPMRemoveTrustedProcess: ";
			res = pInstance->RPMAddTrustedProcess(1, "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}");
			res = pInstance->RPMRemoveTrustedProcess(1, secr);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			cout << endl;
		}
	}



	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}


TEST(RPMEditSaveTCTest999, EditCopySaveTCTest)
{
	bool ret = false;
	SDWLResult res;
	ISDRmcInstance * pInstance;
	ISDRmTenant * pTenant;
	ISDRmUser *puser = NULL;

	string passcode = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	res = RPMGetCurrentLoggedInUser(passcode, pInstance, pTenant, puser);

	pInstance->IsInitFinished(ret);

	res = pInstance->RPMRegisterApp(L"C:\\Jenkins\\Jobs\\rmc-wiki\\QA_TestCode\\x64\\Debug\\RMD-WindowsAPI-Gtest.exe", passcode);
	res = pInstance->RPMNotifyRMXStatus(true, passcode);

	wstring hidden_safefolder = L""; // L"C:\\Users\\Administrator\\AppData\\Local\\SkyDRM\\home\\rms-centos7303.qapf1.qalab01.nextlabs.com\\john.tyler@qapf1.qalab01.nextlabs.com\\RPM";
	wstring sourcefile = L"E:\\BugVerify\\1310\\AD2.txt.nxl";
	//wstring sourcefile = L"C:\\Users\\Administrator\\Desktop\\New folder\\abc-2018.doc.nxl";

	uint32_t fixedfsize = 0;
	try
	{
		// sourcefile = L"/SDK test/abc.txt";
		std::experimental::filesystem::path exactpath(sourcefile);
		wstring prepath = exactpath.make_preferred();
		prepath = exactpath.native();
		ret = std::experimental::filesystem::is_regular_file(sourcefile);
		fixedfsize = std::experimental::filesystem::file_size(sourcefile);
	}
	catch (...) {}

	int i = 0;
	for (i = 0; i < 100; i++)
	{
		cout << i << endl;
		ISDRmNXLFile* pFile_local = NULL;
		res = puser->OpenFile(sourcefile, &pFile_local);

		hidden_safefolder = L"";
		res = pInstance->RPMEditCopyFile(sourcefile, hidden_safefolder);
		if (!res)
			break;
		EXPECT_FALSE(res.GetCode());

		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		ret = pInstance->RPMGetRights(hidden_safefolder, rightsAndWatermarks);
		EXPECT_FALSE(res.GetCode());

		res = pInstance->RPMEditSaveFile(hidden_safefolder, L"", true, 3);
		EXPECT_FALSE(res.GetCode());
		if (!res)
			break;
		uint32_t updatedfsize = std::experimental::filesystem::file_size(sourcefile);

		if (updatedfsize > (fixedfsize + 1024))
		{
			// file size different
			cout << "------------- RPMEditSaveFile error --------" << endl << endl;
			cout << "------------- running times = " << i << endl;
			cout << "------------- original file size = " << fixedfsize << endl;
			cout << "------------- updated file size = " << updatedfsize << endl;
			cout << "-------------------------------------" << endl << endl;

			break;
		}
	}

	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << i << endl << endl;
}


//Login with John.tyler
TEST(RPM, john)
{
	string testFolder = TestDirectory + "John";
	ISDRmcInstance * pInstance;
	ISDRmTenant * pTenant;

	bool ret = false;
	SDWLResult res;
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
	string clientid = "";
	wstring f = s2ws(testFolder);

	cout << "CreateInstance: ";
	//res = SDWLibCreateSDRmcInstance(DEFUALT_SDKLib_FOLDER, f.c_str(), &pInstance, (char*)clientid.c_str(), id);
	res = SDWLibCreateSDRmcInstance(DEFUALT_SDKLib_FOLDER, f.c_str(), &pInstance);
	checkSDWLResult(res);

	string workingfoler = ws2s(f) + "\\working";
	createFolder(workingfoler);
	cout << "Initialize: ";
	res = pInstance->Initialize(s2ws(workingfoler), DEFUALT_ROUTER, DEFUALT_TENANT);
	checkSDWLResult(res);

	waitInstanceInitFinish(pInstance);
	string user_return = "C:\\SDK test data\\john.tyler.json";

	LOG(INFO) << "GetLoginRequest: ";
	ISDRmHttpRequest *prequest = NULL;
	res = pInstance->GetLoginRequest(&prequest);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	LOG(INFO) << "\tGetPath():" << ws2s(prequest->GetPath()) << endl;
	if (res.GetCode() == 0)
	{
		LOG(INFO) << "-----LoginRequest-----" << endl;
		for each (HttpCookie cookie in prequest->GetCookies())
		{
			LOG(INFO) << "Cookie name:[" << ws2s(cookie.first) << "]";
			LOG(INFO) << "  Vaule:[" << ws2s(cookie.second) << "]" << endl;

			if (ws2s(cookie.first) == "clientId")
			{
				clientid = ws2s(cookie.second);
				string exportfilename = plan_path + "client_id.txt";
				ofstream SaveFile(exportfilename, ios::ate);
				SaveFile << clientid;// << endl;
				SaveFile.close();
			}
		}
	}

	cout << "Set SDK LoginResult: ";
	string s = ReadFromFile(user_return);
	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	if (res) {
		cout << "GetHeartBeatInfo: ";
		SDWLResult res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//for System 
		string SystemProjectTenantId = puser->GetSystemProjectTenantId();
		std::vector<SDR_CLASSIFICATION_CAT> cats;
		res = puser->GetClassification(SystemProjectTenantId,cats);
		pInstance->GetCurrentTenant(&pTenant);
		wstring t = pTenant->GetTenant();
		res = puser->GetClassification(ws2s(t), cats);

		//
		res = pInstance->Save();
		EXPECT_EQ(0, res.GetCode());

		string getrightsfile;
		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

		while (1) {
			getrightsfile.clear();
			cout << "Input file full path for GetRights: (input 1 to exit) ";
			cin >> getrightsfile;
			if (getrightsfile == "1") break;

			
			cout << "SDK GetRights(): ";
			rightsAndWatermarks.clear();
			puser->GetRights(s2ws(getrightsfile), rightsAndWatermarks);
			checkResult(res, 0);
			printEvaluationResultRights(res, rightsAndWatermarks);
		}
		

		cout << "----------RPM------------" << endl;

		ret = pInstance->IsRPMDriverExist();
		cout << "IsRPMDriverExist: " << ret << endl;
		if (!ret)
		{
			cout << "Please double check if the RPM build is installed!" << endl;
		}
		//string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

		cout << endl << "---------RPM login-----------" << endl;

		cout << "SetRPMLoginResult: ";
		DWORD start = ::GetTickCount();
		res = pInstance->SetRPMLoginResult(ReadFromFile(user_return), secr);
		DWORD Etime = ::GetTickCount() - start;
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		cout << "  Evaluation time: " << Etime << " (ms)" << endl;

		cout << "SyncUserAttributes: ";
		res = pInstance->SyncUserAttributes();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());


		res = pInstance->Save();
		EXPECT_EQ(0, res.GetCode());

		string iflogout = "";
		cout << "Input Y do logout: ";
		cin >> iflogout;
		if (iflogout == "Y" || iflogout == "y" || iflogout == "yes") {
			cout << "RPMLogout: ";
			DWORD start_logout = ::GetTickCount();
			res = pInstance->RPMLogout();
			Etime = ::GetTickCount() - start_logout;
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			cout << "  Evaluation time: " << Etime << " (ms)" << endl;

			cout << endl << "SDK user logout:";
			res = puser->LogoutUser();
			checkSDWLResult(res);
		}
	}
	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}

string JohntestFolder = TestDirectory + "John";
string Johnworkingfoler = JohntestFolder + "\\working";
string JohnEmail = "";
string Johnpasscode = "";
string useremail = plan_path + "Johninfo_email.txt";
string userpasscode = plan_path + "Johninfo_passcode.txt";
TEST(Offline, firstLogin)
{
	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	createFolder(JohntestFolder);
	ISDRmUser *puser = NULL;
	//string clientid = "CBF7841D45E5F175A0EDCD4B248AB398";
	wstring f = s2ws(JohntestFolder);
	
	string clientID = "";

	res = CreateSDRmcInstance_CreateClientID(s2ws(JohntestFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	/*cout << "CreateInstance: ";
	res = SDWLibCreateSDRmcInstance(DEFUALT_SDKLib_FOLDER, f.c_str(), &pInstance);
	checkSDWLResult(res);

	createFolder(Johnworkingfoler);
	cout << "Initialize: ";
	res = pInstance->Initialize(s2ws(Johnworkingfoler), DEFUALT_ROUTER, DEFUALT_TENANT);
	checkSDWLResult(res);*/

	waitInstanceInitFinish(pInstance);
	string user_return = "C:\\SDK test data\\john.tyler.json";
	string email = "Lifi.yuan@nextlabs.com";
	string password = "123blue!"; md5(password);
	GetUserLoginResponseData(email, password, clientID, user_return);

	cout << "Set SDK LoginResult: ";
	string s = ReadFromFile(user_return);
	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	if (res) {
		JohnEmail = ws2s(puser->GetEmail());
		ofstream SaveFile1(useremail, ios::ate);
		ofstream SaveFile2(userpasscode, ios::ate);
		SaveFile1 << JohnEmail;
		SaveFile1.close();
		Johnpasscode = puser->GetPasscode();
		SaveFile2 << Johnpasscode;
		SaveFile2.close();

		cout << "GetHeartBeatInfo: ";
		SDWLResult res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//
		SDR_Expiration expire;
		SDR_WATERMARK_INFO watermarkinfo;
		expire.type = IExpiryType::NEVEREXPIRE;
		std::vector<SDRmFileRight> rights;
		wstring newcreatedfilePath = L"";
		string rights_str = "view|edit|download";
		rights = Tick_Rights(rights_str);
		std::string tags = "";
		//std::vector<std::string> recipients = { "testA@test.com", "testB@nextlabs.com" };
		wstring recipients =L"testA@test.com,testB@nextlabs.com";
		wstring comments_w = L"Comments to recipients";
		cout << "ShareFile: ";
		res = puser->ProtectFile(L"C:\\SDK test data\\Normal_TXT.txt", newcreatedfilePath, rights, watermarkinfo, expire, tags);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		Sleep(1000);
		LOG(INFO) << "UploadFile: ";
		res = puser->UploadFile(newcreatedfilePath, newcreatedfilePath, recipients, comments_w);
		checkResult(res, 0);
		//

		res = pInstance->Save();
		EXPECT_EQ(0, res.GetCode());

		string passcode = puser->GetPasscode();
		string loginInfo; res = pInstance->GetLoginData(email, passcode, loginInfo);
		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(loginInfo, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << "SyncUserAttributes: ";
		res = pInstance->SyncUserAttributes();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//cout << "SetRPMCachedToken: ";
		//res = pInstance->SetRPMCachedToken();
		//checkSDWLResult(res);
		//cout << "CacheRPMFileToken: ";
		//res = pInstance->CacheRPMFileToken(L"F:\\includingRPM\\Safe4\\Central.txt");
		//checkSDWLResult(res);


		res = pInstance->Save();
		EXPECT_EQ(0, res.GetCode());
	}
	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}

TEST(Offline, GetLogin)
{
	std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
	SDWLResult res;
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	string clientID = "";

	res = CreateSDRmcInstance_CreateClientID(s2ws(JohntestFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);

	waitInstanceInitFinish(pInstance);

	JohnEmail = ReadFromFile(useremail);
	Johnpasscode = ReadFromFile(userpasscode);
	cout << "GetLoginUser(): ";
	res = pInstance->GetLoginUser(JohnEmail, Johnpasscode, &puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	if (res) {
		cout << "GetHeartBeatInfo: ";
		SDWLResult res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		//
		SDR_Expiration expire;
		SDR_WATERMARK_INFO watermarkinfo;
		expire.type = IExpiryType::NEVEREXPIRE;
		std::vector<SDRmFileRight> rights;
		wstring newcreatedfilePath = L"";
		string rights_str = "view|edit|download";
		rights = Tick_Rights(rights_str);
		std::string tags = "";
		std::vector<std::string> recipients = { "testA@test.com", "testB@nextlabs.com" };
		wstring comments_w = L"Comments to recipients";
		cout << "ProtectFile: ";
		res = puser->ProtectFile(L"C:\\SDK test data\\Normal_TXT - Copy.txt", newcreatedfilePath, rights, watermarkinfo, expire, tags);
		checkSDWLResult(res);
		Sleep(1000);
		EXPECT_EQ(0, res.GetCode());

		//cout << "ShareFile: ";
		//res = puser->ShareFile(L"C:\\SDK test data\\Normal_TXT - Copy.txt", newcreatedfilePath, rights, recipients, comments_w, watermarkinfo, expire, tags);
		//checkSDWLResult(res);
		//Sleep(1000);
		//EXPECT_EQ(0, res.GetCode());

		string passcode = puser->GetPasscode();
		string loginInfo; res = pInstance->GetLoginData(email, passcode, loginInfo);
		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(loginInfo, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

	}
	string getrightsfile = "F:\\includingRPM\\Safe4\\Central.txt";
	rightsAndWatermarks.clear();
	cout << "SDK GetRights(): ";
	puser->GetRights(s2ws(getrightsfile), rightsAndWatermarks);
	checkResult(res, 0);
	printEvaluationResultRights(res, rightsAndWatermarks);

	/*while (1) {
		getrightsfile.clear();
		cout << "Input file full path for GetRights: (input 1 to exit) ";
		cin >> getrightsfile;
		if (getrightsfile == "1") break;


		cout << "SDK GetRights(): ";
		rightsAndWatermarks.clear();
		puser->GetRights(s2ws(getrightsfile), rightsAndWatermarks);
		checkResult(res, 0);
		printEvaluationResultRights(res, rightsAndWatermarks);
	}
*/
	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}

//TEST(RPM, RPMSetAppRegistry)
//{
//	ISDRmcInstance * pInstance;
//	SDWLResult res;
//	ISDRmUser *puser = NULL;
//	ISDRmTenant *pTenant = NULL;
//	ISDRmHttpRequest *prequest = NULL;
//	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
//
//	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
//	checkSDWLResult(res);
//	EXPECT_EQ(0, res.GetCode());
//	if (res) {
//		wcout << L"puser info: " << puser->GetEmail() << endl;
//	}
//	std::wstring subkey= L"SOFTWARE\\NextLabs\\lifitest2";
//	std::wstring name=L"testname2";
//	std::wstring data = L"testdata";
//	uint32_t op = 0;
//	cout << "RPMSetAppRegistry(0): ";
//	res = pInstance->RPMSetAppRegistry(subkey, name, data, op);
//	checkSDWLResult(res);
//
//	op = 1;
//	cout << "RPMSetAppRegistry(1): ";
//	res = pInstance->RPMSetAppRegistry(subkey, name, data, op);
//	checkSDWLResult(res);
//}

TEST(Login, AD)
{
	string rmsurl = "https://jenkins-centos7302.qapf1.qalab01.nextlabs.com:8444";
	string tenant = "cc3c1b11-52b3-4093-aa16-0865df9e93ab";
	string clientID = "EB74F97A76F8455107468E170E67CDB5";
	string username = "john.tyler";
	string userid = "233";
	string output_UserJson = "C:\\SDK test data\\john.tyler.json";
	bool result = LoginWithADUser(rmsurl, tenant, clientID, username, userid, output_UserJson);
	EXPECT_TRUE(result);

}


TEST(RPM, forTCtesting)
{
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	ISDRmHttpRequest *prequest = NULL;
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res) {
		wcout << L"puser info: " << puser->GetEmail() << endl;
	}

	string dir;
	cout << "Input dir: ";
	cin >> dir;
	wstring wdir = s2ws(dir);
	//wcout << L"AddRPMDir: [" << wdir << L"]:  ";
	//res = pInstance->AddRPMDir(wdir, 1);
	//checkSDWLResult(res);

	unsigned int dirstatus;
	wcout << L"IsRPMFolder: [" << wdir << L"]:  ";
	res = pInstance->IsRPMFolder(wdir, &dirstatus);
	cout << "Dir is: [" << dirstatus << "]"<< endl;
	checkSDWLResult(res);

	SDWLibDeleteRmcInstance(pInstance);
}


string RPMResetNXLinFolder_ExpectedFileName(string orgName)
{
	//NXL file name in RPMdir (1)hidden last".nxl", (2) remove non-extension ".nxl" 
	string eName = "";

	int pos = orgName.find_last_of('.');
	string filetype(orgName.substr(pos + 1));
	string filename(orgName.substr(0, pos));

	if (filetype != "nxl")
		eName= orgName;
	else
	{
		pos = filename.find_last_of('.');
		string fileSecType(filename.substr(pos));
		string fileRealName(filename.substr(0, pos));
		replaceA2B(fileRealName, ".nxl", "");
		replaceA2B(fileSecType, ".nxl ", " ");
		eName = fileRealName + fileSecType;
	}
	LOG(INFO) << "ExpectedFileName in RPMdir: " << eName;

	return eName;
}

void CreateFile_contentIsFileName(string fileFullpath)
{
	LOG(INFO) << "Create file " << fileFullpath;
	DWORD dwWrite;
	char *tmpBuf;
	tmpBuf = (char*)fileFullpath.c_str();
	HANDLE hFILE = CreateFile(s2ws(fileFullpath).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!WriteFile(hFILE, tmpBuf, strlen(fileFullpath.c_str()), &dwWrite, NULL))
	{
		LOG(INFO) << "WriteFile erro";
	}
	CloseHandle(hFILE);
}
void RefreshDir4File(string dir, string file)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA pNextInfo;
	wstring wildcardPath = s2ws(dir + "\\" + file);
	hFile = FindFirstFile(wildcardPath.c_str(), &pNextInfo);
}
TEST(RPM_Explorer, resetNXLfileName)
{
#pragma region[init]
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	//string password=md5_s(password_plain);
	//wstring tenantName;
	//string passcode;

	LOG(INFO) << "RPMGetCurrentLoggedInUser: ";
	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	checkSDWLResult(res);
	//if (!res) {
	//	userFirstTimeLogin(email_global, password, TestDirectory + "ExplorerAddon", tenantName, passcode);

	//	LOG(INFO) << "GetLoginUser: ";
	//	res = pInstance->GetLoginUser(email_global, passcode, &puser);
	//	checkSDWLResult(res);
	//	EXPECT_EQ(0, res.GetCode());
	//	if (res)
	//	{
	//		LOG(INFO) << "SetRPMLoginResult: ";
	//		res = pInstance->SetRPMLoginResult(pInstance->GetLoginData());
	//		checkSDWLResult(res);
	//		EXPECT_EQ(0, res.GetCode());
	//	}
	//}
	//wcout << L"puser info: " << puser->GetEmail() << endl;
#pragma endregion
#pragma region[TestData]
	struct testData {
		std::string caseId;
		std::string caseName;
		std::string testPath;
		std::string testFileName;
		std::string ExpectedFileName;
		std::string SourcePath;
		std::string SourceFile;
	};
	testData p;
#pragma endregion
	//Case: (1)hidden last".nxl", (2) remove non-extension ".nxl" 
	for each(vector<string> csv in csv_Auto_ExplorerAddon)
	{
		if (csv[0] == "1" && string2Int(csv[3]) <= RunType)
		{
#pragma region[read data from csv]
			p.caseId = csv[1];
			p.caseName = csv[2];			
			replace(csv[4].begin(), csv[4].end(), '<', ',');
			p.testPath = csv[4];
			replace(csv[5].begin(), csv[5].end(), '<', ',');
			p.testFileName = csv[5];
			p.SourcePath = csv[6];
			p.SourceFile = csv[7];
#pragma endregion
			cout << "======= Case[" <<p.caseName<<"]======" << endl;

			//Create test folder
			createFolder(p.testPath);
			//Prepare file, rename file name into the test one
			wstring testFile = s2ws(p.testPath + "\\" + p.testFileName);
			wstring destNXL;
			CopyviaCMD(p.SourcePath + "\\" + p.SourceFile, p.testPath + "\\" + p.testFileName);
			Sleep(1000);
			//AddRPMdir
			LOG(INFO) << "AddRPMDir: " << p.testPath;
			res = pInstance->AddRPMDir(s2ws(p.testPath));
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			if (res)
			{
				Sleep(1000);
				p.ExpectedFileName = RPMResetNXLinFolder_ExpectedFileName(p.testFileName);

				LOG(INFO) << "Check Result: ";

				RefreshDir4File(p.testPath, p.ExpectedFileName+".nxl");
				EXPECT_TRUE(checkFileExist(p.testPath, p.ExpectedFileName));

				//removeRPMdir
				LOG(INFO) << "RemoveRPMDir: " << p.testPath;
				res = pInstance->RemoveRPMDir(s2ws(p.testPath), true);
				checkSDWLResult(res);
				EXPECT_EQ(0, res.GetCode());

				if (res) {
					Sleep(500);
					//check test files again
					RefreshDir4File(p.testPath, p.ExpectedFileName + ".nxl");
					RefreshDir4File(p.testPath, p.ExpectedFileName);
					if (p.ExpectedFileName != p.testFileName)
					{
						EXPECT_TRUE(checkFileExist(p.testPath, p.ExpectedFileName + ".nxl"));
						EXPECT_FALSE(checkFileExist(p.testPath, p.ExpectedFileName));
					}
					else {
						EXPECT_TRUE(checkFileExist(p.testPath, p.ExpectedFileName));
					}
				}

			}
			//Cleanup test env
			string command = "DEL \"" + p.testPath + "\" /Q";
			system(command.c_str());

			command = "RD \"" + p.testPath+"\"";
			system(command.c_str());
			cout << "======= Case END ======" << endl<< endl;

		}
	}

	//Case: (3)rename the same name file if existing
	if (1) {
		string dir = TestDirectory + "ExplorerAddon2";
		string ftext = "bbfilename"; //don't input ".nxl" here
		string ftype = ".txt";
		string nxl = ".nxl";

		//Create Folder
		createFolder(dir);
		//Create test files
		string f = ftext + ftype;                  //f.txt        --> f-(3).txt
		string new_f =  ftext + "-(3)" + ftype;
		string fnxl = f + nxl;                     //f.txt.nxl    --> f-(4).txt
		string new_fnxl = ftext + "-(4)" + ftype;
		string f1 =  ftext + "-(1)" + ftype;       //f-(1).txt    --> f-(2).txt
		string new_f1 =  ftext + "-(2)" + ftype;
		string f1nxl = f1 + nxl;                   //f-(1).txt.nxl  --> f-(1).txt
		string new_f1nxl = f1;
		string fnnxl =  ftext + nxl + ftype + nxl; //f.nxl.txt.nxl  --> f.txt
		string new_fnnxl =  f;

		CreateFile_contentIsFileName(dir+"\\"+f);
		CreateFile_contentIsFileName(dir + "\\" + fnxl);
		CreateFile_contentIsFileName(dir + "\\" + f1);
		CreateFile_contentIsFileName(dir + "\\" + f1nxl);
		CreateFile_contentIsFileName(dir + "\\" + fnnxl);
		//AddRPMdir
		LOG(INFO) << "AddRPMDir " << dir;
		res = pInstance->AddRPMDir(s2ws(dir));
		checkSDWLResult(res);
		if (res) {
			Sleep(3000);
			RefreshDir4File(dir, ftext + "*");

			//Check files
			cout << endl << "dest: " << new_f << endl;
			EXPECT_TRUE(checkFileExist(dir, new_f));
			EXPECT_STREQ(readFileContent(dir, new_f).c_str(), (dir + "\\" + f).c_str());
			
			cout << endl << "dest: " << new_fnxl << endl;
			EXPECT_TRUE(checkFileExist(dir, new_fnxl));
			EXPECT_STREQ(readFileContent(dir, new_fnxl).c_str(), (dir + "\\" + fnxl).c_str());
			
			cout << endl << "dest: " << new_f1 << endl;
			EXPECT_TRUE(checkFileExist(dir, new_f1));
			EXPECT_STREQ(readFileContent(dir, new_f1).c_str(), (dir + "\\" + f1).c_str());
			
			cout << endl << "dest: " << new_f1nxl << endl;
			EXPECT_TRUE(checkFileExist(dir, new_f1nxl));
			EXPECT_STREQ(readFileContent(dir, new_f1nxl).c_str(), (dir + "\\" + f1nxl).c_str());
			
			cout << endl << "dest: " << new_fnnxl << endl;
			EXPECT_TRUE(checkFileExist(dir, new_fnnxl));
			EXPECT_STREQ(readFileContent(dir, new_fnnxl).c_str(), (dir + "\\" + fnnxl).c_str());
			
			cout << endl;
			//RemoveRPMdir
			LOG(INFO) << "RemoveRPMDir " << dir;
			res = pInstance->RemoveRPMDir(s2ws(dir));
			checkSDWLResult(res);
		}
		//Cleanup test env
		Sleep(100);

		string command = "DEL \"" + dir + "\" /Q";
		system(command.c_str());

		command = "RD \"" + dir + "\"";
		system(command.c_str());
		cout << "======= Case END ======" << endl << endl;
	}

	SDWLibDeleteRmcInstance(pInstance);
}




bool RPM_Explorer_DirActions(string action, string dir, int dirstatus, string destDir, bool destRPM=false)
{
	BOOL result;
	if (action == "delete"){
		result = RemoveDirectory(s2ws(dir).c_str());
	}
	else {
		result = MoveFileEx(s2ws(dir).c_str(), s2ws(destDir).c_str(), MOVEFILE_COPY_ALLOWED);
	}

	//result = MoveFile(s2ws(dir).c_str(), s2ws(destDir).c_str());
	if (action == "move" && !destRPM && dirstatus == 2 && !result)
	{
		LOG(INFO) << "Block " << action << " success.";
		return true;
	}
	else if (dirstatus >= 2 && result)
	{
		LOG(INFO) << "Do " << action << " success.";
		return true;
	}
	else if (dirstatus <= 1 && !result)
	{
		LOG(INFO) << "Block " << action << " success.";
		return true;
	}
	else {
		LOG(INFO) << "Case Fail, see below info:";
		LOG(INFO) << action <<" "<< dir << " to " << destDir << " result: " << result;
		return false;
	}
}
//move, rename, delete RPMdir
class RPM_Explorer :public::testing::TestWithParam<vector<string>> {};
TEST_P(RPM_Explorer, operateDir)
{
#pragma region[read csv data]
	vector<string> csv = GetParam();
	if (csv[0] == "1" && string2Int(csv[3]) <= RunType)
	{
		string casename = csv[2];
		string caseId = csv[1];
		string dir = csv[4];
		int dirstatus = string2Int(csv[5]);
		string action = csv[6];
		StringToLower(action);
		string destType = csv[7];
		StringToLower(destType);

		LOG(INFO) << endl;
		LOG(INFO) << "====== Case: [" << caseId << "]" << casename << " ======";
#pragma endregion
#pragma region[init]
		ISDRmcInstance * pInstance;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		ISDRmTenant *pTenant = NULL;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		LOG(INFO) << "RPMGetCurrentLoggedInUser: ";
		res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
		checkSDWLResult(res);
#pragma endregion
		bool run = true;
		wstring RPMdir = L"";
		string pdir;
		string sdir = dir + "\\sub";
		if (getParentdir(dir, pdir))
		{
			switch (dirstatus)
			{
			case 0: RPMdir = s2ws(dir); break;
			case 2:
					RPMdir = s2ws(pdir);
					break;
			case 1:
				createFolder(sdir);
				RPMdir = s2ws(sdir);
				break;
			case 3:
				break;
			default:
				LOG(INFO) << "Unsupport dir type for this case, please check test data.";
				run = false;
				break;
			}

			if (run) {
				createFolder(dir);
				if (RPMdir != L"")
				{
					LOG(INFO) << "AddRPMDir"<<ws2s(RPMdir);
					res = pInstance->AddRPMDir(RPMdir);
					checkSDWLResult(res);
				}
				
				string destDir = "";
				if (action == "rename")
				{
					destDir = dir + "_renamed";
					EXPECT_TRUE(RPM_Explorer_DirActions(action, dir, dirstatus, destDir));
				}
				else if (action == "move")
				{
					bool destBool = false;
					if (destType == "rpm")
					{
						destDir = pdir + "\\destRPMdir";
						createFolder(destDir);
						res = pInstance->AddRPMDir(s2ws(destDir));
						destBool = true;
					}
					else
					{
						destDir = pdir + "\\destNONdir";
						createFolder(destDir);
					}
					EXPECT_TRUE(RPM_Explorer_DirActions(action, dir, dirstatus, destDir, destBool));
					if (destBool)
					{
						res = pInstance->RemoveRPMDir(s2ws(destDir));
					}
				}

				else if (action == "delete")
				{
					EXPECT_TRUE(RPM_Explorer_DirActions(action, dir, dirstatus, destDir));
				}
				else if (action == "copy")
				{

				}
				else
				{
					LOG(INFO) << "Unsupport action, please check the test data.";

				}
			}

		}
		else
		{
			LOG(INFO) << "The input test dir have problem, please check test data.";
		}
		//RemoveRPMdir
		if (RPMdir != L"")
		{
			LOG(INFO) << "RemoveRPMDir" << ws2s(RPMdir);
			res = pInstance->RemoveRPMDir(RPMdir);
			checkSDWLResult(res);
		}
		//Cleanup test env
	}
}
INSTANTIATE_TEST_CASE_P(RPMTest, RPM_Explorer, testing::ValuesIn(csv_Auto_ExplorerAddon2));



////10.5release
//TEST(V105, RPMRequestLogin)
//{
//#pragma region[init]
//	ISDRmcInstance * pInstance;
//	SDWLResult res;
//	ISDRmUser *puser = NULL;
//	ISDRmTenant *pTenant = NULL;
//	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
//
//	LOG(INFO) << "RPMGetCurrentLoggedInUser: ";
//	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
//	checkSDWLResult(res);
//#pragma endregion
//	wstring callback_cmd= L"C:\\Program Files\\Microsoft Office\\Office15\\POWERPNT.EXE";
//	wstring callback_cmd_param;
//
//	res=pInstance->RPMRequestLogin(callback_cmd, callback_cmd_param);
//	checkSDWLResult(res);
//	
//	res = pInstance->RPMRequestLogout(1);
//	checkSDWLResult(res);
//
//
//	CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
//	waitInstanceInitFinish(pInstance);
//	string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
//	GetUserLoginResponseData(email, password, clientID, user_return);
//
//	cout << "Set SDK LoginResult: ";
//	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
//	checkSDWLResult(res);
//	EXPECT_EQ(0, res.GetCode());
//
//	if (res) {
//		cout << "SetRPMLoginResult: ";
//		DWORD start = ::GetTickCount();
//		res = pInstance->SetRPMLoginResult(ReadFromFile(user_return), secr);
//		DWORD Etime = ::GetTickCount() - start;
//		checkSDWLResult(res);
//		EXPECT_EQ(0, res.GetCode());
//		cout << "  Evaluation time: " << Etime << " (ms)" << endl;
//
//		res = pInstance->RPMRequestLogin(callback_cmd, callback_cmd_param);
//		checkSDWLResult(res);
//
//		res = pInstance->RPMRequestLogout(0);
//		checkSDWLResult(res);
//	}
//
//	SDWLibDeleteRmcInstance(pInstance);
//}