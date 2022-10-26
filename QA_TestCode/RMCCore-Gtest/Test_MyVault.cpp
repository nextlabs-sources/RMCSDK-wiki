#include "stdafx.h"
#include "RMCCORE\restful\rmnxlfile.h"
#include "RMCCORE\restful\rmuser.h"
#include "RMCCORE\restful\rmactivitylog.h"
#include "Header_File.h"
#include "time.h"
#include <ctime>

using namespace RMCCORE;
using namespace std;

TEST(MyVaultTest, MyVaultTest) {
	try {
		RMUser	  user = GetDefaultUser();
	    Json::Value jroot;
		//const RMNXLFile nfile = GetNXLFile("NormalTXTFile.txt");
		RMNXLFile nfile = RMNXLFile("D:\\workplace\\RMCCORE\\TestAPI\\TestGoogleTest\\TestGoogleTest\\plan file\\DefaultNXLFile-2018-04-18-02-46-30.txt.nxl");
		nfile.Open();
		string duid1 = nfile.GetDuid();
		string ext1 = nfile.GetFileExtension();
		string link1 = nfile.GetFileLink();
		string name1 = nfile.GetFileName();
		uint64_t modified1 = nfile.GetLastModify();
		RMRecipients rec1 = nfile.GetFileRecipients();
		uint64_t sync_time1 = nfile.GetLastRMSSyncTime();
		uint32_t ml1 = nfile.GetMaintainLevel();
		string get_name1 = nfile.GetName();
		//uint64_t rights = nfile.GetNXLRights();
		string owner_id1 = nfile.GetOwnerID();
		string path_id11 = nfile.GetPathID();
		string file_ext1 = nfile.GetSourceFileExt();
		string source_name1 = nfile.GetSourceFileName();
		string source_path1 = nfile.GetSourceFilePath();
		nfile.Open();
		bool open_result1 = nfile.IsOpened();
		string path_id = nfile.GetPathID();
		JsonDocument doc;
		int err_code = 0;
		size_t err_pos = 0;
		//HTTPRequest request = user.GetNXLFileMetadataQuery(nfile);
		const std::string jsonstr = ReadFromFile("RMNXLFileMData.txt");
		jroot = ParseJsonFile("RMNXLFileMData.txt");
		string data_str = ReadFromFile("MetaData.txt");
		RetValue ret = nfile.ImportMetadataQueryFromRMSResponse(data_str);

		//RetValue retv = nfile.ImportMetadataQueryFromRMSResponse(jsonstr);
		string duid = nfile.GetDuid();
		string ext = nfile.GetFileExtension();
		string link = nfile.GetFileLink();
		string name = nfile.GetFileName();
		uint64_t modified = nfile.GetLastModify();
		RMRecipients rec = nfile.GetFileRecipients();
		uint64_t sync_time = nfile.GetLastRMSSyncTime();
		uint32_t ml = nfile.GetMaintainLevel();
		string get_name = nfile.GetName();
		//uint64_t rights = nfile.GetNXLRights();
		string owner_id = nfile.GetOwnerID();
		string path_id1 = nfile.GetPathID();
		string file_ext = nfile.GetSourceFileExt();
		string source_name = nfile.GetSourceFileName();
		string source_path = nfile.GetSourceFilePath();
		bool is_delete = nfile.IsDeleted();
		bool is_share = nfile.IsShared();
		bool is_revoke = nfile.IsRevoked();
		bool open_result = nfile.IsOpened();
		string ag = nfile.GetAgreement();
		EXPECT_TRUE(0);
		RMRecipients rp(nfile.GetFileRecipients());
		cout << "NXLFile Metadata :" << endl;

	}
	catch (exception& e) {
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}