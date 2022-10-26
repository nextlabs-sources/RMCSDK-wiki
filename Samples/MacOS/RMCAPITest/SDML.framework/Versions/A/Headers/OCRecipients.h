//
//  OCRecipients.h
//  SDML
//
//  Created by Paul (Qian) Chen on 19/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RestfulProtocol.h"
#import "ExposeCClassProtocol.h"

@interface OCRecipients : NSObject <RestfulProtocol, ExposeCClassProtocol>

- (instancetype)initWithValue: (NSValue *)value;
- (instancetype)initWithRecipientList: (NSArray *)list;

- (NSArray *)getRecipients;
- (NSArray *)getAddRecipients;
- (NSArray *)getRemoveRecipients;
- (NSString *)getComments;
- (void)updateComment: (NSString *)comment;

- (void)addRecipients: (NSArray *)recipients;
- (void)removeRecipients: (NSArray *)recipients;

// Restful

- (NSError *)importRMSResponse: (NSString *)response;

// Expose C class
- (NSValue *)getCObject;

@end
