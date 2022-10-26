//
//  TokenTest.cpp

//
//  Created by Jewelry Zhu on 1/15/18.
//  Copyright ?2018 NextLabs, Inc. All rights reserved.
//

#include "stdafx.h"
#include "RMCCORE\restful\rmtoken.h"
#include "RMCCORE\restful\rmuser.h"
#include "RMCCORE\restful\rmtenant.h"

#include "Header_Tenant.h"

using namespace RMCCORE;
using namespace std;

#define DEFAULT_TOKEN_POOL_FILENAME		"successtoken.json"

RMTokenPool & GetDefaultTokens()
{
	static RMTokenPool tokens;
	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile(DEFAULT_TOKEN_POOL_FILENAME);
	if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
		throw;
	}

	JsonValue *root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		throw;
	}

	tokens.ImportFromRMSResponse(root->AsObject());
	//cout << "Token string: " << tokens.ExportToString() << endl;
	return tokens;
}

// Test Pattern to test valid user with different input,IsInitialized
class ValidTokenParamTest :public::testing::TestWithParam<string> {};
TEST_P(ValidTokenParamTest, ValidTokenInfo)
{
	//get parameter of file name, type is string
	string para = GetParam();

	// create a user class first
	RMTokenPool tpool;
	string duid_expect, token_expect;
	string ml;

	// using RMCCORE lib to read and load json file, then call GetName(), GetUserID(), GetEmail(), GetIdpType() to return userName, userId, email, idpType.
	RMCCORE::JsonDocument doc;
	RMCCORE::JsonValue* root;
	int err_code = 0;
	size_t err_pos = 0;

	string tokenstr = ReadFromFile(para);

	std::cout << "tokenstr length is: " << tokenstr.length() << endl;
	doc.LoadJsonString(tokenstr, &err_code, &err_pos);
	root = doc.GetRoot();
	RetValue ret = tpool.ImportFromRMSResponse(root->AsObject());

	if (ret.GetCode() != 0) {
		EXPECT_EQ(1,0);
	}
	// using 3rd party Jsoncpp to read and get json value
	Json::Value jroot;
	Json::Value::Members members;
	Json::Value::Members::iterator iter;

	jroot = ParseJsonFile(para);

	// Read array node
	std::cout << "Size of tokens: " << jroot["results"]["tokens"].size() << endl;
	cout << "Size of tokens from RMCCORE: " << tpool.size() << endl;
	EXPECT_EQ(jroot["results"]["tokens"].size(), tpool.size());
	ml = jroot["results"]["ml"].asString();

	//ExportToFile(para, tpool.ExportToString());
	string test = tpool.ExportToString();
	JsonValue* j_v = tpool.ExportToJson();
	tpool.ImportFromJson(j_v ->AsObject());
	string ee = tpool.ExportToString();
	tpool.ImportFromString(test);
	string tt = tpool.ExportToString();
	RMToken token_notIni;
	bool tok_init = token_notIni.IsInitialized();
	EXPECT_EQ(tok_init, 0);
	// get all members, duid and token from tokens dictionary.
	members = jroot["results"]["tokens"].getMemberNames();
	for (int i = 0; i < jroot["results"]["tokens"].size(); i++) {
		RMToken token = tpool.pop();
		//Test IsInitialized
		bool token_init = token.IsInitialized();
		EXPECT_EQ(token_init,1);
		//cout << "Size of tokens from RMCCORE: " << tpool.size() << endl;
		cout << token.GetDUID() << endl;
		cout << token.GetKey() << endl;
		cout << token.GetMaintainLevel() << endl;

		// get duid and token from tokens dictionary
		for (iter = members.begin(); iter != members.end(); ++iter) {
			duid_expect = *iter;
			token_expect = jroot["results"]["tokens"][duid_expect].asString();
			char* end;
			int i = static_cast<int>(strtol(ml.c_str(), &end, 10));
			// compare token size, token duid, token value 
			if (token.GetDUID() == duid_expect)
			{
				EXPECT_STREQ(duid_expect.c_str(), token.GetDUID().c_str());
				EXPECT_STREQ(token_expect.c_str(), token.GetKey().c_str());
				if (i == token.GetMaintainLevel()) {
					EXPECT_EQ(1,1);
				}
				else { EXPECT_EQ(0, 1); }
				break;
			}
		}
	}
	// Check status is same
	EXPECT_EQ(0, ret.GetCode());

	//EXPECT_STREQ(jsonstr.c_str(), user.ExportToJson());
	cout << "Test finished." << endl;
}
INSTANTIATE_TEST_CASE_P(ValidTokenReturn, ValidTokenParamTest, testing::Values("successtoken.json", "Case1_Token_with2count.json", "Case5_Token_with100count.json", "Case8_Token_withErrorCount.json"));

// Test constructor with encryption id, key and maintenance level
TEST(DiffconstructorTokenParamTest, Constructor1) {
	std::string key = "67D3760E394F0604837F12C0ADA2B629A578CA58D1DD98983DC37BA1BFC00339";
	std::string duid = "1095CAED14E8C6A1889E47F34AA158C3";
	unsigned int ml = 0;
	try {
		RMToken token(duid, key, ml);
		cout << "DUID: " << token.GetDUID() << endl;
		cout << "KEY: " << token.GetKey() << endl;
		cout << "ML: " << token.GetMaintainLevel() << endl;
		EXPECT_STREQ(duid.c_str(), token.GetDUID().c_str());
		EXPECT_STREQ(key.c_str(), token.GetKey().c_str());
		EXPECT_EQ(ml, token.GetMaintainLevel());
		
		cout << "Test finished." << endl;
	}
	catch (...) {
		cout << "Exception!!! " << endl;
		throw;
	}
}

// Test constructor with json and maintenance level
TEST(DiffconstructorTokenParamTest, Constructor2and3) {
	std::string key = "67D3760E394F0604837F12C0ADA2B629A578CA58D1DD98983DC37BA1BFC00340";
	std::string duid = "1095CAED14E8C6A1889E47F34AA158C4";
	unsigned int ml = 1;

	//2
	RMToken token(duid, key, ml);
	//3
	RMToken token2(token.ExportToJson()->AsObject());
	cout << "DUID: " << token.GetDUID() << endl;
	cout << "KEY: " << token.GetKey() << endl;
	cout << "ML: " << token.GetMaintainLevel() << endl;
	cout << "DUID: " << token2.GetDUID() << endl;
	cout << "KEY: " << token2.GetKey() << endl;
	cout << "ML: " << token2.GetMaintainLevel() << endl;
	JsonValue* js_v = token.ExportToJson();
	EXPECT_STREQ(duid.c_str(), token.GetDUID().c_str());
	EXPECT_STREQ(key.c_str(), token.GetKey().c_str());
	EXPECT_EQ(ml, token.GetMaintainLevel());
	EXPECT_STREQ(duid.c_str(), token2.GetDUID().c_str());
	EXPECT_STREQ(key.c_str(), token2.GetKey().c_str());
	EXPECT_EQ(ml, token2.GetMaintainLevel());

	cout << "Test finished." << endl;
}

//Error para
class ErrorTokenParamTest :public::testing::TestWithParam<string> {};
TEST_P(ErrorTokenParamTest, ErrorPara) {
	try {
		//get parameter of file name, type is string
		string para = GetParam();
		int code;
		int base_code = 61440;
		string message;
		string expect_result = "{\"Tokens\":[]}";
		// create a user class first
		RMTokenPool tpool;
		string duid_expect, token_expect;

		// using RMCCORE lib to read and load json file, then call GetName(), GetUserID(), GetEmail(), GetIdpType() to return userName, userId, email, idpType.
		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;
		int err_code = 0;
		size_t err_pos = 0;

		string tokenstr = ReadFromFile(para);

		std::cout << "tokenstr length is: " << tokenstr.length() << endl;
		doc.LoadJsonString(tokenstr, &err_code, &err_pos);
		root = doc.GetRoot();

		Json::Value jroot;
		jroot = ParseJsonFile(para);

		//Read root node: status and message
		code = jroot["statusCode"].asInt();
		message = jroot["message"].asString();

		RetValue retur_value =  tpool.ImportFromRMSResponse(root->AsObject());
		std::cout << "return code is: " << retur_value.GetCode() << endl;  
		std::cout << "return message is: " << retur_value.GetMessage() << endl;
		std::cout << "tpool exported to string is: " << tpool.ExportToString() << endl;
		EXPECT_EQ((base_code + code), retur_value.GetCode());
		EXPECT_STREQ(message.c_str(), retur_value.GetMessage());
		EXPECT_STREQ(expect_result.c_str(), tpool.ExportToString().c_str());
		ExportToFile(para, tpool.ExportToString());
	}
	catch (...) {	
		cout << "Exception " << endl;
		throw;
	}
}
INSTANTIATE_TEST_CASE_P(ErrorTokenParamReturn, ErrorTokenParamTest, testing::Values("Case9_Token_withEmptyAgreement.json", "Case3_Token_withErrorMembership.json", "Case10_Token_withEmptyUserId.json","Case2_Token_withErrorUserid.json"));

class TokenSpecailParaTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(TokenSpecailParaTest,SpecailParaTest) {
	string file_name;
	int code;
	try {
		vector<string> csv = GetParam();
		string expect_udid = csv[3];
		string expect_key = csv[4];
		int expect_ml = atoi(csv[5].c_str());
		RMTokenPool tpool;
		RMToken token;
		string duid_expect, token_expect;
		file_name = csv[2];

		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;
		int err_code = 0;
		size_t err_pos = 0;
		cout << "Case name: " + csv[1] << endl;
		string tokenstr = ReadFromFile(file_name);

		std::cout << "tokenstr length is: " << tokenstr.length() << endl;

		if (!doc.LoadJsonString(tokenstr, &err_code, &err_pos))
		{
			EXPECT_EQ(0, 1);
		}
		root = doc.GetRoot();

		RetValue retur_value = tpool.ImportFromRMSResponse(root->AsObject());
		
		if (!retur_value.GetCode())
		{
			cout << "Export string: " << tpool.ExportToString() << endl;
			
			cout << "code is: " << retur_value.GetCode() << endl;
			cout << "message: " << retur_value.GetMessage() << endl;
			token = tpool.pop();
			EXPECT_STREQ(token.GetDUID().c_str(),expect_udid.c_str());
			EXPECT_STREQ(token.GetKey().c_str(),expect_key.c_str());
			EXPECT_EQ(token.GetMaintainLevel(),expect_ml);
			cout << "UDID is: " << token.GetDUID() << endl;
			cout << "Public Key is: " << token.GetKey() << endl;
			cout << "MaintainLevel is: " << token.GetMaintainLevel() << endl;
		}
		else {
			EXPECT_EQ(retur_value.GetCode(), 13);
		}
		cout << "----Finish----- " << endl;
	}
	catch (...) {
		cout << "Exception" << endl;
		throw;
	}
}
vector<vector<string>> vf_specail = readCSV("TokenSpecailParaTest.csv");
INSTANTIATE_TEST_CASE_P(TokenSpecailParaTestReturn, TokenSpecailParaTest, testing::ValuesIn(vf_specail));

//Empty token
TEST(DiffconstructorTokenParamTest, EmptyToken) {
	std::string key = "";
	std::string duid = "";
	try {
		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;
		int err_code = 0;
		size_t err_pos = 0;

		string jsonstr = ReadFromFile("Token_emptyToken.txt");
		doc.LoadJsonString(jsonstr, &err_code, &err_pos);
		root = doc.GetRoot();
		RMToken token(root->AsObject());
		cout << "DUID: " << token.GetDUID() << endl;
		cout << "KEY: " << token.GetKey() << endl;
		cout << "ML: " << token.GetMaintainLevel() << endl;
		EXPECT_STREQ(duid.c_str(), token.GetDUID().c_str());
		EXPECT_STREQ(key.c_str(), token.GetKey().c_str());
		if (token.GetMaintainLevel() == 0) {
			EXPECT_EQ(0,0);
		}
		else {
			EXPECT_EQ(1, 0);
		}

		cout << "Test finished." << endl;
	}
	catch (...) {
		cout << "Exception " << endl;
		throw;
	}
}

//To pop empty pool
TEST(TokenPoolParamTest, PopTest) {
	std::string key = "";
	std::string duid = "";
	try {
		RMTokenPool tpool;
		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;
		int err_code = 0;
		size_t err_pos = 0;

		// RMToken to get all tokens
		//string tokenstr = ReadFromFile("Case1_Token_with2count.json");
		string tokenstr = ReadFromFile("Case1_Token_with2count.json");

		std::cout << "tokenstr length is: " << tokenstr.length() << endl;
		doc.LoadJsonString(tokenstr, &err_code, &err_pos);
		root = doc.GetRoot();
		RetValue ret = tpool.ImportFromRMSResponse(root->AsObject());

		Json::Value jroot;
		Json::Value::Members members;
		Json::Value::Members::iterator iter;

		jroot = ParseJsonFile("Case1_Token_with2count.json");
		//jroot = ParseJsonFile("Case1_Token_with2count.json");

		// Read array node
		std::cout << "Size of tokens: " << jroot["results"]["tokens"].size() << endl;
		cout << "Size of tokens from RMCCORE: " << tpool.size() << endl;
		EXPECT_EQ(jroot["results"]["tokens"].size(), tpool.size());

		RMToken token = tpool.pop();
		cout << "Tokens from RMCCORE: " << token.GetKey() << endl;
		RMToken token2 = tpool.pop();
		cout << "Tokens2 from RMCCORE: " << token2.GetKey() << endl;
		RMToken token3 = tpool.pop();
		cout << "Tokens3 from RMCCORE: " << endl;
		token.ExportToJson();
	}
	catch(...) {
		cout << "Exception "  << endl;
		throw;
	}
}
