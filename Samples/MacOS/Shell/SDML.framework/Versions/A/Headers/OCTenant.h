//
//  OCTenant.h
//  SDML
//
//  Created by Paul (Qian) Chen on 17/01/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RestfulProtocol.h"
#import "ExposeCClassProtocol.h"

@interface OCTenant : NSObject <RestfulProtocol, ExposeCClassProtocol>

- (instancetype)initWithRouter: (NSString *)router tenant: (NSString *)tenant;

- (NSString *)getTenant;
- (NSString *)getRouterURL;
- (NSString *)getRMSURL;

- (NSURLRequest *)getTenantQuery;

// ExposeCClassProtocol
- (NSValue *)getCObject;

// RestfulProtocol
- (NSError *)importRMSResponse: (NSString *)response;
- (NSError *)importRMString: (NSString *)str;
- (NSString *)exportRMString;

@end
