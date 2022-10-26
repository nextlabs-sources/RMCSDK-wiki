//
//  OCNXLFileLog.h
//  SDML
//
//  Created by Paul (Qian) Chen on 29/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface OCNXLFileLog : NSObject

- (instancetype)initWithValue: (NSValue *)value;

// Wrap Core.

- (NSString *)getEmail;
- (NSString *)getOperation;
- (NSString *)getDeviceType;
- (Boolean)getAccessResult;
- (NSDate *)getAccessTime;

@end
