//
//  SDMLBaseFile.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 15/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLBaseFileInterface {
    
    // Common property.
    
    /// Get file name.
    func getName() -> String
    
    /// Get file kind.
    func getKind() -> String
    
    /// Get file size.
    func getSize() -> Int
    
    /// Get created time.
    func getCreatedTime() -> Date
    
    /// Get last modified time.
    func getModifiedTime() -> Date
    
    // Cloud property.
    
    /// Get path id in server.
    func getPathID() -> String
    // Get the file protectionType
    func getProtectionType() -> Int
    
}

/// Abstract class.

public class SDMLBaseFile {
    
    // Common property.
    
         var name: String?
         var kind: String?
         var size: Int?
         var modified: Date?
         var created: Date?
    // the file protectionType
         var protectionType:Int?
    // Local property.
         var localPath: String?
    // Cloud property.
         var pathID: String?
//         var session: SDMLSession?
    /// Relationship, not use yet.
//    var parent: SDMLFolder?
    
    
}

extension SDMLBaseFile: SDMLBaseFileInterface {
    public func getProtectionType() -> Int {
        return protectionType ?? 0
    }
    
    
    public func getName() -> String {
        return name ?? ""
    }
    
    public func getKind() -> String {
        return kind ?? ""
    }
    
    public func getSize() -> Int {
        return size ?? 0
    }
    
    public func getPathID() -> String {
        return pathID ?? ""
    }
    
    public func getCreatedTime() -> Date {
        return created ?? Date()
    }
    
    public func getModifiedTime() -> Date {
        return modified ?? Date()
    }
    
    public func getLocalPath() -> String {
        return localPath ?? ""
    }
}
