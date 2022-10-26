//
//  common.hpp
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 08/01/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#ifndef common_hpp
#define common_hpp

#include "stdafx.hpp"
#include "rmsdk/restful/rmuser.h"
#include "rmsdk/restful/rmtenant.h"
#include "RMSDK/network/httpClient.h"

const std::string readFromFile(std::string filename);

const std::string generateClientID(void);

const RMSDK::RMTenant createDefaultTenant(void);

void printHttpRequest(const RMSDK::HTTPRequest request);

void printIDPType(int type);

void printMembership(RMSDK::RMMembership & mem);

#endif /* common_hpp */
