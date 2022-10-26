# include "Header_User.h"
# include <iostream>
# include <fstream>
# include <sstream>


using namespace std;
string get_test_report_file(string file_folder, string API_Name,string title_fail, string title,bool protect) {
	time_t t;
	struct tm *p;
	t = (uint64_t)std::time(nullptr);
	p = gmtime(&t);
	char s[30];
	strftime(s, 30, "%Y-%m-%d", p);
	string str = s;
	string generate_result_file_path;
	fstream _file;
	FILE *fp;
	generate_result_file_path = file_folder + "\\" + str + API_Name + "result.csv";
	_file.open(generate_result_file_path, ios::in);
	if (!_file) {
		// create a new file
		fp = fopen(generate_result_file_path.c_str(), "a");
		_file.close();
		// add file title
		ofstream onfile(generate_result_file_path, ios::app | ios::in | ios::out);
		onfile << title << endl;
		onfile.close();
	}
	_file.close();
	if (!protect) {
		generate_result_file_path = file_folder + "\\" + str + API_Name + "fail result.csv";
		_file.open(generate_result_file_path, ios::in);
		if (!_file) {
			// create a new file
			fp = fopen(generate_result_file_path.c_str(), "a");
			_file.close();
			// add file title
			ofstream onfile(generate_result_file_path, ios::app | ios::in | ios::out);
			onfile << title_fail << endl;
			onfile.close();
		}
		_file.close();
	}
	
	return generate_result_file_path;
}

void case_pass(string add_value_pass, string result_file_path_pass) {
	add_value_pass += "pass";
	ofstream onfile(result_file_path_pass, ios::app | ios::in | ios::out);
	onfile << add_value_pass << endl;
	onfile.close();
}
void case_fail(string add_value, string result_file_path_pass,string result_file_path_fail, string add_value_pass) {
	add_value += "fail";
	ofstream onfile(result_file_path_fail, ios::app | ios::in | ios::out);
	onfile << add_value << endl;
	onfile.close();

	add_value_pass += "fail";
	ofstream onfile2(result_file_path_pass, ios::app | ios::in | ios::out);
	onfile2 << add_value_pass << endl;
	onfile.close();

}

void MyAssertEQ(int retCode, int tmep,  string add_value, string result_file_path_pass, string add_value_pass, string result_file_path_fail)
{
	if (retCode != tmep) {
		case_fail(add_value, result_file_path_pass, result_file_path_fail, add_value_pass);
	}
}

void MyAssertTrue(bool is_open, string add_value, string result_file_path_pass, string add_value_pass, string result_file_path_fail)
{
	if (!is_open) {
		case_fail(add_value, result_file_path_pass, result_file_path_fail, add_value_pass);
		
	}
}

void MyAssertStrEQ(string str1, string str2 , string add_value, string result_file_path_pass, string add_value_pass, string result_file_path_fail)
{
	if (str1!=str2) {
		case_fail(add_value, result_file_path_pass, result_file_path_fail, add_value_pass);
	}

}

void copy_file(string source_folder_path, string target_file_path, string target_file_name) {
	string total_folder = source_folder_path;
	string total_file_path = total_folder + target_file_name;
	fstream _file;
	_file.open(target_file_path, ios::in);
	if (!_file) {
		cout << "file isn't exist, need copy file" << endl;
		CopyFile(s2ws(total_file_path).c_str(), s2ws(target_file_path).c_str(), FALSE);
		cout << "copy file success" << endl;
	}
}

void protect_file(string case_id, string test_point, string test_kind, string specify_operation, wstring file_pathw, string test_case, wstring outbox_pathw, string file_rights,
	string watermark, string expiration_type, string start_time, string end_time, string tag_text, string file_name, string expect_result, string add_value, string add_value_pass)
{

	ISDRmcInstance *pInstance = NULL;
	ISDRmTenant *pTenant = NULL;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	try{
		// current_time is current time stamp
		SDR_Expiration expire;
		uint64_t current_time = (uint64_t)std::time(nullptr) * 1000;
		if (expiration_type == "1") {
			expire.type = IExpiryType::RELATIVEEXPIRE;
			int minute = atoi(end_time.c_str());
			uint64_t temp_time_stamp = minute * 60 * 1000;
			expire.end = current_time + temp_time_stamp;
		}else if (expiration_type == "2") {
			expire.type = IExpiryType::ABSOLUTEEXPIRE;
			int minute = atoi(end_time.c_str());
			uint64_t temp_time_stamp = minute * 60 * 1000;
			expire.end = current_time + temp_time_stamp;
		}else if(expiration_type == "3"){
			expire.type = IExpiryType::RANGEEXPIRE;
			int end_minute = atoi(end_time.c_str());
			uint64_t temp_time_stamp = end_minute * 60 * 1000;
			int start_minute = atoi(start_time.c_str());
			uint64_t temp_time_stamp2 = start_minute * 60 * 1000;
			expire.start = current_time + temp_time_stamp2;
			expire.end = current_time + temp_time_stamp;
		}else {
			expire.type = IExpiryType::NEVEREXPIRE;
		}
		string test_report_title_pass = "case_ no,test point,file folder,test case,file name,test result";
		string test_report_title_fail = "case_ no,test point,test kind,Specify the operation,file folder,test case,outbox folder location,file rights,watermark,expiration type,start,end,tags,file name,expect result,test result";
		string result_file_path_pass = get_test_report_file("C:\\Users\\sfeng\\Desktop", " protect API ", test_report_title_fail, test_report_title_pass,true);
		string result_file_path_fail = get_test_report_file("C:\\Users\\sfeng\\Desktop", " protect API ", test_report_title_fail, test_report_title_pass, false);
		//vector<SDRmFileRight> rights = Tick_Rights(file_rights);
		vector<SDRmFileRight> rights;
		SDR_WATERMARK_INFO watermark_info = {};
		int rights_size = rights.size();
		for (int i = 0; i < rights.size(); i++) {
			if (rights[i] == RIGHT_WATERMARK) {
				rights_size--;
				if (watermark == "null") {
					watermark = "";
					watermark_info = { watermark,"font","color",9,0,ANTICLOCKWISE,true };
				}
				if (watermark == "0") {
					watermark_info = watermarkinfo;
				}
			}
			
		}

		//bool login_result = SDKRPM_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
		bool login_result = Admin_Login(pInstance, pTenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, "jimmy.carter", "jimmy.carter", "2");

		if (login_result) {

			bool adhoc;
			int heartbeat;
			int systemProjectId;
			string sysProjectTenantId;
			string tenantId;
			string systemMemID;
			string defaultTokengroupname;
			string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
			bool running = true;
			res = puser->GetListProjects(1, 50, "-lastActionTime", RF_All);
			checkSDWLResult(res);
			res = puser->GetTenantPreference(&adhoc, &heartbeat, &systemProjectId, sysProjectTenantId, tenantId);
			string mem_id = puser->GetMembershipID(systemProjectId);
			res = puser->UpdateUserInfo();
			int value = 0;
			MyAssertEQ(res.GetCode(), value , add_value,result_file_path_pass,add_value_pass,result_file_path_fail);
			ASSERT_EQ(res.GetCode(), value);
			//cout << "filepath: " << ws2s(file_pathw) << " new file path is: " << ws2s(outbox_pathw) << " rights: " << rights[0] << endl;
			//cout << "watermark: " << watermark_info.text << " exprie :" << expire.type << " tag: " << tag_text << endl;
			res = puser->ProtectFile(file_pathw, outbox_pathw, rights, watermark_info, expire, tag_text, mem_id);
			if (res.GetCode() == 0) {
				wstring nxl_file_name;
				pInstance->Save();
				ISDRmNXLFile *nxlfile;
				res = puser->OpenFile(outbox_pathw, &nxlfile);
				if (nxlfile) {
					bool is_open = nxlfile->IsOpen();
					MyAssertTrue(is_open, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
					ASSERT_TRUE(is_open);
					nxl_file_name = nxlfile->GetFileName();
					cout << "file_name is : " << ws2s(nxl_file_name) << endl;
					// check file rights
					vector<SDRmFileRight> get_rights = nxlfile->GetRights();
					MyAssertEQ(rights_size, get_rights.size(), add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
					ASSERT_EQ(rights_size, get_rights.size());
					bool is_rights_same = rights_verified(rights, nxlfile);
					MyAssertTrue(is_rights_same, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
					ASSERT_TRUE(is_rights_same);
					// check file watermark
					SDR_WATERMARK_INFO get_watermark_info = nxlfile->GetWaterMark();
					MyAssertStrEQ(watermark_info.text.c_str(), get_watermark_info.text.c_str(), add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
					ASSERT_STREQ(watermark_info.text.c_str(), get_watermark_info.text.c_str());
					
					// check file expire
					SDR_Expiration get_expire = nxlfile->GetExpiration();
					if (get_expire.type == IExpiryType::RELATIVEEXPIRE) {
						MyAssertEQ(expire.end, get_expire.end,  add_value, result_file_path_pass, add_value_pass,result_file_path_fail);
						ASSERT_EQ(expire.end, get_expire.end);
					}else if (get_expire.type == IExpiryType::ABSOLUTEEXPIRE) {
						MyAssertEQ(expire.end, get_expire.end,  add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
						ASSERT_EQ(expire.end, get_expire.end);
	
					}else if (get_expire.type == IExpiryType::RANGEEXPIRE) {
						MyAssertEQ(expire.start, get_expire.start,  add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
						MyAssertEQ(expire.end, get_expire.end,  add_value,result_file_path_pass, add_value_pass, result_file_path_fail);
						ASSERT_EQ(expire.end, get_expire.end);
						ASSERT_EQ(expire.end, get_expire.end);
					}else {
						MyAssertEQ(expire.start, get_expire.start, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
						MyAssertEQ(expire.end, get_expire.end, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
						ASSERT_EQ(expire.end, get_expire.end);
						ASSERT_EQ(expire.start, get_expire.start);
					}					
					// upload file to server
					res = puser->UploadFile(outbox_pathw, L"", L"john.tyler@qapf1.qalab01.nextlabs.com", L"hello, 123456 user");
					ASSERT_EQ(0, res.GetCode());
				
					// generated test report file
					
					case_pass(add_value_pass, result_file_path_pass);
					cout << "-----Generated NXL file success-----" << endl;
				}
				else {
					cout << "-----Generated NXL file fail-----" << endl;
					cout << "return res.getcode: " << res.GetCode() << endl;
					cout << "error case id is " << case_id << " test point: " << test_point << " test case: " << test_case << endl;
					int temp = atoi(expect_result.c_str());
					MyAssertEQ(res.GetCode(), temp, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
					ASSERT_EQ(res.GetCode(), temp);
					case_pass(add_value_pass, result_file_path_pass);
					
				}

			}
			else {
				cout << "-----user protect file fail-----" << endl;
				cout << "return res.getcode: " << res.GetCode() << endl;
				cout << "error case id is " << case_id << " test point: " << test_point << " test case: " << test_case << endl;
				int temp = atoi(expect_result.c_str());
				MyAssertEQ(res.GetCode(), temp, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
				ASSERT_EQ(res.GetCode(), temp);
				case_pass(add_value_pass, result_file_path_pass);
								
			}
			puser->LogoutUser();
			pInstance->Save();
			SDWLibDeleteRmcInstance(pInstance);

		}
		else {
			cout << "-----SDKRPM user login fail-----" << endl;
			cout << "error case id is " << case_id << " test point: " << test_point << " test case: " << test_case << endl;
			case_fail(add_value, result_file_path_pass, result_file_path_fail, add_value_pass);
			EXPECT_TRUE(false);
			
		}

	}
	catch (const std::exception& e){
		cout <<"e.what:"<< e.what() << endl;
		cout << "EXCEPTION!" << endl;
		throw;
	}

}

void changeProjectRights(string test_user, string password, wstring file_pathw, wstring outbox_pathw, string project_id, string tag, string tag_rights,
	string change_tag, string change_rights, string expect_result, string add_value, string add_value_pass,string path_id) {
	ISDRmcInstance *pinstance=NULL;
	ISDRmNXLFile *nxlfile=NULL;
	ISDRmTenant *ptenant=NULL;
	ISDRmUser *puser=NULL;
	SDWLResult res;
	try
	{
#pragma region [prepration file]
		vector<SDRmFileRight> rights;
		//rights.push_back(RIGHT_VIEW);
		//rights.push_back(RIGHT_SAVEAS);
		SDR_WATERMARK_INFO watermarkinfo;
		watermarkinfo = { "","font","color",9,0,ANTICLOCKWISE,true };
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
#pragma endregion
		int expect_value = atoi(expect_result.c_str());
		string test_report_title_pass = "case_ no,test point,user,test case,proiect id,test result";
		string test_report_title_fail = "case_ no,test point,test kind,Specify the operation,user,password,file folder,file name,test case,outbox folder location,project id, tag,rights,change tag,change rights,project path id,expect result,test result";
		string result_file_path_pass = get_test_report_file("C:\\Users\\sfeng\\Desktop", "ChangeRightsOfProjectFile API ", test_report_title_fail, test_report_title_pass, true);
		string result_file_path_fail = get_test_report_file("C:\\Users\\sfeng\\Desktop", "ChangeRightsOfProjectFile API ", test_report_title_fail, test_report_title_pass, false);
		bool login = SDKRPM_Login(pinstance, ptenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, test_user, password);
		if (login) {
			//int value = 0;
			res=puser->UpdateUserInfo();
			MyAssertEQ(res.GetCode(), 0, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
			ASSERT_EQ(res.GetCode(), 0);

			uint32_t projectID = atoi(project_id.c_str());
			// check project id
			string orderBy = "name";
			RM_ProjectFilter filter = RF_All;
			res = puser->GetListProjects(1, 100, orderBy, filter);
			if (!res.GetCode()) {
				vector<uint32_t> id;
				vector<SDR_PROJECT_INFO> projectsInfo = puser->GetProjectsInfo();
				cout << "project number: " << projectsInfo.size() << endl;
				for (int i = 0; i < projectsInfo.size(); i++) {
					id.push_back(projectsInfo[i].projid);
				}
				vector<uint32_t> ::iterator it;
				it = find(id.begin(), id.end(), projectID);
				if (it != id.end()) {
					cout << "-----project id is correct-----" << endl;
				}
				else {
					cout << "-----project id is not exist-----" << endl;
					case_fail(add_value, result_file_path_pass, result_file_path_fail, add_value_pass);
					ASSERT_TRUE(false);
				}
			}
			else {
				cout << "-----get project list fail-----" << endl;
				case_fail(add_value, result_file_path_pass, result_file_path_fail, add_value_pass);
				ASSERT_TRUE(false);
			}
			string nxl_file_name;
			wstring downloadFolder = L"C:\\SDK test data\\download";
			string membershipID = puser->GetMembershipID(projectID);

			Sleep(1000);
			res = puser->ProtectFile(file_pathw, outbox_pathw, rights, watermarkinfo, expire,tag,membershipID);
			if (res.GetCode() == 0) {
				res = puser->OpenFile(outbox_pathw, &nxlfile);
				if (nxlfile) {
					bool open = nxlfile->IsOpen();
					if (open) {
						nxl_file_name = ws2s(nxlfile->GetFileName());
						res = puser->UploadProjectFile(projectID,s2ws(path_id),nxlfile);
						MyAssertEQ(res.GetCode(), 0, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);						
						ASSERT_EQ(res.GetCode(), 0);
						puser->CloseFile(nxlfile);
						Sleep(3000);
						string pathid = path_id+ nxl_file_name;
						
						res = puser->ProjectDownloadFile(projectID, pathid, downloadFolder, RD_ForOffline);
						checkSDWLResult(res);
						MyAssertEQ(res.GetCode(), 0, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
						ASSERT_EQ(res.GetCode(), 0);
						Sleep(3000);
						res = puser->OpenFile(downloadFolder, &nxlfile);
						MyAssertEQ(res.GetCode(), 0, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
						ASSERT_EQ(res.GetCode(), 0);
						bool is_open = nxlfile->IsOpen();
						if (is_open) {
							// check file rights and tag
							vector<SDRmFileRight> get_rights1;
							res = puser->GetProjectFileRights(project_id, pathid, get_rights1);
							vector<SDRmFileRight> rights1;
							rights1 = Tick_Rights(tag_rights);
							MyAssertEQ(get_rights1.size(), rights1.size(), add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
							ASSERT_EQ(get_rights1.size(), rights1.size());
							int tag_rights_value = 0;
							int get_tag_rights_value1 = 0;
							for (int i = 0; i < get_rights1.size(); i++) {
								get_tag_rights_value1 += get_rights1[i];
							}
							for (int i = 0; i < rights1.size(); i++) {
								tag_rights_value += rights1[i];
							}
							MyAssertEQ(tag_rights_value, get_tag_rights_value1, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
							ASSERT_EQ(tag_rights_value, get_tag_rights_value1);

							string get_tag2 = nxlfile->GetTags();
							MyAssertStrEQ(get_tag2.c_str(), tag.c_str(), add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
							ASSERT_STREQ(get_tag2.c_str(), tag.c_str());
							puser->CloseFile(nxlfile);
							res = puser->ChangeRightsOfProjectFile(downloadFolder, projectID, nxl_file_name, path_id, rights, watermarkinfo, expire, change_tag);
							if (!res.GetCode()) {
								Sleep(2000);
								//check rights and tag again after change rights
								vector<SDRmFileRight> get_rights2;
								res = puser->GetProjectFileRights(project_id, pathid, get_rights2);
								vector<SDRmFileRight> rights2;
								rights2 = Tick_Rights(change_rights);
								MyAssertEQ(get_rights2.size(), rights2.size(), add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
								EXPECT_EQ(get_rights2.size(), rights2.size());
								int change_tag_rights_value = 0;
								int get_tag_rights_value2 = 0;
								for (int i = 0; i < get_rights2.size(); i++) {
									get_tag_rights_value2 += get_rights2[i];
								}
								for (int i = 0; i < rights2.size(); i++) {
									change_tag_rights_value += rights2[i];
								}
								MyAssertEQ(change_tag_rights_value, get_tag_rights_value2, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
								ASSERT_EQ(change_tag_rights_value, get_tag_rights_value2);
								// check tag
								puser->OpenFile(downloadFolder, &nxlfile);
								bool is_open2 = nxlfile->IsOpen();
								if (is_open2) {
									string get_tag = nxlfile->GetTags();
									MyAssertStrEQ(get_tag.c_str(), change_tag.c_str(), add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
									ASSERT_STREQ(get_tag.c_str(), change_tag.c_str());
								}
								case_pass(add_value_pass, result_file_path_pass);
							}
							else {
								cout << "res.getcode: " << res.GetCode() << "res.getmsg: " << res.GetMsg() << endl;
								MyAssertEQ(res.GetCode(), expect_value, add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
								ASSERT_EQ(res.GetCode(), expect_value);
								case_pass(add_value_pass, result_file_path_pass);
							}												
						}
					}
				}
				else {
					cout << "----- file is null-----" << endl;
					case_fail(add_value, result_file_path_pass, result_file_path_fail, add_value_pass);
					ASSERT_TRUE(false);
				}
			}
			else {
				cout << "-----protect file fail-----" << endl;
				MyAssertEQ(res.GetCode(), atoi(expect_result.c_str()), add_value, result_file_path_pass, add_value_pass, result_file_path_fail);
				ASSERT_EQ(res.GetCode(), atoi(expect_result.c_str()));
				case_pass(add_value_pass, result_file_path_pass);
			}
		}
		else {
			cout << " ------user default login fail------ " << endl;
			case_fail(add_value, result_file_path_pass, result_file_path_fail, add_value_pass);
			ASSERT_TRUE(false);
		}

	}
	catch (const std::exception& e)
	{
		cout << "e.what" << e.what() << endl;
	}
	
}


class UserProtectFileSize :public::testing::TestWithParam<vector<string>> {};
TEST_P(UserProtectFileSize, UserProtectFileSizeTest) {

	// if flag = true, run test kind, else run Specify the operation
	bool flag = false;
	// 0 representative spin, 1 representative build verified, 2 representative full,  
	cout << "-------------" << endl;
	string run_type = "0";
	vector<string> csv = GetParam();
	string case_id = csv[0];
	string test_point = csv[1];
	string test_kind = csv[2];
	string specify_operation = csv[3];
	string file_folder = csv[4];
	createFolder(file_folder);
	cout <<"file_folder: "<< file_folder << endl;
	string test_case = csv[5];
	string outbox_path = csv[6];
	string file_rights = csv[7];
	string watermark = csv[8];
	string expiration_type = csv[9];
	string start_time = csv[10];
	string end_time = csv[11];
	string tag_text = csv[12];
	string file_name = csv[13];
	string expect_result = csv[14];
	string file_path = file_folder +"\\"+ file_name;
	cout <<"file_path:" <<file_path << endl;
	wstring file_pathw = s2ws(file_path);
	wstring outbox_pathw = s2ws(outbox_path);
	string use_tag = patchTagString(tag_text);
	cout << "----------start execute case id---------- # " << case_id << endl;
	if (case_id == "null"){
		throw exception("case id is null, please input it");
	}
	
	if (file_rights == "null") {
		throw exception("file rights is null, please input in it ");
	}

	// check file is exist, if not, copy file to test path
	string source_file_folder = "C:\\SDK test data\\test file\\";
	copy_file(source_file_folder, file_path, file_name);

	string add_value_pass;
	for (int i = 0; i < csv.size(); i++) {
		if (i == 0 || i == 1 || i == 4 || i == 5 || i == 13) {
			add_value_pass = add_value_pass + csv[i] + ",";
		}		
	}
	string add_value;
	for (int i = 0; i < csv.size(); i++) {
		add_value += csv[i] + ",";
	}

	if (flag) {
		vector<string> type;
		splitString(Trim(test_kind), type , "|");
		if (run_type == "0") {
			cout << "----------test_kind is spin test----------" << endl;
		}else if (run_type == "1") {
			cout << "----------test_kind is build verified test----------" << endl;
		}else if (run_type == "2") {
			cout << "----------test_kind is full test----------" << endl;
		}else {
		
		}
		for (int i = 0; i < type.size(); i++) {
			if (type[i] == run_type) {
				protect_file(case_id, test_point, test_kind, specify_operation, file_pathw, test_case, outbox_pathw, file_rights, watermark, expiration_type, start_time, end_time, use_tag, file_name, expect_result, add_value, add_value_pass);
				
			}
		}		
	}else {
		if (Trim(specify_operation) == "is run") {
			protect_file(case_id, test_point, test_kind, specify_operation, file_pathw, test_case, outbox_pathw, file_rights, watermark, expiration_type, start_time, end_time, use_tag, file_name, expect_result, add_value, add_value_pass);

		}
	}
}
INSTANTIATE_TEST_CASE_P(UserProtectFileSizeT, UserProtectFileSize, testing::ValuesIn(csv_protect_automation));


class UserChangeProjectRights : public::testing::TestWithParam<vector<string>> {};
TEST_P(UserChangeProjectRights, UserChangeProjectRightsTest) {
	// if flag = true, run test kind, else run Specify the operation
	bool flag = false;
	// 0 representative spin, 1 representative build verified, 2 representative full,  
	string run_type = "0";
	vector<string> csv = GetParam();
	string case_id = csv[0];
	string test_point = csv[1];
	string test_kind = csv[2];
	string specify_operation = csv[3];
	string test_user = csv[4];
	string password = csv[5];
	string file_folder = csv[6];
	string file_name = csv[7];
	string test_case = csv[8];
	string outbox_path = csv[9];
	string project_id = csv[10];
	string tag = csv[11];
	tag = patchTagString(tag);
	string rights = csv[12];
	string change_tag = csv[13];
	change_tag = patchTagString(change_tag);
	string change_rights = csv[14];
	string path_id = csv[15];
	string expect_result = csv[16];
	string file_path = file_folder + "\\" + file_name;
	cout << "file_path:" << file_path << endl;
	wstring file_pathw = s2ws(file_path);
	wstring outbox_pathw = s2ws(outbox_path);
	cout << "----------start execute case id---------- # " << case_id << endl;

	// check file is exist, if not, copy file to test path
	string source_file_folder = "C:\\SDK test data\\test file\\";
	copy_file(source_file_folder, file_path, file_name);

	string add_value_pass;
	for (int i = 0; i < csv.size(); i++) {
		if (i == 0 || i == 1 || i == 4 || i == 8 || i == 10) {
			add_value_pass = add_value_pass + csv[i] + ",";
		}
	}
	string add_value;
	for (int i = 0; i < csv.size(); i++) {
		add_value += csv[i] + ",";
	}

	if (flag) {
		vector<string> type;
		splitString(Trim(test_kind), type, "|");
		if (run_type == "0") {
			cout << "----------test_kind is spin test----------" << endl;
		}
		else if (run_type == "1") {
			cout << "----------test_kind is build verified test----------" << endl;
		}
		else if (run_type == "2") {
			cout << "----------test_kind is full test----------" << endl;
		}
		else {
			
		}
		for (int i = 0; i < type.size(); i++) {
			if (type[i] == run_type) {
				changeProjectRights(test_user, password, file_pathw, outbox_pathw, project_id, tag, rights, change_tag, change_rights, expect_result, add_value, add_value_pass,path_id);
			}
		}
	}
	else {
		if (Trim(specify_operation) == "is run") {
			changeProjectRights(test_user, password, file_pathw, outbox_pathw, project_id, tag, rights, change_tag, change_rights, expect_result, add_value, add_value_pass,path_id);
		}
	}

	
}
INSTANTIATE_TEST_CASE_P(UserChangeProjectRightsT, UserChangeProjectRights,testing::ValuesIn(csv_changeRights_automation));

TEST(SystemBucketfile, SystemBucketfileTest) {
	ISDRmcInstance *pInstance;
	ISDRmUser *pUser;
	ISDRmTenant *pTenant;
	ISDRmNXLFile *nxlFile;
	SDWLResult res;
	wstring outbox = L"C:\\outbox";
	//bool login = SDKRPM_Login(pInstance, pTenant, pUser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	bool login = Admin_Login(pInstance, pTenant, pUser, DEFUALT_ROUTER, DEFUALT_TENANT, "jimmy.carter", "jimmy.carter", "2");
	if (login) {
		bool adhoc;
		int heartbeat;
		int systemProjectId;
		string sysProjectTenantId;
		string tenantId;
		string systemMemID;
		string defaultTokengroupname;
		string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
		bool running = true;
		res = pUser->GetListProjects(1,50,"-lastActionTime", RF_All);
		checkSDWLResult(res); 
		res = pUser->GetTenantPreference(&adhoc,&heartbeat,&systemProjectId,sysProjectTenantId,tenantId);
		checkSDWLResult(res);
		cout << "sysProjectTenantId" << sysProjectTenantId << endl;
		systemMemID = pUser->GetMembershipID(systemProjectId);
		defaultTokengroupname = pUser->GetDefaultTokenGroupName();
		string systemMemID2 = pUser->GetMembershipIDByTenantName(defaultTokengroupname);
		EXPECT_STREQ(systemMemID.c_str(), systemMemID2.c_str());
		//sysProjectTenantId = pUser->GetSystemProjectTenantId();
		string defaultTenantId = pUser->GetDefaultTenantId();

		res = pInstance->RPMRegisterApp(apppath,secr);
		res = pInstance->RPMNotifyRMXStatus(running,secr);
		res = pInstance->RemoveRPMDir(L"C:\\outbox");
		checkSDWLResult(res);
		
		vector<SDRmFileRight> rights;
		//rights.push_back(RIGHT_VIEW);
		SDR_WATERMARK_INFO watermark = {};
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring filePath = L"C:\\SDK test data\\test file\\1.tiff";
		string tag = "{\"Rights\":[\"Rights_View\"]}";
		string temp_tag="";
		int rights_value = 0;
		vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
		vector<SDR_CLASSIFICATION_CAT> cats;
		pUser->GetClassification(defaultTokengroupname, cats);
		bool exit = false;
		for (int i = 0; i < cats.size(); i++) {
			if (exit) {
				break;
			}
			cout << "tag: " << cats[i].name << endl;
			for (int j = 0; j < cats[i].labels.size(); j++) {
				cout << " vaule: " << cats[i].labels[j].name;
				temp_tag = "{\"" + cats[i].name + "\":[\"" + cats[i].labels[j].name + "\"]}";
				pUser->GetFileRightsFromCentralPolicies(sysProjectTenantId, temp_tag, rightsAndWatermarks);
				if (rightsAndWatermarks.size() != 0) {
					exit = true;
					break;
				}
			}
			cout << endl;
		}
		for (int i = 0; i < rightsAndWatermarks.size(); i++) {
			pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>> sss = rightsAndWatermarks[i];
			rights_value += sss.first;
		}
		res = pUser->ProtectFile(filePath, outbox, rights, watermark, expire, temp_tag, systemMemID);
		checkSDWLResult(res);
		if (!res.GetCode()) {
			res = pUser->OpenFile(outbox, &nxlFile);
			checkSDWLResult(res);
			bool is_open = nxlFile->IsOpen();
			if (is_open) {
				res = pInstance->AddRPMDir(L"C:\\outbox");
				// check file rightsand tag
				vector<pair<SDRmFileRight, vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks1;
				wstring outbox_;
				outbox_.assign(outbox, 0, outbox.size() - 4);
				// process is trust
				res = pInstance->RPMGetRights(outbox_, rightsAndWatermarks1);
				EXPECT_EQ(rightsAndWatermarks.size(), rightsAndWatermarks1.size());
				int rights_value1 = 0;
				for (int i = 0; i < rightsAndWatermarks1.size(); i++) {
					pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>> sss = rightsAndWatermarks1[i];
					rights_value1 += sss.first;
				}
				EXPECT_EQ(rights_value, rights_value1);
				res = pUser->CloseFile(nxlFile);
			}
			res = pInstance->RemoveRPMDir(L"C:\\outbox",true);
			res = pUser->ChangeRightsOfFile(outbox,rights,watermark,expire,"");
		}
		res = pUser->LogoutUser();
		checkSDWLResult(res);
		res = pInstance->RPMLogout();
		checkSDWLResult(res);
	}
	else {
		cout << "-----user login fail-----" << endl;
	}
}

TEST(RPMReadFileT, RPMReadFile) {
	ISDRmUser *puser = NULL;
	ISDRmNXLFile* nxlfile;
	ISDRmcInstance *pinstance = NULL;
	ISDRmTenant *ptenant = NULL;
	SDWLResult res;

	bool running = true;
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

	bool login = SDKRPM_Login(pinstance, ptenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	res = pinstance->RPMRegisterApp(apppath, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());
	res = pinstance->RPMNotifyRMXStatus(running, secr);
	checkSDWLResult(res);
	EXPECT_EQ(0, res.GetCode());

	bool is_register;
	res = pinstance->RPMIsAppRegistered(apppath, &is_register);
	EXPECT_TRUE(is_register);
	if (login) {
		vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		//rights.push_back(RIGHT_SAVEAS);
		SDR_WATERMARK_INFO watermarkinfo;
		watermarkinfo = { "","font","color",9,0,ANTICLOCKWISE,true };
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring outbox = L"C:\\SDK test data\\Server File";
		res = puser->ProtectFile(L"C:\\888.txt", outbox, rights, watermarkinfo, expire);
		checkSDWLResult(res);
		res = puser->OpenFile(outbox, &nxlfile);
		cout << "res.getcode: " << res.GetCode() << endl;
		if (!res.GetCode()) {
			wstring tag;
			cout << "----------------->" << endl;
			///res = pinstance->RPMReadFileTags(outbox, tag);
			///cout << "----------------->1" << endl;
			///checkSDWLResult(res);
			///cout << "res.getcode: " << res.GetCode() << endl;
		}
	}
}

TEST(ssssT, ssss) {
	wstring str = L"234343.nxl";
	wstring str1;
	str1.assign(str, 0, str.size()-4 );
	cout << ws2s(str1) << endl;
	cout << "----------" << endl;
	//ISDRmUser *puser = NULL;
	//ISDRmNXLFile* nxlfile;
	//ISDRmcInstance *pinstance = NULL;
	//ISDRmTenant *ptenant = NULL;
	//SDWLResult res;
	//wstring normal_file = L"C:\\Users\\sfeng\\Desktop\\hello.txt";
	////wstring file_path = L"C:\\SDK test data\\Server File";
	//bool login = SDKRPM_Login(pinstance,ptenant,puser, DEFUALT_ROUTER,DEFUALT_TENANT,email,password);
	//if (login) {

	//	string downfoler = "C:\\SDK test data\\download";
	//	res = puser->ProjectDownloadFile(25,"/56-2019-07-11-08-59-16.jpg.nxl",s2ws(downfoler), RD_ForOffline);
	//	cout << res.GetCode() << res.GetMsg() << 1111 << endl;
	//	res = puser->OpenFile(s2ws(downfoler+"\\56-2019-07-11-08-59-16.jpg.nxl"), &nxlfile);
	//	cout << res.GetCode() << res.GetMsg() << 22222 << endl;
	//	cout << "downfoler: " << downfoler << endl;
	//	if (!res.GetCode()) {
	//		bool is_open =nxlfile->IsOpen();
	//		if (is_open) {
	//			wstring fileName = nxlfile->GetFileName();
	//			cout << "fileName: " << ws2s(fileName) << endl;

	//			Sleep(1000);
	//			//RL_OBAccessResult  RL_OBAccessTime
	//			//RL_SFEmpty = 0, RL_SFEmail, RL_SFOperation, RL_SFDeviceId
	//			//res = puser->GetNXLFileActivityLog(nxlfile, 0, 40, RL_SFEmpty, "55555", RL_OBAccessResult, true);
	//			//cout << "GetNXLFileActivitylog: " << res.GetCode() << endl;
	//			//EXPECT_FALSE(res.GetCode());
	//			vector<SDR_FILE_ACTIVITY_INFO> info;

	//			res = puser->GetActivityInfo(fileName, info);
	//			cout << "GetActivityInfo: " << res.GetCode() << endl;
	//			EXPECT_FALSE(res.GetCode());
	//			cout << "log number: " << info.size() << endl;
	//			for (int i = 0; i < info.size(); i++) {
	//				cout << "log: " << info[i].operation << endl;
	//			}

	//		}
	//	}

	//	cout << "--------" << endl;
	//}
	//

	//cout << "----test end----" << endl;

}

TEST(TTTTT, sTTTTT) {
	ISDRmUser *puser = NULL;
	ISDRmNXLFile* nxlfile;
	ISDRmcInstance *pinstance = NULL;
	ISDRmTenant *ptenant = NULL;
	SDWLResult res;
	bool login = SDKRPM_Login(pinstance, ptenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	if (login) {
		wstring outbox = L"C:\\SDK test data\\download\\56-2019-07-11-08-59-16.jpg.nxl";
		res = puser->OpenFile(outbox,&nxlfile);
		if (!res.GetCode()) {
			vector<pair<SDRmFileRight, vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
			res = puser->GetRights(outbox, rightsAndWatermarks);
			checkSDWLResult(res);
			cout << "nxlfileRights: " << rightsAndWatermarks.size() << endl;

		}

		//tokenGroupName = skydrmtest7513test_01_only - starry_14
		/*string orderBy = "name";
		RM_ProjectFilter filter = RF_OwnedByMe;
		string tokengroupname;
		res = puser->GetListProjects(1, 100, orderBy, filter);
		if (!res.GetCode()) {
			vector<uint32_t> id;
			vector<SDR_PROJECT_INFO> projectsInfo = puser->GetProjectsInfo();
			for (int i = 0; i < projectsInfo.size(); i++) {
				id.push_back(projectsInfo[i].projid);
				if (projectsInfo[i].projid == 25) {
					tokengroupname = projectsInfo[i].tokengroupname;
					break;
				}
			}
			
		}
		else {
			cout << "-----get project list fail-----" << endl;
			ASSERT_TRUE(false);
		}
		cout << "tokengroupname: " << tokengroupname<< endl;
		vector<SDR_CLASSIFICATION_CAT> cats;
		res = puser->GetClassification(tokengroupname,cats);
		cout << "cat size: " << cats.size() << endl;
		for (int i = 0; i < cats.size(); i++) {
			cout << "TAG:" << cats[i].name << endl;
			for (int j = 0; j < cats[i].labels.size(); j++) {
				cout << "VALUE: " << cats[i].labels[j].name;
			}
			cout << endl;*/
		//}
		//string TenantID = puser->GetDefaultTenantId();
		//cout << "TenantID: " << TenantID << endl;
	}
	
}

TEST(preference, preferenceTest) {
	ISDRmUser *puser = NULL;
	ISDRmNXLFile* nxlfile;
	ISDRmcInstance *pinstance = NULL;
	ISDRmTenant *ptenant = NULL;
	SDWLResult res;
	bool login = SDKRPM_Login(pinstance, ptenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	if (login) {
		uint32_t option;
		uint64_t start;
		uint64_t end;
		wstring watermark;
		string defwatermark = "hello";
		res = puser->GetUserPreference(option, start, end, watermark);
		cout << "option: " << option << " start: " << start << " end: " << end << " watermark: " << ws2s(watermark) << endl;
		res = puser->UpdateUserPreference(0, 0, 0, s2ws(defwatermark));
		Sleep(10000);
		res = puser->GetUserPreference(option, start, end, watermark);
		EXPECT_EQ(start, 0);
		EXPECT_EQ(end, 0);
		EXPECT_EQ(option, 0);
		string watermark2 = ws2s(watermark);
		EXPECT_STREQ(defwatermark.c_str(),watermark2.c_str());
		
	}
}

TEST(protectFileOffline, protectFileOfflineTest) {
	ISDRmUser *puser = NULL;
	ISDRmNXLFile* nxlfile;
	ISDRmcInstance *pinstance = NULL;
	ISDRmTenant *ptenant = NULL;
	SDWLResult res;
	bool login = SDKRPM_Login(pinstance, ptenant, puser, DEFUALT_ROUTER, DEFUALT_TENANT, email, password);
	if (login) {
		vector<SDRmFileRight> rights;
		rights.push_back(RIGHT_VIEW);
		//rights.push_back(RIGHT_SAVEAS);
		SDR_WATERMARK_INFO watermarkinfo;
		watermarkinfo = { "","font","color",9,0,ANTICLOCKWISE,true };
		SDR_Expiration expire;
		expire.type = IExpiryType::NEVEREXPIRE;
		wstring outbox = L"C:\\SDK test data\\Server File";
		res = puser->ProtectFile(L"C:\\888.txt", outbox, rights, watermarkinfo, expire);
		cout << "passcode: " << puser->GetPasscode();
		checkSDWLResult(res);
	}
}

TEST(uploadOfflineFile, uploadOfflineFileTest) {
	ISDRmUser *puser;
	ISDRmNXLFile* nxlfile;
	ISDRmcInstance *pinstance;
	ISDRmTenant *ptenant;
	SDWLResult res;
	string passcode = "31B6BE514647D6E91D245565FDC9DAB6";
	bool login = PreviousUser_Login(pinstance,puser, DEFUALT_ROUTER, DEFUALT_TENANT,email, passcode);
	if (login) {
		wstring outbox = L"C:\\SDK test data\\Server File\\888-2019-07-18-08-23-16.txt.nxl";
		res = puser->OpenFile(outbox, &nxlfile);
		checkSDWLResult(res);
		bool open = nxlfile->IsOpen();
		if (open) {
			res = puser->UploadFile(outbox, outbox);
			checkSDWLResult(res);
		}

	}
}

TEST(RPMlogoutTest123, RPMlogout) {
	ISDRmcInstance * pInstance;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	ISDRmTenant *pTenant = NULL;
	ISDRmHttpRequest *prequest = NULL;
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	res = pInstance->RPMLogout();

	cout << "res.getcode(): " << res.GetCode() << "   res.msg: " << res.GetMsg() << endl;
	cout << "-----end!----- " << endl;
}
