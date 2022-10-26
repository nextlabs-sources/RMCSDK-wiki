//
//  OCLogPool.h
//  SDML
//
//  Created by Paul (Qian) Chen on 19/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RestfulProtocol.h"
#import "ExposeCClassProtocol.h"

@class OCActivityLog;

@interface OCLogPool : NSObject <RestfulProtocol, ExposeCClassProtocol>

- (NSValue *)getCObject;

// Wrap Core.

- (void)addLog: (OCActivityLog *)log;
- (NSString *)exportString;
- (NSError *)importString: (NSString *)str;
- (NSUInteger)size;

// Restful

- (NSError *)importRMSResponse: (NSString *)response;

@end
