#pragma once

RMUser GetDefaultUser();

void printUserInfo(RMUser user);

RMUser importUser(string jsonFile, int importType);
const RMUser& importUser58(string jsonFile, int RMS_ID);
const RMUser& importUser58_2(string jsonFile, int RMS_ID, RMSystemPara& clientInfo);

int ifExist_Membership(RMUser user, string tenantId_m);
RMMembership FindMembership(RMUser user, string tenantId);

#define DEFAULT_RMSCerts_FILE		"C:\\RMCCORE test data\\test file\\RMSCertsTest.txt"
#define DEFAULT_USER_PROFILE		"C:\\RMCCORE test data\\test fileDUserData.txt"

const string default_plan_file_path = "C:\\RMCCORE test data\\plan file\\";
const string  default_file_path = "C:\\RMCCORE test data\\test file\\";
const string default_generate_file_path = "C:\\RMCCORE test data\\generate file\\";