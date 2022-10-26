//
//  OCUser.m
//  SDML
//
//  Created by Paul (Qian) Chen on 19/01/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import "OCUser.h"

#import "OCTenant.h"
#import "OCMembership.h"

#import "CoreBridge.h"

#include "rmsdk/rmsdk.h"
#include "rmsdk/restful/rmtenant.h"
#include "rmsdk/restful/rmuser.h"


using namespace RMSDK;

@interface OCUser()

@property (nonatomic) RMUser *user;

@end

@implementation OCUser

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.user = new RMUser();
    }
    return self;
}

- (void)dealloc {
    delete self.user;
}

- (NSURLRequest *)getLoginURLRequest: (NSString *)clientId tenant: (OCTenant *)ocTenant {
    RMTenant *tenant = (RMTenant *)ocTenant.getTenantObject.pointerValue;
    HTTPRequest request = self.user->GetUserLoginURL(clientId.UTF8String, *tenant);
    return [CoreBridge getURLRequest:request];
}

- (NSString *)getName {
    return [CoreBridge getString:self.user->GetName()];
}

- (NSString *)getEmail {
    return [CoreBridge getString:self.user->GetEmail()];
}

- (NSUInteger)getUserIdpType {
    return self.user->GetIdpType();
}

- (OCMembership *)getDefaultMembership {
    RMMembership membership = self.user->GetDefaultMembership();
    NSValue *value = [NSValue valueWithPointer:&membership];
    return [[OCMembership alloc] initWithValue:value];
}

- (OCMembership *)findMembership: (NSString *)tenantId {
    RMMembership *membership = self.user->FindMembership([CoreBridge getCString:tenantId]);
    NSValue *value = [NSValue valueWithPointer: membership];
    return [[OCMembership alloc] initWithValue: value];
}

- (NSURLRequest *)getMembershipQuery: (NSString *)clientId deviceId: (NSString *)deviceId tenant: (OCTenant *)tenant membership: (OCMembership *)membership {
    RMTenant *rmtenant = (RMTenant *)[tenant getTenantObject].pointerValue;
    RMMembership *rmmembership = (RMMembership *)[membership getMembershipObject].pointerValue;
    HTTPRequest request = self.user->GetMembershipQuery([CoreBridge getCString:clientId], [CoreBridge getCString:deviceId], *rmtenant, *rmmembership);
    return [CoreBridge getURLRequest:request];
}

- (NSURLRequest *)getMembershipTokenQuery: (NSString *)clientId deviceId: (NSString *)deviceId tenant: (OCTenant *)tenant membership: (OCMembership *)membership count: (NSUInteger)count {
    RMTenant *rmtenant = (RMTenant *)[tenant getTenantObject].pointerValue;
    RMMembership *rmmembership = (RMMembership *)[membership getMembershipObject].pointerValue;
    HTTPRequest request = self.user->GetMembershipTokenQuery([CoreBridge getCString:clientId], [CoreBridge getCString:deviceId], *rmtenant, *rmmembership, count);
    return [CoreBridge getURLRequest:request];
}

// MARK: RestfulProtocol

- (BOOL)importJson: (NSString *)json {
    JsonDocument doc;
    int err_code = 0;
    size_t err_pos = 0;
    assert(doc.LoadJsonString([CoreBridge getCString:json], &err_code, &err_pos));
    
    JsonValue* root = doc.GetRoot();
    assert(NULL != root);
    assert(root->IsObject());
    JsonObject *object = root->AsObject();
    self.user->ImportFromRMSResponse(object);
    
    return true;
}

- (BOOL)importRMString: (NSString *)str {
    self.user->ImportFromString([CoreBridge getCString:str]);
    
    return true;
}

- (NSString *)exportRMString {
    return [CoreBridge getString:self.user->ExportToString()];
}
@end
