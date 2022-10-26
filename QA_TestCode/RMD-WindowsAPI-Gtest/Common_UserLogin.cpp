#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <curl/curl.h>  
#include "string"
#include "stdafx.h"
#include "SDLInc.h"

using namespace std;

void GetUserLoginResponseData(const string email, const string password, const string clientID, const string userReturnFilePath)
{
	CURL *curl;
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	//struct curl_slist *http_header = NULL;
	const char* fileName = userReturnFilePath.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}

	string postURL;
	const string cookie = "Cookie: platformId=0; clientId=" + clientID;
	const string postFields = "email=" + email + "&password=" + password;

	curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, POSTURL);

	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
	headerlist = curl_slist_append(headerlist, cookie.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);
	string cookieFile = userReturnFilePath + "curlpost.cookie";
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookieFile.c_str());

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);	//SKIP_PEER_VERIFICATION

	res = curl_easy_perform(curl);

	fclose(fptr);
	curl_easy_cleanup(curl);
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
	FILE *fptr = (FILE*)userp;
	fwrite(buffer, size, nmemb, fptr);
	
	return size * nmemb;
}

SDWLResult CreateSDRmcInstance_CreateClientID(const WCHAR *testfolder, ISDRmcInstance *&pInstance, string &clientId, const wstring router_ws, const wstring tenant_ws, const WCHAR *sdklibfolder)
{
	ISDRmHttpRequest *prequest = NULL;
	SDWLResult res;
	
	createFolder(ws2s(testfolder));

	LOG(INFO) << "SDWLibCreateSDRmcInstance: ";
	wstring f = testfolder;
	LOG(INFO) << "(tempfolder: " << ws2s(f) << ")";
	res = SDWLibCreateSDRmcInstance("", 1, 0, 0, sdklibfolder, testfolder, &pInstance);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	LOG(INFO) << "(route: " << ws2s(router_ws) << ")"<<endl;
	string workingfoler = ws2s(f) + "\\working";
	createFolder(workingfoler);
	LOG(INFO) << "Workingfolder: " << workingfoler << endl;
	LOG(INFO) << "Initialize: ";
	res = pInstance->Initialize(s2ws(workingfoler), router_ws, tenant_ws);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

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
				clientId = ws2s(cookie.second);
				string exportfilename = plan_path + "client_id.txt";
				ofstream SaveFile(exportfilename, ios::ate);
				SaveFile << clientId;// << endl;
				SaveFile.close();
			}
		}
		LOG(INFO) << "---------------------" << endl;
	}

	return res;
}

void GetUserLoginResponseData_InputServer(const string email, const string password, const string clientID, const string userReturnFilePath, const string RMSname)
{
#define POSTURL_7303 "https://rms-centos7303.qapf1.qalab01.nextlabs.com:8444/rms/rs/usr" 
#define POSTURL_7301 "https://autorms-centos7301.qapf1.qalab01.nextlabs.com:8444/rms/rs/usr"
#define POSTURL_RMtest "https://rmtest.nextlabs.solutions/rms/rs/usr"

	CURL *curl;
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	//struct curl_slist *http_header = NULL;
	const char* fileName = userReturnFilePath.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}

	string postURL;
	const string cookie = "Cookie: platformId=0; clientId=" + clientID;
	const string postFields = "email=" + email + "&password=" + password;

	curl = curl_easy_init();
	if (RMSname == "rms-centos7303")
	{
		curl_easy_setopt(curl, CURLOPT_URL, POSTURL_7303);
	}
	else
	{
		curl_easy_setopt(curl, CURLOPT_URL, POSTURL_RMtest);
	}

	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
	headerlist = curl_slist_append(headerlist, cookie.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);
	string cookieFile = userReturnFilePath + "curlpost.cookie";
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookieFile.c_str());

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);	//SKIP_PEER_VERIFICATION

	res = curl_easy_perform(curl);

	fclose(fptr);
	curl_easy_cleanup(curl);
}

string getUserIDFromJson(const string JosnFilePath)
{
	Json::Value jroot;
	jroot = ParseJsonFile(JosnFilePath);
	string uid = jroot["extra"]["userId"].asString();
	return uid;
}

SDWLResult userFirstTimeLogin(const string email, const string password, const string testFolder, wstring &tenantName, string &passcode)
{
	ISDRmcInstance * pInstance;
	ISDRmUser *puser = NULL;
	SDWLResult res;
	ISDRmTenant *pTenant;

	string clientId = "";
	string user_return = testFolder + "\\User_" + email + "_RMSreturn.json";
	createFolder(testFolder);

	res = CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientId, DEFUALT_ROUTER, DEFUALT_TENANT, DEFUALT_SDKLib_FOLDER);
	GetUserLoginResponseData(email, password, clientId, user_return);

	cout << "SetLoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(user_return), &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res)
	{
		passcode = puser->GetPasscode();
		cout << "get passcode: " << passcode << endl;

		res = pInstance->GetCurrentTenant(&pTenant);
		cout << "GetCurrentTenant(&pTenant):(after user login) ";
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());
		if (res.GetCode() == 0)
		{
			tenantName = pTenant->GetTenant();
			wcout << L"Tenant:" << tenantName << endl;
		}
	}

	cout << "Save: ";
	res = pInstance->Save();
	checkSDWLResult(res);

	SDWLibDeleteRmcInstance(pInstance);

	return res;
}