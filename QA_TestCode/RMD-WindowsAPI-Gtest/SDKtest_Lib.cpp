#include "stdafx.h"
#include "SDLInc.h"


class LibInterface :public::testing::TestWithParam<vector<string>> {};
TEST_P(LibInterface, ValidInput)
{
	vector<string> csv = GetParam();
	string IsRun = csv[0];
	if (IsRun == "run" || IsRun == "1")
	{
		string caseID = csv[1];
		string RmcAPIWorkingPath = csv[2];
		wstring tempPath = s2ws(RmcAPIWorkingPath);
		string clientid = csv[3];
		//char* clientid = "75A8F04A172AB2603EFF57F51DAF2A93";

		cout << "caseID: " << caseID << endl;
		DWORD dwVersion;
		dwVersion = SDWLibGetVersion();

		//? How to check the Expected result
		cout << "dwVersion: " << dwVersion << endl;
		char * v = int_to_hex(dwVersion);
		cout << "Version: " << v << endl;


		ISDRmcInstance * pInstance = NULL;
		ISDRmTenant *pTenant;
		SDWLResult res;

		//? What's this working folder use for?
		wcout << L"tempPath.c_str(): \"" << tempPath.c_str() << "\"" << endl;

		cout << "SDWLibCreateSDRmcInstance: ";
		if (clientid == "null")
		{
			res = SDWLibCreateSDRmcInstance("Product name", 1, 1, 1, (WCHAR*)DEFUALT_LIB_FOLDER.c_str(), tempPath.c_str(), &pInstance);
		}
		else
		{
			if (clientid == "empty") { clientid = ""; }

			res = SDWLibCreateSDRmcInstance("Product name", 1, 1, 1, (WCHAR*)DEFUALT_LIB_FOLDER.c_str(), tempPath.c_str(), &pInstance, (char*)clientid.c_str());
			//? Check the clientID
		}
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (pInstance == NULL) { cout << "pInstance: NULL " << endl; }
		else {
			cout << "pInstance: " << pInstance << endl;

			//check if the working path set in "SDWLibCreateSDRmcInstance" is ok
			pInstance->Save(); //the data files will be saved at temporary folder set at SDWLibCreateSDRmcInstance()
			bool fExist = checkFileExist(RmcAPIWorkingPath, "Instance.history");
			EXPECT_TRUE(fExist);
			if (fExist)
				deleteFile(RmcAPIWorkingPath, "Instance.history");
		}

		//? How to check if it deleted
		cout << "SDWLibDeleteRmcInstance " << endl;
		SDWLibDeleteRmcInstance(pInstance);
		cout << "pInstance: " << pInstance << endl;


		cout << "-------------This case finished--------" << endl;
	}
}
INSTANTIATE_TEST_CASE_P(LibTesting, LibInterface, testing::ValuesIn(csv_lib));

//Check APP version
TEST(CheckAPPVersion, CheckVersion) {
	try {
		SDWLResult res;
		ISDRmcInstance *pInstance = NULL;
		ISDRmcInstance *pInstance_n = NULL;
		std::string newVersionStr;
		std::string downloadURL;
		std::string checksum;
		std::string newVersionStr_n;
		std::string downloadURL_n;
		std::string checksum_n;
	std:string path = "C:\\work1";
		string t = "md \"" + path + "\"";
		string r = "rd  /s/q " + path;


		//Get new version
		system(t.c_str());
		res = SDWLibCreateSDRmcInstance("RMC API Test App", 1, 0, 1621, (WCHAR*)DEFUALT_LIB_FOLDER.c_str(), L"C:\\work1", &pInstance);
		res = pInstance->Initialize(L"C:\\work1", DEFUALT_ROUTER, DEFUALT_TENANT);

		res = pInstance->CheckSoftwareUpdate(newVersionStr, downloadURL, checksum);
		cout << "The return code is: " << res.GetCode() << endl;
		cout << "New version number:  " << newVersionStr << endl;
		cout << "Download URL is: " << downloadURL << endl;
		cout << "Checksum is: " << checksum << endl;
		if (DEFUALT_ROUTER == L"https://rmtest.nextlabs.solutions") {
			EXPECT_STREQ(newVersionStr.c_str(), "1.0.1721");
			EXPECT_STREQ(downloadURL.c_str(), "https://s3.amazonaws.com/nextlabs-rc/SkyDRM/Latest+Version/SkyDRM-AutoUpdate32-1.0.1721.CAB");
			EXPECT_STREQ(checksum.c_str(), "822308D83F1AC442AD5B0526CE7B5BCE33B2EBA3");
		}

		SDWLibDeleteRmcInstance(pInstance);
		system(r.c_str());

		//When it is latest version	
		system(t.c_str());
		res = SDWLibCreateSDRmcInstance("", 1, 0, 1721, (WCHAR*)DEFUALT_LIB_FOLDER.c_str(), L"C:\\work1", &pInstance_n);
		res = pInstance_n->Initialize(L"C:\\work1", DEFUALT_ROUTER, DEFUALT_TENANT);

		res = pInstance_n->CheckSoftwareUpdate(newVersionStr_n, downloadURL_n, checksum_n);
		system(r.c_str());
		if (DEFUALT_ROUTER == L"https://rmtest.nextlabs.solutions") {
			EXPECT_STREQ(res.GetMsg().c_str(), "Not Modified");
		}

		SDWLibDeleteRmcInstance(pInstance_n);
		cout << "Test finished!" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(InstanceTest, manyInstance)
{
	ISDRmcInstance * pInstance1 = NULL;
	ISDRmcInstance * pInstance2 = NULL;

	SDWLResult res;

	cout << "SDWLibCreateSDRmcInstance: ";
	res = SDWLibCreateSDRmcInstance("", 1, 0, 0, DEFUALT_SDKLib_FOLDER, DEFUALT_TEMP_FOLDER.c_str(), &pInstance1);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	res = SDWLibCreateSDRmcInstance("", 1, 0, 0, DEFUALT_SDKLib_FOLDER, DEFUALT_TEMP_FOLDER.c_str(), &pInstance2);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	
	cout << "SDWLibDeleteRmcInstance " << endl;
	SDWLibDeleteRmcInstance(pInstance1);
	SDWLibDeleteRmcInstance(pInstance2);
}

TEST(ChangeRouter, ChangeRouter) {
	try {
		SDWLResult res;

		ISDRmcInstance *pInstance_r = NULL;
		ISDRmUser *puser_r = NULL;
		ISDRmTenant *pTenant_r = NULL;
		ISDRmHttpRequest *prequest_r = NULL;

		res = SDWLibCreateSDRmcInstance("RMC API Test App", 1, 0, 1721, (WCHAR*)DEFUALT_LIB_FOLDER.c_str(), L"C:\\work", &pInstance_r);
		res = pInstance_r->Initialize(L"C:\\work", DEFUALT_ROUTER, DEFUALT_TENANT);
		cout << "GetLoginRequest: ";
		res = pInstance_r->GetLoginRequest(&prequest_r);
		wstring path = prequest_r->GetPath();
		wstring url_r = prequest_r->GetDomainUrl();

		SDWLibDeleteRmcInstance(pInstance_r);

		ISDRmcInstance *pInstance_m = NULL;
		ISDRmUser *puser_m = NULL;
		ISDRmTenant *pTenant_m = NULL;
		ISDRmHttpRequest *prequest_m = NULL;

		res = SDWLibCreateSDRmcInstance("RMC API Test App", 1, 0, 1721, (WCHAR*)DEFUALT_LIB_FOLDER.c_str(), L"C:\\work", &pInstance_m);
		//res = pInstance_m->Initialize(L"C:\\work", L"https://rms-centos7303.qapf1.qalab01.nextlabs.com:8443", L"ee4eddd8-81fc-4488-a1a7-2a71505c4411");
		res = pInstance_m->Initialize(L"C:\\work", L"https://www.baidu.com/", L"");
		EXPECT_STREQ(res.GetMsg().c_str(), "Invalid Tenant query return from Server");
		cout << "GetLoginRequest: ";
		res = pInstance_m->GetLoginRequest(&prequest_m);
		wstring loginurl_m = prequest_m->GetPath();
		wstring url_m = prequest_m->GetDomainUrl();

		SDWLibDeleteRmcInstance(pInstance_m);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(RegisterAPPTest, RegisterTest) {
	try {
		SDWLResult res;

		ISDRmcInstance *pInstance = NULL;
		ISDRmUser *puser = NULL;
		ISDRmTenant *pTenant = NULL;

		ISDRmHttpRequest *prequest = NULL;

		createFolder(ws2s(L"C:\\work"));

		cout << "SDWLibCreateSDRmcInstance: ";
		wstring f = L"C:\\work";
		wcout << "(tempfolder: " << f << ")";
		res = SDWLibCreateSDRmcInstance("Product name", 1, 0, 1621, L"C:\\Program Files\\NextLabs", L"C:\\work", &pInstance);

		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		string workingfoler = ws2s(f) + "\\working";
		createFolder(workingfoler);
		cout << "Workingfolder: " << workingfoler << endl;
		cout << "Initialize: ";
		res = pInstance->Initialize(s2ws(workingfoler), DEFUALT_ROUTER, DEFUALT_TENANT);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		////Trust EXCEL
		//res = pInstance->RPMRegisterApp(L"C:\\Program Files (x86)\\Microsoft Office\\Office15\\EXCEL.EXE", "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}");
		//res = pInstance->RPMNotifyRMXStatus(false, "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}");
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}