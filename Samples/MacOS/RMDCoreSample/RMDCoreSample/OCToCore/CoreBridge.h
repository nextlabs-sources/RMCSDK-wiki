//
//  Common.h
//  SDML
//
//  Created by Paul (Qian) Chen on 18/01/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <iostream>
#include "rmsdk/network/httpClient.h"
#include "rmsdk/utils/json.h"

using namespace RMSDK;

@interface CoreBridge : NSObject

+ (NSURLRequest *)getURLRequest: (HTTPRequest)request;
+ (NSString *)getString: (std::string)cstring;
+ (std::string)getCString: (NSString *)str;

@end
