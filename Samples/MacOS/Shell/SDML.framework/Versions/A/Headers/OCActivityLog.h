//
//  OCActivityLog.h
//  SDML
//
//  Created by Paul (Qian) Chen on 19/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ExposeCClassProtocol.h"

@interface OCActivityLog : NSObject <ExposeCClassProtocol>

// Expose c class

- (void)setCObject: (NSValue *)value;
- (NSValue *)getCObject;

@end
