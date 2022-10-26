//
//  OCSystemPara.h
//  SDML
//
//  Created by Paul (Qian) Chen on 13/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ExposeCClassProtocol.h"
#import "RestfulProtocol.h"

@interface OCSystemPara : NSObject<ExposeCClassProtocol, RestfulProtocol>

- (void)setPlatformID: (NSInteger)platformID;
- (void)setDeviceID: (NSString *)deviceID;
- (void)setClientID: (NSString *)clientID;

- (NSInteger)getPlatformID;
- (NSString *)getDeviceID;
- (NSString *)getClientID;

// ExposeCClassProtocol

- (NSValue *)getCObject;

// Restful

- (NSError *)importRMString: (NSString *)str;
- (NSString *)exportRMString;

@end
