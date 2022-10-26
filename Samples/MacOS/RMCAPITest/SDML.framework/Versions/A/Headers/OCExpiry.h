//
//  OCExpiry.h
//  SDML
//
//  Created by Paul (Qian) Chen on 15/06/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ExposeCClassProtocol.h"

@interface OCExpiry : NSObject <ExposeCClassProtocol>

- (instancetype)initWithType: (NSInteger)type start: (UInt64)start end: (UInt64)end;

- (NSValue *)getCObject;
- (void)setCObject: (NSValue *)value;

- (NSInteger)getType;
- (UInt64)getStartTime;
- (UInt64)getEndTime;

@end
