
#include "stdafx.h"
#include "rmccore\restful\rmactivitylog.h"
#include "Header_Token.h"
#include "Header_File.h"
#include "Header_SystemPara.h"

using namespace RMCCORE;
using namespace std;


RMSystemPara GenerateSysPara() {
	string product_name, product_path, product_publisher, device_id, client_id;
	RMSystemPara sys_para;
	vector<vector<string>> para = readCSV("SysParaProductInfoTest.csv");
	RMPlatformID platform_id = RPWindows7;
	RMProcessorArch processor = RPAWindowsX86;

	product_name = para[0][2];
	product_path = para[0][6];
	product_publisher = para[0][7];
	device_id = para[0][8];
	client_id = para[0][10];
	sys_para.SetClientID(client_id);
	sys_para.SetDeviceID(device_id);
	sys_para.SetPlatformID(platform_id);
	sys_para.SetProcessor(processor);
	RMProduct product(product_name, 111, 222, 4444);
	sys_para.SetProduct(product);
	return sys_para;
}

void GenerateLog(RMUser& user, RMActivityLog& activity_log, RMNXLFile& rmnxlfile, string fileName, string rights_str = "view|print|share|download|edit|watermark", string comment = "123123", vector<std::string> recipient = {}) {

	uint64_t accessTime = 0;
	string plain_file = default_plan_file_path + fileName;
	string nxl_file = default_generate_file_path + fileName + ".nxl";
	RMCCORE::JsonDocument doc;
	RMCCORE::JsonValue* root;
	int err_code = 0;
	size_t err_pos = 0;

	NXLFMT::Obligations obs;
	NXLFMT::Rights rights;
	CONDITION::Expiry expiry(CONDITION::ExpiryType::NEVEREXPIRE);
	rights = getRightsValue(rights_str);
	Watermark::Rotation ration = Watermark::NOROTATION;

	Watermark w_m("$(User)$(Break)$(Date) $(Time)", "", "", 20, 2, ration, 1);
	obs.push_back(w_m);
	NXLAttributes nxl_attr;
	nxl_attr.rights = &rights;
	nxl_attr.obs = &obs;
	nxl_attr.tags = "{}";
	nxl_attr.expiry = &expiry;

	RMTokenPool tokenpool = GetDefaultTokens();
	assert(tokenpool.size() != 0);
	RMToken token = tokenpool.pop();

	//Protect a file generate activity_log
	static RMNXLFile nfile("", "");
	if (recipient.size() == 0)
		RetValue  ret = user.ProtectLocalFile(plain_file, nxl_file, nxl_attr, token, activity_log, rmnxlfile);
	else
		RetValue  ret = user.ShareLocalFile(plain_file, nxl_file, nxl_attr, recipient, token, comment, activity_log, nfile);
}

void CheckLogRequest(RMUser user, HTTPRequest request, string accessType, string header, string method, string url, string body) {
	EXPECT_STREQ(request.GetAcceptTypes()[0].c_str(), accessType.c_str());
	//Check http header
	bool header_result = Check_HttpHeader(request, header, user);
	EXPECT_TRUE(header_result);
	//Check URL and Method
	bool urlmethod_result = Check_HttpURL_Method(request, method, url, user);
	EXPECT_TRUE(urlmethod_result);
	//Check request body
	bool body_result = Check_Http_Body(request, body);
	EXPECT_TRUE(body_result);

	EXPECT_FALSE(request.IsUpload());
}

void CheckLogContent(string log_str, string expect_log) {
	vector<string> log_vec;
	vector<string> log_expect;
	splitString(log_str, log_vec, ",");
	splitString(expect_log, log_expect, ",");

	if (log_expect.size() == log_vec.size()) {
		for (int i = 0; i < log_expect.size(); i++) {
			if (i == 14)
				continue;
			else
			{
				EXPECT_STREQ(log_vec[i].c_str(), log_expect[i].c_str());
			}
		}
	}
}


//Test constructor, Serialize, RMActivityLogOperation
class ActivityLogConstructorTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(ActivityLogConstructorTest, Constructor1Test) {
	std::string nxlduid, fileownerid, userid, repoId, fileid, fileName, filePath, extraData;
	uint64_t accessTime = 0;
	try {
		//vector<string> case_para = GetParam(); 
		//cout << "Para:  " <<  case_para[1] << endl;
		//nxlduid = case_para[2];
		//fileownerid = case_para[3];
		//userid = case_para[4];
		//repoId = case_para[5];
		//fileid = case_para[6];
		//fileName = case_para[7];
		//filePath = case_para[8];
		//extraData = case_para[9];
		//RMActivityLogOperation operation = RLOProtect;
		//RMActivityLogResult access_result = RLRDenied;
		//RMActivityLogAccountType account_type = RLATPersonal;
		//for (operation = RLOProtect;  operation < RLODelete;  operation = (RMActivityLogOperation)(operation + 1))
		//{
		//	RMSystemPara sy_pa = GenerateSysPara();
		//	RMActivityLog activity_log(nxlduid, fileownerid, userid, operation, sy_pa, repoId, fileid, fileName, filePath, access_result, extraData, accessTime, account_type);
		//	//cout << "Generate syspara" << GenerateSysPara().ExportToString() << endl;
		//	cout << " Serialize:  "  << activity_log.Serialize() << endl;
		//	ostringstream oss;
		//	oss << "\"" << nxlduid << "\",\""<< fileownerid<< "\",\""<< userid <<"\",\""<<operation <<"\",\"" << sy_pa.GetDeviceID()<< "\",\""<< sy_pa.GetPlatformID()<< "\",\""<<repoId<<"\",\"" << fileid<< "\",\""<<fileName<< "\",\""<<filePath<< "\",\"" << sy_pa.GetAppName()<<"\",\""<< sy_pa.GetAppPath()<< "\",\"" << sy_pa.GetAppPublisherName()<< "\",\"" << access_result <<"\",\""<< accessTime <<"\",\""<< extraData<< "\",\""<<account_type<<"\""<< endl;
		//	cout << oss.str() << endl;
		//	string str = oss.str();
		//	str = str.replace(str.find("\n"), 1, "");
		//	EXPECT_STREQ(activity_log.Serialize().c_str(), str.c_str());
		//	cout << "Test finished!"  << endl;
		//}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> act_para = readCSV(default_file_path + "ActivityLogTest.csv");
INSTANTIATE_TEST_CASE_P(ActivityLogConstructorTestReturn, ActivityLogConstructorTest, testing::ValuesIn(act_para));

//Test upload log after protect file,RMActivityLog Interface, RMLogPool Interface ,size,GetUploadActivitylogQuery
class UploadActivityLogTest :public::testing::TestWithParam<vector<string>> {};
TEST(UploadActivityLogTest, Upload) {
	try {
		RMUser user = GetDefaultUser();
		string fileName = default_file_path + "TestLog.txt";
		uint64_t accessTime = 0;
		string plain_file = default_plan_file_path + fileName;
		string nxl_file = default_generate_file_path + fileName + ".nxl";
		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;
		int err_code = 0;
		size_t err_pos = 0;
		NXLFMT::Obligations obs;
		NXLFMT::Rights rights(BUILTIN_RIGHT_VIEW | BUILTIN_RIGHT_EDIT | BUILTIN_RIGHT_PRINT);

		RMActivityLog activity_log, activity_log_jsstr;
		
		RMTokenPool tokenpool = GetDefaultTokens();
		assert(tokenpool.size() != 0);
		RMToken token = tokenpool.pop();

		//Protect a file generate activity_log
		static RMNXLFile nfile("", "");
		//For debug RetValue  ret = user.ProtectLocalFile(plain_file, nxl_file, obs, rights, token, activity_log, nfile);
		//RMNXLFile nfile = user.ProtectLocalFile(plain_file, nxl_file, obs, rights, token, activity_log);

		RMLogPool *log_pool = new RMLogPool();
		RMLogPool *log_pool1 = new RMLogPool();
		log_pool1->AddLog(activity_log);
		log_pool1->AddLog(activity_log);
		JsonString *json_str = activity_log.ExportToJson();
		EXPECT_FALSE( json_str->IsNull());
		log_pool->AddLog(activity_log);
		log_pool->AddLog(activity_log);
		log_pool->AddLog(activity_log);
		EXPECT_EQ(log_pool->size(),3);
		//Check http reuest,upload all log
		HTTPRequest request = user.GetUploadActivitylogQuery(log_pool,2);
		EXPECT_STREQ(request.GetAcceptTypes()[0].c_str(),"application/json");
		//Check http header
		bool header_result = Check_HttpHeader(request,"text/csv",user);
		EXPECT_TRUE(header_result);
		//Check URL and Method
		bool urlmethod_result = Check_HttpURL_Method(request,"PUT","/rs/log/v2/activity",user);
		EXPECT_TRUE(urlmethod_result);
		//Check request body
		bool body_result = Check_Http_Body(request, log_pool1->ExportLogStringForUpload());
		EXPECT_TRUE(body_result);

		EXPECT_FALSE(request.IsUpload());
		//Import RMS response
		string response_str = ReadFromFile(default_file_path + "logResponse.txt");
		doc.LoadJsonString(response_str, &err_code, &err_pos);
		root = doc.GetRoot();
		RetValue response_result = log_pool->ImportFromRMSResponse(response_str);
		RetValue response_result1 = log_pool->ImportFromRMSResponse(root->AsObject());

		EXPECT_EQ(log_pool->size(), 1);
		log_pool->Reset();
		for (int i = 0; i < 5;i++) {
			log_pool->AddLog(activity_log);
		}
		EXPECT_EQ(log_pool->size(), 5); 
		//Test ExportToString
		string pool_str = 	log_pool->ExportToString();
		ExportToFile("activityLog.txt", pool_str);
		Json::Value jroot;
		jroot = ParseJsonFile(default_generate_file_path + "ex_activityLog.txt");
		int log_num = jroot["LogNum"].asInt();
		
		EXPECT_EQ(log_num, log_pool->size());
		//Test Export to Json
		JsonValue *j_value = log_pool->ExportToJson();
		RMLogPool *log_pool_ex = new RMLogPool();
		
		RetValue Import_result = log_pool_ex->ImportFromJson(j_value->AsObject());
		EXPECT_FALSE(Import_result.GetCode());
		RetValue Import_result1 = log_pool_ex->ImportFromString(pool_str);
		EXPECT_FALSE(Import_result1.GetCode());
		EXPECT_EQ(log_pool_ex->size(), 10);
		log_pool->Reset();
		EXPECT_EQ(log_pool->size(), 0);
		cout << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//HttpCode Test
class UploadActivityLogHttpCodeTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(UploadActivityLogHttpCodeTest,HttpCodeTest) {
	try {
		RMLogPool log_pool;
		RMActivityLog activity_log;
		int base_code = 61440;
		string statu_code, message;
		vector<string> case_para = GetParam();
		static RMNXLFile nfile("", "");
		RMUser	  user = GetDefaultUser();
		string fileName = default_file_path + case_para[2];;
		uint64_t accessTime = 0;
		string plain_file = default_plan_file_path + fileName;
		string nxl_file = default_generate_file_path + fileName + ".nxl";
		NXLFMT::Obligations obs;
		RMTokenPool tokenpool = GetDefaultTokens();
		assert(tokenpool.size() != 0);
		RMToken token = tokenpool.pop();
		NXLFMT::Rights rights(BUILTIN_RIGHT_VIEW | BUILTIN_RIGHT_EDIT | BUILTIN_RIGHT_PRINT);
		//For debugRetValue  ret = user.ProtectLocalFile(plain_file, nxl_file, obs, rights, token, activity_log, nfile);
		
		Json::Value jroot;
		jroot = ParseJsonFile(fileName);
		statu_code = jroot["statusCode"].asString();
		message = jroot["message"].asString();
		log_pool.AddLog(activity_log);
		string upload_str = ReadFromFile(fileName);
		RetValue ret_log = log_pool.ImportFromRMSResponse(upload_str);
		int code = string2Int(statu_code);
		int code_result = code + base_code;
		EXPECT_EQ(ret_log.GetCode(), code_result);   
		EXPECT_STREQ(message.c_str(), ret_log.GetMessage());
		cout << "Test Finished!" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> act_code = readCSV(default_file_path + "UploadLog_HttpCode.csv");
INSTANTIATE_TEST_CASE_P(UploadActivityLogHttpCodeTestReturn, UploadActivityLogHttpCodeTest, testing::ValuesIn(act_code));

//Test  RMNXLFileLogs Interface , GetNXLFileActivitylogQuery
TEST(RMNXLFileLogsTest, RMNXLFileLogsLog) {
	try {
		RMUser	  user = GetDefaultUser();
		RMNXLFile nfile = GetNXLFile("TestLog.txt");
		int start = 10;
		int count = 20;
		string count_str = "20";
		string start_str = "10";
		string search_filter;
		string searchText = "&searchText==123123test";
		string order_str;
		RMLogSearchField search = RLSFDeviceId;
		RMLogOrderBy orderby;
		string reverse_str;
		bool reverse;
		
		for (search = RLSFEmpty; search < RLSFDeviceId; search = (RMLogSearchField)(search + 1))
		{
			switch (search) {
			case RLSFDeviceId: search_filter = "&searchField=deviceId";   searchText = "device 123";    orderby = RMLOAccessTime; reverse = false;
				break;
			case RLSFOperation: search_filter = "&searchField=operation";  searchText = "protect";  orderby = RMLOAccessResult; reverse = true;
				break;
			case RLSFEmail: search_filter = "&searchField=email";  searchText = "jewelry.zhu@nextlabs.com"; orderby = RMLOAccessTime; reverse = false;
				break;
			case RLSFEmpty: search_filter ="" ;  searchText = ""; orderby = RMLOAccessResult; reverse = true;
				break;
			}
			switch (orderby) {
			case RMLOAccessTime: order_str = "accessTime";
				break;
			case RMLOAccessResult: order_str = "accessResult";
				break;
			}
			switch (reverse) {
			case true: reverse_str = "";
				break;
			case false: reverse_str = "&orderByReverse=false";
				break;
			}


			HTTPRequest request = user.GetNXLFileActivitylogQuery(nfile, start, count, search, searchText, orderby, reverse);
			// Check http header
			bool header_result = Check_HttpHeader(request, "application/json", user);
			EXPECT_TRUE(header_result);
			//Check URL and Method
			string udid = nfile.GetDuid();
			if (search_filter != "") {
				searchText = "&searchText=" + searchText;
			}
			bool urlmethod_result = Check_HttpURL_Method(request, "GET", "/rs/log/v2/activity/" + udid + "?start=" + start_str + "&count=" + count_str + search_filter + searchText + "&orderBy=" + order_str + reverse_str, user);
			EXPECT_TRUE(urlmethod_result);
		}

		//Import response file to logs
		RMNXLFileLogs  file_logs;
		JsonDocument doc;
		int err_code = 0;
		size_t err_pos = 0;
		RMCCORE::JsonValue* root;
		const std::string jsonstr = ReadFromFile(default_file_path + "GetLog_response.txt");
		doc.LoadJsonString(jsonstr, &err_code, &err_pos);
		root = doc.GetRoot();
		EXPECT_EQ(file_logs.size(), 00);
		RetValue retv = file_logs.ImportFromRMSResponse(root ->AsObject());

		if (retv.GetCode() == 0) {
			Json::Value jroot;
			jroot = ParseJsonFile(default_file_path + "GetLog_response.txt");
			string orig_duid = jroot["results"]["data"]["duid"].asString();
			string orig_name = jroot["results"]["data"]["name"].asString();
			string orig_totalnum = jroot["results"]["totalCount"].asString();
			int orig_total_num = string2Int(orig_totalnum);
			size_t size = file_logs.size();

			const RMNXLFileActivity *activity_log = file_logs.GetAt(0);
			string duid = file_logs.GetDUID();
			string name = file_logs.GetFileName();
			size_t total_num = file_logs.GetTotalLogNum();

			EXPECT_STREQ(duid.c_str(), file_logs.GetDUID().c_str());
			EXPECT_STREQ(name.c_str(), file_logs.GetFileName().c_str());
			EXPECT_EQ(orig_total_num, file_logs.GetTotalLogNum());
			EXPECT_EQ(file_logs.size(),20);

			string ext_str = file_logs.ExportToString();
		}
		else {
			cout << "Import fail!";
			EXPECT_TRUE(false);
		}

		JsonValue * jv = file_logs.ExportToJson();
		string j_str = file_logs.ExportToString();
		RMNXLFileLogs  file_logs1;
		RetValue ret1 = file_logs1.ImportFromRMSResponse(jsonstr);
		RetValue ret2 = file_logs1.ImportFromJson(jv ->AsObject());
		RetValue ret3 = file_logs1.ImportFromString(j_str);
		EXPECT_EQ(ret1.GetCode(),0);
		EXPECT_EQ(ret2.GetCode(), 0);
		EXPECT_EQ(ret3.GetCode(), 0);
		
		cout << "Test Finished!" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Test RMNXLFileActivity Interface
TEST(RMNXLFileActivityTest, RMNXLFileActivityTest) {
	try {
		RMNXLFileLogs file_logs;
		RMNXLFileActivity file_activity;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
