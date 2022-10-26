// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <iostream>

#include "windows.h"

#include "gtest\gtest.h"
#include "json\json.h"
// TODO: reference additional headers your program requires here

#include <sstream> 
#include <fstream>
#include "string"

using namespace std;

#include "Header_DataConvert.h"
#include "Header_util.h"
#include "Header_UserLogin.h"
#include "Header_GlobalData.h"
#include "Header_NXLFile.h"
#include "Header_User.h"
#include "Header_RESTAPI.h"

#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog\logging.h"
using namespace google;