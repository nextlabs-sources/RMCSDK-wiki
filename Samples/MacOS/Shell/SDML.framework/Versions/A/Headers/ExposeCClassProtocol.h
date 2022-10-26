//
//  ExposeCClassProtcol.h
//  SDML
//
//  Created by Paul (Qian) Chen on 02/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

@protocol ExposeCClassProtocol

- (NSValue *)getCObject;

@optional
- (void)setCObject: (NSValue *)value;

@end
