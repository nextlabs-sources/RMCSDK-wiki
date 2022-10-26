#pragma once

#include "rmccore\restful\rmnxlfile.h"
#include "rmccore\restful\rmuser.h"
#include "rmccore\restful\rmactivitylog.h"
#include "stdafx.h"
using namespace std;
using namespace NXLFMT;

uint64_t getRightsValue(string rights_csv);
void printRights(NXLFMT::Rights rights);

std::wstring s2ws(const std::string& s);
int GetFileSpaceSize(string path);
void EXPECT_EQ_CompareFileSpaceSize(string sourceFile, string destNXL);

const RMNXLFile& GetNXLFile(string fileName);
vector<std::string> ReturnRecipient(string recipient);
void check_rights(RMNXLFile nfile, NXLFMT::Rights rights);
void check_recipients(RMNXLFile nfile, vector<std::string> recipient);





