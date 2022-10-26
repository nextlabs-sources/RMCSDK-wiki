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
@class OCNXLFile;
@class OCCertificate;
@class OCToken;
@class OCActivityLog;
@class OCLogPool;
@class OCRecipients;
@class OCHeartbeat;
@class OCMyDrive;
@class OCSystemPara;
@class OCWatermark;
@class OCExpiry;

@interface OCUser : NSObject <RestfulProtocol>

- (instancetype)initWithSystemPara: (OCSystemPara *)sysPara tenant: (OCTenant *)ocTenant;
- (instancetype)init;

- (NSURLRequest *)getLoginURLRequest;
- (NSURLRequest *)getLogoutURLRequest;
- (NSError *)logout: (NSString *)json;
- (NSString *)getUserId;
- (NSString *)getName;
- (NSString *)getEmail;
- (NSUInteger)getUserIdpType;
- (NSUInteger)getExpiredTime;
- (OCMembership *)getDefaultMembership;
- (BOOL)isLoginExpired;
- (OCSystemPara *)getSystemParameter;

- (NSError *)protectLocalFile: (NSString *)filePath destFilePath: (NSString *)destFilePath token: (OCToken *)token rights: (NSArray *)rights log: (OCActivityLog *)log file: (OCNXLFile *)file watermark: (OCWatermark *)watermark expiry: (OCExpiry *)expiry;

- (NSError *)shareLocalFile: (NSString *)filePath destFilePath: (NSString *)destFilePath token: (OCToken *)token rights: (NSArray *)rights log: (OCActivityLog *)log file: (OCNXLFile *)file watermark: (OCWatermark *)watermark expiry: (OCExpiry *)expiry recipients: (NSArray *)recipients comment: (NSString *)comment;

- (NSError *)protectLocalFile: (NSString *)filePath destFilePath: (NSString *)destFilePath token: (OCToken *)token tag: (NSString *)tags log: (OCActivityLog *)log file: (OCNXLFile *)file;

- (NSError *)shareLocalFile: (NSString *)filePath destFilePath: (NSString *)destFilePath token: (OCToken *)token tags: (NSString *)tags log: (OCActivityLog *)log recipients: (NSArray *)recipients comment: (NSString *)comment file: (OCNXLFile *)file ;

- (OCMyDrive *)getMyDrive;

- (NSError *)updateSystemParameters: (OCSystemPara *)sysPara;
- (NSError *)updateTenant: (OCTenant *)tenant;

// Helper for url request.

- (NSURLRequest *)getMembershipQuery: (OCMembership *)membership;
- (NSURLRequest *)getMembershipTokenQuery: (OCMembership *)membership tokenCount: (NSUInteger)count;
- (NSURLRequest *)getFileTokenQuery: (OCNXLFile *)file;

- (NSURLRequest *)getUploadShareFileQuery: (OCNXLFile *)nxlFile;
- (NSURLRequest *)getUploadProtectFileQuery: (OCNXLFile *)nxlFile;

- (NSURLRequest *)getUploadActivityLogQuery: (OCLogPool *)pool logNumb: (NSInteger)logNumb;
- (NSURLRequest *)getNXLFileActivityLogQuery: (OCNXLFile *)file;
- (NSURLRequest *)getUpdateRecipientQuery: (OCNXLFile *)file recipients: (OCRecipients *)recipients;
- (NSURLRequest *)getHeartbeatQuery: (OCHeartbeat *)heartbeat;
- (NSURLRequest *)getNXLFileMetadataQuery: (OCNXLFile *)file;

// MARK: RestfulProtocol

- (NSError *)importRMSResponse:(NSString *)response;
- (NSError *)importRMString: (NSString *)str;
- (NSString *)exportRMString;

@end
