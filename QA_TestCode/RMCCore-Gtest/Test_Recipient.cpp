/*
//
//  Created by Jewelry Zhu on 2/07/18.
//  Copyright @2018 NextLabs, Inc. All rights reserved.
//
#include "stdafx.h"
#include "RMCCORE\restful\rmrecipients.h"
#include "RMCCORE\restful\rmtoken.h"
#include "RMCCORE\restful\rmuser.h"
#include "RMCCORE\restful\rmtenant.h"
#include "RMCCORE\restful\rmcertificate.h"

#include "Header_Tenant.h"
#include "Header_Token.h"
#include "Header_File.h"

using namespace RMCCORE;
using namespace std;

const string plain_file_path = "D:\\workplace\\RMCCORE\\TestAPI\\TestGoogleTest\\TestGoogleTest\\plan file\\";
const string nxl_file_path = "D:\\workplace\\RMCCORE\\TestAPI\\TestGoogleTest\\TestGoogleTest\\Generate File\\";

//AddRecipientListTest
class RecipientListAddTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(RecipientListAddTest, AddRecipientTest) {
	string file_name;
	RMRecipientList list{ "test@test.com", "test1@test.com", "test2@test.com", "test2@nextlabs.com" };
	RMRecipientList addlist{ "test@test.com", "testAdd1@nextlabs.com", "testAdd2@nextlabs.com" };
	RMRecipientList removelist{ "testRemove1@test.com", "testRemove2@nextlabs.com", "testRemove3@nextlabs.com" };
	RMRecipientList empty_list;
	try {
		vector<string> csv = GetParam();
		file_name = csv[2];
		RMRecipients recipient;		

		//Add new recepients
		recipient.AddRecipients(list);
		cout << "AddRecipients Need RMS update: " << recipient.NeedUpdateRMS() << endl;
		cout << "AddRecipients add size: " << recipient.GetAddRecipients().size() << endl;
		cout << "AddRecipients Export string: " << recipient.ExportToString() << endl;
		EXPECT_EQ(1, recipient.NeedUpdateRMS());
		EXPECT_EQ(4, recipient.GetAddRecipients().size());
		std::vector<std::string> vc = recipient.GetAddRecipients();
		if(vc.size() == list.size()) {
			for (int i = 0; i<vc.size() ; i++)
			{
				EXPECT_STREQ(list[i].c_str(), vc[i].c_str());
			}
		}
		else {
			cout << "Count of recipients is not correct!(Add new recepients)";
			EXPECT_EQ(vc.size(), list.size());
			
		}

		//ImportFromRMSResponse
		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;
		Json::Value jroot;
		jroot = ParseJsonFile(file_name);

		int err_code = 0;
		size_t err_pos = 0;
		string tokenstr = ReadFromFile(file_name);
		cout << "tokenstr length is: " << tokenstr.length() << endl;
		doc.LoadJsonString(tokenstr, &err_code, &err_pos);
		root = doc.GetRoot();
		//Get from RMS response
		RetValue retur_value = recipient.ImportFromRMSResponse(root->AsObject());
		cout << "ImportFromRMSResponse Need RMS update: " << recipient.NeedUpdateRMS() << endl;
		cout << "ImportFromRMSResponse Export string: " << recipient.ExportToString() << endl;
		EXPECT_EQ(0, retur_value.GetCode());
		EXPECT_EQ(0, recipient.NeedUpdateRMS());
		EXPECT_EQ(4, recipient.GetRecipients().size());
		std::vector<std::string> cr = recipient.GetRecipients();
		Json::Value current_recepient = jroot["results"]["newRecipients"];
		if (recipient.GetRecipients().size() == 4) {
			for (unsigned int i = 0; i < current_recepient.size(); i++)
			{
				cout << "arrnum[" << i << "] = " << current_recepient[i];
				EXPECT_STREQ(current_recepient[i].asString().c_str(), cr[i].c_str());
			}
		}
		else {
			EXPECT_EQ(4, recipient.GetRecipients().size());
			cout << "Count of recipients is not correct!(ImportFromRMSResponse)" << endl;
		}
		
		//Add recepients the same with currentlist
		recipient.AddRecipients(list);
		cout << "AddDuplicateRecipients Need RMS update: " << recipient.NeedUpdateRMS() << endl;
		cout << "AddDuplicateRecipients add size: " << recipient.GetAddRecipients().size() << endl;
		cout << "AddDuplicateRecipients Export string: " << recipient.ExportToString() << endl;
		EXPECT_EQ(0, recipient.NeedUpdateRMS());
		EXPECT_EQ(0, recipient.GetAddRecipients().size());
		std::vector<std::string> ar = recipient.GetAddRecipients();
		if ((ar.size() == 0 )&& (recipient.GetRecipients().size()) == 4) {
			EXPECT_EQ(1,1);
		}
		else {
			cout << "Count of recipients is not correct!(ar)" << endl;
			EXPECT_EQ(1,0);
		}
		
		//Add not in recepients
		recipient.AddRecipients(addlist);
		cout << "AddRecipients Need RMS update: " << recipient.NeedUpdateRMS() << endl;
		cout << "AddRecipients add size: " << recipient.GetAddRecipients().size() << endl;
		cout << "AddRecipients Export string: " << recipient.ExportToString() << endl;
		EXPECT_EQ(1, recipient.NeedUpdateRMS());
		EXPECT_EQ(2, recipient.GetAddRecipients().size());
		RMRecipientList add_result0{ "testAdd1@nextlabs.com", "testAdd2@nextlabs.com" };
		RMRecipientList ar0 = recipient.GetAddRecipients();
		if (ar0.size() == add_result0.size()) {
			for (int i = 0; i<ar0.size(); i++)
			{
				EXPECT_STREQ(add_result0[i].c_str(), ar0[i].c_str());
			}
		}
		else {
			cout << "Count of recipients is not correct!(ar0)" << endl;
			EXPECT_EQ(ar0.size(), add_result0.size());
		}
		RMRecipientList add_result{ "test@test.com", "test1@test.com", "test2@test.com", "test2@nextlabs.com", "testAdd1@nextlabs.com", "testAdd2@nextlabs.com" };
		std::vector<std::string> ar1 = recipient.GetRecipients();
		string tokenstr1 = ReadFromFile("RecipientUpdate4.txt");
		cout << "tokenstr length is: " << tokenstr.length() << endl;
		doc.LoadJsonString(tokenstr1, &err_code, &err_pos);
		root = doc.GetRoot();
		//Get from RMS response
		RetValue retur_value1 = recipient.ImportFromRMSResponse(root->AsObject());
		if (recipient.GetRecipients().size() == 6) {
			for (int i = 0; i<ar1.size(); i++)
			{
				EXPECT_STREQ(add_result[i].c_str(), ar1[i].c_str());
			}
		}
		else {
			cout << "Count of recipients is not correct!(Add not in recepients)" << endl;
			EXPECT_EQ(6, recipient.GetRecipients().size());
		}

		//Add empty list
		recipient.AddRecipients(empty_list);
		cout << "AddEmpty Need RMS update: " << recipient.NeedUpdateRMS() << endl;
		cout << "AddEmpty add size: " << recipient.GetAddRecipients().size() << endl;
		cout << "AddEmpty Export string: " << recipient.ExportToString() << endl;
		EXPECT_EQ(0, recipient.NeedUpdateRMS());
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> vf_reci = readCSV("RecipientUpdate.csv");
INSTANTIATE_TEST_CASE_P(RecipientListAddTestReturn, RecipientListAddTest, testing::ValuesIn(vf_reci));

//RemoveRecipientListTest
TEST(RecipientListRemoveTest, RemoveRecipientTest) {
	string file_name;
	RMRecipientList list{ "test@test.com", "test1@test.com", "test2@test.com", "test2@nextlabs.com" };
	RMRecipientList addlist{ "test@test.com", "testAdd2@nextlabs.com", "testAdd3@nextlabs.com" };
	RMRecipientList removelist{ "test1@test.com", "testRemove1@nextlabs.com", "testRemove3@nextlabs.com" };
	RMRecipientList empty_list;
	try {
		file_name = "RecipientUpdate.txt";
		RMRecipients recipient;

		//Add new recepients
		recipient.AddRecipients(list);
		cout << "Add Export string: " << recipient.ExportToString() << endl;

		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;

		int err_code = 0;
		size_t err_pos = 0;
		string tokenstr = ReadFromFile(file_name);
		cout << "tokenstr length is: " << tokenstr.length() << endl;
		doc.LoadJsonString(tokenstr, &err_code, &err_pos);
		root = doc.GetRoot();
		//Get from RMS response
		RetValue retur_value = recipient.ImportFromRMSResponse(root->AsObject());
		cout << "ImportFromRMSResponse Export string: " << recipient.ExportToString() << endl;

		//Remove recepient from current
		recipient.RemoveRecipients(removelist);
		cout << "Remove Export string: " << recipient.ExportToString() << endl;
		EXPECT_EQ(1, recipient.NeedUpdateRMS());
		EXPECT_EQ(1, recipient.GetRemoveRecipients().size());
		RMRecipientList remove_result1{ "test1@test.com" };
		RMRecipientList current_result1{ "test@test.com", "test2@test.com", "test2@nextlabs.com" };
		vector<string> remove_re1 = recipient.GetRemoveRecipients();
		if (remove_result1.size() == remove_re1.size()) {
			for (int i = 0; i<remove_re1.size(); i++)
			{
				EXPECT_STREQ(remove_result1[i].c_str(), remove_re1[i].c_str());
			}
		}
		else {
			cout << "Count of remove recipient is not correct!(Remove recepient from current)" << endl;
			EXPECT_EQ(remove_result1.size(), remove_re1.size());
		}
		if (current_result1.size() == recipient.GetRecipients().size()) {
			for (int i = 0; i<current_result1.size(); i++)
			{
				EXPECT_STREQ(current_result1[i].c_str(), recipient.GetRecipients()[i].c_str());
			}
		}
		else {
			cout << "Count of current recipients is not correct!(Remove recepient from current)" << endl;
			EXPECT_EQ(current_result1.size(), recipient.GetRecipients().size());
		}
		
		//Add recepient
		recipient.AddRecipients(addlist);
		cout << "Remove Export string: " << recipient.ExportToString() << endl;
		EXPECT_EQ(1, recipient.NeedUpdateRMS());
		EXPECT_EQ(2, recipient.GetAddRecipients().size());

		//ImportFromRMSResponse
		tokenstr = ReadFromFile("RecipientUpdate1.txt");
		cout << "tokenstr length is: " << tokenstr.length() << endl;
		doc.LoadJsonString(tokenstr, &err_code, &err_pos);
		root = doc.GetRoot();
		//Get from RMS response
		RetValue retur_value1 = recipient.ImportFromRMSResponse(root->AsObject());
		RMRecipientList result{ "test@test.com" ,"test2@test.com" ,"test2@nextlabs.com" ,"testAdd2@nextlabs.com" ,"testAdd3@nextlabs.com" };
		EXPECT_EQ(retur_value1.GetCode(), 0);
		EXPECT_EQ(0,recipient.NeedUpdateRMS());
		if (result.size() == recipient.GetRecipients().size()) {
			for (int i = 0; i<current_result1.size(); i++)
			{
				EXPECT_STREQ(result[i].c_str(), recipient.GetRecipients()[i].c_str());
			}
		}
		else {
			EXPECT_EQ(result.size(), recipient.GetRecipients().size());
		}
		cout << "ImportFromRMSResponse Export string: " << recipient.ExportToString() << endl;
		JsonValue* j_value = recipient.ExportToJson();
		recipient.ImportFromJson(j_value ->AsObject());
		EXPECT_EQ(result.size(), recipient.GetRecipients().size());
		string str = recipient.ExportToString();
		recipient.ImportFromString(str);
		EXPECT_EQ(result.size(), recipient.GetRecipients().size());

		//Remove all recipients
		recipient.RemoveRecipients(result);
		EXPECT_EQ(retur_value1.GetCode(), 0);
		EXPECT_EQ(1, recipient.NeedUpdateRMS());
		EXPECT_EQ(0, recipient.GetRecipients().size());

		//Get from RMS
		tokenstr = ReadFromFile("RecipientUpdate2.txt");
		cout << "tokenstr length is: " << tokenstr.length() << endl;
		doc.LoadJsonString(tokenstr, &err_code, &err_pos);
		root = doc.GetRoot();
		//Get from RMS response
		RetValue retur_value2 = recipient.ImportFromRMSResponse(root->AsObject());
		EXPECT_EQ(retur_value2.GetCode(), 0);
		EXPECT_EQ(recipient.GetRecipients().size(), 0);
		cout << "ImportFromRMSResponse is: " << recipient.ExportToString() << endl;
		
	}
	catch (exception& e)
	{ 
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Test offline Add and Remove recipients
TEST(RecipientListAddAndRemoveTest, AddAndRemoveTest) {
	string file_name;
	RMRecipientList list{ "test@test.com", "test1@test.com", "test2@test.com", "test2@nextlabs.com" };
	RMRecipientList addlist{ "test@test.com", "testAdd2@nextlabs.com", "testAdd3@nextlabs.com" };
	RMRecipientList add_removelist{ "test@test.com"};
	RMRecipientList empty_list;
	try {
		file_name = "RecipientUpdate.txt";
		RMRecipients recipient;
		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;
		int err_code = 0;
		size_t err_pos = 0;

		//Add new recepients
		recipient.AddRecipients(list);
		string tokenstr = ReadFromFile(file_name);
		cout << "tokenstr length is: " << tokenstr.length() << endl;
		doc.LoadJsonString(tokenstr, &err_code, &err_pos);
		root = doc.GetRoot();
		//Get from RMS response
		RetValue retur_value = recipient.ImportFromRMSResponse(root->AsObject());
		if (retur_value.GetCode() == 0) {
			//Remove added recipients
			recipient.AddRecipients(addlist);
			EXPECT_EQ(recipient.GetAddRecipients().size(),2);
			EXPECT_EQ(recipient.GetRecipients().size(), 4);
			recipient.RemoveRecipients(addlist);
			EXPECT_EQ(recipient.GetAddRecipients().size(), 0);
			EXPECT_EQ(recipient.GetRecipients().size(), 3);
			EXPECT_EQ(recipient.GetRemoveRecipients().size(), 1);

			//Add the removed recipients back
			recipient.AddRecipients(add_removelist);
			EXPECT_EQ(recipient.GetRemoveRecipients().size(), 0);
			EXPECT_EQ(recipient.GetRecipients().size(), 4);
		}
		cout << "ImportFromRMSResponse Export string: " << recipient.ExportToString() << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Test HttpCode is not 200
class RecipientListHttpCodeTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(RecipientListHttpCodeTest, HttpCodeTest) {
	try {
		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;
		string file_name;
		int err_code = 0;
		size_t err_pos = 0;
		int base_code = 61440;
		string statu_code, message;

		vector<string> csv = GetParam();
		file_name = csv[2];
		RMRecipients recipient;
		string json_str = ReadFromFile(file_name);
		Json::Value jroot;
		jroot = ParseJsonFile(file_name);
		statu_code = jroot["statusCode"].asString();
		message = jroot["message"].asString();

		doc.LoadJsonString(json_str, &err_code, &err_pos);
		root = doc.GetRoot();
		RetValue ret =  recipient.ImportFromRMSResponse(root->AsObject());
		char* end;
		int i = static_cast<int>(strtol(statu_code.c_str(), &end, 10));
		EXPECT_EQ((base_code + i), ret.GetCode());
		EXPECT_STREQ(message.c_str(), ret.GetMessage());
		cout << "Test Finished!" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> vf_code = readCSV("RecipientHttpCode.csv");
INSTANTIATE_TEST_CASE_P(RecipientListHttpCodeTestReturn, RecipientListHttpCodeTest, testing::ValuesIn(vf_code));

//Test comment update
TEST	(RecipientUpdateComment, UpdaeComment) {
	try {
		string comment = "Comments test";
		RMRecipients recipient;
		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;

		int err_code = 0;
		size_t err_pos = 0;
		string tokenstr = ReadFromFile("RecipientUpdate.txt");
		cout << "tokenstr length is: " << tokenstr.length() << endl;
		doc.LoadJsonString(tokenstr, &err_code, &err_pos);
		root = doc.GetRoot();
		//Get from RMS response
		RetValue retur_value = recipient.ImportFromRMSResponse(root->AsObject());

		cout << "Comment Export string: " << recipient.GetComments() << endl;
		EXPECT_STREQ("", recipient.GetComments().c_str());
		recipient.UpdateComments(comment);
		cout << "Comment update Export string: " << recipient.GetComments() << endl;
		EXPECT_STREQ(comment.c_str(), recipient.GetComments().c_str());
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(AddTest,Add) {
	RMRecipients recipients;

	RMRecipientList list{ "test@test.com", "test1@test.com", "test2@test.com", "test2@nextlabs.com" };
	RMRecipientList addlist{ "test2@test.com", "add1@nextlabs.com" , "add2@nextlabs.com"};
	RMRecipientList removelist{ "test3@test.com", "test2@nextlabs.com","testRemove@nextlabs.com" };

	//Add recipients
	recipients.AddRecipients(list);
	cout << "Add:  " << recipients.ExportToString() << endl;
	EXPECT_EQ(1, recipients.NeedUpdateRMS());
	std::vector<std::string> cr_add = recipients.GetAddRecipients();

	//ImportFromRMSResponse
	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;
	const std::string jsonstr = ReadFromFile("RMSRecipients.txt");
	if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
		assert(true);
	}
	JsonValue *root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		assert(true);
	}
	RetValue retv = recipients.ImportFromRMSResponse(root->AsObject());
	cout << "ImportFromRMSResponse: " <<recipients.ExportToString() << endl;
	EXPECT_EQ(0, recipients.NeedUpdateRMS());
	EXPECT_EQ(0, retv.GetCode());

	//Add recipients
	uint8_t ret = recipients.AddRecipients(addlist);
	EXPECT_EQ(2, ret);
	EXPECT_EQ(1, recipients.NeedUpdateRMS());
	
	cout << "Add 2 : " << recipients.ExportToString() << endl;

	ret = recipients.RemoveRecipients(removelist);
	cout << "Add Remove:  " << recipients.ExportToString() << endl;
	//assert(recipients.NeedUpdateRMS());
	//cout << endl;
	//cout << "Recipients: " << recipients.ExportToString() << endl;

	//const std::string jsonstr1 = ReadFromFile("RMSRecipients1.txt");
	//if (!doc.LoadJsonString(jsonstr1, &err_code, &err_pos)) {
	//	assert(true);
	//}
	//root = doc.GetRoot();
	//if (!(NULL != root && root->IsObject())) {
	//	assert(true);
	//}
	//retv = recipients.ImportFromRMSResponse(root->AsObject());
	//assert(!recipients.NeedUpdateRMS());

	//cout << endl;
	//cout << endl;
	//cout << "Recipients: " << recipients.ExportToString() << endl;
}

//Test GetUpdateRecipientsQuery
TEST(GetUpdateRecipientsQuery,Update) {
	try {
		RMRecipientList list{ "test@test.com", "test1@test.com", "test2@test.com", "test2@nextlabs.com" };
		RMRecipientList addlist{ "test@test.com", "test1@test.com","testAdd1@nextlabs.com", "testAdd2@nextlabs.com" };
		RMRecipientList removelist{ "test@test.com", "test2@test.com", "testRemove1@nextlabs.com" };
		RMRecipientList resultlist{ "test@test.com", "test2@test.com" };
		RMRecipientList result_addlist{  "testAdd2@nextlabs.com" };
		RMRecipientList result_removelist{ "test@test.com" };
		
		//RMRecipientList empty_list;
		RMUser user = GetDefaultUser();
		RMNXLFile nfile = GetNXLFile("NormalTXTFile.txt");

		JsonDocument doc;
		int err_code = 0;
		size_t err_pos = 0;
		const std::string jsonstr = ReadFromFile("RecipientUpdate.txt");
		if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
			assert(true);
		}
		JsonValue *root = doc.GetRoot();
		if (!(NULL != root && root->IsObject())) {
			assert(true);
		}
		RMRecipients update_recipients = nfile.GetFileRecipients();
		////Add recipients list and sync with server
		update_recipients.AddRecipients(list);
		HTTPRequest http_add = user.GetUpdateRecipientsQuery(nfile, update_recipients);
		string addbody_str = "{\"parameters\":{\"newRecipients\":[";
		for (int i = 0; i < list.size(); i++) {
			addbody_str = addbody_str + "{\"email\":\""+ list[i];
			if (i != list.size()-1) {
				addbody_str = addbody_str + "\"},";
			}
			else {
				addbody_str = addbody_str + "\"}],\"removedRecipients\":[]}}";
			}
		}
		bool addbody_result = Check_Http_Body(http_add, addbody_str);
		EXPECT_TRUE(addbody_result);
		RetValue retv = update_recipients.ImportFromRMSResponse(root->AsObject());

		//RMRecipientList list{ "test@test.com", "test1@test.com", "test2@test.com", "test2@nextlabs.com" };
		/*RMRecipientList testlist1{ "test@test.com", "test1@test.com" };
		RMRecipientList testlist2{ "test222@test.com" };
		RMRecipientList testlist3{ "test@test.com" };
		update_recipients.RemoveRecipients(testlist1);
		update_recipients.AddRecipients(testlist2);
		update_recipients.RemoveRecipients(testlist2);
		update_recipients.AddRecipients(testlist1);*/
/*
		////Remove recipients removelist not sync with server
		update_recipients.RemoveRecipients(removelist);
		string duid = nfile.GetDuid();
		// Check http request
		HTTPRequest http_q = user.GetUpdateRecipientsQuery(nfile, update_recipients);
		EXPECT_STREQ(http_q.GetAcceptTypes()[0].c_str(), "application/json");
		//Check http header
		bool header_result = Check_HttpHeader(http_q, "application/json", user);
		EXPECT_TRUE(header_result);
		//Check URL and Method
		bool urlmethod_result = Check_HttpURL_Method(http_q, "POST", "/rs/share/"+ duid + "/update", user);
		EXPECT_TRUE(urlmethod_result);
		//Check request body
		string removebody_str = "{\"parameters\":{\"newRecipients\":[],\"removedRecipients\":[";
		for (int i = 0; i < resultlist.size(); i++) {
			removebody_str = removebody_str + "{\"email\":\"" + resultlist[i];
			if (i != resultlist.size()-1) {
				removebody_str = removebody_str + "\"},";
			}
			else {
				removebody_str = removebody_str + "\"}]}}";
			}
		}
		bool removebody_result = Check_Http_Body(http_q, removebody_str);
		EXPECT_TRUE(removebody_result);

		//Add recipients addlist
		update_recipients.AddRecipients(addlist);
		HTTPRequest http_ar = user.GetShareLocalFileQuery(user.GetDefaultMembership(), nfile);
		string body = http_ar.GetBody();
		//Need to verified the request body
		EXPECT_TRUE(0);
		cout << "Test finished!" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
*/