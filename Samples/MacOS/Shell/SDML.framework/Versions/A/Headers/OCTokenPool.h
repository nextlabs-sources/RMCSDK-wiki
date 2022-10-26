//
//  OCTokenPool.h
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 31/01/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RestfulProtocol.h"
#import "ExposeCClassProtocol.h"

@class OCToken;

@interface OCTokenPool : NSObject <RestfulProtocol, ExposeCClassProtocol>

- (NSUInteger)getSize;
- (OCToken *)pop;

// ExposeCClassProtocol
- (NSValue *)getCObject;

// RestfulProtocol
- (NSError *)importRMSResponse: (NSString *)response;
- (NSError *)importRMString: (NSString *)str;
- (NSString *)exportRMString;

@end
