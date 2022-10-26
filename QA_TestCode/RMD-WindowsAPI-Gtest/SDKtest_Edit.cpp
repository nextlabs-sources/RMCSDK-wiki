#include "stdafx.h"
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


#pragma endregion


using namespace std;

//TEST(EditSaveTest, EditSave) {
//	try {
//		SDWLResult res;
//		ISDRmcInstance *pInstance = NULL;
//		ISDRmTenant *pTenant = NULL;
//		ISDRmUser *puser = NULL;
//
//		SDR_Expiration expire;
//		expire.type = IExpiryType::RANGEEXPIRE;
//		expire.start = (uint64_t)std::time(nullptr) * 1000;
//		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;
//
//		wstring localfile_pathw = plan_file_path + txtnormal_testfile;
//
//		std::vector<SDRmFileRight> rights;
//		rights.push_back(RIGHT_VIEW);
//
//		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
//		wstring nxlfilepath;
//		string pathid;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		if (login_result) {
//			std::wstring filename;
//			std::vector<std::string> recipients = { "xiaozhuzhu0712@163.com" };
//			wstring nxlfile_path;
//			res = puser->ShareFile(localfile_pathw, nxlfile_path, rights, recipients, L"Test comments", watermark_info, expire);
//			if (!res.GetCode()) {
//
//			}
//		}
//		else {
//			EXPECT_TRUE(0);
//			cout << "Login failed!" << endl;
//		}
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}


TEST(RPMEDITSAVECOPY01, NXLFileTest) {
	try {
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring originalnxl_pathw = L"C:\\dest\\Normal_TXT-2018-11-20-08-04-34.txt.nxl";
		ISDRFiles* pFile = NULL;
		std::wstring filename;

		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;

		std::vector<SDRmFileRight> rights;
		rights = Tick_Rights("view");
		wstring newcreatedfilePath = L"";
		string pass = password_plain;
		md5(pass);

		string passcode = "IONRT98834TNNGA98HAFGN8YaaiWETMU45WJISEJSDJQW2346GDJJKLSJ89SNGHSDGJDKG";
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_global, pass);
		//RPMGetCurrentLoggedInUser(passcode, pInstance, pTenant, puser);
		if (login_result) {
			std::string tags = "";
			tags = "{\"itar\":[\"TAA-031\"],\"QA\":[\"Lifi3\",\"Hello\"]}";
			SDR_Expiration expire;
			expire.type = IExpiryType::NEVEREXPIRE;
			SDR_WATERMARK_INFO watermarkinfo;
			std::vector<SDRmFileRight> rights;
			rights = Tick_Rights("view|edit|print|copycontent|saveas|send|classify|share|download");
			wstring localfile_pathw = L"C:\\SDK test data\\LifiRMTest.json";
			wstring newcreatedfilePath = L"";
			cout << "ProtectFile: ";
			res = puser->ProtectFile(localfile_pathw, newcreatedfilePath, rights, watermarkinfo, expire, tags);
			cout << ws2s(newcreatedfilePath);
		}
		else {
			cout << "Login failed!" << endl;
		}
		pInstance->Save();
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}


TEST(RPMEDITSAVECOPY02, NXLFileTest) {
	try {
		wstring localfile_pathw = L"C:\\SDK test data\\LifiRMTest.json";
		wstring originalnxl_pathw = L"C:\\dest\\LifiRMTest-2018-11-23-06-16-42.json.nxl";
		wstring rpmfolder = L"C:\\dest\\rpm";
		ISDRFiles* pFile = NULL;
		std::wstring filename;

		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;

		std::vector<SDRmFileRight> rights;
		rights = Tick_Rights("view");
		wstring newcreatedfilePath = L"";
		string pass = password_plain;
		md5(pass);

		string passcode = "IONRT98834TNNGA98HAFGN8YaaiWETMU45WJISEJSDJQW2346GDJJKLSJ89SNGHSDGJDKG";
		SDWLResult ret = RPMGetCurrentLoggedInUser(passcode, pInstance, pTenant, puser);
		if (ret) {
			ret = pInstance->RPMEditCopyFile(originalnxl_pathw, rpmfolder);
			newcreatedfilePath = rpmfolder;
		}
		else {
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


TEST(RPMEDITSAVECOPY03, NXLFileTest) {
	try {
		wstring localfile_pathw = L"C:\\dest\\rpm\\8E1DD45722B3DBAFB11DA663DB23B149\\LifiRMTest-2018-11-23-06-16-42.json";
		wstring originalnxl_pathw = L"C:\\dest\\LifiRMTest-2018-11-23-06-16-42.json.nxl";
		wstring rpmfolder = L"C:\\dest\\rpm";
		ISDRFiles* pFile = NULL;
		std::wstring filename;

		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;

		std::vector<SDRmFileRight> rights;
		rights = Tick_Rights("view");
		wstring newcreatedfilePath = L"";
		string pass = password_plain;
		md5(pass);

		string passcode = "IONRT98834TNNGA98HAFGN8YaaiWETMU45WJISEJSDJQW2346GDJJKLSJ89SNGHSDGJDKG";
		SDWLResult ret = RPMGetCurrentLoggedInUser(passcode, pInstance, pTenant, puser);
		if (ret) {
			ret = pInstance->RPMEditSaveFile(localfile_pathw, L"", true, true);
			newcreatedfilePath = rpmfolder;
		}
		else {
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

//TEST(RPMEditTest, EditTest)
//{
//	wstring safe_folder = L"C:\\SDK test\\RPM\\Dir1";
//	wstring safe_folder_new = L"C:\\SDK test\\RPM\\Dir1";
//	ISDRmcInstance * pInstance;
//	ISDRmTenant * pTenant;
//	bool ret = false;
//	SDWLResult res;
//	string testFolder = TestDirectory + "RPMtest";
//	createFolder(testFolder);
//	ISDRmUser *puser = NULL;
//
//	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, "xiaozhuzhu0712@163.com", password);
//
//	if (login_result) {
//		cout << "GetHeartBeatInfo: ";
//		SDWLResult res = puser->GetHeartBeatInfo();
//		checkSDWLResult(res);
//		EXPECT_EQ(0, res.GetCode());
//		//bool heart_beat = GetHeartBeatResponseData();
//		//printPolicy(pInstance, puser);
//		//string tenantName_s = "";
//		//cout << "Input tenant name: ";
//		//cin >> tenantName_s;
//		//wstring tenantName = s2ws(tenantName_s);
//		//wcout << L"test GetPolicyBundle() with input: [" << tenantName << L"]" << endl;
//		//printPolicyBaseOnTenant(tenantName, puser);
//	}
//	cout << "----------RPM------------" << endl;
//
//	ret = pInstance->IsRPMDriverExist();
//	cout << "IsRPMDriverExist: " << ret << endl;
//	if (!ret)
//	{
//		cout << "Please double check if the RPM build is installed!" << endl;
//	}
//
//	string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);
//
//	cout << "SetRPMLoginResult: ";
//	res = pInstance->SetRPMLoginResult(user_logindata, secr);
//	EXPECT_EQ(0, res.GetCode());
//
//	/*cout << "AddRPMDir: ";
//	res = pInstance->AddRPMDir(safe_folder);
//	checkSDWLResult(res);
//	EXPECT_EQ(0, res.GetCode());*/
//
//	SDR_Expiration expire;
//	expire.type = IExpiryType::NEVEREXPIRE;
//	wstring localfile_pathw = plan_file_path + xlsnormal_testfile;
//
//	std::vector<SDRmFileRight> rights;
//	rights.push_back(RIGHT_VIEW);
//	rights.push_back(RIGHT_EDIT);
//
//	SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
//	std::wstring filename;
//	std::vector<std::string> recipients = { "3057318205@qq.com" };
//	wstring nxlfile_path;
//	res = puser->ShareFile(localfile_pathw, nxlfile_path, rights, recipients, L"Test comments", watermark_info, expire);
//	string passcode = "IONRT98834TNNGA98HAFGN8YaaiWETMU45WJISEJSDJQW2346GDJJKLSJ89SNGHSDGJDKG";
//	res = RPMGetCurrentLoggedInUser(passcode, pInstance, pTenant, puser);
//
//	//res = pInstance->RPMEditCopyFile(nxlfile_path, safe_folder);
//	//EXPECT_FALSE(res.GetCode());
//	//res = pInstance->RPMEditCopyFile(nxlfile_path, safe_folder_new);
//	//EXPECT_FALSE(res.GetCode());
//
//	//wstring nxlfile_path0;
//	//wstring safe_folder0 = L"C:\\SDK test\\RPM\\Dir1";
//	//res = puser->ShareFile(localfile_pathw, nxlfile_path0, rights, recipients, L"Test comments", watermark_info, expire);
//	//res = pInstance->SetRPMCachedToken();
//	//res = pInstance->RPMEditCopyFile(nxlfile_path0, safe_folder0);
//
//	//wstring nxlfile_path1;
//	//wstring safe_folder1 = L"C:\\SDK test\\RPM\\Dir1";
//	//res = puser->ShareFile(localfile_pathw, nxlfile_path1, rights, recipients, L"Test comments", watermark_info, expire);
//	//res = pInstance->SetRPMCachedToken();
//	//res = pInstance->RPMEditCopyFile(nxlfile_path1, safe_folder1);
//
//	//wstring nxlfile_path2;
//	//wstring safe_folder2 = L"C:\\SDK test\\RPM\\Dir1";
//	//wstring safe_folder22 = L"C:\\SDK test\\RPM\\Dir1";
//	//res = puser->ShareFile(localfile_pathw, nxlfile_path2, rights, recipients, L"Test comments", watermark_info, expire);
//	//res = pInstance->SetRPMCachedToken();
//	//res = pInstance->RPMEditCopyFile(nxlfile_path2, safe_folder2);
//	//res = pInstance->RPMEditCopyFile(nxlfile_path2, safe_folder22);
//
//
//	//res = pInstance->RPMEditSaveFile(safe_folder, L"", true, true);
//	//res = pInstance->RPMEditSaveFile(safe_folder0, L"", true, true);
//	//res = pInstance->RPMEditSaveFile(safe_folder1, L"", true, true);
//	//res = pInstance->RPMEditSaveFile(safe_folder2, L"", true, true);
//
//	//if (res.GetCode() == 0) {
//	ISDRFiles* pFile = NULL;
//	//	bool delete_result;
//	//res = puser->GetLocalFileManager(&pFile);
//	//size_t no = pFile->GetListNumber();
//	std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//	ISDRmNXLFile* nxlfile1;
//	res = puser->OpenFile(nxlfile_path, &nxlfile1);
//	//ISDRmNXLFile* nxlfile1 = pFile->GetFile(no - 1);
//	//ISDRmNXLFile* nxlfile2 = pFile->GetFile(no - 2);
//	//ISDRmNXLFile* nxlfile3 = pFile->GetFile(no - 3);
//	//ISDRmNXLFile* nxlfile4 = pFile->GetFile(no - 4);
//	//string passcode = "IONRT98834TNNGA98HAFGN8YaaiWETMU45WJISEJSDJQW2346GDJJKLSJ89SNGHSDGJDKG";
//	//SDWLResult ret = RPMGetCurrentLoggedInUser(passcode, pInstance, pTenant, puser);
//	if (ret) {
//		wstring hidden_path;
//		//ret = pInstance->SetRPMCachedToken();
//		//ret = pInstance->RPMEditCopyFile(nxlfile_path, safe_folder);
//		//ret = pInstance->RPMEditSaveFile(L"C:\\SDK test\\RPM\\Dir1", L"", true, true);
//		ret = puser->UploadFile(nxlfile1);
//		//ret = puser->UploadFile(nxlfile2);
//		//ret = puser->UploadFile(nxlfile3);
//		//ret = puser->UploadFile(nxlfile4);
//	}
//
//	//}
//
//	string iflogout = "";
//	cout << "Input Y do logout: ";
//	cin >> iflogout;
//	if (iflogout == "Y" || iflogout == "y" || iflogout == "yes") {
//		cout << "RPMLogout: ";
//		res = pInstance->RPMLogout();
//		checkSDWLResult(res);
//		EXPECT_EQ(0, res.GetCode());
//
//		cout << endl << "SDK user logout:";
//		res = puser->LogoutUser();
//		checkSDWLResult(res);
//	}
//
//	SDWLibDeleteRmcInstance(pInstance);
//	cout << "-------------This case finished--------" << endl << endl;
//}

TEST(RPMEditSaveTimesTest, EditCopySaveTest)
{
	//wstring safe_folder = L"C:\\SDK test\\RPM\\Dir7";
	wstring safe_hiddenfolder = L"";
	wstring safe_folder = L"C:\\SDK test\\RPM\\Dir1";
	wstring safe_folder3 = L"";
	//wstring safe_folder_new = L"C:\\SDK test\\RPM\\Dir1";
	ISDRmcInstance * pInstance;
	ISDRmTenant * pTenant;
	bool ret = false;
	SDWLResult res;
	string testFolder = TestDirectory + "RPMtest";
	createFolder(testFolder);
	ISDRmUser *puser = NULL;

	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	res = pInstance->RPMRegisterApp(apppath, secr);
	res = pInstance->RPMNotifyRMXStatus(true, secr);
	if (login_result) {
		cout << "GetHeartBeatInfo: ";
		SDWLResult res = puser->GetHeartBeatInfo();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
	}
	cout << "----------RPM------------" << endl;
	bool hearbet = GetHeartBeatResponseData();
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
	ISDRmNXLFile  *file;
	// res = puser->OpenFile(L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\CPviewEDIT-2018-11-29-08-29-25.txt.nxl", &file);
	 //res = puser->OpenFile(L"C:\\SDK test\\EDIT\\CPviewEDIT-2018-11-29-08-29-25.txt.nxl", &file);
	 //res = puser->DecryptNXLFile(file, L"C:\\auto\\output\\SDK\\Project\\abc2.txt", RLOEdit);
	//res = puser->OpenFile(L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\babc.tmp.nxl", &file);
	vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
	//res = puser->GetRights(L"C:\\SDK test\\EDIT\\Copy of RMD SDK CASE-lifi-2018-12-06-05-56-11.xlsx.nxl", rightsAndWatermarks);

	SDR_Expiration expire;
	expire.type = IExpiryType::NEVEREXPIRE;
	wstring localfilea_pathw = L"\\\\autonas01\\Public\\TestData\\Officedata\\3xlt.xlt";
	//wstring localfileb_pathw = plan_file_path + docnormal_testfile;

	std::vector<SDRmFileRight> rights;
	rights.push_back(RIGHT_VIEW);
	rights.push_back(RIGHT_EDIT);

	SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
	std::wstring filename;
	std::vector<std::string> recipients = { "3057318205@qq.com" };
	wstring nxlfile_path;
	wstring nxlfileb_path;
	//res = puser->ShareFile(localfilea_pathw, nxlfile_path, rights, recipients, L"Test comments", watermark_info, expire);
	//res = puser->ShareFile(localfileb_pathw, nxlfileb_path, rights, recipients, L"Test comments", watermark_info, expire);
	//res = puser->ProtectFile(localfileb_pathw, nxlfile_path, rights, watermark_info, expire);
	//string passcode = "IONRT98834TNNGA98HAFGN8YaaiWETMU45WJISEJSDJQW2346GDJJKLSJ89SNGHSDGJDKG";
	//res = RPMGetCurrentLoggedInUser(passcode, pInstance, pTenant, puser);
	res = pInstance->SetRPMServiceStop(true, secr);

	int processid;
	cout << "Process id: ";
	cin >> processid;
	res = pInstance->RPMAddTrustedProcess(processid, secr);




	//wstring server_path = L"\\\\autonas01\\Public\\Users\\jewelry\\test111\\vieweditUserB-2018-11-28-07-58-55.txt.nxl";
	//wstring dest_server_path = L"\\\\autonas01\\Public\\Users\\jewelry\\test\\zhu\\";
	//res = puser->GetUserPreference();

	//Check nxlfileb in original path
	//if (res.GetCode() == 0) {
	//	ISDRFiles* pFile = NULL;
	//	bool delete_result;
	//	res = puser->GetLocalFileManager(&pFile);
	//	size_t no = pFile->GetListNumber();
	//	std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
	//	if (no)
	//	{
	//		ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
	//		//res = puser->UploadFile(nxlfile);
	//		//Remove file
	//		//delete_result = pFile->RemoveFile(nxlfile);
	//		//EXPECT_TRUE(delete_result);
	//	}
	//}
	////res = pInstance->SetRPMCachedToken();
	////res = pInstance->RPMEditCopyFile(nxlfile_path, safe_folder);
	////res = pInstance->RPMEditCopyFile(L"C:\\SDK test\\EDIT\\CPviewEDIT-2018-12-06-02-35-17.txt.nxl", safe_folder2);
	////res = pInstance->RPMEditCopyFile(L"C:\\SDK test\\EDIT\\RMC_test-2018-11-30-05-55-02.xlsx.nxl", safe_folder2);

	//wstring path;

	//res = pInstance->RPMGetSecretDir(path);
	//res = pInstance->RPMEditCopyFile(L"C:\\SDK test\\EDIT\\RMC_Test-2018-12-07-05-51-57.docx.nxl", safe_folder);
	////EXPECT_FALSE(res.GetCode());
	//res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\Final RMD SDK CASE-lifi new-2019-03-28-09-39-59.xlsx.nxl", rightsAndWatermarks, true);
	//res = pInstance->RPMEditSaveFile(L"C:\\SDK test\\RPM\\Dir1\\SDK Performance-2019-03-28-08-34-05.xlsx", L"", true, 3);

	//int processid;
	//cout << "Process id: ";
	//cin >> processid;
	//res = pInstance->RPMAddTrustedProcess(processid, secr);

	std::vector<SDRmFileRight> filerights;
	wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
	res = puser->ProjectDownloadFile(2, "/myvaultsharefiletest-2019-04-15-08-37-56.txt.nxl", projectdownloaded_path, RD_ForOffline);
	res = pInstance->RPMGetRights(L"C:\\SDK test\\RPM\\Dir1\\UserC-adhoc-2019-05-17-07-26-13.txt", rightsAndWatermarks);
	res = pInstance->RPMGetFileRights(L"C:\\SDK test\\RPM\\Dir1\\UserC-adhoc-2019-05-17-07-26-13.txt", filerights);

	//res = pInstance->RPMEditCopyFile(L"C:\\SDK test\\EDIT\\RMC_Test-2018-12-07-05-51-57.docx.nxl", safe_folder3);
	//EXPECT_FALSE(res.GetCode());

	res = pInstance->RPMEditSaveFile(safe_folder3, L"", true, true);
	//res = pInstance->RPMEditSaveFile(safe_folder2, L"", true, true);
	//res = pInstance->RPMEditSaveFile(safe_folder3, L"", true, true);

	cout << "RPMLogout: ";
	res = pInstance->RPMLogout();
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	cout << endl << "SDK user logout:";
	res = puser->LogoutUser();
	checkSDWLResult(res);

	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl << endl;
}

//Edit file in normal folder
TEST(RPMEditSaveTCTest, EditCopySaveTCTest)
{
	//string dir;
	//cout << "Input DIR: ";
	//cin >> dir;
	wstring hidden_safefolder = L"C:\\SDK test\\RPM\\Dir2";
	wstring safe_folder = L"C:\\SDK test\\RPM\\Dir2";
	wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
	wstring original_path = downloaded_path;
	createFolder(ws2s(safe_folder));
	ISDRmcInstance * pInstance;
	ISDRmTenant * pTenant;
	bool ret = false;
	SDWLResult res;
	string testFolder = TestDirectory + "RPMtest";
	createFolder(testFolder);
	ISDRmUser *puser = NULL;
	bool running = true;
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

	ISDRmcInstance * pInstance1;
	ISDRmTenant * pTenant1;
	ISDRmUser *puser1 = NULL;
	std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;

	//bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");
	res = pInstance->RPMRegisterApp(apppath, secr);
	res = pInstance->RPMNotifyRMXStatus(running, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	//string passcode = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	//res = RPMGetCurrentLoggedInUser(passcode, pInstance1, pTenant1, puser1);
	//wstring email = puser1->GetEmail();
	//ISDRmNXLFile * nxlfile;
	//res = puser->OpenFile(L"C:\\Users\\jzhu\\Downloads\\diff udid-2018-12-12-06-19-56.txt.nxl", &nxlfile);
	//res = pInstance->RPMGetRights(L"C:\\SDK test\\RPM\\Dir1\\absolute-2018-12-12-09-08-29.pptx", rightsAndWatermarks);
	//vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks1;
	////File owner, ad-hoc file
	//res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\diff udid-2018-12-12-06-19-56.txt.nxl", rightsAndWatermarks1);
	//bool expired = nxlfile->CheckExpired("m67b4239c-34d5-4971-8f24-ea8279fc8280@bf1a2cc2-83cc-4163-b70d-43bcec44acd8");

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
	res = pInstance->SetRPMServiceStop(true, secr);
#pragma endregion


	//vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rights_watermark;
	//res = pInstance->RPMGetRights(L"C:\\SDK test\\RPM\\Dir1\\protect_normalB-2019-04-09-08-14-13.txt", rights_watermark);
	//res = pInstance->RPMEditCopyFile(L"C:\\Users\\jzhu\\Downloads\\1.3-2019-04-04-11-11-32.pdf.nxl", hidden_safefolder);
	//res = pInstance->SetRPMCachedToken();
	//EXPECT_FALSE(res.GetCode());
	//std::wstring tempFile = L"C:\\SDK test\\RPM\\Dir2\\IOS Automation env setup-2019-04-10-08-01-35.docx.nxl";
	//LPCTSTR lpFileName = tempFile.c_str();
	//WIN32_FIND_DATA pNextInfo;
	//HANDLE h = FindFirstFile(lpFileName, &pNextInfo);
	//if (h != INVALID_HANDLE_VALUE)
	//{
	//	FindClose(h);
	//}
	//res = puser->GetRights(L"C:\\SDK test\\RPM\\Dir2\\IOS Automation env setup-2019-04-10-08-01-35.docx.nxl", rightsAndWatermarks, false);

	//string is_delete = "";
	//cout << "Input delete value: ";
	//cin >> is_delete;
	//if (is_delete == "y") {
	//	res = pInstance->RPMDeleteFileToken(L"C:\\Users\\jzhu\\Downloads\\DebugDump-2018-12-11-08-25-41.txt.nxl");  
	//	res = pInstance->RPMDeleteFileToken(L"C:\\Users\\jzhu\\Downloads\\IsFileProtected-2018-12-11-07-00-42.txt.nxl");
	//	res = pInstance->RPMDeleteFileToken(L"C:\\Users\\jzhu\\Downloads\\root-2018-12-11-07-45-12.txt.nxl");
	//	res = pInstance->RPMDeleteFileToken(L"C:\\Users\\jzhu\\Downloads\\cp file-2018-12-11-09-01-44.txt.nxl");
	//	res = pInstance->RPMDeleteFileToken(L"C:\\Users\\jzhu\\Downloads\\cp file-2018-12-11-09-01-44.txt.nxl");
	//}
	//string input = "";
	//cout << "Input delete value: ";
	//cin >> input;

#pragma region [Genarate a nxl file with edit rights]
	SDR_Expiration expire;
	expire.type = IExpiryType::NEVEREXPIRE;
	wstring localfilea_pathw = plan_file_path + txtnormal_testfile;
	wstring localfileb_pathw = plan_file_path + docnormal_testfile;

	std::vector<SDRmFileRight> rights;
	rights.push_back(RIGHT_VIEW);
	rights.push_back(RIGHT_EDIT);

	SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };
	std::wstring filename;
	std::vector<std::string> recipients = { "xiaozhuzhu@163.com" };
	wstring nxlfile_path;
	wstring nxlfileb_path;
	res = puser->ProtectFile(localfilea_pathw, nxlfile_path, rights, watermark_info, expire);
	if (res.GetCode() == 0) {
		//res = pInstance->SetRPMCachedToken();

		//res = pInstance->RPMEditCopyFile(nxlfile_path, hidden_safefolder);
		//EXPECT_FALSE(res.GetCode());
		res = puser->GetRights(L"C:\\SDK test\\RPM\\Dir2\\WIN10-2019-04-10-08-52-20.PNG.nxl", rightsAndWatermarks, false);

		Sleep(1000);
		res = pInstance->RPMEditSaveFile(hidden_safefolder, L"", true, true);
		EXPECT_FALSE(res.GetCode());

		//ISDRFiles* pFile = NULL;
		bool delete_result;
		//res = puser->GetLocalFileManager(&pFile);
		//size_t no = pFile->GetListNumber();
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
		filename = nxlfile->GetFileName();
		wstring recipientEmails = L"xiaozhuzhu@163.com";
		res = puser->UploadFile(nxlfile_path, nxlfile_path, recipientEmails, L"Test!!");
		//Remove file
		//delete_result = pFile->RemoveFile(nxlfile);
		EXPECT_TRUE(delete_result);

	}
#pragma endregion

	bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "-creationTime", "allFiles");
	string pathid;
	if (file_result) {
		Json::Value jroot;
		jroot = ParseJsonFile(myvaultfilelist_return);
		pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
	}
	res = puser->MyVaultDownloadFile(pathid, downloaded_path, 2);

	//wstring hidden_safefolder = L"";
	////With hidden dir
	//res = pInstance->RPMEditCopyFile(downloaded_path, hidden_safefolder);
	//EXPECT_FALSE(res.GetCode());
	//Sleep(1000);

	//res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	res = pInstance->RPMEditCopyFile(L"C:\\Users\\jzhu\\Downloads\\UserA-cpnondomain-2019-02-28-06-27-25.txt.nxl", safe_folder);
	EXPECT_FALSE(res.GetCode());

	//string edit = "";
	//cout << "Input Y do logout: ";
	//cin >> edit;

	int processid;
	cout << "Process id: ";
	cin >> processid;
	res = pInstance->RPMAddTrustedProcess(processid, secr);

	res = pInstance->RPMEditSaveFile(safe_folder, L"", true, true);
	EXPECT_FALSE(res.GetCode());

	int processid2;
	cout << "Process id: ";
	cin >> processid2;
	res = pInstance->RPMAddTrustedProcess(processid2, secr);


	////Specify a safe folder
	//res = pInstance->RPMEditCopyFile(downloaded_path, safe_folder);
	//EXPECT_FALSE(res.GetCode());
	//Sleep(1000);
	//res = pInstance->RPMEditSaveFile(safe_folder, L"", true, true);
	//EXPECT_FALSE(res.GetCode());

	cout << "RPMLogout: ";
	res = pInstance->RPMLogout();
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	cout << endl << "SDK user logout:";
	res = puser->LogoutUser();
	checkSDWLResult(res);

	SDWLibDeleteRmcInstance(pInstance);

	cout << "-------------This case finished--------" << endl << endl;
}

//Test RPMGetCurrentLoggedInUser
TEST(RPMGetCurrentUserTest, GetCurrentUserTest) {
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

			string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

			cout << "SetRPMLoginResult: ";
			res = pInstance->SetRPMLoginResult(user_logindata, secr);
			EXPECT_EQ(0, res.GetCode());

			res = pInstance->SetRPMServiceStop(true, secr);
			ISDRmcInstance * pInstance11;
			res = RPMGetCurrentLoggedInUser(passcode, pInstance11, pTenant, puser);
			email_user = puser->GetEmail();
			EXPECT_STREQ(ws2s(email_user).c_str(), email.c_str());
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
#pragma endregion
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
		SDWLibDeleteRmcInstance(pInstance);

		if (!res.GetCode()) {
			res = RPMGetCurrentLoggedInUser(passcode, pInstance4, pTenant4, puser4);
			wstring email4 = puser->GetEmail();
			EXPECT_STREQ(res.GetMsg().c_str(), "RPM service return failed");
			EXPECT_EQ(res.GetCode(), 21);
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

TEST(RPMGetRightsTest, GetRightsTest) {
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
		unsigned int dirstatus;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);

		if (login_result) {
			cout << "GetHeartBeatInfo: ";
			SDWLResult res = puser->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
		}
		cout << "----------RPM------------" << endl;
		bool hearbet = GetHeartBeatResponseData();
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

		//res = pInstance->RPMDeleteFileToken(L"C:\\Users\\jzhu\\Downloads\\shareto30-2018-12-12-08-08-21.txt.nxl");
		//ISDRmNXLFile  *file;
		//vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		//res = puser->GetRights(L"C:\\SDK test\\RPM\\Dir1\\absolute-2018-12-12-09-08-29.pptx.nxl", rightsAndWatermarks);
		//vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks_rpm;
		//res = pInstance->RPMGetRights(L"C:\\SDK test\\RPM\\Dir1\\absolute-2018-12-12-09-08-29.pptx", rightsAndWatermarks_rpm);
		//res = pInstance->RPMGetRights(L"C:\\SDK test\\RPM\\Dir1\\absolute-2018-12-12-09-08-29.pptx", rightsAndWatermarks_rpm);
		//res = pInstance->RPMGetRights(L"C:\\SDK test\\RPM\\Dir1\\absolute-2018-12-12-09-08-29.pptx", rightsAndWatermarks_rpm);
		//res = pInstance->RPMGetRights(L"C:\\SDK test\\RPM\\Dir1\\absolute-2018-12-12-09-08-29.pptx.nxl", rightsAndWatermarks_rpm);

		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfilea_pathw = L"\\\\autonas01\\Public\\TestData\\Officedata\\3xlt.xlt";

		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_EDIT);

		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
		std::wstring filename;
		std::vector<std::string> recipients = { "3057318205@qq.com" };
		wstring nxlfile_path;
		wstring nxlfileb_path;
		//res = puser->ShareFile(localfilea_pathw, nxlfile_path, rights, recipients, L"Test comments", watermark_info, expire);
		//res = puser->ProtectFile(localfileb_pathw, nxlfile_path, rights, watermark_info, expire);
		res = pInstance->SetRPMServiceStop(true, secr);

		//wstring file_name = L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate\\Protect_normal-2018-12-11-07-37-32.txt";
		//wstring file_name1 = L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate\\Test.txt";
		//wstring file_name2 = plan_file_path + L"Normal_TXT.txt";
		//wstring file_name3 = server_file_path + L"Protect_normal-2018-07-03-02-52-33.txt.nxl";
		//wstring file_name4 = L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate\\sub1\\shareto30-2018-12-12-08-08-21.txt";
		//wstring file_name5 = L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\IsFileProtected-2018-12-11-07-00-4200.txt.nxl";
		bool filestatus;
		//res = pInstance->RPMGetFileStatus(file_name, &dirstatus, &filestatus);
		//res = pInstance->RPMGetFileStatus(file_name1, &dirstatus, &filestatus);
		//res = pInstance->RPMGetFileStatus(file_name2, &dirstatus, &filestatus);
		//res = pInstance->RPMGetFileStatus(file_name3, &dirstatus, &filestatus);
		//res = pInstance->RPMGetFileStatus(file_name4, &dirstatus, &filestatus);
		//res = pInstance->RPMGetFileStatus(file_name5, &dirstatus, &filestatus);
		//res = pInstance->IsRPMFolder(L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate", folder_tatus);

		//Check nxlfileb in original path
		//if (res.GetCode() == 0) {
		//	ISDRFiles* pFile = NULL;
		//	bool delete_result;
		//	res = puser->GetLocalFileManager(&pFile);
		//	size_t no = pFile->GetListNumber();
		//	std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
		//	if (no)
		//	{
		//		ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
		//		res = puser->UploadFile(nxlfile);
		//		//Remove file
		//		delete_result = pFile->RemoveFile(nxlfile);
		//		EXPECT_TRUE(delete_result);
		//	}
		//}
		cout << "RPMLogout: ";
		res = pInstance->RPMLogout();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << endl << "SDK user logout:";
		res = puser->LogoutUser();
		checkSDWLResult(res);

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

TEST(RPMEditCopyFileTest, InvalidTest) {
	try {
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir1";
		wstring safe_folder_new = L"C:\\SDK test\\RPM\\Dir1";
		createFolder(ws2s(safe_folder));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		string testFolder = TestDirectory + "RPMtest";
		createFolder(testFolder);
		ISDRmUser *puser = NULL;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);

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

		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_folder);

		wstring path_not_exist = safe_folder + L"\\Sub1";
		wstring file_not_exist = safe_folder_new + L"\\aaa.txt";

		res = pInstance->RPMEditCopyFile(path_not_exist, safe_folder);
		EXPECT_EQ(res.GetCode(), 3);
		EXPECT_STREQ(res.GetMsg().c_str(), "File not found");
		res = pInstance->RPMEditCopyFile(file_not_exist, safe_folder_new);
		EXPECT_EQ(res.GetCode(), 3);
		EXPECT_STREQ(res.GetMsg().c_str(), "File not found");

		cout << "RPMLogout: ";
		res = pInstance->RPMLogout();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << endl << "SDK user logout:";
		res = puser->LogoutUser();
		checkSDWLResult(res);

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

TEST(RPMEditSaveFileTest, EditSaveFileTest) {
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
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		int rights_value;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		res = pInstance->RPMRegisterApp(apppath, secr);
		res = pInstance->RPMNotifyRMXStatus(running, secr);
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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_folder);
		res = pInstance->AddRPMDir(safe_folder_new);

		res = pInstance->SetRPMServiceStop(true, secr);
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
		//res = puser->GetLocalFileManager(&pFile);
		//size_t no = pFile->GetListNumber();
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();
			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comment Test!");
			rights_value = rights_changeToValue2(nxlfile->GetRights());
			//////Remove file
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
		//Copy file to RPM folder then edit
		res = pInstance->RPMEditCopyFile(downloaded_p, safe_folder);
		EXPECT_FALSE(res.GetCode());

		std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		std::vector<SDRmFileRight> filerights;
		res = pInstance->RPMGetRights(safe_folder, rightsAndWatermarks);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
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
		const string safe_folder_new_ss = safe_folder_new_s.substr(0, str_len);
		string conten_str1 = read_filecontent(safe_folder_new_ss);
		EXPECT_STREQ(conten_str1.c_str(), original_content.c_str());
		string new_content1 = "Edit !";
		edit_file(safe_folder_new_ss, new_content);
		res = pInstance->RPMEditSaveFile(s2ws(safe_folder_new_ss), L"", false, 3);
		EXPECT_FALSE(res.GetCode());
		string conten_saved1 = read_filecontent(safe_folder_new_ss);
		EXPECT_STREQ(conten_saved1.c_str(), new_content1.c_str());
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

wstring safe_normalfolder = L"C:\\SDK test\\RPM\\Dir2\\longname\\longname\\longname\\longname\\";
wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
string file_owner;
TEST(RPMEditCopyFileTest, NormalTest1) {
	try {
		createFolder(ws2s(safe_normalfolder));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		string testFolder = TestDirectory + "RPMtest";
		createFolder(testFolder);
		ISDRmUser *puser = NULL;
		string pathid;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		res = pInstance->RPMRegisterApp(apppath, secr);
		res = pInstance->RPMNotifyRMXStatus(running, secr);
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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_normalfolder);

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
			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients , L"Comments");
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

		res = puser->MyVaultDownloadFile(pathid, downloaded_path, 2);

#pragma endregion	

		if (!res.GetCode()) {

			//Copy file to RPM folder
			//res = pInstance->RPMEditCopyFile(downloaded_path, safe_normalfolder);
			//res = pInstance->RPMEditCopyFile(L"D:\\Protect_normal-dwuifvbdisbavifsbvuifbsakuvnslknvldsanvlndslVNDLSnlknln2e1rh80egh8fh89vh8ro8gjvp9ra8nvfbvnjsfbkjsjbnsabnvjdbsakjvbfsjkvbkjsdfbvkjdsbk33333333333333333333avbkjvbkjbvkjbskjvbkjabvkjfbvkjbdskjvbdskjavbkjdbvkjsdbvjksbvjksbavkjbsakvbskal.txt.nxl", safe_normalfolder);
			EXPECT_FALSE(res.GetCode());
			//std::wstring tempFile = safe_normalfolder + L".nxl";
			//LPCTSTR lpFileName = tempFile.c_str();
			//WIN32_FIND_DATA pNextInfo;
			//HANDLE h = FindFirstFile(lpFileName, &pNextInfo);
			//if (h != INVALID_HANDLE_VALUE)
			//{
			//	FindClose(h);
			//}
			//Sleep(1000);
			//EXPECT_TRUE(file_exist(ws2s(safe_normalfolder)));

			//vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
			////RPMGetRights
			//res = pInstance->RPMGetRights(safe_normalfolder, rightsAndWatermarks);
			//EXPECT_EQ(rightsAndWatermarks.size(), 10);
			////RPMGetFileRights
			//vector<SDRmFileRight> filerights;
			//res = pInstance->RPMGetFileRights(safe_normalfolder, filerights);
			//EXPECT_EQ(filerights.size(), 10);
			unsigned int dirstatus;
			bool filestatus;
			//res = pInstance->RPMGetFileStatus(safe_normalfolder, &dirstatus, &filestatus);
			unsigned int dirstatus1;
			bool filestatus1;
			//RPMGetFileStatus
			res = pInstance->RPMGetFileStatus(L"C:\\SDK test\\RPM\\Dir2\\longname\\longname\\longname\\longname\\abcd-2019-04-30-09-12-16.docx", &dirstatus1, &filestatus1);



			std::vector<SDRmFileRight> filerights_test;
			res = pInstance->RPMGetFileRights(L"C:\\SDK test\\RPM\\Dir2\\longname\\longname\\longname\\longname\\abcd-2019-04-30-09-12-16.docx", filerights_test);
			res = pInstance->RPMEditSaveFile(safe_normalfolder, L"", false, 1);

			ISDRmNXLFile* nxlfile1;
			res = puser->OpenFile(downloaded_path, &nxlfile1);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			Json::Value jroot;
			jroot = ParseJsonFile(user_respond);
			for (int i = 0; i < jroot["extra"]["memberships"].size(); i++) {
				int pro_id = jroot["extra"]["memberships"][i]["projectId"].asInt();
				if (pro_id == 1) {
					file_owner = jroot["extra"]["memberships"][i]["id"].asString();
				}
			}

			string create_by = nxlfile1->Get_CreatedBy();
			EXPECT_STREQ(file_owner.c_str(), create_by.c_str());
			string modified_by = nxlfile1->Get_ModifiedBy();
			EXPECT_STREQ(file_owner.c_str(), modified_by.c_str());

			uint64_t create_date = nxlfile1->Get_DateCreated();
			uint64_t modified_date = nxlfile1->Get_DateModified();
			string watermark_str = nxlfile1->GetAdhocWaterMarkString();
			EXPECT_STREQ("Value1:Text1", watermark_str.c_str());

		}

#pragma region [User Logout]
		cout << "RPMLogout: ";
		res = pInstance->RPMLogout();
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		cout << endl << "SDK user logout:";
		res = puser->LogoutUser();
		checkSDWLResult(res);
#pragma endregion
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

TEST(RPMEditCopyFileTest, NormalTest2) {
	try {
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		string pathid;
		bool ret = false;
		bool running = true;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		res = pInstance->RPMRegisterApp(apppath, secr);
		res = pInstance->RPMNotifyRMXStatus(running, secr);
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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_normalfolder);

		res = pInstance->SetRPMServiceStop(true, secr);
#pragma endregion


		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		//RPMGetRights
		//res = pInstance->RPMGetRights(L"C:\\SDK test\\RPM\\Dir2\\longname\\longname\\longname\\longname\\1A923D792E325356FA1CB2495A85628320190403064518\\protect_normalB-2019-04-03-06-40-53.txt", rightsAndWatermarks);
		EXPECT_EQ(rightsAndWatermarks.size(), 2);
		//RPMGetFileRights
		vector<SDRmFileRight> filerights;
		res = pInstance->RPMGetFileRights(L"C:\\SDK test\\RPM\\Dir2\\longname\\longname\\longname\\longname\\1A923D792E325356FA1CB2495A85628320190403064518\\protect_normalB-2019-04-03-06-40-53.txt", filerights);
		EXPECT_EQ(filerights.size(), 2);

		//Copy file to RPM folder
		res = pInstance->RPMEditSaveFile(L"C:\\SDK test\\RPM\\Dir2\\longname\\longname\\longname\\longname\\6DA238626C8E6C5CD9ACE61C520C152B20190425063751\\protect_normalB-2019-04-25-06-37-43.txt", L"", false, 3);
		EXPECT_FALSE(res.GetCode());
		EXPECT_FALSE(file_exist(ws2s(safe_normalfolder)));

		ISDRmNXLFile* nxlfile1;
		res = puser->OpenFile(L"C:\\auto\\output\\SDK\\Project\\protect_normalB-2019-04-25-06-37-43.txt.nxl", &nxlfile1);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		Json::Value jroot;
		jroot = ParseJsonFile(user_respond);
		string modified;
		for (int i = 0; i < jroot["extra"]["memberships"].size(); i++) {
			int pro_id = jroot["extra"]["memberships"][i]["projectId"].asInt();
			if (pro_id == 1) {
				modified = jroot["extra"]["memberships"][i]["id"].asString();
			}
		}
		string create_by = nxlfile1->Get_CreatedBy();
		EXPECT_STREQ(file_owner.c_str(), create_by.c_str());
		string modified_by = nxlfile1->Get_ModifiedBy();
		EXPECT_STREQ(modified_by.c_str(), modified.c_str());

		uint64_t create_date = nxlfile1->Get_DateCreated();
		uint64_t modified_date = nxlfile1->Get_DateModified();
		string watermark_str = nxlfile1->GetAdhocWaterMarkString();
		EXPECT_STREQ("Value1:Text1", watermark_str.c_str());

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

vector<string> rights_vc = { "view|edit|print|copycontent|saveas|sc|send|classify|share|download", "view|edit", "view|share|print", "view|edit|print|download", "view|edit|print|copycontent|saveas|sc|send|classify|share|download", "view" };
wstring safe_normal2folder = L"C:\\SDK test\\RPM\\Dir1";
class RPMUserBEditCopyFileTest :public::testing::TestWithParam<string> {};
TEST_P(RPMUserBEditCopyFileTest, NormalTest2)
{
	try {
		createFolder(ws2s(safe_normalfolder));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		string testFolder = TestDirectory + "RPMtest";
		createFolder(testFolder);
		ISDRmUser *puser = NULL;
		string pathid;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		res = pInstance->RPMRegisterApp(apppath, secr);
		res = pInstance->RPMNotifyRMXStatus(running, secr);
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
		//res = puser->GetLocalFileManager(&pFile);
		//size_t no = pFile->GetListNumber();
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();
			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comments!");
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
		res = puser->MyVaultDownloadFile(pathid, downloaded_path, 2);

#pragma endregion	

#pragma region [User Logout]
		cout << endl << "SDK user logout:";
		res = puser->LogoutUser();
		checkSDWLResult(res);
#pragma endregion

		string clientID = ReadFromFile(plan_path + "client_id.txt");
		string  puser_return = plan_path + "User_Return.txt";
		GetUserLoginResponseData(email_zhu, password, clientID, puser_return);
		string userJson = ReadFromFile(puser_return);
		res = pInstance->SetLoginResult(userJson, &puser, secr);
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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_normalfolder);

#pragma endregion

		//Copy file to RPM folder
		res = pInstance->RPMEditCopyFile(downloaded_path, safe_normal2folder);
		EXPECT_FALSE(res.GetCode());
		std::wstring tempFile = safe_normalfolder + L".nxl";
		LPCTSTR lpFileName = tempFile.c_str();
		WIN32_FIND_DATA pNextInfo;
		HANDLE h = FindFirstFile(lpFileName, &pNextInfo);
		if (h != INVALID_HANDLE_VALUE)
		{
			FindClose(h);
		}
		Sleep(1000);
		EXPECT_TRUE(file_exist(ws2s(safe_normalfolder)));

		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		//RPMGetRights
		res = pInstance->RPMGetRights(safe_normal2folder, rightsAndWatermarks);
		EXPECT_EQ(rightsAndWatermarks.size(), rights.size());
		//RPMGetFileRights
		vector<SDRmFileRight> filerights;
		res = pInstance->RPMGetFileRights(safe_normal2folder, filerights);
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
INSTANTIATE_TEST_CASE_P(RPMUserBEditCopyFileTesting, RPMUserBEditCopyFileTest, testing::ValuesIn(rights_vc));


TEST(EditCopyLongNameFileTest, LongNameTest) {
	try {
		createFolder(ws2s(safe_normalfolder));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		string testFolder = TestDirectory + "RPMtest";
		createFolder(testFolder);
		ISDRmUser *puser = NULL;
		string pathid;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir3\\Sub1\\zhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuzhuzhuzhzulonglongzhuzhuzhuzhzuhzuz\\";
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		res = pInstance->RPMRegisterApp(apppath, secr);
		res = pInstance->RPMNotifyRMXStatus(running, secr);
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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_normalfolder);

#pragma endregion

		//res = pInstance->RPMEditCopyFile(plan_pathw + longname_testnxlfile, safe_folder);
		res = pInstance->RPMEditCopyFile(L"C:\\SDK test data\\aa.txt.nxl", safe_folder);
		EXPECT_FALSE(res.GetCode());
		res = pInstance->RPMEditSaveFile(safe_folder, L"", false, 0);
		EXPECT_FALSE(res.GetCode());
		//Delete file
		res = pInstance->RPMDeleteFile(safe_folder);
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

TEST(RPMGetFileStatusTest, RPMGetFileStatusTest) {
	try {
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir3";
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
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		wstring downloaded_p = L"C:\\auto\\output\\SDK\\Project\\";

		bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");
		//bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		res = pInstance->RPMRegisterApp(apppath, secr);
		res = pInstance->RPMNotifyRMXStatus(running, secr);
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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

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
		//res = puser->GetLocalFileManager(&pFile);
		//size_t no = pFile->GetListNumber();
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();
			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comments Test");
			////Remove file
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

		int processid;
		cout << "Process id: ";
		cin >> processid;
		res = pInstance->RPMAddTrustedProcess(processid, secr);
		unsigned int dirstatus;
		bool filestatus;
		wstring des_path = safe_folder + L"\\" + filename;
		wstring des_subpath = safe_subfolder + L"\\" + filename;
		wstring des_prapath = safe_parfolder + L"\\" + filename;
		wstring des_nonpath = unsafe_folder + L"\\" + filename;

		//1. From SDK copy file
		res = pInstance->RPMEditCopyFile(downloaded_p, safe_copyfolder);
		EXPECT_FALSE(res.GetCode());
		res = pInstance->RPMGetFileStatus(safe_copyfolder, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 2);
		EXPECT_TRUE(filestatus);

		//2. Get file status from RPM folder
		CopyFile(downloaded_p.c_str(), des_path.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(des_path, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 0);
		EXPECT_TRUE(filestatus);

		//3. Get file status from RPM sub folder
		CopyFile(downloaded_p.c_str(), des_subpath.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(des_subpath, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 2);
		EXPECT_TRUE(filestatus);

		//4. Get file status from RPM parent folder
		CopyFile(downloaded_p.c_str(), des_prapath.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(des_prapath, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 1);
		EXPECT_TRUE(filestatus);

		//5. Get file status from NonRPM folder
		CopyFile(downloaded_p.c_str(), des_nonpath.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(des_nonpath, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 3);
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
		EXPECT_EQ(dirstatus, 0);
		EXPECT_FALSE(filestatus);

		//7. Not nxl file(without extension ".nxl"), sub safe folder
		CopyFile(nonnxl_sourcepath.c_str(), notnxl_subpath.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(notnxl_subpath, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 2);
		EXPECT_FALSE(filestatus);

		//8. Not nxl file(without extension ".nxl"), parent safe folder
		CopyFile(nonnxl_sourcepath.c_str(), notnxl_parpath.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(notnxl_parpath, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 1);
		EXPECT_FALSE(filestatus);

		//9. Invalid nxl file(with extension ".nxl")
		CopyFile(invalidnxl_sourcepath.c_str(), invalidnxl_path.c_str(), FALSE);
		res = pInstance->RPMGetFileStatus(invalidnxl_path, &dirstatus, &filestatus);
		EXPECT_EQ(dirstatus, 0);
		EXPECT_FALSE(filestatus);

		//Delete file
		res = pInstance->RPMDeleteFile(safe_copyfolder);
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

//TEST(RPMFolderTest, IsFolderRPMTest) {
//	try {
//		ISDRmcInstance * pInstance;
//		SDWLResult res;
//		ISDRmUser *puser = NULL;
//		ISDRmTenant *pTenant = NULL;
//		unsigned int folder_tatus;
//		bool filestatus;
//		unsigned int dirstatus;
//		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
//		res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
//		wstring email = puser->GetEmail();
//		res = pInstance->IsRPMFolder(L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate", &folder_tatus);
//		res = pInstance->IsRPMFolder(L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate\\sub1", &folder_tatus);
//		res = pInstance->IsRPMFolder(L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM", &folder_tatus);
//		res = pInstance->IsRPMFolder(L"C:", &folder_tatus);
//
//		cout << "res.GetCode(): " << res.GetCode() << endl;
//		res = pInstance->RPMGetFileStatus(L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate\\diff udid-2018-12-17-08-44-32.txt", &dirstatus, &filestatus);
//		res = pInstance->RPMGetFileStatus(L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate\\aa.txt", &dirstatus, &filestatus);
//		res = pInstance->RPMGetFileStatus(L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\Intermediate\\sub1\\shareto30-2018-12-12-08-08-21.txt", &dirstatus, &filestatus);
//		res = pInstance->RPMGetFileStatus(L"C:\\Users\\jzhu\\AppData\\Local\\NextLabs\\SkyDRM\\IsFileProtected-2018-12-11-07-00-4200.txt.nxl", &dirstatus, &filestatus);
//
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

TEST(app1test, app1) {
	ISDRmcInstance * pInstance;
	ISDRmTenant * pTenant;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	bool login_result;

	bool ret = false;
	wstring safe_folder = L"C:\\SDK test\\RPM\\Dir7";
	wstring email_user;
	string passcode = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

	string pclientID = "";
	string  puser_return = plan_path + "User_Return.txt";

	res = CreateSDRmcInstance_CreateClientID(L"C:\\SDK test\\TrustApp_Central\\working", pInstance, pclientID, DEFUALT_ROUTER, DEFUALT_TENANT, L"C:\\SDK test\\TrustApp_Central");

	Sleep(1000);
	bool te_result = GetTenantResponseData(DEFUALT_ROUTER);
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
			login_result = true;
			string ps_code = puser->GetPasscode();
			cout << "User passcode is :" + ps_code << endl;
			string exportfilename = plan_path + passcode_txt;
			ofstream SaveFile(exportfilename);
			SaveFile << ps_code;
			SaveFile.close();
			pInstance->Save();
			cout << "Login Successful!" << endl;

		}
		else {
			cout << "Login failed!" << endl;

		}
	}
	else {
		cout << "Create Instance failed!" << endl;
	}

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

		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());


		//res = RPMGetCurrentLoggedInUser(passcode, pInstance, pTenant, puser);

#pragma endregion
		//SDWLibDeleteRmcInstance(pInstance);
	}
	else {
		cout << "Login failed!" << endl;
		EXPECT_TRUE(false);
	}
}

TEST(app2test, app2) {
	ISDRmcInstance * pInstance;
	ISDRmTenant * pTenant;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	string passcode = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	res = RPMGetCurrentLoggedInUser(passcode, pInstance, pTenant, puser);
	if (!res.GetCode()) {
		wstring email = puser->GetEmail();
		cout << "Email: " + ws2s(email) << endl;
	}
	else {
		cout << "Get user failed!!!" << endl;
	}
}

TEST(app3test, app3) {

	ISDRmcInstance * pInstance;
	ISDRmTenant * pTenant;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	bool login_result;

	bool ret = false;
	wstring safe_folder = L"C:\\SDK test\\RPM\\Dir7";
	wstring email_user;
	string passcode = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

	string pclientID = "";
	string  puser_return = plan_path + "User_Return.txt";

	res = CreateSDRmcInstance_CreateClientID(L"C:\\SDK test\\RPMtest\\working", pInstance, pclientID, DEFUALT_ROUTER, DEFUALT_TENANT, L"C:\\SDK test\\RPMtest");

	cout << res.GetCode() << endl;

	//	Sleep(1000);
	//	bool te_result = GetTenantResponseData(DEFUALT_ROUTER);
	//	Sleep(1000);
	//	bool finished = false;
	//	int i = 0;
	//	do {
	//		i++;
	//		Sleep(1000);
	//		res = pInstance->IsInitFinished(finished);
	//		cout << i << ": Wait RMCInstance Class finish initialization...";
	//		cout << " >res: ";
	//		checkSDWLResult(res);
	//		EXPECT_EQ(0, res.GetCode());
	//		if (i == 20)
	//		{
	//			cout << i << "Cannot connect to PDPMan" << endl;
	//			break;
	//		}
	//	} while (!finished);
	//
	//	if (!res.GetCode()) {
	//		// pInstance->Save();
	//		GetUserLoginResponseData(email, password, pclientID, puser_return);
	//		string user_return = ReadFromFile(puser_return);
	//		Json::Value jroot;
	//		jroot = ParseJsonFile(puser_return);
	//
	//		res = pInstance->SetLoginResult(user_return, &puser, secr);
	//		pInstance->GetCurrentTenant(&pTenant);
	//		wcout << L"Current Tenant:  " << pTenant->GetTenant() << endl;
	//		if (!res.GetCode()) {
	//			login_result = true;
	//			string ps_code = puser->GetPasscode();
	//			cout << "User passcode is :" + ps_code << endl;
	//			string exportfilename = plan_path + passcode_txt;
	//			ofstream SaveFile(exportfilename);
	//			SaveFile << ps_code;
	//			SaveFile.close();
	//			pInstance->Save();
	//			cout << "Login Successful!" << endl;
	//
	//		}
	//		else {
	//			cout << "Login failed!" << endl;
	//
	//		}
	//	}
	//	else {
	//		cout << "Create Instance failed!" << endl;
	//	}
	//
	//	if (login_result) {
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
	//		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);
	//
	//		cout << "SetRPMLoginResult: ";
	//		res = pInstance->SetRPMLoginResult(user_logindata, secr);
	//		EXPECT_EQ(0, res.GetCode());
	//
	//
	//		//res = RPMGetCurrentLoggedInUser(passcode, pInstance, pTenant, puser);
	//
	//#pragma endregion
	//	//SDWLibDeleteRmcInstance(pInstance);
	//	}
	//	else {
	//		cout << "Login failed!" << endl;
	//		EXPECT_TRUE(false);
	//	}
}

//#define ArraySize(ptr)    (sizeof(ptr) / sizeof(ptr[0]))

TEST(EditOfficeFileTest, OfficeFileTest) {


	wstring safe_folder = L"C:\\SDK test\\RPM\\Dir3\\Sub1";
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
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	bool running = true;

	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
	res = pInstance->RPMRegisterApp(apppath, secr);
	res = pInstance->RPMNotifyRMXStatus(running, secr);
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

	string passcode = puser->GetPasscode();
	string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

	cout << "SetRPMLoginResult: ";
	res = pInstance->SetRPMLoginResult(user_logindata, secr);
	EXPECT_EQ(0, res.GetCode());

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
	//res = puser->GetLocalFileManager(&pFile);
	//size_t no = pFile->GetListNumber();
	std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

	//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
	ISDRmNXLFile* nxlfile;
	res = puser->OpenFile(nxlfile_path, &nxlfile);
	if (nxlfile)
	{
		filename = nxlfile->GetFileName();
		res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comment Test!");
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
		res = pInstance->RPMAddTrustedProcess(dwPid, secr);

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
	}
};

TEST(RPMEditSaveFileTestBug, EditSaveFileTest)
{
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
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email, password);
		//bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_admin, password_admin, "2");
		res = pInstance->RPMRegisterApp(apppath, secr);
		res = pInstance->RPMNotifyRMXStatus(running, secr);
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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_folder);
		res = pInstance->AddRPMDir(safe_folder_new);

		res = pInstance->SetRPMServiceStop(true, secr);
#pragma endregion

		//res = pInstance->RPMEditCopyFile(L"D:\\test\\a.prt", safe_folder);
		int processid11;
		cout << "Process id: ";
		cin >> processid11;
		res = pInstance->RPMAddTrustedProcess(processid11, secr);
		res = pInstance->RPMEditSaveFile(L"C:\\SDK test\\RPM\\Dir3\\Sub1\\11-2019-05-16-02-16-30.docx", L"", false, 2);
//
//#pragma region[Prepare a MYVAULT nxl file]
//		SDR_Expiration expire;
//		expire.type = IExpiryType::NEVEREXPIRE;
//		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;
//		string original_content = read_filecontent(ws2s(localfile_pathw));
//
//		std::vector<SDRmFileRight> rights;
//		rights.push_back(RIGHT_VIEW);
//		rights.push_back(RIGHT_EDIT);
//
//		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
//		std::wstring filename;
//		wstring recipients = s2ws(email_zhu);
//		wstring nxlfile_path;
//		res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire);
//
//		ISDRFiles* pFile = NULL;
//		bool delete_result;
//		//res = puser->GetLocalFileManager(&pFile);
//		//size_t no = pFile->GetListNumber();
//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//
//		//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
//
//		ISDRmNXLFile* nxlfile;
//		res = puser->OpenFile(nxlfile_path, &nxlfile);
//		if (nxlfile)
//		{
//			filename = nxlfile->GetFileName();
//			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comment Test!");
//			//Remove file
//			delete_result = pFile->RemoveFile(nxlfile);
//			EXPECT_TRUE(delete_result);
//		}
//		else {
//			cout << "No file generated!" << endl;
//			EXPECT_FALSE(res.GetCode());
//		}
//
//		bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
//		if (file_result) {
//			Json::Value jroot;
//			jroot = ParseJsonFile(myvaultfilelist_return);
//			pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
//		}
//		res = puser->MyVaultDownloadFile(pathid, downloaded_p, 2);
//
//#pragma endregion	
//		//Copy file to RPM folder then edit
//		//res = pInstance->RPMEditCopyFile(downloaded_p, safe_folder);
//		//EXPECT_FALSE(res.GetCode());
//		//std::wstring tempFile = safe_folder + L".nxl";
//		//LPCTSTR lpFileName = tempFile.c_str();
//		//WIN32_FIND_DATA pNextInfo;
//		//HANDLE h = FindFirstFile(lpFileName, &pNextInfo);
//		//if (h != INVALID_HANDLE_VALUE)
//		//{
//		//	FindClose(h);
//		//}
//
//		wstring file_rpm = L"C:\\SDK test\\RPM\\Dir1\\protect_normalB-2019-04-16-09-59-06.txt";
//
//		string conten_str = read_filecontent(ws2s(file_rpm));
//		//EXPECT_STREQ(conten_str.c_str(), original_content.c_str());
//		string new_content = "Edit !";
//		edit_file(ws2s(file_rpm), new_content);
//		res = pInstance->RPMEditSaveFile(file_rpm, L"", false, 3);
//
//		int processid;
//		cout << "Process id: ";
//		cin >> processid;
//		res = pInstance->RPMAddTrustedProcess(processid, secr);
//
//		EXPECT_FALSE(res.GetCode());
//		string conten_saved = read_filecontent(ws2s(safe_folder));
//		EXPECT_STREQ(conten_saved.c_str(), new_content.c_str());

		////Download file to RPM folder then edit
		//res = puser->MyVaultDownloadFile(pathid, safe_folder_new, 2);
		//string conten_str1 = read_filecontent(ws2s(safe_folder_new));
		//EXPECT_STREQ(conten_str1.c_str(), original_content.c_str());
		//string new_content1 = "Edit !";
		//edit_file(ws2s(safe_folder_new), new_content);
		//res = pInstance->RPMEditSaveFile(safe_folder_new, L"", false, 3);
		//EXPECT_FALSE(res.GetCode());
		//string conten_saved1 = read_filecontent(ws2s(safe_folder_new));
		//EXPECT_STREQ(conten_saved1.c_str(), new_content1.c_str());
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(UnregisterAPP, Unregister) {
	ISDRmcInstance * pInstance;
	ISDRmTenant * pTenant;
	SDWLResult res;
	bool running = true;
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

	cout << "SDWLibCreateSDRmcInstance: ";
	res = SDWLibCreateSDRmcInstance("Product name", 1, 0, 1621, L"C:\\Program Files\\NextLabs", L"C:\\work", &pInstance);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	string workingfoler = "C:\\work\\working";
	createFolder(workingfoler);
	res = pInstance->Initialize(s2ws(workingfoler), DEFUALT_ROUTER, L"");
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	string apppath;
	cout << "App path is: ";
	cin >> apppath;

	res = pInstance->RPMUnregisterApp(s2ws(apppath), secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
}

//TEST(RPMSetTokenTest, SetTokenTest) {
//	try {
//		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir1";
//		wstring safe_folder_new = L"C:\\SDK test\\RPM\\Dir2";
//		createFolder(ws2s(safe_folder));
//		createFolder(ws2s(safe_folder_new));
//
//		ISDRmcInstance * pInstance;
//		ISDRmTenant * pTenant;
//		bool ret = false;
//		SDWLResult res;
//		string testFolder = TestDirectory + "RPMtest";
//		createFolder(testFolder);
//		ISDRmUser *puser = NULL;
//		string pathid;
//		wstring downloaded_p = L"C:\\auto\\output\\SDK\\Project\\";
//		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
//		bool running = true;
//
//		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
//		res = pInstance->RPMRegisterApp(apppath, secr);
//		res = pInstance->RPMNotifyRMXStatus(running, secr);
//		checkSDWLResult(res);
//		EXPECT_EQ(0, res.GetCode());
//#pragma region[Prepare RPM environment]
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
//		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);
//
//		cout << "SetRPMLoginResult: ";
//		res = pInstance->SetRPMLoginResult(user_logindata, secr);
//		EXPECT_EQ(0, res.GetCode());
//
//		cout << "AddRPMDir: ";
//		res = pInstance->AddRPMDir(safe_folder);
//		res = pInstance->AddRPMDir(safe_folder_new);
//
//		res = pInstance->SetRPMServiceStop(true, secr);
//#pragma endregion
//
//#pragma region[Prepare a MYVAULT nxl file]
//		SDR_Expiration expire;
//		expire.type = IExpiryType::NEVEREXPIRE;
//		wstring localfile_pathw = plan_file_path + txtnormal_testfileb;
//		string original_content = read_filecontent(ws2s(localfile_pathw));
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
//		//res = puser->GetLocalFileManager(&pFile);
//		//size_t no = pFile->GetListNumber();
//		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
//
//		//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
//		ISDRmNXLFile* nxlfile;
//		res = puser->OpenFile(nxlfile_path, &nxlfile);
//		if (nxlfile)
//		{
//			filename = nxlfile->GetFileName();
//			puser->UploadFile(nxlfile);
//			//Remove file
//			delete_result = pFile->RemoveFile(nxlfile);
//			EXPECT_TRUE(delete_result);
//		}
//		else {
//			cout << "No file generated!" << endl;
//			EXPECT_FALSE(res.GetCode());
//		}
//
//
//		bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
//		if (file_result) {
//			Json::Value jroot;
//			jroot = ParseJsonFile(myvaultfilelist_return);
//			pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
//		}
//		res = puser->MyVaultDownloadFile(pathid, downloaded_p, 2);
//#pragma endregion
//
//
//		int processid11;
//		cout << "Process id: ";
//		cin >> processid11;
//		res = pInstance->RPMAddTrustedProcess(processid11, secr);
//
//		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
//
//		res = puser->GetRights(L"C:\\Users\\jzhu\\Downloads\\UserLongNameTest, -2019-04-26-09-57-14.txt.nxl", rightsAndWatermarks, false);
//		//res = pInstance->SetRPMCachedToken();
//
//		//res = pInstance->SetRPMDeleteCacheToken();
//		res = pInstance->RPMDeleteFileToken(L"C:\\SDK test\\RPM\\Dir1\\UserLongNameTest, -2019-04-26-09-57-14.txt");
//		//res = pInstance->CacheRPMFileToken(L"C:\\SDK test\\RPM\\Dir1\\UserLongNameTest, -2019-04-26-09-57-14.txt");
//
//		cout << "Process id: ";
//		cin >> processid11;
//		res = pInstance->RPMAddTrustedProcess(processid11, secr);
//
//		res = pInstance->CacheRPMFileToken(downloaded_p);
//	}
//	catch (exception& e)
//	{
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}

//View Edit centralize Project File
TEST(RPMViewEditCentralizedProjectFileTest, ViewEditCentralizedProjectFileTest) {
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
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		int rights_eval = 0;
		wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
		string filename_lower;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		res = pInstance->RPMRegisterApp(apppath, secr);
		res = pInstance->RPMNotifyRMXStatus(running, secr);
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

		string passcode = puser->GetPasscode();
		string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

		cout << "SetRPMLoginResult: ";
		res = pInstance->SetRPMLoginResult(user_logindata, secr);
		EXPECT_EQ(0, res.GetCode());

		cout << "AddRPMDir: ";
		res = pInstance->AddRPMDir(safe_folder);
		res = pInstance->AddRPMDir(safe_folder_new);

		res = pInstance->SetRPMServiceStop(true, secr);
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

		Json::Value jroot;
		jroot = ParseJsonFile(user_respond);
		string server = jroot["results"]["server"].asString();

		if (jroot["extra"]["memberships"].size() == 2) {
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
				if (id == p_id) {
					tanen_name = jroot["results"]["detail"][i]["tokenGroupName"].asString();
					break;
				}
			}
		}

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
		bool exist_result = file_exist(safe_folder_new_ss);
		EXPECT_FALSE(exist_result);
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

//View Edit system bucket centralize File
TEST(RPMViewEditSystemBucketCPFileTest, ViewEditSytemBucketCPFileTest)
{
	try {
		wstring safe_folder = L"C:\\SDK test\\RPM\\Dir1";
		wstring safe_folder_new = L"C:\\SDK test\\RPM\\Dir2\\";
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
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		int rights_eval = 0;
		wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
		string filename_lower;
		wstring file_name;

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		res = pInstance->RPMRegisterApp(apppath, secr);
		res = pInstance->RPMNotifyRMXStatus(running, secr);
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
		//res = puser->GetLocalFileManager(&pFile);
		//res = puser->GetLocalFileManager(&pFile);
		//size_t no = pFile->GetListNumber();
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		//ISDRmNXLFile* nxlfile = pFile->GetFile(0);
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

			string passcode = puser->GetPasscode();
			string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

			cout << "SetRPMLoginResult: ";
			res = pInstance->SetRPMLoginResult(user_logindata, secr);
			EXPECT_EQ(0, res.GetCode());

			cout << "AddRPMDir: ";
			res = pInstance->AddRPMDir(safe_folder);
			res = pInstance->AddRPMDir(safe_folder_new);

			res = pInstance->SetRPMServiceStop(true, secr);
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
			string conten_str1 = read_filecontent(safe_folder_new_ss);
			EXPECT_STREQ(conten_str1.c_str(), original_content.c_str());
			string new_content1 = "Edit !";
			edit_file(safe_folder_new_ss, new_content);
			res = pInstance->RPMEditSaveFile(s2ws(safe_folder_new_ss), L"", false, 3);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			string conten_saved1 = read_filecontent(safe_folder_new_ss);
			EXPECT_STREQ(conten_saved1.c_str(), new_content1.c_str());
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

//IsRPMFolder, RPMGetFileStatus
TEST(RPMGetFileStatusTest, GetFileStatusTest) {
	try {
		wstring safe_folder = L"C:\\SDK test\\Ancestor\\RPMSafeFolder\\";
		wstring safe_folder_ancestor = L"C:\\SDK test\\Ancestor\\";
		wstring safe_folder_ancestor2 = L"C:\\SDK test\\";
		wstring safe_folder_posterity = L"C:\\SDK test\\Ancestor\\RPMSafeFolder\\Posterity\\";
		wstring unsafe_folder = L"C:\\UnsafeFolder\\";
		wstring safe_folder_file;
		wstring ancestor_folder_file;
		wstring ancestor_folder_file2;
		wstring posterity_folder_file;
		wstring unsafe_folder_file;
		createFolder(ws2s(safe_folder_posterity));
		createFolder(ws2s(unsafe_folder));
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		bool ret = false;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		wstring file_name;
		wstring nxlfile_path;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		res = pInstance->RPMRegisterApp(apppath, secr);
		res = pInstance->RPMNotifyRMXStatus(running, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		if (login_result) {
#pragma region[Prepare a ststembucket nxl file]
			SDR_Expiration expire;
			expire.type = IExpiryType::NEVEREXPIRE;
			wstring localfile_pathw = plan_file_path + txtnormal_testfileb;
			string original_content = read_filecontent(ws2s(localfile_pathw));
			string membership = "";
			string tag_text = "{}";

			std::vector<SDRmFileRight> rights;
			rights.push_back(RIGHT_VIEW);
			rights.push_back(RIGHT_EDIT);
			rights.push_back(RIGHT_DOWNLOAD);

			SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };

			bool adhoc;
			int heartbeat;
			string tenan_str;
			ISDRmNXLFile * file;
			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
			int sysprojectid;
			string sysprojecttenantid;

			res = puser->GetListProjects(1, 10, "-lastActionTime", RF_All);
			res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid, tenan_str);
			string mem_id = puser->GetMembershipID(sysprojectid);
			string default_tokengroup = puser->GetDefaultTokenGroupName();

			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text, mem_id);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			ISDRFiles* pFile = NULL;
			bool delete_result;
			//res = puser->GetLocalFileManager(&pFile);
			//size_t no = pFile->GetListNumber();
			std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

			//ISDRmNXLFile* nxlfile = pFile->GetFile(0);
			ISDRmNXLFile* nxlfile;
			res = puser->OpenFile(nxlfile_path, &nxlfile);
			file_name = nxlfile->GetFileName();

			pInstance->Save();
			res = puser->LogoutUser();
			SDWLibDeleteRmcInstance(pInstance);
#pragma endregion	
		}
		else {
			cout << "Login failed!" << endl;
			EXPECT_TRUE(login_result);
		}

		string cmd_str = "copy /Y \"" + ws2s(nxlfile_path) + "\"  \"" + ws2s(safe_folder) + "\"";
		system(cmd_str.c_str());
		safe_folder_file = safe_folder + file_name;

		string cmd_str_new = "copy /Y \"" + ws2s(nxlfile_path) + "\"  \"" + ws2s(safe_folder_ancestor) + "\"";
		system(cmd_str_new.c_str());
		ancestor_folder_file = safe_folder_ancestor + file_name;

		string cmd_str_new2 = "copy /Y \"" + ws2s(nxlfile_path) + "\"  \"" + ws2s(safe_folder_posterity) + "\"";
		system(cmd_str_new2.c_str());
		posterity_folder_file = safe_folder_posterity + file_name;

		string cmd_str_new3 = "copy /Y \"" + ws2s(nxlfile_path) + "\"  \"" + ws2s(safe_folder_ancestor2) + "\"";
		system(cmd_str_new3.c_str());
		ancestor_folder_file2 = safe_folder_ancestor2 + file_name;

		string cmd_str_unsafe = "copy /Y \"" + ws2s(nxlfile_path) + "\"  \"" + ws2s(unsafe_folder) + "\"";
		system(cmd_str_unsafe.c_str());
		unsafe_folder_file = unsafe_folder + file_name;

		string del_cmd = "del \"" + ws2s(nxlfile_path) + "\"";
		system(del_cmd.c_str());

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

			string passcode = puser->GetPasscode();
			string user_logindata; res = pInstance->GetLoginData(email, passcode, user_logindata);

			cout << "SetRPMLoginResult: ";
			res = pInstance->SetRPMLoginResult(user_logindata, secr);
			EXPECT_EQ(0, res.GetCode());

			cout << "AddRPMDir: ";
			res = pInstance->AddRPMDir(safe_folder);
			res = pInstance->SetRPMServiceStop(true, secr);
#pragma endregion

			uint32_t hearbeat_f = puser->GetHeartBeatFrequency();
			Sleep(1000);
			//Safe folder
			unsigned int dirstatus;
			bool filestatus;
			LPCTSTR lpFileName = safe_folder_file.c_str();
			WIN32_FIND_DATA pNextInfo;
			HANDLE h = FindFirstFile(lpFileName, &pNextInfo);
			if (h != INVALID_HANDLE_VALUE)
			{
				FindClose(h);
			}
			safe_folder_file = safe_folder_file.substr(0, (safe_folder_file.length() - 4));
			res = pInstance->RPMGetFileStatus(safe_folder_file, &dirstatus, &filestatus);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(dirstatus, 0);
			EXPECT_TRUE(filestatus);
			unsigned int dirstatus_folder;
			res = pInstance->IsRPMFolder(safe_folder, &dirstatus_folder);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(0, dirstatus_folder);

			//Ancenstor of safe folder
			unsigned int dirstatus_ancestor;
			bool filestatus_ancestor;
			res = pInstance->RPMGetFileStatus(ancestor_folder_file, &dirstatus_ancestor, &filestatus_ancestor);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(dirstatus_ancestor, 1);
			EXPECT_TRUE(filestatus_ancestor);
			unsigned int dirstatus_folder_ancestor;
			res = pInstance->IsRPMFolder(safe_folder_ancestor, &dirstatus_folder_ancestor);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(dirstatus_folder_ancestor, 1);

			//Posterity of safe folder
			unsigned int dirstatus_posterity;
			bool filestatus_posterity;
			LPCTSTR lpFileName1 = posterity_folder_file.c_str();
			WIN32_FIND_DATA pNextInfo1;
			HANDLE h1 = FindFirstFile(lpFileName1, &pNextInfo1);
			if (h1 != INVALID_HANDLE_VALUE)
			{
				FindClose(h1);
			}
			posterity_folder_file = posterity_folder_file.substr(0, (posterity_folder_file.length() - 4));
			res = pInstance->RPMGetFileStatus(posterity_folder_file, &dirstatus_posterity, &filestatus_posterity);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(dirstatus_posterity, 2);
			EXPECT_TRUE(filestatus_posterity);
			unsigned int dirstatus_folder_posterity;
			res = pInstance->IsRPMFolder(safe_folder_posterity, &dirstatus_folder_posterity);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(dirstatus_folder_posterity, 2);

			//Ancenstor of ancenstor folder
			unsigned int dirstatus_ancestor2;
			bool filestatus_ancestor2;
			res = pInstance->RPMGetFileStatus(ancestor_folder_file2, &dirstatus_ancestor2, &filestatus_ancestor2);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(1, dirstatus_ancestor2);
			EXPECT_TRUE(filestatus_ancestor2);
			unsigned int dirstatus_folder_ancestor2;
			res = pInstance->IsRPMFolder(safe_folder_ancestor2, &dirstatus_folder_ancestor2);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(dirstatus_folder_ancestor2, 1);

			//Unsafe folder
			unsigned int dirstatus_unsafe;
			bool filestatus_unsafe;
			res = pInstance->RPMGetFileStatus(unsafe_folder_file, &dirstatus_unsafe, &filestatus_unsafe);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(3, dirstatus_unsafe);
			EXPECT_TRUE(filestatus_unsafe);
			unsigned int dirstatus_folder_unsafe;
			res = pInstance->IsRPMFolder(unsafe_folder, &dirstatus_folder_unsafe);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(dirstatus_folder_unsafe, 3);

			res = pInstance->RemoveRPMDir(safe_folder, 1);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			unsigned int dirstatus2;
			bool filestatus2;
			safe_folder_file = safe_folder_file + L".nxl";
			res = pInstance->RPMGetFileStatus(safe_folder_file, &dirstatus2, &filestatus2);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(dirstatus2, 3);
			EXPECT_TRUE(filestatus2);
			unsigned int dirstatus_folder2;
			res = pInstance->IsRPMFolder(safe_folder, &dirstatus_folder2);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			EXPECT_EQ(3, dirstatus_folder2);

			res = puser->LogoutUser();
			SDWLibDeleteRmcInstance(pInstance);
		}
		else {
			cout << "Login failed!" << endl;
			EXPECT_TRUE(login_result);
		}

		string del_cmd1 = "del /F /S /Q \"" + ws2s(safe_folder_ancestor) + "\"";
		system(del_cmd1.c_str());

		string del_cmd2 = "del /F /S /Q \"" + ws2s(unsafe_folder) + "\"";
		system(del_cmd2.c_str());
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
