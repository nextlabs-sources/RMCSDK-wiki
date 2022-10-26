// RMCAPITest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common/celog2/celog.h"
#include "SDLInc.h"

using namespace std;

//#define __Test__SKYDRM__
//#define __Test__JPC__

#define CELOG_CUR_MODULE    "RMCAPITest"
#define CELOG_CUR_FILE      ((celog_filepathint_t) 301)

const std::string  ReadFromFile(std::string filename);
void Test_SoftwareUpdate(ISDRmcInstance * pInstance);
void Test_Recipients(ISDRmUser *puser);
void Test_ProtectFile(ISDRmUser *puser);
void Test_MyDrive(ISDRmUser *puser);
void Test_FileProperty(ISDRmUser *puser);
void Test_GetLoginUser(ISDRmcInstance * pInstance, const std::string &security);
void Test_OpenFiles(ISDRmUser *puser);
void Test_DecryptFile(ISDRmUser *puser);
void Test_ActivityLog(ISDRmUser *puser);
void Test_UploadFile(ISDRmUser *puser);
void Test_Project(ISDRmUser *puser);
void Test_RPM(ISDRmcInstance * pInstance, const std::string &jsonreturn, const std::string &security);
void Test_RPMLogout(ISDRmcInstance * pInstance);
void Test_Instance(const std::string &clientid, const std::string &jsonreturn, const std::string &security);
void Test_PolicyBundle(ISDRmcInstance * pInstance, ISDRmTenant *pTenant, ISDRmUser *puser);
void Test_CentralPolicies(ISDRmUser *puser);
void Test_RPM_login(ISDRmcInstance * pInstance, const std::string &security);
void Test_AppWhiteListing(ISDRmcInstance *pInstance, const std::string &security);
void Test_EditMode(ISDRmcInstance *pInstance);
void Test_PartialDecrypt(ISDRmUser *puser);
void Test_PartialDecryptEx(ISDRmUser *puser);


int main(int argc, const char *argv[])
{
	CELog_Init();
	CELOG_LOG(CELOG_INFO, L"%hs starts.\n", argv[0]);
	SDWLibInit();

	DWORD dwVersion;
	dwVersion = SDWLibGetVersion();

	ISDRmcInstance * pInstance;
	ISDRmTenant *pTenant;
	SDWLResult res;

	WCHAR curDir[MAX_PATH];
	GetCurrentDirectory(_countof(curDir), curDir);

	const std::string security = ReadFromFile("security.txt");;
	const std::string clientid = ReadFromFile("clientid.txt");
	const std::string user_return_s = ReadFromFile("logins.txt");
	wcout << L"Press any key to continue...";
	_getch();
	wcout << endl;

	res = SDWLibCreateSDRmcInstance("RMC API Test App", 12, 34, 5678, L"C:\\test_Debug", L"c:\\temp", &pInstance, clientid.c_str(), 0);

	pTenant = NULL;
#if defined(__Test__SKYDRM__)
 	pInstance->Initialize(L"c:\\work", L"https://r.skydrm.com", L"skydrm.com");
#else
	res = pInstance->Initialize(L"C:\\work", L"https://rmtest.nextlabs.solutions", L"skydrm.com"); 
#endif


	// At this point, the app can, and should, spend time on some other
	// processing not involving pInstance, before it calls IsInitFinished() to
	// check if pInstance has finished initialization.
#ifdef __Test__JPC__
	bool finished;
	do {
		wcout << L"Waiting for pInstance to finish initialization..." << endl;
		Sleep(1000);
		res = pInstance->IsInitFinished(finished);
		if (!res) {
			wcout << L"pInstance->IsInitFinished return error, res=" << res.ToString() << endl;
			return 1;
		}
	} while (!finished); 
#endif

	res = pInstance->GetCurrentTenant(&pTenant);
 	wcout << L"Tenant ID:" << pTenant->GetTenant() << endl;
 	wcout << L"Router URL:" << pTenant->GetRouterURL() << endl;
 	wcout << L"RMS URL:" << pTenant->GetRMSURL() << endl;
 	wcout << endl;
	//pInstance->Save();
	//pInstance->Save(L"c:\\work");

	//Test_SoftwareUpdate(pInstance);

	ISDRmHttpRequest *prequest = NULL;
	res = pInstance->GetLoginRequest(&prequest);
	wcout << L"Login URL:" << prequest->GetDomainUrl() << endl;
	wcout << L"Login URI:" << prequest->GetPath() << endl;
	wcout << L"HTTP Method:" << prequest->GetMethod() << endl;
	wcout << endl;
	for each (HttpCookie cookie in prequest->GetCookies())
	{
		wcout << "Cookie name:" << cookie.first;
		wcout << "  Vaule:" << cookie.second << endl;
	}
	for each(wstring htype in prequest->GetAcceptTypes()) {
		wcout << "Accept Type: " << htype << endl;
	}
	wcout << endl;

	// test this function only after SetLoginResult and have passcode
	//Test_GetLoginUser(pInstance, security);
	//Test_RPM_login(pInstance, security);
	Test_RPM(pInstance, user_return_s, security);
	//CELog_Destroy();
	//SDWLibCleanup();
	//return 0;

	ISDRmUser *puser = NULL;
	res = pInstance->SetLoginResult(user_return_s, &puser, security);

	if (!puser)
	{
		wcout << "SetLoginResult failed: " << res.GetMsg().c_str() << endl;
		wcout << L"Press any key to continue...";
		_getch();

		CELog_Destroy();
		SDWLibCleanup();
		return 1;
	}
	wcout << "Username: " << puser->GetName() << endl;
	wcout << "Email address: " << puser->GetEmail() << endl;
	wcout << "IDP type: " << puser->GetIdpType() << endl;
	cout << "passcode : " << puser->GetPasscode() << endl;	
	Test_PartialDecrypt(puser);
	CELog_Destroy();
	SDWLibCleanup();
	return 0;

	Test_CentralPolicies(puser);
	bool adhoc = false; int heartbeat = 1;
	std::string tenantid = "21b06c79-baab-419d-8197-bad3ce3f4476";
	//res = puser->GetTenantPreference(&adhoc, &heartbeat, tenantid);
	//adhoc = puser->IsTenantAdhocEnabled();

	//res = pInstance->RPMDeleteFileToken(L"C:\\work\\RMSTokenTest-2018-09-22-01-06-AD.txt.nxl");

	uint32_t option = 2;
	uint64_t startDate = (uint64_t)std::time(nullptr) * 1000;
	uint64_t endDate = startDate + 5 * 60 * 1000;
	//res = puser->UpdateUserPreferenceQuery(option, startDate, endDate);
	Test_Project(puser);
	//Test_ProtectFile(puser);
	//Test_UploadFile(puser);
	//SDWLibDeleteRmcInstance(pInstance);
	CELog_Destroy();
	SDWLibCleanup();
	return 0;

	bool revoke = false;
	//res = puser->IsFileRevoked(L"c:\\work\\RMHeartbeatData-2018-09-26-18-yahoo.txt.nxl", revoke);

	SDR_FILE_META_DATA metadata;
	std::wstring filepath = L"C:\\work\\RMSTokenTest-2018-11-16-00-42-58.txt.nxl";
	std::string pathId = "/rmstokentest-2018-08-03-19-01-52.txt.nxl";
	//res = puser->GetNXLFileMetadata(filepath, pathId, metadata);
	
	//Test_PolicyBundle(pInstance, pTenant, puser);
	//Test_OpenFiles(puser);
	Test_RPMLogout(pInstance);
	Test_RPM(pInstance, user_return_s, security);
	//SDWLibDeleteRmcInstance(pInstance);
	CELog_Destroy();
	SDWLibCleanup();
	return 0;

	
	//Test_Project(puser);	
	//Test_Recipients(puser);
	//Test_MyDrive(puser);	
	//Test_DecryptFile(puser);
	Test_ProtectFile(puser);
	//Test_OpenFiles(puser);
	Test_UploadFile(puser);
	//Test_ActivityLog(puser);
	//Test_FileProperty(puser);
	
	
	Test_AppWhiteListing(pInstance, security);
	
	SDWLibDeleteRmcInstance(pInstance);

	wcout << L"Press any key to quit...";
	_getch();

	CELog_Destroy();
	SDWLibCleanup();
	return 0;
}

void Test_SoftwareUpdate(ISDRmcInstance *pInstance)
{
	if (!pInstance)
		return;

	SDWLResult res;

	std::string newVersionStr, downloadURL, checksum;
	res = pInstance->CheckSoftwareUpdate(newVersionStr, downloadURL, checksum);
	if (res) {
		cout << "New version: " << newVersionStr << endl;
		cout << "Download URL: " << downloadURL << endl;
		cout << "SHA1 checksum: " << checksum << endl;
	} else {
		wcout << L"CheckSoftwareUpdate failed, res=" << res.ToString() << endl;
	}
	cout << endl;
}

void Test_GetLoginUser(ISDRmcInstance * pInstance, const std::string &security)
{
	if (!pInstance)
		return;

	SDWLResult res;
	std::string useremail = "simon.tsai@nextlabs.com";
	std::string passcode = "85952B07C606824777DD7B23D2422F9E";
	ISDRmUser *puser1 = NULL;
	res = pInstance->GetLoginUser(useremail, passcode, &puser1);
	if (!puser1)
		return;
	std::string loginData;
	res = pInstance->GetLoginData(useremail, passcode, loginData);
	cout << "GetLoginData: status:  " << res.GetCode() << endl;
	cout << "GetLoginData: message:  " << res.GetMsg() << endl;
	cout << "login data: " << loginData << endl;

	res = pInstance->SetRPMLoginResult(loginData, security);
}

void Test_FileProperty(ISDRmUser *puser)
{
	if (!puser)
		return;

	SDWLResult res;
	ISDRmNXLFile* nxlfile = NULL;
	res = puser->OpenFile(L"C:\\test_Debug\\RMSTokenTest-2018-10-03-22-46-17.txt.nxl", &nxlfile);
	if (!nxlfile)
		return;

	std::vector<SDRmFileRight> vec = nxlfile->GetRights();

	bool ret = nxlfile->IsOpen();
	if (!ret)
	{
		cout << "nxl file is not opened, please call OpenFile first " << endl;
		return;
	}
	SDR_WATERMARK_INFO info = nxlfile->GetWaterMark();
	cout << "WATERMARK text: " << info.text << endl;

	SDR_Expiration expire = nxlfile->GetExpiration();
	cout << "Expiration type: " << expire.type << endl;
	cout << "Expiration end: " << expire.end << endl;

	std::string	tags = nxlfile->GetTags();
	cout << "tags: " << tags << endl;

}

void Test_DecryptFile(ISDRmUser *puser)
{
	if (!puser)
		return;

	SDWLResult res;
	ISDRmNXLFile* nxlfile = NULL;
	res = puser->OpenFile(L"C:\\test_Debug\\RMSTokenTest-2018-10-03-22-46-17.txt.nxl", &nxlfile);
	if (!nxlfile)
		return;

	//res = puser->DecryptNXLFile(nxlfile, L"C:\\work\\test\\test2.txt");
	cout << "DecryptNXLFile: status:  " << res.GetCode() << endl;
	cout << "DecryptNXLFile: message:  " << res.GetMsg() << endl;
}

void Test_UploadFile(ISDRmUser *puser)
{
	if (!puser)
		return;

	SDWLResult res;

	const std::wstring fileName = L"C:\\test_Debug\\RMSTokenTest-2018-10-03-22-46-17.txt.nxl";
	res = puser->UploadFile(fileName);
	cout << "UploadFile: status:  " << res.GetCode() << endl;
	cout << "UploadFile: message:  " << res.GetMsg() << endl;

}

void Test_ActivityLog(ISDRmUser *puser)
{
	if (!puser)
		return;

	SDWLResult res;
	ISDRmNXLFile* nxlfile = NULL;
	res = puser->OpenFile(L"C:\\test_Debug\\RMSTokenTest-2018-10-03-22-46-17.txt.nxl", &nxlfile);
	if (!nxlfile)
		return;

	std::wstring filename;
	std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
	filename = nxlfile->GetFileName();
	res = puser->GetNXLFileActivityLog(nxlfile, 0, 20, RL_SFEmpty, "", RL_OBAccessTime, false);
	cout << "ActivityLog: status:  " << res.GetCode() << endl;
	cout << "ActivityLog: message:  " << res.GetMsg() << endl;

	res = puser->GetActivityInfo(filename, infoVec);
	cout << "ActivityInfo: status:  " << res.GetCode() << endl;
	cout << "ActivityInfo: message:  " << res.GetMsg() << endl;
}


void Test_OpenFiles(ISDRmUser *puser)
{
	if (!puser)
		return;

	SDWLResult res;
	ISDRmNXLFile* nxlfile = NULL;
	res = puser->OpenFile(L"c:\\work\\RMSTokenTest-2018-11-08-22-expire.txt.nxl", &nxlfile);
	//res = puser->OpenFile(L"c:\\work\\skydrm.com\\260\\RMSTokenTest-2018-07-02-23-19-16.txt.nxl", &nxlfile);
	//res = puser->OpenFile(L"c:\\work\\skydrm.com\\260\\RMSTokenTest-2018-06-28-18-31-59.txt.nxl", &nxlfile);
	cout << "OpenFile: status:  " << res.GetCode() << endl;
	cout << "OpenFile: message:  " << res.GetMsg() << endl;

	if (!nxlfile)
		return;

	bool bval = false;

	cout << "tenant: " << nxlfile->GetTenantName() << endl;
	//std::string	 ownerid = nxlfile->GetOwnerID();
	bval = nxlfile->CheckExpired();
	bval = nxlfile->IsOwner("m5323@skydrm.com");

	std::vector<SDRmFileRight> vec = nxlfile->GetRights();
	SDR_WATERMARK_INFO info = nxlfile->GetWaterMark();
	cout << "WATERMARK text: " <<info.text << endl;

	SDR_Expiration expire = nxlfile->GetExpiration();
	cout << "Expiration type: " << expire.type << endl;
	cout << "Expiration end: " << expire.end << endl;

	std::string	tags = nxlfile->GetTags();
	cout << "tags: " << tags << endl;

}

void Test_ProtectFile(ISDRmUser *puser)
{
	if (!puser)
		return;

	SDWLResult res;
	std::vector<SDRmFileRight> rights;
	SDR_WATERMARK_INFO watermarkinfo;
	SDR_Expiration expire;
	std::string tags;
	
#if 0
	//
	// Protect file using Ad-hoc Policy
	//

	rights.push_back(RIGHT_VIEW);
	rights.push_back(RIGHT_PRINT);
	//rights.push_back(RIGHT_CLIPBOARD);
	rights.push_back(RIGHT_SHARE);
	rights.push_back(RIGHT_DOWNLOAD);
	rights.push_back(RIGHT_WATERMARK);
	watermarkinfo.text = "$(User)$(Date)$(Time)";
	watermarkinfo.fontName = "";
	watermarkinfo.fontColor = "";
	watermarkinfo.fontSize = 20;
	watermarkinfo.transparency = 2;
	watermarkinfo.rotation = NOROTATION;
	watermarkinfo.repeat = true; 
	expire.type = IExpiryType::RANGEEXPIRE;
	expire.start = (uint64_t)std::time(nullptr) * 1000;
	expire.start += (uint64_t)(10 * 24 * 60 * 60 * 1000); // 10 days
	expire.end = expire.start + (uint64_t)(1000 * 60 * 60);
#else
	//
	// Protect file using Central Policy
	//

	//tags = "{\"Tag 1\":[\"Value 1a\", \"Value 1b\"], \"Tag 2\":[\"Value 2\"]}";

	// We want to add these attributes values with Chinese characters:
	//      Country = ZhongGuo
	//      City    = HangZhou
	//
	// JSON strings are UTF-8 encoded.
	//
	// Character    Unicode   UTF-8 encoding
	// ---------    -------   --------------
	// Zhong        0x4E2D    0xE4 0xB8 0xAD
	// Guo          0x570B    0xE5 0x9C 0x8B
	// Hang         0x676D    0xE6 0x9D 0xAD
	// Zhou         0x5DDE    0xE5 0xB7 0x9E
	//
	// There are two methods for specifying UTF-8 strings in the source code:
	// - Method 1 : We only specify the Unicode values in the source code, and
	//   let the compiler do the UTF-8 encoding.
	// - Method 2: We specify the UTF-8 encoded values in the source code
	//   directly.
	// After compilation, these two methods should produce exactly the same
	// string anyway.
	std::string tags2;
	tags = u8"{\"Country\":[\"\u4E2D\u570B\"],\"City\":[\"\u676D\u5DDE\"]}";
	tags2 = "{\"Country\":[\"\xE4\xB8\xAD\xE5\x9C\x8B\"],\"City\":[\"\xE6\x9D\xAD\xE5\xB7\x9E\"]}";
	assert(tags == tags2);
#endif

	std::wstring newcreatedfilePath;
	res = puser->ProtectFile(L"C:\\work\\RMSTokenTest.txt", newcreatedfilePath, rights, watermarkinfo, expire, tags, "");
	if (!res)
		return;

	ISDRmNXLFile* nxlfile = NULL;
	res = puser->OpenFile(L"C:\\test_Debug\\RMSTokenTest-2018-10-03-22-46-17.txt.nxl", &nxlfile);
	if (!nxlfile)
		return;
	
	std::vector<SDRmFileRight> vec = nxlfile->GetRights();
	

}

void Test_Recipients(ISDRmUser *puser)
{
	if (!puser)
		return;

	std::vector<std::string> addrecipientsemail = { "jewelry.zhu@nextlabs.com" };
	SDWLResult res;
	
	ISDRmNXLFile* nxlfile = NULL;
	res = puser->OpenFile(L"C:\\test_Debug\\RMSTokenTest-2018-10-03-22-46-17.txt.nxl", &nxlfile);
	if (!nxlfile)
		return;
	res = puser->UpdateRecipients(nxlfile, addrecipientsemail);	
}

void Test_MyDrive(ISDRmUser *puser)
{
	if (!puser)
		return;

	SDWLResult res;
	uint64_t usage = 0;
	uint64_t totalquota = 0;
	uint64_t vaultusage;
	uint64_t vaultquota;
	res = puser->UpdateMyDriveInfo();
	puser->GetMyDriveInfo(usage, totalquota, vaultusage, vaultquota);
	cout << "usage : " << usage << endl;
	cout << "totalquota : " << totalquota << endl;
	cout << "vaultusage : " << vaultusage << endl;
	cout << "vaultquota : " << vaultquota << endl;
	cout << endl;

}

void Test_Project(ISDRmUser *puser)
{
	if (!puser)
		return;

	SDWLResult res;
	std::string orderBy = "";
	std::string searchValue = "";
	std::string pathId = "/";
	uint32_t projid = 4332;
	std::vector<SDR_PROJECT_FILE_INFO> listfiles;
	std::vector<SDR_CLASSIFICATION_CAT> cats;
	std::vector<SDRmFileRight> rights;
	
	res = puser->GetListProjects(1, 20, orderBy, RF_All);
	std::vector<SDR_PROJECT_INFO> infoVec = puser->GetProjectsInfo();
	for (size_t i=0; i < infoVec.size(); i++)
	{
		SDR_PROJECT_INFO info = infoVec[i];
		//res = puser->GetClassification(info.projid, cats);
		res = puser->GetProjectListFiles(info.projid, 1, 20, orderBy, pathId, searchValue, listfiles);
		res = puser->GetProjectFileRights("4332", listfiles[0].m_pathid, rights);
	}
	
	std::string wpathId = "/tokentest_mlempty-2018-05-24-22-16-17.txt.nxl";
	const std::wstring downloadPath = L"c:\\work";
	
	//res = puser->ProjectDownloadFile(4332, wpathId, downloadPath, false);
	//res = puser->GetMyVaultFileRights(duid, pathid, rights);

}

void Test_Instance(const std::string &clientid, const std::string &jsonreturn, const std::string &security)
{
	ISDRmcInstance * pInstance;
	ISDRmTenant *pTenant;
	SDWLResult res;

	WCHAR curDir[MAX_PATH];
	GetCurrentDirectory(_countof(curDir), curDir);

	res = SDWLibCreateSDRmcInstance("RMC API Test App", 12, 34, 5678, curDir, L"c:\\temp", &pInstance, clientid.c_str(), 0);

	pTenant = NULL;
#if defined(__Test__SKYDRM__)
	pInstance->Initialize(L"c:\\work", L"https://r.skydrm.com", L"skydrm.com");
#else
	res = pInstance->Initialize(L"C:\\work", L"https://rmtest.nextlabs.solutions", L""); //"skydrm.com"
#endif
	ISDRmUser *puser = NULL;
	res = pInstance->SetLoginResult(jsonreturn, &puser, security);
	if (!puser)
	{
		wcout << "SetLoginResult failed: " << res.GetMsg().c_str() << endl;
		return;
	}
	res = pInstance->GetCurrentTenant(&pTenant);
	wcout << L"Tenant ID:" << pTenant->GetTenant() << endl;
	SDWLibDeleteRmcInstance(pInstance);
}

void Test_RPM_login(ISDRmcInstance * pInstance, const std::string &security)
{
	if (!pInstance)
		return;
	SDWLResult res;

	if (pInstance->IsRPMDriverExist())
	{
		wcout << L"Press any key to continue...";
		_getch();
		wcout << endl;

		const std::string useremail = "simon.tsai@nextlabs.com";
		const std::string passcode = "85952B07C606824777DD7B23D2422F9E";
		std::string loginData;
		res = pInstance->GetLoginData(useremail, passcode, loginData);
		res = pInstance->SetRPMLoginResult(loginData, security);
		res = pInstance->SetRPMServiceStop(true, security);
		res = pInstance->AddRPMDir(L"c:\\test\\Safe Dir 2");
	}
}

void Test_RPM(ISDRmcInstance * pInstance, const std::string &jsonreturn, const std::string &security)
{
	if (!pInstance)
		return;
	SDWLResult res;
	unsigned long status = 0;
	std::string msg;
	std::wstring path;
	std::wstring router; std::wstring tenant; std::wstring workingfolder; std::wstring tempfolder; std::wstring sdklibfolder;
	std::vector<SDRmFileRight> filerights;
	uint32_t dirstatus = 0;
	bool filestatus = false;
	std::wstring subkey = L"Software\\NextLabs\\RPM\\Router";;
	std::wstring name = L"Directory";
	std::wstring data = L"C:\\test\\Safe Dir 2";
	uint32_t op = 0;

	if (pInstance->IsRPMDriverExist())
	{
		wcout << L"Press any key to continue...";
		_getch();
		wcout << endl;

		res = pInstance->SetRPMServiceStop(true, security);
		//res = pInstance->RPMSetAppRegistry(subkey, name, data, op+1);
		//res = pInstance->RPMSetAppRegistry(subkey, name, data, op);
		//res = pInstance->RPMGetFileStatus(L"C:\\test\\Safe Dir 2\\non.txt.nxl", &dirstatus, &filestatus);
		//res = pInstance->RPMGetFileStatus(L"C:\\test\\Safe Dir 2\\RMHeartbeatData-2018-09-26-18-yahoo.txt.nxl", &dirstatus, &filestatus);
		//res = pInstance->RPMGetFileStatus(L"C:\\test\\Safe Dir 2\\RMHeartbeatData-2018-09-26-18-yahoo.txt", &dirstatus, &filestatus);

		res = pInstance->SetRPMLoginResult(jsonreturn, security);
		//res = pInstance->RPMGetSecretDir(path);
		//wcout << "SecretDir: " << path << endl;
		res = pInstance->AddRPMDir(L"c:\\test\\Safe Dir 2");

		res = pInstance->RPMRegisterApp(L"c:\\Program Files\\Notepad++\\notepad++.exe", security);

		//res = pInstance->RPMDeleteFileToken(L"c:\\test\\Safe Dir 2\\RMSTokenTest-2018-09-22-01-06-AD.txt.nxl");
		//res = pInstance->RPMIsFolder(L"c:\\test\\Safe Dir 2\\002");
		//res = pInstance->RPMGetFileRights(L"C:\\test\\Safe Dir 2\\RMSTokenTest-2018-09-22-01-06-AD.txt.nxl", filerights);
		//res = pInstance->RPMGetFileRights(L"C:\\test\\Safe Dir 2\\RMSTokenTest-2018-09-22-01-06-AD.txt", filerights);
		//res = pInstance->RPMDeleteFile(L"C:\\test\\Safe Dir 2\\RMSTokenTest-2018-09-21-central.txt.nxl");
		//cout << "RPMDeleteFile return: " << res.GetMsg() << endl;
		//res = pInstance->RPMDeleteFile(L"C:\\test\\Safe Dir 2\\RMSTokenTest-2018-09-21-central.txt");

		//res = pInstance->RPMGetCurrentUserInfo(router, tenant, workingfolder, tempfolder, sdklibfolder);
		wcout << "router: " << router << endl;
		wcout << "workingfolder: " << workingfolder << endl;
		wcout << "sdklibfolder: " << sdklibfolder << endl;

		//res = pInstance->SyncUserAttributes();
		//res = pInstance->CacheRPMFileToken(L"C:\\work\\RMSTokenTest-2018-09-22-01-06-AD.txt.nxl");
		//res = pInstance->SetRPMCachedToken();
		//res = pInstance->RPMDeleteFileToken(L"C:\\work\\RMSTokenTest-2018-09-22-01-06-AD.txt.nxl");

		/*res = pInstance->AddRPMDir(L"c:\\test\\Safe Dir 2");
		res = pInstance->RPMIsFolder(L"c:\\test\\Safe Dir 2");
		if (res.GetCode() >= SDWL_RMS_ERRORCODE_BASE)
			cout << "return code: " << res.GetCode() - SDWL_RMS_ERRORCODE_BASE << endl;
		res = pInstance->RPMIsFolder(L"c:\\test\\Safe Dir 2\\123");
		if (res.GetCode() >= SDWL_RMS_ERRORCODE_BASE)
			cout << "return code: " << res.GetCode() - SDWL_RMS_ERRORCODE_BASE << endl;
		res = pInstance->RPMIsFolder(L"c:\\test");
		if (res.GetCode() >= SDWL_RMS_ERRORCODE_BASE)
			cout << "return code: " << res.GetCode() - SDWL_RMS_ERRORCODE_BASE << endl; */
	}
}

void Test_RPMLogout(ISDRmcInstance * pInstance)
{
	if (!pInstance)
		return;
	SDWLResult res;

	if (pInstance->IsRPMDriverExist())
	{
		wcout << L"Press any key to continue...";
		_getch();
		wcout << endl;

		res = pInstance->RPMLogout();
	}
}

void Test_PolicyBundle(ISDRmcInstance * pInstance, ISDRmTenant *pTenant, ISDRmUser *puser)
{
	std::string policybundle;
	bool bval = false;
	SDWLResult res;

	res = puser->GetHeartBeatInfo();
	bval = puser->GetPolicyBundle(pTenant->GetTenant(), policybundle);
	if (bval)
		cout << "policybundle: " << policybundle << endl;
	SDR_WATERMARK_INFO winfo = puser->GetWaterMarkInfo();
	cout << "WaterMark text: " << winfo.text << endl;
	cout << "WaterMark fontName: " << winfo.fontName << endl;
	cout << "WaterMark fontColor: " << winfo.fontColor << endl;
	cout << "WaterMark fontSize: " << winfo.fontSize << endl;
	cout << "WaterMark rotation: " << winfo.rotation << endl;
	cout << endl;
}

void Test_CentralPolicies(ISDRmUser *puser)
{
	SDWLResult res;

	// Test getting file rights and watermarks from Central Policies.
	const std::wstring fileName = L"C:\\Test\\RMSTokenTest-2018-09-21-central.txt.nxl";
	std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
	res = puser->GetFileRightsFromCentralPolicies(fileName, rightsAndWatermarks);
	wcout << L"File: " << fileName << endl;
	if (res) {
		wcout << L"Rights:" << endl;
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
	} else {
		wcout << L"GetFileRightsFromCentralPolicies failed, res=" << res.ToString() << endl;
	}
	cout << endl;

	// Test getting resource rights and obligations from Central Policies.
	const std::wstring resourceName = L"sharepoint://sharepoint.nextlabs.com/engineering/Engineering%20Documents";
	const std::wstring resourceType = L"portal";
	const std::vector<std::pair<std::wstring, std::wstring>> attrs = {
		{L"Key3", L"Value3"}, {L"Key4", L"Value4"}
	};
	std::vector<std::pair<SDRmFileRight, std::vector<SDR_OBLIGATION_INFO>>> rightsAndObligations;
	res = puser->GetResourceRightsFromCentralPolicies(resourceName, resourceType, attrs, rightsAndObligations);
	wcout << L"Resource: " << resourceName << endl;
	if (res) {
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
	} else {
		wcout << L"GetResourceRightsFromCentralPolicies failed, res=" << res.ToString() << endl;
	}
	cout << endl;
}

bool TryOpenFile(const std::wstring& nativeFileName)
{
	HANDLE h = CreateFile(nativeFileName.c_str(), GENERIC_READ, FILE_SHARE_READ,
						  NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) {
		wcout << L"CreateFile(" << nativeFileName << L") failed, lastErr=" << GetLastError() << endl;
		return false;
	}

	BYTE buf[10];
	DWORD bytesRead;
	BOOL bRet;

	bRet = ReadFile(h, buf, sizeof buf, &bytesRead, NULL);
	if (bRet) {
		for (int i = 0; i < _countof(buf); i++) {
			printf("0x%02hX ", (unsigned short) buf[i]);
		}
		printf("\n");
	}

	CloseHandle(h);
	return true;
}

void Test_AppWhiteListing(ISDRmcInstance *pInstance, const std::string &security)
{
	SDWLResult res;

	std::wstring appPath, appPathWrongCase, appPathWrongCaseShort;
	bool registered;
	std::wstring nativeFileName;
	bool ret;

	appPath = L"C:\\test_Debug\\RMCAPITest.exe";
	appPathWrongCase = L"c:\\TEST_dEBUG\\rmcapitEST.EXE";
	appPathWrongCaseShort = L"c:\\TEST_d~1\\rmcapi~1.EXE";



    // ******************** STEP 1 ********************

	// Opening file before registering app should see NXL content.
	nativeFileName = L"C:\\test\\Safe Dir 2\\1.txt";
	ret = TryOpenFile(nativeFileName);



    // ******************** STEP 2 ********************

	// Register this app.
	res = pInstance->RPMRegisterApp(appPath, security);
	if (!res) {
		wcout << L"RPMRegisterApp failed, res=" << res.ToString() << endl;
	}

	// Check if app is registered.  (It should be Yes.)
	res = pInstance->RPMIsAppRegistered(appPathWrongCase, &registered);
	if (!res) {
		wcout << L"RPMIsAppRegistered failed, res=" << res.ToString() << endl;
	} else {
		wcout << L"RPMIsAppRegistered(\"" << appPathWrongCase << "\") returned registered=" << registered << endl;
	}

	res = pInstance->RPMIsAppRegistered(appPathWrongCaseShort, &registered);
	if (!res) {
		wcout << L"RPMIsAppRegistered failed, res=" << res.ToString() << endl;
	} else {
		wcout << L"RPMIsAppRegistered(\"" << appPathWrongCaseShort << "\") returned registered=" << registered << endl;
	}

	// Opening file before notifying RMX running should see NXL content.
	nativeFileName = L"C:\\test\\Safe Dir 2\\2.txt";
	ret = TryOpenFile(nativeFileName);



    // ******************** STEP 3 ********************

	// Notify that RMX is running.
	res = pInstance->RPMNotifyRMXStatus(true, security);
	if (!res) {
		wcout << L"RPMNotifyRMXStatus failed, res=" << res.ToString() << endl;
	}

	// Opening file after notifying RMX running should see original content.
	nativeFileName = L"C:\\test\\Safe Dir 2\\3.txt";
	ret = TryOpenFile(nativeFileName);



    // ******************** STEP 4 ********************

	// Add the other process as trusted.
	unsigned long processId;
	wcout << L"Enter process ID to add as trusted: ";
	wcin >> processId;
	res = pInstance->RPMAddTrustedProcess(processId, security);
	if (!res) {
		wcout << L"RPMAddTrustedProcess failed, res=" << res.ToString() << endl;
	}

	// The other process opening file now should see original content.
	wcout << L"After RPMAddTrustedProccess.  Press any key to continue...";
	_getch();
	wcout << endl;



    // ******************** STEP 5 ********************

	// Remove the other process as trusted.
	res = pInstance->RPMRemoveTrustedProcess(processId, security);
	if (!res) {
		wcout << L"RPMRemoveTrustedProcess failed, res=" << res.ToString() << endl;
	}

	// The other process opening file now should see NXL content.
	wcout << L"After RPMRemoveTrustedProccess.  Press any key to continue...";
	_getch();
	wcout << endl;



    // ******************** STEP 6 ********************

	// Notify that RMX is not running.
	res = pInstance->RPMNotifyRMXStatus(false, security);
	if (!res) {
		wcout << L"RPMNotifyRMXStatus failed, res=" << res.ToString() << endl;
	}

	// Opening file after notifying RMX not running should see NXL content.
	nativeFileName = L"C:\\test\\Safe Dir 2\\4.txt";
	ret = TryOpenFile(nativeFileName);



    // ******************** STEP 7 ********************

	// Unregister this app.
	res = pInstance->RPMUnregisterApp(appPath, security);
	if (!res) {
		wcout << L"RPMUnregisterApp failed, res=" << res.ToString() << endl;
	}

	// Check if app is registered.  (It should be No.)
	res = pInstance->RPMIsAppRegistered(appPath, &registered);
	if (!res) {
		wcout << L"RPMIsAppRegistered failed, res=" << res.ToString() << endl;
	} else {
		wcout << L"RPMIsAppRegistered(\"" << appPath << "\") returned registered=" << registered << endl;
	}

	// Opening file after unregistering app should see NXL content.
	nativeFileName = L"C:\\test\\Safe Dir 2\\5.txt";
	ret = TryOpenFile(nativeFileName);
}

void Test_EditMode(ISDRmcInstance *pInstance)
{
	SDWLResult res;

	const std::string security = ReadFromFile("security.txt");;
	const std::wstring appPath = L"C:\\test_Debug\\RMCAPITest.exe";
	res = pInstance->RPMRegisterApp(appPath, security);
	res = pInstance->RPMNotifyRMXStatus(true, security);

	const std::wstring filePath = L"C:\\test_Debug\\RMSTokenTest.txt.nxl";
	std::wstring secretDir;
	std::wstring destPath;

	res = pInstance->RPMGetSecretDir(secretDir);
	if (!res) {
		wcout << L"Test_EditMode: RPMGetSecretDir failed, res=" << res.ToString() << endl;
	} else {
		wcout << L"Test_EditMode: secretDir=" << secretDir << endl;
	}

	destPath = secretDir;
	res = pInstance->RPMEditCopyFile(filePath, destPath);
	if (!res) {
		wcout << L"Test_EditMode: RPMEditCopyFile failed, res=" << res.ToString() << endl;
	} else {
		wcout << L"Test_EditMode: destPath=" << destPath << endl;
	}

	// Append something to the file
	std::ofstream ofs(destPath, std::ofstream::ate);
	ofs << "My process ID is " << GetCurrentProcessId() << std::endl;
	ofs.close();

	bool deleteSource = true;
	uint32_t exitEdit = 3;
	res = pInstance->RPMEditSaveFile(destPath, L"", deleteSource, exitEdit);
	if (!res) {
		wcout << L"Test_EditMode: RPMEditSaveFile failed, res=" << res.ToString() << endl;
	}
}

const std::string  ReadFromFile(std::string filename)
{
	std::string retstr;

	FILE * testfile;
	fopen_s(&testfile, filename.c_str(), "r");
	if (testfile == NULL) {
		return retstr;
	}

	char buffer[257];
	while (!feof(testfile)) {
		memset(buffer, 0, sizeof(buffer));
		size_t s = fread(buffer, sizeof(char), sizeof(buffer) - 1, testfile);
		retstr += buffer;
	}
	fclose(testfile);

	return retstr;
}

void Test_PartialDecrypt(ISDRmUser *puser)
{
	if (!puser)
		return;

	SDWLResult res;
	const std::string nxlFile("c:\\test\\Safe Dir 2\\RMSTokenTest-2018-09-22-01-06-AD.txt.nxl");
	//const std::string nxlFile("c:\\test\\Safe Dir 2\\RMSTokenTest-2018-11-16-00-42-58.txt.nxl");
	const std::string decryptedFile("PDDecryptPartial.out");
	std::fstream fp;
	const long header_len = 4096 * 3;   // Must be big enough to include FilePolicy section
	unsigned char header[header_len];
	const long in_len = 512;
	unsigned char in[in_len];
	long offset = 0;
	unsigned char out[2048];
	long out_len;
	unsigned int contentOffset;
	int64_t contentLength;

	fp.open(nxlFile, ios_base::in | ios_base::binary);
	if (!fp.is_open())
	{
		std::cout << "Error opening file" << std::endl;
		return;
	}
	std::ofstream ofs;
	ofs.open(decryptedFile, ios_base::trunc | ios_base::binary | ios_base::out);
	if (!ofs.is_open())
	{
		std::cout << "Error opening output file" << std::endl;
		return;
	}

	memset(header, 0, header_len);

	fp.seekg(0, std::ios_base::end);
	size_t fsize = (size_t)fp.tellg();
	fp.seekg(0, std::ios_base::beg);
	fp.read((char*)&header, header_len);

	res = puser->PDSetupHeader(header, header_len, &contentLength, &contentOffset);
	if (!res)
	{
		std::cout << "PDSetupHeader failed: " << res.GetMsg() << std::endl;
		return;
	}

	fp.seekg(contentOffset, std::ios_base::beg);

	long bytesToRead = in_len;

	while (contentLength > 0)
	{
		memset(in, 0, sizeof(in));
		memset(out, 0, sizeof(out));
		fp.read((char*)in, bytesToRead);
		if (!fp.good())
		{
			std::cout << "Warning: Error reading file" << std::endl;
		}
		long count = (long)fp.gcount();
		if (count < bytesToRead)
			bytesToRead = count;
		out_len = sizeof out;

		res = puser->PDDecryptPartial(in, bytesToRead, offset, out, &out_len, header, header_len);
		if (!res)
		{
			std::cout << "PDDecryptPartial failed: " << res.GetMsg() << std::endl;
			return;
		}

		ofs.write((char *)out, out_len);
		offset += out_len;
		contentLength -= out_len;
	}

	fp.close();
	ofs.close();
}

typedef struct {
	ISDRmUser *pUser;
	const char *nxlFileStr;
	bool isAdHoc;
	HANDLE startDecryptEvent;
} PartialDecryptParams_t;

DWORD WINAPI Test_PartialDecryptExThreadProc(LPVOID pParams)
{
	const PartialDecryptParams_t* pPDParams = (PartialDecryptParams_t *) pParams;
	DWORD ret;

	if (!pPDParams->pUser)
		return -1;

	std::cout << "Waiting to start decrypting, TID=" << GetCurrentThreadId() << std::endl;
	ret = WaitForSingleObject(pPDParams->startDecryptEvent, INFINITE);
	if (ret != WAIT_OBJECT_0)
	{
		std::cout << "WaitForSingleObject failed, ret=" << ret << std::endl;
	}
	std::cout << "Start decrypting " << pPDParams->nxlFileStr << ", TID=" << GetCurrentThreadId() << std::endl;

	SDWLResult res;
	const std::string nxlFile(pPDParams->nxlFileStr);
	char decryptedFileStr[MAX_PATH];
	std::string decryptedFile;
	std::fstream fp;
	long header_len = pPDParams->isAdHoc ? 4096*3 : 4096*4;
	unsigned char header[4096*4];
	const long in_len = 512;
	unsigned char in[in_len];
	long offset = 0;
	unsigned char out[2048];
	long out_len;
	unsigned int contentOffset;
	int64_t contentLength;
	void *context;

	sprintf_s(decryptedFileStr, "%s.decrypt.%lu", pPDParams->nxlFileStr, GetCurrentThreadId());
	decryptedFile = decryptedFileStr;

	fp.open(nxlFile, ios_base::in | ios_base::binary);
	if (!fp.is_open())
	{
		std::cout << "Error opening file" << std::endl;
		return -2;
	}
	std::ofstream ofs;
	ofs.open(decryptedFile, ios_base::trunc | ios_base::binary | ios_base::out);
	if (!ofs.is_open())
	{
		std::cout << "Error opening output file" << std::endl;
		return -3;
	}

	memset(header, 0, header_len);

	fp.seekg(0, std::ios_base::end);
	size_t fsize = (size_t)fp.tellg();
	fp.seekg(0, std::ios_base::beg);
	fp.read((char*)&header, header_len);

	res = pPDParams->pUser->PDSetupHeaderEx(header, header_len, &contentLength, &contentOffset, context);
	if (!res)
	{
		std::cout << "PDSetupHeader failed: " << res.GetMsg() << std::endl;
		return -4;
	}

	fp.seekg(contentOffset, std::ios_base::beg);

	long bytesToRead = in_len;

	while (contentLength > 0)
	{
		memset(in, 0, sizeof(in));
		memset(out, 0, sizeof(out));
		fp.read((char*)in, bytesToRead);
		if (!fp.good())
		{
			std::cout << "Warning: Error reading file" << std::endl;
		}
		long count = (long)fp.gcount();
		if (count < bytesToRead)
			bytesToRead = count;
		out_len = sizeof out;

		res = pPDParams->pUser->PDDecryptPartialEx(in, bytesToRead, offset, out, &out_len, header, header_len, context);
		if (!res)
		{
			std::cout << "PDDecryptPartial failed: " << res.GetMsg() << std::endl;
			pPDParams->pUser->PDTearDownHeaderEx(context);
			return -5;
		}

		ofs.write((char *)out, out_len);
		offset += out_len;
		contentLength -= out_len;
	}

	pPDParams->pUser->PDTearDownHeaderEx(context);
	fp.close();
	ofs.close();

	std::cout << "Done decrypting " << pPDParams->nxlFileStr << ", TID=" << GetCurrentThreadId() << std::endl;
	return 0;
}

void Test_PartialDecryptEx(ISDRmUser *puser)
{
	const struct {const char *nxlFileStr; bool isAdHoc;} testFiles[] = {
		{"C:\\test_Debug\\100MB-2019-11-26-23-41-48.doc.nxl", false},
		{"C:\\test_Debug\\w3-2019-11-26-23-49-02.txt.nxl", false},
		{"C:\\test_Debug\\CAD files-2019-11-26-23-55-41.zip.nxl", false},
		{"C:\\test\\Safe Dir 2\\1KB-2019-07-18-23-26-37.txt.nxl", false},
		{"C:\\test\\Safe Dir 2\\foo-2019-10-23-09-51-27.txt.nxl", false}
	};

	const int numFiles = _countof(testFiles);
	HANDLE startDecryptEvent;
	PartialDecryptParams_t params[numFiles];
	HANDLE h[numFiles];
	DWORD ret;

	startDecryptEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	for (int i = 0; i < numFiles; i++) {
		params[i].pUser = puser;
		params[i].nxlFileStr = testFiles[i].nxlFileStr;
		params[i].isAdHoc = testFiles[i].isAdHoc;
		params[i].startDecryptEvent = startDecryptEvent;
		h[i] = CreateThread(NULL, 0, &Test_PartialDecryptExThreadProc,
							&params[i], 0, NULL);
		if (h[i] == NULL) {
			wcout << L"CreateThread failed, lastErr=" << GetLastError() << endl;
			return;
		}
	}

	// Wait for all the threads to reach the point of waiting for the
	// start-decrypt event.
	Sleep(1000);
	std::cout << "Ready .... ";
	Sleep(1000);
	std::cout << "Set .... ";
	Sleep(1000);
	std::cout << "Go!" << std::endl;
	SetEvent(startDecryptEvent);

	ret = WaitForMultipleObjects(numFiles, h, TRUE, INFINITE);
	if (ret >= WAIT_OBJECT_0 + numFiles) {
		std::cout << "WaitForMultipleObjects failed, ret=" << ret << std::endl;
		return;
	}

	std::cout << "All threads done" << std::endl;
	for (int i = 0; i < numFiles; i++) {
		CloseHandle(h[i]);
	}

	CloseHandle(startDecryptEvent);
}
