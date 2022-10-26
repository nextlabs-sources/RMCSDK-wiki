//
//  TokenTest.m
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 08/01/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#import "TokenTest.h"

#include "rmsdk/rmsdk.h"
#include "rmsdk/restful/rmtoken.h"
#include "common.hpp"

using namespace RMSDK;

@implementation TokenTest

- (int)testToken {
    std::string clientid = generateClientID();
    
    //    create a user class first
    RMUser user;
    
    JsonDocument doc;
    int err_code = 0;
    size_t err_pos = 0;
    
    const std::string userstr = readFromFile("RMSBasicLogin.txt");
    if (!doc.LoadJsonString(userstr, &err_code, &err_pos)) {
        return 1;
    }
    
    JsonValue *root = doc.GetRoot();
    if (!(NULL != root && root->IsObject())) {
        return 2;
    }
    user.ImportFromRMSResponse(root->AsObject());
    NSLog(@"Import basic user login");
    
    const std::string jsonstr = readFromFile("RMSCertsTest.txt");
    if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
        return 1;
    }
    
    root = doc.GetRoot();
    if (!(NULL != root && root->IsObject())) {
        return 2;
    }
//    user.GetDefaultMembership().ImportCertficateFromRMSResponse(root->AsObject());
    
    RMTenant tenant = createDefaultTenant();
    HTTPRequest request = user.GetMembershipTokenQuery(clientid, "test device", tenant, user.GetDefaultMembership(), 100);
    printHttpRequest(request);
    
    NSLog(@"User data:%s", user.ExportToString().c_str());
    
    RMTokenPool tpool;
    
    const std::string tokenstr = readFromFile("RMSTokenTest.txt");
    if (!doc.LoadJsonString(tokenstr, &err_code, &err_pos)) {
        return 1;
    }
    
    root = doc.GetRoot();
    if (!(NULL != root && root->IsObject())) {
        return 2;
    }
    
    tpool.ImportFromRMSResponse(root->AsObject());
    NSLog(@"Imported %i tokens", tpool.size());
    
    for (int i = 0; i < 10; i++) {
        RMToken token = tpool.pop();
        NSLog(@"Remove Token :%s", token.GetKey().c_str());
    }
    
    const std::string tokenstr1 = readFromFile("RMSTokenTest1.txt");
    if (!doc.LoadJsonString(tokenstr1, &err_code, &err_pos)) {
        return 1;
    }
    
    root = doc.GetRoot();
    if (!(NULL != root && root->IsObject())) {
        return 2;
    }
    
    tpool.ImportFromRMSResponse(root->AsObject());
    NSLog(@"Import another set of tokens");
    NSLog(@"Total %i tokens", tpool.size());
    
    NSLog(@"Token data: %s", tpool.ExportToString().c_str());
    
    return 0;
}

@end

