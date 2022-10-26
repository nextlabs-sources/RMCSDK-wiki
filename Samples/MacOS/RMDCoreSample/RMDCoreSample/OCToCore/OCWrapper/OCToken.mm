//
//  OCToken.m
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 31/01/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#import "OCToken.h"

#import "CoreBridge.h"

#include "rmsdk/rmsdk.h"
#include "rmsdk/restful/rmtoken.h"

using namespace RMSDK;

@interface OCToken()

@property (nonatomic) RMToken *token;
@end

@implementation OCToken

- (NSString *)getKey {
    return [CoreBridge getString: self.token->GetKey()];
}

@end
