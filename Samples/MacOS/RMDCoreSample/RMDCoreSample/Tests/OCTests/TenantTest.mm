//
//  TenantTest.m
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 05/01/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#import "TenantTest.h"

#include "rmsdk/rmsdk.h"
#include "rmsdk/restful/rmtenant.h"
#include "common.hpp"

#define JSON_RETURN  "{\"statusCode\":200,\"message\":\"OK\",\"serverTime\":1512505670638,\"results\":{\"server\":\"https://rmtest.nextlabs.solutions/rms\"}}"
//#define Tenant_Return  "{\"statusCode\":200,\"message\":\"OK\",\"serverTime\":1512505538781,\"results\":{\"server\":\"https://skydrm.com/rms\"}}"
using namespace RMSDK;

@implementation TenantTest

- (int)testTenant {
    RMTenant tenant; // = createDefaultTenant();
    NSLog(@"Tenant ID: %s", tenant.GetTenant().c_str());
    NSLog(@"Tenant Router: %s", tenant.GetRouterURL().c_str());
    NSLog(@"Tenant RMS URL: %s", tenant.GetRMSURL().c_str());
    NSLog(@"export to string: %s", tenant.ExportToString().c_str());

    HTTPRequest req = tenant.GetTenantQuery();
    printHttpRequest(req);

    tenant.SetRouter("https://rmtest.nextlabs.solutions");
    tenant.SetTenant("skydrm.com");

    JsonDocument doc;
    int err_code = 0;
    size_t err_pos = 0;

    const std::string jsonstr = JSON_RETURN;
    if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
        return 1;
    }

    JsonValue* root = doc.GetRoot();
    if (!(NULL != root && root->IsObject())) {
        return 2;
    }
    tenant.ImportFromRMSResponse(root->AsObject());

    NSLog(@"Tenant ID: %s", tenant.GetTenant().c_str());
    NSLog(@"Tenant Router: %s", tenant.GetRouterURL().c_str());
    NSLog(@"Tenant RMS URL: %s", tenant.GetRMSURL().c_str());
    NSLog(@"export to string: %s", tenant.ExportToString().c_str());

    printHttpRequest(tenant.GetTenantQuery());
    
    NSLog(@"%d", tenant.ExportToJson()->IsObject());
    NSLog(@"%d", tenant.ExportToJson()->IsString());

    return 0;
}

@end

