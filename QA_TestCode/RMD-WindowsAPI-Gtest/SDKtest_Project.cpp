#include "stdafx.h"
#include "SDLInc.h"

string project_clientID = "";
string project_testFolder = TestDirectory + "project-init";
string project_email = "lifi.yuan@nextlabs.com";
//string project_email = "472049994@qq.com";
string project_passcode = "";
string project_user_return = project_testFolder + "\\User_" + project_email + "_RMSreturn.json";
TEST(ProjectTest, init)
{
	string password = "123blue!";
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;

	createFolder(project_testFolder);
	md5(password);

	//res = CreateSDRmcInstance_CreateClientID(s2ws(project_testFolder).c_str(), pInstance, project_clientID, DEFUALT_ROUTER, DEFUALT_TENANT, DEFUALT_SDKLib_FOLDER);
	res = CreateSDRmcInstance_CreateClientID(s2ws(project_testFolder).c_str(), pInstance, project_clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
	GetUserLoginResponseData(project_email, password, project_clientID, project_user_return);
	cout << "SetLoginResult: ";
	res = pInstance->SetLoginResult(ReadFromFile(project_user_return), &puser, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	if (res) { project_passcode = puser->GetPasscode(); }
	SDWLibDeleteRmcInstance(pInstance);
}

class projecttestinit :public::testing::TestWithParam<vector<string>> {};
TEST_P(projecttestinit, listProjects)
{
	vector<string> csv = GetParam();
	string IsRun = csv[0];
	if (IsRun == "run" || IsRun == "1")
	{
#pragma region[read test data]
		string caseID = csv[1];
		int pageID = string2Int(csv[2]);
		int pageSize = string2Int(csv[3]);
		std::string orderBy = csv[4];
		if (orderBy == "null") orderBy = "";
		replaceA2B(orderBy, "|", ",");
		std::string searchValue = csv[5];
		if (searchValue == "null") searchValue = "";
		int filterType = string2Int(csv[6]);
		RM_ProjectFilter filter;
		switch (filterType) {
		case 1:
			filter = RF_All;
			break;
		case 2:
			filter = RF_OwnedByMe;
			break;
		case 3:
			filter = RF_OwnedByOther;
			break;
		default:
			filter = RF_All;
			break;
		}
		std::vector<SDR_PROJECT_INFO> projectsInfo;
		//std::vector<std::string> listfiles;
		vector<SDR_PROJECT_FILE_INFO> listfiles;

		ISDRmcInstance * pInstance;
		ISDRmUser *puser = NULL;
		SDWLResult res;
		wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
		//const char *temp = pclientID.c_str();
		//char *cid = new char[strlen(temp) + 1];
		//strcpy(cid, temp);
#pragma endregion
		res = CreateSDRmcInstance_CreateClientID(s2ws(project_testFolder).c_str(), pInstance, project_clientID, DEFUALT_ROUTER, DEFUALT_TENANT);
		
		cout << "GetLoginUser(): ";
		res = pInstance->GetLoginUser(project_email, project_passcode, &puser);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());


		cout << "GetProjectListFiles-for bugRMDC-165:";
		res = puser->GetProjectListFiles(1972, 1, 10, "", "/", "", listfiles);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//GetListProjects
		cout << "GetListProjects: ";
		res = puser->GetListProjects(pageID, pageSize, orderBy, filter);
		res = puser->GetListProjects(pageID + 1, pageSize, orderBy, filter);
		res = puser->GetListProjects(pageID+2, pageSize, orderBy, filter);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//GetProjectsInfo() 
		cout << "GetProjectsInfo: "<<endl;
		cout << "filter: " << filter << endl;
		projectsInfo = puser->GetProjectsInfo();
		int i = 0;
		string pathid = "";
		cout << "Case:[" << orderBy << "]:" << endl;
		for each (SDR_PROJECT_INFO pinfo in projectsInfo)
		{
			cout << "---Project number: " << i << "---" << endl;
			cout << "projid: " << pinfo.projid;
			cout << ", name: " << pinfo.name;
			cout << ", displayname: " << pinfo.displayname;
			cout << ", description: " << pinfo.description;
			cout << ", bowner: " << pinfo.bowner;
			cout << ", totalfiles: " << pinfo.totalfiles << endl;

			//GetProjectListFiles()
			cout << "GetProjectListFiles:";
			res = puser->GetProjectListFiles(pinfo.projid, 1, 10, "", "/", "", listfiles);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			int j = 1;
			if (res) {
				for each (SDR_PROJECT_FILE_INFO f in listfiles)
				{
					cout << "File" << j++ << ":" << endl;
					cout << "\t" << f.m_duid << endl;
					cout << "\t" << f.m_fileid << endl;
					cout << "\t" << f.m_nxlname << endl;
					cout << "\t" << f.m_pathdisplay << endl;
					cout << "\t" << f.m_pathid << endl;
					
					//ProjectDownloadFile()
					if (pinfo.name=="LifiProject-001") {
						pathid = f.m_pathid;
						transform(pathid.begin(), pathid.end(), pathid.begin(), ::tolower);//For bug RMCCORE-242
						cout << "ProjectDownloadFile: " << pathid;
						res = puser->ProjectDownloadFile(pinfo.projid, pathid, downloaded_path, Default_DownloadType);
						checkSDWLResult(res);
						EXPECT_EQ(0, res.GetCode());
					}
				}
			}
			i++;
		}
		SDWLibDeleteRmcInstance(pInstance);
		cout << "-------------This case finished--------" << endl;
	}
}
INSTANTIATE_TEST_CASE_P(ProjectTest, projecttestinit, testing::ValuesIn(csv_project));


string puser_return = "C:\\auto\\output\\SDK\\Project\\User1.txt";
const wstring ptempfolder = L"C:\\auto\\output\\SDK\\Project";
string pclientID = "";

class projecttest1 :public::testing::TestWithParam<vector<string>> {};
TEST_P(projecttest1, listProjects)
{
	vector<string> csv = GetParam();
	string IsRun = csv[0];
	if (IsRun == "run" || IsRun == "1")
	{
		string caseID = csv[1];
		int pageID = string2Int(csv[2]);
		int pageSize = string2Int(csv[3]);
		std::string orderBy = csv[4];
		if (orderBy == "null") orderBy = "";
		replaceA2B(orderBy, "|", ",");
		std::string searchValue = csv[5];
		if (searchValue == "null") searchValue = "";
		int filterType = string2Int(csv[6]);
		RM_ProjectFilter filter = RF_All;
		switch (filterType) {
		case 1:
			filter = RF_All;
			break;
		case 2:
			filter = RF_OwnedByMe;
			break;
		case 3:
			filter = RF_OwnedByOther;
			break;
		default:
			filter = RF_All;
			break;
		}
		std::vector<SDR_PROJECT_INFO> projectsInfo;
		std::vector<std::string> listfiles;

		ISDRmcInstance * pInstance;
		ISDRmUser *puser = NULL;
		SDWLResult res;

		const char *temp = pclientID.c_str();
		char *cid = new char[strlen(temp) + 1];
		strcpy(cid, temp);
		res = SDWLibCreateSDRmcInstance("Product name", 1, 1, 1, ptempfolder.c_str(), ptempfolder.c_str(), &pInstance, cid);
		res = pInstance->Initialize(DEFUALT_ROUTER, DEFUALT_TENANT);

		cout << "SetLoginResult: ";
		res = pInstance->SetLoginResult(ReadFromFile(puser_return), &puser, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//GetListProjects
		cout << "GetListProjects: ";
		res = puser->GetListProjects(pageID, pageSize, orderBy, filter);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//GetProjectsInfo() 
		cout << "GetProjectsInfo: " << endl;
		cout << "filter: " << filter << endl;
		projectsInfo = puser->GetProjectsInfo();
		int i = 0;
		for each (SDR_PROJECT_INFO pinfo in projectsInfo)
		{
			cout << "---Project number: " << i << "---" << endl;
			cout << "projid: " << pinfo.projid;
			cout << ", name: " << pinfo.name;
			cout << ", displayname: " << pinfo.displayname;
			cout << ", description: " << pinfo.description;
			cout << ", bowner: " << pinfo.bowner;
			cout << ", totalfiles: " << pinfo.totalfiles << endl;

			cout << "GetProjectListFiles:";
			std::vector<SDR_PROJECT_FILE_INFO> listfiles;
			res = puser->GetProjectListFiles(i, 1, 10, "", "/", "", listfiles);
			checkSDWLResult(res);
			EXPECT_EQ(0, res.GetCode());
			int j = 1;
			if (res.GetCode() == 0) {
				for each (SDR_PROJECT_FILE_INFO f in listfiles)
				{
					cout << "File" << j++ << ": " << f.m_pathid << endl;
				}
			}
			i++;
		}

		cout << "-------------This case finished--------" << endl;
	}
}
INSTANTIATE_TEST_CASE_P(ProjectTest, projecttest1, testing::ValuesIn(csv_project));

class projecttest2 :public::testing::TestWithParam<vector<string>> {};
TEST_P(projecttest2, listFiles)
{
	vector<string> csv = GetParam();
	string IsRun = csv[0];
	if (IsRun == "run" || IsRun == "1")
	{
		string caseID = csv[1];
		int projectID = string2Int(csv[2]);
		int pageID = string2Int(csv[3]);
		int pageSize = string2Int(csv[4]);
		std::string orderBy = csv[5];
		if (orderBy == "null") orderBy = "";
		replaceA2B(orderBy, "|", ",");
		string pathId = csv[6];
		std::string searchValue = csv[7];
		if (searchValue == "null") searchValue = "";
		std::vector<SDR_PROJECT_FILE_INFO> listfiles;


		ISDRmcInstance * pInstance;
		ISDRmUser *puser = NULL;
		SDWLResult res;

		const char *temp = pclientID.c_str();
		char *cid = new char[strlen(temp) + 1];
		strcpy(cid, temp);
		res = SDWLibCreateSDRmcInstance("Product name", 1, 1, 1, ptempfolder.c_str(), ptempfolder.c_str(), &pInstance, cid);
		res = pInstance->Initialize(DEFUALT_ROUTER, DEFUALT_TENANT);

		cout << "SetLoginResult: ";
		res = pInstance->SetLoginResult(ReadFromFile(puser_return), &puser, secr);
		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		//GetProjectListFiles
		cout << "GetProjectListFiles: ";

		res = puser->GetProjectListFiles(projectID, pageID, pageSize, orderBy, pathId, searchValue, listfiles);

		checkSDWLResult(res);
		EXPECT_EQ(0, res.GetCode());

		int i = 1;
		if (res.GetCode() == 0) {
			for each (SDR_PROJECT_FILE_INFO f in listfiles)
			{
				cout << "File" << i++ << ": " << f.m_pathid << endl;
			}
		}

		//ProjectDownloadFile()

	}
}
INSTANTIATE_TEST_CASE_P(ProjectTest, projecttest2, testing::ValuesIn(csv_projectFiles));

//Test project and file list
TEST(ProjectAndFileListTest, ProjectListAndFileTest) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		uint32_t pageId = 1;
		uint32_t pageSize = 100;
		std::string orderBy = "-name";
		vector<string> order = { "name","-name", "lastActionTime", "-lastActionTime" };
		RM_ProjectFilter filter = RF_All;
		string owedbyme;
		string page = to_string(pageId);
		string size = to_string(pageSize);
		int projectcount;
		int files_count;
		bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		//bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, "john.tyler", "john.tyler", "2");
		if (login_result) {
			for (RM_ProjectFilter f = RF_All; f <= RF_OwnedByOther; f = (RM_ProjectFilter)(f + 1)) {
				for (int i = 0; i < order.size(); i++) {
					orderBy = order[i];
					if (filter == RF_OwnedByMe)
					{
						owedbyme = "true";
					}
					else if (filter == RF_OwnedByOther) {
						owedbyme = "false";
					}
					else {
						owedbyme = "all";
					}
					//Get project list: "-name" "all"
					bool list_result = GetProjectListResponseData(page, size, orderBy, owedbyme);
					if (list_result) {
						Json::Value jroot;
						jroot = ParseJsonFile(projectlist_return);
						projectcount = jroot["results"]["totalProjects"].asInt();
						if (!projectcount) {
							cout << "There is no projects from this user!" << endl;
							EXPECT_TRUE(projectcount);
							return;
						}
						res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
						std::vector<SDR_PROJECT_INFO> project_info_list = puser->GetProjectsInfo();
						EXPECT_EQ(project_info_list.size(), projectcount);
						for (int y = 0; y < projectcount; y++) {
							EXPECT_STREQ(project_info_list[y].displayname.c_str(), jroot["results"]["detail"][y]["displayName"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].description.c_str(), jroot["results"]["detail"][y]["description"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].name.c_str(), jroot["results"]["detail"][y]["name"].asString().c_str());
							EXPECT_EQ(project_info_list[y].bowner, jroot["results"]["detail"][y]["ownedByMe"].asBool());
							EXPECT_STREQ(project_info_list[y].tokengroupname.c_str(), jroot["results"]["detail"][y]["tokenGroupName"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].parenttenantid.c_str(), jroot["results"]["detail"][y]["parentTenantId"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].parenttenantname.c_str(), jroot["results"]["detail"][y]["parentTenantName"].asString().c_str());
							EXPECT_EQ(project_info_list[y].projid, jroot["results"]["detail"][y]["id"].asInt());
							uint64_t   projid = project_info_list[y].projid;
							//Project file info test
							vector<string> orderby_files = { "creationTime","-creationTime", "name", "-name" };

							for (int z = 0; z < 4; z++) {
								string filelist_orderby = orderby_files[z];
								bool filelist_result = GetProjectFilelistResponseData(to_string(projid), page, size, filelist_orderby, "/", "");
								if (filelist_result) {
									Json::Value jroot_filelist;
									jroot_filelist = ParseJsonFile(projectfilelist_return);
									files_count = jroot_filelist["results"]["detail"]["totalFiles"].asInt();
									std::vector<SDR_PROJECT_FILE_INFO> listfiles;
									res = puser->GetProjectListFiles(projid, pageId, pageSize, filelist_orderby, "/", "", listfiles);
									if (files_count) {
										EXPECT_EQ(listfiles.size(), files_count);
										for (int j = 0; j < files_count; j++) {
											bool is_folder;
											Json::Value subjroot;
											subjroot = ParseJsonFile(projectfilelist_return);
											if (is_folder = subjroot["results"]["detail"]["files"][j]["folder"].asBool()) {
												cout << "Is folder: true!" << endl;
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["folder"].asBool(), listfiles[j].m_bfolder);
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["creationTime"].asInt64(), listfiles[j].m_created);
												EXPECT_STREQ("", listfiles[j].m_duid.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["id"].asString().c_str(), listfiles[j].m_fileid.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["lastModified"].asInt64(), listfiles[j].m_lastmodified);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["name"].asString().c_str(), listfiles[j].m_nxlname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["displayName"].asString().c_str(), listfiles[j].m_ownerdisplayname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["email"].asString().c_str(), listfiles[j].m_owneremail.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["owner"]["userId"].asInt64(), listfiles[j].m_ownerid);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathDisplay"].asString().c_str(), listfiles[j].m_pathdisplay.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathId"].asString().c_str(), listfiles[j].m_pathid.c_str());
												string path_id = listfiles[j].m_pathid;
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["size"].asInt64(), listfiles[j].m_size);
											}
											else {
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["folder"].asBool(), listfiles[j].m_bfolder);
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["creationTime"].asInt64(), listfiles[j].m_created);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["duid"].asString().c_str(), listfiles[j].m_duid.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["id"].asString().c_str(), listfiles[j].m_fileid.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["lastModified"].asInt64(), listfiles[j].m_lastmodified);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["name"].asString().c_str(), listfiles[j].m_nxlname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["displayName"].asString().c_str(), listfiles[j].m_ownerdisplayname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["email"].asString().c_str(), listfiles[j].m_owneremail.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["owner"]["userId"].asInt64(), listfiles[j].m_ownerid);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathDisplay"].asString().c_str(), listfiles[j].m_pathdisplay.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathId"].asString().c_str(), listfiles[j].m_pathid.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["size"].asInt64(), listfiles[j].m_size);
											}
										}
									}
									else {
										cout << "No files in this project" << endl;
									}
								}
							}
						}
					}
				}
			}
			res = puser->LogoutUser();
			cout << "Test finished!" << endl;
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
		SDWLibDeleteRmcInstance(pInstance);

		login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		if (login_result) {
			for (RM_ProjectFilter f = RF_All; f <= RF_OwnedByOther; f = (RM_ProjectFilter)(f + 1)) {
				for (int i = 0; i < order.size(); i++) {
					orderBy = order[i];
					if (filter == RF_OwnedByMe)
					{
						owedbyme = "true";
					}
					else if (filter == RF_OwnedByOther) {
						owedbyme = "false";
					}
					else {
						owedbyme = "all";
					}
					//Get project list: "-name" "all"
					bool list_result = GetProjectListResponseData(page, size, orderBy, owedbyme);
					if (list_result) {
						Json::Value jroot;
						jroot = ParseJsonFile(projectlist_return);
						projectcount = jroot["results"]["totalProjects"].asInt();
						if (!projectcount) {
							cout << "There is no projects from this user!" << endl;
							EXPECT_TRUE(projectcount);
							return;
						}
						res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
						std::vector<SDR_PROJECT_INFO> project_info_list = puser->GetProjectsInfo();
						EXPECT_EQ(project_info_list.size(), projectcount);
						for (int y = 0; y < projectcount; y++) {
							EXPECT_STREQ(project_info_list[y].displayname.c_str(), jroot["results"]["detail"][y]["displayName"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].description.c_str(), jroot["results"]["detail"][y]["description"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].name.c_str(), jroot["results"]["detail"][y]["name"].asString().c_str());
							EXPECT_EQ(project_info_list[y].bowner, jroot["results"]["detail"][y]["ownedByMe"].asBool());
							EXPECT_STREQ(project_info_list[y].tokengroupname.c_str(), jroot["results"]["detail"][y]["tokenGroupName"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].parenttenantid.c_str(), jroot["results"]["detail"][y]["parentTenantId"].asString().c_str());
							EXPECT_STREQ(project_info_list[y].parenttenantname.c_str(), jroot["results"]["detail"][y]["parentTenantName"].asString().c_str());
							EXPECT_EQ(project_info_list[y].projid, jroot["results"]["detail"][y]["id"].asInt());
							uint64_t   projid = project_info_list[y].projid;
							//Project file info test
							vector<string> orderby_files = { "creationTime","-creationTime", "name", "-name" };

							for (int z = 0; z < 4; z++) {
								string filelist_orderby = orderby_files[z];
								bool filelist_result = GetProjectFilelistResponseData(to_string(projid), page, size, filelist_orderby, "/", "");
								if (filelist_result) {
									Json::Value jroot_filelist;
									jroot_filelist = ParseJsonFile(projectfilelist_return);
									files_count = jroot_filelist["results"]["detail"]["totalFiles"].asInt();
									std::vector<SDR_PROJECT_FILE_INFO> listfiles;
									res = puser->GetProjectListFiles(projid, pageId, pageSize, filelist_orderby, "/", "", listfiles);
									if (files_count) {
										EXPECT_EQ(listfiles.size(), files_count);
										for (int j = 0; j < files_count; j++) {
											bool is_folder;
											Json::Value subjroot;
											subjroot = ParseJsonFile(projectfilelist_return);
											if (is_folder = subjroot["results"]["detail"]["files"][j]["folder"].asBool()) {
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["folder"].asBool(), listfiles[j].m_bfolder);
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["creationTime"].asInt64(), listfiles[j].m_created);
												EXPECT_STREQ("", listfiles[j].m_duid.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["id"].asString().c_str(), listfiles[j].m_fileid.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["lastModified"].asInt64(), listfiles[j].m_lastmodified);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["name"].asString().c_str(), listfiles[j].m_nxlname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["displayName"].asString().c_str(), listfiles[j].m_ownerdisplayname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["email"].asString().c_str(), listfiles[j].m_owneremail.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["owner"]["userId"].asInt64(), listfiles[j].m_ownerid);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathDisplay"].asString().c_str(), listfiles[j].m_pathdisplay.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathId"].asString().c_str(), listfiles[j].m_pathid.c_str());
												string path_id = listfiles[j].m_pathid;
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["size"].asInt64(), listfiles[j].m_size);
											}
											else {
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["folder"].asBool(), listfiles[j].m_bfolder);
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["creationTime"].asInt64(), listfiles[j].m_created);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["duid"].asString().c_str(), listfiles[j].m_duid.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["id"].asString().c_str(), listfiles[j].m_fileid.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["lastModified"].asInt64(), listfiles[j].m_lastmodified);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["name"].asString().c_str(), listfiles[j].m_nxlname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["displayName"].asString().c_str(), listfiles[j].m_ownerdisplayname.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["owner"]["email"].asString().c_str(), listfiles[j].m_owneremail.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["owner"]["userId"].asInt64(), listfiles[j].m_ownerid);
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathDisplay"].asString().c_str(), listfiles[j].m_pathdisplay.c_str());
												EXPECT_STREQ(subjroot["results"]["detail"]["files"][j]["pathId"].asString().c_str(), listfiles[j].m_pathid.c_str());
												EXPECT_EQ(subjroot["results"]["detail"]["files"][j]["size"].asInt64(), listfiles[j].m_size);
											}
										}
									}
									else {
										cout << "No files in this project" << endl;
									}
								}
							}
						}
					}
				}
			}

			res = puser->LogoutUser();
			cout << "Test finished!" << endl;
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
		SDWLibDeleteRmcInstance(pInstance);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//TEST(ProjectTest, GetProjectClassiification)
//{
//#pragma region[TestData]
//	struct data {
//		string caseName;
//		int projectId;
//	};
//	data d;
//	vector<data> vec;
//	for each(vector<string> csv_line in csv_project_ID)
//	{
//		if (csv_line[0] == "1") {
//			d.caseName = csv_line[1];
//			d.projectId = string2Int(csv_line[2]);
//			vec.push_back(d);
//		}
//	}
//#pragma endregion
//	SDWLResult res;
//	string passcode = "";
//	wstring tenantName = L"";
//	//string email= "lifi.yuan@nextlabs.com";
//	string email = "472049994@qq.com";
//	string password = "c27b83a22f39dceb435ed6d08ec42748";
//	md5(password);
//	string testFolder = TestDirectory + "GetProjectClassiification";
//	res = userFirstTimeLogin(email, password, testFolder, tenantName, passcode);
//
//	if (res) {
//		vector<SDR_CLASSIFICATION_CAT> categories;
//		ISDRmcInstance * pInstance;
//		ISDRmUser *puser = NULL;
//		ISDRmTenant *pTenant;
//		string clientId = "";
//		res = CreateSDRmcInstance_CreateClientID(s2ws(testFolder).c_str(), pInstance, clientId, DEFUALT_ROUTER, tenantName, DEFUALT_SDKLib_FOLDER);
//		//waitInstanceInitFinish(pInstance);
//
//		cout << "GetLoginUser(): ";
//		res = pInstance->GetLoginUser(email, passcode, &puser);
//		checkSDWLResult(res);
//		EXPECT_EQ(0, res.GetCode());
//
//		if (res)
//		{
//			cout << "GetListProjects: ";
//			res = puser->GetListProjects(1, 20, "", RF_All);
//			checkSDWLResult(res);
//			EXPECT_EQ(0, res.GetCode());
//			vector<SDR_PROJECT_INFO>  projectsInfo;
//			projectsInfo = puser->GetProjectsInfo();
//			string tena_id = projectsInfo[0].tenantid;
//			res = puser->GetClassification(tena_id, categories);
//			res = puser->GetClassification(tena_id, categories);
//
//			for each(data testcase in vec) {
//				categories.clear();
//				cout << "Case[" << testcase.caseName << "]:" << endl;
//				res = puser->GetClassification(tena_id, categories);
//				checkSDWLResult(res);
//				EXPECT_EQ(0, res.GetCode());
//				if (res) {
//					for each(SDR_CLASSIFICATION_CAT cat in categories)
//					{
//						cout << "\"name\": " << cat.name << endl;
//						cout << "\"multiSelect\": " << cat.multiSelect << endl;
//						cout << "\"mandatory\": " << cat.mandatory << endl;
//						cout << "\"labels\": [" << endl;
//						for each (SDR_CLASSIFICATION_LABELS lab in cat.labels)
//						{
//							cout << "{" << endl;
//							cout << "\"name\": " << lab.name << endl;
//							cout << "\"default\": " << lab.allow << endl;
//							cout << "}" << endl;
//						}
//						cout << "]" << endl;
//					}
//				}
//			}
//		}
//		SDWLibDeleteRmcInstance(pInstance);
//		cout << "-------------This case finished--------" << endl;
//	}
//}

TEST(GetClassification, GetClassification) {
	ISDRmcInstance *pInstance = NULL;
	ISDRmTenant *pTenant = NULL;
	ISDRmUser *puser = NULL;
	string email1 = "472049994@qq.com";
	string password1 = "c27b83a22f39dceb435ed6d08ec42748";
	vector<SDR_CLASSIFICATION_CAT> categories1;
	vector<SDR_CLASSIFICATION_CAT> categories2;
	SDWLResult res;
	uint32_t pageId = 1;
	uint32_t pageSize = 10;
	std::string orderBy = "name";
	RM_ProjectFilter filter = RF_All;


	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
	if (login_result) {
		res = puser->GetHeartBeatInfo();
		res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
		//res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
		//res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
		std::vector<SDR_PROJECT_INFO> project_info_list = puser->GetProjectsInfo();
		project_info_list = puser->GetProjectsInfo();
		std::vector<SDR_PROJECT_INFO> project_info_list1 = puser->GetProjectsInfo();

		string name = project_info_list[0].name;
		bool owner = project_info_list[0].bowner;
		uint32_t project_id = project_info_list[0].projid;
		string   displayname = project_info_list[0].displayname;
		string   description = project_info_list[0].description;
		uint64_t	  totalfiles = project_info_list[0].totalfiles;
		//string   tenantid = project_info_list[0].tenantid;
		std::vector<SDR_PROJECT_FILE_INFO> listfiles;
		res = puser->GetProjectListFiles(project_info_list[0].projid, 1, 20, orderBy, "/", "", listfiles);
		std::vector<SDR_PROJECT_FILE_INFO> listfiles1;
		res = puser->GetProjectListFiles(project_info_list[0].projid, 1, 20, orderBy, "/123123/", "", listfiles1);

		string name1 = project_info_list[1].name;
		bool owner1 = project_info_list[1].bowner;
		uint32_t project_id1 = project_info_list[1].projid;
		string   displayname1 = project_info_list[1].displayname;
		string   description1 = project_info_list[1].description;
		uint64_t	  totalfiles1 = project_info_list[1].totalfiles;
		string   tokengroupname = project_info_list[1].tokengroupname;
		std::vector<SDR_PROJECT_FILE_INFO> listfiles11;
		res = puser->GetProjectListFiles(project_info_list[0].projid, 1, 20, orderBy, "/aaa/mmm/uuu/", "", listfiles11);

		string name2 = project_info_list[2].name;
		bool owner2 = project_info_list[2].bowner;
		uint32_t project_id2 = project_info_list[2].projid;
		string   displayname2 = project_info_list[2].displayname;
		string   description2 = project_info_list[2].description;
		uint64_t	  totalfiles2 = project_info_list[2].totalfiles;
		string   tokengroupname2 = project_info_list[2].tokengroupname;


		res = puser->GetClassification(tokengroupname2, categories1);
		if (res) {
			for each(SDR_CLASSIFICATION_CAT cat in categories1)
			{
				cout << "\"name\": " << cat.name << endl;
				cout << "\"multiSelect\": " << cat.multiSelect << endl;
				cout << "\"mandatory\": " << cat.mandatory << endl;
				cout << "\"labels\": [" << endl;
				for each (SDR_CLASSIFICATION_LABELS lab in cat.labels)
				{
					cout << "{" << endl;
					cout << "\"name\": " << lab.name << endl;
					cout << "\"default\": " << lab.allow << endl;
					cout << "}" << endl;
				}
				cout << "]" << endl;
			}
		}
		res = puser->GetClassification(tokengroupname2, categories2);

		res = puser->LogoutUser();
	}
	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl;

	login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);

	if (login_result) {
		res = puser->GetListProjects(pageId, pageSize, orderBy, filter);
		std::vector<SDR_PROJECT_INFO> project_info_list = puser->GetProjectsInfo();
		res = puser->GetClassification(project_info_list[0].tokengroupname, categories1);
		if (res) {
			for each(SDR_CLASSIFICATION_CAT cat in categories1)
			{
				cout << "\"name\": " << cat.name << endl;
				cout << "\"multiSelect\": " << cat.multiSelect << endl;
				cout << "\"mandatory\": " << cat.mandatory << endl;
				cout << "\"labels\": [" << endl;
				for each (SDR_CLASSIFICATION_LABELS lab in cat.labels)
				{
					cout << "{" << endl;
					cout << "\"name\": " << lab.name << endl;
					cout << "\"default\": " << lab.allow << endl;
					cout << "}" << endl;
				}
				cout << "]" << endl;
			}
		}
		res = puser->GetClassification(project_info_list[0].tokengroupname, categories2);

		res = puser->LogoutUser();
	}
	SDWLibDeleteRmcInstance(pInstance);
	cout << "-------------This case finished--------" << endl;
}

bool member_emailzhu_exist = false;
wstring projectdownloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
string profile_pathid;
int p_id;
//Test Upload and download project files, and get project file metadata
TEST(ProjectUploadFileTest, UploadFileTest) {
	try {
		ISDRmcInstance *pInstance = NULL;
		ISDRmTenant *pTenant = NULL;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		ISDRmUser *puser_b = NULL;
		uint32_t pageId = 1;
		uint32_t pageSize = 10;
		std::string orderBy = "name";
		RM_ProjectFilter filter = RF_All;
		SDR_Expiration expire;
		expire.type = IExpiryType::RANGEEXPIRE;
		expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000 + 604800000;
		expire.start = (uint64_t)std::time(nullptr) * 1000 + 604800000;
		SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };
		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_DOWNLOAD);

		std::vector<SDRmFileRight> userb_rights;
		userb_rights.push_back(RIGHT_VIEW);
		userb_rights.push_back(RIGHT_PRINT);
		userb_rights.push_back(RIGHT_DOWNLOAD);

		string tag_text = "{}";
		wstring nxlfile_path;
		wstring file_name;
		string mem_id;


		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email_zhu, password);
		if (login_result) {
			res = puser->GetListProjects(1, 100, orderBy, filter);
			if (!res.GetCode()) {
				std::vector<SDR_PROJECT_INFO> project_info_list = puser->GetProjectsInfo();
				if (project_info_list.size()) {
					p_id = project_info_list[1].projid;
					mem_id = puser->GetMembershipID(p_id);
				}
				else {
					cout << "No projects from this user" << endl;
					EXPECT_TRUE(false);
					return;

				}
			}
			else {
				cout << "List project failed!!" << endl;
			}
			//Protect file with ad-hoc rights
			res = puser->ProtectFile(plan_file_path + L"Small.txt", nxlfile_path, rights, watermark_info, expire, "", mem_id);
			bool list_result = GetProjectListResponseData("1", "100", "name", "all");
			if (!res.GetCode()) {
				ISDRFiles* pFile = NULL;
				bool delete_result;
				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				file_name = nxlfile->GetFileName();
				//Rights verified
				vector<SDRmFileRight> rights_v = nxlfile->GetRights();
				EXPECT_EQ(rights_v.size(), rights.size());
				bool rights_result = rights_verified(rights, nxlfile);
				EXPECT_TRUE(rights_result);

				bool has_watermark = nxlfile->CheckRights(RIGHT_WATERMARK);

				res = puser->UploadProjectFile(p_id, L"/", nxlfile);
				Sleep(3000);
				if (!res.GetCode()) {
					std::vector<SDR_PROJECT_FILE_INFO> listfiles;
					res = puser->GetProjectListFiles(p_id, 1, 20, "-creationTime", "/", ws2s(file_name), listfiles);
					if (!res.GetCode()) {
						profile_pathid = listfiles[0].m_pathid;
						//Download from root

						res = puser->ProjectDownloadFile(p_id, profile_pathid, projectdownloaded_path, RD_ForOffline);
						std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
						res = puser->GetRights(projectdownloaded_path, rightsAndWatermarks);
						EXPECT_EQ(11, rightsAndWatermarks.size());

						SDR_FILE_META_DATA metadata;
						res = puser->GetProjectFileMetadata(projectdownloaded_path, to_string(p_id), profile_pathid, metadata);
						std::vector<SDRmFileRight> filerights;
						res = puser->GetProjectFileRights(to_string(p_id), profile_pathid, filerights);
						EXPECT_EQ(rights.size(), filerights.size());

						bool metadata_result = GetProjectFileMetadataResponseData(to_string(p_id), profile_pathid);
						if (metadata_result) {
							Json::Value jroot;
							jroot = ParseJsonFile(projectfilemetadatalist_return);
							//File owner get file rights
							int rights_count = jroot["results"]["fileInfo"]["rights"].size();
							EXPECT_EQ(11, rightsAndWatermarks.size());
							string f_name = jroot["results"]["fileInfo"]["name"].asString();
							string f_pathdisplay = jroot["results"]["fileInfo"]["pathDisplay"].asString();
							string f_pathid = jroot["results"]["fileInfo"]["pathId"].asString();
							string f_filetype = jroot["results"]["fileInfo"]["fileType"].asString();
							uint64_t f_lastModified = jroot["results"]["fileInfo"]["lastModified"].asInt64();
							int f_size = jroot["results"]["fileInfo"]["size"].asInt();
							bool f_owner = jroot["results"]["fileInfo"]["owner"].asBool();
							bool f_nxl = jroot["results"]["fileInfo"]["nxl"].asBool();
							int f_protectiontype = jroot["results"]["fileInfo"]["protectionType"].asInt();
							uint64_t f_expireend = jroot["results"]["fileInfo"]["expiry"]["endDate"].asInt64();
							uint64_t f_expirestart = jroot["results"]["fileInfo"]["expiry"]["startDate"].asInt64();

							uint64_t m_start = metadata.expiration.start;
							uint64_t m_end = metadata.expiration.end;
							uint64_t m_lastmodified = metadata.lastmodify;
							string m_name = metadata.name;
							bool m_nxl = metadata.nxl;
							bool m_owner = metadata.owner;
							string m_pathdisplay = metadata.pathDisplay;
							string m_pathid = metadata.pathid;
							int m_protectionType = metadata.protectionType;

							EXPECT_EQ(m_start, expire.start);
							EXPECT_EQ(m_end, expire.end);
							EXPECT_STREQ(f_name.c_str(), m_name.c_str());
							EXPECT_EQ(m_lastmodified, f_lastModified);
							EXPECT_EQ(m_nxl, f_nxl);
							EXPECT_EQ(m_owner, f_owner);
							EXPECT_STREQ(m_pathdisplay.c_str(), f_pathdisplay.c_str());
							EXPECT_STREQ(m_pathid.c_str(), f_pathid.c_str());
							EXPECT_EQ(m_protectionType, f_protectiontype);
						}
						else {
							cout << "Get Project file metadata failed!!" << endl;
							EXPECT_TRUE(false);
							return;
						}
					}
					else {
						cout << "Upload file to project failed!!" << endl;
						EXPECT_TRUE(false);
						return;
					}
				}
				else {
					cout << "Upload file to project failed!!" << endl;
					EXPECT_TRUE(false);
					return;
				}
				res = puser->LogoutUser();
				cout << "Test finished!" << endl;
			}
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
		SDWLibDeleteRmcInstance(pInstance);

		login_result = Default_Login(pInstance, pTenant, puser_b, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		if (login_result) {
			res = puser_b->GetListProjects(1, 100, orderBy, filter);
			if (!res.GetCode()) {
				std::vector<SDR_PROJECT_INFO> project_info_list = puser_b->GetProjectsInfo();
				if (project_info_list.size()) {
					mem_id = puser_b->GetMembershipID(p_id);
				}
				else {
					cout << "No projects from this user" << endl;
					EXPECT_TRUE(false);
					return;
				}
			}
			else {
				cout << "List project failed!!" << endl;
			}
			//Protect file with ad-hoc rights
			res = puser_b->ProtectFile(plan_file_path + L"Small.txt", nxlfile_path, userb_rights, watermark_info, expire, "", mem_id);
			bool list_result = GetProjectListResponseData("1", "100", "name", "all");
			if (!res.GetCode()) {
				ISDRFiles* pFile = NULL;
				bool delete_result;

				ISDRmNXLFile* nxlfile;
				res = puser->OpenFile(nxlfile_path, &nxlfile);
				file_name = nxlfile->GetFileName();
				//Rights verified
				vector<SDRmFileRight> rights_v = nxlfile->GetRights();
				EXPECT_EQ(rights_v.size(), userb_rights.size());
				bool rights_result = rights_verified(userb_rights, nxlfile);
				EXPECT_TRUE(rights_result);

				bool has_watermark = nxlfile->CheckRights(RIGHT_WATERMARK);

				res = puser_b->UploadProjectFile(p_id, L"/", nxlfile);
				Sleep(3000);
				//Remove file
				delete_result = pFile->RemoveFile(nxlfile);
				EXPECT_TRUE(delete_result);
				if (!res.GetCode()) {
					std::vector<SDR_PROJECT_FILE_INFO> listfiles;
					res = puser_b->GetProjectListFiles(p_id, 1, 20, "-creationTime", "/", ws2s(file_name), listfiles);
					if (!res.GetCode()) {
						string userfile_pathid = listfiles[0].m_pathid;
						//Download from root
						wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
						res = puser_b->ProjectDownloadFile(p_id, userfile_pathid, downloaded_path, RD_ForOffline);
						wstring download_nxlfilepath = downloaded_path;
						std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
						puser_b->GetRights(downloaded_path, rightsAndWatermarks);
						EXPECT_EQ(11, rightsAndWatermarks.size());

					}
					else {
						cout << "Upload file to project failed!!" << endl;
						EXPECT_TRUE(false);
						return;
					}
				}
				else {
					cout << "Upload file to project failed!!" << endl;
					checkSDWLResult(res);
					EXPECT_TRUE(false);
					return;
				}
				res = puser_b->LogoutUser();
				cout << "Test finished!" << endl;
			}
		}
		else {
			EXPECT_TRUE(0);
			cout << "Login failed!" << endl;
		}
		SDWLibDeleteRmcInstance(pInstance);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

TEST(ProjectUploadFileTest, UploadFileTest2) {
	ISDRmcInstance *pInstance = NULL;
	ISDRmTenant *pTenant = NULL;
	SDWLResult res;
	ISDRmUser *puser = NULL;

	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email_zhu, password);

	if (login_result) {
		SDR_FILE_META_DATA metadata;
		res = puser->GetProjectFileMetadata(to_string(p_id), profile_pathid, metadata);
		EXPECT_FALSE(res.GetCode());
		std::vector<SDRmFileRight> filerights;
		res = puser->GetProjectFileRights(to_string(p_id), profile_pathid, filerights);
		EXPECT_EQ(2, filerights.size());

	}
}
//User C
TEST(ProjectUploadFileTest, UploadFileTest3) {
	ISDRmcInstance *pInstance = NULL;
	ISDRmTenant *pTenant = NULL;
	SDWLResult res;
	ISDRmUser *puser = NULL;

	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", "jewelry.zhu@nextlabs.com", password);

	if (login_result) {
		SDR_FILE_META_DATA metadata;
		res = puser->GetProjectFileMetadata("27", "/small-2019-03-06-09-25-10.txt.nxl", metadata);
		EXPECT_FALSE(res.GetCode());
		std::vector<SDRmFileRight> filerights;
		res = puser->GetProjectFileRights(to_string(p_id), profile_pathid, filerights);
		EXPECT_EQ(2, filerights.size());

	}
}

////Tag
//TEST(ProjectMetadataTagTest, MetadataTest) {
//	ISDRmcInstance *pInstance = NULL;
//	ISDRmTenant *pTenant = NULL;
//	SDWLResult res;
//	ISDRmUser *puser = NULL;
//
//	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email, password);
//	if (login_result) {
//		SDR_FILE_META_DATA metadata;
//		wstring filepath = plan_file_path + L"CPviewEDIT-2019-01-25-08-12-01.txt.nxl";
//		wstring tagfilepath = plan_file_path + L"test code-2019-01-25-08-30-25.txt.nxl";
//		wstring ad_path = plan_file_path + L"qqqqqqqqq-2019-01-25-09-37-13.txt.nxl";
//		res = puser->GetProjectFileMetadata(filepath, "13", "/cp/cpviewedit-2018-11-29-08-29-20.txt.nxl", metadata);
//		EXPECT_FALSE(res.GetCode());
//
//		res = puser->GetProjectFileMetadata(ad_path, "13", "/ad/qqqqqqqqq-2019-01-02-06-57-53.txt.nxl", metadata);
//		EXPECT_FALSE(res.GetCode());
//		res = puser->GetProjectFileMetadata(tagfilepath, "13", "/cp/test code-2019-01-25-08-30-13.txt.nxl", metadata);
//		EXPECT_FALSE(res.GetCode());
//		std::vector<SDRmFileRight> filerights;
//		std::vector<SDRmFileRight> filerights1;
//		res = puser->GetProjectFileRights("13", "/cp/cpviewedit-2018-11-29-08-29-20.txt.nxl", filerights);
//		EXPECT_EQ(3, filerights.size());
//		res = puser->GetProjectFileRights("13", "/cp/test code-2019-01-25-08-30-13.txt.nxl", filerights1);
//		EXPECT_EQ(3, filerights.size());
//	}
//	else {
//		EXPECT_TRUE(0);
//		cout << "Login failed!" << endl;
//	}
//	SDWLibDeleteRmcInstance(pInstance);
//}

TEST(ProjectDownloadPartialFile, ProjectDownloadPartialFileTest) {
	try {
		ISDRmcInstance * pInstance;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		ISDRmUser *puser_zhu = NULL;
		ISDRmTenant *pTenant = NULL;
		bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, L"", email, password);
		ASSERT_TRUE(login_result);
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring localfile_pathw = plan_file_path + txtnormal_testfile;
		wstring nxlfile_path = L"";
		wstring transactioncode;
		wstring transactionid;
		std::vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		rights.push_back(RIGHT_DOWNLOAD);
		string tag_text = "{}";
		wstring comments_w = L"test";
		wstring recipients = s2ws(email_zhu);
		SDR_WATERMARK_INFO watermark_info = { "Value1:Text1","font","color",9,0,ANTICLOCKWISE,true };

		wstring targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
		wstring targert_cpfolder = L"C:\\auto\\output\\SDK\\Project\\";
		wstring targert_userb_folder = L"C:\\auto\\output\\SDK\\Project\\";
		//User A share file to User B
		res = puser->ProtectFile(localfile_pathw, nxlfile_path, rights, watermark_info, expire, tag_text);
		if (res.GetCode() == 0) {
			ISDRFiles* pFile = NULL;
			std::wstring filename;
			bool delete_result;
			//res = puser->GetLocalFileManager(&pFile);
			//size_t no = pFile->GetListNumber();
			std::vector<SDR_FILE_ACTIVITY_INFO> infoVec;
			ISDRmNXLFile* nxlfile;
			res = puser->OpenFile(nxlfile_path, &nxlfile);
			//ISDRmNXLFile* nxlfile = pFile->GetFile(no - 1);
			if (nxlfile)
			{
				filename = nxlfile->GetFileName();

				//Upload file
				res = puser->UploadFile(nxlfile_path, nxlfile_path, recipients, comments_w);
				EXPECT_EQ(res.GetCode(), 0);
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
		//puser->LogoutUser();

		if (!res.GetCode()) {
			//User A ad hoc file
			res = puser->ProjectDownloadPartialFile(6, "/222/demo-2019-06-13-09-44-28.xlsx.nxl", targert_folder, RD_ForOffline);
			EXPECT_EQ(res.GetCode(), 0);
			bool isfile_exist = file_exist(ws2s(targert_folder));
			EXPECT_TRUE(isfile_exist);
			ISDRmNXLFile* nxlfile;
			res = puser->OpenFile(targert_folder, &nxlfile);
			vector<SDRmFileRight> rights_nxl;
			rights_nxl = nxlfile->GetRights();
			//User A cp file
			res = puser->ProjectDownloadPartialFile(6, "/222/jewelry-2019-06-13-09-50-52.docx.nxl", targert_cpfolder, RD_ForOffline);
			EXPECT_EQ(res.GetCode(), 0);
		}

		login_result = Default_Login(pInstance, pTenant, puser_zhu, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
		//User B ad hoc file
		targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
		res = puser->ProjectDownloadPartialFile(6, "/222/demo-2019-06-13-09-44-28.xlsx.nxl", targert_folder, RD_ForViewer);
		EXPECT_EQ(res.GetCode(), 0);
		//User B cp file
		targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
		res = puser->ProjectDownloadPartialFile(6, "/222/jewelry-2019-06-13-09-50-52.docx.nxl", targert_folder, RD_ForViewer);
		EXPECT_EQ(res.GetCode(), 0);
		//User B without download rights, ad hoc file
		targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
		res = puser_zhu->ProjectDownloadPartialFile(6, "/222/withoutsharerights-2019-06-13-09-53-34.txt.nxl", targert_folder, RD_ForViewer);
		EXPECT_EQ(res.GetCode(), 0);
		//when file has been deleted
		targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
		res = puser_zhu->ProjectDownloadPartialFile(6, "/222/header-2019-06-13-09-54-24.txt.nxl", targert_folder, RD_Normal);
		EXPECT_EQ(res.GetCode(), 404 + 61440);
		//When target foler not exist
		targert_folder = L"C:\\auto\\output\\SDK\\Project\\SubFolder\\";
		res = puser_zhu->ProjectDownloadPartialFile(6, "/ad/test code-2019-02-21-06-47-46.txt.nxl", targert_folder, RD_Normal);
		EXPECT_EQ(res.GetCode(), 3);
		//When path id is invalid
		targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
		res = puser_zhu->ProjectDownloadPartialFile(6, "/ad/invalid-2019-02-21-07-15-09.png.nxl", targert_folder, RD_Normal);
		EXPECT_EQ(res.GetCode(), 404 + 61440);
		//When project id is invalid
		targert_folder = L"C:\\auto\\output\\SDK\\Project\\";
		res = puser_zhu->ProjectDownloadPartialFile(123, "test code-2019-02-21-06-47-46.txt.nxl", targert_folder, RD_Normal);
		EXPECT_EQ(res.GetCode(), 400 + 61440);

	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

//System project adhoc file test
TEST(ProjectSystemProtectFileTest, SystemProjectTest) {
	ISDRmcInstance * pInstance;
	ISDRmTenant * pTenant;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	wstring nxlfile_path;

	SDR_Expiration expire;
	expire.type = IExpiryType::NEVEREXPIRE;
	string tenant = ws2s(DEFUALT_TENANT);
	string tag_text = "{}";
	wstring w_name;
	wstring localfile_pathw = plan_file_path + bmpnormal_testfile;
	std::vector<SDRmFileRight> rights;
	rights.push_back(RIGHT_VIEW);
	rights.push_back(RIGHT_DOWNLOAD);

	SDR_WATERMARK_INFO watermark_info = { "","font","color",9,0,ANTICLOCKWISE,true };

	bool ret = false;
	wstring safe_folder = L"C:\\SDK test\\RPM\\Dir7";
	string passcode = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

	bool login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);

	//if (login_result) {
	//	res = puser->GetHeartBeatInfo();

	//	bool adhoc;
	//	int heartbeat;
	//	string modified;
	//	string tenan_str;
	//	//= "27aed3b5-e3bd-4bec-a7c0-c0b3e988e82d";
	//	ISDRmNXLFile * file;
	//	vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
	//	int sysprojectid;
	//	string sysprojecttenantid;

	//	res = puser->GetListProjects(1, 10, "-lastActionTime", RF_All);
	//	res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid, tenan_str);
	//	string default_tenant = puser->GetDefaultTenantId();
	//	string mem_id = puser->GetMembershipID(sysprojectid);
	//	string mem_id1 = puser->GetMembershipID("3bcfe071-5d5c-4856-98ca-2547bd29a292");
	//	//string tenant_id = puser->GetSystemProjectTenantId();
	//	vector<SDR_CLASSIFICATION_CAT> cats;
	//	res = puser->GetClassification("3bcfe071-5d5c-4856-98ca-2547bd29a292", cats);

	//	res = puser->ProtectFile(paln_file_ws + txtnormal_testfile, nxlfile_path, rights, watermark_info, expire, tag_text, mem_id);
	//	res = puser->GetRights(nxlfile_path, rightsAndWatermarks, false);
	//	EXPECT_EQ(rightsAndWatermarks.size(), 10);

	//	vector<SDRmFileRight> filerights;
	//	res = pInstance->RPMGetFileRights(nxlfile_path, filerights);

	//	res = puser->LogoutUser();
	//}

	//login_result = Default_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email_zhu, password);
	//if (login_result) {
	//	res = puser->GetHeartBeatInfo();
	//	bool adhoc;
	//	int heartbeat;
	//	string modified;
	//	string tenan_str;
	//	int sysprojectid;
	//	string sysprojecttenantid;
	//	res = puser->GetTenantPreference(&adhoc, &heartbeat, &sysprojectid, sysprojecttenantid, tenan_str);
	//	vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
	//	res = puser->GetRights(nxlfile_path, rightsAndWatermarks, false);
	//	EXPECT_EQ(rightsAndWatermarks.size(), 10);

	//	vector<SDRmFileRight> filerights;
	//	res = pInstance->RPMGetFileRights(nxlfile_path, filerights);

	//	res = puser->LogoutUser();
	//}
}

TEST(ProjectChangeRightsTest, ChangeRightsOfProjectFileTest) {
	try {
		ISDRmcInstance * pInstance;
		ISDRmTenant * pTenant;
		SDWLResult res;
		ISDRmUser *puser = NULL;
		wstring nxlfilepath = L"";
		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		std::vector<SDRmFileRight> rights;
		SDR_WATERMARK_INFO watermarkinfo;
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		//expire.end = (uint64_t)std::time(nullptr) * 1000 + 604800000;
		//expire.start = (uint64_t)std::time(nullptr) * 1000;
		//std::string tags = "{\"cy-all-rights\":[\"no-share-rights\"]}";
		string tags = "{\"cy-all-rights\":[\"no-edit-right\"]}";

		bool login_result = Default_Login(pInstance, pTenant, puser, L"https://rms-rhel74.qapf1.qalab01.nextlabs.com:8443", L"", "jewelry.zhu@nextlabs.com", password);
		//bool login_result = Admin_Login(pInstance, pTenant, puser, L"", L"", "john.tyler", "john.tyler", "2");
		if (login_result) {
			std::vector<SDR_PROJECT_INFO> projectsInfo;
			res = puser->GetHeartBeatInfo();
			//res = puser->ProjectDownloadFile(8, "/cp/header-2019-03-25-08-23-06.txt.nxl", projectdownloaded_path, RD_ForOffline);
			//projectdownloaded_path = L"C:\auto\output\SDK\Project\WIN10-2019-04-01-09-26-34.PNG.nxl";
			res = puser->ProjectDownloadFile(2, "/CP/withoutsharerights-2019-04-23-08-16-31.txt.nxl", projectdownloaded_path, RD_ForOffline);
			//res = puser->GetRights(projectdownloaded_path, rightsAndWatermarks);
			cout << "GetListProjects: ";
			res = puser->GetListProjects(1, 100, "name", RF_All);
			projectsInfo = puser->GetProjectsInfo();
			res = puser->ChangeRightsOfProjectFile(projectdownloaded_path, 53, "setting-2019-04-04-06-47-54.docx.nxl", "/CP/", rights, watermarkinfo, expire, tags);
			//res = puser->GetRights(projectdownloaded_path, rightsAndWatermarks);
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

