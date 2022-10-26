#include "stdafx.h"
#include "RMCCORE\restful\rmnxlfile.h"
#include "RMCCORE\restful\rmuser.h"
#include "Header_HttpRequest.h"
#include "RMCCORE\format\nxlfile.h"
#include "Header_Tenant.h"
#include "Header_Token.h"
#include "RMCCORE\restful\rmactivitylog.h"
#include "Header_File.h"
#include "time.h"
#include <ctime>

using namespace RMCCORE;
using namespace std;
using namespace NXLFMT;


vector<vector<string>> csv_ProtectLocalFile = readCSV("User_protectLocalFile.csv");
vector<vector<string>> csv_ShareLocalFile = readCSV("User_shareLocalFile.csv");

/*
Membership ID: m14@skydrm.com
Recipients: Gavin.Ye@nextlabs.com
Rights: except download
Id: 2E14D330C7B0D5B3F9191A468FB7BDB8
Key: B91CBBB729EB16D1F441A271C825AB7241517433A5D74398D1CDEB09B198E13A
ML: 0
*/
static const uint8_t kTokenId[16] = {
	0x2E, 0x14, 0xD3, 0x30, 0xC7, 0xB0, 0xD5, 0xB3, 0xF9, 0x19, 0x1A, 0x46, 0x8F, 0xB7, 0xBD, 0xB8
};
static const uint8_t kTokenValue[32] = {
	0xB9, 0x1C, 0xBB, 0xB7, 0x29, 0xEB, 0x16, 0xD1, 0xF4, 0x41, 0xA2, 0x71, 0xC8, 0x25, 0xAB, 0x72,
	0x41, 0x51, 0x74, 0x33, 0xA5, 0xD7, 0x43, 0x98, 0xD1, 0xCD, 0xEB, 0x09, 0xB1, 0x98, 0xE1, 0x3A
};
static const uint32_t kTokenLevel = 0;
static const std::string kOwnerId("m14@skydrm.com");
static const std::string kRecipient("Jewelry.zhu@nextlabs.com");

#define NXL_TEST_PLAIN_FILE	"DecryptRMSTokenTest.txt"
#define NXL_TEST_NXL_FILE	"NormalTXTFile.txt.nxl"
#define DEFAULT_RMSCerts_FILE		"D:\\workplace\\RMCCORE\\TestAPI\\TestGoogleTest\\TestGoogleTest\\RMSCertsTest.txt"
const string plain_file_path = "D:\\workplace\\RMCCORE\\TestAPI\\TestGoogleTest\\TestGoogleTest\\plan file\\";
//const string plain_file_path = "\\\\autonas01.qapf1.qalab01.nextlabs.com\\Public\\Users\\jewelry\\";
//const string nxl_file_path = "C:\\GENERATE_FILE\\";
const string nxl_file_path = "D:\\workplace\\RMCCORE\\TestAPI\\TestGoogleTest\\TestGoogleTest\\Generate File\\";
/*
const RMNXLFile& GetNXLFile(string fileName){
	try {
		uint64_t accessTime = 0;
		string file_name_wstr = fileName + ".nxl";
		//std::string file_name_str(file_name_wstr.begin(), file_name_wstr.end());
		string plain_file = plain_file_path + fileName;
		string nxl_file = nxl_file_path + fileName + ".nxl";
		RMUser user = GetDefaultUser();
		NXLFMT::Obligations obs;
		NXLFMT::FullRights rights;
		RMActivityLog activity_log ;
		JsonValue * j_v = activity_log.ExportToJson();
		RMTokenPool tokenpool = GetDefaultTokens();
		assert(tokenpool.size() != 0);
		RMToken token = tokenpool.pop();

		static RMNXLFile nfile("","");
		
		//RetValue  ret= user.ProtectLocalFile(plain_file, nxl_file, obs, rights, token, activity_log, nfile);
		return nfile;
	}
	catch (exception& e) {
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

const RMNXLFile& GetSharedFile(string fileName) {
	uint64_t accessTime = 0;
	string file_name_wstr = fileName + ".nxl";
	//std::string file_name_str(file_name_wstr.begin(), file_name_wstr.end());
	string plain_file = plain_file_path + fileName;
	string nxl_file = nxl_file_path + fileName + ".nxl";
	string comment = "Comment Test";
	vector<std::string> recepients = { "jewelry.zhu@nextlabs.com"};
	RMUser user = GetDefaultUser();
	NXLFMT::Obligations obs;
	NXLFMT::FullRights rights;
	RMActivityLog activity_log;
	JsonValue * j_v = activity_log.ExportToJson();

	RMTokenPool tokenpool = GetDefaultTokens();
	assert(tokenpool.size() != 0);
	RMToken token = tokenpool.pop();

	static RMNXLFile nfile("", "");
	RetValue  ret = user.ShareLocalFile(plain_file, nxl_file, recepients, obs, rights, token, comment, activity_log, nfile);
	return nfile;
}


//ProtectLocalFile - Create by Lifi.Yuan 4/2018 
class testProtectLocalFile :public::testing::TestWithParam<vector<string>> {};
TEST_P(testProtectLocalFile, ValidInput)
{
	vector<string> csv = GetParam();
	string caseID = csv[0];
	int RMS_ID = string2Int(csv[1]);
	//RMTenant tenant = CreateTenant_perRMS(RMS_ID);
	string jsonFile = csv[2];
	string sourceFile = csv[3];
	string destNXL = csv[4];
	string obligation = csv[5];
	string rights_csv = csv[6];

	//import user
	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;
	RMCCORE::JsonValue* root;
	const std::string certstr = ReadFromFile(DEFAULT_RMSCerts_FILE);
	!doc.LoadJsonString(certstr, &err_code, &err_pos);
	root = doc.GetRoot();
	RMUser user = importUser58(jsonFile, RMS_ID);
	user.GetDefaultMembership().m_certificate.ImportFromRMSResponse(root->AsObject());
	//cout << "User string: " << user.ExportToString() << endl;

	//get rights
	uint64_t rights_value = getRightsValue(rights_csv);
	//NXLFMT::FullRights	rights;
	NXLFMT::Rights rights(rights_value);
	printRights(rights);

	//Obligation
	NXLFMT::Obligations obs;

	RMActivityLog activitylog;

	RMTokenPool tokenpool = GetDefaultTokens();
	assert(tokenpool.size() != 0);
	RMToken token = tokenpool.pop();

	//Protect file
	RMNXLFile nfile("", "");
	RetValue rev = user.ProtectLocalFile(sourceFile, destNXL, obs, rights, token, activitylog, nfile);

	////update from build1077
	//RMNXLFile nfile;
	//RetValue ret = user.ProtectLocalFile(sourceFile, destNXL, obs, rights, token, activitylog, nfile);


	assert(nfile.Open());
	cout << nfile.ExportToString() << endl;
	cout << nfile.GetNXLRights() << "," << nfile.GetFileExtension() << "," << nfile.size() << endl;


	//print rights for check result.
	//cout <<"NXL file rights_value: "<< nfile.GetNXLRights() << endl;
	NXLFMT::Rights nxlrights(nfile.GetNXLRights());
	printRights(nxlrights);

	EXPECT_EQ(rights_value, nfile.GetNXLRights());

	nfile.Close();
	cout << "[check after close]rights_value:  " << nfile.GetNXLRights() << endl;

	//GetFileSpaceSize(sourceFile);
	//GetFileSpaceSize(destNXL);
	EXPECT_EQ_CompareFileSpaceSize(sourceFile, destNXL);

	//RMNXLFile nfile2 = user.ProtectLocalFile(sourceFile, "002.txt.nxl", obs, rights, tokens);
}
INSTANTIATE_TEST_CASE_P(RMNXLFileTesting, testProtectLocalFile, testing::ValuesIn(csv_ProtectLocalFile));

//ShareLocalFile - Create by Lifi.Yuan 4/2018 
class testShareLocalFile :public::testing::TestWithParam<vector<string>> {};
TEST_P(testShareLocalFile, ValidInput)
{
	vector<string> csv = GetParam();
	string caseID = csv[0];
	int RMS_ID = string2Int(csv[1]);
	string jsonFile = csv[2];
	string sourceFile = csv[3];
	string destNXL = csv[4];
	string obligation = csv[5];
	string rights_csv = csv[6];
	string recipients_csv = csv[7];
	string comments = csv[8];

	//import user
	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;
	RMCCORE::JsonValue* root;
	const std::string certstr = ReadFromFile(DEFAULT_RMSCerts_FILE);
	!doc.LoadJsonString(certstr, &err_code, &err_pos);
	root = doc.GetRoot();
	RMUser user = importUser58(jsonFile, RMS_ID);
	user.GetDefaultMembership().m_certificate.ImportFromRMSResponse(root->AsObject());
	//cout << "User string: " << user.ExportToString() << endl;

	//get rights
	uint64_t rights_value = getRightsValue(rights_csv);
	//NXLFMT::FullRights	rights;
	NXLFMT::Rights rights(rights_value);
	printRights(rights);

	//Obligation
	NXLFMT::Obligations obs;

	//recipients
	vector<std::string> recipients;
	splitString(recipients_csv, recipients, "|");

	RMActivityLog activitylog;

	RMTokenPool tokenpool = GetDefaultTokens();
	assert(tokenpool.size() != 0);
	RMToken token = tokenpool.pop();
	//share file
	RMNXLFile nfile("", "");
	
	//RetValue rev = user.ShareLocalFile(sourceFile, destNXL, recipients, obs, rights, token, comments, activitylog, nfile);

	assert(nfile.Open());
	cout << nfile.ExportToString() << endl;
	cout << nfile.GetNXLRights() << "," << nfile.GetFileExtension() << "," << nfile.size() << endl;


	//print rights for check result.
	//cout <<"NXL file rights_value: "<< nfile.GetNXLRights() << endl;
	NXLFMT::Rights nxlrights(nfile.GetNXLRights());
	printRights(nxlrights);

	EXPECT_EQ(rights_value, nfile.GetNXLRights());

	//check recipients
	string Recipients_string = "";
	vector<std::string > rlist = nfile.GetFileRecipients().GetRecipients();
	for (size_t i = 0; i < rlist.size(); i++) {
		if (i != 0)
		{
			Recipients_string = Recipients_string + "|";
		}
		Recipients_string = Recipients_string + rlist[i];
	}
	cout << "GetRecipients: " << Recipients_string << endl;
	EXPECT_STREQ(recipients_csv.c_str(), Recipients_string.c_str());


	nfile.Close();
	cout << "[check after close]rights_value:  " << nfile.GetNXLRights() << endl;

	//GetFileSpaceSize(sourceFile);
	//GetFileSpaceSize(destNXL);
	EXPECT_EQ_CompareFileSpaceSize(sourceFile, destNXL);

	//RMNXLFile nfile2 = user.ProtectLocalFile(sourceFile, "002.txt.nxl", obs, rights, tokens);
}
INSTANTIATE_TEST_CASE_P(RMNXLFileTesting, testShareLocalFile, testing::ValuesIn(csv_ShareLocalFile));

//Test RMNXLFile open and close initial with RMNXLFile(const std::string& path)
class RMNXLFileOpenCloseTest : public::testing::TestWithParam<vector<string>>{};
TEST_P(RMNXLFileOpenCloseTest, RMNXLFileOpenCloseTest) {
	bool result;
	string token_file;
	string nxl_file;
	int expect_result;
	int expect_code;
	string expect_message;
	bool open_result;
	try {
		vector<string> csv = GetParam();
		token_file = csv[3];
		nxl_file = csv[2];
		expect_result = const_str_int(csv[4]);
		expect_message = csv[5];
		expect_code = const_str_int(csv[6]);
		RMTokenPool tpool;
		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;
		int err_code = 0;
		size_t err_pos = 0;
		string tokenstr = ReadFromFile(token_file);
		RMNXLFile nxlfile = GetNXLFile(nxl_file);
		open_result = nxlfile.IsOpened();
		EXPECT_EQ(open_result,0);
		std::cout << "tokenstr length is: " << tokenstr.length() << endl;
		doc.LoadJsonString(tokenstr, &err_code, &err_pos);
		root = doc.GetRoot();
		RetValue ret = tpool.ImportFromRMSResponse(root->AsObject());

		RMToken token = tpool.pop();

		cout << "GetDUID result : " << token.GetDUID() << endl;
		cout << "GetKey result : " << token.GetKey() << endl;
		EXPECT_DOUBLE_EQ(expect_code, nxlfile.GetLastError().GetCode());
		//EXPECT_STREQ(expect_message.c_str(), nxlfile.GetLastError().GetMessage());
		cout << "get code " << nxlfile.GetLastError().GetCode()  << endl;
		//cout << "get message " << nxlfile.GetLastError().GetMessage() << endl;
		//Open file
		if (!nxlfile.GetLastError().GetCode()) {
			result = nxlfile.Open(token);
			cout << "Open result : " << result << endl;
			EXPECT_EQ(nxlfile.IsOpened(), 1);
			nxlfile.Close();
			EXPECT_EQ(nxlfile.IsOpened(), 0);
			EXPECT_DOUBLE_EQ(expect_result, result);
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
	}
}
vector<vector<string>> vf = readCSV("RMNXLFileOpen.csv");
INSTANTIATE_TEST_CASE_P(RMNXLFileOpenCloseTestReturn, RMNXLFileOpenCloseTest, testing::ValuesIn(vf));


//Test	RMNXL File from protect rights: view edit print, TXT FILE
class RMNXLFileInfoFromProtectTest : public::testing::TestWithParam<vector<string>> {};
TEST_P(RMNXLFileInfoFromProtectTest, RMNXLNormalTXTTest) {
	try {
		vector<string> csv = GetParam();
		uint64_t accessTime = 0;
		string uiid;
		string path;
		uint64_t size;
		string rights_list = csv[3];
		string file_name = csv[2];
		string nxlFile_name = csv[2] + ".nxl";
		string plain_file = plain_file_path + file_name;
		string nxl_file = nxl_file_path + nxlFile_name;
		vector<std::string> recepients = {"jewelry.zhu@nextlabs.com", "jannet.zhou@nextlabs.com", "lifi.yuan@nextlabs.com"};
		RMUser user = GetDefaultUser();
		NXLFMT::Obligations obs;
		NXLFMT::Rights rights;
		rights = getRightsValue(rights_list);
		//Protect file
		RMActivityLog activity_log;
		JsonValue * j_v = activity_log.ExportToJson();
		RMNXLFile nfile("","");
		RMTokenPool tokenpool = GetDefaultTokens();
		assert(tokenpool.size() != 0);
		RMToken token = tokenpool.pop();
		RetValue ret = user.ProtectLocalFile(plain_file, nxl_file, obs, rights, token, activity_log, nfile);
		if (ret.GetCode() == 0) {
			//Open file
			EXPECT_DOUBLE_EQ(1, nfile.Open());
			//Set recipients and Check
			RMRecipients recipient_result;
			nfile.SetRecipientsList(recepients);
			recipient_result = nfile.GetFileRecipients();
			vector<std::string> rc_result = recipient_result.GetRecipients();
			if (rc_result.size() == recepients.size()) {
				for (size_t i = 0; i<recepients.size(); i++)
					EXPECT_STREQ(rc_result[i].c_str(), recepients[i].c_str());
			}
			else {
				EXPECT_DOUBLE_EQ(rc_result.size(), recepients.size());
			}

			//Check file name, extension
			EXPECT_STREQ(nfile.GetFileName().c_str(), nxlFile_name.c_str());
			EXPECT_STREQ(nfile.GetFileExtension().c_str(), "nxl");

			NXLFMT::Rights nxlrights(nfile.GetNXLRights());
			printRights(nxlrights);
			const uint64_t rights1 = nfile.GetNXLRights();
			cout << "Rights: " << endl;
			cout << rights1 << endl;
			NXLFMT::Rights nxlright(nfile.GetNXLRights());
			std::vector<std::string> rightstr;
			std::vector<std::string> rightstr_result;
			rights.ToStrings(rightstr);
			nxlright.ToStrings(rightstr_result);
			//Check rights
			if (rightstr.size() == rightstr_result.size()) {
				for (size_t i = 0; i < rightstr.size(); i++)
				{
					EXPECT_STREQ(rightstr[i].c_str(), rightstr_result[i].c_str());
				}
			}
			else {
				cout << "Rights issue!" << endl;
				EXPECT_EQ(rightstr.size(), rightstr_result.size());
			}

			//Check ImportFromJson ImportFromString ExportToJson ExporToString
			JsonValue * jv_root = nfile.ExportToJson();
			string export_str = nfile.ExportToString();
			nfile.ImportFromString(export_str);

			EXPECT_STREQ(nfile.GetFileName().c_str(), nxlFile_name.c_str());
			EXPECT_STREQ(nfile.GetFileExtension().c_str(), "nxl");

			nfile.Close();
		}
		else {
			cout << "Failed!" << endl;
			cout << ret.GetCode() << endl;
			EXPECT_FALSE(ret.GetCode());
		}
		cout << "Test Finished!" << endl;
	}
	catch (exception& e) {
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> vf1 = readCSV("ProtectFile.csv");
INSTANTIATE_TEST_CASE_P(RMNXLFileInfoFromProtectTestReturn, RMNXLFileInfoFromProtectTest, testing::ValuesIn(vf1));


//Test	RMNXL File from protect rights: Full rights, TXT FILE
TEST(RMNXLFileFullRightsTest, FullRightsTXTTest) {
	try {
		string file_name = "Full Rights.txt.nxl";
		vector<std::string>rights_result = { "VIEW","EDIT","PRINT","CLIPBOARD","SAVEAS","DECRYPT","SCREENCAP","SEND","CLASSIFY","SHARE","DOWNLOAD" };
		string plain_file = plain_file_path + "Full Rights.txt";
		string nxl_file = nxl_file_path + file_name;
		RMUser user = GetDefaultUser();
		NXLFMT::Obligations obs;
		NXLFMT::FullRights	rights;
		RMNXLFile nfile("", "");
		RMActivityLog activity_log;
		//Protect file
		RMTokenPool tokenpool = GetDefaultTokens();
		assert(tokenpool.size() != 0);
		RMToken token = tokenpool.pop();
		RetValue ret = user.ProtectLocalFile(plain_file, nxl_file, obs, rights, token, activity_log, nfile);

		if (ret.GetCode() == 0) {
			//Open file
			EXPECT_DOUBLE_EQ(1, nfile.Open());
			EXPECT_STREQ(nfile.GetFileName().c_str(), file_name.c_str());
			EXPECT_STREQ(nfile.GetFileExtension().c_str(), "nxl");

			//Check rights
			NXLFMT::Rights nxlright(nfile.GetNXLRights());
			std::vector<std::string> rightstr;
			std::vector<std::string> rightstr_result;
			nxlright.ToStrings(rightstr);
			rights.ToStrings(rightstr_result);

			if (rightstr_result.size() == rightstr.size()) {
				for (size_t i = 0; i < rightstr.size(); i++)
				{
					cout << rightstr[i] << ";" << endl;
					EXPECT_STREQ(rightstr_result[i].c_str(), rightstr[i].c_str());
				}
			}
			else {
				EXPECT_DOUBLE_EQ(rightstr_result.size(), rightstr.size());
			}

			//ImportFromJson OR ImportFromString
			JsonValue *export_js = nfile.ExportToJson();
			string str_result = nfile.ExportToString();
			nfile.ImportFromJson(export_js->AsObject());
			EXPECT_STREQ(nfile.GetFileName().c_str(), file_name.c_str());
			EXPECT_STREQ(nfile.GetFileExtension().c_str(), "nxl");

			nfile.ImportFromString(str_result);
			EXPECT_STREQ(nfile.GetFileName().c_str(), file_name.c_str());
			EXPECT_STREQ(nfile.GetFileExtension().c_str(), "nxl");

			nfile.Close();
		}
		else {
			EXPECT_FALSE(ret.GetCode());
		}
		cout << "Test Finished!" << endl;
	}
	catch (exception& e) {
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}


//Test RMNXL File from share
class RMNXLFileInfoShareTest : public::testing::TestWithParam<vector<string>> {};
TEST_P(RMNXLFileInfoShareTest, NXLFileInfoShareTXTTest) {
	try {
		vector<string> csv = GetParam();
		string rights_list = csv[3];
		string file_name = csv[2];
		string nxlFile_name = csv[2] + ".nxl";
		string recipients_list = csv[5];
		string comment = csv[6];
		string plain_file = plain_file_path + file_name;
		string nxl_file = nxl_file_path + nxlFile_name;
		std::string clientid = GenerateClientID();
		RMUser user = GetDefaultUser();
		RMTenant tenant = CreateDefaultTenant();
		NXLFMT::Obligations obs;
		NXLFMT::Rights rights;
		rights = getRightsValue(rights_list);
		vector<std::string> recipient = ReturnRecipient(recipients_list);
		RMRecipientList remove_recipients{ "jewelry.zhu@nextlabs.com" };
		RMTokenPool tokenpool = GetDefaultTokens();
		assert(tokenpool.size() != 0);
		RMToken token = tokenpool.pop();
		static RMNXLFile nfile("", "");
		RMActivityLog activity_log;
		RetValue  ret1 = user.ShareLocalFile(plain_file, nxl_file, recipient, obs, rights, token, comment, activity_log, nfile);
		if (ret1.GetCode() == 0) {
			//Check comments
			string comment_result = nfile.GetComments();
			EXPECT_STREQ(comment_result.c_str(), comment.c_str());
			//Open nxl file
			EXPECT_DOUBLE_EQ(1, nfile.Open());
			EXPECT_STREQ(nfile.GetFileName().c_str(), nxlFile_name.c_str());
			EXPECT_STREQ(nfile.GetFileExtension().c_str(), "nxl");
			cout << nfile.GetFileName() << endl;

			NXLFMT::Rights nxlright(nfile.GetNXLRights());
			std::vector<std::string> rightstr;
			nxlright.ToStrings(rightstr);

			const uint64_t rights1 = nfile.GetNXLRights();
			cout << "Rights: " << endl;
			cout << rights1 << endl;
			std::vector<std::string> rightstr_result;
			rights.ToStrings(rightstr_result);
			//Check rights
			if (rightstr.size() == rightstr_result.size()) {
				for (size_t i = 0; i < rightstr.size(); i++)
				{
					EXPECT_STREQ(rightstr[i].c_str(), rightstr_result[i].c_str());
				}
			}
			else {
				cout << "Rights issue!" << endl;
				EXPECT_EQ(rightstr.size(), rightstr_result.size());
			}
			//Check recipients
			RMRecipients rc = nfile.GetFileRecipients();
			vector<std::string> recipient_result = rc.GetRecipients();
			if (recipient_result.size() == recipient.size()) {
				for (int i=0;i<recipient_result.size();i++)
				{
					EXPECT_STREQ(recipient_result[i].c_str(), recipient[i].c_str());
				}
			}
			else{
				EXPECT_EQ(recipient_result.size(), recipient.size());
			}
		}
		else {
			EXPECT_FALSE(ret1.GetCode());
		}
		
		//HTTPRequest request = user.GetShareLocalFileQuery(user.GetDefaultMembership(), nfile);


		cout << "Share finished!" << endl;
	}
	catch (exception& e) {
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> vf2 = readCSV("ShareFile.csv");
INSTANTIATE_TEST_CASE_P(RMNXLFileInfoShareTestReturn, RMNXLFileInfoShareTest, testing::ValuesIn(vf2));


//Test file get token request
TEST(GetFileTokenTest, GetFileTokenTest) {
	try {
		RMUser	  user = GetDefaultUser();
		RMNXLFile nfile = RMNXLFile("D:\\workplace\\RMCCORE\\TestAPI\\TestGoogleTest\\TestGoogleTest\\plan file\\DefaultNXLFile-2018-04-18-02-46-30.txt.nxl");
		string user_id = to_string(user.GetUserID());
		string duid = nfile.GetDuid();
		string owner = nfile.GetOwnerID();
		string ag = "041C8A1E8980496A085DF87619A3793D33FAD836F6112022B937B03C245CD809ECF8554827B69D9EE7307CE8DBFAEAE3ECEC994C5BC788C1A96DA2C0D8FB51227B8A9D9AB8983456F2D9A74190FBDB30B4426892EAFCF5F5A9BAF5073B3A816C1A8B5B2C329896D4CC2C03D47E306D58EAD67DB0137BB75D67A2347B8B84C5A45B07CD511A7A67BE9ADD242F113E5792A7FB56A5DA98876BB2DCD83BDE646B3B9795E98C864E242642246FDF74445FFF376D17107BD99CE70FFBB939CBE4199D14E3A93C37B17588D3CC6E41B75A60E6873B8C2BE10088C5431EF497C1B18460807A7567F3C8266B17E06B1A9106E2598B6348BB2A059E8E53382418B84F7E87";
		HTTPRequest request =  user.GetFileTokenQuery(nfile);
		bool header_result = Check_HttpHeader(request, "application/json", user);
		EXPECT_TRUE(header_result);
		bool urlmethod_result = Check_HttpURL_Method(request, "POST", "/rs/token", user);
		EXPECT_TRUE(urlmethod_result);
		string body = "{\"parameters\":{\"userId\":\""+ user_id +"\",\"ticket\":\"8FA19388D635F47600716D99A0D5C611\",\"tenant\":\"skydrm.com\",\"owner\":\""+ owner +"\",\"agreement\":\""+ag+"\",\"duid\":\""+duid+"\",\"ml\":0}}";
		bool body_result = Check_Http_Body(request, body);
		EXPECT_TRUE(body_result);

		string response_str = ReadFromFile("GetFileToken.txt");
		//We need a class to import the response
		EXPECT_TRUE(0);
		cout << "Test Finished!" << endl;
	}
	catch (exception& e) {
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}


//Test RMNXLFile read TXT Normal file
TEST(RMNXLFileReadTest, RMNXLNormalTXTReadTest) {
	try {
		RMUser user = GetDefaultUser();

		const std::string nxlFile("RMSTokenTest.txt.nxl");
		const std::string txtFile(NXL_TEST_PLAIN_FILE);
		NXLFMT::File file;

		//EXPECT_TRUE(file.open(nxlFile, true));
		//assert(0 == memcmp(kTokenId, file.getTokenId().getId().data(), 16));

		//file.setTokenKey(std::vector<uint8_t>(kTokenValue, kTokenValue + 32));

		//std::ofstream ifs;
		//ifs.open(txtFile, ios_base::trunc | ios_base::out);
		//assert(ifs.is_open());
		//uint64_t contentLength = file.getContentLength();
		//uint64_t offset = 0;

		//cout << endl << "Open nxl file " << nxlFile << endl;
		//cout << "File content :" << endl;
		//bool beof = 0;
		//while (contentLength) {
		//	uint8_t buf[513];
		//	uint32_t bytesToRead = 512;
		//	memset(buf, 0, sizeof(buf));
		//	bytesToRead = file.read(offset, buf, bytesToRead);
		//	if (bytesToRead > contentLength)
		//		bytesToRead = (uint32_t)contentLength;
		//	ifs.write((char *)buf, bytesToRead);
		//	cout << buf;
		//	offset += bytesToRead;
		//	contentLength -= bytesToRead;
		//}
		//file.close();
		//ifs.close();


		////Protect file
		//RMNXLFile nfile = GetNXLFile("docx.docx");
		//nfile.Open();

		//std::ofstream ifs;
		//ifs.open(txtFile, ios_base::trunc | ios_base::out);
		//assert(ifs.is_open());
		//uint64_t offset = 0;

		////read file
		//bool beof = 0;
		//uint8_t buf[1024];
		//uint32_t bytesToRead = 1024;
		//memset(buf, 0, sizeof(buf));
		//bytesToRead = nfile.read(offset, buf, bytesToRead);

		//ifs.close();
		//EXPECT_EQ(bytesToRead, 736);
		//nfile.Close();
	}
	catch (exception& e) {
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

////Test RMNXLFile read TXT long content file
//TEST(RMNXLFileReadTest, RMNXLLongTXTReadTest) {
//	try {
//		string plain_file = plain_file_path + "ReadLongFile.txt";
//		string nxl_file = nxl_file_path + "ReadLongFile.txt.nxl";
//		uint64_t contentLength = 20828;
//		std::string clientid = GenerateClientID();
//		RMUser user = GetDefaultUser();
//		RMTenant tenant = CreateDefaultTenant();
//		NXLFMT::Obligations obs;
//		NXLFMT::Rights rights(BUILTIN_RIGHT_VIEW | BUILTIN_RIGHT_EDIT);
//		
//
//		//Protect file
//		RMNXLFile nfile = user.ProtectLocalFile(plain_file, nxl_file, obs, rights, GetDefaultTokens());
//		nfile.Open();
//
//		uint64_t offset = 0;
//		uint8_t buf[512];
//		uint32_t bytesToRead = 512;
//
//		std::ofstream ifs;
//		ifs.open(plain_file, ios_base::trunc | ios_base::out);
//		assert(ifs.is_open());
//
//		cout << "File content :" << endl;
//		bool beof = 0;
//		while (contentLength) {
//			uint8_t buf[512];
//			uint32_t bytesToRead = 512;
//			memset(buf, 0, sizeof(buf));
//			bytesToRead = nfile.read(offset, buf, bytesToRead);
//			if (bytesToRead > contentLength)
//				bytesToRead = (uint32_t)contentLength;
//			ifs.write((char *)buf, bytesToRead);
//			cout << buf;
//			offset += bytesToRead;
//			contentLength -= bytesToRead;
//		}
//		EXPECT_EQ(nfile.read(offset, buf, bytesToRead), 43924);
//		nfile.Close();
//	}
//	catch (exception& e) {
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}


//Login Logout Normal Test
TEST(UserLogoutTest,LogoutTest) {
	try{
		//User login
		RMUser user;
		HTTPRequest http_login = user.GetUserLoginURL();
	    user = GetDefaultUser();
		cout << "Is Login: " << user.IsLogin() << endl;
		EXPECT_EQ(user.IsLogin(),1);

		JsonDocument doc;
		int err_code = 0;
		size_t err_pos = 0;
		const std::string jsonstr = ReadFromFile("Logout.txt");

		HTTPRequest  http = user.GetUserLogoutQuery();
		if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
			throw;
		}

		JsonValue *root = doc.GetRoot();
		if (!(NULL != root && root->IsObject())) {
			throw;
		}
		
		//User logout
		RetValue  rn_value = user.Logout(root->AsObject());
		EXPECT_EQ(rn_value.GetCode(), 0);
		cout << "After logout: " << user.IsLogin();
		EXPECT_EQ(user.IsLogin(), 0);
	}
	catch (exception& e) {
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Logout error respond test
class UserLogoutErrorTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(UserLogoutErrorTest, LogoutErrorTest) {
	RMCCORE::JsonDocument doc;
	RMCCORE::JsonValue* root;
	int err_code = 0;
	size_t err_pos = 0;
	int base_code = 61440;
	int code;
	string message;
	try {
		string file_name;
		vector<string> csv = GetParam();
		file_name = csv[2];
		string respond_str = ReadFromFile(file_name);
		std::cout << "file length is: " << respond_str.length() << endl;
		doc.LoadJsonString(respond_str, &err_code, &err_pos);
		root = doc.GetRoot();
		RMUser user = GetDefaultUser();
		EXPECT_EQ(user.IsLogin(),1);

		//User Logout Fail
		RetValue r_v = user.Logout(root ->AsObject());
		Json::Value jroot_result;
		jroot_result = ParseJsonFile(file_name);

		EXPECT_EQ(user.IsLogin(),1);
		//Read root node: status and message
		code = jroot_result["statusCode"].asInt();
		message = jroot_result["message"].asString();
		std::cout << "return code is: " << r_v.GetCode() << endl;
		std::cout << "return message is: " << r_v.GetMessage() << endl;
		EXPECT_EQ((base_code + code), r_v.GetCode());
		EXPECT_STREQ(message.c_str(), r_v.GetMessage());

		std::cout << "user export is: " << user.ExportToString() << endl;
	}
	catch (exception& e) {
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> csv_logout = readCSV("User_LogoutError.csv");
INSTANTIATE_TEST_CASE_P(UserLogoutErrorTestRturn, UserLogoutErrorTest, testing::ValuesIn(csv_logout));



*/