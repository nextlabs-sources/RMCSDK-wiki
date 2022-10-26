//
//  RestfulProtocol.h
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 01/02/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

@protocol RestfulProtocol

@optional
- (NSError *)importRMString: (NSString *)str;
- (NSString *)exportRMString;
- (NSError *)importRMSResponse: (NSString *)response;

@end
