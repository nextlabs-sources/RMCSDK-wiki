#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <curl/curl.h>  
#include "string"
#include "stdafx.h"
#include "SDLInc.h"

string user_id;
// reply of the requery  
size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
{
	cout << "----->reply" << endl;
	string *str = (string*)stream;
	cout << *str << endl;
	(*str).append((char*)ptr, size*nmemb);
	return size * nmemb;
}

//POST

//GET

//PUT

bool GetTenantResponseData(const wstring router)
{
	// init curl  
	CURL *curl = curl_easy_init();
	// res code  
	CURLcode res;
	FILE *fptr;
	string router_s = ws2s(router);
	string node = router_s + "/router/rs/q/defaultTenant/";
	const char* fileName = tenant_respond.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	if (curl)
	{
		// set params  
		curl_easy_setopt(curl, CURLOPT_URL, node.c_str()); // url  
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
															   //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time  
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
		// start req  
		res = curl_easy_perform(curl);
	}
	// release curl  
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else
	{
		return true;
	}
}

bool GetMyDriveInfoResponseData(uint64_t &result_code, uint64_t &usage, uint64_t &totalquota, uint64_t &vaultusage, uint64_t &vaultquota) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();
	string url = server + "/rs/myDrive/getUsage";
	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string jsonout = "{\"parameters\":{\"userId\":" + user_id + ",\"ticket\":\"" + ticket + "\"}}";
	const char* fileName = mydriveInfo_respond.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	//Set body
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		jroot = ParseJsonFile(mydriveInfo_respond);
		result_code = jroot["statusCode"].asInt64();
		if (result_code == 200) {
			usage = jroot["results"]["usage"].asInt64();
			totalquota = jroot["results"]["quota"].asInt64();
			vaultquota = jroot["results"]["vaultQuota"].asInt64();
			vaultusage = jroot["results"]["myVaultUsage"].asInt64();
		}
		return true;
	}
}

bool GetUpdateUserInfo(uint64_t &result_code, string &display_name)
{
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();
	string url = server + "/rs/usr/profile";
	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string rand_s = to_string(rand());
	display_name = "zhuzhu" + rand_s;
	string jsonout = "{\"parameters\":{\"displayName\":\"" + display_name + "\"}}";

	const char* fileName = updatinfo_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	//Set body
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

bool GetFileInfoResponseData(string file_name)
{
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string path_id;
	string duid;

	bool list_result = GetMyVaultFileListResponseData("1", "10", file_name, "-creationTime", "allFiles");
	Sleep(2000);
	if (list_result) {
		jroot = ParseJsonFile(myvaultfilelist_return);
		path_id = jroot["results"]["detail"]["files"][0]["pathId"].asString();
		duid = jroot["results"]["detail"]["files"][0]["duid"].asString();
	}
	string url = server + "/rs/myVault/" + duid + "/metadata";
	string jsonout = "{\"parameters\":{\"pathId\":\"" + path_id + "\"}}";
	const char* fileName = fileinfo_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	//Set body
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

//order_by: fileName, creationTime, size     filter: activeTransaction, allShared, allFiles, protected,deleted,revoked
bool GetMyVaultFileListResponseData(string page, string size, string file_name, string order_by, string filter) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();
	string url = server + "/rs/myVault?page=" + page + "&size=" + size + "&orderBy=" + order_by + "&q.fileName=" + file_name + "&filter=" + filter;
	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	const char* fileName = myvaultfilelist_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}

	if (curl)
	{
		const string header_client = "clientId:" + client_id;
		const string header_userid = "userId:" + user_id;
		const string header_ticket = "ticket:" + ticket;
		// set params  
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
		headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
		headerlist = curl_slist_append(headerlist, "Accept: application/json");
		headerlist = curl_slist_append(headerlist, "platformId:0");
		headerlist = curl_slist_append(headerlist, header_client.c_str());
		headerlist = curl_slist_append(headerlist, header_userid.c_str());
		headerlist = curl_slist_append(headerlist, header_ticket.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);
		// start req  
		res = curl_easy_perform(curl);
	}
	// release curl  
	fclose(fptr);
	curl_easy_cleanup(curl);

	if (res) {
		return false;
	}
	else {
		return true;
	}

}

bool GetUpdateRecipientsResponseData(string file_name, vector<std::string> addrecipients, vector<std::string> removerecipients) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string path_id;
	string duid;

	bool list_result = GetMyVaultFileListResponseData("1", "10", file_name, "-creationTime", "allFiles");
	Sleep(2000);
	if (list_result) {
		jroot = ParseJsonFile(myvaultfilelist_return);
		path_id = jroot["results"]["detail"]["files"][0]["pathId"].asString();
		duid = jroot["results"]["detail"]["files"][0]["duid"].asString();
	}
	string url = server + "/rs/share/" + duid + "/update";
	string jsonout = "{\"parameters\":{\"newRecipients\":[";
	if (addrecipients.size() == 0) {
		jsonout = jsonout + "],\"removedRecipients\":[";
	}
	else {
		for (int i = 0; i < addrecipients.size(); i++) {
			if (i != 0) {
				jsonout = jsonout + ",";
			}
			jsonout = jsonout + "{\"email\":\"" + addrecipients[i].c_str() + "\"}";
		}
		jsonout = jsonout + "],\"removedRecipients\":[";
	}

	if (removerecipients.size() == 0) {
		jsonout = jsonout + "]}}";
	}
	else {
		for (int i = 0; i < removerecipients.size(); i++) {
			if (i != 0) {
				jsonout = jsonout + ",";
			}
			jsonout = jsonout + "{\"email\":\"" + removerecipients[i].c_str() + "\"}";
		}
		jsonout = jsonout + "]}}";
	}

	const char* fileName = recipients_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	//Set body
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

bool GetActivityLogResponseData(string file_name, string start, string count, string orderBy, string orderByReverse) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string path_id;
	string duid;

	bool list_result = GetMyVaultFileListResponseData("1", "10", file_name, "-creationTime", "allFiles");
	Sleep(2000);
	if (list_result) {
		jroot = ParseJsonFile(myvaultfilelist_return);
		path_id = jroot["results"]["detail"]["files"][0]["pathId"].asString();
		duid = jroot["results"]["detail"]["files"][0]["duid"].asString();
	}
	string url = server + "/rs/log/v2/activity/" + duid + "?start=" + start + "&count=" + count + "&orderBy=" + orderBy + "&orderByReverse=" + orderByReverse;

	const char* fileName = activity_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

bool GetFileTokenResponseData(string file_name) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string path_id;
	string duid;
	string owner;
	string tenant = jroot["extra"]["defaultTenant"].asString();
	for (int i = 0; i < jroot["extra"]["memberships"].size(); i++) {
		if (jroot["extra"]["memberships"][i]["projectId"].asInt() == 1) {
			owner = jroot["extra"]["memberships"][i]["id"].asString();
		}
	}

	bool list_result = GetMyVaultFileListResponseData("1", "10", file_name, "-creationTime", "allFiles");
	Sleep(2000);
	if (list_result) {
		jroot = ParseJsonFile(myvaultfilelist_return);
		path_id = jroot["results"]["detail"]["files"][0]["pathId"].asString();
		duid = jroot["results"]["detail"]["files"][0]["duid"].asString();
	}
	string url = server + "/rs/token";
	string jsonout = "{\"parameters\":{\"userId\":\"" + user_id + "\",\"ticket\":\"" + ticket + "\",\"tenant\":\"" + tenant + "\",\"owner\":\"" + owner + "\",\"agreement\":\"3CCD5670E37F2E1E3FE4A0C1F27127B4BB10255C603D37A290C8E0EFD8D438DA4E32FF46859EDBB52C1E219BF7B3BF713A24B17BA0A571CA7D7AD09BD90365B7A96DA8B97481DC83905CABC2C463E664E87100E6D26B3CF674396E573B2825B85B46629078E89E34B1E570DEB89FF14A5D96939CC2B2BBD013923259AEBD70FB774A9F4F77FF68E6535A4799E45F99C858F5A83A00F684E6570EDE5C9A459F03E1F2A5AA35BE268DFB5CCD68E476BB486115DE6BD35B67CA64280BCE26E1BC5596479A71385C034E91A8A0CDB478CD0DC322A46B341CC47A51FA0C253E1BA19AABDCA5BB8BA986BD8E252CDA81025C6FBE24BC03FEFB6796B45D1493984621A5\",\"duid\":\"" + duid + "\",\"ml\":0}}";

	const char* fileName = filetoken_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	//Set body
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

bool GetHeartBeatResponseData() {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();

	string url = server + "/rs/v2/heartbeat";
	string jsonout = "{\"parameters\":{\"platformId\":0,\"clientData\":[]}}";

	const char* fileName = hearbeat_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	//Set body
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}

}

//Get method /rms/rs/project?
bool GetProjectListResponseData(string page, string size, string orderby, string owedbyme) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string path_id;
	string duid;

	string url = server + "/rs/project?page=" + page + "&size=" + size + "&orderBy=" + orderby;
	if ((owedbyme == "true") || (owedbyme == "false"))
	{
		url = url + "&ownedByMe=" + owedbyme;
	}

	const char* fileName = projectlist_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}

}

bool GetProjectFilelistResponseData(string proid, string page, string size, string orderby, string pathid, string searchstring) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string path_id;
	string duid;

	string url = server + "/rs/project/" + proid + "/files?page" + page + "&size=" + size + "&orderBy=" + orderby + "&pathId=" + pathid;
	if (searchstring != "") {
		url = url + "&q=" + searchstring;
	}

	const char* fileName = projectfilelist_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

bool GetProjectFileMetadataResponseData(string projectid, string filepathid) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string tenant = jroot["extra"]["defaultTenant"].asString();

	string url = server + "/rs/project/" + projectid + "/file/metadata";
	string jsonout = "{\"parameters\":{\"pathId\":\"" + filepathid + "\"}}";

	const char* fileName = projectfilemetadatalist_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	//Set body
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

//orderby: name, size, sharedBy, sharedDate
bool GetShareWithMeListResponseData(string page, string size, string orderby) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string path_id;
	string duid;

	string url = server + "/rs/sharedWithMe/list?page=" + page + "&size=" + size + "&orderBy=" + orderby;
	const char* fileName = shrewithmefilelist_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}

}

//Get myvault file medadata
bool GetMyVaultFileMetadataResponseData(string file_name) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();
	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string tenant = jroot["extra"]["defaultTenant"].asString();

	bool vault_list = GetMyVaultFileListResponseData("1", "10", file_name, "fileName", "allFiles");
	jroot = ParseJsonFile(myvaultfilelist_return);
	string pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
	string duid = jroot["results"]["detail"]["files"][0]["duid"].asString();

	string url = server + "/rs/myVault/" + duid + "/metadata";
	string jsonout = "{\"parameters\":{\"pathId\":\"" + pathid + "\"}}";

	const char* fileName = filemetadatalist_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	//Set body
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}

}

//CreateProject via RMS API    --Lifi.yuan 2018/11/22
bool Project_Create(string user_jsonfile, string client_id, string rmsurl, string projectName, std::vector<std::string> members, string outputProject)
{
	CURL *curl;
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	Json::Value jroot;

	jroot = ParseJsonFile(user_jsonfile);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();

	string url = rmsurl + "/rs/project";
	const char* fileName = outputProject.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;

	string bodyFields = "{\"parameters\":{\"projectName\":\""
		+ projectName + "\",\"projectDescription\" : \"This is default project Description\",\"invitationMsg\" : \"This is the default message\",\"emails\" : [";
	int i = 0;
	for each(string m in members)
	{
		if (i)
			bodyFields += ",\"" + m + "\"";
		else
			bodyFields += "\"" + m + "\"";

		i++;
	}
	bodyFields.append("]}}");

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); //SKIP_PEER_VERIFICATION  
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	//headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyFields.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}

}

//List Pending Invitations For a User
/* The invitationId and code in the response can be used to either accept or decline the invitation
* Lifi.yuan 2018/11/22
*/
bool Project_UserPendingInvitations(string user_jsonfile, string client_id, string rmsurl, string outputJsonPath)
{
	CURL *curl;
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	Json::Value jroot;

	const char* fileName = outputJsonPath.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}

	string url = rmsurl + "/rs/project/user/invitation/pending";

	jroot = ParseJsonFile(user_jsonfile);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 0);   // 1:POST, 0:Get
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); //SKIP_PEER_VERIFICATION  
	//headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	//headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

//Accept Project Invitation --Lifi.yuan 2018/11/22
bool Project_AcceptInvitation(string user_jsonfile, string client_id, string rmsurl, string PendingInvitations_json, int projectID)
{
	CURL *curl;
	CURLcode res;

	struct curl_slist *headerlist = NULL;
	Json::Value jroot;

	jroot = ParseJsonFile(user_jsonfile);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;

	jroot = ParseJsonFile(PendingInvitations_json);
	string invitation_id;
	string code;
	if (jroot["results"]["pendingInvitations"].size() == 0) {
		cout << "No pendingInvitations for this user!" << endl;
		return false;
	}
	else {
		for (int i = 0; i < jroot["results"]["pendingInvitations"].size(); i++) {
			if (jroot["results"]["pendingInvitations"][i]["project"]["id"].asInt() == projectID) {
				invitation_id = jroot["results"]["pendingInvitations"][i]["invitationId"].asString();
				code = jroot["results"]["pendingInvitations"][i]["code"].asString();
				break;
			}
		}
	}
	string url = rmsurl + "/rs/project/accept?id=" + invitation_id + "&code=" + code;


	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 0);   // 1:POST, 0:Get
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); //SKIP_PEER_VERIFICATION  
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	//headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

	res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

//Acept Project Invitation --Jewelry.zhu 2019/05/07
bool Project_AcceptInvitation1(int pro_id) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();
	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string tenant = jroot["extra"]["defaultTenant"].asString();

	Project_UserPendingInvitations(user_respond, client_id, server, userPendingInvitations_return);
	jroot = ParseJsonFile(userPendingInvitations_return);
	string invitation_id;
	string code;
	if (jroot["results"]["pendingInvitations"].size() == 0) {
		cout << "No pendingInvitations for this user!" << endl;
		return false;
	}
	else {
		for (int i = 0; i < jroot["results"]["pendingInvitations"].size(); i++) {
			if (jroot["results"]["pendingInvitations"][i]["project"]["id"].asInt() == pro_id) {
				invitation_id = jroot["results"]["pendingInvitations"][i]["invitationId"].asString();
				code = jroot["results"]["pendingInvitations"][i]["code"].asString();
				break;
			}
		}
	}
	string url = server + "/rs/project/accept?id=" + invitation_id + "&code=" + code;

	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 0);   // 1:POST, 0:Get
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); //SKIP_PEER_VERIFICATION  
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	//headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

	res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

bool Project_MemberListResponseData(string pro_id) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string tenant = jroot["extra"]["defaultTenant"].asString();

	string url = server + "/rs/project/" + pro_id + "/members?page=1&size=10&orderBy=displayName";

	const char* fileName = projectmemberlist_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}

}

bool Project_InviteMemberResponseData(string pro_id, string invite_email) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string tenant = jroot["extra"]["defaultTenant"].asString();

	string url = server + "/rs/project/" + pro_id + "/invite";
	string jsonout = "{\"parameters\":{\"emails\":[\"" + invite_email + "\"],\"invitationMsg\":\"2222\"}}";

	const char* fileName = projectinvitemember_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}


//DELETE
bool InvokeSharedFileResponseData(string duid) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string tenant = jroot["extra"]["defaultTenant"].asString();

	string url = server + "/rs/share/" + duid + "/revoke";

	const char* fileName = invokefile_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

bool DeleteMyvaultFile(string duid, string pathid) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	string client_id = ReadFromFile(clientfilename);
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();

	jroot = ParseJsonFile(user_respond);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	string tenant = jroot["extra"]["defaultTenant"].asString();

	string url = server + "/rs/myVault/" + duid + "/delete";
	string jsonout = "{\"parameters\":{\"pathId\":\"" + pathid + "\"}}";

	const char* fileName = deletefile_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

bool AdminLoginResponseData(string userNanme, string password, string id, string clientid) {
	//Post /IdpManager/LdapAuth/LdapAuthFinish
	CURL *curl;
	CURLcode res;
	struct curl_slist *headerlist_auth = NULL;
	string header_client = "clientId:" + clientid;
	string  cookie_str1 = "platformId=0; clientId=" + clientid;
	const string postFields = "userName=" + userNanme + "&password=" + password + "&id=" + id + "&tenant=centos7513test_test07";
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();
	string url = server + "/IdpManager/LdapAuth/LdapAuthFinish";
	cout << "url is:" + url << endl;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
	headerlist_auth = curl_slist_append(headerlist_auth, "platformId:0");
	headerlist_auth = curl_slist_append(headerlist_auth, header_client.c_str());
	curl_easy_setopt(curl, CURLOPT_COOKIE, cookie_str1.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist_auth);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, adminloginCookie.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);	//SKIP_PEER_VERIFICATION

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	// init curl  
	CURL *curl_profile = curl_easy_init();
	FILE *fptr;
	struct curl_slist *headerlist = NULL;

	string url_profile = server + "/rs/usr/v2/profile";

	const char* fileName = user_respond.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	string cookie_str = ReadFromFile(adminloginCookie);

	////7511
	//const string llId = "ltId:" + cookie_str.substr(207, 36);
	//const string clientid_str = "clientId:" + cookie_str.substr(319, 32);
	//const string ticket = "ticket:" + cookie_str.substr(425, 32);
	//const string idp = "idp:" + cookie_str.substr(528, 1);
	//const string tenantId = "tenantId:" + cookie_str.substr(605, 36);
	//const string lt = "lt:" + cookie_str.substr(711, 6);
	//const string platformid = "platformId:" + cookie_str.substr(795, 1);
	//const string userid = "userId:" + cookie_str.substr(870, 1);

	////7512
	//const string llId = "ltId:" + cookie_str.substr(207, 36);
	//const string clientid_str = "clientId:" + cookie_str.substr(319, 32);
	//const string ticket = "ticket:" + cookie_str.substr(425, 32);
	//const string idp = "idp:" + cookie_str.substr(528, 1);
	//const string tenantId = "tenantId:" + cookie_str.substr(605, 36);
	//const string lt = "lt:" + cookie_str.substr(711, 9);
	//const string platformid = "platformId:" + cookie_str.substr(798, 1);
	//const string userid = "userId:" + cookie_str.substr(873, 1);

	////7513
	//const string llId = "ltId:" + cookie_str.substr(207, 36);
	//const string clientid_str = "clientId:" + cookie_str.substr(319, 32);
	//const string ticket = "ticket:" + cookie_str.substr(425, 32);
	//const string idp = "idp:" + cookie_str.substr(528, 1);
	//const string tenantId = "tenantId:" + cookie_str.substr(605, 36);
	//const string lt = "lt:" + cookie_str.substr(711, 10);
	//const string platformid = "platformId:" + cookie_str.substr(799, 1);
	//const string userid = "userId:" + cookie_str.substr(874, 1);

	//rms-centos7513
	const string llId = "ltId:" + cookie_str.substr(207, 36);
	const string clientid_str = "clientId:" + cookie_str.substr(319, 32);
	const string ticket = "ticket:" + cookie_str.substr(425, 32);
	const string idp = "idp:" + cookie_str.substr(528, 1);
	const string tenantId = "tenantId:" + cookie_str.substr(605, 36);
	const string lt = "lt:" + cookie_str.substr(711, 21);
	const string platformid = "platformId:" + cookie_str.substr(810, 1);
	const string userid = "userId:" + cookie_str.substr(885, 1);

	std::stringstream out;
	curl_profile = curl_easy_init();
	curl_easy_setopt(curl_profile, CURLOPT_URL, url_profile.c_str());
	curl_easy_setopt(curl_profile, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
	headerlist = curl_slist_append(headerlist, llId.c_str());
	headerlist = curl_slist_append(headerlist, clientid_str.c_str());
	headerlist = curl_slist_append(headerlist, ticket.c_str());
	headerlist = curl_slist_append(headerlist, idp.c_str());
	headerlist = curl_slist_append(headerlist, tenantId.c_str());
	headerlist = curl_slist_append(headerlist, lt.c_str());
	headerlist = curl_slist_append(headerlist, platformid.c_str());
	headerlist = curl_slist_append(headerlist, userid.c_str());
	curl_easy_setopt(curl_profile, CURLOPT_HTTPHEADER, headerlist);

	//curl_easy_setopt(curl_profile, CURLOPT_COOKIE, llId.c_str());
	//curl_easy_setopt(curl_profile, CURLOPT_COOKIE, clientid_str.c_str());
	//curl_easy_setopt(curl_profile, CURLOPT_COOKIE, ticket.c_str());
	//curl_easy_setopt(curl_profile, CURLOPT_COOKIE, idp.c_str());
	//curl_easy_setopt(curl_profile, CURLOPT_COOKIE, tenantId.c_str());
	//curl_easy_setopt(curl_profile, CURLOPT_COOKIE, lt.c_str());
	//curl_easy_setopt(curl_profile, CURLOPT_COOKIE, platformid.c_str());
	//curl_easy_setopt(curl_profile, CURLOPT_COOKIE, userid.c_str());

	curl_easy_setopt(curl_profile, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl_profile, CURLOPT_WRITEDATA, &out);
	curl_easy_setopt(curl_profile, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl_profile);
	fclose(fptr);
	curl_easy_cleanup(curl_profile);

	if (res) {
		return false;
	}
	else {
		return true;
	}
}

bool ListProjectAdminResponseData(string tenantid) {
	// init curl  
	CURL *curl_profile = curl_easy_init();
	FILE *fptr;
	CURLcode res;
	struct curl_slist *headerlist = NULL;
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();
	string url_profile = server + "/rs/tenant/" + tenantid + "/projectAdmin";
	string client_id = "clientId:" + ReadFromFile(clientfilename);

	const char* fileName = projectAdminList_resturn.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	jroot = ParseJsonFile(user_respond);
	string ticket = "ticket:" + jroot["extra"]["ticket"].asString();
	string tenant_id = "tenantId:" + tenantid;
	string userId = "userId:" + to_string(jroot["extra"]["userId"].asInt());

	std::stringstream out;
	curl_profile = curl_easy_init();
	curl_easy_setopt(curl_profile, CURLOPT_URL, url_profile.c_str());
	curl_easy_setopt(curl_profile, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  

	headerlist = curl_slist_append(headerlist, client_id.c_str());
	headerlist = curl_slist_append(headerlist, ticket.c_str());
	headerlist = curl_slist_append(headerlist, tenant_id.c_str());
	headerlist = curl_slist_append(headerlist, userId.c_str());
	curl_easy_setopt(curl_profile, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl_profile, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl_profile, CURLOPT_WRITEDATA, &out);
	curl_easy_setopt(curl_profile, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl_profile);
	fclose(fptr);
	curl_easy_cleanup(curl_profile);

	if (res) {
		return false;
	}
	else {
		return true;
	}
}

bool TenantListResponseData() {
	// init curl  
	CURL *curl_profile = curl_easy_init();
	FILE *fptr;
	CURLcode res;
	struct curl_slist *headerlist = NULL;
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();
	string url_profile = server + "/rs/tenant/list";
	string client_id = "clientId:" + ReadFromFile(clientfilename);

	const char* fileName = tenantList_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	jroot = ParseJsonFile(user_respond);
	string ticket = "ticket:" + jroot["extra"]["ticket"].asString();
	string userId = "userId:" + to_string(jroot["extra"]["userId"].asInt());

	std::stringstream out;
	curl_profile = curl_easy_init();
	curl_easy_setopt(curl_profile, CURLOPT_URL, url_profile.c_str());
	curl_easy_setopt(curl_profile, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  

	headerlist = curl_slist_append(headerlist, client_id.c_str());
	headerlist = curl_slist_append(headerlist, ticket.c_str());
	headerlist = curl_slist_append(headerlist, userId.c_str());
	curl_easy_setopt(curl_profile, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl_profile, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl_profile, CURLOPT_WRITEDATA, &out);
	curl_easy_setopt(curl_profile, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl_profile);
	fclose(fptr);
	curl_easy_cleanup(curl_profile);

	if (res) {
		return false;
	}
	else {
		return true;
	}

}

bool AddRemoveTenantAminResponseData(string tenant_name, bool is_add, string email) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();
	string url = server + "/rs/tenant/" + tenant_name;
	string client_id = "clientId:" + ReadFromFile(clientfilename);
	string email_str;

	bool tenant_listresult = TenantListResponseData();
	bool is_repea = false;

	jroot = ParseJsonFile(tenantList_return);
	int tenant_count = jroot["results"]["details"]["tenantList"].size();
	for (int i = 0; i < tenant_count; i++) {
		string tenantname = jroot["results"]["details"]["tenantList"][i]["name"].asString();
		if (tenantname == tenant_name) {
			if (!is_add) {
				int email_count = jroot["results"]["details"]["tenantList"][i]["adminList"].size();
				for (int j = 0; j < email_count; j++) {
					string email_j = jroot["results"]["details"]["tenantList"][i]["adminList"][j].asString();
					if ((email_j == email) & !(is_add)) {
						continue;
					}
					else {
						email_j = email_j + ",";
						email_str = email_str + email_j;
					}
				}
			}
			else {
				int email_count = jroot["results"]["details"]["tenantList"][i]["adminList"].size();
				for (int j = 0; j < email_count; j++) {
					if (jroot["results"]["details"]["tenantList"][i]["adminList"][j].asString() == email) {
						continue;
					}
					else {
						string email_j = jroot["results"]["details"]["tenantList"][i]["adminList"][j].asString() + ",";
						email_str = email_str + email_j;
					}
				}
				email_str = email_str + email;
			}
		}
	}

	string jsonout = "{\"parameters\":{\"admin\":\"" + email_str + "\",\"description\":\"\"}}";
	const char* fileName = addRemoveTenantAdmin_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	jroot = ParseJsonFile(user_respond);
	string ticket = "ticket:" + jroot["extra"]["ticket"].asString();
	string userId = "userId:" + to_string(jroot["extra"]["userId"].asInt());

	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, client_id.c_str());
	headerlist = curl_slist_append(headerlist, ticket.c_str());
	headerlist = curl_slist_append(headerlist, userId.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

//PUT
bool AddRemoveProjectAdminResponseData(string tenant_id, bool is_add, string email) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();
	string url = server + "/rs/tenant/" + tenant_id + "/projectAdmin";
	string client_id = "clientId:" + ReadFromFile(clientfilename);
	string email_str;
	bool is_repea = false;

	bool projectAdmin_Listresult = ListProjectAdminResponseData(tenant_id);

	jroot = ParseJsonFile(projectAdminList_resturn);
	int admin_count = jroot["results"]["projectAdmin"].size();
	if (!is_add) {
		for (int j = 0; j < admin_count; j++) {
			string email_j = jroot["results"]["projectAdmin"][j]["email"].asString();
			if ((email_j == email) & !(is_add)) {
				continue;
			}
			else {
				email_j = "\"" + email_j + "\"" + ",";
				email_str = email_str + email_j;
			}
		}
	}
	else {
		for (int i = 0; i < admin_count; i++) {
			if (jroot["results"]["projectAdmin"][i]["email"].asString() == email) {
				continue;
			}
			else {
				string email_j = "\"" + jroot["results"]["projectAdmin"][i]["email"].asString() + "\"" + ",";
				email_str = email_str + email_j;
			}
		}
		email_str = email_str + "\"" + email + "\",";
	}

	email_str.erase(email_str.end() - 1);
	string jsonout = "{\"parameters\":{\"projectAdmin\":[" + email_str + "]}}";

	const char* fileName = addRemoveProjectAdmin_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	jroot = ParseJsonFile(user_respond);
	string ticket = "ticket:" + jroot["extra"]["ticket"].asString();
	string userId = "userId:" + to_string(jroot["extra"]["userId"].asInt());

	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, client_id.c_str());
	headerlist = curl_slist_append(headerlist, ticket.c_str());
	headerlist = curl_slist_append(headerlist, userId.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

int PolicyEvaluationResponseData(string membership_id, string tag_name, string tag_value) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();
	string url = server + "/rs/policyEval";
	string client_id = "clientId:" + ReadFromFile(clientfilename);

	string jsonout = "{\"parameters\":{\"evalRequest\":{\"adhocPolicy\":\"Policy body\",\"membershipId\" : \"" + membership_id + "\",\"resources\" : [{\"dimensionName\":\"from\",\"resourceType\" : \"fso\",\"resourceName\" : \"Example1.pdf\",\"duid\" : \"12345\",\"classification\" : {\"" + tag_name + "\":[\"" + tag_value + "\"]},\"attributes\":{\"file creation date\":[\"10-jan-2018\"]}}],\"rights\":2047,\"user\" : {\"id\":9,\"attributes\" : {\"Job Function\":[\"engineer\"	],\"Age\" : [	\"14\"	]}},\"application\":{\"name\":\"RMS\",\"path\" : \"/path/to/application\",\"pid\" : \"12345\",\"attributes\" : {\"publisher\":[\"nextlabs\",\"v1\"],\"licensed\" : [\"yes\"]}},\"host\":{\"ipAddress\":\"118.189.77.114\"},\"environments\" : [{\"name\":\"environment\",\"attributes\" : {\"connection_type\":[\"console\"]}}],\"evalType\":0}}}";

	const char* fileName = policyEval_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	jroot = ParseJsonFile(user_respond);
	string ticket = "ticket:" + jroot["extra"]["ticket"].asString();
	string userId = "userId:" + to_string(jroot["extra"]["userId"].asInt());

	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, client_id.c_str());
	headerlist = curl_slist_append(headerlist, ticket.c_str());
	headerlist = curl_slist_append(headerlist, userId.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return 0;
	}
	else {
		jroot = ParseJsonFile(policyEval_return);
		int rights = jroot["results"]["rights"].asInt();
		return rights;
	}
}

bool CheckUpgradeResponseData(string processorArch, string currentVersion) {
	// init curl  
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	Json::Value jroot;
	jroot = ParseJsonFile(tenant_respond);
	string server = jroot["results"]["server"].asString();
	string url = server + "/rs/upgrade?tenant=skydrm";
	string client_id = "clientId:" + ReadFromFile(clientfilename);

	string jsonout = "{\"parameters\":{\"platformId\":0,\"processorArch\":\"X86\",\"currentVersion\":\"1.0.1621\"}}";
	const char* fileName = checkUpgrade_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https   
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}

}

//GetPolicyModels  --Lifi.yuan 2018/11/30
bool GetPolicyModels(string user_jsonfile, string client_id, string rmsurl, string tenantId, string output_PolicyModelsInfo)
{
	CURL *curl;
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	Json::Value jroot;

	const char* fileName = output_PolicyModelsInfo.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	uint64_t t = (uint64_t)std::time(nullptr) * 1000;
	string url = rmsurl + "/rs/policy/"+ tenantId+ "/policies/models?t="+ to_string(t);
	jroot = ParseJsonFile(user_jsonfile);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 0);   // 1:POST, 0:Get
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); //SKIP_PEER_VERIFICATION  
														   //headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	//headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

//Deploy policy  --Lifi.yuan 2018/11/30
bool PolicyDeploy(string user_jsonfile, string client_id, string rmsurl, string policyModelId, string policyModelName,  string tenantId, string output_Project)
{
	CURL *curl;
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	Json::Value jroot;

	jroot = ParseJsonFile(user_jsonfile);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();

	string url = rmsurl + "/rs/policy/" + tenantId + "/policies";
	const char* fileName = output_Project.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;

	string rights = "\"VIEW\",\"PRINT\"";
	string bodyFields = "{\"parameters\":{\"policy\":{\"name\":\"RPM_Spin_policy01\",\"description\":\"RPM_Spin_policy01\",\"resources\":[{\"operator\":\"IN\",\"components\":[{\"conditions\":[{\"attribute\":\"ITAR\",\"operator\":\"=\",\"value\":\"TAA-01\"}],\"policyModel\":{\"id\":"
		+ policyModelId + ",\"name\":\"" + policyModelName + "\"},\"id\":null,\"type\":\"RESOURCE\",\"name\":\"\",\"description\":\"\"}]}],\"userComponents\":[],\"actions\":["
		+ rights + "],\"toDeploy\":true}}}";

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); //SKIP_PEER_VERIFICATION  
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	//headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyFields.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}

}
//Deploy policy  --Lifi.yuan 2018/11/30
bool SetClassification(string user_jsonfile, string client_id, string rmsurl, string tenantId, string output_return)
{
	CURL *curl;
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	Json::Value jroot;

	const char* fileName = output_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	
	string url = rmsurl + "/rs/classification/" + tenantId;
	jroot = ParseJsonFile(user_jsonfile);
	string user_id = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	const string header_client = "clientId:" + client_id;
	const string header_userid = "userId:" + user_id;
	const string header_ticket = "ticket:" + ticket;

	const string postFields = "{\"parameters\":{\"categories\":[{\"name\":\"ITAR\",\"mandatory\":false,\"multiSelect\":false,\"labels\":[{\"name\":\"TAA - 01\",\"default\":false},{\"name\":\"TAA - 02\"}]}]}}";

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);   // 1:POST, 0:Get
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); //SKIP_PEER_VERIFICATION  
	headerlist = curl_slist_append(headerlist, "Accept: application/json");
	//headerlist = curl_slist_append(headerlist, "platformId:0");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	headerlist = curl_slist_append(headerlist, header_userid.c_str());
	headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

//this part not ready --Lifi
bool DeletePolicyInCC(string ccrul, string JSESSIONID, string policyId, string output_return)
{
	CURL *curl;
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	
	string url = ccrul + "/console/api/v1/policy/mgmt/remove/" + policyId;
	const char* fileName = output_return.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}
	const string header_client = "JSESSIONID:" + JSESSIONID;
	//const string header_userid = "userId:" + user_id;
	//const string header_ticket = "ticket:" + ticket;

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); //SKIP_PEER_VERIFICATION  
	headerlist = curl_slist_append(headerlist, "Content-Type:application/json");
	headerlist = curl_slist_append(headerlist, "Connection:keep-alive");
	headerlist = curl_slist_append(headerlist, header_client.c_str());
	//headerlist = curl_slist_append(headerlist, header_userid.c_str());
	//headerlist = curl_slist_append(headerlist, header_ticket.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyFields.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

	res = curl_easy_perform(curl);
	fclose(fptr);
	curl_easy_cleanup(curl);
	if (res) {
		return false;
	}
	else {
		return true;
	}
}

//this part not ready --Lifi 2019-1
bool LoginWithADUser(string rmsurl, string tenant, string clientID, string username, string userid, string output_UserJson)
{
	CURL *curl;
	CURLcode res;
	FILE *fptr;
	struct curl_slist *headerlist = NULL;
	Json::Value jroot;

	const char* fileName = output_UserJson.c_str();
	if ((fptr = fopen(fileName, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", fileName);
		exit(1);
	}

	const string cookie = "Cookie: platformId=0; clientId=" + clientID;
	const string postFields = "userName=" + username + "&password=" + username + "&id=" + userid + "&tenant=" + tenant;
	string cookieFile = "cookieFile.txt";
	//Step_1
	string url = rmsurl + "/rms/IdpManager/LdapAuth/LdapAuthFinish";
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);   // 1:POST, 0:Get
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
	headerlist = curl_slist_append(headerlist, "Content-Type:application/x-www-form-urlencoded");
	headerlist = curl_slist_append(headerlist, cookie.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); //SKIP_PEER_VERIFICATION  
	curl_easy_setopt(curl, CURLOPT_HEADER, 1);
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookieFile);
	res = curl_easy_perform(curl);

	if (!res)
	{
		//Step_2
		curl_easy_cleanup(curl);

		curl = curl_easy_init();
		url = rmsurl + "/rms/login?i=4";
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 0);   // 1:POST, 0:Get
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); //SKIP_PEER_VERIFICATION  
		res = curl_easy_perform(curl);

		if (!res)
		{
			//Step_3
			curl_easy_cleanup(curl);
			url = rmsurl + "/rms/main";
			curl = curl_easy_init();
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_POST, 0);   // 1:POST, 0:Get
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); //SKIP_PEER_VERIFICATION  
			res = curl_easy_perform(curl);
			if (!res)
			{
				//Step_4
				curl_easy_cleanup(curl);
				uint64_t t = (uint64_t)std::time(nullptr) * 1000;
				url = rmsurl + "/rms/rs/usr/v2/profile/?t=" + to_string(t);
				curl = curl_easy_init();
				curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
				curl_easy_setopt(curl, CURLOPT_POST, 0);   // 1:POST, 0:Get
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
				curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookieFile);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); //SKIP_PEER_VERIFICATION 
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

				res = curl_easy_perform(curl);
				fclose(fptr);

				curl_easy_cleanup(curl);
				if (res) {
					cout << "4";
					return false;
				}
				else {
					return true;
				}
			}
			cout << "3";
			return false;
		}
		cout << "2";
		return false;
	}
	cout << "1";
	return false;
}