#pragma once

#define RunType 2

//#define __Test__rms_centos7303__
//#define __Test__rms_hacentos7503__
#define __Test__rms_centos7513__
//#define __Test__autorms_centos7303__
//#define __Test__rms_centos7308__
//#define __Test__rms_rhel74__
//#define __Test__rms_centos7512__
//#define __Test__autorms_centos7301__
//#define __Test__rms_centos7306__

//#define __Test__jenkins_centos7302__
//#define __Test__SkyDRM__

#if defined(__Test__rms_centos7303__)
const wstring DEFUALT_ROUTER = L"https://rms-centos7303.qapf1.qalab01.nextlabs.com:8443";
const wstring DEFUALT_TENANT = L"";
#define POSTURL "https://rms-centos7303.qapf1.qalab01.nextlabs.com:8444/rms/rs/usr" 

#elif defined(__Test__rms_hacentos7503__)
const wstring DEFUALT_ROUTER = L"https://rms-hacentos7503.qapf1.qalab01.nextlabs.com:8443";
const wstring DEFUALT_TENANT = L"";
#define POSTURL "https://rms-hacentos7503.qapf1.qalab01.nextlabs.com:8444/rms/rs/usr"

#elif defined(__Test__rms_centos7513__)
const wstring DEFUALT_ROUTER = L"https://rms-centos7513.qapf1.qalab01.nextlabs.com:8443";
const wstring DEFUALT_TENANT = L"";
#define POSTURL "https://rms-centos7513.qapf1.qalab01.nextlabs.com:8444/rms/rs/usr"

#elif defined(__Test__rms_centos7512__)
const wstring DEFUALT_ROUTER = L"https://rms-centos7512.qapf1.qalab01.nextlabs.com:8443";
const wstring DEFUALT_TENANT = L"";
#define POSTURL "https://rms-centos7512.qapf1.qalab01.nextlabs.com:8444/rms/rs/usr"

#elif defined(__Test__autorms_centos7303__)
const wstring DEFUALT_ROUTER = L"https://autorms-centos7303.qapf1.qalab01.nextlabs.com:8443";
const wstring DEFUALT_TENANT = L"c2453f8c-3a53-47b3-a335-02a093a8fc68";
//const wstring DEFUALT_TENANT = L"";
#define POSTURL "https://rms-centos7308.qapf1.qalab01.nextlabs.com:8444/rms/rs/usr"

#elif defined(__Test__rms_centos7308__)
const wstring DEFUALT_ROUTER = L"https://rms-centos7308.qapf1.qalab01.nextlabs.com:8443";
const wstring DEFUALT_TENANT = L"d7b39d66-eebf-40e0-9eef-f4042d47afd0";
//const wstring DEFUALT_TENANT = L"";
#define POSTURL "https://rms-centos7308.qapf1.qalab01.nextlabs.com:8444/rms/rs/usr"

#elif defined(__Test__rms_rhel74__)
const wstring DEFUALT_ROUTER = L"https://autorms-rhel74.qapf1.qalab01.nextlabs.com:8443";
//const wstring DEFUALT_TENANT = L"b3c24eaf-2cd3-4e49-abf7-15ad4303337f";
const wstring DEFUALT_TENANT = L"";
#define POSTURL "https://autorms-rhel74.qapf1.qalab01.nextlabs.com:8444/rms/rs/usr"

#elif defined(__Test__autorms_centos7301__)
const wstring DEFUALT_ROUTER = L"https://autorms-centos7301.qapf1.qalab01.nextlabs.com:8443";
const wstring DEFUALT_TENANT = L"";
#define POSTURL "https://autorms-centos7301.qapf1.qalab01.nextlabs.com:8444/rms/rs/usr"    

#elif defined(__Test__rms_centos7306__)
const wstring DEFUALT_ROUTER = L"https://rms-centos7306.qapf1.qalab01.nextlabs.com:8443";
const wstring DEFUALT_TENANT = L"";
#define POSTURL "https://rms-centos7306.qapf1.qalab01.nextlabs.com:8444/rms/rs/usr"    

#elif defined(__Test__jenkins_centos7302__)
const wstring DEFUALT_ROUTER = L"https://jenkins-centos7302.qapf1.qalab01.nextlabs.com:8443";
const wstring DEFUALT_TENANT = L"";
#define POSTURL "https://jenkins-centos7302.qapf1.qalab01.nextlabs.com:8444/rms/rs/usr" 

#elif defined(__Test__SkyDRM__)
const wstring DEFUALT_ROUTER = L"https://r.skydrm.com";
const wstring DEFUALT_TENANT = L"skydrm.com";
#define POSTURL "https://skydrm.com/rms/rs/usr" 

#else
const wstring DEFUALT_ROUTER = L"https://rmtest.nextlabs.solutions";
const wstring DEFUALT_TENANT = L"skydrm.com";
#define POSTURL "https://rmtest.nextlabs.solutions/rms/rs/usr" 

#endif



const string email = "3057318205@qq.com";
//const string email = "jewelry.zhu@nextlabs.com";
const string password = "a1409d7e150f67df4e1fbccafe2ec5aa";
//const string password = "c27b83a22f39dceb435ed6d08ec42748";
//const string email = "lifi.yuan@nextlabs.com";
const string email_admin = "john.tyler";
const string password_admin = "john.tyler";

const string email_b = "3079175375@qq.com";
const string email_zhu = "xiaozhuzhu0712@163.com";

//const string email_global = "lifi.yuan@nextlabs.com";
const string email_global = "472049994@qq.com";
const string password_plain = "123blue!";
//
//const string email = "shirley.gu@nextlabs.com ";
//const string password = "c9808cfe7a98acf1088e791b41bdaeed";

//const string password = "a1409d7e150f67df4e1fbccafe2ec5aa";
//const string email = "jewelry.zhu@nextlabs.com";


const string path_userfile = "C:\\SDK test data\\";
const string USERB_FILE = path_userfile + "UserB_Login.txt";
const SDR_WATERMARK_INFO watermarkinfo = { "$(User)$(Break)$(Date) $(Time)","font","color",9,0,ANTICLOCKWISE,true };
//const SDR_WATERMARK_INFO watermarkinfo = { "","","",9,0,ANTICLOCKWISE,true };
//const SDR_WATERMARK_INFO watermarkinfo = { "$(User)$(Break)$(Date)$(Time)","red","Calibri",9,0,10,true };
//const SDR_WATERMARK_INFO watermarkinfo = { "$User$Time","font","color",9,0,WATERMARK_ROTATION::NOROTATION,true };
const string tags = "{\"EAR\":[\"EAR - 01\"]}";
//const string tags = "{\"itar\":[\"taa-01\"]}";
extern string  user_id;
const string DEFUALT_USER_FILE = path_userfile + "User_Login.txt";
////////////////////
const WCHAR * const DEFUALT_SDKLib_FOLDER = L"C:\\Program Files\\NextLabs";
const wstring DEFUALT_LIB_FOLDER = L"C:\\sdklib";
const wstring DEFUALT_TEMP_FOLDER = L"C:\\temp";
const wstring DEFULAT_WORKING_FOLDER = L"C:\\work";
const string TestDirectory = "C:\\SDK test\\";
const string DataDirectory = "C:\\SDK test data\\";

const string plan_path = "C:\\SDK test data\\";
const wstring plan_pathw = L"C:\\SDK test data\\";
const wstring plan_file_path = L"C:\\SDK test data\\plan file\\";
const wstring decrypt_file_path = L"C:\\SDK test data\\decrypt file\\";
const wstring server_file_path = L"C:\\SDK test data\\Server File\\";
const wstring local_file_path = L"C:\\SDK test data\\Local file\\";
const wstring server_userb_file_path = L"C:\\SDK test data\\Server File\\UserB\\";
const wstring apppath = L"C:\\auto\\Clone-rmcsdk-wiki2\\QA_TestCode\\x64\\Debug\\RMD-WindowsAPI-Gtest.exe";
//const wstring apppath = L"D:\\test\\Clone_rmcsdk-wiki3\\QA_TestCode\\x64\\Debug\\RMD-WindowsAPI-Gtest.exe";
const wstring apppath_slash = L"D:/test/Clone_rmcsdk-wiki3/QA_TestCode/x64/Debug/RMD-WindowsAPI-Gtest.exe";
//const wstring apppath = L"C:\\Users\\jzhu\\Desktop\\Debug\\RMD-WindowsAPI-Gtest.exe";
//const wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";
//const wstring downloaded_path = L"C:\\auto\\output\\SDK\\Project\\";

//Below is REST API return 
//Below is REST API return 
const string tenant_respond = DataDirectory + "GetTenantReturn.txt";
const string mydriveInfo_respond = DataDirectory + "GetMyDriveInfoReturn.txt";
const string clientfilename = DataDirectory + "client_id.txt";
const string user_respond = DataDirectory + "User_Return.txt";
const string updatinfo_return = DataDirectory + "GetUpdateInfoReturn.txt";
const string myvaultfilelist_return = DataDirectory + "GetMyvaultFileListReturn.txt";
const string fileinfo_return = DataDirectory + "GetFileinfoReturn.txt";
const string recipients_return = DataDirectory + "GetUpdateRecipientsReturn.txt";
const string activity_return = DataDirectory + "GetActivityReturn.txt";
const string filetoken_return = DataDirectory + "GetFileToeknReturn.txt";
const string hearbeat_return = DataDirectory + "GetHearBeatReturn.txt";
const string projectlist_return = DataDirectory + "GetProjectListReturn.txt";
const string projectfilelist_return = DataDirectory + "GetProjectFileListReturn.txt";
const string projectfilemetadatalist_return = DataDirectory + "GetProjectFileMetadataListReturn.txt";
const string shrewithmefilelist_return = DataDirectory + "GetShareWithMeListReturn.txt";
const string filemetadatalist_return = DataDirectory + "GetFileMetadataReturn.txt";
const string projectmemberlist_return = DataDirectory + "GetProjectMemberListReturn.txt";
const string projectinvitemember_return = DataDirectory + "GetProjectInviteMemberReturn.txt";
const string invokefile_return = DataDirectory + "InvokeFileReturn.txt";
const string deletefile_return = DataDirectory + "DeleteFileReturn.txt";
const string adminlogin_return = DataDirectory + "AdminLoginReturn.txt";
const string adminloginCookie = DataDirectory + "LdapAuthFinishCookie.txt";
const string projectAdminList_resturn = DataDirectory + "ProjectAdminListReturn.txt";
const string tenantList_return = DataDirectory + "TenantListReturn.txt";
const string addRemoveTenantAdmin_return = DataDirectory + "AddRemoveTenantAdminReturn.txt";
const string addRemoveProjectAdmin_return = DataDirectory + "AddRemoveProjectAdminReturn.txt";
const string policyEval_return = DataDirectory + "PolicyEvalReturn.txt";
const string userPendingInvitations_return = DataDirectory + "UserPendingInvitationsReturn.txt";
const string acceptInvitation_return = DataDirectory + "AcceptInvitationReturn.txt";
const string checkUpgrade_return = DataDirectory + "CheckUpgradeReturn.txt";


const RM_ProjectFileDownloadType Default_DownloadType = RD_Normal; //projectFileDownloadType_Default


//const string DEFUALT_USER_FILE = plan_path + "User_Login_7303.txt";
//const wstring clientid = L"29F82871FE4DEF0AA25151DA9498642B";
const string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";

const string passcode_txt = "passcode.txt";
const string tag_txt = "TagString.txt";

//////Test file, normal files
const wstring invalidnxl_testfile = L"nxl header.txt.nxl";
const wstring txtnormal_testfile = L"Protect_normal.txt";
const wstring txtnormal_testfileb = L"protect_normalB.txt";
const wstring txtempty_testfile = L"New Text Document.txt";
const wstring docnormal_testfile = L"abcd.docx";
//const wstring xlsnormal_testfile = L"upload-2017-04-27.xls";
const wstring xlsnormal_testfile = L"RMC_test.xlsx";
const wstring pdfnormal_testfile = L"1.3.pdf";
const wstring jpgnormal_testfile = L"t.jpg";
const wstring pptxnormal_testfile = L"Course Introduction .pptx";
const wstring bmpnormal_testfile = L"t.bmp";
const wstring pngnormal_testfile = L"t.png";
const wstring mp3normal_testfile = L"mp3.mp3";
const wstring opennormal_testfile = L"How to update build on RMS.docx";
const wstring longname_testnxlfile = L"qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq.txt.nxl";


////Test file, server downloaded files
const wstring serveractivity_testfile = L"activityLog-2018-07-16-04-28-44.txt.nxl";
const wstring servernever_testfile = L"never-2018-07-06-06-19-59.txt.nxl";
const wstring serverrelative_testfile = L"relative-2018-07-06-05-55-59.txt.nxl";
const wstring serverabsolute_testfile = L"absolute (2)-2018-07-06-05-56-29.txt.nxl";
const wstring serverrange_testfile = L"range-2018-07-06-05-57-15.txt.nxl";
const string servernever_testfiles = "never-2018-07-06-06-19-59.txt.nxl";
const string serverrelative_testfiles = "relative-2018-07-06-05-55-59.txt.nxl";
const string serverabsolute_testfiles = "absolute (2)-2018-07-06-05-56-29.txt.nxl";
const string serverrange_testfiles = "range-2018-07-06-05-57-15.txt.nxl";

const wstring serveshared_testfile = L"share-2018-07-12-06-35-38.txt.nxl";
const wstring serveshared2_testfile = L"RMC_Test-2018-08-16-07-23-12.docx.nxl";
const wstring servegetlog_testfile = L"1.3-2018-07-17-10-33-31.pdf.nxl";
const wstring log_userb_testfile = L"Current-2019-01-29-03-07-07.txt.nxl";
const wstring log_userc_testfile = L"Test_code2-2018-07-03-02-39-43.txt.nxl";
const wstring open_usera_testfile = L"activityLog-2018-07-16-04-28-44.txt.nxl";
const wstring open_userc_testfile = L"UserC.txt.nxl";
const wstring expire_testfile = L"1.3-2018-11-09-06-21-56.pdf.nxl";
const wstring expirerevoke_testfile = L"share20181106-2018-11-16-09-01-36.txt.nxl";
const wstring expiredelete_testfile = L"WIN10-2018-11-16-09-07-24.PNG.nxl";
const wstring invalid_testfile = L"InvalidNXLFile.txt.nxl";

//const string DEFUALT_USER_FILE = plan_path + "User_Login_7303.txt";
//const wstring clientid = L"29F82871FE4DEF0AA25151DA9498642B";

const wstring clientid = L"29F82871FE4DEF0AA25151DA9498642A";

const vector<vector<string>> csv_lib = readCSV(DataDirectory +"lib.csv");
const vector<vector<string>> csv_valid = readCSV(DataDirectory + "User_Login_Valid.csv");
const vector<vector<string>> csv_share = readCSV(DataDirectory + "User_Share.csv");
const vector<vector<string>> csv_protect = readCSV(DataDirectory + "User_Protect.csv");
const vector<vector<string>> csv_project = readCSV(DataDirectory+ "Project.csv");
const vector<vector<string>> csv_projectFiles = readCSV(DataDirectory + "Project_files.csv");
const vector<vector<string>> csv_Session = readCSV(DataDirectory + "Session.csv");
const vector<vector<string>> csv_Session_GetLoginUser = readCSV(DataDirectory + "Session_GetLoginUser.csv");
const vector<vector<string>> csv_JPC = readCSV(DataDirectory + "JPC.csv");
const vector<vector<string>> csv_JPC_specialInput = readCSV(DataDirectory + "JPC_special.csv");
const vector<vector<string>> csv_UserB_OpenFile = readCSV(DataDirectory + "UserB_OpenFile.csv");
const vector<vector<string>> csv_User_InvalidProtect = readCSV(DataDirectory + "User_InvalidProtect.csv"); 
const vector<vector<string>> csv_JPC_invalidinput = readCSV(DataDirectory + "JPC_pathInput.csv");
const vector<vector<string>> csv_JPC_GetResourceRights = readCSV(DataDirectory + "JPC_GetResourceRights.csv");
const vector<vector<string>> csv_IsFileProtectedTest = readCSV(DataDirectory + "NXL_filePathInput.csv");
const vector<vector<string>> csv_RPMTest = readCSV(DataDirectory + "RPMTest.csv");
const vector<vector<string>> csv_RPMSpin = readCSV(DataDirectory + "RPM_Spin.csv");
const vector<vector<string>> csv_filePath = readCSV(DataDirectory + "filepath.csv");
const vector<vector<string>> csv_appPath = readCSV(DataDirectory + "appPath.csv");
const vector<vector<string>> csv_RPM_deleteFileToken = readCSV(DataDirectory + "RPM_deleteFileToken.csv");
const vector<vector<string>> csv_project_ID = readCSV(DataDirectory + "Project_ID.csv");
const vector<IExpiryType> csv_expiration = { NEVEREXPIRE, RELATIVEEXPIRE, ABSOLUTEEXPIRE, RANGEEXPIRE };
const vector<vector<string>> csv_UserChangeRights = readCSV(DataDirectory + "UserChangeRights.csv");
const vector<vector<string>> csv_PolicyPreview = readCSV(DataDirectory + "10.3PolicyPreview.csv");
const vector<vector<string>> csv_filelist = readCSV(DataDirectory + "filelist.csv");
const vector<vector<string>> csv_performence = readCSV(DataDirectory + "Performance.csv");


const vector<vector<string>> csv_Auto_ExplorerAddon = readCSV(DataDirectory + "autoData\\Auto_ExplorerAddon.csv");
const vector<vector<string>> csv_Auto_ExplorerAddon2 = readCSV(DataDirectory + "autoData\\Auto_ExplorerAddon2.csv");
const vector<vector<string>> csv_WinAPI = readCSV(DataDirectory + "autoData\\WinAPI.csv");
