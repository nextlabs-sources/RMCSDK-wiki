//
//  OCActivityLog.h
//  SDML
//
//  Created by Paul (Qian) Chen on 19/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ExposeCClassProtocol.h"

@class OCNXLFile;
@class OCUser;

@interface OCActivityLog : NSObject <ExposeCClassProtocol>

- (instancetype)initWithFile: (OCNXLFile *)file user: (OCUser *)user operation: (NSString *)operation isAllow: (Boolean)isAllow;
// Expose c class

- (void)setCObject: (NSValue *)value;
- (NSValue *)getCObject;

@end
