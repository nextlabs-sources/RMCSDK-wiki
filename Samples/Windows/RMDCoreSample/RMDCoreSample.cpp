// RMDCoreSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TenantTest.h"
#include "UserTest.h"
#include "TokenTest.h"
#include "NxlTest.h"
#include "RecipTest.h"
#include "LogTest.h"
#include "MyValutTest.h"
#include "HBtest.h"
#include "MyDriveTest.h"
#include "UpdateTest.h"
#include "ProjectTest.h"
#include "ProjectFilesTest.h"

using namespace std;

int main()
{
	TestTenant();

	TestUser();

	TestToken();

	TestNxlFile();

	TestProtectFile();

	TestRecipients();

	TestNxlLog();

	TestMyVault();

	TestHeartbeat();

	TestMyDrive();

	TestProject();

	TestProjectFiles();

	TestAutoUpdate();

	cout << "Press any key to quit...";
	_getch();
	return 0;
}

