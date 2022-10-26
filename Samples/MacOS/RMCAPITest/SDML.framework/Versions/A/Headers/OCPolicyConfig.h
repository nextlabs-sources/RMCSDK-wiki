//
//  OCPolicyConfig.h
//  SDML
//
//  Created by Paul (Qian) Chen on 23/07/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RestfulProtocol.h"
#import "ExposeCClassProtocol.h"

@class OCWatermark;

@interface OCPolicyConfig : NSObject <RestfulProtocol, ExposeCClassProtocol>

- (NSString *)getPolicyBundle;
- (Boolean)hasWatermarkPolicy;
- (OCWatermark *)getWatermarkConfig;
- (NSUInteger)getPolicyLastModify;
- (NSUInteger)getPolicyBundleTimeStamp;

// Restful.
- (NSError *)importRMString: (NSString *)str;
- (NSString *)exportRMString;
- (NSError *)importRMSResponse: (NSString *)response;

// Expose.
- (NSValue *)getCObject;
- (void)setCObject: (NSValue *)value;

@end
