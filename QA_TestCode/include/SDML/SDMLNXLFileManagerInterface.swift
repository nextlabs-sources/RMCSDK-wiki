//
//  SDMLNotCacheFileServiceInterface.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 31/05/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

/// Manage not owner nxl file.
public protocol SDMLNXLFileManagerInterface {
    /// Decrypt file to folder.
    /// - parameter path: NXL file path.
    /// - parameter toFolder: The folder to store decrypt file.
    /// - Parameter isOverwrite: Overwrite file or not.
    /// - parameter completion: Decrypt path or error.
    func decryptFile(path: String, toFolder: String, isOverwrite: Bool, completion: @escaping (SDMLResult<String>) -> ())
    
    /// Share recipients.
    /// - Parameter path: NXL file path.
    /// - Parameter newRecipients: New recipients.
    /// - Parameter completion: Success or fail.
//    func reshare(path: String, newRecipients: [String], completion: @escaping (SDMLResult<Any?>) -> ())
    
    /// Check rights.
    /// - Parameter path: NXL file path.
    /// - Parameter completion: If success, return isOwner and file rights, tags.
    func getRight(path: String, completion: @escaping (SDMLResult<(isOwner: Bool, rights: SDMLRightObligation?, tags: [String: [String]]?)>) -> ())
    
    /// Get meta data.
    /// - Parameter path: NXL file path.
    /// - Parameter completion: Get file info.
//    func getMetadata(path: String, completion: @escaping (SDMLResult<SDMLNXLFileInterface>) -> ())
    
}
