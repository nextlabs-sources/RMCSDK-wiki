#pragma once
#include "stdafx.h"
#include "rmccore\restful\rmtoken.h"
#include "rmccore\restful\rmuser.h"
#include "rmccore\restful\rmtenant.h"

//#include "Header_Tenant.h"


RMTokenPool & GetDefaultTokens();
#define DEFAULT_TOKEN_POOL_FILENAME		"C:\\RMCCORE test data\\test file\\successtoken.json"