//
//  UserTest.m
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 22/12/2017.
//  Copyright © 2017 nextlabs. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "UserTest.h"

#import "rmsdk/rmsdk.h"
#import "rmsdk/restful/rmuser.h"
#import "common.hpp"

using namespace RMSDK;

@implementation UserTest

- (int)testUser {
//    std::string clientid = generateClientID();
//
    RMUser user;
//
//    NSLog(@"Login User HTTP Request");
//    RMTenant tenant = createDefaultTenant();
//    HTTPRequest request = user.GetUserLoginURL(clientid, tenant);
//    printHttpRequest(request);
    
    JsonDocument doc;
    int err_code = 0;
    size_t err_pos = 0;
    
//    const std::string jsonstr = readFromFile("RMSBasicLogin.txt");
    NSString *json = [NSString stringWithContentsOfFile:@"RMSBasicLogin.txt" encoding:NSUTF8StringEncoding error:nil];
    std::string jsonstr = json.UTF8String;
    if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
        return 1;
    }
    
    JsonValue *root = doc.GetRoot();
    if (!(NULL != root && root->IsObject())) {
        return 2;
    }
    
    JsonObject *object = root->AsObject();
    user.ImportFromRMSResponse(object);
    
    NSLog(@"User Name:%s", user.GetName().c_str());
    NSLog(@"User Email:%s", user.GetEmail().c_str());
    NSLog(@"User ID:%i", user.GetUserID());
    
    NSLog(@"Login type:");
    printIDPType(user.GetIdpType());
    
    NSLog(@"Default membership information:");
    printMembership(user.GetDefaultMembership());
    
    std::string tenantid = "21b06c79-baab-419d-8197-bad3ce3f4476";
    RMMembership * mem = user.FindMembership(tenantid);
    if (NULL == mem) {
        NSLog(@"Tenant %s not found.", tenantid.c_str());
    }
    else {
        NSLog(@"Found Tenant %s Information.", tenantid.c_str());
        printMembership(*mem);
    }
    
    tenantid = "8360d053-9f7d-4bcc-a265-dab4a31a92fd";
    mem = user.FindMembership(tenantid);
    if (NULL == mem) {
        NSLog(@"Tenant %s not found.", tenantid.c_str());
    }
    else {
        NSLog(@"Found Tenant %s Information.", tenantid.c_str());
        printMembership(*mem);
    }
    
    tenantid = "8360d053-9f7d-4bcc-a265-bad3ce3f4476";
    mem = user.FindMembership(tenantid);
    if (NULL == mem) {
        NSLog(@"Tenant %s not found.", tenantid.c_str());
    }
    else {
        NSLog(@"Found Tenant %s Information.", tenantid.c_str());
        printMembership(*mem);
    }
    
    NSLog(@"User data:%s", user.ExportToString().c_str());
    NSLog(@"Query membership info:");
//    request = user.GetMembershipQuery(clientid, "this computer", tenant, user.GetDefaultMembership());
//    printHttpRequest(request);
    
    return 0;
}

@end

