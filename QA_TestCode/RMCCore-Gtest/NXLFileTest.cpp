#include "stdafx.h"
#include "rmccore\restful\rmuser.h"
#include "rmccore\format\nxlfile.h"
#include "Header_HttpRequest.h"
#include "Header_Tenant.h"
#include "Header_Token.h"

using namespace RMCCORE;
using namespace std;
using namespace NXLFMT;

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

#define NXL_TEST_PLAIN_FILE_PATH	"D:\\workplace\\FILE_PATH\\"
#define NXL_TEST_NXL_FILE_PATH	"D:\\workplace\\GENERATE_FILE\\"

////Test open
//class NXLOpenTest :public::testing::TestWithParam<string> {};
//TEST_P(NXLOpenTest, OpenTest) {
//	std::vector<std::string> vec;
//	string para = GetParam();
//	vec = splitWithStl(para,",");
//	std::cout << "File name: " << vec[0] << endl;
//	std::cout << "If is read only: " << vec[1] << endl;
//	const std::string nxlFile(vec[0]);
//	try{
//		NXLFMT::File file;
//		bool result = file.open(nxlFile, const_str_int(vec[1]));
//		EXPECT_DOUBLE_EQ(result, const_str_int(vec[2]));
//		cout << endl << "Open nxl file result: " << result << endl;
//	}
//	catch (exception& e) {
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}
//INSTANTIATE_TEST_CASE_P(NXLOpenTestReturn, NXLOpenTest, testing::Values("AppiumTest.xls.nxl,1,1","InvalidNXLFile.txt.nxl,1,0", "NormalTXTFile.txt.nxl,1,1","NormalTXTFile.txt.nxl,0,1","NotExistTXTFile.txt,1,0","Token_emptyToken.txt,0,0"));

////Test getTokenid
//class NXLGetTokenidTest :public::testing::TestWithParam<string> {};
//TEST_P(NXLGetTokenidTest, GetTokenIDTest) {
//	std::vector<std::string> vec;
//	string para = GetParam();
//	vec = splitWithStl(para, ",");
//	std::cout << "File name: " << vec[0] << endl;
//	std::cout << "If is read only: " << vec[1] << endl;
//	const std::string nxlFile(vec[0]);
//	try {
//		const std::string nxlFile("NormalTXTFile.txt.nxl");
//		NXLFMT::File file;
//
//		file.open(nxlFile, true);
//		assert(0 == memcmp(kTokenId, file.getTokenId().getId().data(), 16));
//		file.close();
//	}
//	catch (exception& e) {
//		cout << e.what() << endl;
//		cout << "Exceptions!" << endl;
//		throw;
//	}
//}
//INSTANTIATE_TEST_CASE_P(NXLGetTokenIdTestReturn, NXLGetTokenidTest, testing::Values( "NormalTXTFile.txt.nxl,0,1", "NotExistTXTFile.txt,1,0", "Token_emptyToken.txt,0,0"));

////Test Normal NXL file
//TEST(NXLFileTest,NormalNXLFileTest) {
//	const std::string nxlFile("NormalTXTFile.txt.nxl");
//	const std::string txtFile(NXL_TEST_PLAIN_FILE);
//	NXLFMT::File file;
//
//	file.open(nxlFile, true);
//	assert(0 == memcmp(kTokenId, file.getTokenId().getId().data(), 16));
//
//	file.setTokenKey(std::vector<uint8_t>(kTokenValue, kTokenValue + 32));
//
//	std::ofstream ifs;
//	ifs.open(txtFile, ios_base::trunc | ios_base::out);
//	assert(ifs.is_open());
//	uint64_t contentLength = file.getContentLength();
//	uint64_t offset = 0;
//
//	cout << endl << "Open nxl file " << nxlFile << endl;
//	cout << "File content :" << endl;
//	bool beof = 0;
//	while (contentLength) {
//		uint8_t buf[513];
//		uint32_t bytesToRead = 512;
//		memset(buf, 0, sizeof(buf));
//		bytesToRead = file.read(offset, buf, bytesToRead);
//		if (bytesToRead > contentLength)
//			bytesToRead = (uint32_t)contentLength;
//		ifs.write((char *)buf, bytesToRead);
//		cout << buf;
//		offset += bytesToRead;
//		contentLength -= bytesToRead;
//	}
//	file.close();
//	ifs.close();
//}

vector<vector<string>> csv_IsNXL = readCSV("File_IsNXL.csv");

class IsNXL :public::testing::TestWithParam<vector<string>> {};
TEST_P(IsNXL, ValidInput)
{
	vector<string> csv = GetParam();
	string caseID = csv[0];
	string file = csv[1];
	string isNXL= csv[2];

	RMNXLFile nfile(file);
	if (isNXL=="yes" || isNXL=="true")
	{
		EXPECT_TRUE(nfile.IsNXL());
	}
	else
	{
		EXPECT_FALSE(nfile.IsNXL());
	}
}
INSTANTIATE_TEST_CASE_P(NXLFileTesting, IsNXL, testing::ValuesIn(csv_IsNXL));


