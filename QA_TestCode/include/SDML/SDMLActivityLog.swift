//
//  SDMLActivityLog.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 30/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public class SDMLActivityLog {
    public init(email: String, operation: SDMLLogOperationType, isAllow: Bool,
                accessTime: Date, fileDuid: String, fileName: String) {
        self.email = email
        self.operation = operation
        self.isAllow = isAllow
        self.accessTime = accessTime
        self.fileDUID = fileDuid
        self.fileName = fileName
    }
    
    init() {}
    
   var email: String?
   var operation: SDMLLogOperationType?
   var deviceType: String?
   var isAllow: Bool?
   var accessTime: Date?
   var fileDUID: String?
   var fileName: String?
    
}

extension SDMLActivityLog: SDMLActivityLogInterface {
    /// Get file duid.
    public func getDUID() -> String {
        return fileDUID ?? ""
    }
    
    /// Get file name.
    public func getName() -> String {
        return fileName ?? ""
    }
    
    /// Get operation.
    public func getOperation() -> SDMLLogOperationType {
        return operation ?? .view
    }
    
    /// Allow or deny.
    public func isAllowed() -> Bool {
        return isAllow ?? true
    }
    
    /// Get access time.
    public func getAccessTime() -> Date {
        return accessTime ?? Date()
    }
 
    /// Get email.
    public func getEmail() -> String {
        return email ?? ""
    }
}
