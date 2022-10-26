//
//  OCCertificate.h
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 01/02/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RestfulProtocol.h"
#import "ExposeCClassProtocol.h"

@interface OCCertificate : NSObject <RestfulProtocol, ExposeCClassProtocol>

// ExposeCClassProtocol
- (NSValue *)getCObject;

// RestfulProtocol
- (NSError *)importRMSResponse: (NSString *)response;
- (NSError *)importRMString: (NSString *)str;
- (NSString *)exportRMString;

@end
