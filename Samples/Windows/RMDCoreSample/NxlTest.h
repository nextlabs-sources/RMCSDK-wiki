#pragma once
#include "rmccore\rmccore.h"
#include "common.h"
#include "rmccore\format\nxlfile.h"
#include "rmccore\restful\rmactivitylog.h"
#include "rmccore\restful\rmnxlfile.h"

using namespace std;
using namespace RMCCORE;

void TestNxlFile(void);

void TestProtectFile(void);

const RMCCORE::RMNXLFile & GetDefaultNXLFile(void);

const RMCCORE::RMLogPool & GetDefaultLogPool(void);

