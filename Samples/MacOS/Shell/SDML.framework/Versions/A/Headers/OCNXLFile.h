//
//  OCNXLFile.h
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 01/02/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ExposeCClassProtocol.h"
#import "RestfulProtocol.h"

@class OCWatermark;
@class OCExpiry;

@interface OCNXLFile : NSObject<ExposeCClassProtocol, RestfulProtocol>

- (instancetype)initWithValue: (NSValue *)value;
- (instancetype)initWithPath:(NSString *)localPath sourcePath: (NSString *)sourcePath;

// Get property.
- (NSString *)getName;
- (NSString *)getDUID;
- (NSString *)getPathID;
- (NSString *)getLocalPath;
- (NSString *)getLocalName;
- (NSUInteger)getNXLSize;
- (NSUInteger)getLastModify;
- (NSArray *)getRecipients;
- (NSArray *)getFileRights;
- (Boolean)isUploaded;
- (NSString *)getOwnerID;
- (OCWatermark *)getWatermark;
- (OCExpiry *)getExpiry;
- (NSString *)getSourceFilePath;

// Check methods.
- (NSError *)decyptFile: (NSString *)dest tokenKey: (NSString *)tokenKey;
- (Boolean)setRecipients: (NSArray *)recipients;
- (BOOL)isNXL;

// ExposeCClassProtocol
- (NSValue *)getCObject;
- (void)setCObject: (NSValue *)value;

// RestfulProtocol
- (NSError *)importRMString: (NSString *)str;
- (NSString *)exportRMString;
- (NSError *)importRMSResponse: (NSString *)response;


@end
