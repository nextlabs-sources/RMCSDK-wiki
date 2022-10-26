#include "stdafx.h"

int main(int argc, char **argv)
{
	//testing::GTEST_FLAG(filter) = "RMUserTesting*";
	testing::GTEST_FLAG(filter) = "HTTPRequestTesting*";
	testing::GTEST_FLAG(filter) = "NXLFileTesting*";
	
	//testing::GTEST_FLAG(filter) = "RMNXLFileTesting*";
	//testing::GTEST_FLAG(filter) = "ExpireTimeTest.ExpireTest";
	//testing::GTEST_FLAG(filter) = "UploadActivityLogTest.Upload";
	//testing::GTEST_FLAG(filter) = "CertificateErrorResponseReturn*";
	//testing::AddGlobalTestEnvironment(new RMCCoreEnvironment);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
