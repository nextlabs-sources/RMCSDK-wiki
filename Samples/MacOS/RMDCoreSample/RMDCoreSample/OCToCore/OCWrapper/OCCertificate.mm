//
//  OCCertificate.m
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 01/02/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#import "OCCertificate.h"

#import "CoreBridge.h"

#include "rmsdk/rmsdk.h"
#include "rmsdk/restful/rmcertificate.h"

using namespace RMSDK;

@interface OCCertificate()

@property (nonatomic) RMCertificate *certificate;
@end

@implementation OCCertificate

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.certificate = new RMCertificate();
    }
    return self;
}

- (void)dealloc
{
    delete self.certificate;
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
    self.certificate->ImportFromRMSResponse(object);

    return true;
}

- (BOOL)importRMString: (NSString *)str {
    self.certificate->ImportFromString([CoreBridge getCString:str]);

    return true;
}

- (NSString *)exportRMString {
    return [CoreBridge getString: self.certificate->ExportToString()];
}

@end
