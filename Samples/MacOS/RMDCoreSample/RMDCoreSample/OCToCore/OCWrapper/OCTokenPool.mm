//
//  OCTokenPool.m
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 31/01/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#import "OCTokenPool.h"

#import "CoreBridge.h"

#include "rmsdk/rmsdk.h"
#include "rmsdk/restful/rmtoken.h"

using namespace RMSDK;

@interface OCTokenPool()

@property (nonatomic) RMTokenPool *tokenPool;
@end

@implementation OCTokenPool

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.tokenPool = new RMTokenPool();
    }
    return self;
}

- (void)dealloc
{
    delete self.tokenPool;
}

- (NSUInteger)getSize {
    return self.tokenPool->size();
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
    self.tokenPool->ImportFromRMSResponse(object);
    
    return true;
}

- (BOOL)importRMString: (NSString *)str {
    self.tokenPool->ImportFromString([CoreBridge getCString:str]);
    
    return true;
}

- (NSString *)exportRMString {
    std::string cstr = self.tokenPool->ExportToString();
    return [CoreBridge getString: cstr];
}

@end
