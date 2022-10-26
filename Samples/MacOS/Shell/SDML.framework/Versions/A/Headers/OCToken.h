//
//  OCToken.h
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 31/01/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ExposeCClassProtocol.h"
#import "RestfulProtocol.h"

@interface OCToken : NSObject<ExposeCClassProtocol, RestfulProtocol>


- (instancetype)initWithValue: (NSValue *)value;
- (instancetype)initWithDUID: (NSString *)duid key: (NSString *)key ml: (NSUInteger)ml;

- (NSString *)getDuid;
- (NSString *)getKey;
- (NSUInteger)getLevel;

// Expose protocol
- (NSValue *)getCObject;

- (NSError *)importRMSResponse: (NSString *)response;

@end
