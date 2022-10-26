//
//  OCMyDrive.h
//  SDML
//
//  Created by Paul (Qian) Chen on 11/04/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RestfulProtocol.h"
#import "ExposeCClassProtocol.h"

@interface OCMyDrive : NSObject <RestfulProtocol, ExposeCClassProtocol>

- (NSUInteger)getUsage;
- (NSUInteger)getQuota;
- (NSUInteger)getVaultUsage;
- (NSUInteger)getVaultQuota;

- (NSURLRequest *)getStorageQuery;

// Expose
- (void)setCObject:(NSValue *)value;

// Restful
- (NSError *)importRMSResponse: (NSString *)response;

@end
