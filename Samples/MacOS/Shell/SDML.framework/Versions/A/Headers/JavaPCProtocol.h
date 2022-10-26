//
//  JavaPCProtocol.h
//  JavaPC
//
//  Created by Wutao (Tao) Wu on 8/13/18.
//  Copyright © 2018 NextLabs. All rights reserved.
//

#import <Foundation/Foundation.h>

// The protocol that this service will vend as its API. This header file will also need to be visible to the process hosting the service.
@protocol JavaPCProtocol

// Replace the API of this protocol with an API appropriate to the service you are vending.
- (void)upperCaseString:(NSString *)aString withReply:(void (^)(NSString *))reply;
- (void)initialize:(void (^)(int))reply;
- (void)evaluation:(NSString *)userID withUserName:(NSString *)userName withFilePath:(NSString *)filePath withPQL:(NSString *)PQL withTags:(NSDictionary *)tags withReply:(void (^)(NSDictionary *))reply;
    
@end

/*
 To use the service from an application or other process, use NSXPCConnection to establish a connection to the service by doing something like this:

     _connectionToService = [[NSXPCConnection alloc] initWithServiceName:@"com.nxrmc.skyDRM.JavaPC"];
     _connectionToService.remoteObjectInterface = [NSXPCInterface interfaceWithProtocol:@protocol(JavaPCProtocol)];
     [_connectionToService resume];

Once you have a connection to the service, you can use it like this:

     [[_connectionToService remoteObjectProxy] upperCaseString:@"hello" withReply:^(NSString *aString) {
         // We have received a response. Update our text field, but do it on the main thread.
         NSLog(@"Result string was: %@", aString);
     }];

 And, when you are finished with the service, clean up the connection like this:

     [_connectionToService invalidate];
*/
