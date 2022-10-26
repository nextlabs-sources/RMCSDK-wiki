//
//  Common.m
//  SDML
//
//  Created by Paul (Qian) Chen on 18/01/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import "CoreBridge.h"

@implementation CoreBridge

+ (NSURLRequest *)getURLRequest: (HTTPRequest)request{
    NSMutableURLRequest *urlRequest = [[NSMutableURLRequest alloc] init];
    // Set url
    NSString *url = [NSString stringWithUTF8String:request.GetURL().c_str()];
    urlRequest.URL = [NSURL URLWithString: url];
    // Set method
    NSString *method = [NSString stringWithUTF8String:request.GetMethod().c_str()];
    urlRequest.HTTPMethod = method;
    // Set header
    NSMutableDictionary *headDic = [[NSMutableDictionary alloc] init];
    http::headers headers = request.GetHeaders();
    for (int i = 0; i < headers.size(); i++) {
        NSString *key = [NSString stringWithUTF8String:headers[i].first.c_str()];
        NSString *value = [NSString stringWithUTF8String:headers[i].second.c_str()];
        [headDic setValue:value forKey:key];
    }
    urlRequest.allHTTPHeaderFields = headDic;
    // Set header: Content-Type
    NSMutableString *contentTypeStr = [[NSMutableString alloc] init];
    http::accept_types acceptTypes = request.GetAcceptTypes();
    for (int i = 0; i < acceptTypes.size(); i++) {
        NSString *acceptType = [NSString stringWithUTF8String:acceptTypes[i].c_str()];
        [contentTypeStr appendFormat:@"%@;", acceptType];
    }
    if (contentTypeStr.length > 0) {
        //NSString *value = @"application/x-www-form-urlencoded";
//        NSString *value = @"application/json";
        NSString *value = [contentTypeStr substringToIndex:contentTypeStr.length - 1];
        [urlRequest addValue:value forHTTPHeaderField:@"Content-Type"];
        
    }
    // Set header: Cookie
    NSMutableString *cookieStr = [[NSMutableString alloc] init];
    http::cookies cookies = request.GetCookies();
    for (int i = 0; i < cookies.size(); i++) {
        NSString *key = [NSString stringWithUTF8String:cookies[i].first.c_str()];
        NSString *value = [NSString stringWithUTF8String:cookies[i].second.c_str()];
        [cookieStr appendFormat:@"%@=%@;", key, value];
    }
    if(cookieStr.length > 0) {
        NSString *value = [cookieStr substringToIndex:cookieStr.length - 1];
        [urlRequest addValue:value forHTTPHeaderField:@"Cookie"];
    }
    // Set body
    NSData *body = [NSData dataWithBytes:&request.GetBody() length:request.GetBodyLength()];
    urlRequest.HTTPBody = body;
    
    return urlRequest;
}

+ (NSString *)getString: (std::string)cstring {
    return [NSString stringWithUTF8String:cstring.c_str()];
}

+ (std::string)getCString: (NSString *)str {
    return str.UTF8String;
}

@end
