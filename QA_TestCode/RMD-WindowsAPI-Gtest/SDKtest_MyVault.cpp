#include "stdafx.h"
using namespace std;

//Get Myvault file list
TEST(MyVaultGetMyvaultFiles, GetMyvaultFiles) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		ISDRmUser *puser_b = NULL;
		uint32_t pageId = 1;
		uint32_t pageSize = 1000;
		//orderBy Should be fileName, creationTime, size
		string orderBy;

		std::vector<SDR_MYVAULT_FILE_INFO> listfiles2;
		std::vector<SDR_MYVAULT_FILE_INFO> listfiles3;
		std::vector<SDR_MYVAULT_FILE_INFO> listfiles4;
		std::vector<SDR_MYVAULT_FILE_INFO> listfiles5;

		string filter_s;
		vector<string> order_by = { "fileName","-fileName","creationTime","-creationTime", "size", "-size" };
		vector<string> filter = { "allShared","revoked", "activeTransaction","allFiles", "protected", "deleted" };
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			for (int i = 0; i < order_by.size(); i++) {
				for (int j = 0; j < filter.size(); j++) {
					int file_count;
					std::vector<SDR_MYVAULT_FILE_INFO> listfiles;
					orderBy = order_by[i];
					filter_s = filter[j];
					bool list_result = GetMyVaultFileListResponseData(to_string(pageId), to_string(pageSize), "", orderBy, filter_s);
					res = puser->GetMyVaultFiles(pageId, pageSize, orderBy, filter_s, listfiles);
					if (list_result) {
						Json::Value jroot;
						jroot = ParseJsonFile(myvaultfilelist_return);
						file_count = jroot["results"]["detail"]["totalFiles"].asInt();
						if (file_count == listfiles.size()) {
							for (int z = 0; z < file_count; z++) {
								bool is_delete = jroot["results"]["detail"]["files"][z]["deleted"].asBool();
								bool is_revoke = jroot["results"]["detail"]["files"][z]["revoked"].asBool();
								bool is_share = jroot["results"]["detail"]["files"][z]["shared"].asBool();
								uint64_t create_time = jroot["results"]["detail"]["files"][z]["sharedOn"].asInt64();
								string duid = jroot["results"]["detail"]["files"][z]["duid"].asString();
								uint64_t last_modified;
								string nxl_name = jroot["results"]["detail"]["files"][z]["name"].asString();
								string path_dis = jroot["results"]["detail"]["files"][z]["pathDisplay"].asString();
								string path_id = jroot["results"]["detail"]["files"][z]["pathId"].asString();
								string pro_id = jroot["results"]["detail"]["files"][z]["repoId"].asString();
								uint64_t share_time = jroot["results"]["detail"]["files"][z]["sharedOn"].asInt64();
								string recipients = listfiles[z].m_sharedwith;
								//string share_with = jroot["results"]["detail"]["files"][z]["sharedWith"].asString();
								uint64_t size = jroot["results"]["detail"]["files"][z]["size"].asInt64();
								string source_pathdis = jroot["results"]["detail"]["files"][z]["customMetadata"]["SourceFilePathDisplay"].asString();
								string source_pathid;
								string source_repoid;
								string source_reponame = jroot["results"]["detail"]["files"][z]["customMetadata"]["SourceRepoName"].asString();
								string sorce_retype = jroot["results"]["detail"]["files"][z]["customMetadata"]["SourceRepoType"].asString();
								int rights_size = jroot["results"]["detail"]["files"][z]["rights"].size();
								if (jroot["results"]["detail"]["files"][z]["rights"][rights_size - 1].asString() == "WATERMARK")
									rights_size = rights_size - 1;
								EXPECT_EQ(is_delete, listfiles[z].m_bdeleted);
								EXPECT_EQ(is_revoke, listfiles[z].m_brevoked);
								EXPECT_EQ(is_share, listfiles[z].m_bshared);
								//EXPECT_EQ(create_time, listfiles[z].m_creationtime);
								EXPECT_STREQ(duid.c_str(), listfiles[z].m_duid.c_str());
								EXPECT_STREQ(nxl_name.c_str(), listfiles[z].m_nxlname.c_str());
								EXPECT_STREQ(path_dis.c_str(), listfiles[z].m_pathdisplay.c_str());
								EXPECT_STREQ(path_id.c_str(), listfiles[z].m_pathid.c_str());
								EXPECT_STREQ(pro_id.c_str(), listfiles[z].m_repoid.c_str());
								EXPECT_EQ(share_time, listfiles[z].m_sharedon);
								//EXPECT_STREQ(share_with.c_str(), listfiles[z].m_sharedwith.c_str());
								EXPECT_EQ(size, listfiles[z].m_size);
								EXPECT_STREQ(source_pathdis.c_str(), listfiles[z].m_sourcefilepathdisplay.c_str());
								EXPECT_STREQ(source_reponame.c_str(), listfiles[z].m_sourcereponame.c_str());
								EXPECT_STREQ(sorce_retype.c_str(), listfiles[z].m_sourcerepotype.c_str());

								vector<SDRmFileRight> get_rights;
								res = puser->GetMyVaultFileRights(listfiles[z].m_duid.c_str(), listfiles[z].m_pathid.c_str(), get_rights);
								EXPECT_FALSE(res.GetCode());
								EXPECT_EQ(get_rights.size(), rights_size);
							}
						}
						else {
							cout << "File count not equals!" << endl;
							EXPECT_TRUE(false);
							return;
						}
					}
					else {
						cout << "Can not get myvault file list!" << endl;
						EXPECT_TRUE(false);
						return;
					}
				}

			}

			cout << "Test Finished!" << endl;
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//GetShareWithMe
TEST(ShareWithMeTest, ShareWithMe) {
	try {
		SDWLResult res;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;

		ISDRmUser *puser = NULL;
		ISDRmUser *puser_b = NULL;
		uint32_t pageId = 1;
		uint32_t pageSize = 100;
		//orderBy Should be fileName, creationTime, size
		string orderBy;

		vector<string> order = { "name","-name","size","-size","sharedBy", "-sharedBy", "sharedDate", "-sharedDate" };
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		if (login_result) {
			for (int i = 0; i < order.size(); i++) {
				orderBy = order[i];
				bool shrewithme_result = GetShareWithMeListResponseData(to_string(pageId), to_string(pageSize), orderBy);
				Json::Value jroot;
				jroot = ParseJsonFile(shrewithmefilelist_return);
				int file_count = jroot["results"]["detail"]["totalFiles"].asInt();
				vector<SDR_SHAREDWITHME_FILE_INFO> listfiles;
				res = puser->GetSharedWithMeFiles(pageId, pageSize, orderBy, "", listfiles);
				if (file_count) {
					if (file_count == listfiles.size()) {
						for (int j = 0; j < file_count; j++) {
							string  duid = jroot["results"]["detail"]["files"][j]["duid"].asString();
							string  nxlname = jroot["results"]["detail"]["files"][j]["name"].asString();
							string  filetype = jroot["results"]["detail"]["files"][j]["fileType"].asString();
							uint64_t	 size = jroot["results"]["detail"]["files"][j]["size"].asInt64();
							uint64_t     shareddate = jroot["results"]["detail"]["files"][j]["sharedDate"].asInt64();
							string  sharedby = jroot["results"]["detail"]["files"][j]["sharedBy"].asString();
							string  transactionid = jroot["results"]["detail"]["files"][j]["transactionId"].asString();
							string  transactioncode = jroot["results"]["detail"]["files"][j]["transactionCode"].asString();
							string  sharedlink = jroot["results"]["detail"]["files"][j]["sharedLink"].asString();
							int  rights_size = jroot["results"]["detail"]["files"][j]["rights"].size();
							string  comments;
							int count = jroot["results"]["detail"]["files"][j].size();
							if (count == 13) {
								comments = jroot["results"]["detail"]["files"][j]["sharedLink"].asString();
							}
							bool         isowner = jroot["results"]["detail"]["files"][j]["isOwner"].asBool();
							EXPECT_STREQ(duid.c_str(), listfiles[j].m_duid.c_str());
							EXPECT_STREQ(nxlname.c_str(), listfiles[j].m_nxlname.c_str());
							EXPECT_STREQ(filetype.c_str(), listfiles[j].m_filetype.c_str());
							EXPECT_EQ(size, listfiles[j].m_size);
							EXPECT_EQ(shareddate, listfiles[j].m_shareddate);
							EXPECT_STREQ(sharedby.c_str(), listfiles[j].m_sharedby.c_str());
							EXPECT_STREQ(transactionid.c_str(), listfiles[j].m_transactionid.c_str());
							EXPECT_STREQ(transactioncode.c_str(), listfiles[j].m_transactioncode.c_str());
							EXPECT_STREQ(sharedlink.c_str(), listfiles[j].m_sharedlink.c_str());
							string ri = listfiles[j].m_rights;
						}
					}
					else {
						cout << "The file count is diff!  " << endl;
						EXPECT_TRUE(false);
						return;
					}
				}
				else {
					cout << "There is no file in this user share with me file list!" << endl;
					EXPECT_TRUE(false);
					return;
				}
			}

			//res = puser->SharedWithMeDownloadFile(w_transactioncode, w_transactionid, L"C:\\auto\\output\\SDK\\Project", false);
			puser->LogoutUser();
			SDWLibDeleteRmcInstance(pInstance);
			cout << "Test Finished!" << endl;
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//My vault file metadata, MyVaultDownloadFile
TEST(MyVaultFileMetadataTest, FileMetadata) {
	try {
		SDR_Expiration expire;
		expire.type = IExpiryType::RANGEEXPIRE;
		expire.start = (uint64_t)std::time(nullptr) * 1000;
		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;

		SDWLResult res;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		wstring localfile_pathw = plan_file_path + txtnormal_testfile;

		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);

		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };

		ISDRmUser *puser = NULL;
		ISDRmUser *puser_b = NULL;
		wstring nxlfilepath;
		string pathid;

		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		if (login_result) {
			std::wstring filename;
			wstring recipients = L"123123@qq.com";
			wstring nxlfile_path;
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire);
			//res = puser->ProtectFile(paln_file_ws + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire);
			if (res.GetCode() == 0) {
				ISDRFiles* pFile = NULL;
				bool delete_result;
				//res = puser->GetLocalFileManager(&pFile);
				//size_t no = pFile->GetListNumber();
				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

				//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					filename = nxlfile->GetFileName();
					//Upload file
					res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comments Test!");
					EXPECT_FALSE(res.GetCode());
					//Upload activity log
					//res = puser->UploadActivityLogs();
					//EXPECT_FALSE(res.GetCode());

					bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
					if (file_result) {
						Json::Value jroot;
						jroot = ParseJsonFile(myvaultfilelist_return);
						pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
					}
					wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
					res = puser->MyVaultDownloadFile(pathid, downloaded_path, 2);
					string  name;
					string  fileLink;
					uint64_t     lastmodify;
					vector<SDRmFileRight>  rights;
					bool         shared;
					bool         deleted;
					bool         revoked;
					uint32_t     protectionType;
					uint64_t     protectedon;
					uint64_t     sharedon;
					std::vector<std::string> recipients;
					int recipients_count;
					SDR_Expiration expiration;
					if (!res.GetCode()) {
						SDR_FILE_META_DATA metadata;
						nxlfilepath = downloaded_path;
						res = puser->GetNXLFileMetadata(nxlfilepath, pathid, metadata);
						//res = puser->GetMyVaultFileRights();


						protectedon = metadata.protectedon;
						sharedon = metadata.sharedon;
						deleted = metadata.deleted;
						expiration = metadata.expiration;
						fileLink = metadata.fileLink;
						lastmodify = metadata.lastmodify;
						name = metadata.name;
						protectionType = metadata.protectionType;
						recipients = metadata.recipients;
						revoked = metadata.revoked;
						//rights = metadata.rights;
						shared = metadata.shared;
						recipients_count = recipients.size();

						bool metadata_result = GetMyVaultFileMetadataResponseData(ws2s(filename));
						string  name_json;
						string  fileLink_json;
						uint64_t     lastmodify_json;
						vector<string>  rights_json;
						bool         shared_json;
						bool         deleted_json;
						bool         revoked_json;
						uint32_t     protectionType_json;
						int recipients_json;
						uint64_t     start_json;
						uint64_t     end_json;
						uint64_t protecton_json;
						uint64_t shareon_json;
						if (metadata_result) {
							Json::Value jroot;
							jroot = ParseJsonFile(filemetadatalist_return);
							name_json = jroot["results"]["detail"]["name"].asString();
							fileLink_json = jroot["results"]["detail"]["fileLink"].asString();
							lastmodify_json = jroot["results"]["detail"]["protectedOn"].asInt64();
							protectionType_json = jroot["results"]["detail"]["protectionType"].asInt();
							start_json = jroot["results"]["detail"]["validity"]["startDate"].asInt64();
							end_json = jroot["results"]["detail"]["validity"]["endDate"].asInt64();
							shared_json = jroot["results"]["detail"]["shared"].asBool();
							deleted_json = jroot["results"]["detail"]["deleted"].asBool();
							revoked_json = jroot["results"]["detail"]["revoked"].asBool();
							recipients_json = jroot["results"]["detail"]["recipients"].size();
							protecton_json = jroot["results"]["detail"]["protectedOn"].asInt64();
							shareon_json = jroot["results"]["detail"]["sharedOn"].asInt64();
							EXPECT_STREQ(name_json.c_str(), name.c_str());
							EXPECT_STREQ(fileLink_json.c_str(), fileLink.c_str());
							EXPECT_EQ(lastmodify_json, lastmodify);
							EXPECT_EQ(protectionType_json, protectionType);
							EXPECT_EQ(revoked, revoked_json);
							EXPECT_EQ(deleted, deleted_json);
							EXPECT_EQ(shared, shared_json);
							EXPECT_EQ(recipients_count, recipients_json);
							EXPECT_EQ(start_json, expiration.start);
							EXPECT_EQ(end_json, expiration.end);
							EXPECT_EQ(protecton_json, protectedon);
							EXPECT_EQ(shareon_json, sharedon);
						}
					}
					else {
						cout << "Downloaded file failed!" << endl;
						EXPECT_TRUE(false);
					}
				}
				else {
					cout << "Upload file failed!" << endl;
					EXPECT_FALSE(res.GetCode());
				}
			}
			else {
				cout << "Share file failed!" << endl;
				EXPECT_TRUE(false);
			}
			res = pInstance->Save();
			puser->LogoutUser();
			SDWLibDeleteRmcInstance(pInstance);
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(SharedWithMeDownloadPartialFileTest, SharedWithMeDownloadPartialFileTest) {
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmUser *puser_zhu = NULL;
	ISDRmTenant *pTenant = NULL;
	bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	SDR_Expiration expire;
	expire.type = IExpiryType::NEVEREXPIRE;
	wstring localfile_pathw = plan_file_path + txtnormal_testfile;
	wstring nxlfile_path = L"";
	wstring transactioncode;
	wstring transactionid;
	wstring duid;
	string pathid;
	std::wstring filename;
	std::vector<SDRmFileRight> rights;
	rights.push_back(RIGHT_VIEW);
	rights.push_back(RIGHT_DOWNLOAD);

	string tag_text = "{}";
	wstring comments_w = L"test";
	wstring recipients = s2ws(email_zhu);
	SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };

	wstring targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
	wstring targert_folder2 = L"C:\\auto\\output\\SDK\\Project\\";
	//User A share file to User B
	res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
	if (res.GetCode() == 0) {
		ISDRFiles* pFile = NULL;

		bool delete_result;
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		bool revoked_owner;
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();

			//Upload file
			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comments Test!");
			EXPECT_EQ(res.GetCode(), 0);
		}
		else {
			cout << "Upload file failed!" << endl;
			EXPECT_FALSE(res.GetCode());
		}

		bool myvault_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "creationTime", "allFiles");
		Json::Value jroot;
		jroot = ParseJsonFile(myvaultfilelist_return);
		pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
	}
	else {
		cout << "Share file failed!" << endl;
		EXPECT_TRUE(false);
	}

	//User B login and download partial file
	login_result = SDKRPM_Login(pInstance, pTenant, puser_zhu, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
	if (!res.GetCode()) {
		vector<SDR_SHAREDWITHME_FILE_INFO> listfiles;
		res = puser_zhu->GetSharedWithMeFiles(1, 10, "sharedDate", ws2s(filename), listfiles);
		transactioncode = s2ws(listfiles[0].m_transactioncode);
		transactionid = s2ws(listfiles[0].m_transactionid);
		duid = s2ws(listfiles[0].m_duid);
		res = puser_zhu->SharedWithMeDownloadPartialFile(transactioncode, transactionid, targert_folder, true);
		EXPECT_EQ(res.GetCode(), 0);
		bool isfile_exist = file_exist(ws2s(targert_folder));
		EXPECT_TRUE(isfile_exist);
		bool revoked1;

		//When transactioncode is invalid
		res = puser_zhu->SharedWithMeDownloadPartialFile(L"123", transactionid, targert_folder, true);
		EXPECT_EQ(res.GetCode(), 400 + 61440);
		puser_zhu->LogoutUser();
	}

	//When file is invoked
	login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	bool revoke_result = InvokeSharedFileResponseData(ws2s(duid));
	puser->LogoutUser();

	login_result = SDKRPM_Login(pInstance, pTenant, puser_zhu, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
	res = puser_zhu->SharedWithMeDownloadPartialFile(transactioncode, transactionid, targert_folder2, true);
	EXPECT_EQ(res.GetCode(), 403 + 61440);
	EXPECT_STREQ(res.GetMsg().c_str(), "download failed");

	//when file is deleted
	login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	bool delete_result = DeleteMyvaultFile(ws2s(duid), pathid);
	login_result = Default_Login(pInstance, pTenant, puser_zhu, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
	res = puser_zhu->SharedWithMeDownloadPartialFile(transactioncode, transactionid, targert_folder2, true);
	EXPECT_EQ(res.GetCode(), 403 + 61440);
	EXPECT_STREQ(res.GetMsg().c_str(), "download failed");
}

TEST(MyVaultDownloadPartialFileTest, MyVaultDownloadPartialFile) {
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmUser *puser_zhu = NULL;
	ISDRmTenant *pTenant = NULL;
	bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	SDR_Expiration expire;
	expire.type = IExpiryType::NEVEREXPIRE;
	wstring localfile_pathw = plan_file_path + txtnormal_testfile;
	wstring nxlfile_path = L"";
	wstring transactioncode;
	wstring transactionid;
	string duid;
	string pathid;
	std::wstring filename;
	std::vector<SDRmFileRight> rights;
	rights.push_back(RIGHT_VIEW);
	rights.push_back(RIGHT_DOWNLOAD);

	string tag_text = "{}";
	wstring comments_w = L"test";
	wstring recipients = s2ws(email_zhu);
	SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };

	wstring targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
	wstring targert_folder2 = L"C:\\auto\\output\\SDK\\Project\\";
	//User A protect file
	res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
	if (res.GetCode() == 0) {
		ISDRFiles* pFile = NULL;

		bool delete_result;
		//res = puser->GetLocalFileManager(&pFile);
		//size_t no = pFile->GetListNumber();
		std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

		//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
		ISDRmNXLFile* nxlfile;
		res = puser->OpenFile(nxlfile_path, &nxlfile);
		if (nxlfile)
		{
			filename = nxlfile->GetFileName();

			//Upload file
			res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, L"Comments Test");
			EXPECT_EQ(res.GetCode(), 0);

			////Remove file
			//delete_result = pFile->RemoveFile(nxlfile);
			//EXPECT_TRUE(delete_result);

			bool myvault_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "creationTime", "allFiles");
			Json::Value jroot;
			jroot = ParseJsonFile(myvaultfilelist_return);
			pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
			duid = jroot["results"]["detail"]["files"][0]["duid"].asString();

			//Download protected file
			res = puser->MyVaultDownloadPartialFile(pathid, targert_folder, 2);
			EXPECT_EQ(res.GetCode(), 0);
			bool isfile_exist = file_exist(ws2s(targert_folder));
			EXPECT_TRUE(isfile_exist);

			ISDRmNXLFile* nxlfile;
			res = puser->OpenFile(targert_folder, &nxlfile);
			vector<SDRmFileRight> rights_nxl;
			rights_nxl = nxlfile->GetRights();
			EXPECT_EQ(rights_nxl.size(), rights.size());

			//When file is invoked
			targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
			bool revoke_result = InvokeSharedFileResponseData(duid);
			res = puser->MyVaultDownloadPartialFile(pathid, targert_folder, 2);
			EXPECT_EQ(res.GetCode(), 0);

			//When target folder is invalid
			wstring target_invalidfolder = L"C:\\invalidfolder\\";
			res = puser->MyVaultDownloadPartialFile(pathid, target_invalidfolder, 2);
			EXPECT_EQ(res.GetCode(), 3);

			//When path id  is invalid
			targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
			res = puser->MyVaultDownloadPartialFile("\123", targert_folder, 2);
			EXPECT_EQ(res.GetCode(), 400 + 61440);

			//when file is deleted
			targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
			bool delete_result = DeleteMyvaultFile(duid, pathid);
			res = puser->MyVaultDownloadPartialFile(pathid, targert_folder, 2);
			EXPECT_EQ(res.GetCode(), 500 + 61440);
		}
		else {
			cout << "Upload file failed!" << endl;
			EXPECT_FALSE(res.GetCode());
		}
	}
	else {
		cout << "Protect file failed!" << endl;
		ASSERT_TRUE(false);
	}
}

string fileName;
TEST(MyVaultAndWithMeShareFileTest, ShareFileTest1) {
	try {
		vector<std::string> recipients = { "3333@qwe.com", "2222@qq.com" };
		vector<std::string> recipient_shared = { email_zhu };
		vector<std::string> recipient_end = { email_zhu, "3333@qwe.com", "2222@qq.com" };
		string repositoryId;

		string filePathId;
		string filePath;
		string pathid;
		string comment = "Share file with you!";
		wstring downloaded_p = L"C:\\auto\\output\\SDK\\Project\\";
		SDWLResult res;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		ISDRmUser *puser = NULL;
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {

#pragma region[Prepare a MYVAULT nxl file]
			SDR_Expiration expire;
			expire.type = IExpiryType::NEVEREXPIRE;
			wstring localfile_pathw = plan_file_path + txtnormal_testfileb;

			std::vector<SDRmFileRight> rights;
			rights.push_back(RIGHT_VIEW);
			rights.push_back(RIGHT_SHARE);
			rights.push_back(RIGHT_EDIT);

			SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };
			std::wstring filename;
			wstring nxlfile_path;
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire);

			ISDRFiles* pFile = NULL;
			std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
			ISDRmNXLFile* nxlfile;
			res = puser->OpenFile(nxlfile_path, &nxlfile);
			if (nxlfile)
			{
				filename = nxlfile->GetFileName();
				res = puser->UploadFile(nxlfile_path, nxlfile_path);
			}
			else {
				cout << "No file generated!" << endl;
				EXPECT_FALSE(res.GetCode());
			}


			bool file_result = GetMyVaultFileListResponseData("1", "10", ws2s(filename), "fileName", "allFiles");
			if (file_result) {
				Json::Value jroot;
				jroot = ParseJsonFile(myvaultfilelist_return);
				pathid = jroot["results"]["detail"]["files"][0]["pathId"].asString();
			}

			res = puser->MyVaultDownloadFile(pathid, downloaded_p, 2);
#pragma endregion	

			std::vector<SDR_MYVAULT_FILE_INFO> listfiles;
			res = puser->GetMyVaultFiles(1, 100, "-creationTime", "all", listfiles);
			repositoryId = listfiles[0].m_repoid;
			filePathId = listfiles[0].m_pathid;
			fileName = listfiles[0].m_nxlname;
			filePath = listfiles[0].m_pathdisplay;
			//Share protected file
			res = puser->ShareFileFromMyVault(downloaded_p, recipients, repositoryId, fileName, filePathId, filePath, comment);

			bool metadate_result = GetMyVaultFileMetadataResponseData(ws2s(filename));
			Json::Value jroot;
			jroot = ParseJsonFile(filemetadatalist_return);
			int recipients_count = jroot["results"]["detail"]["recipients"].size();
			EXPECT_EQ(recipients_count, recipients.size());
			for (int i = 0; i < recipients.size(); i++) {
				string email_name = jroot["results"]["detail"]["recipients"][i].asString();
				EXPECT_STREQ(recipients[i].c_str(), email_name.c_str());
			}

			//Share shared file
			Sleep(1000);
			res = puser->ShareFileFromMyVault(downloaded_p, recipient_shared, repositoryId, fileName, filePathId, filePath, comment);
			metadate_result = GetMyVaultFileMetadataResponseData(ws2s(filename));
			jroot = ParseJsonFile(filemetadatalist_return);
			recipients_count = jroot["results"]["detail"]["recipients"].size();
			EXPECT_EQ(recipients_count, recipient_end.size());
			for (int i = 0; i < recipient_end.size(); i++) {
				string email_name = jroot["results"]["detail"]["recipients"][i].asString();
				EXPECT_STREQ(recipient_end[i].c_str(), email_name.c_str());
			}
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(MyVaultAndWithMeShareFileTest, ShareFileTest2) {
	try {
		SDWLResult res;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		ISDRmUser *puser = NULL;
		string transactionid;
		string transactioncode;
		string duid;
		string emaillist = "4444qq.com";
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		if (login_result)
		{
			vector<SDR_SHAREDWITHME_FILE_INFO> listfiles;
			res = puser->GetSharedWithMeFiles(1, 10, "name", fileName, listfiles);
			int count = listfiles.size();
			for (int i = 0; i < count; i++) {
				if (listfiles[i].m_nxlname == fileName)
				{
					transactioncode = listfiles[i].m_transactioncode;
					transactionid = listfiles[i].m_transactionid;
					duid = listfiles[i].m_duid;
					break;
				}
			}
			//when user with share rights
			res = puser->SharedWithMeReShareFile(transactionid, transactioncode, emaillist);
			EXPECT_EQ(res.GetCode(), 0);
		}
		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			bool metadate_result = GetMyVaultFileMetadataResponseData(fileName);
			Json::Value jroot;
			jroot = ParseJsonFile(filemetadatalist_return);
			int cout = jroot["results"]["detail"]["recipients"].size();
			EXPECT_EQ(cout, 4);

			bool revoke_result = InvokeSharedFileResponseData(duid);
		}
		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		if (login_result) {
			//when user without share rights
			res = puser->SharedWithMeReShareFile(transactionid, transactioncode, emaillist);
			EXPECT_EQ(res.GetCode(), 65441);
			EXPECT_STREQ(res.GetMsg().c_str(), "File has been revoked");
		}

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(ShareWithMeDownloadFileTest, ShareWithMeDownloadFileTest) {
	try {
		SDWLResult res;
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		ISDRmUser *puser = NULL;
		string transactionid;
		string transactioncode;
		string duid;
		wstring emaillist = L"4444@qq.com";
		wstring localfile_pathw = plan_file_path + txtnormal_testfile;
		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_SHARE);
		rights.push_back(RIGHT_DOWNLOAD);
		std::vector<std::string> recipients = { email };
		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		std::wstring filename;
		wstring download_path = L"C:\\auto\\output\\SDK\\Project\\";
		createFolder(ws2s(download_path));

		wstring nxlfile_path;
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		if (login_result) {
			res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, "{}");
			if (res.GetCode() == 0) {
				ISDRFiles* pFile = NULL;
				//res = puser->GetLocalFileManager(&pFile);
				//size_t no = pFile->GetListNumber();
				std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;

				//ISDRmNXLFile* nxlfile = pFile->GetFile(nxlfile_path);
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				if (nxlfile)
				{
					filename = nxlfile->GetFileName();
					res = puser->UploadFile(nxlfile_path, nxlfile_path, emaillist, L"Share file to you!");
					if (!res.GetCode()) {
						cout << "Upload file result!" << endl;
						EXPECT_FALSE(res.GetCode());
					}
					else {
						cout << "Upload file failed!" << endl;
						EXPECT_FALSE(res.GetCode());
					}
					//Remove file
					bool delete_result = pFile->RemoveFile(nxlfile);
					EXPECT_TRUE(delete_result);
				}
				else {
					cout << "No file generated!" << endl;
					EXPECT_FALSE(res.GetCode());
				}

			}
			res = puser->LogoutUser();
		}
		else {
			cout << "Login failed!" << endl;
			EXPECT_TRUE(login_result);
		}
		SDWLibDeleteRmcInstance(pInstance);

		uint32_t pageId = 1;
		uint32_t pageSize = 1000;
		//orderBy Should be fileName, creationTime, size
		string orderBy;

		std::vector<SDR_SHAREDWITHME_FILE_INFO> listfiles;

		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		if (login_result) {
			res = puser->GetSharedWithMeFiles(1, 10, "sharedDate", ws2s(filename), listfiles);
			string transactioncode = listfiles[0].m_transactioncode;
			string transactionid = listfiles[0].m_transactionid;

			res = puser->SharedWithMeDownloadFile(s2ws(transactioncode), s2ws(transactionid), download_path, false);
			bool exist_result = file_exist(ws2s(download_path));
			EXPECT_TRUE(exist_result);
		}
		else {
			cout << "Login failed!" << endl;
			EXPECT_TRUE(login_result);
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}