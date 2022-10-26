//
//  TokenTest.cpp

//
//  Created by Jewelry.zhu 2018-02-27
//  Copyright ?2018 NextLabs, Inc. All rights reserved.
//

#include "stdafx.h"
#include "RMCCORE\restful\rmsyspara.h"

using namespace RMCCORE;
using namespace std;

const RMSystemPara CreateDefaultSysPara(void)
{
	RMSystemPara sys_para;
	RMPlatformID platform_id = RPWindows7;

	sys_para.SetDeviceID("d-id553543434141414134134");
	sys_para.SetPlatformID(platform_id);

	cout << "Export to string: " << sys_para.ExportToString() << endl;
	return sys_para;
}

//Test syspara and user.UpdateSystemParameters
class SysParamTest :public::testing::TestWithParam<vector<string>> {};
TEST_P(SysParamTest, SysParam)
{
	string paroduct_name, product_path, product_publisher, device_id;
	try {
		RMUser user = GetDefaultUser();
		RMSystemPara sys_para;
		RMPlatformID platform_enum ;
		RMProcessorArch processor_enum;
		uint32_t major_ver, minor_ver, build_no;
		major_ver = 111;
		minor_ver = 222;
		build_no = 444;
		//get parameter of file name, type is string
		vector<string> csv = GetParam();
		paroduct_name = csv[2];
		product_path = csv[6];
		product_publisher = csv[7];
		string platform_id = csv[9];
		string clientId_set = csv[10];
		device_id = csv[8];
		char* end;
		int p_id = static_cast<int>(strtol(platform_id.c_str(), &end, 10));
		string processor_type = csv[11];
		int processor_id = static_cast<int>(strtol(processor_type.c_str(), &end, 10));
		
		switch (p_id) {
		case 4: platform_enum = RPWindows7; break;
		case 5: platform_enum = RPWindows8; break;
		case 7: platform_enum = RPWindows10; break;
		case 300: platform_enum = RPMac; break;
		case 900: platform_enum = RPAndriodTablet; break;
		default: platform_enum = RPWindowsDesktop; break;
		
		}
		switch (processor_id) {
		case 0: processor_enum = RPAUnknown; break;
		case 1: processor_enum = RPAWindowsX86; break;
		case 2: processor_enum = RPAWindowsX64; break;
		case 3: processor_enum = RPAWindowsIA64; break;
		default: processor_enum = RPAUnknown; break;
		}

		RMProduct product(paroduct_name, major_ver, minor_ver, build_no);
		sys_para.SetDeviceID(device_id);
		sys_para.SetPlatformID(platform_enum);
		sys_para.SetProduct(product);
		sys_para.SetClientID(clientId_set);
		sys_para.SetProcessor(processor_enum);
		sys_para.GetProcessorType();
		EXPECT_STREQ(sys_para.GetClientID().c_str(), clientId_set.c_str());
		EXPECT_EQ(sys_para.GetPlatformID(), platform_enum);
		EXPECT_STREQ(device_id.c_str(), sys_para.GetDeviceID().c_str());
		EXPECT_EQ(sys_para.GetProcessorType(), processor_enum);

		user.UpdateSystemParameters(sys_para);
		HTTPRequest http = user.GetUserLogoutQuery();
		http::header hd = http.GetHeaders()[0];
		string header_client = hd.second;
		EXPECT_STREQ(sys_para.GetClientID().c_str(), header_client.c_str());
		
		cout << "Get client id: " << sys_para.GetClientID() << endl;
		cout << "Get platform id: " << sys_para.GetPlatformID() << endl;
		cout << "Get device id: " << sys_para.GetDeviceID() << endl;
		cout << "Test finished." << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}
vector<vector<string>> sys_para = readCSV("SysParaProductInfoTest.csv");
INSTANTIATE_TEST_CASE_P(SysParamTestReturn, SysParamTest, testing::ValuesIn(sys_para));


//Test Product
TEST(SysParaProductTest,ProductTest) {
	try {
		cout << "Test Finished!" << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		cout << "Exceptions!" << endl;
		throw;
	}
}

