//
//  OCMembership.h
//  SDML
//
//  Created by Paul (Qian) Chen on 29/01/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RestfulProtocol.h"
#import "ExposeCClassProtocol.h"

@class OCCertificate;

@interface OCMembership : NSObject <RestfulProtocol, ExposeCClassProtocol>

- (instancetype)initWithValue: (NSValue *)value;

- (NSString *)getID;
- (NSString *)getTenantID;
- (NSUInteger)getIdpType;
- (NSUInteger)getProjectID;

- (void)setCertificate: (OCCertificate *)certificate;
- (Boolean)hasCertificate;

// ExposeCClassProtocol
- (NSValue *)getCObject;

// RestfulProtocol

- (NSError *)importRMSResponse: (NSString *)response;
- (BOOL)importRMString: (NSString *)str;
- (NSString *)exportRMString;


@end
