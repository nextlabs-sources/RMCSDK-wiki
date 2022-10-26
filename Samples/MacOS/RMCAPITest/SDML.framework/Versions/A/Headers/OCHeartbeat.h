//
//  OCHeartbeat.h
//  SDML
//
//  Created by Paul (Qian) Chen on 10/04/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RestfulProtocol.h"
#import "ExposeCClassProtocol.h"

@class OCWatermark;
@class OCPolicyConfig;

@interface OCHeartbeat : NSObject <RestfulProtocol, ExposeCClassProtocol>

- (NSUInteger)getFrequency;

// Watermark.
- (OCWatermark *)getDefaultWatermarkSetting;

// Policy.
- (NSUInteger)getPolicyConfigCount;
- (NSString *)getPolicyConfigTenantID: (NSInteger)index;
- (Boolean)getPolicyConfig: (NSString *)tenantID config: (OCPolicyConfig *)config;

// Expose C class
- (NSValue *)getCObject;

// Restful protocol
- (NSError *)importRMSResponse:(NSString *)response;
- (NSError *)importRMString: (NSString *)str;
- (NSString *)exportRMString;

@end
