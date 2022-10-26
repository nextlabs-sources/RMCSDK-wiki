//
//  OCUser.h
//  SDML
//
//  Created by Paul (Qian) Chen on 19/01/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RestfulProtocol.h"

@class OCTenant;
@class OCMembership;

@interface OCUser : NSObject <RestfulProtocol>

- (instancetype)init;
- (NSURLRequest *)getLoginURLRequest: (NSString *)clientId tenant: (OCTenant *)ocTenant;
- (NSString *)getName;
- (NSString *)getEmail;
- (NSUInteger)getUserIdpType;
- (OCMembership *)getDefaultMembership;

- (OCMembership *)findMembership: (NSString *)tenantId;
- (NSURLRequest *)getMembershipQuery: (NSString *)clientId deviceId: (NSString *)deviceId tenant: (OCTenant *)tenant membership: (OCMembership *)membership;
- (NSURLRequest *)getMembershipTokenQuery: (NSString *)clientId deviceId: (NSString *)deviceId tenant: (OCTenant *)tenant membership: (OCMembership *)membership count: (NSUInteger)count;

// MARK: RestfulProtocol

- (BOOL)importJson: (NSString *)json;
- (BOOL)importRMString: (NSString *)str;
- (NSString *)exportRMString;

@end
