#include "stdafx.h"
#include "rmccore\restful\rmuser.h"
#include "rmccore\network\httpClient.h"
#include "Header_Tenant.h"
#include "Header_Token.h"
#include "Header_SystemPara.h"
#include "Header_File.h"

#include "rmccore\policy\condition.h"

using namespace std;
using namespace RMCCORE;

vector<vector<string>> csv_HttpRequest_MemTokenQuery = readCSV("User_HttpRequest_MemTokenQuery.csv");
vector<vector<string>> csv_HttpRequest_MemQuery = readCSV("User_HttpRequest_MemQuery.csv");
vector<vector<string>> csv_HttpRequest_LoginURL = readCSV("User_HttpRequest_LoginURL.csv");
vector<vector<string>> csv_HttpRequest_ShareLocalFileQuery = readCSV("User_HttpRequest_ShareLocalFile.csv");
vector<vector<string>> csv_HttpRequest_GetProductUpdateQuery = readCSV("Token_HttpRequest_ProductUpdate.csv");


//RMUser: GetUserLoginURL
class GetUserLoginURL :public::testing::TestWithParam<vector<string>> {};
TEST_P(GetUserLoginURL, ValidInput)
{
	//string clientid = GenerateClientID();

	vector<string> csv = GetParam();
	string caseID = csv[0];
	string jsonFile = csv[1];
	int importType = string2Int(csv[2]);
	int RMS_ID = string2Int(csv[3]);
	RMTenant tenant = CreateTenant_perRMS(RMS_ID);
	//string clientid = csv[4];
	string DeviceID = csv[4];

	//RMUser user;
	RMSystemPara clientInfo = GenerateSystemParameter(DeviceID);
	RMUser user(clientInfo, CreateTenant_perRMS(RMS_ID));
	string clientID_2 = clientInfo.GetClientID();
	//user = importUser(jsonFile, importType);

	//HTTPRequest request = user.GetUserLoginURL(clientid, tenant);
	HTTPRequest request = user.GetUserLoginURL();

	printHttpRequest(request);
	//Expected result
	string platfromid = "0";
	string cookies = "clientId" + clientID_2 + "platformId" + platfromid;
	exceptedHTTPRequest e = setValue(1, "", cookies, "", tenant);

	check_httpRequest(e, request);

}
INSTANTIATE_TEST_CASE_P(HTTPRequestTesting, GetUserLoginURL, testing::ValuesIn(csv_HttpRequest_LoginURL));

//RMUser: GetMembershipQuery
class GetMembershipQuery :public::testing::TestWithParam<vector<string>> {};
TEST_P(GetMembershipQuery, ValidInput)
{
	vector<string> csv = GetParam();
	string caseID = csv[0];
	int RMS_ID = string2Int(csv[1]);
	RMTenant tenant = CreateTenant_perRMS(RMS_ID);

	string jsonFile = csv[2];
	int importType = string2Int(csv[3]);
	string tenantId = csv[4];
	string deviceId = csv[5];
	//string clientid = csv[6];
	//if (clientid == "default")
	//{
	//	clientid = GenerateClientID();
	//}
	//string clientid = GenerateClientID();
	RMSystemPara clientInfo = GenerateSystemParameter(deviceId);
	cout << "clientInfo: " << clientInfo.ExportToString() << endl;
	RMUser user(clientInfo, CreateTenant_perRMS(RMS_ID));
	//cout <<"User 1----:" << user.ExportToString() << endl;
	RMMembership mem;

	//RMUser u = importUser(jsonFile, importType);
	//user.ImportFromJson(u.ExportToJson()->AsObject());

	RMCCORE::JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	string jsonstr = ReadFromFile(jsonFile);
	cout << "jsonstr length is: " << jsonstr.length() << endl;
	doc.LoadJsonString(jsonstr, &err_code, &err_pos);
	RMCCORE::JsonValue* root;

	root = doc.GetRoot();
	//cout << "debug importUser- before import" << endl;
	RetValue ret = user.ImportFromRMSResponse(root->AsObject());
	checkErrorCode(ret, jsonFile);
	printUserInfo(user);

	cout << "User 2----:" << user.ExportToString() << endl;

	mem = FindMembership(user, tenantId);

	/*rmccore::JsonDocument doc;
	rmccore::JsonValue* root;
	int err_code = 0;
	size_t err_pos = 0;
	string jsonstr = ReadFromFile("successlogin.json");
	doc.LoadJsonString(jsonstr, &err_code, &err_pos);
	root = doc.GetRoot();
	user.ImportFromRMSResponse(root->AsObject());
	mem = user.GetDefaultMembership();*/

	//HTTPRequest request = user.GetMembershipQuery(clientid, deviceId, tenant, mem);
	HTTPRequest request = user.GetMembershipQuery(mem);

	printHttpRequest(request);

	Json::Value jroot;
	jroot = ParseJsonFile(jsonFile);
	string userId = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();

	string platfromid = "0";
	string publicKey = "";

	string head = "clientId" + clientInfo.GetClientID()
		+ "platformId" + platfromid
		+ "deviceId" + deviceId
		+ "tenant" + tenant.GetTenant().c_str()
		+ "userId" + userId
		+ "ticket" + ticket
		+ "Content-Type" + "application/json";
	//string body = "{\"parameters\": {\"userId\":\"" + userId + "\",\"ticket\":\"" + ticket + "\",\"membership\":\"" + mem.GetID() + "\",\"publicKey\":\"" + publicKey + "\"}}";
	string body = "{\"parameters\":{\"userId\":\"" + userId + "\",\"ticket\":\"" + ticket + "\",\"membership\":\"" + mem.GetID() + "\",\"publicKey\":\"" + publicKey;

	exceptedHTTPRequest e = setValue(2, head, "", body, tenant);

	check_httpRequest(e, request, true);
}
INSTANTIATE_TEST_CASE_P(HTTPRequestTesting, GetMembershipQuery, testing::ValuesIn(csv_HttpRequest_MemQuery));

//RMUser: GetMembershipTokenQuery
class GetMembershipTokenQuery :public::testing::TestWithParam<vector<string>> {};
TEST_P(GetMembershipTokenQuery, ValidInput)
{
	vector<string> csv = GetParam();
	string caseID = csv[0];
	int RMS_ID = string2Int(csv[1]);
	RMTenant tenant = CreateTenant_perRMS(RMS_ID);
	string jsonFile = csv[2];
	int importType = string2Int(csv[3]);
	string tenantId = csv[4];
	string deviceId = csv[5];
	int TokenCount = string2Int(csv[7]);

	RMSystemPara clientInfo = GenerateSystemParameter(deviceId);

	RMMembership mem;

	RMCCORE::JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	RMCCORE::JsonValue* root;
	const std::string certstr = ReadFromFile(DEFAULT_RMSCerts_FILE);
	!doc.LoadJsonString(certstr, &err_code, &err_pos);
	root = doc.GetRoot();

	RMUser user = importUser58_2(jsonFile, RMS_ID, clientInfo);
	if (tenantId == "Default")
	{
		user.GetDefaultMembership().m_certificate.ImportFromRMSResponse(root->AsObject());
	}
	else
	{
		user.FindMembership(tenantId)->m_certificate.ImportFromRMSResponse(root->AsObject());
	}

	printUserInfo(user);

	mem = FindMembership(user, tenantId);

	//HTTPRequest request = user.GetMembershipTokenQuery(clientid, deviceId, tenant, mem, TokenCount);
	HTTPRequest request = user.GetMembershipTokenQuery(mem, TokenCount);

	printHttpRequest(request);
	Json::Value jroot;
	jroot = ParseJsonFile(jsonFile);
	string userId = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();

	string platfromid = "0";
	string agreement = "532EDC7688417404A61A9F7D4D2F4535873131DE2E180EB5C3A1D9AADF4E6FBDCAA470043CBAF5BF0DF130556AB725C9F449C68F1A419A3A1A7376A2EB935B89DBCCBD0CEAA0AA547B63B963F8F34DAEE4D2991EBD2469A47F6DF915A31DC8A36D0A69FBFA96171716FBACDB270F62C372A9A4CEC9503D62AB7B54DA0B0BBC5F5C6171BCE43FD2EE137D4C3CB2CDFE4AF6B99567127658754BADA622415E06D6C691127D8CD2061CA878A41757C4F3E72EE3DD189D1E5760FCDB75A5678396F297F92A9CF9DD96DB35BCF3D9FE490C24B02B24F516AC0AFE7663398880D038A69C43E7BA847A5A48C2CF008768F62B41BFFD58BFDB5F6BAA589ED9B5B3662D8D";
	//string head = "clientId" + clientInfo.GetClientID()
	//	+ "platformId" + platfromid
	//	+ "deviceId" + deviceId
	//	+ "tenant" + tenant.GetTenant().c_str()
	//	+ "userId" + userId
	//	+ "ticket" + ticket
	//	+ "Content-Type" + "application/json";
	//string body = "{\"parameters\":{\"userId\":\"" + userId
	//	+ "\",\"ticket\":\"" + ticket + "\",\"membership\":\"" + mem.GetID()
	//	+ "\",\"agreement\":\"" + agreement
	//	+ "\",\"count\":\"" + to_string(TokenCount) + "\"}}";

	string head = "clientId" + clientInfo.GetClientID()
		+ "platformId" + to_string(clientInfo.GetPlatformID())
		+ "deviceId" + clientInfo.GetDeviceID()
		+ "tenant" + tenant.GetTenant().c_str()
		+ "userId" + userId
		+ "ticket" + ticket
		+ "Content-Type" + "application/json";
	string body = "{\"parameters\":{\"userId\":\"" + userId
		+ "\",\"ticket\":\"" + ticket + "\",\"membership\":\"" + mem.GetID()
		+ "\",\"agreement\":\"" + agreement
		+ "\",\"count\":" + to_string(TokenCount) + "}}";

	exceptedHTTPRequest e = setValue(3, head, "", body, tenant);

	check_httpRequest(e, request);
}
INSTANTIATE_TEST_CASE_P(HTTPRequestTesting, GetMembershipTokenQuery, testing::ValuesIn(csv_HttpRequest_MemTokenQuery));

//RMUser: GetShareLocalFileQuery - Create by Lifi 3/14/2018
class GetShareLocalFileQuery :public::testing::TestWithParam<vector<string>> {};
TEST_P(GetShareLocalFileQuery, ValidInput)
{
	vector<string> csv = GetParam();
	string caseID = csv[0];
	int RMS_ID = string2Int(csv[1]);
	string jsonFile = csv[2];
	string tenantId = csv[3];

	string sourceFile = csv[4];
	string destNXL = csv[5];
	string rights_csv = csv[6];
	string obligation = csv[7];
	string recipients_csv = csv[8];

	string comment_csv = csv[9];

	//int expiry_option= string2Int(csv[10]);
	string expiry_option = csv[10];
	const char* expiry_startDate = csv[11].c_str();
	const char* expiry_endDate = csv[12].c_str();

	string tag_csv = csv[13];


	//import user
	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;
	RMCCORE::JsonValue* root;
	const std::string certstr = ReadFromFile(DEFAULT_RMSCerts_FILE);
	!doc.LoadJsonString(certstr, &err_code, &err_pos);
	root = doc.GetRoot();
	RMSystemPara clientInfo;
	RMUser user = importUser58_2(jsonFile, RMS_ID, clientInfo);

	RMMembership mem;
	//mem = FindMembership(user, tenantId);
	//user.FindMembership(tenantId)->m_certificate.ImportFromRMSResponse(root->AsObject());
	mem = user.GetDefaultMembership();
	user.GetDefaultMembership().m_certificate.ImportFromRMSResponse(root->AsObject());

	//get rights
	uint64_t rights_value = getRightsValue(rights_csv);
	//NXLFMT::FullRights	rights;
	NXLFMT::Rights rights(rights_value);
	printRights(rights);

	//Obligation
	NXLFMT::Obligations obs;

	//recipients
	string r_tolower = recipients_csv;
	transform(r_tolower.begin(), r_tolower.end(), r_tolower.begin(), ::tolower);
	if (r_tolower == "null")
	{
		recipients_csv = "";
	}
	vector<std::string> recipients;
	splitString(recipients_csv, recipients, "|");

	//activitylog
	RMActivityLog activitylog;

	//token
	RMTokenPool tokenpool = GetDefaultTokens();
	assert(tokenpool.size() != 0);
	RMToken token = tokenpool.pop();

	//expiry
	int expiry_type= string2Int(expiry_option);


	//CONDITION::Expiry expiry(CONDITION::ExpiryType::NEVEREXPIRE);
	CONDITION::Expiry expiry(CONDITION::ExpiryType::RELATIVEEXPIRE, 1541521573,RMCCORE::NXTime::nowTime() * 1000);

	NXLAttributes attr;
	attr.rights = &rights;
	attr.obs = &obs;
	attr.tags = "{}";
	attr.expiry = &expiry;

	//share file
	RMNXLFile nfile("", "");

	RetValue rev = user.ShareLocalFile(sourceFile, destNXL, attr, recipients, token, comment_csv, activitylog, nfile);
	cout << "---Share finished." << endl;

	//cout << "nfile opened?: " << nfile.IsOpened() << endl;
	//nfile.Open(token);
	//cout<< "nfile opened?: "<< nfile.IsOpened() << endl;
	//nfile.Open();
	//cout << "nfile opened?: " << nfile.IsOpened() << endl;
	//assert(nfile.Open());
	NXLFMT::Rights nxlrights(nfile.GetNXLRights());
	printRights(nxlrights);
	HTTPRequest request = user.GetShareLocalFileQuery(mem, nfile);

	cout << "====1===" << endl;
	printHttpRequest(request);

	RMNXLFile nfile2(nfile.ExportToJson()->AsObject());
	cout << "duid: " << nfile2.GetDuid();
	HTTPRequest request2 = user.GetShareLocalFileQuery(mem, nfile2);
	cout << endl << "====2=nfile.ExportToJson()==" << endl;
	printHttpRequest(request2);

	RMNXLFile nfile3(destNXL);
	cout << "duid: " << nfile3.GetDuid();
	nfile3.SetRecipientsList(recipients);
	HTTPRequest request3 = user.GetShareLocalFileQuery(mem, nfile3);
	cout << endl << "====3=destNXL==" << endl;
	printHttpRequest(request3);


	cout << "====================" << endl;

	//Below for check result
	Json::Value jroot;
	jroot = ParseJsonFile(jsonFile);
	string userId = jroot["extra"]["userId"].asString();
	string ticket = jroot["extra"]["ticket"].asString();
	RMTenant tenant = CreateTenant_perRMS(RMS_ID);
	string ContentType = "multipart/form-data";
	string head = "Content-Type" + ContentType
		+ "; boundary=" + "\"7e1c41151168\""
		+ "clientId" + clientInfo.GetClientID()
		+ "platformId" + "0"
		+ "deviceId" + clientInfo.GetDeviceID()
		+ "tenant" + tenant.GetTenant().c_str()
		+ "userId" + userId
		+ "ticket" + ticket;

	cout << "---debug1" << endl;

#pragma region
	string b_parameters = "{\"parameters\":{";
	string b_comma = ",";
	string b_asAttachment = "\"asAttachment\":\"false\"";
	string b_sharedDocument = "\"sharedDocument\":{";
	string b_end = "}}}";

	string b_membershipId = "\"membershipId\":\"" + mem.GetID() + "\"";

	ostringstream os;
	os << rights_value;
	string permissions;
	istringstream is(os.str());
	is >> permissions;
	string b_permissions = "\"permissions\":" + permissions;
	int pos = sourceFile.find_last_of('\\');
	string fname(sourceFile.substr(pos+1));
	pos= fname.find_last_of('.');
	string ext(fname.substr(pos+1));
	string b_metadata = "\"metadata\":\"{\\\"fileName\\\":\\\""+ fname +"\\\",\\\"fileExtension\\\":\\\""+ext+"\\\"}\"";

	replace_all(sourceFile, "\\", "\\\\");
	string b_fileName = "\"fileName\":\"" + sourceFile + "\"";
	string b_filePathId = "\"filePathId\":\"" + sourceFile + "\"";
	string b_filePath = "\"filePath\":\"" + sourceFile + "\"";

	//Recipients
	string b_recipients = getJsonPara_recipients(recipients);

	//Comment
	string b_comment = getJsonPara_oneValue("comment", comment_csv);

	//Watermark
	string b_watermark = getJsonPara_oneValue("watermark", obligation);

	//expiry
	string b_expiry = getJsonPara_expiry(expiry_option, expiry_startDate, expiry_endDate);

	//Tags
	string b_tags = getJsonPara_tags(tag_csv);

	cout << "---debug2" << endl;

#pragma endregion
	string body = b_parameters
		+ b_asAttachment + b_comma
		+ b_sharedDocument
		+ b_membershipId + b_comma
		+ b_tags
		+ b_permissions + b_comma
		+ b_metadata + b_comma
		+ b_filePathId + b_comma
		+ b_filePath + b_comma
		+ b_recipients
		+ b_comment
		+ b_expiry
		+ b_watermark
		+ b_end;

	cout << "---- My test excepted Josn body:---- " << endl << body << endl << "-------------------------" << endl;


	exceptedHTTPRequest e = setValue(4, head, "", body, tenant);

	check_httpRequest(e, request);


}
INSTANTIATE_TEST_CASE_P(HTTPRequestTesting, GetShareLocalFileQuery, testing::ValuesIn(csv_HttpRequest_ShareLocalFileQuery));


//Token: GetProductUpdateQuery - Create by Lifi 4/22/2018
class GetProductUpdateQuery :public::testing::TestWithParam<vector<string>> {};
TEST_P(GetProductUpdateQuery, ValidInput)
{
	vector<string> csv = GetParam();
	string caseID = csv[0];
	int RMS_ID = string2Int(csv[1]);
	int pid1 = string2Int(csv[2]);
	int pid2 = string2Int(csv[3]);


	RMPlatformID platformId;
	RMProcessorArch processorArch;
	for (RMPlatformID p = RPWindowsDesktop; p <= RPAndriodTablet; p = (RMPlatformID)(p + 1))
	{
		cout << p << ",";
		if (p == pid1)
		{
			platformId = p;
			break;
		}
	}
	cout << endl;

	for (RMProcessorArch p = RPAUnknown; p <= RPAWindowsIA64; p = (RMProcessorArch)(p + 1))
	{
		if (p == pid2)
		{
			processorArch = p;
			break;
		}
	}

	RMProduct product = RMProduct("TestAPP", 10, 1, 1001);
	RMSystemPara clientInfo = GenerateSystemParameter(product, platformId, processorArch);
	RMTenant tenant = CreateTenant_perRMS(RMS_ID);

	HTTPRequest request = tenant.GetProductUpdateQuery(clientInfo);
	printHttpRequest(request);


}
INSTANTIATE_TEST_CASE_P(HTTPRequestTesting, GetProductUpdateQuery, testing::ValuesIn(csv_HttpRequest_GetProductUpdateQuery));
