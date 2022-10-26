//
//  SDMLProtectManager.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 05/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLProtectManagerInterface {
    
    
    /// Protect origin file to local path
    ///
    /// - Parameters:
    ///   - destPath: local path
    ///   - path: origin file path
    ///   - rights: e.g 'view'...
    ///   - tags: tags
    ///   - completion: full target path or error
    func protectOriginFileToLocal(destPath: String, path: String, rights: SDMLRightObligation?, tags: [String: [String]]?, completion: @escaping (SDMLResult<SDMLNXLFile>) -> ())
    
    
    /// Protect origin file
    ///
    /// - Parameters:
    ///   - type: myVault or project
    ///   - parentFile: if the type value is myVault, that value write nil; if the type value is project,that value write SDMLProject or SDMLProjectFile
    ///   - path: origin file path
    ///   - rights: e.g 'view'...
    ///   - tags: tags
    ///   - completion: type is myVault the file is SDMLNXLFile,otherwise the file is SDMLLocalProjectFile
    func protectOriginFile(type: ProtectType, parentFile: SDMLNXLFile?, path: String, rights: SDMLRightObligation?, tags: [String: [String]]?, completion: @escaping (SDMLResult<SDMLNXLFile>) -> ())
    
    /// Protect origin file.
    /// - parameter path: Origin file path.
    /// - parameter rights: e.g 'view'...
    /// - returns: Cache file struct or error.
    func protectOriginFile(path: String, rights: SDMLRightObligation, completion: @escaping (SDMLResult<SDMLNXLFile>) -> ())
    
    /**
     Share origin file to server.
     - parameters:
     - path: Origin file path.
     - recipients: e.g 'email: xxx'
     - rights: e.g 'view'...
     - watermark: Watermark.
     - expiredTime: When to expired.
     - comment: Comment word.
     - returns: Cache file struct or error.
     */
    func shareOriginFile(path: String, rights: SDMLRightObligation, recipients: [String], comment: String?, completion: @escaping (SDMLResult<SDMLNXLFile>) -> ())
    
    /** Protect file with tags.
     - parameters:
     - path: file path.
     - tags: tags.
     - completion: result return.
    */
    func protectOriginFile(path: String, tags: [String: [String]], completion: @escaping (SDMLResult<SDMLNXLFile>) -> ())
    
    /**
     - parameters:
     - path: file path.
     - tags: tags.
     - recipients: share users.
     - comment: share reason.
     - completion: result return.
     */
    func shareOriginFile(path: String, tags: [String: [String]], recipients: [String], comment: String?, completion: @escaping (SDMLResult<SDMLNXLFile>) -> ())
}
