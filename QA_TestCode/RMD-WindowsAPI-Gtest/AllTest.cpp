#include "stdafx.h"

int main(int argc, char **argv)
{

	// Start google log system:
	string logDir = TestDirectory + "Logs";
	createFolder(logDir);
	FLAGS_log_dir = logDir;
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::GLOG_INFO, logDir.append("\\SDK_").c_str());
	google::SetStderrLogging(google::GLOG_INFO);
	google::SetLogFilenameExtension("log_");
	FLAGS_colorlogtostderr = true;  // Set log color
	FLAGS_logbufsecs = 0;  // Set log output speed(s)
	FLAGS_max_log_size = 1024;  // Set max log file size
	FLAGS_stop_logging_if_full_disk = true;  // If disk is full

	testing::GTEST_FLAG(output) = "xml:SDK_TestReport.xml";

	//testing::GTEST_FLAG(filter) = "JPCtest.tokenCases*";
	//testing::GTEST_FLAG(filter) = "JPCtest*in*";
	//testing::GTEST_FLAG(filter) = "JPCtest*GetHeartBeatInfo*";
	//testing::GTEST_FLAG(filter) = "JPCtest*policyenv*";
	//testing::GTEST_FLAG(filter) = "SessionGetloginUser*";
	//testing::GTEST_FLAG(filter) = "ProjectTest*";
	//testing::GTEST_FLAG(filter) = "RPMTest.runcase*";
	//testing::GTEST_FLAG(filter) = "RPMTest.run*";
	//testing::GTEST_FLAG(filter) = "RPM.john*";
	//testing::GTEST_FLAG(filter) = "RPMTest.runlogin*";
	//testing::GTEST_FLAG(filter) = "RPMTest.serverStopEnable*";
	//testing::GTEST_FLAG(filter) = "RPMTest.bugverify*";
	//testing::GTEST_FLAG(filter) = "RPMTest.base*";
	//testing::GTEST_FLAG(filter) = "RPMTest.forExplorerAddon2*";
	//testing::GTEST_FLAG(filter) = "RPMTest.GetUserInfo*";
	//testing::GTEST_FLAG(filter) = "RPMTest.protect*";
	//testing::GTEST_FLAG(filter) = "processTest*";
	//testing::GTEST_FLAG(filter) = "RPMTest.OfflineCase*";
	//testing::GTEST_FLAG(filter) = "SessionTesting*SessionInterface*";
	//testing::GTEST_FLAG(filter) = "Policy.cleanDataInCC*";
	
	testing::GTEST_FLAG(filter) = "TrustApp.app1*";
	//testing::GTEST_FLAG(filter) = "TrustApp.App0*";
	//testing::GTEST_FLAG(filter) = "TrustApp.App_getloginuser*";
	//testing::GTEST_FLAG(filter) = "RPMEditSaveTCTest999*";
	//testing::GTEST_FLAG(filter) = "TrustApp.AppRegister*";

	//testing::GTEST_FLAG(filter) = "*UserGetrights*Getrights*";

	//testing::GTEST_FLAG(filter) = "Offline.firstLogin*";
	//testing::GTEST_FLAG(filter) = "Offline.GetLogin*";
	//testing::GTEST_FLAG(filter) = "Login.AD*";
	//testing::GTEST_FLAG(filter) = "Offline.*";
	//testing::GTEST_FLAG(filter) = "RPM.OpenfileWorkflow*";
	//testing::GTEST_FLAG(filter) = "RPM.RPMSetAppRegistry*";
	//testing::GTEST_FLAG(filter) = "ReadFileBeginningChar*";
	//testing::GTEST_FLAG(filter) = "RPM*Spin_OpenSameFileFor2Users*";
	//testing::GTEST_FLAG(filter) = "SDKLogin.com*";
	//testing::GTEST_FLAG(filter) = "V103*";
	//testing::GTEST_FLAG(filter) = "V104*";
	//testing::GTEST_FLAG(filter) = "V105*";
	//testing::GTEST_FLAG(filter) = "V103*protect_new";
	//testing::GTEST_FLAG(filter) = "V103*ChangeRightsOfFile*";
	
	//testing::GTEST_FLAG(filter) = "ProjectListTest.ProjectListTest";
	//testing::GTEST_FLAG(filter) = "IsFileProtectedTest*";
	//testing::GTEST_FLAG(filter) = "Test_RPM*threads*";
	//testing::GTEST_FLAG(filter) = "Test_RPM*all_100times*";
	//testing::GTEST_FLAG(filter) = "RPM*forTCtesting*";
	//testing::GTEST_FLAG(filter) = "RPM*AutoExplorerAddon*";
	//testing::GTEST_FLAG(filter) = "TC*Win*";
	//testing::GTEST_FLAG(filter) = "RPMTest*RPM_Explorer*operateDir*";
	//testing::GTEST_FLAG(filter) = "RPM*withoutLogin";
	//testing::GTEST_FLAG(filter) = "RPMViewEditSystemBucketCPFileTest*";
	//testing::GTEST_FLAG(filter) = "Security.*";
	//testing::GTEST_FLAG(filter) = "RPM*logoutIssue*";


	testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	google::ShutdownGoogleLogging();
	return ret;
}