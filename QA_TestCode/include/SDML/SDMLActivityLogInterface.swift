//
//  SDMLActivityLogInterface.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 21/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

/// Activity log infor.
public enum SDMLLogOperationType: CustomStringConvertible {
    case view
    case print
    case download
    case upload
    case protect
    case share
    
    public var description: String {
        switch self {
        case .view:
            return "View"
        case .print:
            return "Print"
        case .download:
            return "Download"
        case .upload:
            return "Upload"
        case .protect:
            return "Protect"
        case .share:
            return "Share"
        }
    }
}

public protocol SDMLActivityLogInterface {
    
    /// Get file duid.
    func getDUID() -> String

    /// Get file name.
    func getName() -> String

    /// Get operation.
    func getOperation() -> SDMLLogOperationType

    /// Allow or deny.
    func isAllowed() -> Bool

    /// Get access time.
    func getAccessTime() -> Date
    
    /// Get email.
    func getEmail() -> String

}
