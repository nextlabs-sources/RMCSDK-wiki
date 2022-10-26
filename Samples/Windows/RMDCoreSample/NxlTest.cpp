#include "stdafx.h"
#include "NxlTest.h"

#include <vector>
#include "time.h"
#include "assert.h"
#include "usertest.h"
#include "TenantTest.h"
#include "TokenTest.h"
#include "HttpReqest.h"

#include "rmccore/restful/rmnxlfile.h"
#include "rmccore\restful\rmrecipients.h"
#include "rmccore\policy\condition.h"

using namespace RMCCORE;

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
static const std::string kRecipient("Gavin.Ye@nextlabs.com");

#define NXL_TEST_PLAIN_FILE	"DecryptRMSTokenTest.txt"
#define NXL_TEST_NXL_FILE	"RMSToken.txt.nxl"

#define DEFAULT_NXLFILE_NAME	"DNFileData.txt"
#define DEFAULT_LOGFILE_NAME	"DLogPool.txt"

const RMLogPool& GetDefaultLogPool(void)
{
	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile(DEFAULT_LOGFILE_NAME);
	if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
		throw;
	}

	static RMLogPool logs;
	logs.ImportFromString(jsonstr);

	return logs;
}

const RMNXLFile& GetDefaultNXLFile(void)
{
	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile(DEFAULT_NXLFILE_NAME);
	if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
		throw;
	}

	JsonValue *root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		throw;
	}

	static RMNXLFile file(root->AsObject());

	return file;
}

void TestNxlFile()
{
	const std::string nxlFile("RMSTokenTest.txt.nxl");
	const std::string txtFile(NXL_TEST_PLAIN_FILE);
	NXLFMT::File file;

	file.open(nxlFile, true);
	assert(0 == memcmp(kTokenId, file.getTokenId().getId().data(), 16));

	file.setTokenKey(std::vector<uint8_t>(kTokenValue, kTokenValue + 32));

	std::ofstream ifs;
	ifs.open(txtFile, ios_base::trunc | ios_base::out);
	assert(ifs.is_open());
	uint64_t contentLength = file.getContentLength();
	uint64_t offset = 0;

	cout << endl << "Open nxl file " << nxlFile << endl;
	cout << "File content :" << endl;
	bool beof = 0;
	while (contentLength) {
		uint8_t buf[513];
		uint32_t bytesToRead = 512;
		memset(buf, 0, sizeof(buf));
		bytesToRead = file.read(offset, buf, bytesToRead);
		if (bytesToRead > contentLength)
			bytesToRead = (uint32_t)contentLength;
		ifs.write((char *)buf, bytesToRead);
		cout << buf;
		offset += bytesToRead;
		contentLength -= bytesToRead;
	}
	file.close();
	ifs.close();
}

void TestProtectFile()
{
	std::string clientid = GenerateClientID();
	RMUser user = GetDefaultUser();
	RMTenant tenant = CreateDefaultTenant();
	NXLFMT::Obligations obs;
	//NXLFMT::FullRights	rights;
	NXLFMT::Rights rights(BUILTIN_RIGHT_VIEW | BUILTIN_RIGHT_EDIT | BUILTIN_RIGHT_PRINT | BUILTIN_OBRIGHT_WATERMARK);

	RMActivityLog activitylog;
	RMLogPool	logpool;

	RMTokenPool tokenpool = GetDefaultTokens();
	assert(tokenpool.size() != 0);
	RMToken token = tokenpool.pop();
	RMNXLFile nfile("", "");
	uint64_t start = RMCCORE::NXTime::nowTime() * 1000;
	uint64_t end = RMCCORE::NXTime::nowTime() * 1000 + 604800000;
	CONDITION::Expiry expiry(CONDITION::ExpiryType::ABSOLUTEEXPIRE, end, start);
	//CONDITION::Expiry expiry(CONDITION::ExpiryType::NEVEREXPIRE);
	Watermark::Rotation ration = Watermark::NOROTATION;
	Watermark w_m("$(User)$(Date)$(Time)", "", "", 20, 2, ration, 1);
	obs.push_back(w_m);
	NXLAttributes attr;
	attr.rights = &rights;
	attr.obs = &obs;
	attr.tags = "{}";
	attr.expiry = &expiry;
	RetValue value = user.ProtectLocalFile(NXL_TEST_PLAIN_FILE, "Tokentest1.txt.nxl", attr, token, activitylog, nfile);
	
	bool exp_result = nfile.GetNXLExpiration();
	//exp_result must be true	
	CONDITION::Expiry exp = nfile.GetExpiration();
	int type = exp.getType();
	uint64_t end_date = exp.getEndDate();
	uint64_t start_date = exp.getStartDate();
	
	const Obligations obs1 = nfile.getObligations();

	NXLFMT::Rights nxlright(nfile.GetNXLRights());
	std::vector<std::string> rightstr;
	nxlright.ToStrings(rightstr);

	//assert(rightstr.size() == 3);
	cout << endl;
	cout << "NXL file right: ";
	for (size_t i = 0; i < rightstr.size(); i++)
		cout << rightstr[i] << ";";
	cout << endl;

	//assert(nfile.Open());
	logpool.AddLog(activitylog);
	cout << "NXL Protect Local file Query: " << endl;
	HTTPRequest request = user.GetProtectLocalFileQuery(nfile);
	printHttpRequest(request);
	cout << endl;

	const std::string protectstr = ReadFromFile("RMNXLFileProtect.txt");
	RetValue retv = nfile.ImportFromRMSResponse(protectstr);

	//assert(retv);

	token = tokenpool.pop();
	CONDITION::Expiry expiry1(CONDITION::ExpiryType::ABSOLUTEEXPIRE, (time(NULL) + 24*60*60)*1000);
	Watermark watermark("test test", "", "", 0, 0, Watermark::NOROTATION, false);
	obs.push_back(watermark);
	RMRecipientList sharelist{ "test@test.com", "test1@test.com", "test2@test.com", "test2@nextlabs.com" };
	attr.expiry = &expiry1;
	value = user.ShareLocalFile(NXL_TEST_PLAIN_FILE, NXL_TEST_NXL_FILE, attr, sharelist, token, "test123", activitylog, nfile);
	logpool.AddLog(activitylog);
	//assert(nfile.Open());

	WriteToFile(DEFAULT_LOGFILE_NAME, logpool.ExportToString());



	cout << "NXL query token:" << endl;
	HTTPRequest tokenquery = user.GetFileTokenQuery(nfile);
	printHttpRequest(tokenquery);
	cout << endl;


	RMRecipients list = nfile.GetFileRecipients();
	cout << "NXL update recipients: " << endl;
	HTTPRequest protectrequest = user.GetShareLocalFileQuery(user.GetDefaultMembership(), nfile);
	printHttpRequest(protectrequest);
	cout << endl;

	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile("RMNXLFileShare.txt");
	assert(doc.LoadJsonString(jsonstr, &err_code, &err_pos));

	JsonValue *root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		return ;
	}
	retv = nfile.ImportFromRMSResponse(root->AsObject());

	RMRecipients rp(list);
	rp.AddRecipients(RMRecipientList{ "test3@test.com" });
	rp.RemoveRecipients(RMRecipientList{ "test2@nextlabs.com" });
	HTTPRequest recipentrequest = user.GetUpdateRecipientsQuery(nfile, rp);
	printHttpRequest(recipentrequest);

	WriteToFile(DEFAULT_NXLFILE_NAME, nfile.ExportToString());
	
}