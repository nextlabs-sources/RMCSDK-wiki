//
//  OCNXLFileLogPool.h
//  SDML
//
//  Created by Paul (Qian) Chen on 29/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RestfulProtocol.h"

@class OCNXLFileLog;

@interface OCNXLFileLogPool : NSObject <RestfulProtocol>

- (NSUInteger)size;
- (OCNXLFileLog *)getFileLog: (NSUInteger)index;
- (NSString *)getFileName;
- (NSString *)getDUID;

// Restful

- (NSError *)importRMSResponse: (NSString *)response;

@end
