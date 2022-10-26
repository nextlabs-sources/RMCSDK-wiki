#include "stdafx.h"
using namespace std;

bool rights_verified(std::vector<SDRmFileRight> rights, ISDRmNXLFile* nxlfile) {
	std::vector<SDRmFileRight> rights_nxl = nxlfile->GetRights();
	int rights_size = rights.size();
	int right_nxlsize = rights_nxl.size();
	bool rights_result;
	if (rights_size == right_nxlsize) {
		for (int i = 0; i < rights_size; i++) {
			rights_result = nxlfile->CheckRights(rights[i]);
			if (!rights_result) {
				cout << "Rights not match!" << endl;
				return false;
			}
		}
		return true;
	}
	else {
		cout << "File size incorrect!" << endl;
		return false;
	}

}

//Test local protected NXL File
TEST(NXLFileLocalProtectedNXLFileTest, NXLFileTest) {
	try {
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = L"C:\\SDK test data\\Normal_TXT.txt";
		ISDRFiles* pFile = NULL;
		std::wstring filename;

		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;

		std::vector<SDRmFileRight> rights;
		rights = Tick_Rights("view");

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			wstring nxlfile_path;
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermarkinfo, expire, tags);
			if (!res.GetCode()) {
				//res = puser->GetLocalFileManager(&pFile);
				//size_t no;
				//no = pFile->GetListNumber();
				//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					filename = nxlfile->GetFileName();
					uint64_t size = nxlfile->GetFileSize();
					//Check nxl file rights
					bool rights_result = rights_verified(rights, nxlfile);
					EXPECT_TRUE(rights_result);
					//Check valid nxl file
					bool valid_nxl = nxlfile->IsValidNXL();
					EXPECT_TRUE(valid_nxl);
					//Remove file
					bool delete_result = pFile->RemoveFile(nxlfile);
					EXPECT_TRUE(delete_result);
					cout << "Test Finished!" << endl;
				}
			}
			else {
				cout << "Protect local file failed!" << endl;
			}
		}
		else {
			cout << "Login failed!" << endl;
		}
		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Test local shared NXL File
TEST(NXLFileLocalSharedNXLFileTest, NXLFileTest) {
	try {
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = L"C:\\SDK test data\\Normal_TXT.txt";
		ISDRFiles* pFile = NULL;
		std::wstring filename;
		wstring recipients = L"jewelry.zhu@nextlabs.com,lifi.yuan@nextlabs.com";
		wstring comments_w = L"Comments to recipients";

		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;

		std::vector<SDRmFileRight> rights;
		rights = Tick_Rights("view");

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			wstring nxlfile_path;
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermarkinfo, expire, tags);
			if (!res.GetCode()) {
				//res = puser->GetLocalFileManager(&pFile);
				//size_t no;
				//no = pFile->GetListNumber();
				//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);

				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					filename = nxlfile->GetFileName();
					uint64_t size = nxlfile->GetFileSize();
					//Check nxl file rights
					bool rights_result = rights_verified(rights, nxlfile);
					EXPECT_TRUE(rights_result);
					//Check valid nxl file
					bool valid_nxl = nxlfile->IsValidNXL();
					EXPECT_TRUE(valid_nxl);
					bool open_result = nxlfile->IsOpen();
					EXPECT_TRUE(open_result);
					////Remove file
					//bool delete_result = pFile->RemoveFile(nxlfile);
					//EXPECT_TRUE(delete_result);
					//cout << "Test Finished!" << endl;
				}
			}
			else {
				cout << "Share local file failed!" << endl;
			}

			cout << "Test Finished!" << endl;
		}
		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//Test downloaded server NXL File
TEST(NXLFileFromUploadedTest, FromUploaded) {
	SDR_Expiration expire;
	expire.type = IExpiryType::RANGEEXPIRE;
	expire.start = (uint64_t)std::time(nullptr) * 1000;
	expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;
	wstring file_name = L"Protect_normal.txt";

	wstring localfile_pathw = plan_file_path + file_name;
	ISDRmcInstance *pInstance = NULL;
	ISDRmTenant *pTenant = NULL;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	std::wstring filename;

	std::vector<SDRmFileRight> rights;
	rights.push_back(RIGHT_VIEW);

	wstring comments_w = L"test";
	wstring recipients = s2ws(email_zhu);

	SDR_WATERMARK_INFO watermark_info = { "$USER","font","color",9,0,ANTICLOCKWISE,true };
	wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";

	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	if (login_result) {
		wstring nxlfile_path;
		res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire);
		if (res.GetCode() == 0) {
			ISDRFiles* pFile = NULL;

			bool delete_result;
			//res = puser->GetLocalFileManager(&pFile);
			//size_t no = pFile->GetListNumber();
			std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

			//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
			ISDRmNXLFile* nxlfile;
			res = puser->OpenFile(nxlfile_path, &nxlfile);
			if (nxlfile) {
				filename = nxlfile->GetFileName();
				//Upload file
				res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, comments_w);
				EXPECT_FALSE(res.GetCode());

				//Remove file
				delete_result = pFile->RemoveFile(nxlfile);
				EXPECT_TRUE(delete_result);

				bool file_info_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
				if (file_info_result) {
					Json::Value jroot;
					jroot = ParseJsonFile(myvaultfilelist_return);
					string path_id = jroot["results"]["detail"]["files"][0]["pathId"].asString();
					uint64_t fsize = jroot["results"]["detail"]["files"][0]["size"].asInt64();
					jroot = ParseJsonFile(user_respond);
					string defualt_tenant = jroot["extra"]["defaultTenant"].asString();

					res = puser->MyVaultDownloadFile(path_id, downloaded_path, 2);
					if (!res.GetCode()) {
						ISDRmNXLFile* pFile_local = NULL;
						res = puser->OpenFile(downloaded_path, &pFile_local);

						if (!res.GetCode()) {
							bool is_expired = pFile_local->CheckExpired();
							EXPECT_FALSE(is_expired);
							bool view_rihjts = pFile_local->CheckRights(RIGHT_VIEW);
							EXPECT_TRUE(view_rihjts);
							string watermark_string = pFile_local->GetAdhocWaterMarkString();
							EXPECT_STREQ(watermark_string.c_str(), "$USER");
							uint64_t size = pFile_local->GetFileSize();
							EXPECT_EQ(size, fsize);
							string tenant = pFile_local->GetTenantName();
							EXPECT_STREQ(defualt_tenant.c_str(), tenant.c_str());
							bool is_open = pFile_local->IsOpen();
							EXPECT_TRUE(is_open);
							string mem_id;
							Json::Value jroot;
							jroot = ParseJsonFile(user_respond);
							for (int i = 0; i < jroot["extra"]["memberships"].size(); i++) {
								int pro_id = jroot["extra"]["memberships"][i]["projectId"].asInt();
								if (pro_id == 1) {
									mem_id = jroot["extra"]["memberships"][i]["id"].asString();
								}
							}
							bool is_owner = pFile_local->IsOwner(mem_id);
							EXPECT_TRUE(is_owner);
							//bool recipient_sync = pFile_local->IsRecipientsSynced();
							//EXPECT_TRUE(recipient_sync);
							bool is_valid = pFile_local->IsValidNXL();
							EXPECT_TRUE(is_valid);

						}
					}
				}
			}


			puser->LogoutUser();
		}
		else {
			cout << "Share file failed!" << endl;
			EXPECT_TRUE(false);
		}
	}
	else {
		EXPECT_FALSE(res.GetCode());
		cout << "User login failed!" << endl;
	}
	SDWLibDeleteRmcInstance(pInstance);

	login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
	if (login_result) {
		Json::Value jroot;
		jroot = ParseJsonFile(myvaultfilelist_return);
		string path_id = jroot["results"]["detail"]["files"][0]["pathId"].asString();
		uint64_t fsize = jroot["results"]["detail"]["files"][0]["size"].asInt64();
		jroot = ParseJsonFile(user_respond);
		string defualt_tenant = jroot["extra"]["defaultTenant"].asString();

		ISDRmNXLFile* pFile_local = NULL;
		res = puser->OpenFile(downloaded_path, &pFile_local);
		if (!res.GetCode()) {
			bool is_expired = pFile_local->CheckExpired();
			EXPECT_FALSE(is_expired);
			bool view_rihjts = pFile_local->CheckRights(RIGHT_VIEW);
			EXPECT_TRUE(view_rihjts);
			string watermark_string = pFile_local->GetAdhocWaterMarkString();
			EXPECT_STREQ(watermark_string.c_str(), "$USER");
			uint64_t size = pFile_local->GetFileSize();
			EXPECT_EQ(size, fsize);
			string tenant = pFile_local->GetTenantName();
			EXPECT_STREQ(defualt_tenant.c_str(), tenant.c_str());
			bool is_open = pFile_local->IsOpen();
			EXPECT_TRUE(is_open);
			string mem_id;
			Json::Value jroot;
			jroot = ParseJsonFile(user_respond);
			for (int i = 0; i < jroot["extra"]["memberships"].size(); i++) {
				int pro_id = jroot["extra"]["memberships"][i]["projectId"].asInt();
				if (pro_id == 1) {
					mem_id = jroot["extra"]["memberships"][i]["id"].asString();
				}
			}
			bool is_owner = pFile_local->IsOwner(mem_id);
			EXPECT_FALSE(is_owner);
			//bool recipient_sync = pFile_local->IsRecipientsSynced();
			//EXPECT_TRUE(recipient_sync);
			bool is_valid = pFile_local->IsValidNXL();
			EXPECT_TRUE(is_valid);
		}
		puser->LogoutUser();
	}
	else {
		EXPECT_FALSE(res.GetCode());
		cout << "User login failed!" << endl;
	}
	SDWLibDeleteRmcInstance(pInstance);
}

//Test project file owner and user B
TEST(NXLFileFromProject, FromProject) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;

		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			SDWLResult res = puser->GetHeartBeatInfo();
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());

			wstring nxlfilepath;
			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks1;
			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks2;
			vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks3;
			//File owner, ad-hoc file
			res = puser->GetRights(server_file_path + L"FileOwner-ad-hoc.txt.nxl", rightsAndWatermarks);
			//User B, ad-hoc file
			res = puser->GetRights(server_file_path + L"UserB-adhoc.txt.nxl", rightsAndWatermarks1);
			//File owner, cp rights
			res = puser->GetRights(server_file_path + L"FileOwner-cprights.txt.nxl", rightsAndWatermarks2);
			//User B, cp rights
			res = puser->GetRights(server_file_path + L"UserB-cprights.txt.nxl", rightsAndWatermarks3);

		}
		else {
			cout << "User login failed!" << endl;
			EXPECT_TRUE(false);
		}

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
