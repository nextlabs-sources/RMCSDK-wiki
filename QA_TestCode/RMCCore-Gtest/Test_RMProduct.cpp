//
//  Created by Lifi.Yuan on 4/19/18.
//  Copyright © 2018 NextLabs, Inc. All rights reserved.
//
#include "stdafx.h"
#include "Header_RMProduct.h"

using namespace RMCCORE;
using namespace std;

vector<vector<string>> csv_RMProduct_valid = readCSV("Product_valid.csv");

class ValidProductParamTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(ValidProductParamTest, ValidInput)
{
	vector<string> csv = GetParam();
	string caseID = csv[0];
	string productname = csv[1];
	string version = csv[2];
	string installPath = csv[3];
	string publisher = csv[4];
	string checkUpdateJsonFile = csv[5];
	int importType = string2Int(csv[6]);

	int major_ver = 0;
	int minor_ver = 0;
	int build_no = 0;

	vector<string> v;
	splitString(version, v,".");

	if (v.size()!=3)
	{
		cout << "Verion input is Invalid, will using defert vaule 0.0.0" << endl;
	}
	else
	{
		major_ver = string2Int(v[0]);
		minor_ver = string2Int(v[1]);
		build_no = string2Int(v[2]);
	}
	cout << "Constructor product/ ";
	RMProduct product = RMProduct(productname, major_ver, minor_ver, build_no);
	cout << "Done/"<<endl;

	cout << "SetAppInfo/ ";
	product.SetAppInfo(installPath, publisher);
	cout << "Done/" << endl;

	EXPECT_STREQ(version.c_str(), product.GetVersionString().c_str());
	EXPECT_STREQ(productname.c_str(), product.GetName().c_str());
	EXPECT_STREQ(installPath.c_str(), product.GetPath().c_str());
	EXPECT_STREQ(publisher.c_str(), product.GetPublisherName().c_str());

	cout << "CheckUpdate_viaImportJson:" << endl;
	CheckUpdate_viaImportJson(product, importType, checkUpdateJsonFile);


	cout << "Test finished." << endl;

	
}
INSTANTIATE_TEST_CASE_P(RMProductTesting, ValidProductParamTest, testing::ValuesIn(csv_RMProduct_valid));



/****
importType:
0: ImportFromRMSResponse(JsonObject * root);
default: ImportFromRMSResponse(std::string jsonstr);
2: ImportFromJson(JsonObject * value);
3: ImportFromString(std::string jsonstr);
****/
void CheckUpdate_viaImportJson(RMProduct &product, int importType, string jsonFile)
{
	string jsonstr = ReadFromFile(jsonFile);
	RMProduct p = product;
	RetValue ret = p.ImportFromRMSResponse(jsonstr);

	if (importType == 0)
	{
		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;
		int err_code = 0;
		size_t err_pos = 0;
		doc.LoadJsonString(jsonstr, &err_code, &err_pos);
		root = doc.GetRoot();
		ret = product.ImportFromRMSResponse(root->AsObject());
	}
	else if (importType == 2)
	{
		JsonValue * Jvalue = p.ExportToJson();
		product.ImportFromJson(Jvalue->AsObject());
	}
	else if (importType == 3)
	{
		jsonstr = p.ExportToString();
		product.ImportFromString(jsonstr);
	}
	else
	{
		ret = product.ImportFromRMSResponse(jsonstr);
	}

	Check_checkUpdate(product, jsonFile, ret);
}

void Check_checkUpdate(RMProduct &product, string jsonFile, RetValue ret)
{
	// using 3rd party Jsoncpp to read and get json value
	Json::Value jroot;
	jroot = ParseJsonFile(jsonFile);

	if (ret.GetCode() == 0) {
		string newVersion = jroot["results"]["newVersion"].asString();
		string downloadURL = jroot["results"]["downloadURL"].asString();
		string sha1Checksum = jroot["results"]["sha1Checksum"].asString();

		EXPECT_TRUE(product.IsNewVersionAvailable());
		EXPECT_STREQ(newVersion.c_str(), product.GetNewVersionString().c_str());
		EXPECT_STREQ(downloadURL.c_str(), product.GetDownloadURL().c_str());
		EXPECT_STREQ(sha1Checksum.c_str(), product.GetDownloadChecksum().c_str());
	}
	else
	{
		EXPECT_FALSE(product.IsNewVersionAvailable());
		checkErrorCode(ret, jsonFile);
	}
}