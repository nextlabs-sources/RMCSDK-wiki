//
//  OCWatermark.h
//  SDML
//
//  Created by Paul (Qian) Chen on 10/04/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ExposeCClassProtocol.h"

@interface OCWatermark : NSObject <ExposeCClassProtocol>

- (void)setWatermark: (NSString *)text fontName: (NSString *)fontName fontColor: (NSString *)fontColor
        fontSize: (NSInteger)fontSize transparency: (NSInteger)transparency rotation: (NSInteger)rotation
        isRepeat: (Boolean)isRepeat;

- (NSValue *)getCObject;
- (void)setCObject: (NSValue *)value;

- (NSString *)getText;
- (NSString *)getFontName;
- (NSInteger)getFontSize;
- (NSString *)getFontColor;
- (NSInteger)getTransparency;
- (NSInteger)getRotation;
- (NSString *)getRotationString;
- (BOOL)getRepeat;

@end
