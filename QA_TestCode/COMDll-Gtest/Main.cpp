#include "pch.h"

int main(int argc, char **argv)
{
	//testing::GTEST_FLAG(output) = "xml:COMdll_TestReport.xml";

	//testing::GTEST_FLAG(filter) = "singleViewFileTest.singleViewFile";
	//testing::GTEST_FLAG(filter) = "singleGetRightsTest.singleGetRights";
	//testing::GTEST_FLAG(filter) = "singleProtectFileTest.singleProtectFile";
	//testing::GTEST_FLAG(filter) = "singLockFileSyncTest.singLockFileSync";
	testing::GTEST_FLAG(filter) = "singResumeFileSyncTest.singResumeFileSync";
	//testing::GTEST_FLAG(filter) = "LockFileSync.test";
	//testing::GTEST_FLAG(filter) = "nxrcomGetFileRightsTesting*";
	//testing::GTEST_FLAG(filter) = "nxrcomProtectFileTesting*";
	//testing::GTEST_FLAG(filter) = "nxrcomViewFileTesting*";

	testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	return ret;
}