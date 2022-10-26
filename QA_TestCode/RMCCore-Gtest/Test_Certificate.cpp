//
//  Created by Jewelry Zhu on 2/07/18.
//  Copyright @2018 NextLabs, Inc. All rights reserved.
//
#include "stdafx.h"
#include "rmccore\restful\rmtoken.h"
#include "rmccore\restful\rmuser.h"
#include "rmccore\restful\rmtenant.h"
#include "rmccore\restful\rmcertificate.h"
#include "Header_Tenant.h"

using namespace RMCCORE;
using namespace std;

const string file_path = "D:\\workplace\\API\\Git\\test file\\";

void PrintOutVector(std::vector<uint8_t>& in)
{
	string result;
	std::cout << "Vector Contains: ";
	stringbuf str;
	std::string output(10, 0);
	for (std::vector<uint8_t>::iterator i = in.begin(); i != in.end(); ++i)
		//output[*i] = static_cast<unsigned int>(*i);
		std::cout << static_cast<unsigned int>(*i);
	std::cout << "\n";
}

//Test agreement, case block
class CertificateParaTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(CertificateParaTest, ChangeRMSResponseTest) {
	string file_name;
	string expect_ag0, expect_ag1;
	int code;
	try {
		RMCertificate certificate;
		vector<string> csv = GetParam();
		file_name = csv[2];
		expect_ag0 = csv[3];
		expect_ag1 = csv[4];
		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;
		int err_code = 0;
		size_t err_pos = 0;
		cout << "Case name: " << csv[1] << endl;
		string tokenstr = ReadFromFile(default_file_path + file_name);

		cout << "tokenstr length is: " << tokenstr.length() << endl;
		if (!doc.LoadJsonString(tokenstr, &err_code, &err_pos))
		{
			assert(true);
		}
		root = doc.GetRoot();
		RetValue retur_value = certificate.ImportFromRMSResponse(root->AsObject());
		//RetValue retur_value = certificate.ImportFromRMSResponse(tokenstr);

		JsonValue *  jv = certificate.ExportToJson();
		certificate.ImportFromJson(jv->AsObject());
		cout << "Exported to string" << certificate.ExportToString() << endl;
		ExportToFile("test_cert.txt", certificate.ExportToString());
		cout << "Code: " << retur_value.GetCode() << endl;
		cout << "Message: " << retur_value.GetMessage() << endl;
		RMAgreement ag0 = certificate.GetAgreement0();
		RMAgreement ag1 = certificate.GetAgreement1();

		cout << "Agreetment0: " << certificate.GetAgreement0().size() << endl;
		cout << "Agreetment1: " << certificate.GetAgreement1().size() << endl;
		PrintOutVector(ag0);
		PrintOutVector(ag1);
		/*const std::string tokenstr1 = ReadFromFile("RMSTokenTest.txt");
		if (!doc.LoadJsonString(tokenstr1, &err_code, &err_pos))
		{
		assert(true);
		}*/
		/*	if (!retur_value.GetCode()) {
		string test = "532EDC7688417404A61A9F7D4D2F4535873131DE2E180EB5C3A1D9AADF4E6FBDCAA470043CBAF5BF0DF130556AB725C9F449C68F1A419A3A1A7376A2EB935B89DBCCBD0CEAA0AA547B63B963F8F34DAEE4D2991EBD2469A47F6DF915A31DC8A36D0A69FBFA96171716FBACDB270F62C372A9A4CEC9503D62AB7B54DA0B0BBC5F5C6171BCE43FD2EE137D4C3CB2CDFE4AF6B99567127658754BADA622415E06D6C691127D8CD2061CA878A41757C4F3E72EE3DD189D1E5760FCDB75A5678396F297F92A9CF9DD96DB35BCF3D9FE490C24B02B24F516AC0AFE7663398880D038A69C43E7BA847A5A48C2CF008768F62B41BFFD58BFDB5F6BAA589ED9B5B3662D8D";
		char*p = (char*)test.data();
		cout << p << endl;
		uint8_t * b = (uint8_t *)p;
		ostringstream oss;
		oss  << b << endl;
		cout << oss.str() << endl;
		RMAgreement ag0 = certificate.GetAgreement0();
		RMAgreement ag1 = certificate.GetAgreement1();
		cout << "Agreetment0: " << certificate.GetAgreement0().size() << endl;
		cout << "Agreetment1: " << certificate.GetAgreement1().size() << endl;
		PrintOutVector(ag0);
		PrintOutVector(ag1);

		//EXPECT_STREQ(expect_ag0.c_str(),certificate.GetAgreement0);
		//EXPECT_STREQ(expect_ag1.c_str(), certificate.GetAgreement1);
		//ExportToFile(file_name, certificate.ExportToString());
		}*/
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> vf_ce = readCSV(default_file_path + "MembershipTest.csv");
INSTANTIATE_TEST_CASE_P(CertificateParaTestReturn, CertificateParaTest, testing::ValuesIn(vf_ce));

class CertificateErrorResponse :public::testing::TestWithParam<vector<string>> {};
TEST_P(CertificateErrorResponse, CertificateErrorTest) {
	try {
		string file_name;
		string expect_ag0, expect_ag1;
		RMCertificate certificate;
		int result_code;
		int code;
		string message;
		int base_code = 61440;
		vector<string> csv = GetParam();
		file_name = csv[2];
		RMCCORE::JsonDocument doc;
		RMCCORE::JsonValue* root;
		int err_code = 0;
		size_t err_pos = 0;
		cout << "Case name: " << csv[1] << endl;
		string tokenstr = ReadFromFile(default_file_path + file_name);

		Json::Value jroot;
		jroot = ParseJsonFile(default_file_path + file_name);

		//Read root node: status and message
		code = jroot["statusCode"].asInt();
		message = jroot["message"].asString();

		cout << "tokenstr length is: " << tokenstr.length() << endl;
		doc.LoadJsonString(tokenstr, &err_code, &err_pos);
		root = doc.GetRoot();
		RetValue retur_value = certificate.ImportFromRMSResponse(root->AsObject());
		result_code = code + base_code;
		EXPECT_EQ(result_code, retur_value.GetCode());
		EXPECT_STREQ(message.c_str(), retur_value.GetMessage());
		cout << "Get code:  " << retur_value.GetCode() << endl;
		cout << "Get message:  " << retur_value.GetMessage() << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> error_para = readCSV(default_file_path + "MembershipError.csv");
INSTANTIATE_TEST_CASE_P(CertificateErrorResponseReturn, CertificateErrorResponse, testing::ValuesIn(error_para));