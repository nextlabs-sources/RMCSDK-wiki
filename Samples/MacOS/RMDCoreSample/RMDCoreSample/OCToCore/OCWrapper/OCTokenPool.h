//
//  OCTokenPool.h
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 31/01/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RestfulProtocol.h"

@interface OCTokenPool : NSObject <RestfulProtocol>

// MARK: RestfulProtocol

- (BOOL)importJson: (NSString *)json;
- (BOOL)importRMString: (NSString *)str;
- (NSString *)exportRMString;

@end
